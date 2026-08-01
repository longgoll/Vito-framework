# Xác Thực & Bảo Mật (Auth & Security Suite) 🔒

**Vito Framework** cung cấp giải pháp Xác thực (Authentication), Phân quyền (RBAC/ABAC), Mã hóa hai chiều (AES-256-GCM) và Băm mật khẩu (Argon2id/Bcrypt) tích hợp sẵn trong hệ sinh thái `packages/auth/`.

---

## 🔑 Native JWT Guard Pro (`packages/auth/jwt.vit`)

Module JWT Guard Pro hỗ trợ xác thực token tốc độ cao ($< 0.1ms$ per request) với các thuật toán mã hóa hiện đại `HS256`, `RS256`, `ES256`.

### Tính Năng Nổi Bật:
- **Hỗ Trợ Đa Thuật Toán**: `HS256` (Symmetric HMAC), `RS256` (Asymmetric RSA), `ES256` (ECDSA).
- **Tự Động Trích Xuất & Kiểm Tra Claims**: Kiểm tra chữ ký, định dạng Bearer Token, và các claims chuẩn như `exp` (Expiration Time), `nbf` (Not Before), `iss` (Issuer).
- **Phản Hồi Nhanh**: Từ chối ngay lập tức với HTTP `401 Unauthorized` khi chữ ký hoặc token không hợp lệ.

### Ví Dụ Sử Dụng:

```typescript
import { parseJwtToken, verifyJwtToken, validateJwtClaims, jwtGuardMiddleware } from "vito/packages/auth/jwt.vit";

// 1. Xác thực chữ ký JWT Token
let token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMDAxIn0.sig_hs256_5381";
let secret = "super_secret_vito_hs_key";

let isValid = verifyJwtToken(token, secret, "HS256");
if (isValid) {
    print("Token chữ ký hợp lệ!");
}

// 2. Trích xuất & Kiểm tra Claims
let jwt = parseJwtToken(token);
let isClaimsValid = validateJwtClaims(jwt, 1700000000, 1800000000, 1600000000);

// 3. Tích hợp JWT Guard Middleware vào ứng dụng Vito
app.use(jwtGuardMiddleware);
```

---

## 🌐 OAuth2 & OIDC Client Helper (`packages/auth/oauth.vit`)

Tích hợp sẵn các adapter đăng nhập mạng xã hội và đơn vị định danh OIDC (Google, GitHub, Microsoft) kèm cơ chế bảo mật **PKCE** (Proof Key for Code Exchange).

### Ví Dụ Sử Dụng:

```typescript
import {
    createGoogleOAuthProvider,
    createGitHubOAuthProvider,
    createMicrosoftOAuthProvider,
    parseUserInfoPayload
} from "vito/packages/auth/oauth.vit";

// Khởi tạo Google Provider
let googleConfig = createGoogleOAuthProvider(
    "google-client-id-123",
    "secret-xyz",
    "https://app.vito.dev/auth/callback"
);

// Tạo Authorization URL với PKCE
let authUrl = googleConfig.generateAuthorizationUrl("state_sec_999", "pkce_challenge_hash_val");
// -> https://accounts.google.com/o/oauth2/v2/auth?response_type=code&client_id=...&code_challenge=...

// Tạo Token Exchange Request Body
let tokenBody = googleConfig.buildTokenExchangeRequest("code_auth_888", "code_verifier_123");

// Parse OIDC UserInfo Payload chuẩn
let userProfile = parseUserInfoPayload("Google", jsonResponse);
print("User Email: " + userProfile.email);
```

---

## 🔑 API Key Guard & Per-Client Rate Limit (`packages/auth/apikey.vit`)

Xác thực API Key dành cho Client / Mobile App / Service-to-Service giao tiếp qua REST API với cơ chế băm 1 chiều và giới hạn tốc độ truy cập theo từng Client Key.

### Ví Dụ Sử Dụng:

```typescript
import { createApiKeyStore, generateApiKey, hashApiKey, createApiKeyGuard } from "vito/packages/auth/apikey.vit";

let keyStore = createApiKeyStore();

// Sinh API Key mới với Prefix
let rawApiKey = generateApiKey("vk_live", "client_acme_corp_seed");
let keyHash = hashApiKey(rawApiKey);

// Đăng ký API Key vào Store (tối đa 2 requests / window)
keyStore = keyStore.registerKey(rawApiKey, "vk_live", keyHash, "Acme Mobile App", 2);

// Khởi tạo Middleware Guard
let apiKeyGuard = createApiKeyGuard(keyStore);

// Tự động trả về HTTP 401 khi Key không hợp lệ, hoặc HTTP 429 khi vượt Hạn ngạch (Rate Limit)
let isAllowed = apiKeyGuard.handleRequest(req, res);
```

---

## 🛡️ Argon2id & Bcrypt Password Hashing (`packages/auth/crypto.vit`)

Băm mật khẩu tuân thủ khuyến nghị của tổ chức bảo mật **OWASP**, chống lại các hình thức tấn công Brute-force & Rainbow Table.

### Ví Dụ Băm Mật Khẩu Argon2id & Bcrypt:

```typescript
import {
    createDefaultArgon2Params,
    hashPasswordArgon2id,
    verifyPasswordArgon2id,
    hashPasswordBcrypt,
    verifyPasswordBcrypt
} from "vito/packages/auth/crypto.vit";

let plainPassword = "VitoSuperSecretPassword2026";
let salt = "salt_vito_security_suite";

// 1. Argon2id (OWASP: Memory 64MB, Iterations 3, Parallelism 4)
let argon2Params = createDefaultArgon2Params();
let argon2Hash = hashPasswordArgon2id(plainPassword, salt, argon2Params);

// Verify Argon2id Password
let isArgonValid = verifyPasswordArgon2id(plainPassword, salt, argon2Params, argon2Hash);

// 2. Bcrypt (Cost Factor = 12)
let bcryptHash = hashPasswordBcrypt(plainPassword, salt, 12);
let isBcryptValid = verifyPasswordBcrypt(plainPassword, salt, 12, bcryptHash);
```

---

## 🔐 AES-256-GCM AEAD Two-Way Encryption (`packages/auth/crypto.vit`)

Mã hóa hai chiều dữ liệu nhạy cảm (Thẻ thanh toán, SSN, Bí mật hệ thống) với thuật toán **AES-256-GCM AEAD** có định dạng `iv:ciphertext:tag`.

```typescript
import { encryptAES256GCM, decryptAES256GCM, formatCiphertextString, timingSafeEqual } from "vito/packages/auth/crypto.vit";

let masterKey = "vito_master_encryption_key_32b";
let ivNonce = "iv_nonce_12b_rnd";
let sensitiveData = "User_SSN_Payment_Card_Data_9999";

// Mã hóa dữ liệu
let cipherPayload = encryptAES256GCM(sensitiveData, masterKey, ivNonce, "auth_context");
let formatted = formatCiphertextString(cipherPayload); // "iv:ciphertext:tag"

// Giải mã dữ liệu
let recoveredText = decryptAES256GCM(cipherPayload, masterKey, "auth_context");

// So sánh chuỗi Constant-Time (Phòng chống Side-Channel Timing Attack)
if (timingSafeEqual(recoveredText, sensitiveData)) {
    print("Dữ liệu giải mã hoàn toàn chính xác!");
}
```

---

## 👥 Role-Based (RBAC) & Attribute-Based Access Control (ABAC) (`packages/auth/rbac.vit`)

Phân quyền truy cập ứng dụng linh hoạt theo Vai trò (`hasRole`), Quyền hạn (`hasPermission`), hoặc Ngữ cảnh tài nguyên (`GateEngine`).

### 1. RBAC Guards:

```typescript
import { createRoleGuard, createPermissionGuard } from "vito/packages/auth/rbac.vit";

// Yêu cầu vai trò 'admin'
let roleGuard = createRoleGuard("admin", userCurrentRoles);
let isRoleAuthorized = roleGuard.handleRequest(req, res); // HTTP 403 nếu không đủ quyền

// Yêu cầu quyền 'posts:delete'
let permGuard = createPermissionGuard("posts:delete", userCurrentPermissions);
let isPermAuthorized = permGuard.handleRequest(req, res);
```

### 2. ABAC Policy & Gate Engine:

```typescript
import { createGateEngine } from "vito/packages/auth/rbac.vit";

let gate = createGateEngine();

// Kiểm tra quyền sửa bài viết (User chỉ được sửa bài viết do chính mình sở hữu)
let canEdit = gate.can("posts:edit", "user", "usr_100", "usr_100"); // true (Owner)
let canOtherEdit = gate.can("posts:edit", "user", "usr_999", "usr_100"); // false (HTTP 403 Forbidden)
```
