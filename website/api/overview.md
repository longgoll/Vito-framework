# Vito API Reference 📚

Tài liệu chi tiết về các hàm, đối tượng chính trong **Vito Web Framework Core Engine**.

---

## 🏗 `VitoApp` Class

### `createApp()`
Khởi tạo một thể hiện mới của ứng dụng Vito.

```javascript
let app = createApp();
```

### `app.get(path, handler)` / `app.post(path, handler)` / `app.put()` / `app.delete()`
Đăng ký route handler cho phương thức HTTP tương ứng.

### `app.group(prefix, callback)`
Tạo nhóm các đường dẫn với chung tiền tố URL.

### `app.use(middleware)`
Thêm middleware vào pipeline xử lý request.

### `app.setNotFoundHandler(handler)`
Thiết lập handler tùy chỉnh khi không tìm thấy route (404 Not Found).

### `app.listen(port)`
Mở socket và bắt đầu lắng nghe kết nối HTTP trên cổng được chỉ định.

---

## 📥 `Request` Class

| Phương Thức | Mô Tả | Ví Dụ |
| :--- | :--- | :--- |
| `req.param(key)` | Lấy giá trị tham số dynamic path `:key` | `req.param("id")` |
| `req.query(key)` | Lấy giá trị search query string `?key=val` | `req.query("page")` |
| `req.header(key)` | Lấy giá trị HTTP request header | `req.header("Authorization")` |
| `req.method` | Thuộc tính trả về phương thức HTTP (GET, POST...) | `req.method` |
| `req.path` | Thuộc tính trả về đường dẫn URL | `req.path` |

---

## 📤 `Response` Class

| Phương Thức | Mô Tả | Ví Dụ |
| :--- | :--- | :--- |
| `res.setStatus(code)` | Thiết lập HTTP Status Code (200, 404, 500...) | `res.setStatus(200)` |
| `res.setHeader(name, val)`| Thiết lập HTTP Response Header | `res.setHeader("X-Powered-By", "Vito")` |
| `res.json(jsonStr)` | Trả về chuỗi JSON với header `Content-Type: application/json` | `res.json("{\"ok\":true}")` |
| `res.html(htmlStr)` | Trả về nội dung HTML với `Content-Type: text/html` | `res.html("<h1>Hi</h1>")` |
| `res.send(textStr)` | Trả về chuỗi Plain Text | `res.send("Hello World")` |
| `res.sendChunk(chunk)` | Trả về dữ liệu kiểu Stream / Chunked response | `res.sendChunk(data)` |
