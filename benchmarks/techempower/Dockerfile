# TechEmpower Benchmark Round 23 Official Dockerfile for Vito Framework
# Fixes applied:
#   RI-06: -march=x86-64-v3 (not -march=native → prevents SIGILL on TFB server)
#   VRI-05: -std=c11 for _Thread_local / C11 features
#   VRI-07: TFB_COMPILE_SAFE=1 → NativeCompiler.cpp uses x86-64-v3 instead of native
#   VRI-12: concurrency_rt_posix.c compiled for Linux (not Win32 concurrency_rt.c)
#   RI-22: Explicit compile of all vit runtime .c files with correct -I paths
#   RI-10, RI-17: libpq-dev for real PostgreSQL connectivity

FROM debian:bookworm-slim AS build

ENV DEBIAN_FRONTEND=noninteractive
# VRI-07: Set TFB_COMPILE_SAFE to disable -march=native in VIT NativeCompiler
ENV TFB_COMPILE_SAFE=1

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc \
    g++ \
    cmake \
    git \
    liburing-dev \
    libpq-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Expected layout when built via TFB tooling:
#   /app/vito/  → Vito-framework source
#   /app/vit/   → VIT compiler + runtime source
COPY . .

# Compile benchmark server with all vit runtime files:
#   -march=x86-64-v3  = AVX2+FMA, safe for Intel Haswell+ / AMD Ryzen (TFB Dell PowerEdge)
#   -std=c11          = for _Thread_local, C11 atomics (VRI-05)
#   All runtime .c files are compiled in the same gcc invocation (RI-22)
#   libpq             = PostgreSQL client library (RI-10, RI-17)
RUN gcc -O3 -march=x86-64-v3 -flto -ffast-math -DNDEBUG -std=c11 \
    -I/app/vit/src/runtime \
    -I/app/vit/include \
    /app/vito/benchmarks/techempower/main.c \
    /app/vit/src/runtime/async_iouring_rt.c \
    /app/vit/src/runtime/net_rt.c \
    /app/vit/src/runtime/slab_allocator_rt.c \
    /app/vit/src/runtime/simd_json_rt.c \
    /app/vit/src/runtime/http_parser_simd.c \
    /app/vit/src/runtime/string_rt.c \
    /app/vit/src/runtime/memory_rt.c \
    /app/vit/src/runtime/concurrency_rt_posix.c \
    -o /app/vito_techempower_app \
    -luring -lpq -lpthread -lm -s

# Stage 2: Minimal Runtime Container
# VRI-11: debian:bookworm-slim matches TFB runtime environment (not ubuntu:24.04)
FROM debian:bookworm-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    liburing2 \
    libpq5 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/vito_techempower_app /app/vito_techempower_app

EXPOSE 8080

ENTRYPOINT ["/app/vito_techempower_app"]
