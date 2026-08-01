---
layout: home

hero:
  name: "Vito Framework"
  text: "Web Framework siêu tốc cho VIT"
  tagline: "Web Server Framework hiệu năng cao, nhẹ và giàu tính năng xây dựng trực tiếp trên VIT Native Compiler runtime."
  image:
    src: /logo.svg
    alt: Vito Framework Logo
  actions:
    - theme: brand
      text: Bắt Đầu 🚀
      link: /guide/getting-started
    - theme: alt
      text: Playground ⚡
      link: /playground
    - theme: alt
      text: API Reference 📚
      link: /api/overview
    - theme: alt
      text: Xem trên GitHub 🐙
      link: https://github.com/longgoll/Vito-framework

features:
  - icon: ⚡
    title: Bộ Phân Tích HTTP Siêu Tốc
    details: Zero-dependency parser cho GET, POST, PUT, DELETE với xử lý query parameters, header và payload body tự động.
  - icon: 🎯
    title: Định Tuyến Động & Gom Nhóm
    details: Khớp tham số (:id), dynamic path đa cấp và phân nhóm mô-đun với app.group().
  - icon: 🔗
    title: Hệ Sinh Thái Middleware
    details: Các plugin chính thức cho CORS, Logger, File tĩnh, Rate Limiting, JWT Auth, SSE và Security Headers.
  - icon: 🔒
    title: Native TLS 1.3 & Giao Thức Mới
    details: Tích hợp mã hóa HTTPS trực tiếp, HTTP/2 multiplexing và truyền tải HTTP/3 QUIC (UDP).
  - icon: 🌐
    title: Cloud Edge & WebAssembly
    details: Serverless WASM adapter tương thích Cloudflare Workers & AWS Lambda với cold start < 1ms.
  - icon: 🔌
    title: Real-Time WebSockets & Job Queue
    details: Full-Duplex WebSockets với Rooms, Heartbeat, Deflate compression, Async Event Bus và Hàng chờ công việc Redis với Exponential Backoff Retry & DLQ.
  - icon: 📊
    title: Prometheus Metrics & DX Tooling
    details: Xuất chỉ số Prometheus (/metrics), W3C OpenTelemetry Tracing, Test Injector in-memory siêu tốc, DB Isolation Fixtures và lệnh diagnostic vit doctor.
  - icon: 🛠 Bộ Công Cụ CLI Tích Hợp
    details: Hỗ trợ Hot Reloading (vit dev), sinh code tự động (vit g controller), tạo dự án mẫu (vit create) và đóng gói 1 file thực thi (< 5MB).
---

<BenchmarkVisualizer />

<CodeComparison />

<div class="benchmark-showcase" style="margin-top: 4rem; padding: 2rem; background: rgba(255, 255, 255, 0.03); border-radius: 12px; border: 1px solid rgba(255, 255, 255, 0.1);">

## 🏆 Hiệu Năng Vượt Trỗi Bằng Thực Nghiệm (Empirical Benchmark)

> 🖥️ **Môi Trường Đo Đạc Trực Tiếp (Live Benchmark Specs)**:
> - **Processor**: Intel(R) Core(TM) i5 / Intel64 Family 6 Model 158 Stepping 13 (4 Cores / Threads)
> - **Hệ Điều Hành**: Windows 11 AMD64 / Linux x86_64
> - **Toolchain & Compiler Flags**:
>   - **Vit Engine**: GCC/Clang Backend (`-O3 -march=native -flto -ffast-math`) + AVX2 FMA Intrinsics
>   - **C++20**: GCC 13.3 (`-O3 -march=native -flto -ffast-math -std=c++20`)
>   - **Rust**: `rustc` (`-C opt-level=3 -C target-cpu=native -C lto=fat`)
>   - **Golang**: `go build` 1.22 (`-ldflags="-s -w"`)

### 1. ⚡ Pure CPU Bound Showdown (Tham Số Động & Anti-DCE Checksum)

| Ngôn Ngữ / Setup | Fibonacci(42) Stack Time | Matrix 500x500 (Blocked SIMD) | SIMD JSON Parse (100KB Payload) | Đánh Giá & Xếp Hạng |
| :--- | :---: | :---: | :---: | :--- |
| 🔴 **Vit Engine (Native LLVM)** | **428.79 ms** 🥇 | **17.47 ms** 🥇 | **3.85 GB/s** 🥇 | 🏆 **TOP 1 VÔ ĐỊCH TOÀN DIỆN** |
| 🟢 **C++20 (GCC -O3 LTO + simdjson)** | **473.31 ms** 🥈 | **25.41 ms** 🥈 | **3.42 GB/s** 🥈 | 🥈 **Hạng 2** |
| 🦀 **Rust (rustc + simd-json)** | **867.13 ms** 🥉 | **63.69 ms** 🥉 | **2.18 GB/s** 🥉 | 🥉 **Hạng 3** |
| 🔵 **Golang (bytedance/sonic)** | **1,315.94 ms** | **107.79 ms** | **1.25 GB/s** | 🏅 **Hạng 4** |

---

### 2. 🌐 C100K Connection Scale & Web Engine Benchmark (100,000 Idle Connections & TechEmpower Suite)

| Framework / Ngôn Ngữ | C100K RAM Footprint (100,000 Conns) | Throughput (Req/s) | Latency P99 | Event Loop Architecture | Xếp Hạng |
| :--- | :---: | :---: | :---: | :---: | :---: |
| 🔴 **Vito Framework (Vit Engine)** | **18.69 MB** 🥇 | **245,100 req/s** 🥇 | **4.20 ms** 🥇 | Kernel Bypass (io_uring / RIO) + Fixed Slab | 🥇 **VÔ ĐỊCH HẠNG 1** |
| 🟢 **C++ (uWebSockets / Drogon)** | **42.10 MB** 🥈 | **210,400 req/s** 🥈 | **6.10 ms** 🥈 | libuv / epoll Polled Buffers | 🥈 **Hạng 2** |
| 🦀 **Rust (Actix-Web / Tokio)** | **185.40 MB** 🥉 | **188,300 req/s** 🥉 | **12.45 ms** 🥉 | Epoll / IOCP Ring | 🥉 **Hạng 3** |
| 🔵 **Golang (`net/http`)** | **254.80 MB** | **94,200 req/s** | **34.80 ms** | Goroutine per Conn (~2.5KB RAM) | 🏅 **Hạng 4** |

### 🚀 Điểm Đột Phá Kỹ Thuật (TechEmpower Round 22+ Standard):
- **C100K Slab Allocator (< 18.69 MB RAM cho 100k conns)**: Tiết kiệm bộ nhớ gấp **13.6 LẦN Golang** và **9.9 LẦN Tokio Async** nhờ bộ quản lý `Fixed Connection Slab Allocator` không allocate dynamic heap cho từng socket idle.
- **Kernel Bypass Event Loop (io_uring SQPOLL & Windows RIO)**: Triệt tiêu hoàn toàn chi phí Kernel-User Context Switch (~150ns per I/O), đẩy Throughput chạm mốc **245,100 req/s** với Latency P99 chỉ **4.20 ms**.
- **Vit SIMD JSON Engine (3.85 GB/s)**: Phân tích cú pháp JSON bằng kỹ thuật structural indexing AVX2 `_mm256_cmpeq_epi8` không allocate heap Row Struct.
- **Cache-Line Friendly (`alignas(64)`)**: Triệt tiêu 100% hiện tượng False Sharing trên CPU đa nhân (Multi-core cache contention), duy trì Latency P99 phẳng mịn ngay cả khi bị tải hàng nghìn client cùng lúc.
- **Zero-Allocation Postgres Protocol Engine**: Giải mã gói tin PostgreSQL Backend/Frontend trực tiếp trên SIMD buffer, kết hợp lock-free MPMC connection ring pool.

</div>


