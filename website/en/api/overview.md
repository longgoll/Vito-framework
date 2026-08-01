# Vito API Reference 📚

Complete API reference for the core components of the **Vito Web Framework**.

---

## 🏗 `VitoApp` Class

### `createApp()`
Initializes a new instance of the Vito application.

```javascript
let app = createApp();
```

### `app.get(path, handler)` / `app.post(path, handler)` / `app.put()` / `app.delete()`
Registers a route handler for the specified HTTP verb.

### `app.group(prefix, callback)`
Groups routes under a common path prefix.

### `app.use(middleware)`
Applies a middleware to the request processing pipeline.

### `app.setStrictSlash(strict: boolean)`
Enables/disables Strict Trailing Slash Normalization (`/users/` vs `/users`). Default: `false` (Lax mode).

### `app.setNotFoundHandler(handler)`
Sets a custom handler for 404 Not Found responses.

### `app.listen(port)`
Starts listening for HTTP connections on the given port.

---

## 📊 Observability, Metrics & DX (`Phase 14`)

| Function / Struct | Package | Description |
| :--- | :--- | :--- |
| `createPrometheusRegistry()` | `packages/metrics/metrics.vit` | Initializes Prometheus Metrics Registry collecting HTTP & OS runtime stats. |
| `registry.recordRequest(method, path, status, duration)` | `packages/metrics/metrics.vit` | Records HTTP request metrics across latency quantiles (p50, p90, p99). |
| `registry.renderPrometheusMetrics()` | `packages/metrics/metrics.vit` | Exports standard Prometheus exposition format text data. |
| `extractTraceParent(header)` / `injectTraceParent` | `packages/metrics/metrics.vit` | Extracts and injects OpenTelemetry (OTel) W3C `traceparent` headers. |
| `createMockHttpEngine()` | `packages/testing/testing.vit` | Initializes ultra-fast In-Memory HTTP Injector for Test Suites. |
| `mockEngine.inject(app, method, path, headers, body)` | `packages/testing/testing.vit` | Simulates HTTP requests directly in memory without TCP sockets. |
| `createTestDbFixture(name)` | `packages/testing/testing.vit` | Instantiates test fixture helper for seeding data. |
| `fixture.beginIsolationTransaction()` / `rollbackIsolationTransaction` | `packages/testing/testing.vit` | Automatically ROLLBACKS DB transactions post-test to maintain clean state. |
| `createVitCodeGenerator()` | `packages/cli/cli.vit` | Generates boilerplate for Controllers, Services, Middlewares, Models, and Migrations. |
| `createVitDoctor()` | `packages/cli/cli.vit` | Performs interactive system environment and resource diagnostics. |

---

## 🔒 Auth & Security Suite Packages (`packages/auth/`)

| Function / Struct | Package | Description |
| :--- | :--- | :--- |
| `verifyJwtToken(token, secret, algo)` | `packages/auth/jwt.vit` | Verifies JWT Token signatures (`HS256`, `RS256`, `ES256`) in $< 0.1\text{ms}$. |
| `parseJwtToken(token)` | `packages/auth/jwt.vit` | Extracts Header and Payload Claims from raw JWT strings. |
| `validateJwtClaims(jwt, now, exp, nbf)` | `packages/auth/jwt.vit` | Validates token expiration (`exp`) and active time (`nbf`). |
| `jwtGuardMiddleware` | `packages/auth/jwt.vit` | Automatically authenticates Bearer tokens and rejects unauthorized requests with HTTP 401. |
| `createGoogleOAuthProvider(...)` | `packages/auth/oauth.vit` | Initializes Google OAuth2 & OIDC Client Helper with PKCE support. |
| `createGitHubOAuthProvider(...)` | `packages/auth/oauth.vit` | Initializes GitHub OAuth2 Provider Config. |
| `createMicrosoftOAuthProvider(...)` | `packages/auth/oauth.vit` | Initializes Microsoft Azure AD / Entra ID OAuth2 Provider Config. |
| `config.generateAuthorizationUrl(state, challenge)` | `packages/auth/oauth.vit` | Generates OAuth2 redirect URL with PKCE challenge parameter. |
| `createApiKeyStore()` | `packages/auth/apikey.vit` | Instantiates API Key Store with per-client-key rate limiting quota management. |
| `hashApiKey(rawKey)` | `packages/auth/apikey.vit` | Single-way key hashing for secure database storage. |
| `hashPasswordArgon2id(pass, salt, params)` | `packages/auth/crypto.vit` | Hashes passwords with Argon2id matching OWASP recommendations. |
| `hashPasswordBcrypt(pass, salt, cost)` | `packages/auth/crypto.vit` | Hashes passwords with Bcrypt using configurable cost factors. |
| `encryptAES256GCM(plain, key, iv, ad)` | `packages/auth/crypto.vit` | Two-way AEAD encryption formatted as `iv:ciphertext:tag`. |
| `decryptAES256GCM(payload, key, ad)` | `packages/auth/crypto.vit` | Decrypts payloads and validates AES-256-GCM authentication tag. |
| `timingSafeEqual(strA, strB)` | `packages/auth/crypto.vit` | Constant-time string comparison preventing side-channel timing attacks. |
| `createRoleGuard(role, currentRoles)` | `packages/auth/rbac.vit` | Initializes RBAC Role Guard (rejecting unauthorized with HTTP 403). |
| `createPermissionGuard(perm, currentPerms)` | `packages/auth/rbac.vit` | Initializes RBAC Permission Guard for route action authorization. |
| `createGateEngine()` | `packages/auth/rbac.vit` | Instantiates ABAC Policy Engine for resource context authorization. |

---

## 🗄️ Database & ORM Packages (`packages/db` & `packages/orm`)

| Function / Struct | Package | Description |
| :--- | :--- | :--- |
| `createConnectionPool(type, connStr, min, max)` | `packages/db/db_pool.vit` | Initializes auto-scaling Connection Pool with transparent health checks. |
| `pool.acquireConnection()` | `packages/db/db_pool.vit` | Borrows a valid database connection from Connection Pool. |
| `pool.checkHealthAndReconnect()` | `packages/db/db_pool.vit` | Performs connection health check & transparent reconnect ($< 100\text{ms}$). |
| `beginTransaction(id)` | `packages/orm/transaction.vit` | Starts atomic database transaction enforcing ACID compliance. |
| `tx.createSavepoint(name)` | `packages/orm/transaction.vit` | Creates savepoints for nested transactions. |
| `tx.rollbackToSavepoint(name)` | `packages/orm/transaction.vit` | Rollbacks transaction state to specified savepoint. |
| `createMigrationRunner()` | `packages/orm/migration.vit` | Instantiates Migration Runner executing Up/Down scripts tracked in `schema_migrations`. |
| `createSchemaDiffGenerator()` | `packages/orm/migration.vit` | Generates SQL migration scripts automatically from Vito ORM Models. |
| `createDatabaseSeeder()` | `packages/orm/migration.vit` | Creates Database Seeder helper for Dev & Staging environments. |

---

## ⚡ Performance Packages (`packages/pool` & `packages/router`)

| Function / Helper | Package | Description |
| :--- | :--- | :--- |
| `createMemoryArena()` | `packages/pool/pool.vit` | Creates reusable Request Memory Arena slots manager. |
| `createTcpBufferPool()` | `packages/pool/pool.vit` | Creates Buffer Recycling Pool for TCP Read/Write buffers. |
| `zeroAllocHeaderLookup(raw, target)` | `packages/pool/pool.vit` | Extracts header value directly from raw HTTP stream without memory allocation. |
| `createRadixRouter(strict)` | `packages/router/router.vit` | Initializes Radix Trie Router with $O(1)$ Static Route Fast Path. |
| `normalizePath(path, strict)` | `packages/router/router.vit` | Normalizes URL paths according to Lax / Strict slash mode. |

---

## 📥 `Request` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `req.param(key)` | Retrieves dynamic path parameter `:key` or `*wildcard` | `req.param("id")` |
| `req.query(key)` | Retrieves search query parameter `?key=val` | `req.query("page")` |
| `req.header(key)` | Retrieves HTTP request header value | `req.header("Authorization")` |
| `req.method` | Property returning HTTP method (GET, POST...) | `req.method` |
| `req.path` | Property returning URL path | `req.path` |

---

## 📤 `Response` Class

| Method | Description | Example |
| :--- | :--- | :--- |
| `res.setStatus(code)` | Sets HTTP Status Code (200, 404, 500...) | `res.setStatus(200)` |
| `res.setHeader(name, val)`| Sets HTTP Response Header | `res.setHeader("X-Powered-By", "Vito")` |
| `res.setCookie(key, val, opts)`| Sets Cookie with HMAC signature & HttpOnly/SameSite flags | `res.setCookie("sid", "abc", opts)` |
| `res.json(jsonStr)` | Responds with JSON string and `Content-Type: application/json` | `res.json("{\"ok\":true}")` |
| `res.html(htmlStr)` | Responds with HTML content and `Content-Type: text/html` | `res.html("<h1>Hi</h1>")` |
| `res.send(textStr)` | Responds with Plain Text | `res.send("Hello World")` |
| `res.sendChunk(chunk)` | Sends chunked / streamed response data | `res.sendChunk(data)` |

---

## ⚡ WebSockets, Event Bus & Job Queue (`Phase 13`)

| Function / Structure | Package | Description |
| :--- | :--- | :--- |
| `createWebSocketServer(port, maxConn)` | `packages/websocket/websocket.vit` | Initializes High-Concurrency WebSocket Server engine. |
| `wsServer.broadcastToRoom(room, msg)` | `packages/websocket/websocket.vit` | Broadcasts message to all connected clients in a Room. |
| `wsServer.unicast(client, msg)` | `packages/websocket/websocket.vit` | Sends a private message to a specific WebSocket Client. |
| `wsServer.checkHeartbeats(client, time)` | `packages/websocket/websocket.vit` | Checks Ping/Pong and evicts dead connections (Stale eviction). |
| `compressFrame(payload)` / `decompressFrame` | `packages/websocket/websocket.vit` | Compresses and decompresses WebSocket frames via per-message deflate. |
| `createEventBus()` | `packages/events/event_bus.vit` | Initializes Async Internal Event Bus dispatcher. |
| `eventBus.emitAsync(event, payload)` | `packages/events/event_bus.vit` | Dispatches events asynchronously without blocking main loop. |
| `createRedisPubSubAdapter(host, port)` | `packages/events/redis_pubsub.vit` | Creates Redis Pub/Sub adapter for inter-node cluster communication. |
| `createJobQueue(driver)` | `packages/queue/queue.vit` | Initializes Background Job Queue (Driver: `"memory"` or `"redis"`). |
| `jobQueue.scheduleJob(job, delayMs)` | `packages/queue/queue.vit` | Pushes delayed job into schedule queue based on timer. |
| `jobQueue.processNextJob(shouldFail, reason)` | `packages/queue/queue.vit` | Processes next job, handling Exponential Backoff Retries & DLQ. |
