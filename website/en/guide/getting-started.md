# Getting Started with Vito 🚀

**Vito** is a high-performance, lightweight, and extensible Web Framework designed for the **VIT** programming language. The framework provides an excellent Developer Experience (DX) similar to Express, Fastify, or Hono in the JS ecosystem, running natively on top of the **VIT Compiler Runtime**.

---

## 🛠 1-Line Quick Installation

Install the complete **Vit Compiler Engine** and **Vito CLI** with a single command:

::: code-group

```bash [Linux / macOS]
curl -fsSL https://vit.dev/install.sh | bash
```

```powershell [Windows (PowerShell)]
iwr https://vit.dev/install.ps1 -useb | iex
```

:::

### 1. Prerequisites
- **VIT Compiler Engine**: Bundled automatically by the installer.
- **Supported OS**: Windows (AMD64 / AVX2), Linux (x86_64 / arm64), macOS (Apple Silicon / Intel).

### 2. Scaffold a Project in 5 Seconds
Using the official `vito CLI`:

```bash
vito create my-web-app
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
