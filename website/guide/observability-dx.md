# Giám Sát Metrics, Tracing & Công Cụ DX 📊

**Vito Framework** tích hợp sẵn khả năng giám sát hệ thống thời gian thực (**Prometheus Metrics Exporter & OpenTelemetry Tracing**) cùng bộ công cụ kiểm thử in-memory siêu tốc và công cụ dòng lệnh (**CLI Generators & Diagnostics**) giúp tối ưu trải nghiệm phát triển (DX).

---

## 📈 Prometheus Metrics Exporter (`packages/metrics`)

Module `packages/metrics/metrics.vit` cung cấp endpoint `/metrics` chuẩn định dạng Prometheus text-based format, phục vụ trực tiếp cho Grafana Dashboard và Prometheus Scraper.

### Chỉ Số Thu Thập Chi Tiết:
- **HTTP Request Counters**: `http_requests_total{method="GET|POST", status="200|404|500"}`
- **Latency Histogram Summary**: `http_request_duration_seconds` với phân vị quantiles p50, p90, p99.
- **Process Memory & CPU**: `process_cpu_seconds_total` & `process_resident_memory_bytes`.

### Ví Dụ Sử Dụng:

```typescript
import { createPrometheusRegistry } from "vito/packages/metrics/metrics.vit";

let metricsRegistry = createPrometheusRegistry();

// Ghi nhận HTTP Request
metricsRegistry.recordRequest("GET", "/api/v1/users", 200, 3);
metricsRegistry.recordRequest("POST", "/api/v1/users", 200, 12);

// Đăng ký endpoint /metrics trong middleware
app.use((req, res) => metricsRegistry.handleMetricsEndpoint(req, res));
```

---

## 🔗 OpenTelemetry (OTel) Tracing Integration

Vito hỗ trợ chuẩn truyền dẫn Trace Context W3C (`traceparent` HTTP Header), giúp liên kết log và vết xử lý dữ liệu qua các microservices.

```typescript
import { extractTraceParent, injectTraceParent } from "vito/packages/metrics/metrics.vit";

// Trích xuất Trace ID & Span ID từ request header
let traceCtx = extractTraceParent(req.header("traceparent"));

// Truyền tiếp Trace context sang microservice downstream
let outgoingHeader = injectTraceParent(traceCtx);
```

---

## 🧪 Testing Framework & In-Memory Injector (`packages/testing`)

Gói `packages/testing/testing.vit` cho phép thực thi test suite siêu tốc trực tiếp trên bộ nhớ mà không cần mở cổng socket TCP thực tế trên hệ điều hành.

### Giả Lập Fast Request với `app.inject()`:
- Hỗ trợ đẩy đủ Body, Request Headers, Query String và Cookies.
- **Đạt tiêu chí DoD**: Chạy 100+ test cases trong $< 1$ giây.

### DB Test Fixture & Isolation:
- Tự động thực thi `SAVEPOINT` và `ROLLBACK` sau mỗi lượt test case để đảm bảo trạng thái CSDL luôn sạch.

```typescript
import { createMockHttpEngine, createTestDbFixture } from "vito/packages/testing/testing.vit";

let mockEngine = createMockHttpEngine();
let res = mockEngine.inject(app, "GET", "/healthz", "Accept: application/json", "");

print("HTTP Status: " + res.statusCode);
print("Response Body: " + res.json());

// DB Isolation Test
let dbFixture = createTestDbFixture("UserTests");
dbFixture.beginIsolationTransaction();
dbFixture.seedFixture("users", "{\"id\":\"usr_100\",\"email\":\"dev@vito.dev\"}");
dbFixture.rollbackIsolationTransaction();
```

---

## 🛠 CLI Code Generators & Diagnostics (`packages/cli`)

Công cụ `vit` CLI hỗ trợ tự động sinh mã nguồn chuẩn kiến trúc (Scaffolding Generator) và chẩn đoán môi trường hệ thống (`vit doctor`).

### Lệnh Tự Động Sinh Mã (`vit generate` / `vit g`):
- `vit g controller User`: Sinh Controller xử lý tài nguyên `User`.
- `vit g service User`: Sinh Service logic nghiệp vụ.
- `vit g middleware Auth`: Sinh Custom Middleware.
- `vit g model User`: Sinh Vito ORM Model.
- `vit g migration CreateUsers`: Sinh file SQL Auto-Migration.

### Lệnh Chẩn Đoán Hệ Thống (`vit doctor`):
Kích hoạt kiểm tra toàn diện cấu hình môi trường, phiên bản CSDL, tài nguyên CPU/RAM và các package trong monorepo.
