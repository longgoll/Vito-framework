# Vito CLI & Developer Tooling 🛠️

The **Vito CLI** is a versatile command-line toolkit for automating the development workflow — from project scaffolding, live-reload watching, and database migration to compiling and packaging your application as a Native Binary.

---

## ⚡ Common CLI Commands

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4><code>vit init &lt;app-name&gt;</code></h4>
    <p>Scaffold a new Vito project directory with standard structure and template files.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔥</div>
    <h4><code>vit dev</code></h4>
    <p>Start the Dev Server with ultra-fast File Watcher & Hot-Reloading.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🚀</div>
    <h4><code>vit build</code></h4>
    <p>Compile source code into a fully optimized Native Binary for Production.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🔍</div>
    <h4><code>vit setup</code></h4>
    <p>Verify PATH environment variables, Clang/LLVM compiler, and bundled tools.</p>
  </div>
</div>

---

## 🗄️ Database Management with Vito CLI Migration Runner

::: code-group

```bash [1. Create New Migration]
vit migrate create add_users_table
# -> Created file: migrations/20260802_001_add_users_table.vit
```

```bash [2. Apply Schema (Up)]
vit migrate up
# -> [✓] Executed migration: 20260802_001_add_users_table (4ms)
```

```bash [3. Revert Schema (Down)]
vit migrate rollback
# -> [✓] Reverted migration: 20260802_001_add_users_table (2ms)
```

:::

---

## 🛠 Full CLI Command Reference (Cheat Sheet)

| CLI Command | Parameters / Options | Purpose |
| :--- | :--- | :--- |
| `vit setup` | N/A | Verify Toolchain & PATH |
| `vit init` | `<app-name>` | Scaffold a new project |
| `vit dev` | `--port 3000` | Start Dev Server with Hot-Reload |
| `vit build` | `--release --target native` | Compile Production Binary |
| `vit migrate up` | N/A | Run all pending migrations |
| `vit migrate rollback` | N/A | Revert the last migration step |
| `vit seed` | N/A | Run database seeder with sample data |
