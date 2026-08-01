# PHASE 12: Native Authentication & Security Suite 🔒

> **Mục tiêu**: Xây dựng giải pháp xác thực (Auth) và phân quyền (RBAC) tích hợp sẵn trong Vito Framework, tối ưu bảo mật ứng dụng web với thuật toán mã hóa hiện đại.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 🔑 Native Authentication Middleware (`packages/auth`)
- [x] **JWT Guard Pro**: Hỗ trợ thuật toán mã hóa `HS256`, `RS256`, `ES256`, tự động verify Token, trích xuất Claims và gắn vào `req.user`.
- [x] **OAuth2 & OIDC Client Helper**: Tích hợp sẵn adapter đăng nhập qua Google, GitHub, Microsoft.
- [x] **API Key Guard**: Middleware xác thực API Key với Hashing & Rate Limit riêng cho từng Client Key.

### 2. 🛡 Cryptographic Utilities & Password Hashing
- [x] **Argon2id & Bcrypt Native Drivers**: Tích hợp thuật toán băm mật khẩu chuẩn OWASP chống tấn công Brute-force & Rainbow Table.
- [x] **AES-256-GCM Encryption**: Module mã hóa dữ liệu nhạy cảm hai chiều (Two-way Encryption).

### 3. 👥 Role-Based Access Control (RBAC) & ABAC
- [x] **Permission Middleware**: Định nghĩa quyền truy cập route dạng `auth.hasRole("admin")` hoặc `auth.hasPermission("posts:delete")`.
- [x] **Policy & Gate Engine**: Hỗ trợ kiểm tra quyền linh hoạt theo ngữ cảnh tài nguyên (Attribute-Based Access Control).

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Xác thực JWT Token với thời gian xử lý $< 0.1ms$ per request.
2. Băm mật khẩu Argon2id tuân thủ khuyến nghị bảo mật OWASP.
3. Từ chối ngay lập tức HTTP 401/403 khi Client không đủ quyền truy cập.
