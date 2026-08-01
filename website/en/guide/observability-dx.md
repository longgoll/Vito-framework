# Observability Metrics, OTel Tracing & DX Tooling 📊

**Vito Framework** features native real-time system observability (**Prometheus Metrics Exporter & OpenTelemetry Tracing**) alongside an ultra-fast in-memory testing framework and developer CLI tools (**CLI Generators & System Diagnostics**).

---

## 📈 Prometheus Metrics Exporter (`packages/metrics`)

The `packages/metrics/metrics.vit` module exposes a native `/metrics` endpoint compliant with standard Prometheus text exposition format, directly consumable by Prometheus scrapers and Grafana Dashboards.

### Collected Metrics:
- **HTTP Request Counters**: `http_requests_total{method="GET|POST", status="200|404|500"}`
- **Latency Summary Quantiles**: `http_request_duration_seconds` (p50, p90, p99 quantiles).
- **Process Memory & CPU**: `process_cpu_seconds_total` & `process_resident_memory_bytes`.

### Usage Example:

```typescript
import { createPrometheusRegistry } from "vito/packages/metrics/metrics.vit";

let metricsRegistry = createPrometheusRegistry();

// Record HTTP Request metrics
metricsRegistry.recordRequest("GET", "/api/v1/users", 200, 3);
metricsRegistry.recordRequest("POST", "/api/v1/users", 200, 12);

// Register /metrics endpoint middleware
app.use((req, res) => metricsRegistry.handleMetricsEndpoint(req, res));
```

---

## 🔗 OpenTelemetry (OTel) Tracing Integration

Vito supports W3C Trace Context headers (`traceparent`) to seamlessly correlate logs and trace spans across distributed microservices.

```typescript
import { extractTraceParent, injectTraceParent } from "vito/packages/metrics/metrics.vit";

// Extract Trace ID & Span ID from incoming request headers
let traceCtx = extractTraceParent(req.header("traceparent"));

// Inject Trace context into downstream requests
let outgoingHeader = injectTraceParent(traceCtx);
```

---

## 🧪 Testing Framework & In-Memory Injector (`packages/testing`)

The `packages/testing/testing.vit` package enables ultra-fast mock HTTP request execution directly in memory without binding real OS TCP sockets.

### Fast Simulation with `app.inject()`:
- Full support for Request Body, Headers, Query Strings, and Cookies.
- **Passes DoD Target**: Executes 100+ mock test cases in $< 1$ second.

### DB Test Fixture & Isolation:
- Automatic `SAVEPOINT` and `ROLLBACK` after each test execution to guarantee clean database isolation.

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

The `vit` CLI provides scaffolding code generators (`vit generate`) and interactive system diagnostics (`vit doctor`).

### Boilerplate Generator (`vit generate` / `vit g`):
- `vit g controller User`: Generates Controller structure.
- `vit g service User`: Generates Service business logic.
- `vit g middleware Auth`: Generates Custom Middleware.
- `vit g model User`: Generates Vito ORM Model.
- `vit g migration CreateUsers`: Generates SQL Migration template.

### Interactive Diagnostics (`vit doctor`):
Runs comprehensive checks verifying compiler environment, database connectivity, memory/CPU allocation, and package integrity.
