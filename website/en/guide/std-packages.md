# Standard Library & Official Packages (std & Packages) 📦

**Vito Framework** comes with a powerful **Standard Library (`std`)** built specifically for the **VIT Native Compiler Engine**, along with **27 Official Packages** for building scalable enterprise web applications.

---

## 📚 1. Standard Library Modules (`vito/std`)

The standard library follows a zero-dependency philosophy, providing maximum execution performance with SIMD CPU acceleration and minimal RAM overhead.

| Standard Module | Import Path | Primary Functionality | Usage Example |
| :--- | :--- | :--- | :--- |
| **IO / Terminal** | `import { print, println } from "std/io"` | Standard input/output, formatted terminal printing. | `print("Hello Vito!");` |
| **Math Engine** | `import { abs, sqrt, max } from "std/math"` | SIMD-optimized mathematical functions. | `let res = sqrt(16.0);` |
| **String Utilities**|`import { String } from "std/string"` | UTF-8 string manipulation, split, join, find, replace. | `let s = str.trim(" hello ");` |
| **Collections** | `import { Map, Vector } from "std/collections"`| Dynamic arrays (Vector), HashMaps, Lock-free queues. | `let map = Map.new();` |
| **File System (FS)**|`import { File, readText } from "std/fs"` | File I/O, streaming buffers, directory traversal. | `let text = readText("config.json");` |
| **Environment** | `import { getEnv, osName } from "std/env"` | Environment variables (.env), OS info, CPU specs. | `let port = getEnv("PORT");` |
| **Async Runtime** | `import { async, await } from "std/async"` | Coroutines, Event Loop, Futures & Async Task Spawner. | `async function fetchData() { ... }` |
| **Thread Engine** | `import { Thread, Channel } from "std/thread"`| Native OS threads, MPMC cross-thread channels. | `let t = Thread.spawn(worker);` |
| **Socket & Network**|`import { TcpServer, Socket } from "std/net"` | Non-blocking TCP/UDP sockets, IP address parser. | `let server = TcpServer.bind(8080);` |
| **HTTP Parser** | `import { HttpRequest, HttpResponse }` | Zero-copy HTTP/1.1 protocol packet parser. | `let req = HttpRequest.parse(buf);` |
| **JSON Serialization**|`import { JSON } from "std/json"` | Ultra-fast AVX2 SIMD JSON parser (3.85 GB/s throughput). | `let obj = JSON.parse(str);` |

---

## 🛠️ 2. Official Packages Ecosystem (`vito/packages`)

Vito provides 27 official packages out of the box so you never need unverified third-party libraries:

- **Core HTTP & Routing**: `vito/router`, `vito/http_parser`, `vito/pool`
- **Next-Gen Protocols**: `vito/http2`, `vito/http3` (QUIC), `vito/tls` (HTTPS Native)
- **Realtime & AI Streaming**: `vito/websocket`, `vito/sse` (Server-Sent Events for LLM Tokens), `vito/events`
- **Data & ORM**: `vito/orm`, `vito/db` (Postgres, SQLite, MySQL drivers)
- **Auth & Security**: `vito/auth` (JWT/Session), `vito/security` (CSRF, Rate Limiting, Helmet), `vito/cors`
- **Background & Queue**: `vito/queue` (Async Redis Queue), `vito/logger`, `vito/upload`
- **Monitoring & OpenAPI**: `vito/metrics` (Prometheus), `vito/health`, `vito/swagger` (OpenAPI Generator)
- **Dev & Cloud Edge**: `vito/edge` (WASM / Cloudflare Workers), `vito/testing`, `vito/validation`
