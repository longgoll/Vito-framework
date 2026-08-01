# CLI & Developer Tooling 🛠

Bộ công cụ **`vit` CLI** được tích hợp sẵn theo triết lý **All-in-One**, mang lại trải nghiệm phát triển mượt mà và tiện lợi tối đa cho lập trình viên mà không cần thêm công cụ bên thứ ba.

---

## 🛠 Danh Sách Lệnh CLI Chính (`vit <command>`)

### 1. `vit init [project-name]`
Khởi tạo cấu trúc dự án Vit/Vito mới với file cấu hình `vit.json` và code mẫu `src/main.vit`:

```bash
vit init my-awesome-api
cd my-awesome-api
```

---

### 2. `vit dev [file]` (Live-Reload Server)
Khởi chạy Development Server hỗ trợ **Live-Reloading & File Watcher**. Tự động theo dõi các file nguồn `.vit` và tự động khởi động lại ứng dụng tức thì ngay khi bạn lưu file (Ctrl+S):

```bash
vit dev
# Hoặc chỉ định file cụ thể:
vit dev src/main.vit
```

---

### 3. `vit run <file>`
Biên dịch và thực thi ứng dụng Vit trực tiếp bằng JIT Compiler siêu tốc mà không xuất file ra đĩa:

```bash
vit run src/main.vit
```

---

### 4. `vit build <file> [options]`
Biên dịch AOT ứng dụng thành **Single Standalone Executable Binary** siêu nhỏ gọn (< 5MB), không phụ thuộc môi trường khi deploy production:

```bash
# Biên dịch AOT mặc định
vit build src/main.vit -o ./dist/server.exe

# Biên dịch tối ưu hóa cao cấp (-O3 -march=native --lto=thin)
vit build src/main.vit -O3 -march=native --lto=thin -o ./dist/server_fast.exe

# Biên dịch ra WebAssembly (WASI)
vit build src/main.vit --target wasm32-wasi -o ./dist/app.wasm
```

---

### 5. `vit setup`
Tự động cấu hình biến môi trường `VIT_HOME` và tự động thêm thư mục chứa `vit` vào User `PATH` trong Hệ điều hành Windows/Linux/macOS:

```bash
vit setup
```

---

### 6. `vit fmt [path]`
Code Formatter tự động định dạng và chuẩn hóa thụt lề, dấu ngoặc cho mã nguồn Vit:

```bash
vit fmt src/
```

---

### 7. `vit lint [path]`
Linter tích hợp kiểm tra tĩnh mã nguồn, phát hiện các nguy cơ tiềm ẩn và gợi ý tối ưu hiệu năng:

```bash
vit lint src/main.vit
```

---

### 8. `vit lsp`
Khởi chạy **Language Server Protocol (JSON-RPC)** qua stdin/stdout, phục vụ kết nối trực tiếp với các IDE như VS Code, Neovim, Sublime Text.

```bash
vit lsp
```

---

## 🧪 In-Memory Testing Injector (`app.inject`)

Vito hỗ trợ phương thức thử nghiệm trực tiếp API trong bộ nhớ mà không cần mở cổng TCP socket thực tế:

```typescript
// Test GET /users/:id trực tiếp trong bộ nhớ
let res = app.inject("GET", "/users/42");

// Kiểm tra Status Code và Body
if (res.statusCode == 200) {
    print("Test Passed: " + res.body);
}
```
