# Vito Web Framework 🚀

> High-performance, lightweight, expressive Web Server Framework for the **VIT Language ecosystem**.

## 🌟 Overview
**Vito** is an open-source, modern Web Framework built on top of the native **VIT Compiler runtime**. Designed for simplicity, speed, and DX excellence.

---

## 🛠 Features

- ⚡ **HTTP Request Parser**: Automatically parses HTTP Methods (`GET`, `POST`, `PUT`, `DELETE`), Path, Query parameters, and Body content.
- 🎯 **Dynamic Route Parameters (`:id`)**: Support for parameterized routes (e.g. `/users/:id`, `/products/:category/:id`) with `req.param(key)`.
- 🔍 **Query & Header Helpers**: Fast access to URL search query parameters with `req.query(key)` and request headers with `req.header(key)`.
- 🛣 **Expressive Router**: Route registration with `.get()`, `.post()`, `.put()`, `.delete()`.
- 👥 **Route Grouping**: Modular route prefixing with `app.group("/api/v1", (v1) => { v1.get("/users", ...); })`.
- 🎨 **Custom Error Handling**: Customize 404 Not Found & error handlers using `app.setNotFoundHandler(...)`.
- 🔗 **Middleware Pipeline**: Chainable middleware execution (`app.use()`) for CORS, request logging, and static file serving.
- 📁 **Static File Middleware**: Serve static HTML/CSS/JS/Images directly from disk via `createStaticMiddleware(publicDir)`.
- 📦 **Response Context**: Built-in methods for JSON (`res.json()`), HTML (`res.html()`), text (`res.send()`), status codes (`res.setStatus(code)`), and headers (`res.setHeader()`).

---

## 📁 Repository Architecture (Monorepo)

```text
vito/
├── vit.json            # Framework package config
├── README.md           # Documentation
├── src/                # Core Web Framework Engine
│   └── vito.vit        # HTTP Parser, Dynamic Matcher, Route Grouping & App Engine
├── packages/           # Official Middleware Plugins
│   ├── cors/           # CORS header & OPTIONS preflight middleware
│   ├── health/         # Connection tracking & Kubernetes health probes (/healthz, /readyz)
│   ├── logger/         # Request logger middleware
│   ├── session/        # Signed/Encrypted cookies & session management
│   ├── static/         # Static file serving middleware
│   └── upload/         # Multipart form data streaming & file validation engine
└── examples/           # Web app examples
    ├── app_demo.vit    # Complete router, dynamic params, grouping & middleware demo
    └── standalone_demo.vit
```

---

## 🚀 Quick Start

```javascript
import { createApp, Request, Response } from "vito/src/vito.vit";
import { loggerMiddleware } from "vito/packages/logger/logger.vit";
import { corsMiddleware } from "vito/packages/cors/cors.vit";

function main(): number {
    let app = createApp();

    // 1. Register Middlewares
    app.use(corsMiddleware);
    app.use(loggerMiddleware);

    // 2. Custom Error Handler (404)
    app.setNotFoundHandler((req: Request, res: Response) => {
        res.setStatus(404).html("<h1>404 Not Found 😢</h1>");
    });

    // 3. Dynamic Route Parameter (/users/:id)
    app.get("/users/:id", (req: Request, res: Response) => {
        let userId = req.param("id");
        res.json("{\"user_id\":\"" + userId + "\"}");
    });

    // 4. Query Parameter (/search?q=vit)
    app.get("/search", (req: Request, res: Response) => {
        let keyword = req.query("q");
        res.json("{\"search_query\":\"" + keyword + "\"}");
    });

    // 5. Route Grouping (/api/v1)
    app.group("/api/v1", (v1) => {
        v1.get("/products/:category/:id", (req: Request, res: Response) => {
            let cat = req.param("category");
            let pId = req.param("id");
            res.json("{\"category\":\"" + cat + "\",\"id\":\"" + pId + "\"}");
        });
    });

    // 6. Start Server
    app.listen(8888);
    return 0;
}
```

---

## 📜 License
MIT License - Open-source & Free for the VIT Community.
