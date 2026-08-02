# Authentication & Security Suite 🔒

The **Auth & Security Enterprise** suite is built into `packages/auth/` and `packages/security/`, meeting the strictest **OWASP** security standards.

---

## 🔑 1. Native JWT Guard Pro (`packages/auth/jwt.vit`)

Ultra-fast JWT Token verification (< 0.1ms per request) supporting `HS256`, `RS256`, and `ES256`:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">⚡</div>
    <h4>Zero-Overhead Verification</h4>
    <p>Decodes and verifies token signatures directly via the VIT Native C Engine.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">🛡️</div>
    <h4>Claims Validation</h4>
    <p>Automatically validates `exp`, `nbf`, and `iss` timestamps, rejecting expired tokens.</p>
  </div>
</div>

```typescript
import { verifyJwtToken, parseJwtToken, jwtGuardMiddleware } from "vito/packages/auth/jwt.vit";

let token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...";
let secret = "super_secret_vito_key";

// 1. Direct verification
let isValid = verifyJwtToken(token, secret, "HS256");

// 2. Use as Middleware for Routes
app.use("/api/v1/protected/*", jwtGuardMiddleware);
```

---

## 🌐 2. OAuth2 & OIDC Client with PKCE (`packages/auth/oauth.vit`)

Built-in social login adapters (Google, GitHub, Microsoft) with **PKCE** (Proof Key for Code Exchange) security:

```typescript
import { createGoogleOAuthProvider } from "vito/packages/auth/oauth.vit";

let googleAuth = createGoogleOAuthProvider(
    "client-id-123.apps.googleusercontent.com",
    "client-secret-xyz",
    "https://my-app.com/auth/callback"
);

// Generate Login URL with PKCE Challenge Hash
let loginUrl = googleAuth.generateAuthorizationUrl("state_999", "pkce_challenge_hash_val");
```

---

## 🛡️ 3. Argon2id & Bcrypt Hashing (`packages/auth/crypto.vit`)

OWASP-recommended password hashing, resistant against Brute-force & GPU Parallel attacks:

::: code-group

```typescript [1. Argon2id Password Hashing]
import { hashPasswordArgon2id, verifyPasswordArgon2id, createDefaultArgon2Params } from "vito/packages/auth/crypto.vit";

let password = "MySuperSecretPassword2026";
let salt = "random_salt_value";
let params = createDefaultArgon2Params(); // Memory 64MB, Iterations 3, Parallelism 4

// Hash the password
let hash = hashPasswordArgon2id(password, salt, params);

// Verify the password
let isMatched = verifyPasswordArgon2id(password, salt, params, hash);
```

```typescript [2. AES-256-GCM Two-Way Encryption]
import { encryptAES256GCM, decryptAES256GCM } from "vito/packages/auth/crypto.vit";

let key = "vito_master_encryption_key_32b";
let iv = "iv_nonce_12b";
let sensitiveData = "User_Credit_Card_Number_9999";

// Two-way encrypt data
let cipherPayload = encryptAES256GCM(sensitiveData, key, iv, "auth_context");

// Decrypt
let plainText = decryptAES256GCM(cipherPayload, key, "auth_context");
```

:::

---

## 👥 4. RBAC & ABAC Gate Engine (`packages/auth/rbac.vit`)

Flexible access control management by Role or contextual Attributes:

```typescript
import { createGateEngine } from "vito/packages/auth/rbac.vit";

let gate = createGateEngine();

// ABAC Rule: Only the author of a post can edit it
let canEdit = gate.can("posts:edit", "user", "author_101", "author_101"); // true
let canOtherEdit = gate.can("posts:edit", "user", "user_999", "author_101"); // false (HTTP 403 Forbidden)
```
