# Middleware System 🔗

**Middleware** trong Vito là các hàm can thiệp vào chuỗi xử lý HTTP Request trước khi request tới route handler chính.

---

## 🔗 Đăng Ký Middleware Phổ Biến

Vito cung cấp sẵn hệ sinh thái gói middleware chính thức trong thư mục `packages/`:

```javascript
import { createApp } from "vito/src/vito.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { createStaticMiddleware } from "vito/packages/static/static.vit";
import { securityHeadersMiddleware, createRateLimiter } from "vito/packages/security/security.vit";

function main(): number {
    let app = createApp();

    // 1. Log HTTP requests (Method, Path, Processing Time)
    app.use(loggerMiddleware);

    // 2. Enable CORS headers & OPTIONS preflight
    app.use(corsMiddleware);

    // 3. Security Headers (Helmet style) & Rate Limiting
    app.use(securityHeadersMiddleware);
    app.use(createRateLimiter(100, 60)); // Tối đa 100 req/phút

    // 4. Serve Static Files từ thư mục "public"
    app.use(createStaticMiddleware("./public"));

    return 0;
}
```

---

## 🛡 Danh Sách Official Packages

| Package | Mục Đích | File |
| :--- | :--- | :--- |
| **CORS** | Xử lý Cross-Origin Resource Sharing & OPTIONS Preflight | `packages/cors/cors.vit` |
| **Logger** | In log chi tiết các HTTP request ra console | `packages/logger/logger.vit` |
| **Static** | Phục vụ file tĩnh (HTML, CSS, JS, Images, Fonts) | `packages/static/static.vit` |
| **Validation** | Kiểm tra Schema request, tự động trả 422 Unprocessable Entity | `packages/validation/validation.vit` |
| **Security** | Helmet security headers, Rate Limiting (429) & JWT Guard (401) | `packages/security/security.vit` |
| **Swagger** | Tự động sinh `openapi.json` & Giao diện Swagger UI (`/docs`) | `packages/swagger/swagger.vit` |
| **SSE Stream** | Server-Sent Events cho Response Streaming (AI/LLM token) | `packages/sse/sse.vit` |
| **TLS 1.3** | Mã hóa HTTPS trực tiếp không cần Nginx | `packages/tls/tls.vit` |
| **Database/ORM**| Driver Postgres, SQLite, Redis RESP & Vito ORM | `packages/db/`, `packages/orm/` |
| **Edge WASM** | Adapter đóng gói chạy trên Cloudflare Workers / AWS Lambda | `packages/edge/edge.vit` |

---

## ⚡ Tự Viết Custom Middleware

Bạn có thể dễ dàng viết Middleware cho riêng mình:

```javascript
app.use((req: Request, res: Response, next: Function) => {
    let authHeader = req.header("Authorization");
    if (authHeader == "") {
        res.setStatus(401).json("{\"error\":\"Unauthorized access\"}");
        return; // Dừng pipeline không gọi next()
    }
    next(); // Chuyển sang middleware / handler tiếp theo
});
```
