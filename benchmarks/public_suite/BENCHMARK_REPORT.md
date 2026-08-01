# Vit Public Benchmark Suite - Official Summary Report

**Date:** 2026-08-02 01:49:09
**OS:** Windows 11 (AMD64)
**Processor:** Intel64 Family 6 Model 158 Stepping 13, GenuineIntel

## 1. Pure CPU Compute Bound Results

| Benchmark Test | Language / Setup | Time (ms) | Speedup vs Go |
|---|---|---|---|
| **Fibonacci** | Vit | **428.79 ms** | 3.07x |
| **Fibonacci** | C++20 | **473.31 ms** | 2.78x |
| **Fibonacci** | Rust | **867.13 ms** | 1.52x |
| **Fibonacci** | Go | **1315.94 ms** | 1.00x |
| **Matrix 500x500** | Vit (Blocked) | **17.47 ms** | 6.17x |
| **Matrix 500x500** | C++20 (Blocked) | **25.41 ms** | 4.24x |
| **Matrix 500x500** | C++20 (Naive) | **28.22 ms** | 3.82x |
| **Matrix 500x500** | Vit (Naive) | **36.25 ms** | 2.97x |
| **Matrix 500x500** | Rust (Blocked) | **63.69 ms** | 1.69x |
| **Matrix 500x500** | Go (Blocked) | **107.79 ms** | 1.00x |
| **Matrix 500x500** | Go (Naive) | **112.18 ms** | 0.96x |
| **Matrix 500x500** | Rust (Naive) | **120.36 ms** | 0.90x |