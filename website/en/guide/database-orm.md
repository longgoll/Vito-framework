# Database Management & Vito ORM Enterprise 🗄️

**Vito Framework** includes an enterprise-grade Database Manager and **Vito ORM** with automatic Connection Pooling, 100% ACID Atomic Transactions, Savepoints, and Schema Auto-Migrations.

---

## 🏊 1. Connection Pool Manager (`packages/db/db_pool.vit`)

Intelligent database connection lifecycle management with automatic reconnection and hardware resource optimization.

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Dynamic Connection Pooling</h4>
    <p>Automatically scales connections based on real load between `minConnections` and `maxConnections`.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔄</div>
    <h4>Fast Auto-Reconnect (< 100ms)</h4>
    <p>Automatically detects network disruption and silently re-establishes connections.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🧹</div>
    <h4>Idle Connection Eviction</h4>
    <p>Reclaims idle connections exceeding the `idleTimeoutMs` threshold to free RAM.</p>
  </div>
</div>

```typescript
import { createConnectionPool } from "vito/packages/db/db_pool.vit";

// Initialize Connection Pool for PostgreSQL
let pool = createConnectionPool(
    "PostgreSQL",
    "postgresql://admin:secret@localhost:5432/vito_db",
    2,  // minConnections
    10  // maxConnections
);

// Borrow a connection from the Pool
let connId = pool.acquireConnection();

// Release the connection back to the Pool
pool.releaseConnection(connId, 1000);
```

---

## 🔄 2. Transaction Management & Savepoints (`packages/orm/transaction.vit`)

Guarantees absolute data integrity (**100% ACID**) for financial and banking operations:

::: code-group

```typescript [1. Atomic Commit / Rollback]
import { beginTransaction } from "vito/packages/orm/transaction.vit";

// Start a Transaction
let tx = beginTransaction(1001);

try {
    tx.execute("UPDATE accounts SET balance = balance - 500 WHERE id = 10");
    tx.execute("UPDATE accounts SET balance = balance + 500 WHERE id = 20");
    
    // Safe commit if no errors occurred
    tx.commit();
} catch (e) {
    // Automatic 100% Rollback on error
    tx.rollback();
}
```

```typescript [2. Nested Savepoints]
let tx = beginTransaction(1002);
tx.execute("INSERT INTO orders (id, user_id) VALUES (501, 12)");

// Create a Savepoint
tx.createSavepoint("sp_order_created");

// Try applying a discount code
tx.execute("UPDATE coupons SET used = true WHERE code = 'PROMO50'");

// If the coupon step fails, rollback only to the Savepoint instead of cancelling the order
tx.rollbackToSavepoint("sp_order_created");

tx.commit(); // Order is still saved safely!
```

:::

---

## 📜 3. Schema Migration & Seeder (`packages/orm/migration.vit`)

Manage table schema change history (Up/Down Migrations) and sample data seeding tools:

::: code-group

```typescript [1. Schema Migration Runner]
import { createMigrationRunner } from "vito/packages/orm/migration.vit";

let migrator = createMigrationRunner();

// Register a Migration
migrator.registerMigration(
    "20260802_001",
    "create_users_table",
    "CREATE TABLE users (id SERIAL PRIMARY KEY, username VARCHAR(50), email VARCHAR(100));",
    "DROP TABLE users;"
);

// Run schema upgrade
migrator.runUp();
```

```typescript [2. Database Seeder]
import { createDatabaseSeeder } from "vito/packages/orm/migration.vit";

let seeder = createDatabaseSeeder();

seeder.addSeed("users", JSON.stringify({ username: "admin", role: "SuperAdmin" }));
seeder.addSeed("products", JSON.stringify({ name: "Vito Enterprise", price: 999 }));

// Execute seeding
seeder.runSeeder();
```

:::
