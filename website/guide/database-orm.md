# Quản Lý CSDL, ORM & Migration CLI 🗄️

**Vito Framework** tích hợp bộ database driver native zero-allocation cho **PostgreSQL**, **MySQL/MariaDB**, và **SQLite**, kết hợp với **Migration CLI** mạnh mẽ tương tự Prisma/Goose/Flyway — hỗ trợ auto-rollback, dry-run và schema diff tự động.

---

## 🚀 Tổng Quan Hệ Sinh Thái Driver

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🐘</div>
    <h4>PostgreSQL Native Driver</h4>
    <p>Wire Protocol v3.0 — zero-allocation binary decoding, query pipelining, lock-free ring buffer pool.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🐬</div>
    <h4>MySQL/MariaDB Native Driver</h4>
    <p>Wire Protocol v10 — không phụ thuộc libmysqlclient, SHA1 auth, COM_QUERY pipeline batch.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🪶</div>
    <h4>SQLite Native Driver</h4>
    <p>SQLite3 amalgamation — WAL mode mặc định, prepared statements, WAL checkpoint tự động.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🔌</div>
    <h4>Unified DbDriver Interface</h4>
    <p>Một interface duy nhất cho cả 3 backend — ORM và Migration không phụ thuộc vào driver cụ thể.</p>
  </div>
</div>

---

## 1. Kết Nối Database (`packages/db/driver.vit`) {#database-connection}

`DbDriver` là abstraction layer thống nhất. ORM và Migration đều dùng interface này:

::: code-group

```typescript [PostgreSQL]
import { connectPostgres } from "vito/packages/db/driver.vit";

let db = connectPostgres("postgres://admin:secret@127.0.0.1:5432/vito_db");

// Query — trả về DbResult với named column access
let res = db.query("SELECT id, username, email FROM users WHERE id = 1;");
print(res.row0.get("username")); // → "alice"

// Execute — DDL / DML
db.execute("UPDATE users SET last_seen = NOW() WHERE id = 1;");

// Ping
db.ping(); // → true

db.close();
```

```typescript [MySQL / MariaDB]
import { createMySQLClient } from "vito/packages/db/mysql.vit";

let client = createMySQLClient("127.0.0.1", 3306, "root", "secret", "vito_db");

// Query thông thường
let res = client.query("SELECT id, username FROM users LIMIT 10;");
print(res.getCell(0, 0)); // → "1"
print(res.getCell(0, 1)); // → "alice"

// Prepared Statement với fluent bind API
let stmt = client.prepare("SELECT * FROM users WHERE id = ? AND active = ?;");
stmt.bindInt(0, 42).bindText(1, "1");
let result = stmt.execute();

// Pipeline Batch — gửi nhiều SQL trong 1 round-trip (MySQL 8.0+)
let batch = client.pipeline(
    "UPDATE stats SET views = views + 1 WHERE id = 1;" +
    "INSERT INTO access_log (ts) VALUES (NOW());" +
    "SELECT COUNT(*) FROM users;"
);
print("[OK] Batch sent in 1 TCP round-trip.");

client.disconnect();
```

```typescript [SQLite]
import { openSqlite, openSqliteMemory } from "vito/packages/db/sqlite.vit";

// Mở database file (WAL mode tự động bật)
let db = openSqlite("./vito_app.db");

// Hoặc in-memory database cho testing
let memDb = openSqliteMemory();

// DDL
db.exec("CREATE TABLE IF NOT EXISTS products (id INTEGER PRIMARY KEY, name TEXT, price REAL);");

// Query với named column access
let res = db.query("SELECT id, name, price FROM products ORDER BY price DESC;");
print(res.get("name"));        // → giá trị cột 'name' của row 0
print(res.getValue(1, 2));     // → row 1, col 2

// Prepared Statement (1-based indexing như SQLite3 API)
let stmt = db.prepare("INSERT INTO products (name, price) VALUES (?, ?);");
stmt.bindText(1, "Vito Pro").bindInt(2, 99);
stmt.step();
stmt.finalize();

// WAL Checkpoint (đồng bộ WAL frames vào main DB file)
let ckpt = db.checkpoint();
print(ckpt.checkpointedFrames + "/" + ckpt.logFrames + " frames synced.");

// Maintenance
db.vacuum();   // Defragment file
db.analyze();  // Cập nhật query planner stats

db.close();
```

:::

---

## 2. Unified DbDriver Interface {#unified-dbdriver-interface}

Dùng `connectDb()` để viết code driver-agnostic — ORM và Migration dùng cùng interface:

```typescript
import { connectDb, DbDriver, DbResult } from "vito/packages/db/driver.vit";

// Tự động dispatch đến đúng driver
let db = connectDb("mysql", "root:pw@tcp(localhost:3306)/vito");
// hoặc: connectDb("postgres", "postgres://...")
// hoặc: connectDb("sqlite", "./app.db")

// Chạy query qua interface thống nhất
let res: DbResult = db.query("SELECT * FROM users LIMIT 5;");
if (res.success) {
    let i = 0;
    while (i < res.rowCount) {
        let row = res.getRow(i);
        print(row.get("username"));
        i = i + 1;
    }
}

// Transaction API
db.beginTx();
db.execute("UPDATE wallets SET balance = balance - 100 WHERE user_id = 1;");
db.execute("UPDATE wallets SET balance = balance + 100 WHERE user_id = 2;");
db.commitTx();

db.close();
```

---

## 3. Connection Pool (`packages/db/db_pool.vit`) {#connection-pool}

Connection Pool quản lý vòng đời kết nối, auto-reconnect và idle eviction:

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

let pool = createConnectionPool(
    "postgres",
    "postgres://admin:secret@localhost:5432/vito_db",
    2,   // minConnections
    10   // maxConnections
);

// Mượn kết nối từ Pool (non-blocking)
let connId = pool.acquireConnection();

// Dùng kết nối xử lý request...

// Trả lại Pool
pool.releaseConnection(connId, Date.now());

// Khôi phục kết nối bị đứt
pool.checkHealthAndReconnect();

// Thu hồi kết nối idle quá lâu (> idleTimeoutMs)
pool.reclaimIdleConnections(Date.now());
```

---

## 4. Transaction & Savepoints (`packages/orm/transaction.vit`) {#transactions-savepoints}

ACID transactions 100% với hỗ trợ nested savepoints cho các thao tác phức tạp:

::: code-group

```typescript [Atomic Commit / Rollback]
import { beginTransaction } from "vito/packages/orm/transaction.vit";

let tx = beginTransaction(1001);

tx.execute("UPDATE accounts SET balance = balance - 500 WHERE id = 10;");
tx.execute("UPDATE accounts SET balance = balance + 500 WHERE id = 20;");
tx.execute("INSERT INTO audit_log (event, ts) VALUES ('transfer', NOW());");

// Commit toàn bộ
tx.commit();

// Hoặc nếu xảy ra lỗi — rollback toàn bộ
// tx.rollback();
```

```typescript [Nested Savepoints]
let tx = beginTransaction(1002);
tx.execute("INSERT INTO orders (id, user_id, total) VALUES (501, 12, 150.00);");

// Lưu điểm an toàn TRƯỚC khi áp dụng mã giảm giá
tx.createSavepoint("sp_before_coupon");

tx.execute("UPDATE coupons SET used = true WHERE code = 'PROMO50';");

// Mã coupon lỗi? → chỉ rollback về savepoint, giữ đơn hàng
tx.rollbackToSavepoint("sp_before_coupon");

// Đơn hàng vẫn được lưu an toàn
tx.commit();
```

:::

---

## 5. Schema Migration CLI 🛠️ {#schema-migration-cli}

Migration CLI tương tự Prisma/Goose/Flyway — hỗ trợ **dry-run**, **auto-rollback**, **batch tracking**, và **schema diff tự động**.

### Các lệnh Migration

| Lệnh | Mô tả |
|------|-------|
| `vit migrate:up` | Áp dụng tất cả migrations chưa chạy |
| `vit migrate:up --step 3` | Áp dụng tối đa 3 migration tiếp theo |
| `vit migrate:down --step 1` | Rollback 1 bước (mặc định) |
| `vit migrate:status` | Hiển thị bảng trạng thái applied/pending |
| `vit migrate:dry-run` | Xem SQL sẽ chạy mà **không thực thi** |
| `vit migrate:create <name>` | Tạo file migration mới có timestamp |
| `vit migrate:reset --force` | Rollback tất cả (chỉ dùng trong dev) |
| `vit migrate:fresh --force` | Drop all tables + re-migrate |
| `vit migrate:diff` | So sánh schema models vs live database |

### Đăng ký và chạy Migration bằng code:

::: code-group

```typescript [Đăng ký & Chạy Up]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

// Tạo runner cho MySQL
let runner = createMigrationRunner("mysql", "root:pw@tcp(localhost:3306)/vito");

// Đăng ký migrations (format version: YYYYMMDD_NNNNNN)
runner.register(
    "20260101_000001", "create_users_table",
    // ↑ UP SQL
    "CREATE TABLE IF NOT EXISTS users (" +
    "  id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT," +
    "  username VARCHAR(64) NOT NULL," +
    "  email VARCHAR(255) NOT NULL UNIQUE," +
    "  created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP," +
    "  PRIMARY KEY (id)" +
    ");",
    // ↓ DOWN SQL
    "DROP TABLE IF EXISTS users;"
);

runner.register(
    "20260115_000002", "add_role_column",
    "ALTER TABLE users ADD COLUMN role VARCHAR(32) NOT NULL DEFAULT 'user';",
    "ALTER TABLE users DROP COLUMN role;"
);

// Chạy tất cả pending migrations
let applied = runner.runUp(0);
print("Đã áp dụng: " + applied + " migrations.");
```

```typescript [Dry-Run Mode]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

let runner = createMigrationRunner("postgres", "postgres://admin@localhost/vito");
// ... (đăng ký migrations như trên)

// Xem trước SQL sẽ chạy — KHÔNG thực thi vào database
let report = runner.dryRun("up", 0);

print("Planned: " + report.totalPlanned + " migrations");
print("Destructive ops: " + report.destructiveCount);

// Output mẫu:
// ╔══════════════════════════════════════════════╗
// ║  🔍 DRY RUN MODE — No changes will be made  ║
// ╚══════════════════════════════════════════════╝
//   [DRY-RUN up 1] ✓ 20260101_000001_create_users_table
//     SQL: CREATE TABLE IF NOT EXISTS users (...)
//   [DRY-RUN up 2] ⚠ 20260501_000006_truncate_legacy_sessions
//     SQL: TRUNCATE TABLE legacy_sessions;
//     ⚠️  DESTRUCTIVE OPERATION — data will be lost!
```

```typescript [Rollback & Status]
// Rollback 2 migrations gần nhất (LIFO)
let reverted = runner.runDown(2);
print("Đã rollback: " + reverted + " migrations.");

// Xem trạng thái tổng quan
let status = runner.getStatus();
// Output:
// ┌──────────────┬──────────────────────────┬───────────────┐
// │ Version      │ Name                     │ Status        │
// ├──────────────┼──────────────────────────┼───────────────┤
// │ 20260101_001 │ create_users_table       │ ✅ applied    │
// │ 20260115_002 │ add_role_column          │ ✅ applied    │
// │ 20260201_003 │ create_posts_table       │ ⏳ pending    │
// └──────────────┴──────────────────────────┴───────────────┘
// Batch #1 | Applied: 2 | Pending: 1 | Failed: 0
```

:::

### Auto-Rollback khi Migration Thất Bại

Mỗi migration chạy trong **transaction riêng biệt**. Nếu lỗi xảy ra, tự động rollback ngay migration đó và dừng batch:

```
▶ Running migrations [UP]...
  [UP] Applying: 20260301_000003_create_posts_table
    SQL: CREATE TABLE IF NOT EXISTS posts (...)
  ✅ Applied (batch #1)
  
  [UP] Applying: 20260401_000004_add_foreign_key
    SQL: ALTER TABLE posts ADD CONSTRAINT fk_user_id...
  ❌ FAILED: 20260401_000004_add_foreign_key
     ↩ Auto-rollback applied. Stopping migration batch.
```

---

## 6. Database Seeder {#database-seeder}

Đổ dữ liệu mẫu vào database cho môi trường Dev/Staging:

```typescript
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder("sqlite", "./dev.db");

seeder.addSeed("users",    "1, 'alice', 'alice@vito.dev', datetime('now')");
seeder.addSeed("users",    "2, 'bob',   'bob@vito.dev',   datetime('now')");
seeder.addSeed("products", "1, 'Vito Pro', 99.99");
seeder.addSeed("products", "2, 'Vito Enterprise', 499.99");

// Chạy seeder trong transaction
let seeded = seeder.run();
print("Seeded " + seeded + " bảng thành công!");
```

---

## 7. Schema Introspector & Diff {#schema-introspector-diff}

So sánh schema đang chạy vs model để tự động sinh migration SQL:

```typescript
import { createSchemaIntrospector } from "vito/packages/db/driver.vit";

let intro = createSchemaIntrospector("mysql");

// Lấy SQL để query schema từ INFORMATION_SCHEMA
let colsSql   = intro.getColumnsSql("users");
let tablesSql = intro.getTableListSql();
let idxSql    = intro.getIndexesSql("orders");

// Tự động sinh diff SQL: existing vs desired schema
let diffSql = intro.generateDiffSql(
    "Order",
    "orders",
    "id, total",                      // cột hiện tại
    "status VARCHAR(32) NOT NULL DEFAULT 'pending'"  // cột cần thêm
);
// → ALTER TABLE orders ADD COLUMN IF NOT EXISTS status VARCHAR(32)...
```

---

::: tip 💡 Khuyến Nghị cho Production
- **MySQL/MariaDB 8.0+**: Bật `pipeline_mode = true` để giảm round-trips
- **SQLite**: Giữ WAL mode mặc định; gọi `checkpoint()` định kỳ mỗi 1000 writes
- **Migration**: Luôn chạy `migrate:dry-run` trước khi áp dụng trên production
- **Secrets**: Không hardcode DSN — dùng `std/env` để đọc từ environment variables
:::

::: warning ⚠️ Lưu Ý Quan Trọng
Lệnh `migrate:reset --force` và `migrate:fresh --force` **xóa toàn bộ dữ liệu**. Chỉ dùng trong môi trường dev/test. Không bao giờ dùng trên production!
:::
