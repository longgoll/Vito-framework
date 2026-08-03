# Bộ Công Cụ Vito CLI & Migration Runner 🛠️

**Vito CLI** là công cụ dòng lệnh đa năng giúp tự động hóa toàn bộ quy trình phát triển — từ khởi tạo dự án, hot-reload dev server, cho đến **Schema Migration** mạnh mẽ với **dry-run**, **auto-rollback** và **schema diff** tự động.

---

## ⚡ Lệnh CLI Thường Dùng

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4><code>vit init &lt;app-name&gt;</code></h4>
    <p>Khởi tạo thư mục dự án Vito mới với cấu trúc chuẩn và file mẫu.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4><code>vit dev</code></h4>
    <p>Khởi chạy Dev Server với File Watcher & Hot-Reloading siêu tốc.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4><code>vit build</code></h4>
    <p>Biên dịch ra Native Binary tối ưu hóa cho Production.</p>
  </div>
  <div class="feature-mini-card">
    <div class="icon">🩺</div>
    <h4><code>vit doctor</code></h4>
    <p>Kiểm tra toàn diện môi trường: Compiler, DB drivers, packages, PATH.</p>
  </div>
</div>

---

## 🗄️ Migration CLI — Đầy Đủ Tính Năng

Migration CLI của Vito tương đương Prisma Migrate / Goose / Flyway với các tính năng **production-grade**:

### Bảng Lệnh Migration

| Lệnh | Mô tả |
|------|-------|
| `vit migrate:up` | Áp dụng tất cả pending migrations |
| `vit migrate:up --step N` | Áp dụng tối đa N migrations tiếp theo |
| `vit migrate:down` | Rollback 1 bước (1 batch) |
| `vit migrate:down --step N` | Rollback N migrations theo thứ tự ngược |
| `vit migrate:status` | Bảng trạng thái applied/pending/failed |
| `vit migrate:dry-run` | Preview SQL sẽ chạy — **KHÔNG thực thi** |
| `vit migrate:create <name>` | Scaffold file migration mới (có timestamp) |
| `vit migrate:reset --force` | Rollback tất cả (chỉ dùng trong dev) |
| `vit migrate:fresh --force` | Drop all + re-migrate từ đầu |
| `vit migrate:diff` | So sánh live schema vs model definitions |

### 1. Chạy Migration

::: code-group

```bash [Up — Áp dụng tất cả]
vit migrate:up

# Output:
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

```bash [Up — Step giới hạn]
vit migrate:up --step 3
# Chỉ áp dụng tối đa 3 migrations tiếp theo
```

```bash [Down — Rollback]
vit migrate:down
# Rollback 1 bước (batch gần nhất)

vit migrate:down --step 3
# Rollback 3 migrations theo thứ tự ngược (LIFO)
```

:::

### 2. Dry-Run — Preview Trước Khi Thực Thi

::: tip
Luôn chạy `migrate:dry-run` trước khi deploy production để kiểm tra SQL và phát hiện các thao tác **DESTRUCTIVE** (DROP/TRUNCATE).
:::

```bash
vit migrate:dry-run

# Output:
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

# ┌──────────────────┬─────────────────────────────┬───────────────┐
# │ Version          │ Name                        │ Status        │
# ├──────────────────┼─────────────────────────────┼───────────────┤
# │ 20260101_000001  │ create_users_table          │ ✅ applied    │
# │ 20260115_000002  │ add_role_column             │ ✅ applied    │
# │ 20260201_000003  │ create_posts_table          │ ⏳ pending    │
# │ 20260501_000006  │ truncate_legacy_sessions    │ ⏳ pending    │
# └──────────────────┴─────────────────────────────┴───────────────┘
# Batch #1 | Applied: 2 | Pending: 2 | Failed: 0
```

### 4. Tạo Migration File Mới

```bash
vit migrate:create add_payment_table

# Output:
#   [migrate:create] Scaffolding migration files:
#     ↑ UP:   migrations/20260803_000001_add_payment_table.up.sql
#     ↓ DOWN: migrations/20260803_000001_add_payment_table.down.sql
```

File được tạo tự động với cấu trúc chuẩn:

```sql
-- migrations/20260803_000001_add_payment_table.up.sql
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
DROP TABLE IF EXISTS add_payment_tables;
```

### 5. Auto-Rollback khi Migration Lỗi

Mỗi migration chạy trong **transaction riêng**. Lỗi → tự động rollback, dừng batch:

```
  [UP] Applying: 20260401_000004_add_foreign_key
    SQL: ALTER TABLE posts ADD CONSTRAINT fk_user_id...
  ❌ FAILED: 20260401_000004_add_foreign_key
     ↩ Auto-rollback applied. Stopping migration batch.

  Migrations applied: 3  (batch stopped at failure)
```

---

## 🏗️ Code Generators (`vit generate`)

Tạo scaffold code tự động cho Controller, Service, Model, Middleware:

::: code-group

```bash [Controller]
vit generate controller User
# → Tạo UserController với index() và show() methods
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
# → function seedUser(driverKind, dsn) { ... }
```

```bash [Eden RPC Client]
vit generate client UserApp
# → UserAppClient với getStatus(), getUser() methods
```

:::

---

## 🩺 `vit doctor` — Chẩn Đoán Môi Trường

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

## 📋 Cheat Sheet — Toàn Bộ Lệnh CLI

| Lệnh | Tham số | Mục đích |
|------|---------|----------|
| `vit init` | `<app-name>` | Khởi tạo dự án mới |
| `vit dev` | `--port 3000` | Dev Server với Hot-Reload |
| `vit build` | `--release --target native` | Build Binary Production |
| `vit doctor` | N/A | Kiểm tra toàn bộ môi trường |
| `vit generate` | `controller\|model\|service\|middleware\|migration\|seeder\|client` | Scaffold code |
| `vit migrate:up` | `--step N` | Áp dụng migrations |
| `vit migrate:down` | `--step N` | Rollback migrations |
| `vit migrate:status` | N/A | Xem trạng thái |
| `vit migrate:dry-run` | `--step N` | Preview SQL |
| `vit migrate:create` | `<name>` | Tạo file migration |
| `vit migrate:reset` | `--force` | Reset tất cả (dev only) |
| `vit migrate:fresh` | `--force` | Drop all + re-migrate |
| `vit migrate:diff` | N/A | Schema diff |
| `vit seed` | N/A | Chạy Database Seeder |
