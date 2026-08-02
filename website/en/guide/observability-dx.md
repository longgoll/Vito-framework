# Developer Experience (DX) & Observability Suite 📊

**Vito Framework** includes a built-in **Developer Experience Suite** for smooth development, automatic input validation (**Schema Validation**), request lifecycle management (**Lifecycle Hooks**), **OpenAPI 3.0 & Swagger UI** doc generation, Client SDK generators, **Prometheus** metrics, and **OpenTelemetry** tracing.

---

## 🛡️ 1. Schema Validation Engine (`packages/validation`)

Validates data types and constraints of Request Query & Params with zero-copy memory layout. Automatically returns HTTP `422 Unprocessable Entity` when data is invalid:

```typescript
import { SchemaValidator } from "packages/validation/validation.vit";

let validator: SchemaValidator;
validator.init();
validator.addStringRule("name");
validator.addEmailRule("email");

app.get("/api/v1/users", (req: Request, res: Response) => {
    // Automatically validates req.query or req.param
    if (!validator.validateRequest(req, res)) {
        return; // Automatically returns 422 JSON on failure
    }

    res.json("{\"status\":\"success\",\"message\":\"Request validated!\"}");
});
```

---

## 🔄 2. Request Lifecycle Hooks & Custom Error Handlers (`vito.vit`)

Vito supports flexible interceptor points throughout the lifecycle of every HTTP Request:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚓</div>
    <h4>beforeHandle Hook</h4>
    <p>Runs before entering the main handler. Allows Auth checks, header injection, or early request blocking.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">✨</div>
    <h4>afterHandle Hook</h4>
    <p>Runs immediately after a successful handler response for logging or adding response metadata.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🚨</div>
    <h4>onError Hook</h4>
    <p>Centrally catches and handles all exceptions raised during request processing.</p>
  </div>
</div>

```typescript
// 1. Pre-execution hook
app.beforeHandle((req: Request, res: Response) => {
    res.setHeader("X-Powered-By", "Vito-Native-Engine");
    return true; // Return true to continue the processing chain
});

// 2. Post-execution hook
app.afterHandle((req: Request, res: Response) => {
    print("Request executed successfully: " + req.path);
});

// 3. Global Error Handler
app.onError((req: Request, res: Response) => {
    res.setStatus(500.0).json("{\"error\":\"Internal Error\",\"path\":\"" + req.path + "\"}");
});
```

---

## 📖 3. Dynamic OpenAPI 3.0 Spec & Swagger UI Generator (`packages/swagger`)

Automatically initializes and serves OpenAPI 3.0 docs at `/openapi.json` and interactive Swagger UI at `/docs`:

```typescript
import { OpenAPISpec } from "packages/swagger/swagger.vit";

let spec: OpenAPISpec;
spec.init("Vito Payment Gateway API", "v1.0.0");

// Register endpoint documentation
spec.addRouteDoc("/api/v1/users", "get", "List all registered users with validation");
spec.addRouteDoc("/api/v1/products", "post", "Create new product item");

// Serve OpenAPI 3.0 JSON spec
app.get("/openapi.json", (req: Request, res: Response) => {
    res.json(spec.generateJSON());
});
```

---

## 📦 4. Type-Safe Client SDK Code Generator (`packages/cli`)

Automatically generates type-safe Client SDK source code from backend config via the CLI toolkit:

```typescript
import { VitCodeGenerator } from "packages/cli/cli.vit";

let cliGen: VitCodeGenerator;
cliGen.generatedCount = 0.0;

// Generate Client SDK to communicate with the Vito App
let sdkCode = cliGen.generateEdenClient("VitoUserApp");
print(sdkCode);
```

**Auto-generated Client SDK sample:**
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

Monitor real-time operational metrics and export Prometheus data at `/metrics`:

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

Ensures the application is fully compatible with Orchestration environments (Kubernetes, Docker Swarm):

```typescript
import { createTestDbFixture } from "packages/testing/testing.vit";

let db = createTestDbFixture("vito_production_db");
db.beginIsolationTransaction();
```
