# Authentication & Security Suite 🔒

**Vito Framework** provides an enterprise-grade Authentication, Role-Based Access Control (RBAC/ABAC), Two-Way Encryption (AES-256-GCM), and Password Hashing (Argon2id/Bcrypt) solution native to the `packages/auth/` ecosystem.

---

## 🔑 Native JWT Guard Pro (`packages/auth/jwt.vit`)

The JWT Guard Pro module supports high-speed token verification ($< 0.1ms$ per request) with modern cryptographic algorithms `HS256`, `RS256`, and `ES256`.

### Key Features:
- **Multi-Algorithm Support**: `HS256` (Symmetric HMAC), `RS256` (Asymmetric RSA), `ES256` (ECDSA).
- **Automatic Claims Extraction & Validation**: Verifies signature, Bearer scheme, and standard claims such as `exp` (Expiration Time), `nbf` (Not Before), and `iss` (Issuer).
- **Fast Rejection**: Rejects unauthorized requests immediately with HTTP `401 Unauthorized`.

### Usage Example:

```typescript
import { parseJwtToken, verifyJwtToken, validateJwtClaims, jwtGuardMiddleware } from "vito/packages/auth/jwt.vit";

// 1. Verify JWT Signature
let token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMDAxIn0.sig_hs256_5381";
let secret = "super_secret_vito_hs_key";

let isValid = verifyJwtToken(token, secret, "HS256");
if (isValid) {
    print("JWT Signature is Valid!");
}

// 2. Extract & Validate Claims
let jwt = parseJwtToken(token);
let isClaimsValid = validateJwtClaims(jwt, 1700000000, 1800000000, 1600000000);

// 3. Register JWT Guard Middleware in Vito App
app.use(jwtGuardMiddleware);
```

---

## 🌐 OAuth2 & OIDC Client Helper (`packages/auth/oauth.vit`)

Built-in social login adapters for Google, GitHub, and Microsoft featuring **PKCE** (Proof Key for Code Exchange) security mechanism.

### Usage Example:

```typescript
import {
    createGoogleOAuthProvider,
    createGitHubOAuthProvider,
    createMicrosoftOAuthProvider,
    parseUserInfoPayload
} from "vito/packages/auth/oauth.vit";

// Initialize Google OAuth Provider
let googleConfig = createGoogleOAuthProvider(
    "google-client-id-123",
    "secret-xyz",
    "https://app.vito.dev/auth/callback"
);

// Generate PKCE Authorization URL
let authUrl = googleConfig.generateAuthorizationUrl("state_sec_999", "pkce_challenge_hash_val");
// -> https://accounts.google.com/o/oauth2/v2/auth?response_type=code&client_id=...&code_challenge=...

// Build Token Exchange Request Body
let tokenBody = googleConfig.buildTokenExchangeRequest("code_auth_888", "code_verifier_123");

// Parse OIDC Standard UserInfo Payload
let userProfile = parseUserInfoPayload("Google", jsonResponse);
print("User Email: " + userProfile.email);
```

---

## 🔑 API Key Guard & Per-Client Rate Limit (`packages/auth/apikey.vit`)

API Key authentication designed for client applications, mobile apps, and service-to-service communication with single-way key hashing and per-client-key rate limiting.

### Usage Example:

```typescript
import { createApiKeyStore, generateApiKey, hashApiKey, createApiKeyGuard } from "vito/packages/auth/apikey.vit";

let keyStore = createApiKeyStore();

// Generate API Key with Prefix
let rawApiKey = generateApiKey("vk_live", "client_acme_corp_seed");
let keyHash = hashApiKey(rawApiKey);

// Register Key in Store (quota: 2 requests / window)
keyStore = keyStore.registerKey(rawApiKey, "vk_live", keyHash, "Acme Mobile App", 2);

// Create API Key Guard Middleware
let apiKeyGuard = createApiKeyGuard(keyStore);

// Automatically returns HTTP 401 on invalid keys, or HTTP 429 when quota is exceeded
let isAllowed = apiKeyGuard.handleRequest(req, res);
```

---

## 🛡️ Argon2id & Bcrypt Password Hashing (`packages/auth/crypto.vit`)

OWASP recommended password hashing algorithms resistant against Brute-force and Rainbow Table attacks.

### Argon2id & Bcrypt Hashing Example:

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

// 1. Argon2id (OWASP Specs: Memory 64MB, Iterations 3, Parallelism 4)
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

Two-way encryption for sensitive data (SSN, payment cards, credentials) using **AES-256-GCM AEAD** formatted as `iv:ciphertext:tag`.

```typescript
import { encryptAES256GCM, decryptAES256GCM, formatCiphertextString, timingSafeEqual } from "vito/packages/auth/crypto.vit";

let masterKey = "vito_master_encryption_key_32b";
let ivNonce = "iv_nonce_12b_rnd";
let sensitiveData = "User_SSN_Payment_Card_Data_9999";

// Encrypt payload
let cipherPayload = encryptAES256GCM(sensitiveData, masterKey, ivNonce, "auth_context");
let formatted = formatCiphertextString(cipherPayload); // "iv:ciphertext:tag"

// Decrypt payload
let recoveredText = decryptAES256GCM(cipherPayload, masterKey, "auth_context");

// Constant-Time String Comparison (Prevents Side-Channel Timing Attacks)
if (timingSafeEqual(recoveredText, sensitiveData)) {
    print("Decrypted payload matches original!");
}
```

---

## 👥 Role-Based (RBAC) & Attribute-Based Access Control (ABAC) (`packages/auth/rbac.vit`)

Flexible authorization management based on Roles (`hasRole`), Permissions (`hasPermission`), or Resource Context (`GateEngine`).

### 1. RBAC Guards:

```typescript
import { createRoleGuard, createPermissionGuard } from "vito/packages/auth/rbac.vit";

// Require 'admin' role
let roleGuard = createRoleGuard("admin", userCurrentRoles);
let isRoleAuthorized = roleGuard.handleRequest(req, res); // HTTP 403 if missing role

// Require 'posts:delete' permission
let permGuard = createPermissionGuard("posts:delete", userCurrentPermissions);
let isPermAuthorized = permGuard.handleRequest(req, res);
```

### 2. ABAC Policy & Gate Engine:

```typescript
import { createGateEngine } from "vito/packages/auth/rbac.vit";

let gate = createGateEngine();

// Verify edit post permission (Users can only edit posts they own)
let canEdit = gate.can("posts:edit", "user", "usr_100", "usr_100"); // true (Owner)
let canOtherEdit = gate.can("posts:edit", "user", "usr_999", "usr_100"); // false (HTTP 403 Forbidden)
```
