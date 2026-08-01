# Realtime WebSockets & Job Queue ⚡

Hạ tầng giao tiếp thời gian thực hai chiều (**WebSockets Server Engine**), **SSE AI Streaming** và hệ thống hàng chờ công việc nền (**Background Job Queues**) giúp Vito xử lý trôi chảy các bài toán quy mô lớn.

---

## 🔌 1. Enterprise WebSocket Engine (`packages/websocket`)

Truyền nhận dữ liệu 2 chiều độ trễ siêu thấp, hỗ trợ 50,000+ kết nối mở đồng thời với chi phí RAM $< 500\text{MB}$:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📡</div>
    <h4>Room & Broadcast Engine</h4>
    <p>Phân nhóm kết nối vào các Room / Channel riêng biệt, gửi broadcast theo phòng hoặc toàn hệ thống.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">💓</div>
    <h4>Heartbeat Ping/Pong</h4>
    <p>Tự động phát hiện và thu hồi (Evict) các kết nối bị đứt ngầm hoặc hết thời gian chờ.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4>Per-Message Deflate</h4>
    <p>Nén WebSocket frame ngầm để tiết kiệm tối đa băng thông khi truyền tin nhắn lớn.</p>
  </div>
</div>

```typescript
import { createWebSocketServer } from "vito/packages/websocket/websocket.vit";

// Khởi tạo WebSocket Server tại cổng 8080 (Sức chứa 50,000 conns)
let wsServer = createWebSocketServer(8080, 50000);

// Tiếp nhận kết nối từ Client
let client1 = wsServer.acceptConnection("192.168.1.10");
let client2 = wsServer.acceptConnection("192.168.1.11");

// Gửi tin nhắn Broadcast theo Room
wsServer.broadcastToRoom("room_vip", JSON.stringify({ event: "trade:update", price: 95400 }));
```

---

## 🌊 2. Server-Sent Events (SSE) cho AI Streaming (`packages/sse`)

Chuyên biệt cho các ứng dụng trí tuệ nhân tạo (AI/LLM) cần phát trực tiếp từng token văn bản về Client:

```typescript
import { createSSEResponse } from "vito/packages/sse/sse.vit";

app.get("/api/v1/chat/stream", (req: Request, res: Response) => {
    let sse = createSSEResponse(res);

    // Stream từng token dữ liệu AI
    sse.sendEvent("token", JSON.stringify({ text: "Xin " }));
    sse.sendEvent("token", JSON.stringify({ text: "chào " }));
    sse.sendEvent("token", JSON.stringify({ text: "bạn!" }));
    
    sse.close();
});
```

---

## ⏳ 3. Background Job Queue Engine (`packages/queue`)

Đẩy các tác vụ nặng (Xử lý video, gửi mail hàng loạt, xuất file Excel) xuống hàng chờ công việc nền để xử lý bất đồng bộ:

::: code-group

```typescript [1. Push Job to Queue]
import { createJobQueue, Job } from "vito/packages/queue/queue.vit";

// Khởi tạo Redis Job Queue
let jobQueue = createJobQueue("redis");

// Tạo Job gửi Mail
let emailJob: Job;
emailJob.init("job_101", "email:send", JSON.stringify({ to: "user@vito.dev" }));

// Đẩy Job vào hàng chờ
jobQueue.pushJob(emailJob);
```

```typescript [2. Delayed Job & Dead Letter Queue (DLQ)]
// Lên lịch hoãn chạy Job (Delay 5000ms)
let reportJob: Job;
reportJob.init("job_102", "report:export", JSON.stringify({ format: "pdf" }));
jobQueue.scheduleJob(reportJob, 5000);

// Khi Job xử lý thất bại quá maxRetries, tự động chuyển vào Dead Letter Queue (DLQ)
jobQueue.processNextJob(true, "Lỗi kết nối tới SMTP Server");
```

:::
