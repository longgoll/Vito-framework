# Hệ Thống Middleware 🔗

**Middleware** trong **Vito Framework** đóng vai trò là các trạm can thiệp (Interceptors) nằm giữa luồng xử lý HTTP Request và Route Handler chính.

Hệ sinh thái Vito đi kèm **27 Official Packages** sẵn sàng đưa vào sản xuất (Production-Ready) mà không cần cài đặt thêm thư viện bên ngoài.

---

## 🌊 Luồng Xử Lý Request qua Pipeline Middleware

<div class="pipeline-flow">
  <div class="pipeline-step">📡 HTTP Request</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">🔒 Security & Rate Limit</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">📝 Logger Middleware</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">🌐 CORS & Preflight</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">🔑 Auth & JWT Guard</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">🎯 Route Handler</div>
  <span class="pipeline-arrow">➔</span>
  <div class="pipeline-step">⚡ HTTP Response</div>
</div>

---

## 📦 Danh Sách 27 Official Middleware & Packages

Vito tích hợp sẵn các gói mô-đun trong thư mục `packages/`:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🔒</div>
    <h4>Security Suite</h4>
    <p>Helmet HTTP headers, Anti-CSRF, Rate Limiting (429) & XSS Sanitizer.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📝</div>
    <h4>Logger & Tracer</h4>
    <p>In log định dạng Pretty Console hoặc Structured JSON với Correlation ID.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📁</div>
    <h4>Static Server</h4>
    <p>Phục vụ file tĩnh tốc độ cao với bộ đệm ETag, Cache-Control, Gzip/Brotli.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📤</div>
    <h4>Stream Uploader</h4>
    <p>Phân tích `multipart/form-data` dạng stream trực tiếp không tốn bộ nhớ RAM.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📖</div>
    <h4>Swagger OpenAPI</h4>
    <p>Tự động sinh tài liệu API `openapi.json` & Giao diện Swagger UI tại `/docs`.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🌊</div>
    <h4>SSE AI Streamer</h4>
    <p>Server-Sent Events chuyên biệt cho ứng dụng AI/LLM Streaming response token.</p>
  </div>
</div>

---

## 💻 Ví Dụ Khởi Tạo Pipeline Chuẩn Production

```typescript
import { Vito, Request, Response } from "vito";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";
import { createStaticMiddleware } from "vito/packages/static/static.vit";
import { securityHeadersMiddleware, createRateLimiter } from "vito/packages/security/security.vit";

function main(): number {
    let app = Vito.new();

    // 1. Log HTTP requests (Method, Path, Response Time)
    app.use(loggerMiddleware);

    // 2. Kích hoạt CORS headers & OPTIONS preflight
    app.use(corsMiddleware);

    // 3. Bảo mật HTTP Headers (Helmet style) & Giới hạn tần suất 100 req/phút
    app.use(securityHeadersMiddleware);
    app.use(createRateLimiter(100, 60));

    // 4. Phục vụ tài nguyên tĩnh từ thư mục "./public"
    app.use(createStaticMiddleware("./public"));

    return 0;
}
```

---

## ⚡ Tự Viết Custom Middleware

Bạn có thể tự tạo các Middleware tùy chỉnh chỉ với vài dòng code:

::: code-group

```typescript [1. Custom Auth Guard]
// Middleware kiểm tra Header Authorization
app.use((req: Request, res: Response, next: Function) => {
    let token = req.header("Authorization");
    
    if (token == "") {
        res.setStatus(401).json({ error: "Yêu cầu cung cấp Auth Token" });
        return; // Ngắt pipeline
    }
    
    next(); // Chuyển sang middleware kế tiếp
});
```

```typescript [2. Performance Execution Timer]
// Middleware đo thời gian xử lý request
app.use((req: Request, res: Response, next: Function) => {
    let start = Date.now();
    next();
    let duration = Date.now() - start;
    print("⏱️ Request " + req.path() + " xử lý trong " + duration + "ms");
});
```

:::

---

## 📊 Bảng Tra Cứu Gói Mô-Đun Middleware

| Tên Package | Đường dẫn file | Công dụng chính |
| :--- | :--- | :--- |
| **CORS** | `packages/cors/cors.vit` | Xử lý Cross-Origin & Preflight |
| **Logger** | `packages/logger/logger.vit` | Ghi log hệ thống |
| **Static** | `packages/static/static.vit` | Phục vụ File tĩnh (Images/JS/CSS) |
| **Upload** | `packages/upload/upload.vit` | Phân tích file tải lên (Multipart) |
| **Session** | `packages/session/session.vit` | Quản lý Cookie & Encrypted Session |
| **Health** | `packages/health/health.vit` | Kubernetes Probes (`/healthz`, `/readyz`) |
| **Validation** | `packages/validation/validation.vit` | Kiểm tra Schema request body |
| **Swagger** | `packages/swagger/swagger.vit` | Tự động tạo Swagger UI API Docs |
| **SSE Stream** | `packages/sse/sse.vit` | AI LLM Token Streaming |
