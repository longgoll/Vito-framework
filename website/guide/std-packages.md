# Thư Viện Chuẩn & Các Package Chính Thức (Standard Library & Packages) 📦

**Vito Framework** đi kèm với bộ **Thư viện chuẩn (Standard Library - `std`)** mạnh mẽ được tối ưu trực tiếp cho trình biên dịch **VIT Native Engine**, cùng **27 Package chính thức** cung cấp đầy đủ công cụ để xây dựng ứng dụng web từ nhỏ đến quy mô Enterprise.

---

## 📚 1. Bộ Thư Viện Chuẩn (Vito Standard Library - `std`)

Bộ thư viện chuẩn của VIT / Vito được thiết kế theo triết lý Zero-Dependency, đạt hiệu năng xử lý cực cao nhờ tối ưu hóa lệnh SIMD và không gây overhead cho bộ nhớ.

| Mô-đun Standard | Đường dẫn Import | Chức Năng Chính | Ví Dụ Sử Dụng |
| :--- | :--- | :--- | :--- |
| **IO / Terminal** | `import { print, println } from "std/io"` | Xử lý nhập/xuất dữ liệu chuẩn, formatted output, đọc input từ Terminal. | `print("Hello Vito!");` |
| **Toán Học (Math)** | `import { abs, sqrt, max } from "std/math"` | Các hàm toán học cơ bản và nâng cao tối ưu hóa SIMD CPU. | `let res = sqrt(16.0);` |
| **Xử Lý Chuỗi** | `import { String } from "std/string"` | Thao tác chuỗi UTF-8, split, join, replace, regex matching, trim. | `let s = str.trim(" hello ");` |
| **Tập Hợp (Collections)**|`import { Map, Vector } from "std/collections"`| Mảng động (Vector), Bảng băm (HashMap), Stack, Queue lock-free. | `let map = Map.new();` |
| **Tệp Tin (FS)** | `import { File, readText } from "std/fs"` | Đọc/Viết file, Stream buffer, duyệt thư mục, kiểm tra tồn tại. | `let text = readText("config.json");` |
| **Môi Trường & Hệ Thống**|`import { getEnv, osName } from "std/env"` | Đọc biến môi trường `.env`, thông tin HĐH, số nhân CPU, RAM. | `let port = getEnv("PORT");` |
| **Xử Lý Bất Đồng Bộ**| `import { async, await } from "std/async"` | Coroutines, Event Loop, Promise/Future API và Task Spawner. | `async function fetchData() { ... }` |
| **Đa Luồng (Thread)** | `import { Thread, Channel } from "std/thread"`| Tạo Native OS Thread, MPMC Channel truyền dữ liệu giữa các luồng. | `let t = Thread.spawn(worker);` |
| **Socket & Network** | `import { TcpServer, Socket } from "std/net"` | Low-level Non-blocking TCP/UDP Socket, IP Address parsing. | `let server = TcpServer.bind(8080);` |
| **HTTP Parser** | `import { HttpRequest, HttpResponse }` | Phân tích cú pháp gói tin HTTP/1.1 zero-copy header parser. | `let req = HttpRequest.parse(buf);` |
| **JSON Serialization**|`import { JSON } from "std/json"` | Phân tích cú pháp & mã hóa JSON đạt tốc độ 3.85 GB/s (AVX2 SIMD). | `let obj = JSON.parse(str);` |

---

## 🛠️ 2. Hệ Sinh Thái 27 Package Chính Thức (`vito/packages`)

Hệ sinh thái Vito cung cấp sẵn 27 package chính chủ giúp bạn phát triển ứng dụng mà không cần phụ thuộc vào thư viện bên thứ 3.

<div class="card-grid">
  <div class="feature-mini-card">
    <h4>🧱 1. Vito Core & Router</h4>
    <p><code>vito/router</code>, <code>vito/http_parser</code>, <code>vito/pool</code></p>
    <p>Bộ định tuyến Radix Tree siêu tốc, parser gói tin và Connection Slab Allocator.</p>
  </div>

  <div class="feature-mini-card">
    <h4>⚡ 2. Protocols Thế Hệ Mới</h4>
    <p><code>vito/http2</code>, <code>vito/http3</code>, <code>vito/tls</code></p>
    <p>Tích hợp sẵn HTTPS Native (OpenSSL/BoringSSL), HTTP/2 Multiplexing & HTTP/3 QUIC.</p>
  </div>

  <div class="feature-mini-card">
    <h4>🗄️ 3. Vito ORM & CSDL</h4>
    <p><code>vito/orm</code>, <code>vito/db</code></p>
    <p>Query Builder, Schema Migration, Driver Postgres / SQLite / MySQL với MPMC Pool.</p>
  </div>

  <div class="feature-mini-card">
    <h4>🔒 4. Bảo Mật & Auth</h4>
    <p><code>vito/auth</code>, <code>vito/security</code>, <code>vito/cors</code>, <code>vito/session</code></p>
    <p>Xác thực JWT token, Session Cookie, Helmet Security Headers, Anti-CSRF, CORS Guard.</p>
  </div>

  <div class="feature-mini-card">
    <h4>📡 5. Real-Time & AI Streaming</h4>
    <p><code>vito/websocket</code>, <code>vito/sse</code>, <code>vito/events</code></p>
    <p>WebSockets Full-Duplex với Rooms, Server-Sent Events chuyên dụng cho LLM Token Streaming.</p>
  </div>

  <div class="feature-mini-card">
    <h4>⚙️ 6. Hàng Chờ Jobs & Upload</h4>
    <p><code>vito/queue</code>, <code>vito/logger</code>, <code>vito/upload</code></p>
    <p>Hàng chờ công việc Redis bất đồng bộ, Structured JSON Logger, Multipart File Upload.</p>
  </div>

  <div class="feature-mini-card">
    <h4>📊 7. Monitoring & Docs Auto</h4>
    <p><code>vito/metrics</code>, <code>vito/health</code>, <code>vito/swagger</code></p>
    <p>Xuất chỉ số Prometheus (/metrics), Healthcheck endpoint, tự động tạo OpenAPI / Swagger UI.</p>
  </div>

  <div class="feature-mini-card">
    <h4>🛠️ 8. Cloud Edge & DX Tooling</h4>
    <p><code>vito/edge</code>, <code>vito/testing</code>, <code>vito/validation</code>, <code>vito/cli</code></p>
    <p>Cloudflare Workers / WASM adapter, Test Injector in-memory và Data Validator schema.</p>
  </div>
</div>

---

## 💻 Mã Nguồn Mẫu Kết Hợp Thư Viện Chuẩn & Packages

```typescript
import { print } from "std/io";
import { getEnv } from "std/env";
import { JSON } from "std/json";
import { Vito, Request, Response } from "vito";
import { Auth } from "vito/auth";
import { ORM } from "vito/orm";

function main(): number {
    let app = Vito.new();
    let port = getEnv("PORT") || "3000";

    // Route sử dụng Vito ORM & std/json
    app.get("/api/users", (req: Request, res: Response) => {
        let users = ORM.table("users").limit(10).get();
        res.json(JSON.stringify(users));
    });

    app.listen(port);
    print("⚡ Server started on port: " + port);
    return 0;
}
```
