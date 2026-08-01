# Routing & Parameters 🎯

The Routing system in **Vito** is built on top of a **Radix Trie / Segment Matcher Engine**, ensuring fast and precise path matching with support for dynamic parameters and route grouping.

---

## 🛣 Basic Route Definition

Vito supports standard HTTP verbs:

```javascript
app.get("/users", (req, res) => { res.send("GET /users"); });
app.post("/users", (req, res) => { res.send("POST /users"); });
app.put("/users", (req, res) => { res.send("PUT /users"); });
app.delete("/users", (req, res) => { res.send("DELETE /users"); });
```

---

## 🎯 Dynamic Route Parameters (`:param`)

Dynamic route segments are specified with a colon `:` prefix. Retrieve parameter values using `req.param(key)`:

```javascript
// Matches /users/123 or /users/abc
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id");
    res.json("{\"user_id\":\"" + userId + "\"}");
});

// Matches multi-level dynamic segments: /products/electronics/laptop-1
app.get("/products/:category/:id", (req: Request, res: Response) => {
    let cat = req.param("category");
    let pId = req.param("id");
    res.json("{\"category\":\"" + cat + "\", \"item_id\":\"" + pId + "\"}");
});
```

---

## 🔍 URL Search Query Parameters (`?key=value`)

Extract query string parameters from URLs (e.g., `/search?q=vit-lang&page=2`) using `req.query(key)`:

```javascript
app.get("/search", (req: Request, res: Response) => {
    let query = req.query("q");
    let page = req.query("page");
    res.json("{\"search_term\":\"" + query + "\", \"page\":\"" + page + "\"}");
});
```

---

## 👥 Route Grouping (`app.group`)

Group routes under a common URL prefix to keep your API modular and clean:

```javascript
app.group("/api/v1", (v1) => {
    // Actual URL: /api/v1/users
    v1.get("/users", (req, res) => {
        res.json("[\"Alice\", \"Bob\"]");
    });

    // Actual URL: /api/v1/users/:id
    v1.get("/users/:id", (req, res) => {
        let id = req.param("id");
        res.json("{\"id\":\"" + id + "\"}");
    });
});
```

---

## 🎨 Custom 404 Not Found & Error Handling

Customize the 404 response using `app.setNotFoundHandler`:

```javascript
app.setNotFoundHandler((req: Request, res: Response) => {
    res.setStatus(404).html("""
        <div style="text-align: center; padding: 50px;">
            <h1>404 - Page Not Found 😢</h1>
            <p>The requested route does not exist on the Vito server!</p>
        </div>
    """);
});
```
