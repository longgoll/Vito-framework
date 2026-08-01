# Database Connection Pool & Vito ORM 🗄️

**Vito Framework** provides an Enterprise-ready Database Connection Pooling and ORM suite out of the box, featuring dynamic pooling, safe transaction management, and automated schema migrations.

---

## 🏊 Connection Pool Manager (`packages/db/db_pool.vit`)

The Connection Pool Manager automatically manages database connection lifecycles, optimizes query performance, and auto-recovers broken connections.

### Key Features:
- **Dynamic Connection Pooling**: Dynamically scales connections to PostgreSQL, SQLite, or MySQL according to workload, constrained by `minConnections` and `maxConnections`.
- **Fast Auto-Reconnect (< 100ms)**: Automatically detects broken network connections and silently restores them.
- **Idle Connection Cleanup**: Automatically releases idle connections exceeding the `idleTimeoutMs` threshold.

### Example Usage:

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

// Create Connection Pool for PostgreSQL
let pool = createConnectionPool(
    "PostgreSQL",
    "postgresql://vito_admin:secret@localhost:5432/vito_prod",
    2, // minConnections
    10 // maxConnections
);

// Acquire connection from Pool
let connId = pool.acquireConnection();

// Release connection back to Pool when done
pool.releaseConnection(connId, 1000);

// Auto-check connection health and restore broken links (< 100ms)
let restoredCount = pool.checkHealthAndReconnect();

// Reclaim idle connections
pool.reclaimIdleConnections(5000);
```

---

## 🔄 Transaction Management Engine (`packages/orm/transaction.vit`)

Supports atomic database transactions ensuring **100% ACID integrity** and Savepoints for nested transaction blocks.

### Example Usage & ACID Rollback:

```typescript
import { beginTransaction } from "vito/packages/orm/transaction.vit";

// 1. Successful Transaction (COMMIT)
let tx = beginTransaction(1001);
tx.execute("INSERT INTO users (name, email) VALUES ('Alice', 'alice@vito.dev')");
tx.execute("UPDATE account_balances SET amount = amount - 500 WHERE user_id = 10");
tx.commit(); // Safely commit operations

// 2. Failed Transaction (Automatic ROLLBACK)
let tx2 = beginTransaction(1002);
tx2.execute("INSERT INTO orders (id, total) VALUES (501, 1200)");
// When an error occurs:
tx2.rollback(); // Discards all staged queries cleanly (100% ACID integrity)
```

### Nested Transactions & Savepoints:

```typescript
let tx3 = beginTransaction(1003);
tx3.execute("INSERT INTO audit_logs (action) VALUES ('CHECKOUT')");

// Create Savepoint
tx3.createSavepoint("sp_audit_done");

// Perform sub-queries
tx3.execute("UPDATE coupons SET used = true WHERE code = 'DISCOUNT50'");

// Roll back only to Savepoint on error without aborting entire transaction
tx3.rollbackToSavepoint("sp_audit_done");

tx3.commit();
```

---

## 📜 Migration Runner & Database Seeder (`packages/orm/migration.vit`)

Manages database schema version history (Up/Down Migrations) and populates mock data for Dev & Staging environments.

### 1. Schema Migration Engine

```typescript
import { createMigrationRunner, createSchemaDiffGenerator } from "vito/packages/orm/migration.vit";

let migrator = createMigrationRunner();

// Register migration
migrator.registerMigration(
    "20260801_001",
    "create_users_table",
    "CREATE TABLE users (id SERIAL PRIMARY KEY, username VARCHAR(50));",
    "DROP TABLE users;"
);

// Execute Up Migration (Tracks version history in `schema_migrations`)
migrator.runUp();

// Rollback migration
migrator.runDown();
```

### 2. Schema Diff Generator

```typescript
let diffGen = createSchemaDiffGenerator();
let diffSql = diffGen.generateDiffSql("Product", "products", "", "id INT, title VARCHAR(100), price NUMERIC");
// Returns SQL: CREATE TABLE products (id INT, title VARCHAR(100), price NUMERIC);
```

### 3. Database Seeder

```typescript
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder();
seeder.addSeed("users", '{"username":"vito_admin","email":"admin@vito.dev"}');
seeder.addSeed("products", '{"title":"Vito Enterprise Server","price":999}');

// Populate database
seeder.runSeeder();
```
