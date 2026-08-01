# Xác Thực & Bảo Mật (Auth & Security Suite) 🔒

Bộ giải pháp **Auth & Security Enterprise** của Vito tích hợp sẵn trong `packages/auth/` và `packages/security/`, đáp ứng tiêu chuẩn bảo mật khắt khe nhất của **OWASP**.

---

## 🔑 1. Native JWT Guard Pro (`packages/auth/jwt.vit`)

Xác thực JWT Token tốc độ cực đại ($< 0.1\text{ms}$ mỗi request) hỗ trợ `HS256`, `RS256`, `ES256`:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Zero-Overhead Verification</h4>
    <p>Giải mã và kiểm tra chữ ký token trực tiếp bằng VIT Native C Engine.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🛡️</div>
    <h4>Claims Validation</h4>
    <p>Tự động xác minh các mốc thời gian `exp`, `nbf`, `iss` và từ chối token hết hạn.</p>
  </div>
</div>

```typescript
import { verifyJwtToken, parseJwtToken, jwtGuardMiddleware } from "vito/packages/auth/jwt.vit";

let token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...";
let secret = "super_secret_vito_key";

// 1. Xác thực trực tiếp
let isValid = verifyJwtToken(token, secret, "HS256");

// 2. Sử dụng dạng Middleware cho Route
app.use("/api/v1/protected/*", jwtGuardMiddleware);
```

---

## 🌐 2. OAuth2 & OIDC Client với PKCE (`packages/auth/oauth.vit`)

Tích hợp sẵn bộ Adapter đăng nhập mạng xã hội (Google, GitHub, Microsoft) kèm cơ chế bảo mật **PKCE** (Proof Key for Code Exchange):

```typescript
import { createGoogleOAuthProvider } from "vito/packages/auth/oauth.vit";

let googleAuth = createGoogleOAuthProvider(
    "client-id-123.apps.googleusercontent.com",
    "client-secret-xyz",
    "https://my-app.com/auth/callback"
);

// Sinh URL Đăng nhập kèm PKCE Challenge Hash
let loginUrl = googleAuth.generateAuthorizationUrl("state_999", "pkce_challenge_hash_val");
```

---

## 🛡️ 3. Argon2id & Bcrypt Hashing (`packages/auth/crypto.vit`)

Băm mật khẩu tuân thủ khuyến nghị của OWASP, chống tấn công Brute-force & GPU Parallel:

::: code-group

```typescript [1. Argon2id Password Hashing]
import { hashPasswordArgon2id, verifyPasswordArgon2id, createDefaultArgon2Params } from "vito/packages/auth/crypto.vit";

let password = "MySuperSecretPassword2026";
let salt = "random_salt_value";
let params = createDefaultArgon2Params(); // Memory 64MB, Iterations 3, Parallelism 4

// Băm mật khẩu
let hash = hashPasswordArgon2id(password, salt, params);

// Kiểm tra mật khẩu
let isMatched = verifyPasswordArgon2id(password, salt, params, hash);
```

```typescript [2. AES-256-GCM Two-Way Encryption]
import { encryptAES256GCM, decryptAES256GCM } from "vito/packages/auth/crypto.vit";

let key = "vito_master_encryption_key_32b";
let iv = "iv_nonce_12b";
let sensitiveData = "User_Credit_Card_Number_9999";

// Mã hóa dữ liệu 2 chiều
let cipherPayload = encryptAES256GCM(sensitiveData, key, iv, "auth_context");

// Giải mã
let plainText = decryptAES256GCM(cipherPayload, key, "auth_context");
```

:::

---

## 👥 4. Phân Quyền RBAC & ABAC Gate Engine (`packages/auth/rbac.vit`)

Quản lý quyền hạn truy cập linh hoạt theo Role hoặc Thuộc tính ngữ cảnh:

```typescript
import { createGateEngine } from "vito/packages/auth/rbac.vit";

let gate = createGateEngine();

// Quy tắc ABAC: Chỉ tác giả của bài viết mới được quyền chỉnh sửa bài viết đó
let canEdit = gate.can("posts:edit", "user", "author_101", "author_101"); // true
let canOtherEdit = gate.can("posts:edit", "user", "user_999", "author_101"); // false (HTTP 403 Forbidden)
```
