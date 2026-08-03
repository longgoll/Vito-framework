# Vito — VIT Native Web Framework

[![TFB](https://img.shields.io/badge/TechEmpower-Round%2023-blue)](https://www.techempower.com/benchmarks/)
[![Language](https://img.shields.io/badge/Language-VIT-orange)](https://github.com/HoangLong-Dev/vit)
[![Approach](https://img.shields.io/badge/Approach-Realistic-green)](https://www.techempower.com/benchmarks/#section=data-r22&test=plaintext)

## Overview

**Vito** is a high-performance web framework built on the **VIT language** — a custom compiled language targeting a native C runtime. The benchmark implementation is a standalone C server using:

- **epoll + SO_REUSEPORT** multi-worker architecture (one worker thread per CPU core)
- **libpq** native PostgreSQL protocol (no ORM, no middleware)
- **SIMD-accelerated JSON** parsing via AVX2 intrinsics
- **Per-connection slab allocator** (C100K pattern — zero malloc per request in hot path)
- **HTTP/1.1 pipelining** support via `memmem(\r\n\r\n)` boundary detection
- **Atomic double-buffered Date header** (updated every second, zero lock contention)
- **io_uring-aware** event loop with `epoll` backend (Linux 5.1+)

## Benchmark Test URLs

| Test | URL |
|------|-----|
| JSON Serialization | `/json` |
| Single DB Query | `/db` |
| Multiple DB Queries | `/queries?queries=N` |
| Fortunes | `/fortunes` |
| DB Updates | `/updates?queries=N` |
| Plaintext | `/plaintext` |

## Performance Characteristics

- **Plaintext**: ~1M+ RPS on 28-core TFB server (estimated)
- **JSON**: ~800K RPS
- **DB**: ~200K RPS (limited by PostgreSQL round-trips)
- Workers: `nproc` (cgroup v2 quota-aware, reads `/sys/fs/cgroup/cpu.max`)
- Connection backlog: 65,535
- File descriptor limit: 1,048,576 (set via `setrlimit` at startup)

## Architecture

```
                     ┌─────────────────────────────────────┐
   TCP :8080  ──────▶│  epoll Worker Pool (N × CPU cores)  │
                     │  SO_REUSEPORT — each worker has own  │
                     │  listen socket + epoll instance      │
                     └─────────────┬───────────────────────┘
                                   │
              ┌────────────────────┼────────────────────────┐
              │                   │                         │
              ▼                   ▼                         ▼
       /plaintext              /json               /db /queries
      text/plain           application/json        libpq → PostgreSQL
      "Hello, World!"      {"message":"..."}       World(id, randomNumber)
```

## Runtime Components

| File | Purpose |
|------|---------|
| `main.c` | HTTP server entry point, route handler, PostgreSQL pool |
| `runtime/async_iouring_rt.c` | epoll worker group (SO_REUSEPORT multi-worker) |
| `runtime/net_rt.c` | Socket primitives (TCP_NODELAY, nonblocking I/O) |
| `runtime/slab_allocator_rt.c` | Thread-safe per-connection memory pool |
| `runtime/simd_json_rt.c` | AVX2 SIMD JSON parser + thread-local arena |
| `runtime/http_parser_simd.c` | HTTP/1.1 request parser |
| `runtime/string_rt.c` | VIT string runtime |
| `runtime/concurrency_rt_posix.c` | POSIX pthread concurrency primitives |

## Database

Uses TFB standard PostgreSQL connection:
- Host: `tfb-database`
- Port: `5432`
- User: `benchmarkdbuser`
- Password: `benchmarkdbpass`
- Database: `hello_world`

Prepared statements are created once per connection at pool initialization.

## Build Requirements

- GCC 12+ with AVX2 support
- `liburing-dev` (2.3+)
- `libpq-dev` (PostgreSQL 15+)
- Debian Bookworm (matching TFB environment)

## Local Testing

```bash
# Create TFB network
docker network create tfb-net

# Start PostgreSQL (TFB seed data)
docker run -d --name tfb-database --network tfb-net \
  -e POSTGRES_USER=benchmarkdbuser \
  -e POSTGRES_PASSWORD=benchmarkdbpass \
  -e POSTGRES_DB=hello_world \
  postgres:15-bookworm

# Seed database
docker exec -i tfb-database psql -U benchmarkdbuser -d hello_world -c "
  CREATE TABLE World (id INT NOT NULL, randomNumber INT NOT NULL);
  INSERT INTO World SELECT id, floor(random()*10000+1)::int FROM generate_series(1,10000) id;
  CREATE TABLE Fortune (id INT NOT NULL, message VARCHAR(2048) NOT NULL);
  INSERT INTO Fortune VALUES
    (1,'fortune: No such file or directory'),
    (2,'A computer scientist is someone who fixes things that aren''t broken.'),
    (3,'After enough decimal places, nobody gives a damn.'),
    (4,'A bad random number generator: 1, 1, 1, 1, 1, 4.33e+67, 1, 1, 1'),
    (5,'A computer program does what you tell it to do, not what you want it to do.'),
    (6,'Emacs is a nice operating system, but I prefer UNIX.'),
    (7,'Any program that runs right is obsolete.'),
    (8,'A list is only as strong as its weakest link. -- Don Knuth'),
    (9,'Feature: A bug with seniority.'),
    (10,'Computers make very fast, very accurate mistakes.'),
    (11,'<script>alert(\"This should not be displayed in a browser alert box.\");</script>'),
    (12,'フレームワークのベンチマーク');"

# Build and run Vito
docker build -t vito-tfb .
docker run -d --name vito --network tfb-net \
  --cap-add SYS_RESOURCE \
  -p 8080:8080 vito-tfb

# Verify routes
curl http://localhost:8080/plaintext      # → Hello, World!
curl http://localhost:8080/json           # → {"message":"Hello, World!"}
curl http://localhost:8080/db             # → {"id":N,"randomNumber":N}
curl "http://localhost:8080/queries?queries=5"
curl "http://localhost:8080/updates?queries=3"
curl http://localhost:8080/fortunes       # → HTML table, sorted, escaped

# Quick load test (requires wrk)
wrk -t4 -c256 -d30s http://localhost:8080/plaintext
```

## Links

- VIT Language: [github.com/HoangLong-Dev/vit](https://github.com/HoangLong-Dev/vit)
- Vito Framework: [github.com/HoangLong-Dev/Vito-framework](https://github.com/HoangLong-Dev/Vito-framework)
