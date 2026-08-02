# Vit Public Benchmark Suite - Official Summary Report

**Date:** 2026-08-02 13:37:00
**OS:** Windows 11 (AMD64)
**Processor:** Intel64 Family 6 Model 158 Stepping 13, GenuineIntel

## 1. Pure CPU Compute Bound Results

| Benchmark Test | Language / Setup | Time (ms) | Speedup vs Go |
|---|---|---|---|
| **Fibonacci** | Vit | **430.90 ms** | 3.67x |
| **Fibonacci** | C++20 | **451.82 ms** | 3.50x |
| **Fibonacci** | Rust | **838.56 ms** | 1.88x |
| **Fibonacci** | Go | **1579.91 ms** | 1.00x |
| **Matrix 500x500** | Vit (Blocked) | **16.56 ms** | 6.50x |
| **Matrix 500x500** | C++20 (Blocked) | **20.26 ms** | 5.31x |
| **Matrix 500x500** | C++20 (Naive) | **53.16 ms** | 2.02x |
| **Matrix 500x500** | Rust (Blocked) | **63.92 ms** | 1.68x |
| **Matrix 500x500** | Vit (Naive) | **66.82 ms** | 1.61x |
| **Matrix 500x500** | Go (Blocked) | **107.59 ms** | 1.00x |
| **Matrix 500x500** | Go (Naive) | **118.01 ms** | 0.91x |
| **Matrix 500x500** | Rust (Naive) | **121.72 ms** | 0.88x |