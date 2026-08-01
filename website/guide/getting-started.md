# Khởi Đầu Siêu Tốc (Quick Start Guide) 🚀

Chào mừng bạn đến với **Vito Framework** — Web Framework hiệu năng cao, siêu nhẹ và giàu tính năng được thiết kế riêng cho ngôn ngữ **VIT**.

::: tip 💡 Trải Nghiệm Học Tập Tương Tác
Sử dụng bộ công cụ tương tác 4 bước bên dưới để thực hành theo lộ trình cài đặt, viết code và vận hành Web Server Vito trong **3 phút**!
:::

<QuickStartFlow />

---

## ⚡ Các Tính Năng Nổi Bật Của Vito Framework

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4>Cực Kỳ Nhanh (Native Speed)</h4>
    <p>Chạy trực tiếp trên VIT Native Engine với bộ phân luồng Async I/O đạt 245K+ req/s.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🧩</div>
    <h4>Cú Pháp Mạch Lạc (Expressive DX)</h4>
    <p>Cú pháp quen thuộc gần gũi với Express / Fastify / Hono giúp lập trình viên tiếp cận tức thì.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4>Zero-Config Live Reload</h4>
    <p>Biên dịch & tự động cập nhật Server trong vài millisecond ngay khi bạn bấm lưu file.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🛠️</div>
    <h4>Bộ Công Cụ CLI Tích Hợp</h4>
    <p>Hỗ trợ khởi tạo dự án `vit init`, kiểm tra `vit setup` và biên dịch `vit build` tiện lợi.</p>
  </div>
</div>

---

## 💻 Mã Nguồn Dự Án Mẫu Chi Tiết (`src/main.vit`)

Dưới đây là cấu trúc hoàn chỉnh của một ứng dụng Web API thương mại viết bằng **Vito**:

```typescript
import { Vito, Request, Response } from "vito";

function main(): number {
    // 1. Khởi tạo Vito App Engine
    let app = Vito.new();

    // 2. Định nghĩa Route GET HTML
    app.get("/", (req: Request, res: Response) => {
        res.html("""
            <!DOCTYPE html>
            <html>
            <head><title>Vito Web Server</title></head>
            <body style="font-family: sans-serif; text-align: center; padding: 50px;">
                <h1>⚡ Welcome to Vito Web Framework!</h1>
                <p>Native speed, zero overhead, modern DX.</p>
            </body>
            </html>
        """);
    });

    // 3. Định nghĩa API Health Check (JSON)
    app.get("/api/v1/health", (req: Request, res: Response) => {
        res.json({
            status: "UP",
            engine: "Vit Native Phase 9",
            uptime_seconds: 3600
        });
    });

    // 4. Lắng nghe tại cổng 3000
    app.listen(3000);
    print("⚡ Vito HTTP Server is listening at http://localhost:3000");
    return 0;
}
```

---

## 🛠 Tra Cứu Nhanh Lệnh CLI (Developer Cheat Sheet)

| Lệnh CLI | Tác dụng | Trường hợp sử dụng |
| :--- | :--- | :--- |
| `vit setup` | Kiểm tra môi trường PATH & LLVM Toolchain | Thực thi 1 lần sau khi cài đặt |
| `vit init <app-name>` | Tự động tạo thư mục dự án Vito chuẩn | Khởi tạo dự án mới |
| `vit dev` | Chạy dev server với Live-Reload tự động | Sử dụng trong quá trình phát triển |
| `vit build` | Đóng gói ra file Binary Native tối ưu | Sử dụng để deploy Server Production |

---

## 🧩 Các Bước Tiếp Theo

Bây giờ bạn đã sẵn sàng khám phá sâu hơn về hệ sinh thái Vito Framework:

- 🎯 **[Định Tuyến & Route Params](/guide/routing)**: Học cách xử lý URL Dynamic Parameters, Query Strings và Route Grouping.
- 🧱 **[Hệ Thống Middleware](/guide/middleware)**: Xây dựng Logger, CORS, Rate Limiting và Auth Guard.
- ⚡ **[Thử Nghiệm Trực Tiếp Trên Browser](/playground)**: Trải nghiệm viết và chạy code Vito trực tiếp bằng WebAssembly!
