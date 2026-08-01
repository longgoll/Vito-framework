---
layout: home

hero:
  name: "Vito Framework"
  text: "Ultra-Fast Web Framework for VIT"
  tagline: "High-performance, lightweight, and expressive Web Server Framework built on the native VIT Language runtime."
  image:
    src: /logo.svg
    alt: Vito Framework Logo
  actions:
    - theme: brand
      text: Get Started 🚀
      link: /en/guide/getting-started
    - theme: alt
      text: API Reference 📚
      link: /en/api/overview
    - theme: alt
      text: View on GitHub 🐙
      link: https://github.com/vit-lang/vito

features:
  - icon: ⚡
    title: Fast HTTP Request Engine
    details: Zero-dependency parser for GET, POST, PUT, DELETE with automatic query parameters, header, and payload body extraction.
  - icon: 🎯
    title: Dynamic Routing & Grouping
    details: Segment matching (:id), multi-level path parameters, and modular prefix grouping with app.group().
  - icon: 🔗
    title: Rich Middleware Ecosystem
    details: Official chainable plugins for CORS, Logger, Static Files, Rate Limiting, JWT Auth, SSE, and Security Headers.
  - icon: 🔒
    title: Native TLS 1.3 & Protocols
    details: Built-in TLS encryption, HTTP/2 multiplexing, and HTTP/3 QUIC (UDP) transport engine.
  - icon: 🌐
    title: Edge Runtime & WASM
    details: Serverless WebAssembly adapter for Cloudflare Workers & AWS Lambda with < 1ms cold start time.
  - icon: 🛠 Developer Tooling & CLI
    details: Hot Module Reloading (vit dev), project scaffolding (vit create), and single binary compilation (< 5MB).
---

<div class="code-showcase" style="margin-top: 3rem; text-align: center;">

## Expressive & Intuitive Syntax 🚀

Building high-performance Web Applications has never been so clean and pleasant in the **VIT** ecosystem.

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";

function main(): number {
    let app = createApp();

    // 1. Register Global Middlewares
    app.use(corsMiddleware);
    app.use(loggerMiddleware);

    // 2. Dynamic Route Parameter (/users/:id)
    app.get("/users/:id", (req: Request, res: Response) => {
        let userId = req.param("id");
        res.json("{\"status\":\"ok\", \"user_id\":\"" + userId + "\"}");
    });

    // 3. Route Grouping (/api/v1)
    app.group("/api/v1", (v1) => {
        v1.get("/products/:category/:id", (req: Request, res: Response) => {
            let cat = req.param("category");
            let pId = req.param("id");
            res.json("{\"category\":\"" + cat + "\",\"id\":\"" + pId + "\"}");
        });
    });

    // 4. Start HTTP Server
    app.listen(8888);
    return 0;
}
```

</div>
