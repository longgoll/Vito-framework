# Vito API Reference 📚

Detailed documentation for the core functions and objects of the **Vito Web Framework Core Engine**.

---

## 🏗 `VitoApp` Class

### `createApp()`
Initialize a new Vito application instance.

```javascript
let app = createApp();
```

### `app.get(path, handler)` / `app.post(path, handler)` / `app.put()` / `app.delete()`
Register a route handler for the corresponding HTTP method.

### `app.group(prefix, callback)`
Create a group of paths sharing a common URL prefix.

### `app.use(middleware)`
Add middleware to the request processing pipeline.

### `app.setStrictSlash(strict: boolean)`
Enable/disable Strict Trailing Slash Normalization (`/users/` vs `/users`). Default: `false` (Lax mode).

### `app.setNotFoundHandler(handler)`
Set a custom handler when no route is found (404 Not Found).

### `app.listen(port)`
Open the socket and begin listening for HTTP connections on the specified port.

---

## 📊 Observability, Metrics & DX (`Phase 14`)

| Function / Structure | Package | Description |
| :--- | :--- | :--- |
| `createPrometheusRegistry()` | `packages/metrics/metrics.vit` | Initialize a Prometheus Metrics Registry collecting HTTP & OS metrics. |
| `registry.recordRequest(method, path, status, duration)` | `packages/metrics/metrics.vit` | Record HTTP Request metrics for latency percentiles (p50, p90, p99). |
| `registry.renderPrometheusMetrics()` | `packages/metrics/metrics.vit` | Export data in standard Prometheus exposition text format. |
| `extractTraceParent(header)` / `injectTraceParent` | `packages/metrics/metrics.vit` | Extract and propagate OpenTelemetry (OTel) W3C `traceparent` headers. |
| `createMockHttpEngine()` | `packages/testing/testing.vit` | Initialize an In-Memory Fast HTTP Injector for Test Suites. |
| `mockEngine.inject(app, method, path, headers, body)` | `packages/testing/testing.vit` | Simulate HTTP Requests directly in RAM without TCP socket. |
| `createTestDbFixture(name)` | `packages/testing/testing.vit` | Initialize a sample data loader for Test Suites. |
| `fixture.beginIsolationTransaction()` / `rollbackIsolationTransaction` | `packages/testing/testing.vit` | Automatically ROLLBACK the DB after each test case to maintain clean state. |
| `createVitCodeGenerator()` | `packages/cli/cli.vit` | Generate template source code for Controller, Service, Middleware, Model, Migration. |
| `createVitDoctor()` | `packages/cli/cli.vit` | Execute development environment diagnostics and system resource checks. |

---

## 🔒 Auth & Security Suite Packages (`packages/auth/`)

| Function / Struct | Package | Description |
| :--- | :--- | :--- |
| `verifyJwtToken(token, secret, algo)` | `packages/auth/jwt.vit` | Verify JWT Token signature (`HS256`, `RS256`, `ES256`) in < 0.1ms. |
| `parseJwtToken(token)` | `packages/auth/jwt.vit` | Extract Header and Payload Claims from a raw JWT string. |
| `validateJwtClaims(jwt, now, exp, nbf)` | `packages/auth/jwt.vit` | Validate expiry (`exp`) and not-before (`nbf`) timestamps. |
| `jwtGuardMiddleware` | `packages/auth/jwt.vit` | Middleware that auto-validates Bearer tokens and rejects HTTP 401 on failure. |
| `createGoogleOAuthProvider(...)` | `packages/auth/oauth.vit` | Initialize OAuth2 & OIDC Config for Google Provider with PKCE. |
| `createGitHubOAuthProvider(...)` | `packages/auth/oauth.vit` | Initialize OAuth2 Config for GitHub Provider. |
| `createMicrosoftOAuthProvider(...)` | `packages/auth/oauth.vit` | Initialize OAuth2 Config for Microsoft Azure AD / Entra ID. |
| `config.generateAuthorizationUrl(state, challenge)` | `packages/auth/oauth.vit` | Generate OAuth2 redirect login URL with PKCE challenge. |
| `createApiKeyStore()` | `packages/auth/apikey.vit` | Initialize API Key storage and per-client-key Rate Limit management. |
| `hashApiKey(rawKey)` | `packages/auth/apikey.vit` | One-way hash an API Key securely for database storage. |
| `hashPasswordArgon2id(pass, salt, params)` | `packages/auth/crypto.vit` | Hash passwords with Argon2id following OWASP recommendations. |
| `hashPasswordBcrypt(pass, salt, cost)` | `packages/auth/crypto.vit` | Hash passwords with Bcrypt using a custom Cost Factor. |
| `encryptAES256GCM(plain, key, iv, ad)` | `packages/auth/crypto.vit` | Two-way encrypt data using AES-256-GCM AEAD (`iv:ciphertext:tag`). |
| `decryptAES256GCM(payload, key, ad)` | `packages/auth/crypto.vit` | Decrypt data and verify GCM Tag integrity. |
| `timingSafeEqual(strA, strB)` | `packages/auth/crypto.vit` | Constant-time string comparison to prevent Side-channel Timing Attacks. |
| `createRoleGuard(role, currentRoles)` | `packages/auth/rbac.vit` | Initialize RBAC Guard to check user roles (rejects HTTP 403). |
| `createPermissionGuard(perm, currentPerms)` | `packages/auth/rbac.vit` | Initialize RBAC Guard to check route operation permissions. |
| `createGateEngine()` | `packages/auth/rbac.vit` | Initialize ABAC Engine for context-based resource authorization. |

---

## 🗄️ Database & ORM Packages (`packages/db` & `packages/orm`)

| Function / Struct | Package | Description |
| :--- | :--- | :--- |
| `createConnectionPool(type, connStr, min, max)` | `packages/db/db_pool.vit` | Initialize a Connection Pool with automatic scaling and silent reconnection. |
| `pool.acquireConnection()` | `packages/db/db_pool.vit` | Borrow a valid database connection from the Pool. |
| `pool.checkHealthAndReconnect()` | `packages/db/db_pool.vit` | Check connection health & auto-reconnect silently (< 100ms). |
| `beginTransaction(id)` | `packages/orm/transaction.vit` | Initialize an Atomic Database Transaction supporting ACID. |
| `tx.createSavepoint(name)` | `packages/orm/transaction.vit` | Create a Savepoint for nested transactions. |
| `tx.rollbackToSavepoint(name)` | `packages/orm/transaction.vit` | Roll back transaction state to the specified Savepoint. |
| `createMigrationRunner()` | `packages/orm/migration.vit` | Create an object for managing and running Migrations (Up/Down) stored in `schema_migrations`. |
| `createSchemaDiffGenerator()` | `packages/orm/migration.vit` | Auto-generate SQL Migration statements from Vito ORM Model definitions. |
| `createDatabaseSeeder()` | `packages/orm/migration.vit` | Create a sample data seeder for Dev & Staging environments. |

---

## ⚡ Performance Packages (`packages/pool` & `packages/router`)

| Function / Helper | Package | Description |
| :--- | :--- | :--- |
| `createMemoryArena()` | `packages/pool/pool.vit` | Create a reusable Request Memory Arena slot manager. |
| `createTcpBufferPool()` | `packages/pool/pool.vit` | Create a Buffer Recycling Pool for TCP Read/Write buffers. |
| `zeroAllocHeaderLookup(raw, target)` | `packages/pool/pool.vit` | Extract Header values directly from raw HTTP stream without memory allocation. |
| `createRadixRouter(strict)` | `packages/router/router.vit` | Initialize a Static Route Fast Path Router with O(1) memory slot lookup. |
| `normalizePath(path, strict)` | `packages/router/router.vit` | Normalize URL paths in Lax / Strict slash mode. |

---

## 📥 `Request` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `req.param(key)` | Get value of dynamic path parameter `:key` or `*wildcard` | `req.param("id")` |
| `req.query(key)` | Get value of search query string `?key=val` | `req.query("page")` |
| `req.header(key)` | Get HTTP request header value | `req.header("Authorization")` |
| `req.method` | Property returning the HTTP method (GET, POST...) | `req.method` |
| `req.path` | Property returning the URL path | `req.path` |

---

## 📤 `Response` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `res.setStatus(code)` | Set HTTP Status Code (200, 404, 500...) | `res.setStatus(200)` |
| `res.setHeader(name, val)`| Set HTTP Response Header | `res.setHeader("X-Powered-By", "Vito")` |
| `res.setCookie(key, val, opts)`| Write Cookie with HMAC signature & HttpOnly/SameSite flags | `res.setCookie("sid", "abc", opts)` |
| `res.json(jsonStr)` | Return JSON string with `Content-Type: application/json` header | `res.json("{\"ok\":true}")` |
| `res.html(htmlStr)` | Return HTML content with `Content-Type: text/html` | `res.html("<h1>Hi</h1>")` |
| `res.send(textStr)` | Return plain text string | `res.send("Hello World")` |
| `res.sendChunk(chunk)` | Return data as Stream / Chunked response | `res.sendChunk(data)` |

---

## ⚡ WebSockets, Event Bus & Job Queue (`Phase 13`)

| Function / Structure | Package | Description |
| :--- | :--- | :--- |
| `createWebSocketServer(port, maxConn)` | `packages/websocket/websocket.vit` | Initialize a High-Concurrency WebSocket Server engine. |
| `wsServer.broadcastToRoom(room, msg)` | `packages/websocket/websocket.vit` | Broadcast a message to all connections in a Room. |
| `wsServer.unicast(client, msg)` | `packages/websocket/websocket.vit` | Send a message to a specific WebSocket Client. |
| `wsServer.checkHeartbeats(client, time)` | `packages/websocket/websocket.vit` | Check Ping/Pong and evict stale connections. |
| `compressFrame(payload)` / `decompressFrame` | `packages/websocket/websocket.vit` | Compress and decompress WebSocket frame data with per-message deflate. |
| `createEventBus()` | `packages/events/event_bus.vit` | Initialize an Async Internal Event Bus dispatcher. |
| `eventBus.emitAsync(event, payload)` | `packages/events/event_bus.vit` | Emit an asynchronous event without blocking the main thread. |
| `createRedisPubSubAdapter(host, port)` | `packages/events/redis_pubsub.vit` | Create a Redis Pub/Sub adapter for inter-node communication in a Server cluster. |
| `createJobQueue(driver)` | `packages/queue/queue.vit` | Initialize Background Job Queue (Driver: `"memory"` or `"redis"`). |
| `jobQueue.scheduleJob(job, delayMs)` | `packages/queue/queue.vit` | Push a job to the delayed queue with a configured delay. |
| `jobQueue.processNextJob(shouldFail, reason)` | `packages/queue/queue.vit` | Process the next job, supporting Exponential Backoff Retry & DLQ. |
