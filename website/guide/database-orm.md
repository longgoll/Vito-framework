# Quản Lý CSDL & Vito ORM Enterprise 🗄️

**Vito Framework** tích hợp sẵn bộ quản lý Cơ sở dữ liệu và **Vito ORM** cấp Enterprise, bao gồm Connection Pooling tự động, Giao dịch nguyên tử ACID Transactions 100%, Savepoints và Schema Auto-Migrations.

---

## 🏊 1. Connection Pool Manager (`packages/db/db_pool.vit`)

Quản lý vòng đời kết nối CSDL thông minh, tự động khôi phục kết nối bị gián đoạn và tối ưu hóa tài nguyên phần cứng.

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Dynamic Connection Pooling</h4>
    <p>Tự động mở rộng kết nối theo tải thực tế trong khoảng `minConnections` và `maxConnections`.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔄</div>
    <h4>Fast Auto-Reconnect (< 100ms)</h4>
    <p>Tự động phát hiện đứt gãy mạng và tái tạo kết nối ngầm tức thì.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🧹</div>
    <h4>Idle Connection Eviction</h4>
    <p>Thu hồi kết nối nhàn rỗi vượt quá ngưỡng `idleTimeoutMs` để giải phóng RAM.</p>
  </div>
</div>

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

// Khởi tạo Connection Pool cho PostgreSQL
let pool = createConnectionPool(
    "PostgreSQL",
    "postgresql://admin:secret@localhost:5432/vito_db",
    2,  // minConnections
    10  // maxConnections
);

// Mượn kết nối từ Pool
let connId = pool.acquireConnection();

// Giải phóng kết nối về lại Pool
pool.releaseConnection(connId, 1000);
```

---

## 🔄 2. Transaction Management & Savepoints (`packages/orm/transaction.vit`)

Đảm bảo tính toàn vẹn dữ liệu tuyệt đối (**ACID 100%**) cho các thao tác tài chính và ngân hàng:

::: code-group

```typescript [1. Atomic Commit / Rollback]
import { beginTransaction } from "vito/packages/orm/transaction.vit";

// Khởi tạo Transaction
let tx = beginTransaction(1001);

try {
    tx.execute("UPDATE accounts SET balance = balance - 500 WHERE id = 10");
    tx.execute("UPDATE accounts SET balance = balance + 500 WHERE id = 20");
    
    // Commit an toàn nếu không xảy ra lỗi
    tx.commit();
} catch (e) {
    // Tự động Rollback 100% nếu có lỗi
    tx.rollback();
}
```

```typescript [2. Nested Savepoints]
let tx = beginTransaction(1002);
tx.execute("INSERT INTO orders (id, user_id) VALUES (501, 12)");

// Tạo điểm lưu Savepoint
tx.createSavepoint("sp_order_created");

// Thử áp dụng mã giảm giá
tx.execute("UPDATE coupons SET used = true WHERE code = 'PROMO50'");

// Khi gặp sự cố với mã giảm giá, chỉ rollback về Savepoint thay vì hủy đơn hàng
tx.rollbackToSavepoint("sp_order_created");

tx.commit(); // Đơn hàng vẫn được lưu an toàn!
```

:::

---

## 📜 3. Schema Migration & Seeder (`packages/orm/migration.vit`)

Quản lý lịch sử thay đổi cấu trúc bảng (Up/Down Migrations) và công cụ đổ dữ liệu mẫu:

::: code-group

```typescript [1. Schema Migration Runner]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

let migrator = createMigrationRunner();

// Đăng ký Migration
migrator.registerMigration(
    "20260802_001",
    "create_users_table",
    "CREATE TABLE users (id SERIAL PRIMARY KEY, username VARCHAR(50), email VARCHAR(100));",
    "DROP TABLE users;"
);

// Tiến hành nâng cấp Schema
migrator.runUp();
```

```typescript [2. Database Seeder]
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder();

seeder.addSeed("users", JSON.stringify({ username: "admin", role: "SuperAdmin" }));
seeder.addSeed("products", JSON.stringify({ name: "Vito Enterprise", price: 999 }));

// Thực thi Nạp dữ liệu
seeder.runSeeder();
```

:::
