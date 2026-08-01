# Bộ Công Cụ Vito CLI & Developer Tooling 🛠️

**Vito CLI** là công cụ dòng lệnh đa năng giúp tự động hóa quy trình phát triển, từ khởi tạo dự án, live-reload watcher, chạy migration CSDL đến biên dịch đóng gói sản phẩm ra file Native Binary.

---

## ⚡ Các Lệnh CLI Thường Dùng

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4><code>vit init &lt;app-name&gt;</code></h4>
    <p>Khởi tạo thư mục dự án Vito mới với cấu trúc chuẩn và các file mẫu.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4><code>vit dev</code></h4>
    <p>Khởi chạy Dev Server với công cụ File Watcher & Hot-Reloading siêu tốc.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4><code>vit build</code></h4>
    <p>Biên dịch mã nguồn ra file Binary Native tối ưu hóa tối đa cho Production.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔍</div>
    <h4><code>vit setup</code></h4>
    <p>Kiểm tra biến môi trường PATH, bộ dịch Clang/LLVM và công cụ đi kèm.</p>
  </div>
</div>

---

## 🗄️ Quản Lý CSDL với Vito CLI Migration Runner

::: code-group

```bash [1. Tạo Migration Mới]
vit migrate create add_users_table
# -> Đã tạo file: migrations/20260802_001_add_users_table.vit
```

```bash [2. Nâng Cấp Schema (Up)]
vit migrate up
# -> [✓] Executed migration: 20260802_001_add_users_table (4ms)
```

```bash [3. Khôi Phục Schema (Down)]
vit migrate rollback
# -> [✓] Reverted migration: 20260802_001_add_users_table (2ms)
```

:::

---

## 🛠 Tra Cứu Toàn Bộ Lệnh CLI (Cheat Sheet)

| Câu lệnh CLI | Tham số / Tuỳ chọn | Mục đích sử dụng |
| :--- | :--- | :--- |
| `vit setup` | N/A | Xác minh Toolchain & PATH |
| `vit init` | `<app-name>` | Tạo khung dự án mới |
| `vit dev` | `--port 3000` | Khởi chạy Dev Server Hot-Reload |
| `vit build` | `--release --target native` | Biên dịch Binary Production |
| `vit migrate up` | N/A | Chạy tất cả các migrations chưa áp dụng |
| `vit migrate rollback` | N/A | Khôi phục 1 bước migration gần nhất |
| `vit seed` | N/A | Đổ dữ liệu mẫu (Seeder) vào CSDL |
