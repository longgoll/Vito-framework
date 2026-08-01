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
  - icon: 🛠 Bộ Công Cụ CLI Tích Hợp
    details: Hỗ trợ Hot Reloading (vit dev), tạo dự án mẫu (vit create) và đóng gói 1 file thực thi (< 5MB).
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
