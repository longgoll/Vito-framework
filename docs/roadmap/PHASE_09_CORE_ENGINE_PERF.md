# PHASE 9: Core Engine Optimization & High-Performance Memory Engine ⚡

> **Mục tiêu**: Tối ưu hóa hiệu năng cốt lõi của Vito Engine lên mức cực hạn (Peak Throughput & Ultra-low Latency $p99 < 1ms$), quản lý bộ nhớ thông minh (Zero-Allocation Memory Arena) và cải tiến thuật toán Routing.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 🧬 Zero-Allocation Request Memory Pool (`vito/core/pool`)
- [x] **Memory Arena Allocator**: Khởi tạo vùng nhớ tái sử dụng cho từng HTTP Request để tránh cấp phát động (Heap allocation) liên tục.
- [x] **Buffer Recycling Engine**: Tái sử dụng TCP Read/Write Buffer qua đối tượng Pool để giảm tối đa chi phí Garbage Collection (GC) hoặc Deallocation.
- [x] **Header Parsing without Allocation**: Trích xuất HTTP Headers trực tiếp bằng String View / Byte Slice chỉ trỏ đến buffer có sẵn.

### 2. ⚡ Dynamic Radix Trie Router Enhancement (`packages/router`)
- [x] **Zero-Alloc Param Matcher**: Trích xuất các tham số động `:param` và `*wildcard` mà không cần khởi tạo mảng / Map mới trên mỗi request.
- [x] **Static Route Fast Path**: Phân tách luồng xử lý riêng cho đường dẫn tĩnh (Static Routes) với độ phức tạp $O(1)$ Hash Map Lookup.
- [x] **Strict & Lax Trailing Slash Normalization**: Tự động và tối ưu hóa xử lý dấu gạch chéo cuối URL (`/users` vs `/users/`).

### 3. 🚀 High-Speed HTTP/1.1 & HTTP/2 Parser Optimization
- [x] **SIMD-Accelerated Chunk & Boundary Scanner**: Tận dụng các chỉ thị SIMD để quét nhanh các ký tự ngắt dòng `\r\n\r\n` và delimiter.
- [x] **Stream Response Pipeline**: Tối ưu hóa pipeline phản hồi `res.send()` và `res.json()` với zero-copy buffer flushing.

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Độ trễ trung bình khi xử lý Plaintext / JSON Benchmark $< 0.5ms$.
2. Không xảy ra rò rỉ bộ nhớ (Zero Memory Leak) khi stress test 1,000,000 requests.
3. Radix Router đạt tốc độ khớp đường dẫn $> 10,000,000$ ops/sec.
