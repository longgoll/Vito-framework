# Vito Web Framework 🚀

> High-performance, lightweight, expressive Web Server Framework for the **VIT Language ecosystem**.

🌐 **Official Documentation & Website**: [https://longgoll.github.io/Vito-framework/](https://longgoll.github.io/Vito-framework/)

[English](#english) | [Tiếng Việt](#tiếng-việt)

---

<a name="english"></a>
## 🇬🇧 English Documentation

### 🌟 Overview
**Vito** is an open-source, modern Web Framework built on top of the native **VIT Compiler runtime**. Designed for simplicity, speed, and DX excellence.

📖 **Full Online Guide & Interactive Playground**: [https://longgoll.github.io/Vito-framework/](https://longgoll.github.io/Vito-framework/)

### 🛠 Features
- ⚡ **HTTP Request Parser**: Automatically parses HTTP Methods (`GET`, `POST`, `PUT`, `DELETE`), Path, Query parameters, and Body content.
- 🎯 **Dynamic Route Parameters (`:id`)**: Support for parameterized routes (e.g. `/users/:id`, `/products/:category/:id`) with `req.param(key)`.
- 🔍 **Query & Header Helpers**: Fast access to URL search query parameters with `req.query(key)` and request headers with `req.header(key)`.
- 🛣 **Expressive Router**: Route registration with `.get()`, `.post()`, `.put()`, `.delete()`.
- 👥 **Route Grouping**: Modular route prefixing with `app.group("/api/v1", (v1) => { v1.get("/users", ...); })`.
- 🎨 **Custom Error Handling**: Customize 404 Not Found & error handlers using `app.setNotFoundHandler(...)`.
- 🔗 **Middleware Pipeline**: Chainable middleware execution (`app.use()`) for CORS, request logging, static file serving, sessions, and multipart uploads.
- 📁 **Static File Middleware**: Serve static HTML/CSS/JS/Images directly from disk via `createStaticMiddleware(publicDir)`.
- 📦 **Response Context**: Built-in methods for JSON (`res.json()`), HTML (`res.html()`), text (`res.send()`), status codes (`res.setStatus(code)`), and headers (`res.setHeader()`).

---

### 📁 Repository Architecture (Monorepo)

```text
vito/
├── vit.json            # Framework package config
├── README.md           # Documentation
├── website/            # Documentation Website (VitePress)
├── src/                # Core Web Framework Engine
│   └── vito.vit        # HTTP Parser, Dynamic Matcher, Route Grouping & App Engine
├── packages/           # Official Middleware & Enterprise Packages (27 Modules)
│   ├── auth/           # JWT Guard Pro, OAuth2 PKCE, API Key, Argon2id/AES-256-GCM, RBAC
│   ├── cors/           # CORS header & OPTIONS preflight middleware
│   ├── db/             # Connection Pooling for Postgres, SQLite, MySQL
│   ├── events/         # Async Event Emitter & Redis Pub/Sub Cluster
│   ├── health/         # Connection tracking & Kubernetes health probes (/healthz, /readyz)
│   ├── logger/         # Request logger middleware
│   ├── metrics/        # Prometheus /metrics exporter & OpenTelemetry
│   ├── orm/            # Vito ORM, ACID Transactions, Savepoints, Migrations & Seeders
│   ├── queue/          # Background Job Queue (Redis/Memory drivers, DLQ)
│   ├── router/         # Radix Trie O(1) fast path & dynamic routing engine
│   ├── security/       # Helmet security headers, Rate Limiter (429) & JWT Guard
│   ├── session/        # Signed/Encrypted cookies & session management
│   ├── sse/            # Server-Sent Events for AI/LLM Token Streaming
│   ├── static/         # Static file serving middleware with ETag & Gzip
│   ├── swagger/        # OpenAPI 3.0 auto generator & Swagger UI (/docs)
│   ├── upload/         # Multipart form data streaming & file validation engine
│   └── websocket/      # WebSockets Engine (50K+ conns, Room Broadcast & Heartbeat)
└── examples/           # Web app examples
    ├── app_demo.vit    # Complete router, dynamic params, grouping & middleware demo
    └── standalone_demo.vit
```

---

### 🚀 Quick Start

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";

function main(): number {
    let app = createApp();

    // 1. Register Middlewares
    app.use(corsMiddleware);
    app.use(loggerMiddleware);

    // 2. Custom Error Handler (404)
    app.setNotFoundHandler((req: Request, res: Response) => {
        res.setStatus(404).html("<h1>404 Not Found 😢</h1>");
    });

    // 3. Dynamic Route Parameter (/users/:id)
    app.get("/users/:id", (req: Request, res: Response) => {
        let userId = req.param("id");
        res.json("{\"user_id\":\"" + userId + "\"}");
    });

    // 4. Query Parameter (/search?q=vit)
    app.get("/search", (req: Request, res: Response) => {
        let keyword = req.query("q");
        res.json("{\"search_query\":\"" + keyword + "\"}");
    });

    // 5. Route Grouping (/api/v1)
    app.group("/api/v1", (v1) => {
        v1.get("/products/:category/:id", (req: Request, res: Response) => {
            let cat = req.param("category");
            let pId = req.param("id");
            res.json("{\"category\":\"" + cat + "\",\"id\":\"" + pId + "\"}");
        });
    });

    // 6. Start Server
    app.listen(8888);
    return 0;
}
```

---

<a name="tiếng-việt"></a>
## 🇻🇳 Tài Liệu Tiếng Việt

### 🌟 Tổng Quan
**Vito** là một Web Framework nguồn mở, hiện đại được xây dựng trực tiếp trên bộ biên dịch **VIT Compiler runtime**. Được thiết kế tối ưu cho tốc độ thực thi Native, cú pháp ngắn gọn và trải nghiệm lập trình (DX) mượt mà.

📖 **Trang Tài Liệu Hướng Dẫn & Thử Nghiệm Tương Tác**: [https://longgoll.github.io/Vito-framework/](https://longgoll.github.io/Vito-framework/)

### 🛠 Tính Năng Chi Tiết
- ⚡ **Bộ Phân Tích HTTP Request**: Tự động phân tích các HTTP Method (`GET`, `POST`, `PUT`, `DELETE`), URL Path, Query Parameter và nội dung Body.
- 🎯 **Tham Số Route Động (`:id`)**: Hỗ trợ khớp đường dẫn với tham số động (ví dụ `/users/:id`, `/products/:category/:id`) thông qua `req.param(key)`.
- 🔍 **Trích Xuất Query & Header**: Truy cập nhanh các query URL bằng `req.query(key)` và các HTTP request header với `req.header(key)`.
- 🛣 **Router Linh Hoạt**: Đăng ký các route xử lý ngắn gọn bằng `.get()`, `.post()`, `.put()`, `.delete()`.
- 👥 **Phân Nhóm Đường Dẫn (Route Grouping)**: Tạo tiền tố route dạng module hóa bằng `app.group("/api/v1", (v1) => { v1.get("/users", ...); })`.
- 🎨 **Tùy Biến Xử Lý Lỗi**: Định nghĩa handler tùy chỉnh cho lỗi 404 Trang Không Tìm Thấy bằng `app.setNotFoundHandler(...)`.
- 🔗 **Chuỗi Middleware Execution**: Cho phép liên kết chuỗi middleware (`app.use()`) để hỗ trợ CORS, ghi log request, phục vụ static file, quản lý Session và Upload file multipart.
- 📁 **Static File Serving**: Trả về các file tĩnh HTML/CSS/JS/Hình ảnh trực tiếp từ ổ đĩa bằng `createStaticMiddleware(publicDir)`.
- 📦 **Nội Dung Trả Về (Response Context)**: Hỗ trợ sẵn các hàm tiện ích trả về JSON (`res.json()`), HTML (`res.html()`), văn bản (`res.send()`), mã trạng thái (`res.setStatus(code)`), và thiết lập header (`res.setHeader()`).

---

### 📦 27 Gói Mô-Đun & Middleware Tích Hợp (`vito/packages`)

- `packages/auth`: JWT Guard Pro (HS256/RS256/ES256), OAuth2 PKCE (Google/GitHub/Microsoft), API Key Guard, Argon2id/Bcrypt Hasher, AES-256-GCM Encryption, RBAC/ABAC Gate Engine.
- `packages/cors`: Middleware xử lý cross-origin request và tự động phản hồi OPTIONS preflight request.
- `packages/db` & `packages/orm`: Vito ORM, Connection Pooling (Postgres, SQLite, MySQL), ACID Transactions, Savepoints & Migrations.
- `packages/events`: Async Event Emitter & Redis Pub/Sub Cluster Adapter.
- `packages/logger`: Middleware ghi log mọi HTTP request (Method, Path, Response Code, Execution Time) ra console.
- `packages/metrics`: Prometheus `/metrics` Exporter & OpenTelemetry tracing.
- `packages/queue`: Background Job Queue Engine (Redis/Memory drivers, Exponential Backoff Retry & DLQ).
- `packages/sse`: Server-Sent Events cho AI/LLM Token Streaming.
- `packages/static`: Middleware phục vụ file tĩnh với bộ đệm bộ nhớ đĩa hiệu năng cao.
- `packages/swagger`: Tự động tạo OpenAPI 3.0 Specs & Giao diện Swagger UI tại `/docs`.
- `packages/session`: Quản lý cookie mã hóa và phiên làm việc của người dùng.
- `packages/upload`: Xử lý upload file dạng Multipart Form-Data và kiểm tra định dạng file.
- `packages/websocket`: WebSockets Server Engine (50K+ conns, Room Broadcast & Heartbeat).
- `packages/health`: Cung cấp các Endpoint probe kiểm tra sức khỏe hệ thống cho Kubernetes (`/healthz`, `/readyz`).

---

## 📜 License
MIT License - Open-source & Free for the VIT Community.
