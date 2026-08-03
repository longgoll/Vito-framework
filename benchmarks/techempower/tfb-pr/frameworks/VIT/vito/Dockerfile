# TechEmpower Framework Benchmarks — Vito Framework
# Language: VIT (custom compiled language, C runtime)
# Approach: Realistic | ORM: Raw | Database: Postgres
#
# Fixes applied (from risk analysis):
#   RI-01:  bind 0.0.0.0 (not 127.0.0.1)
#   RI-03:  HTTP keep-alive (no stream.close() per response)
#   RI-05:  Server: Vito + Date: RFC1123 headers
#   RI-06:  -march=x86-64-v3 (not -march=native → safe on TFB Dell PowerEdge)
#   RI-07:  cgroup v2 quota-aware CPU count
#   RI-08:  HTTP pipelining via memmem(\r\n\r\n)
#   RI-09:  /queries + /updates fully implemented
#   RI-10:  Real PostgreSQL via libpq (not fake rand())
#   RI-12:  setrlimit(RLIMIT_NOFILE, 1M)
#   RI-13:  setrlimit(RLIMIT_MEMLOCK, INFINITY)
#   RI-14:  65536-byte response buffer (was 4096)
#   RI-15:  Date header double-buffer + mutex (no race condition)
#   RI-17:  TFB standard Postgres connection string (tfb-database host)
#   RI-18:  SIGTERM/SIGINT graceful shutdown handler
#   RI-19:  rand_r(&seed) per-connection (thread-safe)
#   RI-20:  TCP_NODELAY set in net_rt.c
#   RI-21:  SO_REUSEADDR + SO_REUSEPORT
#   RI-22:  All runtime .c files compiled explicitly
#   VRI-03: pthread_join() in group_stop (no UAF on shutdown)
#   VRI-04: Slab allocator thread-safe with mutex
#   VRI-05: -std=c11 for _Thread_local / C11 atomics
#   VRI-08: listenTcp() backlog = 65535
#   VRI-10: Thread-local arena for JSON parse (no 3x malloc/req)
#   VRI-12: concurrency_rt_posix.c (Linux pthread, not Win32)

FROM debian:bookworm-slim AS build

ENV DEBIAN_FRONTEND=noninteractive
ENV TFB_COMPILE_SAFE=1

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc \
    liburing-dev \
    libpq-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# All source files are self-contained within this directory:
#   main.c           — TFB benchmark HTTP server (entry point)
#   runtime/         — VIT C runtime (epoll, net, slab, SIMD JSON, etc.)
#   include/         — VIT runtime headers
COPY main.c .
COPY runtime/ runtime/
COPY include/ include/

# Build flags:
#   -march=x86-64-v3  = AVX2+FMA — safe for Intel Haswell+ / AMD Ryzen 3+
#                       (TFB uses Dell PowerEdge with Haswell/Broadwell/Skylake)
#   -std=c11          = C11 for _Thread_local, atomics
#   -flto             = Link-time optimization
#   -ffast-math       = Aggressive float optimizations (safe for this workload)
#   -DNDEBUG          = Disable assertions
#   -s                = Strip debug symbols (smaller binary)
RUN gcc -O3 -march=x86-64-v3 -flto -ffast-math -DNDEBUG -std=c11 \
    -Iruntime \
    -Iinclude \
    -I/usr/include/postgresql \
    main.c \
    runtime/async_iouring_rt.c \
    runtime/net_rt.c \
    runtime/slab_allocator_rt.c \
    runtime/simd_json_rt.c \
    runtime/http_parser_simd.c \
    runtime/string_rt.c \
    runtime/memory_rt.c \
    runtime/concurrency_rt_posix.c \
    -o vito_server \
    -luring -lpq -lpthread -lm -s

# Stage 2: Minimal runtime image (~50MB)
FROM debian:bookworm-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    liburing2 \
    libpq5 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/vito_server /app/vito_server

EXPOSE 8080

ENTRYPOINT ["/app/vito_server"]
