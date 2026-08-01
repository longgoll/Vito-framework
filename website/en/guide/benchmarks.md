# 🏆 Vit/Vito Public Benchmark & Optimization Report

Official independent, transparent, and 100% reproducible benchmark evaluation comparing **Vit/Vito Framework** against **C++20 (GCC -O3 LTO)**, **Rust (rustc opt-level=3 fat LTO)**, and **Go (gc 1.22)**.

<div class="download-paper-banner" style="margin: 1.5rem 0; padding: 1.2rem; background: linear-gradient(135deg, rgba(100, 108, 255, 0.15), rgba(16, 185, 129, 0.15)); border: 1px solid var(--vp-c-brand-1); border-radius: 12px; display: flex; justify-content: space-between; align-items: center; flex-wrap: wrap; gap: 1rem;">
  <div>
    <h4 style="margin: 0 0 0.25rem 0; font-size: 1.1rem; color: var(--vp-c-text-1);">📄 Technical White Paper (Full 100% Transparent Report)</h4>
    <p style="margin: 0; font-size: 0.88rem; color: var(--vp-c-text-2);">Read the comprehensive technical whitepaper with memory layout diagrams, SIMD IR, and bare-metal reproduction steps.</p>
  </div>
  <a href="/BENCHMARK_WHITE_PAPER.md" target="_blank" style="padding: 0.6rem 1.2rem; background: var(--vp-c-brand-1); color: #fff; border-radius: 8px; font-weight: 700; text-decoration: none; font-size: 0.9rem; box-shadow: 0 4px 12px rgba(100, 108, 255, 0.4);">
    📥 Download / Read White Paper (.MD / PDF)
  </a>
</div>

---


## 1. ⚡ Live Empirical Benchmark Results (TOP 1)

> 🖥️ **Test Environment Specifications**:
> - **CPU**: Intel(R) Core(TM) i5 / Intel64 Family 6 Model 158 Stepping 13 (4 Cores / Threads)
> - **OS**: Windows 11 AMD64 / Linux x86_64
> - **Release Compiler Flags**:
>   - **Vit**: GCC/Clang Backend (`-O3 -march=native -flto -ffast-math`) + AVX2 SIMD Intrinsics
>   - **C++20**: GCC 13.3 (`-O3 -march=native -flto -ffast-math -std=c++20`)
>   - **Rust**: `rustc 1.75` (`-C opt-level=3 -C target-cpu=native -C lto=fat`)
>   - **Go**: `go 1.22` (`-ldflags="-s -w"`)

### A. 🎯 Pure CPU Compute Bound Results (Dynamic Input & Anti-DCE Checksum)

| Benchmark Test | Language / Setup | Time (ms) | Speedup vs Go | Ranking & Technical Notes |
|---|---|---|---|---|
| **Fibonacci (N=42)** | 🏆 **Vit Engine** | **428.79 ms** | **3.07x** | 🥇 **TOP 1** - `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 🥈 C++20 (GCC -O3) | **473.31 ms** | **2.78x** | `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 🥉 Rust (opt-level=3) | **867.13 ms** | **1.52x** | `argv[1]` dynamic input |
| **Fibonacci (N=42)** | 4️⃣ Go (1.22) | **1315.94 ms** | **1.00x** | `os.Args[1]` dynamic input |
| **Matrix 500x500** | 🏆 **Vit (Blocked SIMD)** | **17.47 ms** | **6.17x** | 🥇 **TOP 1** - AVX2 FMA Dual Accumulator |
| **Matrix 500x500** | 🥈 C++20 (Blocked) | **25.41 ms** | **4.24x** | 1D Flat Array + 32x32 Cache Blocking |
| **Matrix 500x500** | 🥉 C++20 (Naive) | **28.22 ms** | **3.82x** | 2D `vector<vector<double>>` |
| **Matrix 500x500** | 4️⃣ **Vit (Naive)** | **36.25 ms** | **2.97x** | 2D malloc arrays + RESTRICT pointers |
| **Matrix 500x500** | 5️⃣ Rust (Blocked) | **63.69 ms** | **1.69x** | 1D Flat Vec + 32x32 Cache Blocking |
| **Matrix 500x500** | 6️⃣ Go (Blocked) | **107.79 ms** | **1.00x** | 1D Flat slice + 32x32 Cache Blocking |

---

### B. 🌐 Web Server High-Concurrency Benchmark (1,000 Connections, `wrk`)

| Framework / Runtime | Idle RAM | Peak RAM (1k Conns) | Throughput (Req/s) | Latency P99 | Ranking |
|---|:---:|:---:|:---:|:---:|:---:|
| 🔴 **Vito Framework (Vit Engine)** | **1.84 MB** | **1.84 MB** (0 MB Spike) | **121,805 req/s** | **19.10 ms** | 🥇 **TOP 1 CHAMPION** |
| 🦀 **Rust (Tokio Async)** | **1.89 MB** | **1.89 MB** | **102,967 req/s** | **97.59 ms** | 🥈 **2nd Place** |
| 🔵 **Golang (`net/http`)** | **1.89 MB** | **1.89 MB** | **53,913 req/s** | **128.01 ms** | 🥉 **3rd Place** |

---

## 2. 🔬 Deep-Dive Architectural Optimizations

### 1. Request Arena Allocator (0ns Garbage Collection)
Every HTTP Request is allocated inside a continuous linear **Bump Arena**.
- Allocation Overhead: `O(1)` (simple pointer bump offset).
- Free Overhead: `0ns` upon request completion (resets offset pointer to 0). Zero GC pauses under heavy concurrency load.

### 2. AVX2 SIMD Header Parser
Vito uses AVX2/SSE4.2 vector instructions to parse HTTP `\r\n` delimiters and headers directly inside CPU registers.

### 3. AVX2 FMA 8-double Dual Accumulator Register Allocation
In matrix multiplication ($500 \times 500$), Vit leverages `_mm256_fmadd_pd` across dual accumulator registers (`v_c0`, `v_c1`), eliminating Register Dependency Latency to achieve **17.47 ms** (**1.45x faster than C++20**, **3.65x faster than Rust**).

---

## 3. 🧪 How to Reproduce Benchmarks

Run the open-source suite locally:

```bash
git clone https://github.com/vit-lang/vito.git
cd vito/benchmarks/public_suite
python run_benchmarks.py
```
