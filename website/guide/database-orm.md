# Quản Lý CSDL & Vito ORM 🗄️

**Vito Framework** cung cấp giải pháp quản lý cơ sở dữ liệu và ORM chuẩn Enterprise, tích hợp sẵn Connection Pooling tự động, Transaction Management an toàn và Auto Schema Migrations.

---

## 🏊 Connection Pool Manager (`packages/db/db_pool.vit`)

Connection Pool Manager giúp tự động quản lý vòng đời kết nối CSDL, tối ưu hiệu năng và tự động phục hồi kết nối đứt gãy.

### Tính Năng Nổi Bật:
- **Dynamic Connection Pooling**: Tự động mở rộng số lượng kết nối tới CSDL khi tải tăng cao và giới hạn theo `minConnections` và `maxConnections`.
- **Fast Auto-Reconnect (< 100ms)**: Tự động phát hiện gián đoạn kết nối CSDL và tái tạo kết nối ngầm.
- **Idle Connection Cleanup**: Tự động dọn dẹp các kết nối nhàn rỗi vượt quá ngưỡng `idleTimeoutMs`.

### Ví Dụ Sử Dụng:

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

// Khởi tạo Connection Pool cho PostgreSQL
let pool = createConnectionPool(
    "PostgreSQL",
    "postgresql://vito_admin:secret@localhost:5432/vito_prod",
    2, // minConnections
    10 // maxConnections
);

// Mượn kết nối từ Pool
let connId = pool.acquireConnection();

// Giải phóng kết nối về Pool sau khi hoàn tất
pool.releaseConnection(connId, 1000);

// Tự động kiểm tra sức khỏe và phục hồi kết nối đứt gãy (< 100ms)
let restoredCount = pool.checkHealthAndReconnect();

// Thu hồi kết nối nhàn rỗi
pool.reclaimIdleConnections(5000);
```

---

## 🔄 Transaction Management Engine (`packages/orm/transaction.vit`)

Hỗ trợ giao dịch CSDL nguyên tử (Atomic Transactions), đảm bảo tính toàn vẹn dữ liệu **ACID 100%** và điểm lưu (Savepoint) cho các giao dịch lồng nhau.

### Ví Dụ Sử Dụng Transaction & ACID Rollback:

```typescript
import { beginTransaction } from "vito/packages/orm/transaction.vit";

// 1. Giao dịch thành công (COMMIT)
let tx = beginTransaction(1001);
tx.execute("INSERT INTO users (name, email) VALUES ('Alice', 'alice@vito.dev')");
tx.execute("UPDATE account_balances SET amount = amount - 500 WHERE user_id = 10");
tx.commit(); // Xử lý commit an toàn

// 2. Giao dịch gặp lỗi (ROLLBACK tự động)
let tx2 = beginTransaction(1002);
tx2.execute("INSERT INTO orders (id, total) VALUES (501, 1200)");
// Khi phát hiện lỗi trong quá trình xử lý:
tx2.rollback(); // Toàn bộ câu lệnh trước đó bị hủy bỏ hoàn toàn (100% ACID)
```

### Nested Transactions & Savepoints:

```typescript
let tx3 = beginTransaction(1003);
tx3.execute("INSERT INTO audit_logs (action) VALUES ('CHECKOUT')");

// Tạo điểm lưu Savepoint
tx3.createSavepoint("sp_audit_done");

// Thử hiện các câu lệnh phụ
tx3.execute("UPDATE coupons SET used = true WHERE code = 'DISCOUNT50'");

// Khi gặp lỗi ở câu lệnh phụ, chỉ khôi phục về Savepoint thay vì hủy toàn bộ
tx3.rollbackToSavepoint("sp_audit_done");

tx3.commit();
```

---

## 📜 Migration Runner & Database Seeder (`packages/orm/migration.vit`)

Quản lý lịch sử nâng cấp cấu trúc bảng (Up/Down Migrations) và công cụ đổ dữ liệu mẫu (Seeder) cho Dev & Staging.

### 1. Schema Migration Engine

```typescript
import { createMigrationRunner, createSchemaDiffGenerator } from "vito/packages/orm/migration.vit";

let migrator = createMigrationRunner();

// Đăng ký migration
migrator.registerMigration(
    "20260801_001",
    "create_users_table",
    "CREATE TABLE users (id SERIAL PRIMARY KEY, username VARCHAR(50));",
    "DROP TABLE users;"
);

// Chạy Migration (Lưu vết tự động vào bảng `schema_migrations`)
migrator.runUp();

// Revert migration nếu cần
migrator.runDown();
```

### 2. Schema Diff Generator

```typescript
let diffGen = createSchemaDiffGenerator();
let diffSql = diffGen.generateDiffSql("Product", "products", "", "id INT, title VARCHAR(100), price NUMERIC");
// Trả về câu lệnh SQL: CREATE TABLE products (id INT, title VARCHAR(100), price NUMERIC);
```

### 3. Database Seeder

```typescript
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder();
seeder.addSeed("users", '{"username":"vito_admin","email":"admin@vito.dev"}');
seeder.addSeed("products", '{"title":"Vito Enterprise Server","price":999}');

// Nạp dữ liệu vào CSDL
seeder.runSeeder();
```
