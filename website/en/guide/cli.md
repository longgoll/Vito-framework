# Vito CLI Tooling & Migration Runner 🛠️

**Vito CLI** is the all-in-one command-line toolkit for every step of the development lifecycle — from scaffolding a new project and running a hot-reload dev server, to a **full-featured Schema Migration CLI** with **dry-run**, **per-migration auto-rollback**, and **schema diffing**.

---

## ⚡ Core CLI Commands

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4><code>vit init &lt;app-name&gt;</code></h4>
    <p>Scaffold a new Vito project with the standard directory structure and starter files.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4><code>vit dev</code></h4>
    <p>Start the development server with file watching and hot-reloading.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4><code>vit build</code></h4>
    <p>Compile to an optimized native binary ready for production deployment.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🩺</div>
    <h4><code>vit doctor</code></h4>
    <p>Run a full environment diagnostic: compiler, DB drivers, packages, and PATH.</p>
  </div>
</div>

---

## 🗄️ Migration CLI — Full Feature Set

Vito's Migration CLI is on par with Prisma Migrate / Goose / Flyway and includes **production-grade** features out of the box.

### Command Reference

| Command | Description |
|---------|-------------|
| `vit migrate:up` | Apply all pending migrations |
| `vit migrate:up --step N` | Apply at most N pending migrations |
| `vit migrate:down` | Rollback last batch (default: 1 step) |
| `vit migrate:down --step N` | Rollback last N migrations (LIFO) |
| `vit migrate:status` | Show applied / pending / failed table |
| `vit migrate:dry-run` | Preview SQL without executing anything |
| `vit migrate:create <name>` | Scaffold new timestamped migration files |
| `vit migrate:reset --force` | Rollback all migrations (dev only) |
| `vit migrate:fresh --force` | Drop all tables + re-migrate from scratch |
| `vit migrate:diff` | Compare live schema vs model definitions |

### 1. Running Migrations

::: code-group

```bash [Up — Apply All]
vit migrate:up

# ╔══════════════════════════════════════════════════════════════╗
# ║  🛠  vit migrate:up
# ╚══════════════════════════════════════════════════════════════╝
# ▶ Running migrations [UP]... Driver: mysql
#
#   [UP] Applying: 20260101_000001_create_users_table
#     SQL: CREATE TABLE IF NOT EXISTS users (...)
#   ✅ Applied: 20260101_000001_create_users_table (batch #1)
#
#   [UP] Applying: 20260115_000002_add_role_column
#     SQL: ALTER TABLE users ADD COLUMN role VARCHAR(32)...
#   ✅ Applied: 20260115_000002_add_role_column (batch #1)
#
#   Migrations applied: 2
#   Total applied so far: 2/3
```

```bash [Up — Step Limit]
vit migrate:up --step 3
# Apply at most 3 pending migrations
```

```bash [Down — Rollback]
vit migrate:down
# Rollback 1 step (last batch)

vit migrate:down --step 3
# Rollback the last 3 migrations in reverse order (LIFO)
```

:::

### 2. Dry-Run — Preview Before Executing

::: tip Best Practice
Always run `migrate:dry-run` before deploying to production. It will inspect the pending SQL and flag any **DESTRUCTIVE** operations (DROP / TRUNCATE) with warnings.
:::

```bash
vit migrate:dry-run

# ╔══════════════════════════════════════════════════════════════╗
# ║  🔍 DRY RUN MODE — No changes will be made to the database  ║
# ╚══════════════════════════════════════════════════════════════╝
#
#   [DRY-RUN up 1] ✓ 20260101_000001_create_users_table
#     SQL: CREATE TABLE IF NOT EXISTS users (id BIGINT...
#
#   [DRY-RUN up 2] ✓ 20260201_000003_create_posts_table
#     SQL: CREATE TABLE IF NOT EXISTS posts (id BIGINT...
#
#   [DRY-RUN up 3] ⚠ 20260501_000006_truncate_legacy_sessions
#     SQL: TRUNCATE TABLE legacy_sessions;
#     ⚠️  DESTRUCTIVE OPERATION — data will be lost!
#
#   Summary: 3 migration(s) planned, 1 destructive.
#   ✅ DRY RUN PASSED — run without --dry-run to apply.
#   No changes were made to the database.
```

### 3. Migration Status

```bash
vit migrate:status

# ┌──────────────────┬──────────────────────────────┬───────────────┐
# │ Version          │ Name                         │ Status        │
# ├──────────────────┼──────────────────────────────┼───────────────┤
# │ 20260101_000001  │ create_users_table           │ ✅ applied    │
# │ 20260115_000002  │ add_role_column              │ ✅ applied    │
# │ 20260201_000003  │ create_posts_table           │ ⏳ pending    │
# │ 20260501_000006  │ truncate_legacy_sessions     │ ⏳ pending    │
# └──────────────────┴──────────────────────────────┴───────────────┘
# Batch #1 | Applied: 2 | Pending: 2 | Failed: 0
```

### 4. Creating a New Migration

```bash
vit migrate:create add_payment_table

#   [migrate:create] Scaffolding migration files:
#     ↑ UP:   migrations/20260803_000001_add_payment_table.up.sql
#     ↓ DOWN: migrations/20260803_000001_add_payment_table.down.sql
```

The generated files follow the standard naming convention `YYYYMMDD_NNNNNN_name`:

```sql
-- migrations/20260803_000001_add_payment_table.up.sql
-- Migration: add_payment_table
-- Version:   20260803_000001
-- Created:   2026-08-03

CREATE TABLE IF NOT EXISTS add_payment_tables (
    id         BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);
CREATE INDEX idx_add_payment_tables_created_at ON add_payment_tables (created_at);
```

```sql
-- migrations/20260803_000001_add_payment_table.down.sql
-- Rollback: add_payment_table

DROP TABLE IF EXISTS add_payment_tables;
```

### 5. Auto-Rollback on Failure

Each migration runs in its **own transaction**. If it fails, it is automatically rolled back and the batch stops — your database is never left in a partial or inconsistent state:

```
  [UP] Applying: 20260401_000004_add_foreign_key
    SQL: ALTER TABLE posts ADD CONSTRAINT fk_user_id...
  ❌ FAILED: 20260401_000004_add_foreign_key
     ↩ Auto-rollback applied. Stopping migration batch.

  Migrations applied: 3  (batch halted at first failure)
```

---

## 🏗️ Code Generators (`vit generate`)

Quickly scaffold boilerplate code for controllers, services, models, middleware, and more:

::: code-group

```bash [Controller]
vit generate controller User
# → Generates UserController with index() and show() methods
```

```bash [Migration + Model]
vit generate migration Product
# → migrations/20260803_000001_Product.up.sql
# → migrations/20260803_000001_Product.down.sql

vit generate model Product
# → struct Product { id: string, createdAt: string }
```

```bash [Seeder]
vit generate seeder User
# → seedUser(driverKind, dsn) function with addSeed + run
```

```bash [Eden RPC Client]
vit generate client UserApp
# → UserAppClient with getStatus() and getUser() methods
```

:::

---

## 🩺 `vit doctor` — Environment Health Check

Runs a comprehensive diagnostic across the entire Vito toolchain:

```bash
vit doctor

# =========================================================
#  🩺 `vit doctor` Interactive Environment Diagnostics
# =========================================================
#   [✓] VIT Compiler Engine: v1.0.4-native (LLVM 17.0.6 Backend)
#   [✓] Operating System: Windows_NT x64 (Build 26100)
#   [✓] Memory Footprint: 28.4 MB (Arena Allocator Active)
#   [✓] Project Config: vit.json valid (vito@1.0.0)
#   [✓] DB Driver: PostgreSQL (native wire protocol v3.0, zero-alloc)
#   [✓] DB Driver: MySQL/MariaDB (native wire protocol v10, pipeline)
#   [✓] DB Driver: SQLite (amalgamation, WAL mode, busy_timeout=5s)
#   [✓] Unified DbDriver Interface: packages/db/driver.vit
#   [✓] Migration CLI: migrate:up/down/status/dry-run/create/reset/diff/fresh
#   [✓] schema_migrations tracking table support
#   [✓] All 30 Ecosystem Packages & Core Plugins verified clean.
#   [DIAGNOSTIC STATUS] Vito Environment is 100% HEALTHY!
```

---

## 📋 Full CLI Cheat Sheet

| Command | Options | Purpose |
|---------|---------|---------|
| `vit init` | `<app-name>` | Scaffold a new project |
| `vit dev` | `--port 3000` | Dev server with hot-reload |
| `vit build` | `--release --target native` | Compile production binary |
| `vit doctor` | N/A | Full environment diagnostic |
| `vit generate` | `controller\|model\|service\|middleware\|migration\|seeder\|client` | Code scaffolding |
| `vit migrate:up` | `--step N` | Apply migrations |
| `vit migrate:down` | `--step N` | Rollback migrations |
| `vit migrate:status` | N/A | View status table |
| `vit migrate:dry-run` | `--step N` | Preview SQL (no execution) |
| `vit migrate:create` | `<name>` | Scaffold migration files |
| `vit migrate:reset` | `--force` | Rollback all (dev only) |
| `vit migrate:fresh` | `--force` | Drop all + re-migrate |
| `vit migrate:diff` | N/A | Schema diff |
| `vit seed` | N/A | Run database seeder |
