# Vito API Reference 📚

Tài liệu chi tiết về các hàm, đối tượng chính trong **Vito Web Framework Core Engine**.

---

## 🏗 `VitoApp` Class

### `createApp()`
Khởi tạo một thể hiện mới của ứng dụng Vito.

```javascript
let app = createApp();
```

### `app.get(path, handler)` / `app.post(path, handler)` / `app.put()` / `app.delete()`
Đăng ký route handler cho phương thức HTTP tương ứng.

### `app.group(prefix, callback)`
Tạo nhóm các đường dẫn với chung tiền tố URL.

### `app.use(middleware)`
Thêm middleware vào pipeline xử lý request.

### `app.setStrictSlash(strict: boolean)`
Bật/tắt chế độ Strict Trailing Slash Normalization (`/users/` vs `/users`). Mặc định: `false` (Lax mode).

### `app.setNotFoundHandler(handler)`
Thiết lập handler tùy chỉnh khi không tìm thấy route (404 Not Found).

### `app.listen(port)`
Mở socket và bắt đầu lắng nghe kết nối HTTP trên cổng được chỉ định.

---

## 📊 Observability, Metrics & DX (`Phase 14`)

| Hàm / Structure | Package | Mô Tả |
| :--- | :--- | :--- |
| `createPrometheusRegistry()` | `packages/metrics/metrics.vit` | Khởi tạo Prometheus Metrics Registry thu thập chỉ số HTTP & OS. |
| `registry.recordRequest(method, path, status, duration)` | `packages/metrics/metrics.vit` | Ghi nhận thông số HTTP Request cho các phân vị latency (p50, p90, p99). |
| `registry.renderPrometheusMetrics()` | `packages/metrics/metrics.vit` | Xuất chuỗi dữ liệu định dạng chuẩn Prometheus exposition format text. |
| `extractTraceParent(header)` / `injectTraceParent` | `packages/metrics/metrics.vit` | Trích xuất và truyền dẫn OpenTelemetry (OTel) W3C `traceparent` headers. |
| `createMockHttpEngine()` | `packages/testing/testing.vit` | Khởi tạo In-Memory Fast HTTP Injector cho Test Suite. |
| `mockEngine.inject(app, method, path, headers, body)` | `packages/testing/testing.vit` | Giả lập HTTP Request trực tiếp trên RAM không qua TCP socket. |
| `createTestDbFixture(name)` | `packages/testing/testing.vit` | Khởi tạo bộ nạp dữ liệu mẫu cho Test Suite. |
| `fixture.beginIsolationTransaction()` / `rollbackIsolationTransaction` | `packages/testing/testing.vit` | Tự động ROLLBACK CSDL sau mỗi test case để duy trì trạng thái sạch. |
| `createVitCodeGenerator()` | `packages/cli/cli.vit` | Sinh mã nguồn mẫu cho Controller, Service, Middleware, Model, Migration. |
| `createVitDoctor()` | `packages/cli/cli.vit` | Thực thi chẩn đoán môi trường phát triển và kiểm tra tài nguyên hệ thống. |

---

## 🔒 Auth & Security Suite Packages (`packages/auth/`)

| Function / Struct | Package | Mô Tả |
| :--- | :--- | :--- |
| `verifyJwtToken(token, secret, algo)` | `packages/auth/jwt.vit` | Xác thực chữ ký JWT Token (`HS256`, `RS256`, `ES256`) trong $< 0.1\text{ms}$. |
| `parseJwtToken(token)` | `packages/auth/jwt.vit` | Trích xuất thông tin Header và Payload Claims từ chuỗi JWT Raw. |
| `validateJwtClaims(jwt, now, exp, nbf)` | `packages/auth/jwt.vit` | Kiểm tra thời gian hết hạn (`exp`) và chưa hiệu lực (`nbf`). |
| `jwtGuardMiddleware` | `packages/auth/jwt.vit` | Middleware tự động xác thực Bearer token và từ chối HTTP 401 khi không đủ quyền. |
| `createGoogleOAuthProvider(...)` | `packages/auth/oauth.vit` | Khởi tạo OAuth2 & OIDC Config cho Google Provider với PKCE. |
| `createGitHubOAuthProvider(...)` | `packages/auth/oauth.vit` | Khởi tạo OAuth2 Config cho GitHub Provider. |
| `createMicrosoftOAuthProvider(...)` | `packages/auth/oauth.vit` | Khởi tạo OAuth2 Config cho Microsoft Azure AD / Entra ID. |
| `config.generateAuthorizationUrl(state, challenge)` | `packages/auth/oauth.vit` | Sinh URL chuyển hướng đăng nhập OAuth2 tích hợp PKCE challenge. |
| `createApiKeyStore()` | `packages/auth/apikey.vit` | Khởi tạo Bộ lưu trữ API Key và quản lý Hạn ngạch Rate Limit từng Client Key. |
| `hashApiKey(rawKey)` | `packages/auth/apikey.vit` | Mã hóa 1 chiều API Key bảo mật cho việc lưu trữ CSDL. |
| `hashPasswordArgon2id(pass, salt, params)` | `packages/auth/crypto.vit` | Băm mật khẩu Argon2id tuân thủ khuyến nghị OWASP. |
| `hashPasswordBcrypt(pass, salt, cost)` | `packages/auth/crypto.vit` | Băm mật khẩu Bcrypt với cấu hình Cost Factor tùy chỉnh. |
| `encryptAES256GCM(plain, key, iv, ad)` | `packages/auth/crypto.vit` | Mã hóa dữ liệu hai chiều chuẩn AES-256-GCM AEAD (`iv:ciphertext:tag`). |
| `decryptAES256GCM(payload, key, ad)` | `packages/auth/crypto.vit` | Giải mã dữ liệu và kiểm tra tính toàn vẹn GCM Tag. |
| `timingSafeEqual(strA, strB)` | `packages/auth/crypto.vit` | So sánh chuỗi Constant-time phòng chống Side-channel Timing Attack. |
| `createRoleGuard(role, currentRoles)` | `packages/auth/rbac.vit` | Khởi tạo RBAC Guard kiểm tra vai trò người dùng (từ chối HTTP 403). |
| `createPermissionGuard(perm, currentPerms)` | `packages/auth/rbac.vit` | Khởi tạo RBAC Guard kiểm tra quyền hạn thao tác route. |
| `createGateEngine()` | `packages/auth/rbac.vit` | Khởi tạo Engine kiểm tra phân quyền ABAC theo ngữ cảnh tài nguyên. |

---

## 🗄️ Database & ORM Packages (`packages/db` & `packages/orm`)

| Function / Struct | Package | Mô Tả |
| :--- | :--- | :--- |
| `createConnectionPool(type, connStr, min, max)` | `packages/db/db_pool.vit` | Khởi tạo Connection Pool tự động mở rộng/co hẹp và khôi phục ngầm. |
| `pool.acquireConnection()` | `packages/db/db_pool.vit` | Mượn một kết nối CSDL hợp lệ từ Connection Pool. |
| `pool.checkHealthAndReconnect()` | `packages/db/db_pool.vit` | Kiểm tra sức khỏe kết nối & tự động tái kết nối ngầm ($< 100\text{ms}$). |
| `beginTransaction(id)` | `packages/orm/transaction.vit` | Khởi tạo Giao dịch CSDL nguyên tử (Atomic Transaction) hỗ trợ ACID. |
| `tx.createSavepoint(name)` | `packages/orm/transaction.vit` | Tạo điểm lưu Savepoint cho giao dịch lồng nhau. |
| `tx.rollbackToSavepoint(name)` | `packages/orm/transaction.vit` | Khôi phục trạng thái giao dịch về Savepoint được chỉ định. |
| `createMigrationRunner()` | `packages/orm/migration.vit` | Tạo đối tượng quản lý và chạy Migration (Up/Down) lưu vào `schema_migrations`. |
| `createSchemaDiffGenerator()` | `packages/orm/migration.vit` | Sinh câu lệnh SQL Migration tự động từ định nghĩa Model Vito ORM. |
| `createDatabaseSeeder()` | `packages/orm/migration.vit` | Tạo bộ nạp dữ liệu mẫu (Database Seeder) cho môi trường Dev & Staging. |

---

## ⚡ Performance Packages (`packages/pool` & `packages/router`)

| Function / Helper | Package | Mô Tả |
| :--- | :--- | :--- |
| `createMemoryArena()` | `packages/pool/pool.vit` | Tạo đối tượng quản lý Request Memory Arena slots tái sử dụng. |
| `createTcpBufferPool()` | `packages/pool/pool.vit` | Tạo Buffer Recycling Pool cho TCP Read/Write buffers. |
| `zeroAllocHeaderLookup(raw, target)` | `packages/pool/pool.vit` | Trích xuất giá trị Header trực tiếp từ raw HTTP stream không cấp phát memory. |
| `createRadixRouter(strict)` | `packages/router/router.vit` | Khởi tạo Static Route Fast Path Router với $O(1)$ memory slot lookup. |
| `normalizePath(path, strict)` | `packages/router/router.vit` | Chuẩn hóa đường dẫn URL theo chế độ Lax / Strict slash. |

---

## 📥 `Request` Class

| Phương Thức | Mô Tả | Ví Dụ |
| :--- | :--- | :--- |
| `req.param(key)` | Lấy giá trị tham số dynamic path `:key` hoặc `*wildcard` | `req.param("id")` |
| `req.query(key)` | Lấy giá trị search query string `?key=val` | `req.query("page")` |
| `req.header(key)` | Lấy giá trị HTTP request header | `req.header("Authorization")` |
| `req.method` | Thuộc tính trả về phương thức HTTP (GET, POST...) | `req.method` |
| `req.path` | Thuộc tính trả về đường dẫn URL | `req.path` |

---

## 📤 `Response` Class

| Phương Thức | Mô Tả | Ví Dụ |
| :--- | :--- | :--- |
| `res.setStatus(code)` | Thiết lập HTTP Status Code (200, 404, 500...) | `res.setStatus(200)` |
| `res.setHeader(name, val)`| Thiết lập HTTP Response Header | `res.setHeader("X-Powered-By", "Vito")` |
| `res.setCookie(key, val, opts)`| Ghi Cookie với chữ ký mã hóa HMAC & cờ HttpOnly/SameSite | `res.setCookie("sid", "abc", opts)` |
| `res.json(jsonStr)` | Trả về chuỗi JSON với header `Content-Type: application/json` | `res.json("{\"ok\":true}")` |
| `res.html(htmlStr)` | Trả về nội dung HTML với `Content-Type: text/html` | `res.html("<h1>Hi</h1>")` |
| `res.send(textStr)` | Trả về chuỗi Plain Text | `res.send("Hello World")` |
| `res.sendChunk(chunk)` | Trả về dữ liệu kiểu Stream / Chunked response | `res.sendChunk(data)` |

---

## ⚡ WebSockets, Event Bus & Job Queue (`Phase 13`)

| Hàm / Structure | Package | Mô Tả |
| :--- | :--- | :--- |
| `createWebSocketServer(port, maxConn)` | `packages/websocket/websocket.vit` | Khởi tạo High-Concurrency WebSocket Server engine. |
| `wsServer.broadcastToRoom(room, msg)` | `packages/websocket/websocket.vit` | Gửi tin nhắn Broadcast đến toàn bộ kết nối trong một Room. |
| `wsServer.unicast(client, msg)` | `packages/websocket/websocket.vit` | Gửi tin nhắn đơn tới một WebSocket Client cụ thể. |
| `wsServer.checkHeartbeats(client, time)` | `packages/websocket/websocket.vit` | Kiểm tra Ping/Pong và giải phóng kết nối chết (Stale eviction). |
| `compressFrame(payload)` / `decompressFrame` | `packages/websocket/websocket.vit` | Nén và giải nén dữ liệu khung truyền WebSocket per-message deflate. |
| `createEventBus()` | `packages/events/event_bus.vit` | Khởi tạo Async Internal Event Bus dispatcher. |
| `eventBus.emitAsync(event, payload)` | `packages/events/event_bus.vit` | Phát hành sự kiện bất đồng bộ không ngắt luồng chính. |
| `createRedisPubSubAdapter(host, port)` | `packages/events/redis_pubsub.vit` | Tạo Redis Pub/Sub adapter giao tiếp liên Node trong cụm Server. |
| `createJobQueue(driver)` | `packages/queue/queue.vit` | Khởi tạo Background Job Queue (Driver: `"memory"` hoặc `"redis"`). |
| `jobQueue.scheduleJob(job, delayMs)` | `packages/queue/queue.vit` | Đẩy job vào hàng chờ hoãn thi hành theo thời gian thiết lập. |
| `jobQueue.processNextJob(shouldFail, reason)` | `packages/queue/queue.vit` | Xử lý job tiếp theo, hỗ trợ Exponential Backoff Retry & DLQ. |
