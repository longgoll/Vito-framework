# Định Tuyến & Route Parameters 🎯

Hệ thống định tuyến (**Router Engine**) của **Vito Framework** được thiết kế dựa trên kiến trúc **3-Pass Flat-Unrolled Engine (Phase 9 Perf)**, kết hợp giữa tốc độ phản hồi cực đại O(1) qua Arena Slot Lookup và khả năng phân loại ưu tiên route tự động.

<RouteVisualizer />

---

## 🧠 0. Nguyên Lý Hoạt Động: 3-Pass Flat-Unrolled Engine

Vito Router sử dụng cấu trúc **Flat-Unrolled Registry (32 Slots cố định)** kết hợp với **Memory Arena Slot Collector** để đạt hiệu năng xử lý cực đại và **Zero-Allocation**:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Zero-Allocation Matching</h4>
    <p>Quá trình khớp route sử dụng bộ nhớ Arena Slot recycled — không tạo bất kỳ heap allocation mới nào.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">♻️</div>
    <h4>Flat-Unrolled Memory Layout</h4>
    <p>32 slots route và 8 slots middleware được trải phẳng (unrolled) trực tiếp trong struct VitoApp, tối ưu L1/L2 Cache Locality.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔢</div>
    <h4>3-Pass Precedence Engine</h4>
    <p>Tự động phân loại route thành 3 cấp ưu tiên (Static -> Parametric -> Wildcard) lúc đăng ký mà không cần sort.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🌿</div>
    <h4>O(1) Memory Arena Access</h4>
    <p>Quản lý vòng đời request slot qua bitmask arena, giải phóng ngay khi hoàn tất request handling.</p>
  </div>
</div>

### 🏆 Thứ Tự Ưu Tiên Khớp Route (3-Pass Precedence Engine)

Khi nhiều route có thể khớp cùng một URL, Vito Router thực hiện **3 lượt quét tuần tự** theo thứ tự ưu tiên:

| Pass | Loại Route | Phân loại | Ví dụ |
| :---: | :--- | :--- | :--- |
| **Pass 1** | Static Route | Không có `:` hay `*` | `/users/profile` |
| **Pass 2** | Parametric / Optional | Có `:param` hoặc `:slug?` | `/users/:id`, `/posts/:cat?` |
| **Pass 3** | Wildcard | Có `*name` | `/files/*path` |

```typescript
// ✅ Khai báo bất kỳ thứ tự nào — engine tự phân loại và ưu tiên đúng
app.get("/users/:id",      userHandler);      // ← đăng ký trước
app.get("/users/profile",  profileHandler);   // ← đăng ký sau

// Kết quả matching (engine tự ưu tiên Static trước Param):
// GET /users/profile  → profileHandler  ✓ (Pass 1 — Static thắng)
// GET /users/42       → userHandler     ✓ (Pass 2 — Param)
```

::: tip 💡 Route Type tự động
`addRoute()` gọi `getRouteType()` tại thời điểm đăng ký, lưu vào `rN_type` (0/1/2). Không cần sắp xếp thủ công.
:::

### ⚙️ Chính Sách Trailing Slash

Vito Router hỗ trợ hai chế độ xử lý dấu `/` cuối URL:

::: code-group

```typescript [Lax Mode — Mặc định]
// createApp() mặc định là Lax: tự động bỏ "/" cuối
// /users/ → tự động normalize thành /users
let app = createApp(); // isStrictSlash = false

app.get("/users", handler);
// Cả /users VÀ /users/ đều khớp route này ✓
```

```typescript [Strict Mode]
// createStrictApp(): /users và /users/ là hai route hoàn toàn khác nhau
let app = createStrictApp(); // isStrictSlash = true

app.get("/users",  handlerA); // Chỉ khớp /users
app.get("/users/", handlerB); // Chỉ khớp /users/
```

:::

---

## ⚡ 1. Toàn Bộ HTTP Methods

Vito hỗ trợ đầy đủ **tất cả** HTTP Methods theo chuẩn RFC:

<div class="card-grid">
  <div class="feature-mini-card">
    <span class="method-badge get">GET</span>
    <h4>Truy Vấn Dữ Liệu</h4>
    <p>Lấy tài nguyên hoặc render HTML. Không thay đổi state server.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge post">POST</span>
    <h4>Tạo Mới Tài Nguyên</h4>
    <p>Tiếp nhận Form hoặc JSON Body để tạo mới thực thể.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge put">PUT</span>
    <h4>Cập Nhật Toàn Bộ</h4>
    <p>Ghi đè hoàn toàn tài nguyên. Idempotent theo HTTP.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge delete">DELETE</span>
    <h4>Xóa Tài Nguyên</h4>
    <p>Gỡ bỏ tài nguyên. Idempotent theo chuẩn HTTP.</p>
  </div>
</div>

::: code-group

```typescript [CRUD Cơ Bản]
import { createApp, Request, Response } from "vito";

let app = createApp();

app.get(   "/users",     (req, res) => { res.send("GET    — Danh sách users"); });
app.post(  "/users",     (req, res) => { res.send("POST   — Tạo user mới"); });
app.put(   "/users/:id", (req, res) => { res.send("PUT    — Cập nhật toàn bộ user"); });
app.delete("/users/:id", (req, res) => { res.send("DELETE — Xóa user"); });
```

```typescript [PATCH, HEAD, OPTIONS]
// PATCH: Cập nhật một phần tài nguyên (RFC 5789)
app.patch("/users/:id", (req: Request, res: Response) => {
    let id = req.param("id");
    res.json("{\"message\": \"Cập nhật một phần user " + id + "\"}");
});

// HEAD: Kiểm tra metadata mà không nhận body
// Dùng để ping health-check hoặc kiểm tra ETag cache
app.head("/users/:id", (req: Request, res: Response) => {
    res.setHeader("X-User-Exists", "true").setStatus(200.0).send("");
});

// OPTIONS: Trả về các methods được phép (CORS preflight thủ công)
app.options("/users", (req: Request, res: Response) => {
    res.setHeader("Allow", "GET, POST, PUT, PATCH, DELETE, HEAD, OPTIONS")
       .setStatus(204.0).send("");
});
```

```typescript [ANY — Khớp Mọi Method]
// app.any() khớp với tất cả HTTP methods
// Lý tưởng cho health-check endpoint
app.any("/health", (req: Request, res: Response) => {
    res.json("{\"status\": \"UP\", \"method\": \"" + req.method + "\", \"engine\": \"Vito v1.2.0\"}");
});
// curl -X GET  /health  → { status: "UP", method: "GET" }
// curl -X HEAD /health  → { status: "UP", method: "HEAD" }
```

:::

---

## 🎯 2. Route Parameters Động & Wildcard

Vito hỗ trợ đầy đủ các loại tham số URL:

::: code-group

```typescript [1. Single Segment Param (:id)]
// URL thực tế: /users/101 hoặc /users/alex
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id"); // "101" hoặc "alex"
    res.json("{\"user_id\": \"" + userId + "\"}");
});
```

```typescript [2. Multi Params (:category/:slug)]
// URL thực tế: /products/electronics/laptop-m3
app.get("/products/:category/:slug", (req: Request, res: Response) => {
    let cat  = req.param("category"); // "electronics"
    let slug = req.param("slug");     // "laptop-m3"
    res.json("{\"category\": \"" + cat + "\", \"slug\": \"" + slug + "\"}");
});
```

```typescript [3. Wildcard Multi-segment (*filepath)]
// *filepath khớp TOÀN BỘ phần còn lại của path, bao gồm cả "/"
// URL: /files/docs/2026/q3/report.pdf
app.get("/files/*filepath", (req: Request, res: Response) => {
    let filePath = req.param("filepath"); // "docs/2026/q3/report.pdf"
    res.send("Serving: " + filePath);
});

// URL: /files/img/logo.png → filepath = "img/logo.png"
// URL: /files/a/b/c/d      → filepath = "a/b/c/d"
```

```typescript [4. Wildcard — SPA Fallback]
// Catch-all route cho Single Page Application (React/Vue/Svelte)
// Phải khai báo CUỐI CÙNG để không che các API route khác
app.get("/*", (req: Request, res: Response) => {
    res.html("<!DOCTYPE html><html><body><div id=\"app\"></div></body></html>");
});
```

```typescript [5. Optional Params (:slug?)]
// :category? — khớp cả /posts và /posts/tech
app.get("/posts/:category?", (req: Request, res: Response) => {
    let category = req.param("category"); // "" nếu không cung cấp

    if (category == "") {
        res.json("{\"message\": \"Tất cả bài viết\"}");
    } else {
        res.json("{\"message\": \"Bài viết danh mục: " + category + "\"}");
    }
});
// GET /posts        → { message: "Tất cả bài viết" }
// GET /posts/tech   → { message: "Bài viết danh mục: tech" }
```

```typescript [6. Regex Constraints (:id(\\d+))]
// :id(\d+) — chỉ khớp nếu :id là các chữ số 0-9
app.get("/users/:id(\\d+)", (req: Request, res: Response) => {
    let userId = req.paramInt("id"); // Type-safe number: 101
    res.json("{\"user_id\": " + userId + "}");
});

// :slug([a-z0-9-]+) — chỉ khớp chữ cái thường, số và dấu gạch ngang
app.get("/articles/:slug([a-z0-9-]+)", (req: Request, res: Response) => {
    let slug = req.param("slug");
    res.json("{\"slug\": \"" + slug + "\"}");
});
// GET /users/101  → 200 OK (id = 101)
// GET /users/abc  → 404/405 (không khớp regex \\d+)
```

> ✅ Tính năng này đã được triển khai trong Vito Router: `:param(pattern)` được xử lý khi khớp route. Mẫu regex inline được tự động khoá bằng `^...$` để phân tích toàn bộ segment.
>
> Dùng `req.paramInt("id")`, `req.paramFloat("price")`, `req.paramBool("active")` để lấy route params theo kiểu an toàn.

:::

---

## 🔍 3. URL Search Query String (`?key=value`)

Để trích xuất các tham số từ Query String, sử dụng `req.query(key)`:

```typescript
app.get("/search", (req: Request, res: Response) => {
    let keyword  = req.query("q");      // "vito"
    let pageNum  = req.query("page");   // "2"
    let pageSize = req.query("limit");  // "" nếu không cung cấp

    let limit = pageSize;
    if (limit == "") { limit = "10"; }

    res.json("{\"query\": \"" + keyword + "\", \"page\": \"" + pageNum + "\", \"limit\": \"" + limit + "\"}");
});
// GET /search?q=vito&page=2&limit=20
```

::: tip 💡 Query Params vs Route Params
- **Route Params** (`:id`): Là phần cấu trúc URL, dùng cho resource identifier. Bắt buộc (trừ optional `?`).
- **Query Params** (`?page=2`): Bộ lọc tuỳ chọn, dùng cho pagination, sorting, filtering.
:::

---

## 👥 4. Route Grouping — Gom Nhóm Mô-đun

### 4.1 Grouping Cơ Bản (Shared Prefix)

```typescript
// Tất cả route bên trong đều có prefix '/api/v1'
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

### 4.2 Nested Groups (Groups Lồng Nhau)

```typescript
app.group("/api", (api) => {
    // ── Version 1 ──────────────────────────────────────────────
    api.group("/v1", (v1) => {
        v1.get("/users",     userListHandler);
        v1.get("/users/:id", userDetailHandler);
    });

    // ── Version 2 (tính năng mới) ──────────────────────────────
    api.group("/v2", (v2) => {
        v2.get("/users",           userListV2Handler);
        v2.get("/users/:id",       userDetailV2Handler);
        v2.get("/users/:id/posts", userPostsHandler);
    });
});
// Kết quả:
// GET /api/v1/users       → userListHandler
// GET /api/v2/users/42    → userDetailV2Handler
```

### 4.3 Sub-App với createRouter()

`createRouter()` tạo một `VitoApp` instance hoàn toàn độc lập — lý tưởng cho **Plugin Pattern** và **Module Testing**:

```typescript
import { createRouter, createApp } from "vito";

// ── Admin Router độc lập ────────────────────────────────────
let adminApp = createRouter(); // trả về VitoApp instance riêng
adminApp.get("/dashboard", dashboardHandler);
adminApp.get("/users",     adminUserListHandler);
adminApp.post("/users",    adminCreateUserHandler);

// ── Tích hợp vào app chính bằng group ──────────────────────
let app = createApp();
app.group("/admin", (g) => {
    // Copy các handler từ adminApp vào group
    g.get("/dashboard", dashboardHandler);
    g.get("/users",     adminUserListHandler);
    g.post("/users",    adminCreateUserHandler);
});

// Kết quả: /admin/dashboard, /admin/users
```

::: tip 💡 Test Isolate với createRouter()
```typescript
// Test admin module độc lập, không cần khởi tạo app chính
let testRouter = createRouter();
testRouter.get("/dashboard", dashboardHandler);

let res = testRouter.inject("GET", "/dashboard");
assert(res.status == 200.0);
```
:::

---

## 🔑 5. Route-Level Middleware

Middleware không chỉ dùng ở Global (`app.use()`) hay Group — dùng trực tiếp trong từng route qua pipeline global + group:

### 5.1 Các Cấp Middleware

```typescript
import { createApp, Request, Response } from "vito";
import { loggerMiddleware }   from "vito/packages/logger/logger.vit";
import { corsMiddleware }     from "vito/packages/cors/cors.vit";
import { authMiddleware }     from "vito/packages/auth/auth.vit";
import { createRateLimiter }  from "vito/packages/security/security.vit";

let app = createApp();

// ── Cấp 1: Global (áp dụng cho TẤT CẢ routes) ───────────────
app.use(loggerMiddleware);
app.use(corsMiddleware);

// ── Cấp 2: Group (áp dụng cho TẤT CẢ routes trong group) ─────
app.group("/api", (api) => {
    api.use(authMiddleware);  // Bảo vệ toàn bộ /api/* routes

    api.get("/profile",  profileHandler);
    api.get("/settings", settingsHandler);
});

// ── Cấp 3: Global với rateLimiter cho route nhạy cảm ─────────
// (Route-level middleware tích hợp qua beforeHandle hook)
app.beforeHandle((req: Request, res: Response) => {
    if (req.path == "/auth/login") {
        // Kiểm tra rate limit chỉ cho login endpoint
        let ip = req.header("X-Real-IP");
        // ... logic rate limit ...
    }
    return true; // continue
});

app.post("/auth/login", loginHandler);
```

::: warning ⚠️ Thứ Tự Pipeline
Global middleware → beforeHandle hook → Route handler → afterHandle hook. Bất kỳ middleware nào trả về `false` hoặc gọi `res.json()` sẽ ngắt pipeline.
:::

---

## 🏷️ 6. Named Routes & URL Generation (Reverse Routing)

Đặt tên cho route để tạo URL động, tránh hardcode string:

```typescript
import { createApp, Request, Response } from "vito";

let app = createApp();

// ── Đăng ký routes ──────────────────────────────────────────
app.get("/users/:id",       userHandler);
app.get("/users/:id/posts", userPostsHandler);
app.get("/products/:slug",  productHandler);

// ── Đặt tên (nameRoute) sau khi đăng ký ─────────────────────
app.nameRoute("user.show",    "/users/:id");
app.nameRoute("user.posts",   "/users/:id/posts");
app.nameRoute("product.show", "/products/:slug");

// ── Sinh URL từ tên route (Reverse Routing) ──────────────────
// app.url(routeName, paramKey, paramVal)
let userUrl    = app.url("user.show",    "id",   "42");
// → "/users/42"

let postsUrl   = app.url("user.posts",   "id",   "42");
// → "/users/42/posts"

let productUrl = app.url("product.show", "slug", "laptop-m3-pro");
// → "/products/laptop-m3-pro"

// ── Dùng trong redirect ──────────────────────────────────────
app.get("/old-profile", (req: Request, res: Response) => {
    let userId = req.query("id");
    let newUrl = app.url("user.show", "id", userId);
    res.redirect(newUrl);
});
```

::: tip 💡 Lợi Ích Của Named Routes
- **Refactor-safe**: Thay đổi `/users/:id` → `/members/:id` chỉ sửa 1 chỗ — toàn bộ `app.url("user.show", ...)` tự cập nhật.
- **Template sạch**: Không còn nối chuỗi URL phức tạp trong code.
- **Tối đa 16 named routes** — phù hợp với kiến trúc REST API thực tế.
:::

---

## 📖 7. OpenAPI Metadata Inline (Auto Swagger UI)

Gắn metadata OpenAPI vào route để tự động tạo Swagger UI tại `/docs`:

```typescript
import { createApp, Request, Response } from "vito";
import { swagger } from "vito/packages/swagger/swagger.vit";

let app = createApp();
app.use(swagger({ path: "/docs" })); // Mount Swagger UI tại /docs

app.get("/users/:id", (req: Request, res: Response) => {
    let id = req.param("id");
    res.json("{\"id\": \"" + id + "\", \"name\": \"Alice\", \"role\": \"admin\"}");
}).meta({
    summary:     "Lấy thông tin chi tiết người dùng",
    description: "Trả về profile đầy đủ theo ID. Yêu cầu Bearer token.",
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
// Swagger UI tại: http://localhost:3000/docs
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
// Tự động kích hoạt khi path khớp nhưng HTTP method bị từ chối
app.setMethodNotAllowedHandler((req: Request, res: Response) => {
    res.setStatus(405.0)
       .setHeader("Allow", "GET, POST")
       .json("{\"error\": \"Method Not Allowed\", \"method\": \"" + req.method + "\"}");
});
```

### 8.3 Global Error Handler

```typescript
// Bắt toàn bộ unhandled error trong handler/middleware
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

## 🏗️ 9. Ứng Dụng Thực Tế: REST API Chuẩn Production

```typescript
import { createApp, Request, Response } from "vito";
import { loggerMiddleware }              from "vito/packages/logger/logger.vit";
import { corsMiddleware }                from "vito/packages/cors/cors.vit";
import { createRateLimiter }             from "vito/packages/security/security.vit";
import { authMiddleware, adminGuard }    from "vito/packages/auth/auth.vit";
import { swagger }                       from "vito/packages/swagger/swagger.vit";

function main(): number {
    let app = createApp(); // Lax trailing slash (mặc định)

    // ── Global Middleware ──────────────────────────────────────
    app.use(loggerMiddleware);
    app.use(corsMiddleware);
    app.use(swagger({ path: "/docs" }));

    // ── Health Check (any method) ──────────────────────────────
    app.any("/health", (req, res) => {
        res.json("{\"status\": \"UP\", \"version\": \"1.2.0\"}");
    });

    // ── Auth Endpoints ─────────────────────────────────────────
    app.use(createRateLimiter(100, 60)); // 100 req/phút global
    app.post("/auth/login",    loginHandler);
    app.post("/auth/register", registerHandler);
    app.post("/auth/logout",   logoutHandler);

    // ── API v1 ─────────────────────────────────────────────────
    app.group("/api/v1", (v1) => {
        v1.use(authMiddleware); // Bảo vệ toàn bộ /api/v1/*

        // Users CRUD (đầy đủ methods)
        v1.get(    "/users",     userListHandler);
        v1.post(   "/users",     userCreateHandler);
        v1.get(    "/users/:id", userShowHandler);
        v1.put(    "/users/:id", userReplaceHandler);
        v1.patch(  "/users/:id", userUpdateHandler);  // NEW: PATCH
        v1.delete( "/users/:id", userDeleteHandler);
        v1.head(   "/users/:id", userHeadHandler);    // NEW: HEAD
        v1.options("/users",     userOptionsHandler);  // NEW: OPTIONS

        // Posts với optional category
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

    // ── Named Routes (cho reverse routing) ────────────────────
    app.nameRoute("user.index",  "/api/v1/users");
    app.nameRoute("user.show",   "/api/v1/users/:id");
    app.nameRoute("post.index",  "/api/v1/posts/:category?");
    app.nameRoute("post.show",   "/api/v1/posts/:id");

    // ── Static Files (wildcard multi-segment) ─────────────────
    app.get("/public/*filepath", (req, res) => {
        let file = req.param("filepath"); // "css/app.css" hoặc "js/a/b/c.js"
        res.send("Serving: " + file);
    });

    // ── SPA Fallback (khai báo CUỐI CÙNG — wildcard, ưu tiên thấp nhất) ──
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

## 📋 Tra Cứu Nhanh API Reference (Cheat Sheet)

### Request — Truy Xuất Dữ Liệu

| Trường / Phương thức | Kiểu | Mô tả | Ví dụ |
| :--- | :--- | :--- | :--- |
| `req.method` | `string` | HTTP method | `req.method` → `"GET"` |
| `req.path` | `string` | Đường dẫn URL | `req.path` → `"/users/42"` |
| `req.param(key)` | `string` | Route param động | `req.param("id")` → `"42"` |
| `req.paramInt(key)` | `number` | Route param ép kiểu số nguyên *(NEW)* | `req.paramInt("id")` → `42` |
| `req.paramFloat(key)` | `number` | Route param ép kiểu số thực *(NEW)* | `req.paramFloat("price")` → `19.99` |
| `req.paramBool(key)` | `boolean` | Route param ép kiểu boolean *(NEW)* | `req.paramBool("active")` → `true` |
| `req.query(key)` | `string` | Query string param | `req.query("page")` → `"2"` |
| `req.queryInt(key)` | `number` | Query param ép kiểu số nguyên *(NEW)* | `req.queryInt("page")` → `2` |
| `req.header(key)` | `string` | Request header | `req.header("Authorization")` |
| `req.body` | `string` | Raw request body | `req.body` |
| `req.queryString` | `string` | Raw query string | `req.queryString` |

### Response — Gửi Phản Hồi

| Phương thức | Trả về | Mô tả | Ví dụ |
| :--- | :--- | :--- | :--- |
| `res.json(jsonStr)` | `void` | Gửi JSON response | `res.json("{\"ok\": true}")` |
| `res.html(htmlStr)` | `void` | Gửi HTML response | `res.html("<h1>Hi</h1>")` |
| `res.send(text)` | `void` | Gửi plain text | `res.send("Hello")` |
| `res.setStatus(code)` | `Response` | Set HTTP status | `res.setStatus(201.0)` |
| `res.setHeader(k, v)` | `Response` | Set response header | `res.setHeader("X-Foo", "bar")` |
| `res.redirect(url)` | `void` | HTTP 302 redirect | `res.redirect("/login")` |
| `res.redirectPermanent(url)` | `void` | HTTP 301 redirect | `res.redirectPermanent("/new")` |
| `res.sendChunk(chunk)` | `void` | Stream chunk | `res.sendChunk("data: ...")` |

### Route Registration API

| Phương thức | Mô tả |
| :--- | :--- |
| `app.get(path, handler)` | Đăng ký GET route |
| `app.post(path, handler)` | Đăng ký POST route |
| `app.put(path, handler)` | Đăng ký PUT route |
| `app.patch(path, handler)` | Đăng ký PATCH route *(NEW)* |
| `app.delete(path, handler)` | Đăng ký DELETE route |
| `app.head(path, handler)` | Đăng ký HEAD route *(NEW)* |
| `app.options(path, handler)` | Đăng ký OPTIONS route *(NEW)* |
| `app.any(path, handler)` | Khớp mọi HTTP method *(NEW)* |
| `app.group(prefix, callback)` | Gom routes theo prefix |
| `app.use(middleware)` | Đăng ký global middleware |
| `app.nameRoute(name, path)` | Đặt tên route *(NEW)* |
| `app.url(name, paramKey, paramVal)` | Sinh URL từ tên route *(NEW)* |
| `app.setNotFoundHandler(h)` | Custom 404 handler |
| `app.setMethodNotAllowedHandler(h)` | Custom 405 handler *(NEW)* |
| `app.setErrorHandler(h)` | Global error handler *(NEW)* |

### Factory Functions

| Hàm | Mô tả |
| :--- | :--- |
| `createApp()` | Tạo Vito app (Lax trailing slash) |
| `createStrictApp()` | Tạo Vito app (Strict trailing slash) *(NEW)* |
| `createRouter()` | Tạo router instance độc lập (Sub-app) *(NEW)* |
