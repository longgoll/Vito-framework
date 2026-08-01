# Routing & Parameters 🎯

Hệ thống định tuyến (Router) trong **Vito** dựa trên **Radix Trie / Segment Matcher Engine** thế hệ 9 (Phase 9 Performance Engine), giúp khớp đường dẫn chính xác với tốc độ tối ưu $O(1)$ Fast Path và hỗ trợ nhiều dạng dynamic parameters & wildcard matching.

---

## 🛣 Định Nghĩa Route Cơ Bản

Vito hỗ trợ đầy đủ các phương thức HTTP chuẩn:

```javascript
app.get("/users", (req, res) => { res.send("GET /users"); });
app.post("/users", (req, res) => { res.send("POST /users"); });
app.put("/users", (req, res) => { res.send("PUT /users"); });
app.delete("/users", (req, res) => { res.send("DELETE /users"); });
```

---

## ⚡ Static Route Fast Path ($O(1)$ Direct Lookup)

Đối với các đường dẫn tĩnh (Static Routes), Vito tự động kích hoạt luồng **Static Fast Path** phân tách với độ phức tạp $O(1)$ bỏ qua giai đoạn duyệt cây Trie động, mang lại tốc độ phản hồi tối đa:

```javascript
app.get("/health", (req, res) => { res.json("{\"status\":\"healthy\"}"); });
app.get("/metrics", (req, res) => { res.json("{\"uptime\": 3600}"); });
```

---

## 🎯 Route Parameters Động (`:param` & `*wildcard`)

Bạn có thể khai báo các phân đoạn tham số động bằng dấu cú pháp `:paramName` hoặc đường dẫn đa cấp wildcard `*filepath`. Truy xuất giá trị tham số thông qua hàm `req.param(key)`:

```javascript
// Khớp URL dạng /users/123 hoặc /users/abc
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id");
    res.json("{\"user_id\":\"" + userId + "\"}");
});

// Khớp URL nhiều cấp: /products/electronics/laptop-1
app.get("/products/:category/:id", (req: Request, res: Response) => {
    let cat = req.param("category");
    let pId = req.param("id");
    res.json("{\"category\":\"" + cat + "\", \"item_id\":\"" + pId + "\"}");
});

// Khớp Wildcard đa phân đoạn: /files/docs/2026/report.pdf
app.get("/files/*filepath", (req: Request, res: Response) => {
    let filePath = req.param("filepath");
    res.send("Requested file: " + filePath);
});
```

---

## 🔄 Trailing Slash Normalization (Strict vs Lax Mode)

Mặc định, Vito hoạt động ở chế độ **Lax Trailing Slash**, tự động chuẩn hóa `/users/` tương đương `/users` mà không cần ghi đè hoặc khai báo 2 lần. Bạn có thể bật chế độ **Strict Mode** nếu muốn phân biệt chính xác:

```javascript
// Thiết lập Strict Trailing Slash Mode
app.setStrictSlash(true);
```

---

## 🔍 URL Search Query Parameters (`?key=value`)

Để lấy tham số URL search query (ví dụ: `/search?q=vit-lang&page=2`), sử dụng hàm `req.query(key)`:

```javascript
app.get("/search", (req: Request, res: Response) => {
    let query = req.query("q");
    let page = req.query("page");
    res.json("{\"search_term\":\"" + query + "\", \"page\":\"" + page + "\"}");
});
```

---

## 👥 Route Grouping (`app.group`)

Gom nhóm các đường dẫn có chung tiền tố URL (Prefix) giúp cấu trúc mã nguồn mô-đun và gọn gàng hơn:

```javascript
app.group("/api/v1", (v1) => {
    // URL thực tế: /api/v1/users
    v1.get("/users", (req, res) => {
        res.json("[\"Alice\", \"Bob\"]");
    });

    // URL thực tế: /api/v1/users/:id
    v1.get("/users/:id", (req, res) => {
        let id = req.param("id");
        res.json("{\"id\":\"" + id + "\"}");
    });
});
```

---

## 🎨 Custom 404 Not Found & Error Handling

Mặc định Vito sẽ trả về trang 404 tiêu chuẩn. Bạn có thể tùy biến trang lỗi 404 bằng `app.setNotFoundHandler`:

```javascript
app.setNotFoundHandler((req: Request, res: Response) => {
    res.setStatus(404).html("""
        <div style="text-align: center; padding: 50px;">
            <h1>404 - Page Not Found 😢</h1>
            <p>Đường dẫn bạn yêu cầu không tồn tại trên server Vito!</p>
        </div>
    """);
});
```
