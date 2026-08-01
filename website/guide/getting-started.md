# Getting Started with Vito 🚀

**Vito** là một Web Framework hiệu năng cao, nhẹ và dễ mở rộng dành cho ngôn ngữ lập trình **VIT**. Framework được thiết kế nhằm mang lại trải nghiệm phát triển (**Developer Experience - DX**) tuyệt vời, zero-config tương tự như Express / Fastify / Hono trong hệ sinh thái JavaScript/TypeScript, nhưng chạy trực tiếp trên **VIT Native Engine & Runtime**.

---

## 🛠 Cài Đặt 1 Dòng Lệnh (1-Line Quick Install)

Cài đặt trọn bộ **Vit Compiler Engine** và **Vito CLI** chỉ với 1 dòng lệnh duy nhất (Tự động thiết lập PATH & kiểm tra Toolchain):

::: code-group

```powershell [Windows (PowerShell)]
iwr -useb https://raw.githubusercontent.com/longgoll/vit/main/install.ps1 | iex
```

```bash [Linux / macOS]
curl -fsSL https://raw.githubusercontent.com/longgoll/vit/main/install.sh | bash
```

:::

### 1. Tự Động Kiểm Tra Toolchain (`vit setup`)
Sau khi cài đặt xong, bạn có thể kiểm tra môi trường bằng lệnh:

```bash
vit setup
```

Hệ thống sẽ tự động xác minh đường dẫn `PATH` và kiểm tra bộ trình biên dịch `Clang/LLVM` đi kèm.

---

## ⚡ Khởi Tạo Dự Án Mẫu Siêu Tốc (`vit init`)

Chỉ với 1 lệnh đơn giản, `vit` sẽ tự động tạo thư mục dự án với cấu hình chuẩn và file `main.vit`:

```bash
vit init my-web-app
cd my-web-app
```

Cấu trúc dự án được khởi tạo bao gồm:
```text
my-web-app/
├── vit.json          # File cấu hình thông tin dự án
└── src/
    └── main.vit      # Entrypoint chính chứa ứng dụng Web Vito
```

---

## 💻 Viết Ứng Dụng Web API Đầu Tiên (`main.vit`)

Mở file `src/main.vit` và nhập mã nguồn:

```typescript
import { Vito, Request, Response } from "vito";

function main(): number {
    // 1. Khởi tạo Vito Web App Engine
    let app = Vito.new();

    // 2. Route GET HTML
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    // 3. Route GET JSON API
    app.get("/api/health", (req: Request, res: Response) => {
        res.json({ status: "UP", engine: "Vit Native", uptime: "ok" });
    });

    // 4. Lắng nghe cổng 3000
    app.listen(3000);
    print("⚡ Vito Server running at http://localhost:3000");
    return 0;
}
```

---

## 🚀 Chạy Ứng Dụng Với Live-Reload (`vit dev`)

Khởi chạy ứng dụng ở chế độ phát triển với công cụ **Live-Reloading & File Watcher**:

```bash
vit dev
```

> 💡 **Tính năng Live-Reload**: Khi bạn chỉnh sửa bất kỳ file `.vit` hoặc `.json` nào trong thư mục `src/`, `vit dev` sẽ tự động biên dịch và khởi động lại server tức thì trong vài millisecond!

Mở trình duyệt tại địa chỉ `http://localhost:3000` để trải nghiệm kết quả! 🎉

---

## 🧩 Tích Hợp VS Code Extension & Intellisense

Để có trải nghiệm lập trình tốt nhất với **Autocomplete**, **Syntax Highlighting** và **Code Snippets**:

1. Tải về extension **[vscode-vit](https://github.com/longgoll/vit/tree/main/editors/vscode-vit)**.
2. Mở file `.vit` trong VS Code.
3. Gõ `vito-server` hoặc `vito-get` và nhấn `Tab` để tự động sinh khung code mẫu!
4. Extension sẽ tự động kết nối với `vit-lsp` để hiển thị lỗi và gợi ý hàm thời gian thực.

---

## ⚡ Thử Nghiệm Ngay Trên Web (Zero-Install Playground)

Nếu bạn chưa muốn cài đặt vào máy, hãy truy cập ngay **[Interactive Playground](/playground)** trên trang web này để viết và chạy thử code Vito trực tiếp bằng bộ máy WebAssembly (WASI) trong 0 giây!
