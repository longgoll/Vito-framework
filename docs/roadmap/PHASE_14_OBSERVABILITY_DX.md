# PHASE 14: Observability, Metrics & Developer Experience (DX) 📊

> **Mục tiêu**: Tích hợp khả năng giám sát hệ thống thời gian thực (Prometheus Metrics & Tracing) cùng bộ công cụ phát triển (CLI Generators & Testing Framework) chuẩn hóa.

---

## 🎯 Danh Sách Mục Tiêu Chi Tiết

### 1. 📈 Prometheus Metrics & Health Monitoring (`packages/metrics`)
- [x] **Native `/metrics` Endpoint**: Tự động thu thập và xuất các chỉ số chuẩn Prometheus:
  - `http_requests_total` (by method, status code, path).
  - `http_request_duration_seconds` (Histogram latency quantiles p50, p90, p99).
  - `process_cpu_seconds_total` & `process_resident_memory_bytes`.
- [x] **OpenTelemetry (OTel) Tracing Integration**: Tự động truyền dẫn Trace ID và Span ID qua HTTP Headers (`traceparent`).

### 2. 🧪 Testing Framework & Mock Suite (`vito/testing`)
- [x] **Enhanced `app.inject()`**: Mở rộng khả năng giả lập HTTP request siêu tốc không cần cổng TCP thực tế với đầy đủ Body, Headers, Cookies.
- [x] **DB Test Fixtures & Isolation**: Hỗ trợ tự động Rollback CSDL sau mỗi lượt Test để đảm bảo môi trường Test sạch sẽ.

### 3. 🛠 CLI Code Generators & DX Tooling (`vit` CLI)
- [x] **`vit generate` (alias `vit g`)**: Tự động sinh boilerplate code cho Controller, Service, Middleware, Model, Migration.
- [x] **Interactive CLI Diagnostics**: Lệnh `vit doctor` kiểm tra cấu hình môi trường, phiên bản CSDL và các dependency.

---

## 📊 Tiêu Chí Hoàn Thành (Definition of Done)
1. Tích hợp Grafana Dashboard đọc dữ liệu trực tiếp từ endpoint `/metrics` của Vito.
2. Bộ Test Suite sử dụng `app.inject()` chạy 100 bài test trong $< 1$ giây.
3. Sinh thành công mã nguồn chuẩn với lệnh `vit g controller User`.
