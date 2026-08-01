# Vito API Reference 📚

Complete API reference for the core components of the **Vito Web Framework**.

---

## 🏗 `VitoApp` Class

### `createApp()`
Initializes a new instance of the Vito application.

```javascript
let app = createApp();
```

### `app.get(path, handler)` / `app.post(path, handler)` / `app.put()` / `app.delete()`
Registers a route handler for the specified HTTP verb.

### `app.group(prefix, callback)`
Groups routes under a common path prefix.

### `app.use(middleware)`
Applies a middleware to the request processing pipeline.

### `app.setStrictSlash(strict: boolean)`
Enables/disables Strict Trailing Slash Normalization (`/users/` vs `/users`). Default: `false` (Lax mode).

### `app.setNotFoundHandler(handler)`
Sets a custom handler for 404 Not Found responses.

### `app.listen(port)`
Starts listening for HTTP connections on the given port.

---

## ⚡ Performance Packages (`packages/pool` & `packages/router`)

| Function / Helper | Package | Description |
| :--- | :--- | :--- |
| `createMemoryArena()` | `packages/pool/pool.vit` | Instantiates reusable Request Memory Arena slots manager. |
| `createTcpBufferPool()` | `packages/pool/pool.vit` | Instantiates TCP Read/Write Buffer Recycling Pool. |
| `zeroAllocHeaderLookup(raw, target)` | `packages/pool/pool.vit` | Extracts header value directly from raw HTTP stream with zero heap allocation. |
| `createRadixRouter(strict)` | `packages/router/router.vit` | Initializes Radix Trie Router with $O(1)$ Static Route Fast Path. |
| `normalizePath(path, strict)` | `packages/router/router.vit` | Normalizes URL path according to Lax / Strict slash rules. |

---

## 📥 `Request` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `req.param(key)` | Get dynamic path parameter `:key` or `*wildcard` | `req.param("id")` |
| `req.query(key)` | Get search query parameter `?key=val` | `req.query("page")` |
| `req.header(key)` | Get HTTP request header value | `req.header("Authorization")` |
| `req.method` | Property returning the HTTP method (GET, POST...) | `req.method` |
| `req.path` | Property returning the URL path | `req.path` |

---

## 📤 `Response` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `res.setStatus(code)` | Set HTTP status code (200, 404, 500...) | `res.setStatus(200)` |
| `res.setHeader(name, val)`| Set custom HTTP response header | `res.setHeader("X-Powered-By", "Vito")` |
| `res.json(jsonStr)` | Send JSON response with `Content-Type: application/json` | `res.json("{\"ok\":true}")` |
| `res.html(htmlStr)` | Send HTML response with `Content-Type: text/html` | `res.html("<h1>Hi</h1>")` |
| `res.send(textStr)` | Send Plain Text response | `res.send("Hello World")` |
| `res.sendChunk(chunk)` | Send stream / chunked response | `res.sendChunk(data)` |
