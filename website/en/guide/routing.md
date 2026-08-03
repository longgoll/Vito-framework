# Routing & Route Parameters 🎯

The **Router Engine** of **Vito Framework** is designed around a high-performance **3-Pass Flat-Unrolled Engine (Phase 9 Perf)** architecture, combining zero-allocation arena slot lookups with automatic route precedence sorting.

<RouteVisualizer />

---

## 🧠 0. How It Works: 3-Pass Flat-Unrolled Engine

Vito Router utilizes a **Flat-Unrolled Registry (32 Fixed Slots)** combined with a **Memory Arena Slot Collector** for maximum performance and zero-allocation execution:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Zero-Allocation Matching</h4>
    <p>Route matching reuses recycled Arena Slots — eliminating heap allocations during request handling.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">♻️</div>
    <h4>Flat-Unrolled Memory Layout</h4>
    <p>32 route slots and 8 middleware slots are unrolled directly inside the VitoApp struct for optimal CPU cache locality.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔢</div>
    <h4>3-Pass Precedence Engine</h4>
    <p>Automatically classifies routes into 3 priority passes (Static -> Parametric -> Wildcard) at registration time.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🌿</div>
    <h4>O(1) Memory Arena Access</h4>
    <p>Manages request slot lifecycles via bitmask arena allocation, released instantly when request finishes.</p>
  </div>
</div>

### 🏆 Route Matching Precedence (3-Pass Engine)

When multiple routes can match the same URL, Vito Router performs **3 sequential passes** in priority order:

| Pass | Route Type | Classification | Example |
| :---: | :--- | :--- | :--- |
| **Pass 1** | Static Route | No `:` or `*` | `/users/profile` |
| **Pass 2** | Parametric / Optional | Has `:param` or `:slug?` | `/users/:id`, `/posts/:cat?` |
| **Pass 3** | Wildcard | Has `*name` | `/files/*path` |

```typescript
// ✅ Register in any order — the engine auto-classifies and prioritizes correctly
app.get("/users/:id",     userHandler);     // ← registered first
app.get("/users/profile", profileHandler);  // ← registered second

// Matching results (engine auto-prioritizes Static over Param):
// GET /users/profile  → profileHandler  ✓ (Pass 1 — Static wins)
// GET /users/42       → userHandler     ✓ (Pass 2 — Param)
```

::: tip 💡 Automatic Route Type Classification
`addRoute()` calls `getRouteType()` at registration time, storing the type in `rN_type` (0/1/2). No manual ordering required.
:::

### ⚙️ Trailing Slash Policy

Vito Router supports two trailing slash handling modes:

::: code-group

```typescript [Lax Mode — Default]
// createApp() defaults to Lax: automatically strips trailing "/"
// /users/ → normalized to /users automatically
let app = createApp(); // isStrictSlash = false

app.get("/users", handler);
// Both /users AND /users/ match this route ✓
```

```typescript [Strict Mode]
// createStrictApp(): /users and /users/ are two distinct routes
let app = createStrictApp(); // isStrictSlash = true

app.get("/users",  handlerA); // Only matches /users
app.get("/users/", handlerB); // Only matches /users/
```

:::

---

## ⚡ 1. All HTTP Methods

Vito supports all **RFC-standard** HTTP Methods out of the box:

<div class="card-grid">
  <div class="feature-mini-card">
    <span class="method-badge get">GET</span>
    <h4>Query Data</h4>
    <p>Retrieve resources or render HTML. Must not change server state.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge post">POST</span>
    <h4>Create Resource</h4>
    <p>Accept Form or JSON Body to create a new entity.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge put">PUT</span>
    <h4>Full Update</h4>
    <p>Completely overwrite a resource. Idempotent per HTTP spec.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge delete">DELETE</span>
    <h4>Delete Resource</h4>
    <p>Permanently remove a resource. Idempotent per HTTP spec.</p>
  </div>
</div>

::: code-group

```typescript [Core CRUD]
import { createApp, Request, Response } from "vito";

let app = createApp();

app.get(   "/users",     (req, res) => { res.send("GET    — List users"); });
app.post(  "/users",     (req, res) => { res.send("POST   — Create user"); });
app.put(   "/users/:id", (req, res) => { res.send("PUT    — Full update user"); });
app.delete("/users/:id", (req, res) => { res.send("DELETE — Remove user"); });
```

```typescript [PATCH, HEAD, OPTIONS]
// PATCH: Partial resource update (RFC 5789)
app.patch("/users/:id", (req: Request, res: Response) => {
    let id = req.param("id");
    res.json("{\"message\": \"Partially updated user " + id + "\"}");
});

// HEAD: Check metadata without receiving a body
// Useful for health-check pings or ETag cache validation
app.head("/users/:id", (req: Request, res: Response) => {
    res.setHeader("X-User-Exists", "true").setStatus(200.0).send("");
});

// OPTIONS: Return allowed methods (CORS preflight)
app.options("/users", (req: Request, res: Response) => {
    res.setHeader("Allow", "GET, POST, PUT, PATCH, DELETE, HEAD, OPTIONS")
       .setStatus(204.0).send("");
});
```

```typescript [ANY — Match All Methods]
// app.any() matches all HTTP methods (GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, ...)
// Ideal for health-check or metrics endpoints
app.any("/health", (req: Request, res: Response) => {
    res.json("{\"status\": \"UP\", \"method\": \"" + req.method + "\", \"engine\": \"Vito v1.2.0\"}");
});
// curl -X GET  /health  → { status: "UP", method: "GET" }
// curl -X HEAD /health  → { status: "UP", method: "HEAD" }
```

:::

---

## 🎯 2. Dynamic Route Parameters & Wildcards

Vito supports the full spectrum of URL parameter patterns:

::: code-group

```typescript [1. Single Segment Param (:id)]
// Actual URL: /users/101 or /users/alex
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id"); // "101" or "alex"
    res.json("{\"user_id\": \"" + userId + "\"}");
});
```

```typescript [2. Multi Params (:category/:slug)]
// Actual URL: /products/electronics/laptop-m3
app.get("/products/:category/:slug", (req: Request, res: Response) => {
    let cat  = req.param("category"); // "electronics"
    let slug = req.param("slug");     // "laptop-m3"
    res.json("{\"category\": \"" + cat + "\", \"slug\": \"" + slug + "\"}");
});
```

```typescript [3. Wildcard Multi-segment (*filepath)]
// *filepath captures the ENTIRE remaining path, including "/"
// URL: /files/docs/2026/q3/report.pdf → filepath = "docs/2026/q3/report.pdf"
app.get("/files/*filepath", (req: Request, res: Response) => {
    let filePath = req.param("filepath"); // "docs/2026/q3/report.pdf"
    res.send("Serving: " + filePath);
});

// URL: /files/img/logo.png → filepath = "img/logo.png"
// URL: /files/a/b/c/d      → filepath = "a/b/c/d"
```

```typescript [4. Wildcard — SPA Fallback]
// Catch-all route for Single Page Applications (React/Vue/Svelte)
// MUST be declared LAST — it has the lowest matching priority (Pass 3)
app.get("/*", (req: Request, res: Response) => {
    res.html("<!DOCTYPE html><html><body><div id=\"app\"></div></body></html>");
});
```

```typescript [5. Optional Params (:slug?)]
// :category? matches both /posts and /posts/tech
app.get("/posts/:category?", (req: Request, res: Response) => {
    let category = req.param("category"); // "" if not provided

    if (category == "") {
        res.json("{\"message\": \"All posts\"}");
    } else {
        res.json("{\"message\": \"Posts in category: " + category + "\"}");
    }
});
// GET /posts        → { message: "All posts" }
// GET /posts/tech   → { message: "Posts in category: tech" }
```

```typescript [6. Regex Constraints (:id(\\d+))]
// :id(\d+) — only matches if :id consists of digits 0-9
app.get("/users/:id(\\d+)", (req: Request, res: Response) => {
    let userId = req.paramInt("id"); // Type-safe number: 101
    res.json("{\"user_id\": " + userId + "}");
});

// :slug([a-z0-9-]+) — only matches lowercase letters, numbers, and hyphens
app.get("/articles/:slug([a-z0-9-]+)", (req: Request, res: Response) => {
    let slug = req.param("slug");
    res.json("{\"slug\": \"" + slug + "\"}");
});
// GET /users/101  → 200 OK (id = 101)
// GET /users/abc  → 404/405 (does not match regex \\d+)
```

:::

---

## 🔍 3. URL Search Query String (`?key=value`)

Use `req.query(key)` to extract Query String parameters:

```typescript
app.get("/search", (req: Request, res: Response) => {
    let keyword  = req.query("q");      // "vito"
    let pageNum  = req.query("page");   // "2"
    let pageSize = req.query("limit");  // "" if not provided

    let limit = pageSize;
    if (limit == "") { limit = "10"; }

    res.json("{\"query\": \"" + keyword + "\", \"page\": \"" + pageNum + "\", \"limit\": \"" + limit + "\"}");
});
// GET /search?q=vito&page=2&limit=20
```

::: tip 💡 Query Params vs Route Params
- **Route Params** (`:id`): Required part of URL structure, used for resource identifiers. Required unless marked optional with `?`.
- **Query Params** (`?page=2`): Optional filters, used for pagination, sorting, and filtering.
:::

---

## 👥 4. Route Grouping — Modular Organization

### 4.1 Basic Grouping (Shared Prefix)

```typescript
// All routes inside share the '/api/v1' prefix
app.group("/api/v1", (v1) => {
    // 📍 GET  /api/v1/users
    v1.get("/users", (req, res) => {
        res.json("[\"Alice\", \"Bob\", \"Charlie\"]");
    });

    // 📍 GET  /api/v1/users/:id
    v1.get("/users/:id", (req, res) => {
        let id = req.param("id");
        res.json("{\"id\": \"" + id + "\"}");
    });

    // 📍 POST /api/v1/users
    v1.post("/users", (req, res) => {
        res.setStatus(201.0).json("{\"message\": \"User created\"}");
    });

    // 📍 PATCH /api/v1/users/:id (NEW)
    v1.patch("/users/:id", (req, res) => {
        let id = req.param("id");
        res.json("{\"message\": \"Patched user " + id + "\"}");
    });
});
```

### 4.2 Nested Groups

```typescript
app.group("/api", (api) => {
    // ── Version 1 ──────────────────────────────────────────────
    api.group("/v1", (v1) => {
        v1.get("/users",     userListHandler);
        v1.get("/users/:id", userDetailHandler);
    });

    // ── Version 2 (new features) ───────────────────────────────
    api.group("/v2", (v2) => {
        v2.get("/users",           userListV2Handler);
        v2.get("/users/:id",       userDetailV2Handler);
        v2.get("/users/:id/posts", userPostsHandler);
    });
});
// Results:
// GET /api/v1/users       → userListHandler
// GET /api/v2/users/42    → userDetailV2Handler
```

### 4.3 Sub-App with createRouter()

`createRouter()` returns a fully independent `VitoApp` instance — ideal for the **Plugin Pattern** and **isolated module testing**:

```typescript
import { createRouter, createApp } from "vito";

// ── Standalone Admin Router ──────────────────────────────────
let adminApp = createRouter(); // returns an independent VitoApp
adminApp.get("/dashboard", dashboardHandler);
adminApp.get("/users",     adminUserListHandler);
adminApp.post("/users",    adminCreateUserHandler);

// ── Integrate into main app via group() ─────────────────────
let app = createApp();
app.group("/admin", (g) => {
    g.get("/dashboard", dashboardHandler);
    g.get("/users",     adminUserListHandler);
    g.post("/users",    adminCreateUserHandler);
});

// Results: /admin/dashboard, /admin/users
```

::: tip 💡 Isolated Testing with createRouter()
```typescript
// Test admin module in isolation, without spinning up the full app
let testRouter = createRouter();
testRouter.get("/dashboard", dashboardHandler);

let res = testRouter.inject("GET", "/dashboard");
assert(res.status == 200.0);
```
:::

---

## 🔑 5. Middleware

Middleware integrates at multiple levels via global pipeline and lifecycle hooks:

### 5.1 Middleware Scopes

```typescript
import { createApp, Request, Response } from "vito";
import { loggerMiddleware }  from "vito/packages/logger/logger.vit";
import { corsMiddleware }    from "vito/packages/cors/cors.vit";
import { authMiddleware }    from "vito/packages/auth/auth.vit";
import { createRateLimiter } from "vito/packages/security/security.vit";

let app = createApp();

// ── Scope 1: Global (applies to ALL routes) ──────────────────
app.use(loggerMiddleware);
app.use(corsMiddleware);

// ── Scope 2: Group (applies to ALL routes in the group) ───────
app.group("/api", (api) => {
    api.use(authMiddleware);  // Protects all /api/* routes

    api.get("/profile",  profileHandler);
    api.get("/settings", settingsHandler);
});

// ── Scope 3: beforeHandle hook (request-level gate) ────────────
app.beforeHandle((req: Request, res: Response) => {
    // Runs after global middleware, before route dispatch
    // Return false or write response to stop the pipeline
    if (req.path == "/admin" && req.header("X-Admin-Key") == "") {
        res.setStatus(403.0).json("{\"error\": \"Forbidden\"}");
        return false;
    }
    return true;
});
```

::: warning ⚠️ Pipeline Order
Global middleware (`app.use()`) → `beforeHandle` hook → Route handler → `afterHandle` hook. Any middleware returning `false` or writing a response stops the pipeline immediately.
:::

---

## 🏷️ 6. Named Routes & URL Generation (Reverse Routing)

Name your routes to generate URLs dynamically, avoiding hardcoded strings:

```typescript
import { createApp, Request, Response } from "vito";

let app = createApp();

// ── Register routes ──────────────────────────────────────────
app.get("/users/:id",       userHandler);
app.get("/users/:id/posts", userPostsHandler);
app.get("/products/:slug",  productHandler);

// ── Name routes via nameRoute() ──────────────────────────────
app.nameRoute("user.show",    "/users/:id");
app.nameRoute("user.posts",   "/users/:id/posts");
app.nameRoute("product.show", "/products/:slug");

// ── Generate URLs via app.url(name, paramKey, paramVal) ──────
let userUrl    = app.url("user.show",    "id",   "42");
// → "/users/42"

let postsUrl   = app.url("user.posts",   "id",   "42");
// → "/users/42/posts"

let productUrl = app.url("product.show", "slug", "laptop-m3-pro");
// → "/products/laptop-m3-pro"

// ── Use in a redirect ────────────────────────────────────────
app.get("/old-profile", (req: Request, res: Response) => {
    let userId = req.query("id");
    let newUrl = app.url("user.show", "id", userId);
    res.redirect(newUrl);
});
```

::: tip 💡 Benefits of Named Routes
- **Refactor-safe**: Changing `/users/:id` → `/members/:id` only requires one edit — all `app.url("user.show", ...)` calls update automatically.
- **Cleaner code**: No more complex URL string concatenation scattered through your codebase.
- **Up to 16 named routes** — suitable for real-world REST API architectures.
:::

---

## 📖 7. OpenAPI Metadata Inline (Auto Swagger UI)

Attach OpenAPI metadata to routes and Vito automatically generates Swagger UI at `/docs`:

```typescript
import { createApp, Request, Response } from "vito";
import { swagger } from "vito/packages/swagger/swagger.vit";

let app = createApp();
app.use(swagger({ path: "/docs" })); // Mount Swagger UI at /docs

app.get("/users/:id", (req: Request, res: Response) => {
    let id = req.param("id");
    res.json("{\"id\": \"" + id + "\", \"name\": \"Alice\", \"role\": \"admin\"}");
}).meta({
    summary:     "Get user details",
    description: "Returns the full profile of a user by ID. Requires Bearer token.",
    tags:        ["Users"],
    params: {
        id: { type: "string", description: "User ID", example: "42" }
    },
    responses: {
        200: { description: "User found",     schema: { id: "string", name: "string", role: "string" } },
        404: { description: "User not found", schema: { error: "string" } },
        401: { description: "Unauthorized" }
    }
});

app.listen(3000);
// Swagger UI available at: http://localhost:3000/docs
```

---

## 🎨 8. Custom Error Handlers

### 8.1 Not Found Handler (404)

```typescript
app.setNotFoundHandler((req: Request, res: Response) => {
    res.setStatus(404.0).json(
        "{\"error\": \"Route Not Found\", \"path\": \"" + req.path + "\", \"method\": \"" + req.method + "\"}"
    );
});
```

### 8.2 Method Not Allowed (405)

```typescript
// Automatically triggered when the path exists but the HTTP method is rejected
app.setMethodNotAllowedHandler((req: Request, res: Response) => {
    res.setStatus(405.0)
       .setHeader("Allow", "GET, POST")
       .json("{\"error\": \"Method Not Allowed\", \"method\": \"" + req.method + "\"}");
});
```

### 8.3 Global Error Handler

```typescript
// Catches all unhandled errors in handlers / middleware
app.setErrorHandler((req: Request, res: Response) => {
    res.setStatus(500.0).json(
        "{\"error\": \"Internal Server Error\", \"path\": \"" + req.path + "\"}"
    );
});
```

### 8.4 res.redirect()

```typescript
// HTTP 302 Found (temporary redirect)
app.get("/home", (req: Request, res: Response) => {
    res.redirect("/dashboard");
});

// HTTP 301 Moved Permanently
app.get("/old-api", (req: Request, res: Response) => {
    res.redirectPermanent("/api/v2");
});
```

---

## 🏗️ 9. Real-World Example: Production-Grade REST API

```typescript
import { createApp, Request, Response } from "vito";
import { loggerMiddleware }           from "vito/packages/logger/logger.vit";
import { corsMiddleware }             from "vito/packages/cors/cors.vit";
import { createRateLimiter }          from "vito/packages/security/security.vit";
import { authMiddleware, adminGuard } from "vito/packages/auth/auth.vit";
import { swagger }                    from "vito/packages/swagger/swagger.vit";

function main(): number {
    let app = createApp(); // Lax trailing slash (default)

    // ── Global Middleware ──────────────────────────────────────
    app.use(loggerMiddleware);
    app.use(corsMiddleware);
    app.use(swagger({ path: "/docs" }));

    // ── Health Check (any method) ──────────────────────────────
    app.any("/health", (req, res) => {
        res.json("{\"status\": \"UP\", \"version\": \"1.2.0\"}");
    });

    // ── Auth Endpoints ─────────────────────────────────────────
    app.use(createRateLimiter(100, 60)); // 100 req/min global
    app.post("/auth/login",    loginHandler);
    app.post("/auth/register", registerHandler);
    app.post("/auth/logout",   logoutHandler);

    // ── API v1 ─────────────────────────────────────────────────
    app.group("/api/v1", (v1) => {
        v1.use(authMiddleware); // Protects all /api/v1/* routes

        // Users — full CRUD with all methods
        v1.get(    "/users",     userListHandler);
        v1.post(   "/users",     userCreateHandler);
        v1.get(    "/users/:id", userShowHandler);
        v1.put(    "/users/:id", userReplaceHandler);
        v1.patch(  "/users/:id", userUpdateHandler);   // NEW: PATCH
        v1.delete( "/users/:id", userDeleteHandler);
        v1.head(   "/users/:id", userHeadHandler);     // NEW: HEAD
        v1.options("/users",     userOptionsHandler);   // NEW: OPTIONS

        // Posts with optional category filter
        v1.get("/posts/:category?", postListHandler);
        v1.get("/posts/:id",        postShowHandler);
    });

    // ── Admin Group ────────────────────────────────────────────
    app.group("/admin", (admin) => {
        admin.use(authMiddleware);
        admin.use(adminGuard);
        admin.get("/dashboard",    adminDashboardHandler);
        admin.get("/users",        adminUserListHandler);
        admin.delete("/users/:id", adminDeleteUserHandler);
    });

    // ── Named Routes (for reverse routing) ────────────────────
    app.nameRoute("user.index",  "/api/v1/users");
    app.nameRoute("user.show",   "/api/v1/users/:id");
    app.nameRoute("post.index",  "/api/v1/posts/:category?");
    app.nameRoute("post.show",   "/api/v1/posts/:id");

    // ── Static Files (multi-segment wildcard) ─────────────────
    app.get("/public/*filepath", (req, res) => {
        let file = req.param("filepath"); // "css/app.css" or "js/a/b/c.js"
        res.send("Serving: " + file);
    });

    // ── SPA Fallback (declare LAST — lowest wildcard priority) ─
    app.get("/*", (req, res) => {
        res.html("<!DOCTYPE html><html><body><div id=\"app\"></div></body></html>");
    });

    // ── Error Handlers ─────────────────────────────────────────
    app.setNotFoundHandler((req, res) => {
        res.setStatus(404.0).json("{\"error\": \"Not Found\", \"path\": \"" + req.path + "\"}");
    });
    app.setMethodNotAllowedHandler((req, res) => {
        res.setStatus(405.0).json("{\"error\": \"Method Not Allowed\", \"method\": \"" + req.method + "\"}");
    });

    app.listen(3000);
    print("⚡ Vito Server: http://localhost:3000 | Swagger: http://localhost:3000/docs");
    return 0;
}
```

---

## 📋 Quick API Reference (Cheat Sheet)

### Request — Data Access

| Field / Method | Type | Description | Example |
| :--- | :--- | :--- | :--- |
| `req.method` | `string` | HTTP method | `req.method` → `"GET"` |
| `req.path` | `string` | URL path | `req.path` → `"/users/42"` |
| `req.param(key)` | `string` | Dynamic route param | `req.param("id")` → `"42"` |
| `req.paramInt(key)` | `number` | Type-safe integer param *(NEW)* | `req.paramInt("id")` → `42` |
| `req.paramFloat(key)` | `number` | Type-safe float param *(NEW)* | `req.paramFloat("price")` → `19.99` |
| `req.paramBool(key)` | `boolean` | Type-safe boolean param *(NEW)* | `req.paramBool("active")` → `true` |
| `req.query(key)` | `string` | Query string param | `req.query("page")` → `"2"` |
| `req.queryInt(key)` | `number` | Type-safe integer query param *(NEW)* | `req.queryInt("page")` → `2` |
| `req.header(key)` | `string` | Request header | `req.header("Authorization")` |
| `req.body` | `string` | Raw request body | `req.body` |
| `req.queryString` | `string` | Raw query string | `req.queryString` |

### Response — Sending Replies

| Method | Returns | Description | Example |
| :--- | :--- | :--- | :--- |
| `res.json(jsonStr)` | `void` | Send JSON response | `res.json("{\"ok\": true}")` |
| `res.html(htmlStr)` | `void` | Send HTML response | `res.html("<h1>Hi</h1>")` |
| `res.send(text)` | `void` | Send plain text | `res.send("Hello")` |
| `res.setStatus(code)` | `Response` | Set HTTP status | `res.setStatus(201.0)` |
| `res.setHeader(k, v)` | `Response` | Set response header | `res.setHeader("X-Foo", "bar")` |
| `res.redirect(url)` | `void` | HTTP 302 redirect | `res.redirect("/login")` |
| `res.redirectPermanent(url)` | `void` | HTTP 301 redirect | `res.redirectPermanent("/new")` |
| `res.sendChunk(chunk)` | `void` | Stream a chunk | `res.sendChunk("data: ...")` |

### Route Registration API

| Method | Description |
| :--- | :--- |
| `app.get(path, handler)` | Register GET route |
| `app.post(path, handler)` | Register POST route |
| `app.put(path, handler)` | Register PUT route |
| `app.patch(path, handler)` | Register PATCH route *(NEW)* |
| `app.delete(path, handler)` | Register DELETE route |
| `app.head(path, handler)` | Register HEAD route *(NEW)* |
| `app.options(path, handler)` | Register OPTIONS route *(NEW)* |
| `app.any(path, handler)` | Match any HTTP method *(NEW)* |
| `app.group(prefix, callback)` | Group routes by prefix |
| `app.use(middleware)` | Register global middleware |
| `app.nameRoute(name, path)` | Register a named route *(NEW)* |
| `app.url(name, paramKey, paramVal)` | Generate URL from named route *(NEW)* |
| `app.setNotFoundHandler(h)` | Custom 404 handler |
| `app.setMethodNotAllowedHandler(h)` | Custom 405 handler *(NEW)* |
| `app.setErrorHandler(h)` | Global error handler *(NEW)* |

### Factory Functions

| Function | Description |
| :--- | :--- |
| `createApp()` | Create Vito app (Lax trailing slash) |
| `createStrictApp()` | Create Vito app (Strict trailing slash) *(NEW)* |
| `createRouter()` | Create standalone router instance (Sub-app) *(NEW)* |
