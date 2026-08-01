# PHASE 13: Real-Time WebSockets & Background Task Queues ⚡

> **Mục tiêu**: Cung cấp giao tiếp hai chiều thời gian thực (Full-Duplex WebSockets) chuẩn Production và hệ thống hàng chờ công việc nền (Background Job Queues / Event Bus) cho Vito.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 🔌 Production-Grade WebSocket Engine (`packages/websocket`)
- [x] **High-Concurrency WebSocket Server**: Xử lý hàng trăm nghìn kết nối đồng thời với bộ nhớ tối thiểu.
- [x] **Rooms & Broadcast System**: Hỗ trợ phân nhóm Room/Channel, gửi tin nhắn Broadcast hoặc Unicast.
- [x] **Heartbeat Ping/Pong & Auto Reconnect**: Tự động dọn dẹp các kết nối chết (Stale Connections) và giữ kết nối mở.
- [x] **Per-Message Deflate Compression**: Nén dữ liệu khung truyền WebSocket để giảm băng thông.

### 2. 📬 Internal Event Bus & Pub/Sub (`packages/events`)
- [x] **Async Event Emitter**: Phát hành và đăng ký sự kiện nội bộ không bất đồng bộ (`app.on("user:registered", handler)`).
- [x] **Redis Pub/Sub Adapter**: Mở rộng giao tiếp sự kiện liên Node trong mô hình đa Server.

### 3. ⏳ Background Job Queue Engine (`packages/queue`)
- [x] **In-Memory & Redis Job Queue**: Hỗ trợ đẩy tác vụ nặng (Gửi email, xử lý ảnh, xuất báo cáo) xuống hàng chờ.
- [x] **Delayed & Scheduled Jobs**: Hỗ trợ lên lịch công việc chạy sau $N$ giây hoặc theo định kỳ Cron.
- [x] **Retry Policy & Dead Letter Queue (DLQ)**: Khôi phục tác vụ thất bại với Exponential Backoff và lưu trữ job lỗi.

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Duy trì 50,000 WebSocket connections đồng thời trên RAM $< 500MB$.
2. Phát sự kiện Event Bus bất đồng bộ không làm ngắt quãng vòng đời HTTP Request chính.
3. Job Queue xử lý tự động retry chính xác theo cấu hình.
