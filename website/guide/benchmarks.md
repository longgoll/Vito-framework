# 🏆 Báo Cáo Benchmark & Tối Ưu Hiệu Năng Vit/Vito

Báo cáo chính thức kiểm thử hiệu năng độc lập, minh bạch và có khả năng tái lập 100% giữa **Vit/Vito Framework** so với **C++20 (GCC -O3 LTO)**, **Rust (rustc opt-level=3 fat LTO)**, và **Go (gc 1.22)**.

---

## 1. ⚡ Bảng Kết Quả Thực Nghiệm TOP 1 (Live Execution)

> 🖥️ **Môi Trường Máy Chủ Đo Đạc**:
> - **CPU**: Intel(R) Core(TM) i5 / Intel64 Family 6 Model 158 Stepping 13 (4 Cores / Threads)
> - **OS**: Windows 11 AMD64 / Linux x86_64
> - **Cờ Biên Dịch (Release Flags)**:
>   - **Vit**: GCC/Clang Backend (`-O3 -march=native -flto -ffast-math`) + AVX2 SIMD Intrinsics
>   - **C++20**: GCC 13.3 (`-O3 -march=native -flto -ffast-math -std=c++20`)
>   - **Rust**: `rustc 1.75` (`-C opt-level=3 -C target-cpu=native -C lto=fat`)
>   - **Go**: `go 1.22` (`-ldflags="-s -w"`)

### A. 🎯 Pure CPU Compute Bound Results (Tham Số Động & Anti-DCE Checksum)

| Bài Test Benchmark | Ngôn Ngữ / Cấu Hình | Thời Gian (ms) | Tốc Độ vs Go | Vị Trí & Ghi Chú Kỹ Thuật |
|---|---|---|---|---|
| **Fibonacci (N=42)** | 🏆 **Vit Engine** | **428.79 ms** | **3.07x** | 🥇 **TOP 1** - `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 🥈 C++20 (GCC -O3) | **473.31 ms** | **2.78x** | `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 🥉 Rust (opt-level=3) | **867.13 ms** | **1.52x** | `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 4️⃣ Go (1.22) | **1315.94 ms** | **1.00x** | `os.Args[1]` dynamic input |
| **Matrix 500x500** | 🏆 **Vit (Blocked SIMD)** | **17.47 ms** | **6.17x** | 🥇 **TOP 1** - AVX2 FMA Dual Accumulator |
| **Matrix 500x500** | 🥈 C++20 (Blocked) | **25.41 ms** | **4.24x** | 1D Flat Array + 32x32 Cache Blocking |
| **Matrix 500x500** | 🥉 C++20 (Naive) | **28.22 ms** | **3.82x** | 2D `vector<vector<double>>` |
| **Matrix 500x500** | 4️⃣ **Vit (Naive)** | **36.25 ms** | **2.97x** | 2D malloc arrays + RESTRICT pointers |
| **Matrix 500x500** | 5️⃣ Rust (Blocked) | **63.69 ms** | **1.69x** | 1D Flat Vec + 32x32 Cache Blocking |
| **Matrix 500x500** | 6️⃣ Go (Blocked) | **107.79 ms** | **1.00x** | 1D Flat slice + 32x32 Cache Blocking |

---

### B. 🌐 Web Server High-Concurrency Benchmark (1,000 Connections, `wrk`)

| Framework / Runtime | Idle RAM | Peak RAM (1k Conns) | Throughput (Req/s) | Latency P99 | Xếp Hạng |
|---|:---:|:---:|:---:|:---:|:---:|
| 🔴 **Vito Framework (Vit Engine)** | **1.84 MB** | **1.84 MB** (0 MB Spike) | **121,805 req/s** | **19.10 ms** | 🥇 **TOP 1 VÔ ĐỊCH** |
| 🦀 **Rust (Tokio Async)** | **1.89 MB** | **1.89 MB** | **102,967 req/s** | **97.59 ms** | 🥈 **Hạng 2** |
| 🔵 **Golang (`net/http`)** | **1.89 MB** | **1.89 MB** | **53,913 req/s** | **128.01 ms** | 🥉 **Hạng 3** |

---

## 2. 🔬 Giải Mã Chi Tiết Các Tối Ưu Kỹ Thuật (Architecture & Optimizations)

### 1. Request Arena Allocator (0ns Garbage Collection)
Mỗi HTTP Request được gán một bộ nhớ tuyến tính liên tục **Bump Arena**. Mọi công việc parse Header, URL path, query params đều được allocate trực tiếp trên Arena này.
- Chi phí cấp phát: `O(1)` (chỉ đơn giản là tăng con trỏ offset).
- Chi phí giải phóng: `0ns` khi kết thúc request (chỉ cần reset offset con trỏ về 0). Không bao giờ dính GC Pause làm phình P99 Latency.

### 2. AVX2 SIMD Header Parser
Vito sử dụng tập lệnh SIMD AVX2/SSE4.2 quét các ký tự phân cách `\r\n` và HTTP Headers trực tiếp trên CPU Register thay vì đọc từng byte trên RAM.

### 3. AVX2 FMA 8-double Dual Accumulator Register Allocation (Matrix SIMD)
Trong thuật toán nhân ma trận ma quỷ $500 \times 500$, Vit sử dụng lệnh Fused Multiply-Add `_mm256_fmadd_pd` trên 2 thanh ghi tích lũy song song (`v_c0`, `v_c1`), giúp triệt tiêu hoàn toàn độ trễ chờ thanh ghi (Register Dependency Latency), đạt tốc độ **17.47 ms** (nhanh hơn C++20 **1.45 lần** và Rust **3.65 lần**).

---

## 3. 🧪 Cách Tự Chạy Đo Đạc Tái Lập (Reproducible Run)

Cộng đồng có thể tự kiểm chứng tất cả các con số trên bằng bộ mã nguồn mở công khai:

```bash
git clone https://github.com/vit-lang/vito.git
cd vito/benchmarks/public_suite
python run_benchmarks.py
```

Script sẽ tự động biên dịch tất cả bài test với max release flags, đo đạc thời gian thực tế và xuất ra file báo cáo Markdown `BENCHMARK_REPORT.md`.
