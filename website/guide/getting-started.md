# Getting Started with Vito 🚀

**Vito** là một Web Framework hiệu năng cao, nhẹ và dễ mở rộng dành cho ngôn ngữ lập trình **VIT**. Framework được thiết kế nhằm mang lại trải nghiệm phát triển (DX) tuyệt vời tương tự như Express / Fastify / Hono trong hệ sinh thái JavaScript/TypeScript, nhưng chạy trực tiếp trên **VIT Native Runtime**.

---

## 🛠 Cài Đặt 1 Dòng Lệnh (1-Line Quick Install)

Cài đặt trọn bộ **Vit Compiler Engine** và **Vito CLI** chỉ với 1 dòng lệnh duy nhất:

::: code-group

```bash [Linux / macOS]
curl -fsSL https://vit.dev/install.sh | bash
```

```powershell [Windows (PowerShell)]
iwr https://vit.dev/install.ps1 -useb | iex
```

:::

### 1. Yêu cầu tiền đề
- **VIT Compiler Engine**: Đã tích hợp sẵn khi chạy installer.
- **Hệ điều hành hỗ trợ**: Windows (AMD64 / AVX2), Linux (x86_64 / arm64), macOS (Apple Silicon / Intel).

### 2. Tạo dự án mẫu trong 5 giây
Sử dụng bộ công cụ `vito CLI` chính thức:

```bash
vito create my-web-app
cd my-web-app
```


---

## ⚡ Tạo Ứng Dụng HTTP Server Đầu Tiên

Tạo file `main.vit` với nội dung sau:

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";

function main(): number {
    // 1. Khởi tạo Vito App Engine
    let app = createApp();

    // 2. Định nghĩa Route GET đơn giản
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    // 3. Phản hồi JSON API
    app.get("/api/health", (req: Request, res: Response) => {
        res.json("{\"status\":\"UP\", \"uptime\":\"ok\"}");
    });

    // 4. Lắng nghe cổng 8888
    app.listen(8888);
    return 0;
}
```

---

## 🚀 Chạy Ứng Dụng

Sử dụng `vit CLI` để khởi chạy ứng dụng ở chế độ phát triển với **Hot Module Reloading (HMR)**:

```bash
vit dev
```

Truy cập trình duyệt tại địa chỉ: `http://localhost:8888` để xem kết quả! 🎉

---

## 🏗 Kiến Trúc Dự Án (Monorepo)

Cấu trúc dự án Vito chuẩn được tổ chức như sau:

```text
vito/
├── vit.json            # Cấu hình Package Vito
├── src/                # Core HTTP Server Engine & Parser
│   └── vito.vit
├── packages/           # Official Middlewares & Official Extensions
│   ├── cors/           # Middleware xử lý CORS
│   ├── logger/         # Request Logger
│   ├── static/         # Phục vụ File Tĩnh
│   ├── validation/     # Schema Validator (HTTP 422)
│   ├── security/       # Rate Limiting & Auth Guard
│   ├── swagger/        # Swagger UI & OpenAPI Docs Generator
│   ├── sse/            # Server-Sent Events cho AI/LLM Stream
│   ├── db/ & orm/      # Native DB Drivers & Vito ORM
│   └── edge/           # Cloud Edge Runtime WASM Adapter
└── examples/           # Các ứng dụng mẫu thực tế
```
