# WebSockets & Background Job Queues ⚡

**Vito Framework** cung cấp hạ tầng giao tiếp thời gian thực hai chiều chuẩn Production (**Production-Grade WebSockets Engine**) và hệ thống hàng chờ công việc nền (**Background Job Queues & Async Event Bus**) với độ trễ thấp và khả năng mở rộng cao.

---

## 🔌 Production-Grade WebSocket Engine (`packages/websocket`)

Module `packages/websocket/websocket.vit` cung cấp giải pháp xử lý kết nối WebSocket độ tin cậy cao, hỗ trợ phân nhóm Room/Channel, kiểm tra kết nối sống (Heartbeat Ping/Pong) và nén dữ liệu theo khung truyền per-message deflate.

### Tính Năng Nổi Bật:
- **Xử Lý Kết Nối Đồng Thời**: Hỗ trợ tới 50,000+ kết nối mở đồng thời với chi phí bộ nhớ tối thiểu ($< 500MB$ RAM).
- **Phân Nhóm Room & Broadcast**: Phân chia kết nối vào các Room / Channel riêng biệt, hỗ trợ gửi tin nhắn toàn hệ thống (Broadcast), theo phòng (Room Broadcast) hoặc tin nhắn riêng (Unicast).
- **Heartbeat Ping/Pong & Stale Eviction**: Tự động gửi PING frame định kỳ và thu hồi (Evict) các kết nối bị đứt ngầm hoặc hết thời gian chờ (Stale connections).
- **Per-Message Deflate Compression**: Nén nòng dữ liệu WebSocket frame để giảm thiểu băng thông mạng khi truyền dữ liệu lớn.

### Ví Dụ Sử Dụng:

```typescript
import { createWebSocketServer, compressFrame, decompressFrame } from "vito/packages/websocket/websocket.vit";

// 1. Khởi tạo WebSocket Server (Cổng 8080, Tối đa 50,000 kết nối)
let wsServer = createWebSocketServer(8080, 50000);

// 2. Tiếp nhận kết nối mới
let client1 = wsServer.acceptConnection("192.168.1.10");
let client2 = wsServer.acceptConnection("192.168.1.11");

// 3. Quản lý Room & Gửi tin nhắn Broadcast
let chatRoom: WebSocketRoom;
chatRoom.init("room_vip_lounge");
chatRoom.addClient(client1.id);
chatRoom.addClient(client2.id);

wsServer.broadcastToRoom(chatRoom, "{\"event\":\"chat:msg\",\"text\":\"Chào mừng tới VIP Room!\"}");

// 4. Kiểm tra Heartbeat & Dọn dẹp kết nối chết
wsServer.checkHeartbeats(client1, 1700000010);
```

---

## 📬 Internal Async Event Bus & Redis Pub/Sub (`packages/events`)

Hệ thống sự kiện nội bộ giúp tách rời (decouple) logic nghiệp vụ của ứng dụng mà không làm gián đoạn vòng đời xử lý HTTP Request chính.

### Tính Năng Nổi Bật:
- **Async Event Emitter** (`packages/events/event_bus.vit`): Đăng ký và phát hành sự kiện bất đồng bộ (`app.on("user:registered", handler)`).
- **Redis Pub/Sub Cluster Adapter** (`packages/events/redis_pubsub.vit`): Mở rộng giao tiếp sự kiện liên Node trong mô hình hạ tầng đa Server (Distributed Cluster).

### Ví Dụ Sử Dụng:

```typescript
import { createEventBus } from "vito/packages/events/event_bus.vit";
import { createRedisPubSubAdapter } from "vito/packages/events/redis_pubsub.vit";

// 1. Đăng ký & Dispatch Sự Kiện Nội Bộ
let eventBus = createEventBus();
eventBus.on("user:registered", "SendWelcomeEmailHandler");
eventBus.on("user:registered", "AuditLoggerHandler");

// Phát hành sự kiện không ngắt quãng HTTP Thread
eventBus.emitAsync("user:registered", "{\"userId\":\"usr_999\"}");

// 2. Redis Pub/Sub liên Node
let redisPubSub = createRedisPubSubAdapter("127.0.0.1", 6379);
redisPubSub.subscribeChannel("vito:cluster:events");
redisPubSub.publishCluster("vito:cluster:events", "{\"event\":\"cache:purge\"}");
```

---

## ⏳ Background Job Queue Engine (`packages/queue`)

Module `packages/queue/queue.vit` cho phép đẩy các tác vụ nặng (Gửi email hàng loạt, xử lý ảnh, tạo báo cáo) xuống hàng chờ công việc nền để xử lý bất đồng bộ.

### Tính Năng Nổi Bật:
- **Hỗ Trợ Multi-Driver**: Driver `Memory` (Trong bộ nhớ) cho ứng dụng đơn lẻ và `Redis` cho môi trường Production đa Server.
- **Hoãn & Lên Lịch Chạy Job**: Hỗ trợ hoãn chạy công việc theo thời gian tùy chỉnh (`scheduleJob`).
- **Cơ Chế Thử Lại (Exponential Backoff Retry)**: Tự động khôi phục tác vụ thất bại theo chiến lược tăng thời gian chờ mũ ($1s, 2s, 4s, ...$).
- **Dead Letter Queue (DLQ)**: Cách ly và lưu trữ các job lỗi quá số lần retry cho phép (`maxRetries`) để phục vụ quản trị và kiểm tra.

### Ví Dụ Sử Dụng:

```typescript
import { createJobQueue, Job } from "vito/packages/queue/queue.vit";

// 1. Khởi tạo Hàng chờ công việc Redis
let jobQueue = createJobQueue("redis");

// 2. Khởi tạo Job & Đẩy vào hàng chờ
let emailJob: Job;
emailJob.init("job_email_001", "email:send", "{\"to\":\"dev@vito.dev\"}");
jobQueue.pushJob(emailJob);

// 3. Lên lịch hoãn chạy Job (Delay 5000ms)
let reportJob: Job;
reportJob.init("job_report_002", "report:export", "{\"format\":\"pdf\"}");
jobQueue.scheduleJob(reportJob, 5000);

// 4. Worker xử lý Job & Tự động đẩy vào Dead Letter Queue (DLQ) nếu thất bại
jobQueue.processNextJob(false, ""); // Thành công

// Thử lại thất bại -> Chuyển vào DLQ khi vượt maxRetries
jobQueue.processNextJob(true, "Lỗi kết nối tới dịch vụ Email");
```
