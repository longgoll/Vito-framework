# WebSockets & Background Job Queues ⚡

**Vito Framework** provides a production-grade full-duplex real-time communication engine (**Production-Grade WebSockets Engine**) and a high-throughput background job queue system (**Background Job Queues & Async Event Bus**).

---

## 🔌 Production-Grade WebSocket Engine (`packages/websocket`)

The `packages/websocket/websocket.vit` module provides high-concurrency WebSocket connection handling, channel room broadcasting, connection keep-alive management, and per-message frame compression.

### Key Features:
- **High Concurrency**: Supports over 50,000+ concurrent open connections with low memory footprint ($< 500MB$ RAM).
- **Rooms & Broadcast**: Partition connections into Rooms/Channels, with support for system-wide Broadcast, Room Broadcast, and Unicast messages.
- **Heartbeat Ping/Pong & Stale Eviction**: Automatically issues periodic PING frames and evicts stale or disconnected client connections.
- **Per-Message Deflate Compression**: Compresses frame payloads to minimize network bandwidth consumption.

### Usage Example:

```typescript
import { createWebSocketServer, compressFrame, decompressFrame } from "vito/packages/websocket/websocket.vit";

// 1. Initialize WebSocket Server (Port 8080, Max 50,000 connections)
let wsServer = createWebSocketServer(8080, 50000);

// 2. Accept inbound connections
let client1 = wsServer.acceptConnection("192.168.1.10");
let client2 = wsServer.acceptConnection("192.168.1.11");

// 3. Room Management & Broadcast
let chatRoom: WebSocketRoom;
chatRoom.init("room_vip_lounge");
chatRoom.addClient(client1.id);
chatRoom.addClient(client2.id);

wsServer.broadcastToRoom(chatRoom, "{\"event\":\"chat:msg\",\"text\":\"Welcome to VIP Room!\"}");

// 4. Heartbeat check & stale connection cleanup
wsServer.checkHeartbeats(client1, 1700000010);
```

---

## 📬 Internal Async Event Bus & Redis Pub/Sub (`packages/events`)

Decouple internal business logic without stalling or blocking the main HTTP request life cycle.

### Key Features:
- **Async Event Emitter** (`packages/events/event_bus.vit`): Register and emit non-blocking asynchronous events (`app.on("user:registered", handler)`).
- **Redis Pub/Sub Cluster Adapter** (`packages/events/redis_pubsub.vit`): Expand event broadcasting across multiple server nodes in a distributed cluster.

### Usage Example:

```typescript
import { createEventBus } from "vito/packages/events/event_bus.vit";
import { createRedisPubSubAdapter } from "vito/packages/events/redis_pubsub.vit";

// 1. Register & Dispatch Internal Events
let eventBus = createEventBus();
eventBus.on("user:registered", "SendWelcomeEmailHandler");
eventBus.on("user:registered", "AuditLoggerHandler");

// Emit event asynchronously
eventBus.emitAsync("user:registered", "{\"userId\":\"usr_999\"}");

// 2. Cross-Node Redis Pub/Sub
let redisPubSub = createRedisPubSubAdapter("127.0.0.1", 6379);
redisPubSub.subscribeChannel("vito:cluster:events");
redisPubSub.publishCluster("vito:cluster:events", "{\"event\":\"cache:purge\"}");
```

---

## ⏳ Background Job Queue Engine (`packages/queue`)

The `packages/queue/queue.vit` module offloads heavy tasks (batch emails, image manipulation, report exports) to asynchronous background workers.

### Key Features:
- **Multi-Driver Support**: In-memory `Memory` driver for local single-node apps and `Redis` driver for production cluster environments.
- **Delayed & Scheduled Jobs**: Support executing jobs after a configured time delay (`scheduleJob`).
- **Exponential Backoff Retry**: Automatically retry failed tasks using exponential backoff ($1s, 2s, 4s, ...$).
- **Dead Letter Queue (DLQ)**: Isolate and inspect permanently failed jobs after exceeding `maxRetries`.

### Usage Example:

```typescript
import { createJobQueue, Job } from "vito/packages/queue/queue.vit";

// 1. Initialize Redis Job Queue
let jobQueue = createJobQueue("redis");

// 2. Push job to queue
let emailJob: Job;
emailJob.init("job_email_001", "email:send", "{\"to\":\"dev@vito.dev\"}");
jobQueue.pushJob(emailJob);

// 3. Schedule delayed job (5000ms delay)
let reportJob: Job;
reportJob.init("job_report_002", "report:export", "{\"format\":\"pdf\"}");
jobQueue.scheduleJob(reportJob, 5000);

// 4. Worker execution & DLQ fallback
jobQueue.processNextJob(false, ""); // Success

// Retry failed job -> Moves to DLQ upon exceeding maxRetries
jobQueue.processNextJob(true, "Email Microservice Timeout");
```
