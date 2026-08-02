# Developer Experience (DX) & Observability Suite 📊

**Vito Framework** tích hợp sẵn bộ công cụ lập trình mượt mà (**Developer Experience Suite**), tự động hóa kiểm tra dữ liệu đầu vào (**Schema Validation**), quản lý chu kỳ sống request (**Lifecycle Hooks**), sinh tài liệu **OpenAPI 3.0 & Swagger UI**, bộ tạo SDK Client, mét dữ liệu **Prometheus** và định vết **OpenTelemetry**.

---

## 🛡️ 1. Schema Validation Engine (`packages/validation`)

Kiểm tra kiểu dữ liệu và ràng buộc của Request Query & Params với zero-copy memory layout. Tự động trả về HTTP `422 Unprocessable Entity` khi dữ liệu không hợp lệ:

```typescript
import { SchemaValidator } from "packages/validation/validation.vit";

let validator: SchemaValidator;
validator.init();
validator.addStringRule("name");
validator.addEmailRule("email");

app.get("/api/v1/users", (req: Request, res: Response) => {
    // Tự động kiểm tra req.query hoặc req.param
    if (!validator.validateRequest(req, res)) {
        return; // Tự động trả về 422 JSON nếu thất bại
    }

    res.json("{\"status\":\"success\",\"message\":\"Request validated!\"}");
});
```

---

## 🔄 2. Request Lifecycle Hooks & Custom Error Handlers (`vito.vit`)

Vito hỗ trợ các điểm chặn (interceptors) linh hoạt xuyên suốt chu kỳ sống của một HTTP Request:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚓</div>
    <h4>beforeHandle Hook</h4>
    <p>Chạy trước khi vào handler chính. Cho phép kiểm tra Auth, inject headers hoặc chặn sớm request.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">✨</div>
    <h4>afterHandle Hook</h4>
    <p>Chạy ngay sau khi handler phản hồi thành công để ghi log hoặc bổ sung thông số phản hồi.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🚨</div>
    <h4>onError Hook</h4>
    <p>Bắt và xử lý tập trung mọi ngoại lệ phát sinh trong quá trình xử lý request.</p>
  </div>
</div>

```typescript
// 1. Hook trước khi xử lý (Pre-execution hook)
app.beforeHandle((req: Request, res: Response) => {
    res.setHeader("X-Powered-By", "Vito-Native-Engine");
    return true; // Trả về true để tiếp tục chuỗi xử lý
});

// 2. Hook sau khi xử lý (Post-execution hook)
app.afterHandle((req: Request, res: Response) => {
    print("Request executed successfully: " + req.path);
});

// 3. Xử lý lỗi tập trung (Global Error Handler)
app.onError((req: Request, res: Response) => {
    res.setStatus(500.0).json("{\"error\":\"Internal Error\",\"path\":\"" + req.path + "\"}");
});
```

---

## 📖 3. Dynamic OpenAPI 3.0 Spec & Swagger UI Generator (`packages/swagger`)

Tự động khởi tạo và phục vụ tài liệu chuẩn OpenAPI 3.0 tại `/openapi.json` và giao diện tương tác Swagger UI tại `/docs`:

```typescript
import { OpenAPISpec } from "packages/swagger/swagger.vit";

let spec: OpenAPISpec;
spec.init("Vito Payment Gateway API", "v1.0.0");

// Đăng ký tài liệu cho endpoint
spec.addRouteDoc("/api/v1/users", "get", "List all registered users with validation");
spec.addRouteDoc("/api/v1/products", "post", "Create new product item");

// Xuất chuỗi JSON OpenAPI 3.0
app.get("/openapi.json", (req: Request, res: Response) => {
    res.json(spec.generateJSON());
});
```

---

## 📦 4. Type-Safe Client SDK Code Generator (`packages/cli`)

Tự động sinh mã nguồn Client SDK type-safe từ cấu hình backend thông qua bộ công cụ CLI:

```typescript
import { VitCodeGenerator } from "packages/cli/cli.vit";

let cliGen: VitCodeGenerator;
cliGen.generatedCount = 0.0;

// Sinh ra Client SDK để giao tiếp với Vito App
let sdkCode = cliGen.generateEdenClient("VitoUserApp");
print(sdkCode);
```

**Mẫu Client SDK tự động sinh ra:**
```typescript
import { fetchUrl } from "std/http";

struct VitoUserAppClient {
    baseUrl: string,
    function init(url: string): VitoUserAppClient {
        this.baseUrl = url;
        return this;
    },
    function getStatus(): string {
        return fetchUrl(this.baseUrl + "/api/v1/status");
    },
    function getUser(id: string): string {
        return fetchUrl(this.baseUrl + "/users/" + id);
    }
}
```

---

## 📊 5. Prometheus Metrics & OpenTelemetry Exporter (`packages/metrics`)

Theo dõi các chỉ số vận hành hệ thống thực tế và xuất dữ liệu Prometheus tại `/metrics`:

```typescript
import { createPrometheusRegistry } from "packages/metrics/metrics.vit";

let metrics = createPrometheusRegistry();
metrics.recordRequest("GET", "/api/v1/users", 200.0, 1.25);

app.get("/metrics", (req: Request, res: Response) => {
    res.send(metrics.renderPrometheusMetrics());
});
```

---

## 🩺 6. Kubernetes Probes & Graceful Shutdown (`packages/health`)

Đảm bảo ứng dụng tương thích hoàn hảo với môi trường Orchestration (Kubernetes, Docker Swarm):

```typescript
import { createTestDbFixture } from "packages/testing/testing.vit";

let db = createTestDbFixture("vito_production_db");
db.beginIsolationTransaction();
```
