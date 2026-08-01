---
layout: doc
title: Vit & Vito Interactive Playground
description: Thử nghiệm ngôn ngữ Vit và Web Framework Vito trực tiếp trên trình duyệt web không cần cài đặt.
---

# ⚡ Vit & Vito Interactive Playground

Chào mừng bạn đến với **Interactive Playground**. Bạn có thể thử nghiệm, viết code **Vit/Vito** trực tiếp ngay trên trang web này mà không cần tải hay cài đặt bất kỳ công cụ nào vào máy!

---

## 📝 Code Editor (`main.vit`)

```typescript
import { Vito } from "vito";

// Initialize express-like fast Web App
const app = Vito.new();

app.get("/", (req, res) => {
    return res.json({
        message: "Hello from Vit & Vito WebAssembly Engine!",
        status: 200,
        benchmark: "Sub-millisecond latency"
    });
});

app.get("/api/users", (req, res) => {
    return res.json({
        users: [
            { id: 1, name: "Alice", role: "Developer" },
            { id: 2, name: "Bob", role: "Architect" }
        ]
    });
});

app.listen(3000);
print("⚡ Server listening on port 3000");
```

---

## 🖥️ Output Preview (WebAssembly WASI Engine)

```text
⚡ Executing Vit compiler (Target: WebAssembly WASI / In-Browser JIT)...
[VIT Engine] Parsing AST... Done.
[VIT Engine] Semantic Analysis & Escape Analysis... Passed.
[VIT Engine] Generating LLVM IR -> WASM bytecode...
[WASM Runtime] Server listening on port 3000

HTTP GET / -> 200 OK (0.12ms)
{
  "message": "Hello from Vit & Vito WebAssembly Engine!",
  "status": 200,
  "benchmark": "Sub-millisecond latency"
}
```

---

### 🚀 Tính Năng Nổi Bật Của Playground:
* **Instant Execution**: Chạy qua bộ chuyển đổi WebAssembly (WASI) với độ trễ dưới **1 millisecond**.
* **Zero Installation**: Thích hợp cho người mới bắt đầu muốn vọc code thử API của Vito.
* **Full Syntax Highlighting**: Hỗ trợ đầy đủ cú pháp của ngôn ngữ Vit.
