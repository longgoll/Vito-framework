# CLI & Developer Tooling 🛠

The **`vit` CLI** suite provides modern developer tooling for fast prototyping and deployment.

---

## 🛠 Main CLI Commands

### 1. `vit create <app-name>`
Scaffold a new Vito project with ready-to-use directory layout and sample code:

```bash
vit create my-awesome-api
cd my-awesome-api
```

---

### 2. `vit dev` (Hot Module Reloading)
Start the development server with file watching and instant Hot Module Reloading (HMR):

```bash
vit dev
```

---

### 3. `vit build --binary`
Bundle your Vito framework and application into a single **Standalone Executable Binary** (< 5MB), ready for production deployment without external runtime dependencies:

```bash
vit build --binary --output ./dist/server
```

---

## 🧪 In-Memory Testing Injector (`app.inject`)

Test API endpoints in-memory without opening a TCP socket:

```javascript
// Test GET /users/:id directly in-memory
let res = app.inject("GET", "/users/42");

// Validate Status Code & Body
if (res.statusCode == 200) {
    print("Test Passed: " + res.body);
}
```
