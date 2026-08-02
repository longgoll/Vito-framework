# Realtime WebSockets & Job Queue ⚡

Bi-directional real-time communication infrastructure (**WebSockets Server Engine**), **SSE AI Streaming**, and background job queue system (**Background Job Queues**) that allows Vito to handle large-scale workloads smoothly.

---

## 🔌 1. Enterprise WebSocket Engine (`packages/websocket`)

Ultra-low latency bidirectional data transfer, supporting 50,000+ simultaneous open connections with RAM cost < 500MB:

<div class="card-grid">
  <div class="feature-mini-card">
    <div class="icon">📡</div>
    <h4>Room & Broadcast Engine</h4>
    <p>Groups connections into separate Rooms / Channels and broadcasts to a specific room or globally.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">💓</div>
    <h4>Heartbeat Ping/Pong</h4>
    <p>Automatically detects and evicts silently dropped or timed-out connections.</p>
  </div>

  <div class="feature-mini-card">
    <div class="icon">📦</div>
    <h4>Per-Message Deflate</h4>
    <p>Transparently compresses WebSocket frames to minimize bandwidth usage for large messages.</p>
  </div>
</div>

```typescript
import { createWebSocketServer } from "vito/packages/websocket/websocket.vit";

// Initialize WebSocket Server on port 8080 (capacity 50,000 conns)
let wsServer = createWebSocketServer(8080, 50000);

// Accept connection from clients
let client1 = wsServer.acceptConnection("192.168.1.10");
let client2 = wsServer.acceptConnection("192.168.1.11");

// Broadcast message to a Room
wsServer.broadcastToRoom("room_vip", JSON.stringify({ event: "trade:update", price: 95400 }));
```

---

## 🌊 2. Server-Sent Events (SSE) for AI Streaming (`packages/sse`)

Specifically designed for AI/LLM applications that need to stream individual text tokens to the client:

```typescript
import { createSSEResponse } from "vito/packages/sse/sse.vit";

app.get("/api/v1/chat/stream", (req: Request, res: Response) => {
    let sse = createSSEResponse(res);

    // Stream individual AI data tokens
    sse.sendEvent("token", JSON.stringify({ text: "Hello " }));
    sse.sendEvent("token", JSON.stringify({ text: "from " }));
    sse.sendEvent("token", JSON.stringify({ text: "Vito!" }));
    
    sse.close();
});
```

---

## ⏳ 3. Background Job Queue Engine (`packages/queue`)

Offload heavy tasks (video processing, bulk emails, Excel exports) to background job queues for asynchronous processing:

::: code-group

```typescript [1. Push Job to Queue]
import { createJobQueue, Job } from "vito/packages/queue/queue.vit";

// Initialize Redis Job Queue
let jobQueue = createJobQueue("redis");

// Create an email sending Job
let emailJob: Job;
emailJob.init("job_101", "email:send", JSON.stringify({ to: "user@vito.dev" }));

// Push job to the queue
jobQueue.pushJob(emailJob);
```

```typescript [2. Delayed Job & Dead Letter Queue (DLQ)]
// Schedule a delayed Job (5000ms delay)
let reportJob: Job;
reportJob.init("job_102", "report:export", JSON.stringify({ format: "pdf" }));
jobQueue.scheduleJob(reportJob, 5000);

// When a Job fails beyond maxRetries, it is automatically moved to the Dead Letter Queue (DLQ)
jobQueue.processNextJob(true, "Failed to connect to SMTP Server");
```

:::
