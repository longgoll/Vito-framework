# Getting Started with Vito 🚀

**Vito** is a high-performance, lightweight, and extensible Web Framework designed for the **VIT** programming language. The framework provides an excellent Developer Experience (DX) similar to Express, Fastify, or Hono in the JS ecosystem, running natively on top of the **VIT Compiler Runtime**.

---

## 🛠 Installation

### 1. Prerequisites
- **VIT Compiler Engine**: Version 1.0 or higher.
- **VIT Package Manager (`vit pm`)**: Official package manager tool.

### 2. Adding Vito to your project
Add `vito` to your project's `vit.json` configuration file:

```json
{
  "name": "my-web-app",
  "version": "1.0.0",
  "dependencies": {
    "vito": "^2.0.0"
  }
}
```

Or scaffold via CLI:
```bash
vit create my-web-app
cd my-web-app
```

---

## ⚡ Creating Your First HTTP Server

Create a `main.vit` file with the following contents:

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";

function main(): number {
    // 1. Initialize Vito App Engine
    let app = createApp();

    // 2. Define simple GET Route
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    // 3. Respond with JSON API
    app.get("/api/health", (req: Request, res: Response) => {
        res.json("{\"status\":\"UP\", \"uptime\":\"ok\"}");
    });

    // 4. Listen on port 8888
    app.listen(8888);
    return 0;
}
```

---

## 🚀 Running the Application

Use the `vit CLI` to start the application in development mode with **Hot Module Reloading (HMR)**:

```bash
vit dev
```

Visit `http://localhost:8888` in your browser to see your server in action! 🎉

---

## 🏗 Repository Structure (Monorepo)

Vito monorepo is structured as follows:

```text
vito/
├── vit.json            # Vito Package Config
├── src/                # Core HTTP Server Engine & Parser
│   └── vito.vit
├── packages/           # Official Middlewares & Extensions
│   ├── cors/           # CORS Middleware
│   ├── logger/         # Request Logger
│   ├── static/         # Static File Server
│   ├── validation/     # Schema Validator (HTTP 422)
│   ├── security/       # Rate Limiting & Auth Guard
│   ├── swagger/        # Swagger UI & OpenAPI Generator
│   ├── sse/            # Server-Sent Events for AI/LLM Streaming
│   ├── db/ & orm/      # Native DB Drivers & Vito ORM
│   └── edge/           # Cloud Edge Runtime WASM Adapter
└── examples/           # Real-world Example Apps
```
