# PHASE 10: Essential Backend Toolkit & Stream Handling 🧰

> **Mục tiêu**: Bổ sung đầy đủ các công cụ Backend tiêu chuẩn không thể thiếu cho ứng dụng web sản xuất: Xử lý Multipart File Upload dạng Stream, Cookie mã hóa, Session Manager và Graceful Shutdown.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 📤 Multipart Form Data & Streaming File Upload (`packages/upload`)
- [ ] **Streaming Multipart Parser**: Phân tích dữ liệu multipart/form-data trực tiếp từ socket stream không nạp toàn bộ dữ liệu vào RAM.
- [ ] **File Save Handler**: Hỗ trợ lưu trữ file lên đĩa cứng (Disk Storage) hoặc chuyển tiếp trực tiếp sang S3/Cloud Storage.
- [ ] **File Validation Engine**: Kiểm tra kích thước file (Max File Size), MIME Type và Extension an toàn trước khi lưu.

### 2. 🍪 Secure Cookie & Session Management (`packages/session`)
- [ ] **Signed & Encrypted Cookies**: Hỗ trợ `res.setCookie(key, val, options)` với HMAC signing và AES encryption chống giả mạo.
- [ ] **Session Middleware**: Quản lý Session lưu trữ trong In-Memory, Redis hoặc Database với Cookie Session ID an toàn.
- [ ] **SameSite, HttpOnly, Secure Flags**: Tự động áp dụng các cờ bảo mật chuẩn cho Cookies.

### 3. 🛑 Graceful Shutdown & Health Checks (`packages/health`)
- [ ] **OS Signal Interceptor**: Lắng nghe các tín hiệu `SIGINT`, `SIGTERM` từ hệ điều hành để hoàn tất các HTTP request đang dở dang trước khi đóng ứng dụng.
- [ ] **Active Connection Tracker**: Đếm và quản lý kết nối đang hoạt động (Active Requests Count).
- [ ] **Built-in Health Check**: Endpoint `/healthz` (Liveness) và `/readyz` (Readiness) cho Kubernetes / Docker Swarm probes.

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Upload file dung lượng lớn (> 1GB) với lượng RAM tiêu thụ duy trì mức ổn định $< 20MB$.
2. Cookie mã hóa chống sửa đổi thông tin phía Client.
3. Ứng dụng phản hồi `503 Service Unavailable` và hoàn tất kết nối cũ khi nhận tín hiệu Shutdown.
