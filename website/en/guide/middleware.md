# Middleware System 🔗

**Middleware** in **Vito Framework** acts as interceptors placed between the HTTP Request processing flow and the main Route Handler.

The Vito ecosystem ships with **27 Official Packages** that are Production-Ready without needing to install external libraries.

---

## 🌊 Request Flow Through the Middleware Pipeline

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

## 📦 27 Official Middleware & Packages

Vito bundles all modules inside the `packages/` directory:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🔒</div>
    <h4>Security Suite</h4>
    <p>Helmet HTTP headers, Anti-CSRF, Rate Limiting (429) & XSS Sanitizer.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📝</div>
    <h4>Logger & Tracer</h4>
    <p>Prints logs in Pretty Console or Structured JSON format with Correlation ID.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📁</div>
    <h4>Static Server</h4>
    <p>Serves static files at high speed with ETag, Cache-Control, Gzip/Brotli buffering.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📤</div>
    <h4>Stream Uploader</h4>
    <p>Parses `multipart/form-data` as a stream directly with zero RAM overhead.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📖</div>
    <h4>Swagger OpenAPI</h4>
    <p>Automatically generates `openapi.json` API docs & Swagger UI at `/docs`.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🌊</div>
    <h4>SSE AI Streamer</h4>
    <p>Dedicated Server-Sent Events middleware for AI/LLM streaming token responses.</p>
  </div>
</div>

---

## 💻 Production-Ready Pipeline Setup Example

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

    // 2. Enable CORS headers & OPTIONS preflight
    app.use(corsMiddleware);

    // 3. Security HTTP Headers (Helmet style) & Rate Limit to 100 req/min
    app.use(securityHeadersMiddleware);
    app.use(createRateLimiter(100, 60));

    // 4. Serve static assets from "./public"
    app.use(createStaticMiddleware("./public"));

    return 0;
}
```

---

## ⚡ Writing Custom Middleware

You can create custom middleware with just a few lines of code:

::: code-group

```typescript [1. Custom Auth Guard]
// Middleware that checks the Authorization header
app.use((req: Request, res: Response, next: Function) => {
    let token = req.header("Authorization");
    
    if (token == "") {
        res.setStatus(401).json({ error: "Auth Token required" });
        return; // Break the pipeline
    }
    
    next(); // Pass to the next middleware
});
```

```typescript [2. Performance Execution Timer]
// Middleware that measures request processing time
app.use((req: Request, res: Response, next: Function) => {
    let start = Date.now();
    next();
    let duration = Date.now() - start;
    print("⏱️ Request " + req.path() + " handled in " + duration + "ms");
});
```

:::

---

## 📊 Middleware Package Reference Table

| Package Name | File Path | Main Purpose |
| :--- | :--- | :--- |
| **CORS** | `packages/cors/cors.vit` | Cross-Origin & Preflight handling |
| **Logger** | `packages/logger/logger.vit` | System request logging |
| **Static** | `packages/static/static.vit` | Static file serving (Images/JS/CSS) |
| **Upload** | `packages/upload/upload.vit` | Multipart file upload parsing |
| **Session** | `packages/session/session.vit` | Cookie & Encrypted Session management |
| **Health** | `packages/health/health.vit` | Kubernetes Probes (`/healthz`, `/readyz`) |
| **Validation** | `packages/validation/validation.vit` | Request body schema validation |
| **Swagger** | `packages/swagger/swagger.vit` | Auto-generate Swagger UI API Docs |
| **SSE Stream** | `packages/sse/sse.vit` | AI LLM Token Streaming |
