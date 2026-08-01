# Middleware System 🔗

**Middleware** functions intercept incoming HTTP requests before reaching final route handlers.

---

## 🔗 Official Middlewares Usage

Vito provides official middleware packages located in `packages/`:

```javascript
import { createApp } from "vito/src/vito.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { createStaticMiddleware } from "vito/packages/static/static.vit";
import { securityHeadersMiddleware, createRateLimiter } from "vito/packages/security/security.vit";

function main(): number {
    let app = createApp();

    // 1. Log HTTP requests
    app.use(loggerMiddleware);

    // 2. Enable CORS & OPTIONS preflight
    app.use(corsMiddleware);

    // 3. Security Headers & Rate Limiting
    app.use(securityHeadersMiddleware);
    app.use(createRateLimiter(100, 60)); // Max 100 req/min

    // 4. Serve Static Files from "public" directory
    app.use(createStaticMiddleware("./public"));

    return 0;
}
```

---

## 🛡 Official Package List

| Package | Purpose | Path |
| :--- | :--- | :--- |
| **CORS** | Cross-Origin Resource Sharing & OPTIONS Preflight | `packages/cors/cors.vit` |
| **Logger** | HTTP request logger in console | `packages/logger/logger.vit` |
| **Static** | Serve static files (HTML, CSS, JS, Images, Fonts) | `packages/static/static.vit` |
| **Upload** | Streaming multipart/form-data parser & file size validator | `packages/upload/upload.vit` |
| **Session** | HMAC signed/encrypted cookies & session management | `packages/session/session.vit` |
| **Health** | Active connection tracking & Kubernetes health probes (`/healthz`, `/readyz`) | `packages/health/health.vit` |
| **Validation** | Type-safe schema validation (HTTP 422) | `packages/validation/validation.vit` |
| **Security** | Helmet security headers, Rate Limiting (429) & JWT Guard (401) | `packages/security/security.vit` |
| **Swagger** | Interactive Swagger UI (`/docs`) & OpenAPI generator | `packages/swagger/swagger.vit` |
| **SSE Stream** | Server-Sent Events for Response Streaming (AI/LLM) | `packages/sse/sse.vit` |
| **TLS 1.3** | Direct HTTPS TLS 1.3 encryption without Nginx | `packages/tls/tls.vit` |
| **Database/ORM**| PostgreSQL, SQLite, Redis RESP drivers & Vito ORM | `packages/db/`, `packages/orm/` |
| **Edge WASM** | Cloudflare Workers & AWS Lambda serverless adapter | `packages/edge/edge.vit` |

---

## ⚡ Custom Middleware

You can write custom middleware functions easily:

```javascript
app.use((req: Request, res: Response, next: Function) => {
    let authHeader = req.header("Authorization");
    if (authHeader == "") {
        res.setStatus(401).json("{\"error\":\"Unauthorized access\"}");
        return; // Stop execution, do not call next()
    }
    next(); // Pass control to next handler
});
```
