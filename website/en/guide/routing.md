# Routing & Route Parameters 🎯

The **Router Engine** of **Vito Framework** is built on the **Radix Trie (Phase 9 Engine)** algorithm, combining maximum response speed with flexible parameter extraction.

<RouteVisualizer />

---

## ⚡ 1. Core HTTP Methods

Vito provides an extremely concise syntax for all common HTTP Methods:

<div class="card-grid">
  <div class="feature-mini-card">
    <span class="method-badge get">GET</span>
    <h4>Query Data</h4>
    <p>Use `app.get("/path", handler)` to handle data retrieval or HTML page requests.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge post">POST</span>
    <h4>Create Data</h4>
    <p>Use `app.post("/path", handler)` to receive data from a Form or JSON Body.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge put">PUT</span>
    <h4>Update Data</h4>
    <p>Use `app.put("/path", handler)` to overwrite or modify an existing resource.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge delete">DELETE</span>
    <h4>Delete Resource</h4>
    <p>Use `app.delete("/path", handler)` to perform resource removal operations.</p>
  </div>
</div>

```typescript
app.get("/users", (req: Request, res: Response) => { res.send("GET /users"); });
app.post("/users", (req: Request, res: Response) => { res.send("POST /users"); });
app.put("/users/:id", (req: Request, res: Response) => { res.send("PUT /users"); });
app.delete("/users/:id", (req: Request, res: Response) => { res.send("DELETE /users"); });
```

---

## 🎯 2. Dynamic Route Parameters & Wildcards

You can easily define URL parameters with the `:paramName` syntax or multi-segment wildcards `*filepath`:

::: code-group

```typescript [1. Single Segment Param (:id)]
// Actual URL: /users/101 or /users/alex
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id");
    res.json({ status: "success", user_id: userId });
});
```

```typescript [2. Multi Params (:category/:id)]
// Actual URL: /products/electronics/laptop-m3
app.get("/products/:category/:id", (req: Request, res: Response) => {
    let cat = req.param("category");
    let itemId = req.param("id");
    res.json({ category: cat, item_id: itemId });
});
```

```typescript [3. Wildcard Path (*filepath)]
// Actual URL: /files/docs/2026/q3/report.pdf
app.get("/files/*filepath", (req: Request, res: Response) => {
    let path = req.param("filepath");
    res.send("Requested path: " + path);
});
```

:::

---

## 🔍 3. URL Search Query String (`?key=value`)

To extract parameters from the Query String (e.g. `/search?q=vit-lang&page=2`), use the `req.query(key)` method:

```typescript
app.get("/search", (req: Request, res: Response) => {
    let keyword = req.query("q");
    let pageNum = req.query("page");

    res.json({
        search_query: keyword,
        page: pageNum,
        results: []
    });
});
```

---

## 👥 4. Modular Route Grouping

Manage path prefixes and centralized Middleware using the **Route Grouping** feature:

```typescript
// All routes inside share the '/api/v1' prefix
app.group("/api/v1", (v1) => {
    // 📍 GET /api/v1/users
    v1.get("/users", (req, res) => {
        res.json(["Alice", "Bob", "Charlie"]);
    });

    // 📍 GET /api/v1/users/42
    v1.get("/users/:id", (req, res) => {
        let id = req.param("id");
        res.json({ id: id, name: "User " + id });
    });
});
```

---

## 🎨 5. Custom 404 Not Found Handler

Easily replace the default 404 error page with a project-standard HTML or JSON interface:

```typescript
app.setNotFoundHandler((req: Request, res: Response) => {
    res.setStatus(404).json({
        error: "Route Not Found",
        path: req.path(),
        timestamp: "2026-08-02"
    });
});
```

---

## 📋 Quick API Reference (Cheat Sheet)

| Method | Returns | Usage Example |
| :--- | :--- | :--- |
| `req.param(key)` | `string` | `let userId = req.param("id");` |
| `req.query(key)` | `string` | `let page = req.query("page");` |
| `res.json(data)` | `Response` | `res.json({ success: true });` |
| `res.html(content)` | `Response` | `res.html("<h1>Title</h1>");` |
| `res.setStatus(code)`| `Response` | `res.setStatus(200);` |
