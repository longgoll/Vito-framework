# Database, ORM & Migration CLI 🗄️

**Vito Framework** ships with native zero-allocation database drivers for **PostgreSQL**, **MySQL/MariaDB**, and **SQLite**, combined with a powerful **Migration CLI** on par with Prisma/Goose/Flyway — featuring auto-rollback on failure, dry-run mode, and automatic schema diffing.

---

## 🚀 Driver Ecosystem Overview

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">🐘</div>
    <h4>PostgreSQL Native Driver</h4>
    <p>Wire Protocol v3.0 — zero-allocation binary row decoding, query pipelining, lock-free ring buffer pool.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🐬</div>
    <h4>MySQL / MariaDB Native Driver</h4>
    <p>Wire Protocol v10 — no libmysqlclient dependency, SHA1 auth, COM_QUERY pipeline batch mode.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🪶</div>
    <h4>SQLite Native Driver</h4>
    <p>SQLite3 amalgamation — WAL mode on by default, prepared statements, automatic WAL checkpoint.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🔌</div>
    <h4>Unified DbDriver Interface</h4>
    <p>One interface for all three backends — ORM and Migration are fully driver-agnostic.</p>
  </div>
</div>

---

## 1. Connecting to a Database (`packages/db/driver.vit`)

`DbDriver` is the unified abstraction layer. Both the ORM and Migration engine target this interface:

::: code-group

```typescript [PostgreSQL]
import { connectPostgres } from "vito/packages/db/driver.vit";

let db = connectPostgres("postgres://admin:secret@127.0.0.1:5432/vito_db");

// Query — returns DbResult with named column access
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

// Standard query
let res = client.query("SELECT id, username FROM users LIMIT 10;");
print(res.getCell(0, 0)); // → "1"
print(res.getCell(0, 1)); // → "alice"

// Prepared statement with fluent bind API
let stmt = client.prepare("SELECT * FROM users WHERE id = ? AND active = ?;");
stmt.bindInt(0, 42).bindText(1, "1");
let result = stmt.execute();

// Pipeline batch mode — multiple SQL in a single TCP round-trip (MySQL 8.0+)
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

// Open a database file (WAL mode is automatically enabled)
let db = openSqlite("./vito_app.db");

// Or open an in-memory database for testing
let memDb = openSqliteMemory();

// DDL
db.exec("CREATE TABLE IF NOT EXISTS products (id INTEGER PRIMARY KEY, name TEXT, price REAL);");

// Query with named column access
let res = db.query("SELECT id, name, price FROM products ORDER BY price DESC;");
print(res.get("name"));        // → value of 'name' column in row 0
print(res.getValue(1, 2));     // → row 1, column 2

// Prepared statement (1-based indexing, following SQLite3 convention)
let stmt = db.prepare("INSERT INTO products (name, price) VALUES (?, ?);");
stmt.bindText(1, "Vito Pro").bindInt(2, 99);
stmt.step();
stmt.finalize();

// WAL Checkpoint — sync WAL frames to the main DB file
let ckpt = db.checkpoint();
print(ckpt.checkpointedFrames + "/" + ckpt.logFrames + " frames synced.");

// Maintenance
db.vacuum();   // Defragment the file
db.analyze();  // Update query planner statistics

db.close();
```

:::

---

## 2. Unified DbDriver Interface

Use `connectDb()` to write driver-agnostic code — ORM and Migration share this single interface:

```typescript
import { connectDb, DbDriver, DbResult } from "vito/packages/db/driver.vit";

// Auto-dispatches to the correct driver
let db = connectDb("mysql", "root:pw@tcp(localhost:3306)/vito");
// or: connectDb("postgres", "postgres://...")
// or: connectDb("sqlite", "./app.db")

// Query via the unified interface
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

## 3. Connection Pool (`packages/db/db_pool.vit`)

The Connection Pool manages the full connection lifecycle, auto-reconnect, and idle connection eviction:

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

let pool = createConnectionPool(
    "postgres",
    "postgres://admin:secret@localhost:5432/vito_db",
    2,   // minConnections
    10   // maxConnections
);

// Borrow a connection from the pool (non-blocking)
let connId = pool.acquireConnection();

// ... handle request with the connection

// Return connection to pool
pool.releaseConnection(connId, Date.now());

// Restore broken connections
pool.checkHealthAndReconnect();

// Evict connections idle longer than idleTimeoutMs
pool.reclaimIdleConnections(Date.now());
```

---

## 4. Transactions & Savepoints (`packages/orm/transaction.vit`)

100% ACID transactions with nested savepoint support for complex financial operations:

::: code-group

```typescript [Atomic Commit / Rollback]
import { beginTransaction } from "vito/packages/orm/transaction.vit";

let tx = beginTransaction(1001);

tx.execute("UPDATE accounts SET balance = balance - 500 WHERE id = 10;");
tx.execute("UPDATE accounts SET balance = balance + 500 WHERE id = 20;");
tx.execute("INSERT INTO audit_log (event, ts) VALUES ('transfer', NOW());");

// Commit everything atomically
tx.commit();

// Or roll back everything on error
// tx.rollback();
```

```typescript [Nested Savepoints]
let tx = beginTransaction(1002);
tx.execute("INSERT INTO orders (id, user_id, total) VALUES (501, 12, 150.00);");

// Save a point BEFORE applying the coupon
tx.createSavepoint("sp_before_coupon");

tx.execute("UPDATE coupons SET used = true WHERE code = 'PROMO50';");

// Coupon failed? Roll back only to the savepoint — keep the order
tx.rollbackToSavepoint("sp_before_coupon");

// The order is safely preserved
tx.commit();
```

:::

---

## 5. Schema Migration CLI 🛠️

The Migration CLI is comparable to Prisma Migrate / Goose / Flyway — with **dry-run**, **per-migration auto-rollback**, **batch tracking**, and **automatic schema diffing**.

### Migration Commands

| Command | Description |
|---------|-------------|
| `vit migrate:up` | Apply all pending migrations |
| `vit migrate:up --step N` | Apply at most N pending migrations |
| `vit migrate:down` | Rollback last batch (step = 1) |
| `vit migrate:down --step N` | Rollback last N migrations (LIFO) |
| `vit migrate:status` | Show applied/pending/failed table |
| `vit migrate:dry-run` | Preview SQL without executing |
| `vit migrate:create <name>` | Scaffold a new timestamped migration file |
| `vit migrate:reset --force` | Rollback all migrations (dev only) |
| `vit migrate:fresh --force` | Drop all tables + re-run from scratch |
| `vit migrate:diff` | Compare live schema vs registered models |

### Registering and Running Migrations in Code:

::: code-group

```typescript [Register & Run Up]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

// Create a runner for MySQL
let runner = createMigrationRunner("mysql", "root:pw@tcp(localhost:3306)/vito");

// Register migrations (version format: YYYYMMDD_NNNNNN)
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

// Apply all pending migrations
let applied = runner.runUp(0);
print("Applied: " + applied + " migration(s).");
```

```typescript [Dry-Run Mode]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

let runner = createMigrationRunner("postgres", "postgres://admin@localhost/vito");
// ... (register migrations as above)

// Preview SQL that would run — does NOT write to the database
let report = runner.dryRun("up", 0);

print("Planned: " + report.totalPlanned + " migrations");
print("Destructive ops: " + report.destructiveCount);

// Sample output:
// ╔══════════════════════════════════════════════════════════════╗
// ║  🔍 DRY RUN MODE — No changes will be made to the database  ║
// ╚══════════════════════════════════════════════════════════════╝
//   [DRY-RUN up 1] ✓ 20260101_000001_create_users_table
//     SQL: CREATE TABLE IF NOT EXISTS users (...)
//   [DRY-RUN up 2] ⚠ 20260501_000006_truncate_legacy_sessions
//     SQL: TRUNCATE TABLE legacy_sessions;
//     ⚠️  DESTRUCTIVE OPERATION — data will be lost!
```

```typescript [Rollback & Status]
// Roll back the last 2 migrations (LIFO order)
let reverted = runner.runDown(2);
print("Reverted: " + reverted + " migration(s).");

// View status report
let status = runner.getStatus();
// ┌──────────────────┬─────────────────────────┬───────────────┐
// │ Version          │ Name                    │ Status        │
// ├──────────────────┼─────────────────────────┼───────────────┤
// │ 20260101_000001  │ create_users_table      │ ✅ applied    │
// │ 20260115_000002  │ add_role_column         │ ✅ applied    │
// │ 20260201_000003  │ create_posts_table      │ ⏳ pending    │
// └──────────────────┴─────────────────────────┴───────────────┘
// Batch #1 | Applied: 2 | Pending: 1 | Failed: 0
```

:::

### Auto-Rollback on Migration Failure

Each migration runs inside its **own transaction**. On error, the failed migration is automatically rolled back and the batch stops — ensuring your database is never left in a partial state:

```
▶ Running migrations [UP]...
  [UP] Applying: 20260301_000003_create_posts_table
    SQL: CREATE TABLE IF NOT EXISTS posts (...)
  ✅ Applied (batch #1)

  [UP] Applying: 20260401_000004_add_foreign_key
    SQL: ALTER TABLE posts ADD CONSTRAINT fk_user_id...
  ❌ FAILED: 20260401_000004_add_foreign_key
     ↩ Auto-rollback applied. Stopping migration batch.

  Migrations applied: 1 (batch halted at first failure)
```

---

## 6. Database Seeder

Seed development and staging environments with reproducible fixture data:

```typescript
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder("sqlite", "./dev.db");

seeder.addSeed("users",    "1, 'alice', 'alice@vito.dev', datetime('now')");
seeder.addSeed("users",    "2, 'bob',   'bob@vito.dev',   datetime('now')");
seeder.addSeed("products", "1, 'Vito Pro', 99.99");
seeder.addSeed("products", "2, 'Vito Enterprise', 499.99");

// Runs inside a single transaction
let seeded = seeder.run();
print("Seeded " + seeded + " table(s) successfully!");
```

---

## 7. Schema Introspector & Diff

Compare the live database schema against your model definitions to automatically generate migration SQL:

```typescript
import { createSchemaIntrospector } from "vito/packages/db/driver.vit";

let intro = createSchemaIntrospector("mysql");

// Get the SQL to introspect the live schema from INFORMATION_SCHEMA
let colsSql   = intro.getColumnsSql("users");
let tablesSql = intro.getTableListSql();
let idxSql    = intro.getIndexesSql("orders");

// Auto-generate diff SQL: existing columns vs desired columns
let diffSql = intro.generateDiffSql(
    "Order",
    "orders",
    "id, total",                                        // existing columns
    "status VARCHAR(32) NOT NULL DEFAULT 'pending'"     // desired addition
);
// → ALTER TABLE orders ADD COLUMN IF NOT EXISTS status VARCHAR(32)...
```

---

::: tip 💡 Production Recommendations
- **MySQL/MariaDB 8.0+**: Enable `pipeline_mode = true` to reduce round-trips on bulk operations
- **SQLite**: Keep WAL mode (default); call `checkpoint()` periodically every ~1000 writes
- **Migrations**: Always run `migrate:dry-run` before applying to production
- **Secrets**: Never hardcode DSN strings — read them from environment variables using `std/env`
:::

::: warning ⚠️ Important Warning
`migrate:reset --force` and `migrate:fresh --force` **destroy all data**. Use them only in dev/test environments. Never run on production!
:::
