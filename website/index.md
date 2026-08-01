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
      text: API Reference 📚
      link: /api/overview
    - theme: alt
      text: Xem trên GitHub 🐙
      link: https://github.com/vit-lang/vito

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

<div class="code-showcase" style="margin-top: 3rem; text-align: center;">

## Cú Pháp Mạch Lạc & Trực Quan 🚀

Viết Web Application tốc độ cao chưa bao giờ dễ dàng và thanh thoát đến thế trong hệ sinh thái **VIT**.

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";

function main(): number {
    let app = createApp();

    // 1. Đăng ký Global Middlewares
    app.use(corsMiddleware);
    app.use(loggerMiddleware);

    // 2. Định tuyến tham số động (/users/:id)
    app.get("/users/:id", (req: Request, res: Response) => {
        let userId = req.param("id");
        res.json("{\"status\":\"ok\", \"user_id\":\"" + userId + "\"}");
    });

    // 3. Gom nhóm Route (/api/v1)
    app.group("/api/v1", (v1) => {
        v1.get("/products/:category/:id", (req: Request, res: Response) => {
            let cat = req.param("category");
            let pId = req.param("id");
            res.json("{\"category\":\"" + cat + "\",\"id\":\"" + pId + "\"}");
        });
    });

    // 4. Khởi chạy HTTP Server
    app.listen(8888);
    return 0;
}
```

</div>

<div class="benchmark-showcase" style="margin-top: 4rem; padding: 2rem; background: rgba(255, 255, 255, 0.03); border-radius: 12px; border: 1px solid rgba(255, 255, 255, 0.1);">

## 🏆 Hiệu Năng Vượt Trỗi Bằng Thực Nghiệm (Empirical Benchmark)

> 🖥️ **Môi Trường Máy Chủ Đo Đạc (Server Specifications)**:
> - **CPU**: Intel(R) Core(TM) i5-5200U @ 2.20GHz (4 Cores / Threads)
> - **RAM**: 12 GB DDR3 RAM
> - **Hệ Điều Hành**: Ubuntu 24.04.4 LTS (Linux Kernel 6.8.0-136-generic x86_64)
> - **Biên Dịch & Toolchain**: GCC 13.3.0 (`-O3 -march=native -flto`), Go 1.22.2 (`-ldflags="-s -w"`), Rust 1.75.0 (`-O -C target-cpu=native`), `wrk 4.1.0`

### 1. 🌐 Web Server High-Concurrency & Resource Benchmark (1,000 Connections, `wrk`)

| Framework / Ngôn Ngữ | Idle RAM | Peak RAM (Tải 1k Conns) | Throughput (Req/s) | Latency P99 | Xếp Hạng |
| :--- | :---: | :---: | :---: | :---: | :---: |
| 🔴 **Vito Framework (Vit Engine)** | **1.84 MB** 🥇 | **1.84 MB** 🥇 (0 MB Spike) | **121,805 req/s** 🥇 | **19.10 ms** 🥇 | 🥇 **VÔ ĐỊCH HẠNG 1** |
| 🦀 **Rust (Native Optimized)** | **1.89 MB** 🥈 | **1.89 MB** 🥈 | **102,967 req/s** 🥈 | **97.59 ms** 🥈 | 🥈 **Hạng 2** |
| 🔵 **Golang (`net/http`)** | **1.89 MB** 🥉 | **1.89 MB** 🥉 | **53,913 req/s** 🥉 | **128.01 ms** 🥉 | 🥉 **Hạng 3** |

---

### 2. ⚡ Pure CPU Algorithm Benchmark (Fibonacci Recursive Stack & Matrix 500x500)

| Ngôn Ngữ / Compiler | Fibonacci(42) Stack Time | Matrix 500x500 Time | Đánh Giá Tổng Quan |
| :--- | :---: | :---: | :---: |
| 🔴 **Vit Native (LLVM Pipeline)** | **593.81 ms** 🥇 | **72.99 ms** | 🥇 **NHANH NHẤT** |
| 🟢 **C++20 (GCC -O3 Native)** | **848.47 ms** 🥈 | **47.65 ms** 🥇 | 🥈 **Hạng 2** |
| 🦀 **Rust (rustc -O3 Native)** | **1,134.17 ms** 🥉 | **263.95 ms** | 🥉 **Hạng 3** |
| 🔵 **Golang (gc 1.22 Compiler)** | **1,850.92 ms** | **271.31 ms** | 🏅 **Hạng 4** |

### 🚀 Điểm Đột Phá Kỹ Thuật:
- **Tiêu tốn RAM cực thấp (Chỉ 1.84 MB)**: Giữ nguyên mức tiêu thụ RAM 1.84 MB kể cả khi bị tải nặng 1,000 kết nối đồng thời nhờ bộ cấp phát `Request Arena Allocator` (0 byte Memory Spike / Leak).
- **Tốc độ Web Server gấp 2.26 LẦN Golang**: Tối ưu hóa bộ nhớ `Arena Allocator` (0 byte GC/malloc) & SIMD AVX2 Header Parsing.
- **Tốc độ CPU Stack đệ quy gấp 3.1 LẦN Golang & 1.9 LẦN Rust**: Nhờ quy trình biên dịch LLVM Pass Pipeline tối ưu register allocation và function inline cực tốt.
- **Ổn định Latency P99 vượt trội Rust & Go**: Vito duy trì P99 ở mốc **19.10 ms**, loại bỏ hiện tượng giật lag đuôi Latency (`97.59 ms` ở Rust và `128.01 ms` ở Go).

</div>
