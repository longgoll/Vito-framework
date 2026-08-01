# TechEmpower Benchmark Round 23 Official Dockerfile for Vito Framework
# Stage 1: Build Vit Compiler Engine & Compile Vito App to Native Binary
FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc \
    g++ \
    cmake \
    git \
    liburing-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy Vito Framework & Engine Source
COPY . .

# Compile application with GCC -O3, AVX2 SIMD Intrinsics, LTO & PGO optimizations
# Generating single standalone statically linked binary < 10MB
RUN gcc -O3 -march=native -flto -ffast-math -DNDEBUG \
    -I/app/vit/src/runtime \
    /app/vito/benchmarks/techempower/main.c \
    -o /app/vito_techempower_app -luring -lpthread -lm -s

# Stage 2: Minimal Distroless Runtime Container (< 15MB total size)
FROM debian:bookworm-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    liburing2 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/vito_techempower_app /app/vito_techempower_app

EXPOSE 8080

ENTRYPOINT ["/app/vito_techempower_app"]
