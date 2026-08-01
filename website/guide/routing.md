# Định Tuyến & Route Parameters 🎯

Hệ thống định tuyến (**Router Engine**) của **Vito Framework** được thiết kế dựa trên thuật toán **Radix Trie (Phase 9 Engine)**, kết hợp giữa tốc độ phản hồi cực đại và khả năng trích xuất tham số linh hoạt.

<RouteVisualizer />

---

## ⚡ 1. Các Phương Thức HTTP Cơ Bản

Vito cung cấp cú pháp cực kỳ ngắn gọn cho tất cả các HTTP Methods phổ biến:

<div class="card-grid">
  <div class="feature-mini-card">
    <span class="method-badge get">GET</span>
    <h4>Truy Vấn Dữ Liệu</h4>
    <p>Sử dụng `app.get("/path", handler)` để xử lý câu lệnh lấy dữ liệu hoặc trang HTML.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge post">POST</span>
    <h4>Tạo Mới Dữ Liệu</h4>
    <p>Sử dụng `app.post("/path", handler)` để tiếp nhận dữ liệu từ Form hoặc Body JSON.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge put">PUT</span>
    <h4>Cập Nhật Dữ Liệu</h4>
    <p>Sử dụng `app.put("/path", handler)` để ghi đè hoặc chỉnh sửa tài nguyên hiện có.</p>
  </div>

  <div class="feature-mini-card">
    <span class="method-badge delete">DELETE</span>
    <h4>Xóa Tài Nguyên</h4>
    <p>Sử dụng `app.delete("/path", handler)` để thực hiện các thao tác gỡ bỏ tài nguyên.</p>
  </div>
</div>

```typescript
app.get("/users", (req: Request, res: Response) => { res.send("GET /users"); });
app.post("/users", (req: Request, res: Response) => { res.send("POST /users"); });
app.put("/users/:id", (req: Request, res: Response) => { res.send("PUT /users"); });
app.delete("/users/:id", (req: Request, res: Response) => { res.send("DELETE /users"); });
```

---

## 🎯 2. Route Parameters Động & Wildcard

Bạn có thể dễ dàng định nghĩa tham số URL với cú pháp `:paramName` hoặc wildcard nhiều phân đoạn `*filepath`:

::: code-group

```typescript [1. Param Động Single Segment (:id)]
// URL thực tế: /users/101 hoặc /users/alex
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id");
    res.json({ status: "success", user_id: userId });
});
```

```typescript [2. Multi Params (:category/:id)]
// URL thực tế: /products/electronics/laptop-m3
app.get("/products/:category/:id", (req: Request, res: Response) => {
    let cat = req.param("category");
    let itemId = req.param("id");
    res.json({ category: cat, item_id: itemId });
});
```

```typescript [3. Wildcard Path (*filepath)]
// URL thực tế: /files/docs/2026/q3/report.pdf
app.get("/files/*filepath", (req: Request, res: Response) => {
    let path = req.param("filepath");
    res.send("Requested path: " + path);
});
```

:::

---

## 🔍 3. URL Search Query String (`?key=value`)

Để trích xuất các tham số từ Query String (ví dụ: `/search?q=vit-lang&page=2`), sử dụng phương thức `req.query(key)`:

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

## 👥 4. Gom Nhóm Route Mô-đun (Route Grouping)

Quản lý tiền tố đường dẫn (Prefix) và Middleware tập trung bằng tính năng **Route Grouping**:

```typescript
// Tất cả các route bên trong đều có prefix '/api/v1'
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

## 🎨 5. Tùy Biến Trang Lỗi 404 (Not Found Handler)

Dễ dàng thay thế trang thông báo lỗi 404 mặc định bằng giao diện HTML hoặc JSON chuẩn của dự án:

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

## 📋 Tra Cứu Nhanh API Reference (Cheat Sheet)

| Phương thức | Trả về | Ví dụ sử dụng |
| :--- | :--- | :--- |
| `req.param(key)` | `string` | `let userId = req.param("id");` |
| `req.query(key)` | `string` | `let page = req.query("page");` |
| `res.json(data)` | `Response` | `res.json({ success: true });` |
| `res.html(content)` | `Response` | `res.html("<h1>Title</h1>");` |
| `res.setStatus(code)`| `Response` | `res.setStatus(200);` |
