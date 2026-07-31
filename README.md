# Vito Web Framework 🚀

> High-performance, lightweight, expressive Web Server Framework for the **VIT Language ecosystem**.

## 🌟 Overview
**Vito** is an open-source, modern Web Framework built on top of the native **VIT Compiler runtime**. Designed for simplicity, speed, and DX excellence.

---

## 🛠 Features

- ⚡ **HTTP Request Parser**: Automatically parses HTTP Methods (`GET`, `POST`, `PUT`, `DELETE`), Path, Query parameters, and Body content.
- 🛣 **Flexible Router**: Intuitive route registration with `.get()`, `.post()`, `.put()`, `.delete()`.
- 🔗 **Middleware Pipeline**: Chainable middleware execution (`app.use()`) for logging, CORS, authentication, and preflight headers.
- 📦 **Request & Response Context**: Built-in methods for JSON (`res.json()`), HTML (`res.html()`), text (`res.send()`), status codes (`res.setStatus(code)`), and header manipulation (`res.setHeader()`).
- 🚨 **Automatic Error Handling**: Pre-configured `404 Not Found` JSON responses for unregistered routes.

---

## 📁 Repository Architecture (Monorepo)

```text
vito/
├── vit.json            # Framework package config
├── README.md           # Documentation
├── src/                # Core Web Framework Engine
│   └── vito.vit        # HTTP Parser, Response Context, Router & App Engine
├── packages/           # Official Middleware Plugins
│   ├── cors/           # CORS header & OPTIONS preflight middleware
│   └── logger/         # Request logger middleware
└── examples/           # Web app examples
    ├── app_demo.vit    # Complete router & middleware app demo
    └── standalone_demo.vit
```

---

## 🚀 Quick Start

### 1. Basic Web Server Example

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";

function main(): number {
    let app = createApp();

    // Register Middlewares
    app.use(corsMiddleware);
    app.use(loggerMiddleware);

    // Register Routes
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    app.get("/api/status", (req: Request, res: Response) => {
        res.json("{\"status\":\"ok\",\"engine\":\"VIT Native\"}");
    });

    app.post("/api/data", (req: Request, res: Response) => {
        res.json("{\"received\":\"" + req.body + "\"}");
    });

    // Start Server
    app.listen(8080);
    return 0;
}
```

### 2. Run Application
```bash
vit run main.vit
```

---

## 📜 License
MIT License - Open-source & Free for the VIT Community.
