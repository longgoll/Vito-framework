# Vito Framework - TechEmpower Round 23 Benchmark Suite 🏆

Thư mục này chứa toàn bộ cấu hình và mã nguồn để nộp và kiểm tra **Vito Framework** chính thức trên hệ thống xếp hạng **TechEmpower Web Framework Benchmarks (Round 23)**.

## 📁 Cấu Trúc File
- `benchmark_config.json`: Cấu hình khai báo 6 test types theo chuẩn TechEmpower v2.
- `Dockerfile`: Multi-stage Dockerfile tối ưu hoá với GCC `-O3 -march=native`, Linux `io_uring SQPOLL` và Fixed Memory Slab Allocator.
- `main.vit`: Mã nguồn Vito xử lý 6 thử nghiệm:
  1. `/json`: JSON Serialization
  2. `/db`: Single Query PostgreSQL ORM
  3. `/queries`: Multiple Queries
  4. `/fortunes`: HTML Server-Side Template Rendering
  5. `/updates`: Database Updates
  6. `/plaintext`: Zero-Allocation Raw HTTP Plaintext

## 🚀 Chạy Thử Nghiệm Với Docker
```bash
docker build -t vito-techempower -f Dockerfile ../..
docker run --rm -p 8080:8080 --cpuset-cpus="0-3" vito-techempower
```

## 📊 Chạy Stress Test Với wrk / tfb
```bash
# Plaintext test
wrk -t4 -c100 -d10s http://localhost:8080/plaintext

# JSON test
wrk -t4 -c100 -d10s http://localhost:8080/json
```
