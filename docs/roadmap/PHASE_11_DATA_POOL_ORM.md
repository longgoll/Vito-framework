# PHASE 11: Enterprise Database Connection Pool & Advanced ORM 🗄️

> **Mục tiêu**: Nâng cấp hệ thống kết nối cơ sở dữ liệu Vito DB & Vito ORM lên chuẩn Enterprise: Connection Pooling tự động, Transaction Management an toàn và Auto Schema Migrations linh hoạt.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 🏊 Connection Pool Manager (`packages/db/pool`)
- [ ] **Dynamic Connection Pooling**: Tự động mở rộng/co hẹp số lượng kết nối tới PostgreSQL, SQLite, MySQL theo tải (`minConnections`, `maxConnections`).
- [ ] **Connection Health Check & Reconnect**: Tự động phát hiện kết nối đứt gãy và khôi phục kết nối ngầm.
- [ ] **Idle Connection Timeout**: Tự động giải phóng kết nối nhàn rỗi để tiết kiệm tài nguyên database server.

### 2. 🔄 Transaction Management Engine (`packages/orm/transaction`)
- [ ] **Atomic Transactions**: Hỗ trợ cú pháp `orm.transaction(async (tx) => { ... })` với tự động `COMMIT` khi thành công và `ROLLBACK` khi có lỗi.
- [ ] **Nested Transactions / Savepoints**: Hỗ trợ điểm lưu (Savepoint) cho các giao dịch lồng nhau.

### 3. 📜 Migration & Seeding Runner (`packages/orm/migration`)
- [ ] **Schema Migration Engine**: Quản lý lịch sử thay đổi cấu trúc bảng (Up/Down migrations) với các file timestamped SQL/Code.
- [ ] **Database Seeder**: Công cụ đổ dữ liệu mẫu (Mock data) cho môi trường Development & Staging.
- [ ] **Schema Diff Generator**: Tự động so sánh Model Vito ORM với DB thực tế để sinh file Migration.

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Tự động phục hồi kết nối CSDL trong vòng $< 100ms$ khi DB server bị gián đoạn chớp nhoáng.
2. Đảm bảo tính toàn vẹn dữ liệu (ACID) 100% trong các bài test Rollback Transaction khi gặp lỗi.
3. Chạy lệnh `vit db:migrate` thành công và cập nhật bảng `schema_migrations` chính xác.
