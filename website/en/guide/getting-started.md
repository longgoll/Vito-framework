# Quick Start Guide 🚀

Welcome to **Vito Framework** — high-performance, lightweight, and feature-rich Web Framework tailored for the **VIT** programming language.

::: tip 💡 Interactive Learning Experience
Use the 4-step interactive flow below to practice installation, coding, and running your Vito Web Server in **3 minutes**!
:::

<QuickStartFlow />

---

## ⚡ Key Features of Vito Framework

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4>Native Speed</h4>
    <p>Runs directly on the VIT Native Engine with Async I/O threading achieving 245K+ req/s.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🧩</div>
    <h4>Expressive DX</h4>
    <p>Familiar syntax close to Express / Fastify / Hono for instant developer adoption.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4>Zero-Config Live Reload</h4>
    <p>Compiles and automatically reloads the server within milliseconds upon saving files.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🛠️</div>
    <h4>Integrated CLI Tooling</h4>
    <p>Provides handy commands for project scaffolding (`vit init`), setup (`vit setup`), and building (`vit build`).</p>
  </div>
</div>

---

## 💻 Sample Project Source Code (`src/main.vit`)

Below is the complete structure of a production-ready Web API application written in **Vito**:

```typescript
import { Vito, Request, Response } from "vito";

function main(): number {
    // 1. Initialize Vito App Engine
    let app = Vito.new();

    // 2. Define HTML GET Route
    app.get("/", (req: Request, res: Response) => {
        res.html("""
            <!DOCTYPE html>
            <html>
            <head><title>Vito Web Server</title></head>
            <body style="font-family: sans-serif; text-align: center; padding: 50px;">
                <h1>⚡ Welcome to Vito Web Framework!</h1>
                <p>Native speed, zero overhead, modern DX.</p>
            </body>
            </html>
        """);
    });

    // 3. Define JSON API Health Check
    app.get("/api/v1/health", (req: Request, res: Response) => {
        res.json({
            status: "UP",
            engine: "Vit Native Phase 9",
            uptime_seconds: 3600
        });
    });

    // 4. Listen on port 3000
    app.listen(3000);
    print("⚡ Vito HTTP Server is listening at http://localhost:3000");
    return 0;
}
```

---

## 🛠 CLI Developer Cheat Sheet

| CLI Command | Action | Use Case |
| :--- | :--- | :--- |
| `vit setup` | Verify PATH environment & LLVM Toolchain | Run once after installation |
| `vit init <app-name>` | Automatically scaffold a standard Vito project | Initialize a new project |
| `vit dev` | Run development server with auto Live-Reload | Use during development |
| `vit build` | Package into optimized Native Binary | Use for production deployment |

---

## 🧩 Next Steps

Now you're ready to dive deeper into the Vito Framework ecosystem:

- 🎯 **[Routing & Route Params](/en/guide/routing)**: Learn how to handle URL Dynamic Parameters, Query Strings, and Route Grouping.
- 🧱 **[Middleware System](/en/guide/middleware)**: Build Logger, CORS, Rate Limiting, and Auth Guard.
- ⚡ **[Live Browser Playground](/playground)**: Experience writing and executing Vito code right inside your browser with WebAssembly!
