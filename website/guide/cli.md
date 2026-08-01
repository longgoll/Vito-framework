# CLI & Developer Tooling 🛠

Bộ công cụ **`vit` CLI** mang lại trải nghiệm phát triển hiện đại và tiện lợi tối đa cho lập trình viên.

---

## 🛠 Bộ Lệnh CLI Chính

### 1. `vit create <app-name>`
Tạo dự án Web Vito mới với cấu hình chuẩn và mẫu code có sẵn:

```bash
vit create my-awesome-api
cd my-awesome-api
```

---

### 2. `vit dev` (Hot Module Reloading)
Khởi chạy Development Server hỗ trợ theo dõi thay đổi tập tin nguồn và tự động reload server tức thì mà không cần khởi động lại thủ công:

```bash
vit dev
```

---

### 3. `vit build --binary`
Đóng gói toàn bộ ứng dụng web và framework Vito thành **1 file thực thi đơn lẻ (Standalone Executable Binary)** siêu nhỏ gọn (< 5MB), không cần môi trường runtime phụ thuộc khi deploy sản xuất:

```bash
vit build --binary --output ./dist/server
```

---

## 🧪 In-Memory Testing Injector (`app.inject`)

Vito hỗ trợ phương thức thử nghiệm trực tiếp API không cần mở cổng TCP socket thực tế:

```javascript
// Test GET /users/:id trực tiếp trong bộ nhớ
let res = app.inject("GET", "/users/42");

// Kiểm tra Status Code và Body
if (res.statusCode == 200) {
    print("Test Passed: " + res.body);
}
```
