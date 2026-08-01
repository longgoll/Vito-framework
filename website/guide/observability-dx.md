# Metrics & DX Observability 📊

**Vito Framework** tích hợp sẵn hệ thống quan sát thông số hệ thống (**Observability Suite**), tự động xuất mét dữ liệu chuẩn **Prometheus**, định vết **OpenTelemetry**, kiểm tra sức khỏe Kubernetes Probes và tự động sinh tài liệu **Swagger UI**.

---

## 📊 1. Prometheus Metrics & OpenTelemetry Exporter (`packages/metrics`)

Tự động theo dõi các chỉ số vận hành hệ thống thực tế và xuất ra endpoint `/metrics`:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📈</div>
    <h4>HTTP Throughput & Latency</h4>
    <p>Theo dõi tổng số requests/giây (RPS), phân bố thời gian phản hồi (p50, p95, p99 histogram).</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">💾</div>
    <h4>Memory & Connection Pool</h4>
    <p>Giám sát dung lượng RAM Heap sử dụng, số lượng kết nối CSDL active/idle trong Pool.</p>
  </div>
</div>

```typescript
import { createPrometheusExporter } from "vito/packages/metrics/metrics.vit";

let metrics = createPrometheusExporter();

// Xuất dữ liệu định dạng Prometheus text format tại /metrics
app.get("/metrics", (req: Request, res: Response) => {
    res.send(metrics.renderMetricsText());
});
```

---

## 📖 2. Auto Swagger UI & OpenAPI 3.0 (`packages/swagger`)

Tự động tạo tài liệu API tương tác tại đường dẫn `/docs` mà không cần viết file YAML thủ công:

```typescript
import { createSwaggerUI } from "vito/packages/swagger/swagger.vit";

let swagger = createSwaggerUI();

// Đăng ký thông tin API
swagger.setApiInfo("Vito Payment Gateway API", "v1.0.0", "Hệ thống thanh toán độ trễ siêu thấp");

// Tự động phục vụ giao diện Swagger UI tại /docs
app.use("/docs", swagger.middleware());
```

---

## 🩺 3. Kubernetes Probes & Graceful Shutdown (`packages/health`)

Đảm bảo ứng dụng tương thích hoàn hảo với môi trường Orchestration (Kubernetes, Docker Swarm):

::: code-group

```typescript [1. Health Probes (/healthz & /readyz)]
import { createHealthChecker } from "vito/packages/health/health.vit";

let health = createHealthChecker();

// Liveness Probe (Kiểm tra Process sống)
app.get("/healthz", (req, res) => {
    res.json(health.getLivenessStatus());
});

// Readiness Probe (Kiểm tra kết nối CSDL & Redis)
app.get("/readyz", (req, res) => {
    let isReady = health.checkDatabaseConnection() && health.checkRedisConnection();
    res.setStatus(isReady ? 200 : 503).json({ ready: isReady });
});
```

```typescript [2. Graceful Shutdown]
// Tự động ngắt kết nối an toàn khi nhận tín hiệu SIGTERM / SIGINT
health.onGracefulShutdown(() => {
    print("🛑 Đang đóng kết nối CSDL và xử lý nốt các HTTP Requests dở dang...");
    pool.closeAll();
});
```

:::
