# Vit Public Benchmark Suite - Official Summary Report

**Date:** 2026-08-02 02:28:01
**OS:** Windows 11 (AMD64)
**Processor:** Intel64 Family 6 Model 158 Stepping 13, GenuineIntel

## 1. Pure CPU Compute Bound Results

| Benchmark Test | Language / Setup | Time (ms) | Speedup vs Go |
|---|---|---|---|
| **Fibonacci** | Vit | **454.83 ms** | 4.61x |
| **Fibonacci** | C++20 | **486.50 ms** | 4.31x |
| **Fibonacci** | Rust | **691.18 ms** | 3.03x |
| **Fibonacci** | Go | **2094.82 ms** | 1.00x |
| **Matrix 500x500** | C++20 (Blocked) | **23.58 ms** | 4.39x |
| **Matrix 500x500** | Vit (Blocked) | **24.61 ms** | 4.21x |
| **Matrix 500x500** | C++20 (Naive) | **38.45 ms** | 2.69x |
| **Matrix 500x500** | Vit (Naive) | **44.55 ms** | 2.33x |
| **Matrix 500x500** | Rust (Blocked) | **63.63 ms** | 1.63x |
| **Matrix 500x500** | Go (Blocked) | **103.60 ms** | 1.00x |
| **Matrix 500x500** | Rust (Naive) | **125.77 ms** | 0.82x |
| **Matrix 500x500** | Go (Naive) | **179.87 ms** | 0.58x |