# 🏆 Phân Tích Rủi Ro TechEmpower Benchmark — Vito Framework

> **Dựa trên khảo sát thực tế 2 codebase:**
> - `d:\HoangLong\Dev\lang\Vito-framework` — High-level framework (.vit API, Dockerfile, benchmark config)
> - `d:\HoangLong\Dev\lang\vit` — VIT Compiler & C Runtime (io_uring, SIMD parser, slab allocator, net_rt, NativeCompiler)
>
> Ngày cập nhật: 2026-08-03 (v2 — bổ sung phân tích VIT runtime)

---

## 🔴 Rủi Ro Nghiêm Trọng (CRITICAL — Sẽ bị FAILED ngay)

### RI-01 · `listen()` chỉ bind `127.0.0.1`, không phải `0.0.0.0`

**File:** [`app.vit` L661](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/app.vit#L661-L661)

```vit
// ❌ Hiện tại
let listener = listenTcp("127.0.0.1", port);
```

**Hậu quả:** Container TFB chạy trên mạng bridge của Docker/Podman. TFB client gọi vào IP của container, không phải localhost. Nếu bind `127.0.0.1`, **server sẽ không nhận được một request nào từ TFB**. Test sẽ bị `Connection refused`.

**Khắc phục ngay:**
```vit
// ✅ Phải đổi thành
let listener = listenTcp("0.0.0.0", port);
```

---

### RI-02 · Single-threaded Accept Loop — Không multi-core

**File:** [`app.vit` L649–L689](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/app.vit#L649-L689)

```vit
function listen(port: number): void {
    let listener = listenTcp("0.0.0.0", port);
    while (true) {
        let stream = listener.accept();  // ← Blocking, single goroutine/thread
        // xử lý đồng bộ từng request
    }
}
```

Toàn bộ `listen()` là một vòng lặp đơn luồng, blocking, xử lý **từng kết nối một**. TFB test với concurrency 16,384 connections.

**Hậu quả:**
- RPS thực tế có thể chỉ đạt **1–5% tiềm năng** của phần cứng Dell PowerEdge 28-core.
- Plaintext test sẽ timeout vì không drain connections đủ nhanh.
- TFB ghi nhận `socket errors` và đánh `FAILED`.

**Khắc phục:** Cần implement worker pool với `SO_REUSEPORT`. Mỗi worker chạy một event loop riêng, dùng `pthread` hoặc `fork()` × N_CORES. Tham khảo `main.c` đã có sẵn `vit_iouring_group_create()` — nhưng `.vit` high-level API chưa expose tính năng này.

---

### RI-03 · `stream.close()` sau mỗi response — Phá vỡ Keep-Alive & Pipelining

**File:** [`response.vit` L77, L92, L107](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/response.vit#L66-L108)

```vit
function send(text: string): void {
    // ...
    this.stream.write(headerStr + text);
    this.stream.close();  // ❌ Đóng connection sau mỗi response!
    this.isClosed = 1.0;
},
```

**Hậu quả:**
- HTTP Keep-Alive bị phá vỡ hoàn toàn. TFB dùng persistent connections.
- Plaintext pipelining test sẽ `Connection reset by peer` liên tục.
- Tỷ lệ lỗi > 0% → **FAILED** ngay lập tức.

**Khắc phục:** Chỉ gọi `stream.close()` khi nhận `Connection: close` header, hoặc sau khi hết timeout. Logic keep-alive đã có sơ bộ trong `listen()` (L680–L681) nhưng bị vô hiệu hóa vì `response.vit` đóng stream sớm.

---

### RI-04 · `Content-Type: application/json; charset=utf-8` — Sai chuẩn TFB

**File:** [`response.vit` L102](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/response.vit#L101-L103)

```vit
"Content-Type: application/json; charset=utf-8\r\n"
```

TFB verifier kiểm tra **chính xác** `application/json` (không có charset suffix). Nhiều framework bị reject vì header không khớp regex của verifier.

> [!WARNING]
> Trên thực tế TFB hiện tại **chấp nhận cả hai**, nhưng để an toàn nên dùng `application/json` cho `/json` route và `text/plain` cho `/plaintext` route — **không thêm charset**.

---

### RI-05 · Thiếu `Server` và `Date` header trong response của `.vit` high-level API

**File:** [`response.vit`](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/response.vit) — Không có `Server:` và `Date:` header nào.

TFB verifier **bắt buộc** có:
- `Server: <bất kỳ giá trị nào>`
- `Date: <HTTP-date theo RFC 1123>` — phải cập nhật mỗi giây

**Hậu quả:** `Verification Failed: Missing required headers`.

Lưu ý: `main.c` đã xử lý đúng với `g_date_header` + background thread cập nhật mỗi giây — nhưng `main.vit` (high-level API) hoàn toàn bỏ qua.

---

## 🟠 Rủi Ro Cao (HIGH — Làm giảm điểm nghiêm trọng)

### RI-06 · `-march=native` trong Dockerfile — Illegal Instruction trên server TFB

**File:** [`Dockerfile` L22](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/Dockerfile#L22-L25)

```dockerfile
# ❌ Rủi ro cao
RUN gcc -O3 -march=native -flto -ffast-math -DNDEBUG \
```

CI build của TFB chạy trên GitHub Actions Runner (Intel Xeon gen X). Runtime là Dell PowerEdge với CPU khác (có thể thiếu AVX-512, hoặc ngược lại có AVX-512 nhưng binary được compile cho CPU cũ hơn).

**Hậu quả:** `SIGILL (Illegal Instruction)` → Container crash ngay khi start.

**Khắc phục:**
```dockerfile
# ✅ An toàn hơn: target cụ thể
RUN gcc -O3 -march=x86-64-v3 -flto -ffast-math -DNDEBUG \
# x86-64-v3 = AVX2 + FMA, phù hợp với hầu hết Intel Haswell+ server
```

---

### RI-07 · `num_workers = 4` hardcode trong `main.c` (fallback khi `sysconf` fail)

**File:** [`main.c` L193](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L192-L196)

```c
int num_workers = 4;  // ❌ Fallback hardcode
#ifndef _WIN32
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs > 0) num_workers = (int)nprocs;
#endif
```

Trong Docker container với cgroup CPU quota bị giới hạn (ví dụ `--cpus=8`), `sysconf(_SC_NPROCESSORS_ONLN)` trả về số CPU của **host** (28), không phải quota được cấp. Nếu spawn 28 workers nhưng chỉ có 8 CPU quota, context switching sẽ giết performance.

**Khắc phục:** Đọc từ cgroup v2: `/sys/fs/cgroup/cpu.max` và tính quota CPU thực tế:
```c
// Ưu tiên đọc cgroup quota
int quota_cores = get_cgroup_cpu_quota(); // implement riêng
if (quota_cores > 0 && quota_cores < nprocs)
    num_workers = quota_cores;
```

---

### RI-08 · HTTP Pipelining parser thiếu hỗ trợ partial read

**File:** [`main.c` L93–L181](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L93-L181)

```c
static void techempower_handler(int client_fd, const char* req, size_t len) {
    size_t offset = 0;
    while (offset < len) {
        if (memcmp(req + offset, "GET /json ", 10) == 0 ...) {
            // ...
            offset += 10;  // ❌ Chỉ skip đúng 10 bytes của path, không skip toàn bộ HTTP request
        }
        // Advance to next request
        const char* next_req = strstr(req + offset, "GET ");  // ❌ Naive search
```

**Vấn đề:**
1. Khi parse pipelined requests, code chỉ advance `offset` đúng số bytes của path (`10` cho `/json`), không advance qua toàn bộ HTTP/1.1 request bao gồm headers và `\r\n\r\n`.
2. `strstr(req + offset, "GET ")` sẽ match `GET ` trong HTTP header values (ví dụ `Referer: http://example.com/GET blah`).
3. Nếu TCP packet bị fragment (partial read), handler nhận incomplete request và parse sai → response lỗi.

**Hậu quả:** Bad responses > 0% → FAILED.

---

### RI-09 · `benchmark_config.json` — Khai báo test không tồn tại

**File:** [`benchmark_config.json`](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/benchmark_config.json)

```json
{
  "query_url": "/queries?queries=",
  "update_url": "/updates?queries=",
}
```

`main.c` (binary thực tế chạy) **không implement** `/queries` và `/updates` routes. Chỉ có `/json`, `/plaintext`, `/db`, `/fortunes`.

**Hậu quả:** TFB verify `/queries` → 404 → `Verification Failed`.

**Khắc phục:** Hoặc implement đầy đủ, hoặc xóa `query_url` và `update_url` khỏi config cho đến khi implement xong.

---

### RI-10 · `/db` route dùng `rand()` giả thay vì PostgreSQL thật

**File:** [`main.c` L129–L143](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L129-L143)

```c
else if (memcmp(req + offset, "GET /db", 7) == 0) {
    int id = (rand() % 10000) + 1;
    int randomNumber = (rand() % 10000) + 1;
    // ❌ Không query PostgreSQL — fabricate data
```

TFB yêu cầu `/db` phải **thực sự query PostgreSQL** với table `World(id, randomNumber)`.

**Hậu quả:** Điểm `/db` sẽ bị loại do "fabricated data" — vi phạm TFB integrity rules.

---

## 🟡 Rủi Ro Trung Bình (MEDIUM — Làm giảm điểm benchmark)

### RI-11 · `Content-Length` tính theo `string.length` (không phải byte length)

**File:** [`response.vit` L70, L85, L100](file:///d:/HoangLong/Dev/lang/Vito-framework/src/core/response.vit)

```vit
let contentLen = text.length;  // ← string.length (character count)
```

Nếu VIT runtime tính `string.length` = UTF-16 character count (giống JS), thì các chuỗi UTF-8 multi-byte sẽ bị tính sai `Content-Length`. TFB verifier so sánh Content-Length với byte thực tế.

**Hậu quả:** Fortunes test có Unicode characters → `Content-Length mismatch` → FAILED.

---

### RI-12 · Thiếu `ulimit` / `rlimit` setup cho io_uring trong container

**File:** [`Dockerfile`](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/Dockerfile) — Không có `ulimit` config nào.

Default `nofile` trong container = 1024. TFB test với 16,384 connections đồng thời.

**Khắc phục thêm vào Dockerfile:**
```dockerfile
# Hoặc set trong entrypoint script
RUN echo "* soft nofile 1048576\n* hard nofile 1048576" >> /etc/security/limits.conf
```

Hoặc trong `main.c` startup:
```c
struct rlimit rl = {.rlim_cur = 1048576, .rlim_max = 1048576};
setrlimit(RLIMIT_NOFILE, &rl);
```

---

### RI-13 · `MEMLOCK` limit cho io_uring SQPOLL mode

`io_uring` với SQPOLL cần pin memory (mlock) cho SQ/CQ ring buffers. Container mặc định có `RLIMIT_MEMLOCK = 64KB`. io_uring với queue depth 4096+ cần ít nhất vài MB.

**Khắc phục:**
```c
// Trong main.c trước khi tạo io_uring instance
struct rlimit rl = {.rlim_cur = RLIM_INFINITY, .rlim_max = RLIM_INFINITY};
setrlimit(RLIMIT_MEMLOCK, &rl);
```

Hoặc thêm vào Dockerfile:
```dockerfile
# Cần --privileged hoặc --cap-add IPC_LOCK khi run container
```

---

### RI-14 · `response buffer` 4096 bytes — Overflow với Fortunes HTML lớn

**File:** [`main.c` L94](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L94)

```c
char response[4096];  // ← Stack buffer cố định
```

Fortunes test với 12 rows từ DB + sort + HTML escape có thể > 4KB → `snprintf` truncate → response bị cụt → FAILED.

**Khắc phục:** Dùng heap-allocated buffer hoặc tăng lên 65536 bytes, hoặc dùng slab allocator đã có sẵn.

---

### RI-15 · Date header update dùng `sleep(1)` — Không atomic, race condition

**File:** [`main.c` L48–L55](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L48-L55)

```c
static char g_date_header[64] = "Date: Sun, 02 Aug 2026 00:00:00 GMT\r\n";

static void* date_timer_thread(void* arg) {
    while (g_running) {
        update_date_header();  // ← Write vào g_date_header
        sleep(1);
    }
}
```

`g_date_header` được write bởi timer thread và read bởi tất cả worker threads **không có mutex/atomic**. Trên multi-core, một worker thread có thể đọc partial-write string dở dang.

**Khắc phục:** Dùng double-buffering + `atomic_store`/`atomic_load` với `_Atomic char*`, hoặc đơn giản hơn là `pthread_mutex`.

---

## 🔵 Rủi Ro Phát Hiện Thêm (ADDITIONAL)

### RI-16 · `benchmark_config.json` — `"approach": "Realistic"` không đúng classification

```json
"approach": "Realistic",
"classification": "Fullstack"
```

TFB chỉ nhận `approach` là `"Realistic"` hoặc `"Stripped"`. `"classification"` field **không tồn tại** trong TFB schema mới. Field hợp lệ là `"orm": "Raw"`, `"webserver": "..."`.

**Rủi ro:** Config bị reject bởi TFB tooling parser → build fail.

---

### RI-17 · Không có `docker-compose.yml` cho Postgres service

TFB yêu cầu framework có thể kết nối với Postgres trên `tfb-database` hostname (TFB tự cung cấp DB container). Không cần `docker-compose.yml` riêng, nhưng **connection string phải đúng**:

```
host=tfb-database port=5432 user=benchmarkdbuser password=benchmarkdbpass dbname=hello_world
```

Hiện tại `main.c` không kết nối DB thật nào.

---

### RI-18 · Không có graceful shutdown signal handler

```c
static volatile bool g_running = true;
// Không có SIGTERM/SIGINT handler
```

TFB container dùng `docker stop` (gửi `SIGTERM`). Nếu không có handler, container bị killed ngay lập tức, có thể gây incomplete responses trong flight → error rate tăng.

**Khắc phục:**
```c
signal(SIGTERM, handle_shutdown);
signal(SIGINT, handle_shutdown);

void handle_shutdown(int sig) {
    g_running = false;
}
```

---

### RI-19 · `rand()` không thread-safe khi multi-worker

**File:** [`main.c` L130–L131](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L130-L131)

```c
int id = (rand() % 10000) + 1;
int randomNumber = (rand() % 10000) + 1;
```

`rand()` dùng global state, **không thread-safe**. Khi 28 workers đồng thời gọi `rand()`, có race condition và undefined behavior.

**Khắc phục:** Dùng `rand_r(&seed)` với seed per-thread, hoặc `random_r()`.

---

### RI-20 · Thiếu TCP_NODELAY — Nagle's Algorithm làm trễ responses nhỏ

Responses nhỏ (`Hello, World!` = 13 bytes) sẽ bị giữ lại bởi Nagle's Algorithm lên đến 200ms trước khi gửi nếu không set `TCP_NODELAY`.

**Khắc phục:**
```c
int flag = 1;
setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
```

---

### RI-21 · Không có SO_REUSEADDR — Port reuse khi restart container

Nếu container restart nhanh, `TIME_WAIT` sockets có thể chiếm port 8080 → `bind() failed: Address already in use`.

**Khắc phục:**
```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
```

---

### RI-22 · `main.c` include các header runtime không tồn tại

**File:** [`main.c` L25–L28](file:///d:/HoangLong/Dev/lang/Vito-framework/benchmarks/techempower/main.c#L25-L28)

```c
#include "async_iouring_rt.h"
#include "slab_allocator_rt.h"
#include "simd_json_rt.h"
#include "http_parser_simd.h"
```

Những file này **không tồn tại** trong repo (chỉ là placeholder/stub). `gcc` build sẽ thất bại với `No such file or directory`.

**Hậu quả:** Docker build thất bại → không có binary để chạy.

---

## 📋 Check-list Ưu Tiên Khắc Phục

| # | Rủi Ro | Mức Độ | Tác Động |
|---|--------|--------|----------|
| RI-01 | `127.0.0.1` → `0.0.0.0` | 🔴 CRITICAL | 0 RPS — server unreachable |
| RI-03 | `stream.close()` sau mỗi response | 🔴 CRITICAL | FAILED — pipelining broken |
| RI-05 | Thiếu `Server` + `Date` header | 🔴 CRITICAL | Verification Failed |
| RI-09 | Config khai báo routes không implement | 🔴 CRITICAL | Verification Failed |
| RI-22 | Include headers không tồn tại | 🔴 CRITICAL | Docker build fail |
| RI-02 | Single-threaded accept loop | 🟠 HIGH | RPS giảm 10–20× |
| RI-06 | `-march=native` → SIGILL | 🟠 HIGH | Container crash |
| RI-08 | HTTP Pipelining parser sai | 🟠 HIGH | Bad responses > 0% |
| RI-10 | `/db` fake data | 🟠 HIGH | DQ'd for integrity |
| RI-11 | Content-Length sai UTF-8 | 🟡 MEDIUM | Fortunes FAILED |
| RI-12 | ulimit nofile thấp | 🟡 MEDIUM | Socket errors ở 16K conns |
| RI-13 | MEMLOCK cho io_uring | 🟡 MEDIUM | io_uring init fail |
| RI-14 | Buffer 4096 quá nhỏ | 🟡 MEDIUM | Response truncated |
| RI-15 | Date header race condition | 🟡 MEDIUM | Corrupted Date header |
| RI-16 | Config schema sai | 🟡 MEDIUM | Build tool reject |
| RI-17 | Thiếu Postgres connection | 🟡 MEDIUM | DB tests skip |
| RI-18 | Thiếu SIGTERM handler | 🔵 LOW | Error spike on stop |
| RI-19 | `rand()` không thread-safe | 🔵 LOW | UB trên multi-core |
| RI-20 | Thiếu TCP_NODELAY | 🔵 LOW | Latency tăng |
| RI-21 | Thiếu SO_REUSEADDR | 🔵 LOW | Restart fail |
| RI-07 | Worker count vs cgroup quota | 🔵 LOW | Performance noise |

---

## 🚀 Lộ Trình Đề Xuất — 3 Giai Đoạn

### Giai Đoạn 1 — Fix "Không bị FAILED" (1–2 ngày)
1. Fix `listen()` bind `0.0.0.0`
2. Tạo các runtime header files thực sự (hoặc dùng pure POSIX sockets thay cho placeholder)
3. Thêm `Server: Vito` và `Date:` header vào tất cả responses
4. Fix `stream.close()` — chỉ close khi `Connection: close`
5. Xóa `query_url` và `update_url` khỏi config nếu chưa implement
6. Đổi `-march=native` → `-march=x86-64-v3`

### Giai Đoạn 2 — Multi-core & Stability (1 tuần)
1. Implement multi-process/multi-thread worker với `SO_REUSEPORT`
2. Fix HTTP pipelining parser (advance đúng qua `\r\n\r\n`)
3. Setup `setrlimit(RLIMIT_NOFILE)` và `RLIMIT_MEMLOCK`
4. Fix Date header race condition với mutex
5. Thêm `TCP_NODELAY`, `SO_REUSEADDR`, SIGTERM handler

### Giai Đoạn 3 — DB Integration & Full Compliance (2–3 tuần)
1. Implement Postgres connection với `libpq` (native protocol)
2. Implement `/db`, `/queries`, `/updates` với pool connection thực sự
3. Fix Fortunes HTML buffer size
4. Verify với `./tfb --mode verify --test vito` local
5. Stress test với `wrk -t12 -c16384 -d30s`
6. Submit PR lên TechEmpower/FrameworkBenchmarks

---

*Phân tích Vito-framework dựa trên review trực tiếp source code. Một số rủi ro (RI-22) là showstopper build-time, cần ưu tiên giải quyết trước tất cả.*

---

# 🔬 Phân Tích Chuyên Sâu — VIT Compiler & Runtime (`d:\HoangLong\Dev\lang\vit`)

> Sau khi review toàn bộ VIT compiler + C runtime, phát hiện thêm **12 rủi ro bổ sung** ảnh hưởng trực tiếp đến TFB submission.

---

## 🟠 Phát Hiện Quan Trọng: `async_iouring_rt.c` Dùng epoll, KHÔNG Phải io_uring Thật

### VRI-01 · Tên gọi misleading: "io_uring" nhưng thực tế là `epoll`

**File:** [`async_iouring_rt.c` L87–L143](file:///d:/HoangLong/Dev/lang/vit/src/runtime/async_iouring_rt.c#L87-L143)

```c
// worker_thread_loop trên Linux
int epoll_fd = epoll_create1(0);       // ← epoll, không phải io_uring!
ev.events = EPOLLIN | EPOLLET;
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ...);
nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 50);
```

Mặc dù file tên là `async_iouring_rt.c`, **worker event loop thực tế dùng `epoll`**. io_uring chỉ được dùng như "ring descriptor" (`ring_fd`) nhưng không có SQE/CQE submission nào trong event loop chính.

**Hậu quả:**
- Benchmark config ghi `"notes": "...io_uring SQPOLL..."` — sẽ sai với implementation thực tế → có thể bị reject bởi TFB reviewer vì misleading claim.
- `IORING_SETUP_SQPOLL` được request khi `vit_iouring_init()` nhưng nếu thành công, `ring_fd` không bao giờ được dùng trong worker loop → memory/FD leak.
- Đây là **thiết kế tốt về mặt performance** (epoll là sản xuất-grade), nhưng tài liệu phải phản ánh đúng thực tế.

**Khắc phục:** Cập nhật `benchmark_config.json` → `"notes": "...epoll + SO_REUSEPORT multi-worker..."`. Không cần thay đổi code, chỉ cần honest documentation.

---

### VRI-02 · io_uring `vit_iouring_init()` dùng struct không đúng với kernel API

**File:** [`async_iouring_rt.c` L198–L231](file:///d:/HoangLong/Dev/lang/vit/src/runtime/async_iouring_rt.c#L198-L231)

```c
struct {
    uint32_t sq_entries;
    uint32_t cq_entries;
    uint32_t flags;
    uint32_t sq_thread_cpu;
    uint32_t sq_thread_idle;
    uint32_t features;
    uint32_t wq_fd;
    uint32_t resv[3];
    uint32_t sq_off[10];    // ❌ sai — io_uring_params.sq_off là struct io_sqring_offsets
    uint32_t cq_off[10];    // ❌ sai — io_uring_params.cq_off là struct io_cqring_offsets
} p;
```

Kernel `io_uring_params` struct có `sq_off` và `cq_off` là **nested struct** (`struct io_sqring_offsets` = 40 bytes), không phải `uint32_t[10]` = 40 bytes (may coincidentally equal but semantically wrong). Khi kernel Linux cũ hơn 5.4 hoặc có padding khác, struct size mismatch sẽ gây `EFAULT` trên `io_uring_setup` syscall.

**Hậu quả:** `sys_io_uring_setup` fail → fallback to standard mode → `SQPOLL` claim trong docs sai → io_uring benefit bị mất hoàn toàn.

**Khắc phục:** Dùng `liburing` (`#include <liburing.h>`) thay vì raw syscall, hoặc copy đúng struct từ `linux/io_uring.h`.

---

### VRI-03 · Worker thread join bị bỏ qua — Thread handle leak

**File:** [`async_iouring_rt.c` L319–L327](file:///d:/HoangLong/Dev/lang/vit/src/runtime/async_iouring_rt.c#L319-L327)

```c
#else
pthread_t thread;
pthread_create(&thread, NULL, worker_thread_loop, warg);
group->workers[i].thread_handle = (void*)thread;  // ← lưu thread handle
// ❌ nhưng vit_iouring_group_stop() KHÔNG join thread nào!
```

**File:** [`async_iouring_rt.c` L329–L343](file:///d:/HoangLong/Dev/lang/vit/src/runtime/async_iouring_rt.c#L329-L343)

```c
void vit_iouring_group_stop(vit_iouring_worker_group_t* group) {
    for (int i = 0; i < group->num_workers; i++) {
        group->workers[i].running = 0;
        // ❌ Không có pthread_join() — worker thread chưa dừng hẳn khi free memory
        vit_iouring_cleanup(&group->workers[i].ring);
    }
    free(group->workers);  // ← UAF nếu worker thread vẫn đang chạy!
    free(group);
}
```

**Hậu quả:** Khi TFB container dừng (SIGTERM → `vit_iouring_group_stop`), worker threads vẫn đang chạy và truy cập vào bộ nhớ đã được `free()` → **Use-After-Free**, undefined behavior, possible SIGSEGV.

---

### VRI-04 · `slab_allocator_rt.c` — `vit_slab_alloc/free` không thread-safe

**File:** [`slab_allocator_rt.c` L47–L64](file:///d:/HoangLong/Dev/lang/vit/src/runtime/slab_allocator_rt.c#L47-L64)

```c
vit_connection_slab_t* vit_slab_alloc(vit_slab_pool_t* pool) {
    // ❌ Không có mutex/spinlock
    uint32_t index = pool->free_indices[--pool->free_head];  // race condition trên --free_head
    pool->allocated_count++;                                  // race condition
    ...
}

void vit_slab_free(vit_slab_pool_t* pool, vit_connection_slab_t* slab) {
    pool->free_indices[pool->free_head++] = (uint32_t)diff;  // race condition trên free_head++
    if (pool->allocated_count > 0) pool->allocated_count--;  // race condition
}
```

Cả `vit_slab_alloc` và `vit_slab_free` đều modify `pool->free_head` mà không có synchronization. Với 28 worker threads đồng thời, đây là **critical section** không được bảo vệ → double-alloc, double-free, corruption slab pool.

**Hậu quả:** Crash ngẫu nhiên dưới tải cao của TFB. Khó reproduce vì chỉ xảy ra ở concurrency cao.

**Khắc phục:** Thêm `_Atomic uint32_t free_head` + `atomic_fetch_sub`/`atomic_fetch_add`, hoặc dùng `pthread_mutex_t` trên pool.

---

### VRI-05 · `net_rt.c` — `g_fast_recv_buf` là thread-local nhưng `_Thread_local` không khả dụng trên mọi C compiler

**File:** [`net_rt.c` L52](file:///d:/HoangLong/Dev/lang/vit/src/runtime/net_rt.c#L52)

```c
static _Thread_local char g_fast_recv_buf[FAST_BUFFER_SIZE + 1];  // ← C11
```

`_Thread_local` là C11. Dockerfile.benchmark dùng `gcc` mà không chỉ định `-std=c11`. Nếu gcc default là C99/C89, `_Thread_local` sẽ không compile.

**Dockerfile benchmark:**
```dockerfile
# ❌ Không có -std=c11
RUN gcc -O3 -march=native -flto -Iinclude -Isrc \
    ... -pthread -o benchmark_server
```

**Khắc phục:** Thêm `-std=c11` hoặc dùng `__thread` (GCC extension, tương thích rộng hơn).

---

### VRI-06 · `http_parser_simd.c` — Parser chỉ parse **một request**, không hỗ trợ pipelining batch

**File:** [`http_parser_simd.c` L66–L131](file:///d:/HoangLong/Dev/lang/vit/src/runtime/http_parser_simd.c#L66-L131)

```c
int vit_http_parse_simd(const char* buf, size_t len, vit_http_request_span_t* req) {
    // Parse method, path, version, headers, body...
    // ❌ Chỉ parse DUY NHẤT 1 request từ buffer
    // Không có loop, không có "parse next request" pointer
    return 0;
}
```

Khi TFB gửi pipelined requests (nhiều `GET /plaintext HTTP/1.1\r\n...` trong 1 TCP packet), parser này chỉ xử lý request đầu tiên và bỏ qua phần còn lại của buffer.

**Hậu quả:** Pipelining throughput bị drop xuống single-request throughput, error rate tăng (unhandled data in buffer).

**Lưu ý:** `main.c` có logic pipelining riêng (memcmp loop) nhưng không dùng `vit_http_parse_simd`. Hai path xử lý khác nhau này tạo ra inconsistency khó debug.

---

### VRI-07 · `NativeCompiler.cpp` — Compile VIT code với `-march=native` trên developer machine

**File:** [`NativeCompiler.cpp` L213–L215](file:///d:/HoangLong/Dev/lang/vit/src/codegen/NativeCompiler.cpp#L213-L215)

```cpp
if (options.marchNative) {
    extraOptFlags += "-march=native -mtune=native ";
}
```

Khi developer chạy `vit build` để compile `main.vit` trên máy local (Windows với Intel i7 Gen 12), binary output sẽ có AVX-512 instructions. Khi copy binary này vào Docker image và chạy trên TFB server có CPU khác → `SIGILL`.

**Đây là rủi ro RI-06 nhưng ở layer VIT compiler**, không chỉ ở Dockerfile. Developer có thể vô tình build binary với native flags mà không biết.

**Khắc phục:** Trong Dockerfile, luôn compile lại từ source với flags cố định, không copy binary từ host.

---

### VRI-08 · `std/net.vit` — `listenTcp()` dùng backlog=128, quá thấp cho TFB

**File:** [`net.vit` L70](file:///d:/HoangLong/Dev/lang/vit/std/net.vit#L70)

```vit
function listenTcp(host: string, port: number): TcpListener {
    let fd = vit_net_socket_create();
    vit_net_socket_bind(fd, host, port);
    vit_net_socket_listen(fd, 128);  // ❌ backlog = 128
```

TFB test với 16,384 concurrent connections. TCP listen backlog = 128 nghĩa là kernel chỉ queue tối đa 128 connections chờ `accept()`. Các connections còn lại sẽ nhận `Connection refused` hoặc bị reset.

Lưu ý: `net_rt.c` có `backlog = 4096` trong `vit_net_socket_listen()` khi được gọi từ `main.c`. Nhưng khi `.vit` code gọi `listenTcp()` → backlog là 128.

**Khắc phục:** Đổi backlog trong `net.vit` lên ít nhất 4096 hoặc 65535.

---

### VRI-09 · `string_rt.c` — `strlen()` được expose như `string.length` → Byte length, không phải char count

**File:** [`string.vit` L13–L15](file:///d:/HoangLong/Dev/lang/vit/std/string.vit#L13-L15)

```vit
extern function strlen(str: string): number;

function length(str: string): number {
    return strlen(str);  // ← strlen = byte count (C semantics)
}
```

VIT gọi C `strlen()` cho `string.length`. `strlen()` đếm **bytes**, không phải Unicode codepoints. Khi Fortunes HTML chứa ký tự Unicode (ví dụ: tên `ϗ` trong fortune table), `strlen()` trả về byte count đúng cho UTF-8, nhưng nếu VIT runtime intern strings as UTF-16 (như nhiều language runtimes), đây sẽ là wrong byte count.

Cần xác minh: VIT strings là UTF-8 hay UTF-16 internally? Nếu UTF-8, `strlen` là đúng cho `Content-Length`. Nếu UTF-16/wchar_t, sẽ sai.

**Khắc phục:** Document rõ VIT string encoding; nếu UTF-8, đây không phải issue. Nhưng cần kiểm tra thực tế với Fortunes data.

---

### VRI-10 · `simd_json_rt.c` — `vit_simd_json_parse()` allocate `malloc` mỗi parse

**File:** [`simd_json_rt.c` L66–L108](file:///d:/HoangLong/Dev/lang/vit/src/runtime/simd_json_rt.c#L66-L108)

```c
vit_simd_json_doc_t* vit_simd_json_parse(const char* json_str, size_t length) {
    vit_simd_json_doc_t* doc = malloc(sizeof(vit_simd_json_doc_t));  // ← malloc
    doc->tokens = malloc(doc->token_capacity * sizeof(vit_json_token_t));  // ← malloc
    uint32_t* struct_indexes = malloc(length * sizeof(uint32_t));  // ← malloc (length có thể lớn!)
    // ...
    free(struct_indexes);
    return doc;  // ← caller phải gọi vit_simd_json_free()
}
```

Mỗi JSON parse = 3 lần `malloc`. Ở throughput cao (300K+ RPS), đây là **bottleneck nghiêm trọng** — malloc contention trên global allocator heap giữa 28 threads.

Ngoài ra `struct_indexes = malloc(length * sizeof(uint32_t))` với `length` có thể rất lớn — allocate `length` uint32_t entries dù chỉ cần một phần nhỏ.

**Khắc phục:** Dùng `vit_thread_arena_alloc()` (đã có sẵn trong `slab_allocator_rt.c`) thay cho `malloc` trong JSON parse path.

---

### VRI-11 · `Dockerfile.benchmark` trong VIT repo — Dùng `ubuntu:24.04` nhưng TFB dùng `debian:bookworm`

**File:** [`Dockerfile.benchmark` L1](file:///d:/HoangLong/Dev/lang/vit/Dockerfile.benchmark#L1)

```dockerfile
FROM ubuntu:24.04
RUN gcc -O3 -march=native -flto ...
```

TFB environment chạy trên **Debian Bookworm** (kiểm chứng qua `Vito-framework/benchmarks/techempower/Dockerfile` dùng `debian:bookworm-slim`). Ubuntu 24.04 và Debian Bookworm có thể có glibc version khác nhau. Binary compiled trên Ubuntu 24.04 có thể không chạy được trên Debian vì `GLIBC_2.38` vs `GLIBC_2.36` ABI differences.

**Khắc phục:** Sử dụng duy nhất `debian:bookworm-slim` cho tất cả Docker builds liên quan đến TFB.

---

### VRI-12 · `NativeCompiler.cpp` — `concurrency_rt.c` là Windows-only implementation

**File:** [`concurrency_rt.c` L1–L50](file:///d:/HoangLong/Dev/lang/vit/src/runtime/concurrency_rt.c#L1-L50)

```c
// Forward C runtime and Win32 declarations
typedef void* HANDLE;
typedef unsigned long DWORD;
// __declspec(dllimport) CreateThread, WaitForSingleObject...
// ← Hoàn toàn là Windows WINAPI, không có #ifdef _WIN32 guard!
```

`concurrency_rt.c` dùng Win32 API trực tiếp (`CreateThread`, `CRITICAL_SECTION`, v.v.) mà **không có POSIX fallback**. Khi TFB build trong Docker Linux, file này sẽ compile fail với `undefined reference to __declspec(dllimport) CreateThread`.

NativeCompiler.cpp tự động link `concurrency_rt.c` vào mọi build:
```cpp
addRtCandidate(rtPath, exeDir, "concurrency_rt.c");  // ← luôn được thêm vào
```

**Hậu quả:** Mọi VIT-compiled binary (kể cả `main.vit`) khi build trên Linux Docker sẽ **link fail** vì `concurrency_rt.c`.

**Khắc phục:** Tách thành `concurrency_rt_win32.c` và `concurrency_rt_posix.c`, chọn đúng file theo platform trong `NativeCompiler.cpp`.

---

## 📊 Bảng Tổng Hợp — VIT Runtime Risks

| # | Rủi Ro | Mức Độ | Tác Động TFB |
|---|--------|--------|--------------|
| VRI-01 | epoll vs io_uring — misleading docs | 🟡 MEDIUM | PR review reject |
| VRI-02 | io_uring struct definition sai kernel ABI | 🟡 MEDIUM | SQPOLL fail silently |
| VRI-03 | Worker thread join bị bỏ qua → UAF | 🟠 HIGH | SIGSEGV on shutdown |
| VRI-04 | Slab allocator không thread-safe | 🟠 HIGH | Crash at high concurrency |
| VRI-05 | `_Thread_local` cần `-std=c11` flag | 🟡 MEDIUM | Compile fail trên strict C99 |
| VRI-06 | HTTP parser chỉ xử lý 1 request/buffer | 🟠 HIGH | Pipelining throughput crash |
| VRI-07 | `-march=native` trong VIT build chain | 🟠 HIGH | SIGILL trên TFB server |
| VRI-08 | `listenTcp()` backlog=128 quá thấp | 🟠 HIGH | Connection refused ở 16K conns |
| VRI-09 | `strlen` vs Unicode byte count | 🔵 LOW | Fortunes Content-Length sai |
| VRI-10 | SIMD JSON parse = 3× malloc/request | 🟡 MEDIUM | Throughput penalty |
| VRI-11 | Ubuntu vs Debian glibc mismatch | 🟡 MEDIUM | Docker runtime fail |
| VRI-12 | `concurrency_rt.c` Windows-only, no Linux path | 🔴 CRITICAL | Linux build fail |

---

## 🗺️ Lộ Trình Cập Nhật — Kết Hợp Cả 2 Repos

### Giai Đoạn 0 — Fix Build Failures (Ưu tiên tuyệt đối)

> Không có các fix này, không gì compile được trên Linux/Docker.

1. **[VRI-12]** Tạo `concurrency_rt_posix.c` với pthread implementation, conditional include trong `NativeCompiler.cpp`
2. **[VRI-05]** Thêm `-std=c11` vào tất cả gcc/clang invocations trong Dockerfile
3. **[RI-22]** Đảm bảo `main.c` trong Vito-framework có đúng `-I` path đến `vit/include/runtime/` và `vit/src/runtime/`

### Giai Đoạn 1 — Fix Verification Failures

4. **[RI-01]** `listenTcp("0.0.0.0", port)`
5. **[RI-03]** Remove `stream.close()` khỏi `send()`, `json()`, `html()` — chỉ close khi `Connection: close`
6. **[RI-05]** Thêm `Server: Vito` + `Date: <RFC1123>` header (dùng cached/atomic update)
7. **[RI-09]** Xóa `query_url`/`update_url` khỏi `benchmark_config.json` cho đến khi implement xong
8. **[VRI-08]** Đổi `listenTcp()` backlog từ 128 → 65535

### Giai Đoạn 2 — Performance & Stability

9. **[RI-06]** Đổi `-march=native` → `-march=x86-64-v3` trong Dockerfile
10. **[VRI-03]** Thêm `pthread_join()` trong `vit_iouring_group_stop()`
11. **[VRI-04]** Thread-safe slab allocator với atomic `free_head`
12. **[RI-08]** Fix HTTP pipelining: advance qua toàn bộ `\r\n\r\n` boundary
13. **[RI-12, RI-13]** `setrlimit(RLIMIT_NOFILE, 1M)` + `RLIMIT_MEMLOCK` khi startup
14. **[RI-15]** Date header atomic double-buffer
15. **[VRI-11]** Thống nhất `debian:bookworm-slim` cho tất cả Dockerfiles

### Giai Đoạn 3 — Full TFB Compliance

16. **[VRI-01]** Cập nhật `benchmark_config.json` notes — honest about epoll backend
17. **[VRI-10]** Zero-alloc JSON path dùng thread-local arena
18. **[RI-10]** Implement PostgreSQL `/db`, `/queries`, `/updates` với libpq
19. **[RI-14]** Fix Fortunes response buffer size
20. Local verify: `./tfb --mode verify --test vito`
21. Stress test: `wrk -t$(nproc) -c16384 -d60s http://localhost:8080/plaintext`
22. Submit PR → TechEmpower/FrameworkBenchmarks

---

## 📌 Tóm Tắt Số Lượng Rủi Ro

| Nguồn | CRITICAL | HIGH | MEDIUM | LOW | Tổng |
|-------|----------|------|--------|-----|------|
| Vito-framework | 5 | 4 | 7 | 6 | **22** |
| VIT runtime | 1 | 4 | 5 | 2 | **12** |
| **Tổng cộng** | **6** | **8** | **12** | **8** | **34** |

---

*Phiên bản v2 — Kết hợp phân tích cả Vito-framework và VIT compiler runtime.*
