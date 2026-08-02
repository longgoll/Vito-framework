# Thư Viện Chuẩn (STD) & Các Package Chính Thức 📦

Tài liệu này cung cấp **hướng dẫn toàn diện**, danh sách hàm (API Signatures), kiểu dữ liệu và ví dụ mã nguồn thực tế cho toàn bộ **24 Module thuộc Thư Viện Chuẩn VIT (`vit/std`)** cùng các **Package chính thức thuộc Vito Framework**.

---

## 📚 1. Danh Sách Chi Tiết 24 Module Thư Viện Chuẩn VIT (`std`)

Toàn bộ các module trong `std` đều được tối ưu hóa ở mức mã máy (Machine Code) thông qua **LLVM**, hỗ trợ phân bổ bộ nhớ ARC tự động và đạt hiệu năng thực thi tương đương C/C++.

---

### 📁 1.1. Module Hệ Thống Tệp Tin (`std/fs`)

```typescript
import { 
    readFile, writeFile, appendFile, exists, 
    removeFile, fileSize, createDir, readDir 
} from "std/fs";
```

| Tên Hàm | Kiểu Trả Về | Mô Tả |
| :--- | :--- | :--- |
| `readFile(path: string)` | `string` | Đọc toàn bộ nội dung file văn bản dưới dạng UTF-8 string. |
| `writeFile(path: string, content: string)` | `boolean` | Ghi đè nội dung vào file. Tự tạo file mới nếu chưa có. |
| `appendFile(path: string, content: string)`| `boolean` | Ghi nối tiếp dữ liệu vào cuối file. |
| `exists(path: string)` | `boolean` | Kiểm tra sự tồn tại của file hoặc thư mục. |
| `removeFile(path: string)` | `boolean` | Xóa tệp tin khỏi ổ đĩa. |
| `fileSize(path: string)` | `number` | Lấy kích thước tệp tin tính bằng bytes. |
| `createDir(path: string)` | `boolean` | Tạo thư mục mới. |
| `readDir(path: string)` | `string` | Đọc danh sách các file trong thư mục. |

#### Code Mẫu:
```typescript
let isExist: boolean = exists("config.json");
if (isExist) {
    let content: string = readFile("config.json");
    println("File size: " + fileSize("config.json") + " bytes");
} else {
    writeFile("config.json", '{"env": "production"}');
}
```

---

### 🛣️ 1.2. Module Đường Dẫn Tệp (`std/path`)

```typescript
import { pathJoin, pathBasename, pathExtname, pathDirname } from "std/path";
```

#### API Reference & Code Mẫu:
```typescript
let fullPath = pathJoin("var/www", "index.html"); // "var/www/index.html"
let base = pathBasename("/usr/local/bin/vit.exe"); // "vit.exe"
let ext = pathExtname("document.pdf");              // ".pdf"
let dir = pathDirname("/a/b/c/file.txt");          // "/a/b/c"
```

---

### 🔢 1.3. Module JSON (`std/json`)

```typescript
import { parseJSON, stringifyString, stringifyBoolean } from "std/json";
```

#### API Reference & Code Mẫu:
```typescript
let rawJson: string = '{"name": "Vito", "version": "1.0.0"}';
let data = parseJSON(rawJson);

let safeString: string = stringifyString('Hello "Vit"');
let jsonBool: string = stringifyBoolean(true); // "true"
```

---

### 📦 1.4. Module Tập Hợp / Cấu Trúc Dữ Liệu (`std/collections`)

Cung cấp cấu trúc dữ liệu **Bảng Băm (HashMap)** và **Tập Hợp (Set)** siêu tốc.

```typescript
import { HashMap, Set } from "std/collections";
```

#### API Reference & Code Mẫu:
```typescript
// 1. Sử dụng HashMap
let map = HashMap {};
map.init();
map.set("user_1", "Alice");
map.set("user_2", "Bob");

if (map.has("user_1")) {
    let name: string = map.get("user_1"); // "Alice"
    println("User name: " + name);
}
println("Map size: " + map.size());
map.remove("user_2");
map.free(); // Giải phóng bộ nhớ hashmap

// 2. Sử dụng Set
let set = Set {};
set.init();
set.add("admin");
set.add("editor");
println("Has admin? " + set.has("admin")); // true
set.free();
```

---

### 🗓️ 1.5. Module Ngày Tháng & Thời Gian (`std/datetime` & `std/time`)

```typescript
import { 
    datetime_now, 
    datetime_is_leap, 
    datetime_days_in_month, 
    datetime_format_time, 
    datetime_format_duration, 
    datetime_weekday_name, 
    datetime_month_name 
} from "std/datetime";
```

#### API Reference & Code Mẫu:
```typescript
let timestamp: number = datetime_now(); // Unix timestamp
let isLeap: boolean = datetime_is_leap(2028); // true
let days: number = datetime_days_in_month(2, 2028); // 29

let formattedTime: string = datetime_format_time(3665); // "01:01:05"
let durationStr: string = datetime_format_duration(1500); // "1.50s"

let dayName: string = datetime_weekday_name(1); // "Monday"
let monthName: string = datetime_month_name(8); // "August"
```

---

### 📋 1.6. Module Logging Cấu Trúc (`std/log`)

Ghi log phân cấp có màu sắc ANSI ANSI và định dạng cấu trúc.

```typescript
import { 
    log_set_level, log_trace, log_debug, 
    log_info, log_warn, log_error, log_fatal, 
    log_section, log_field 
} from "std/log";
```

#### API Reference & Code Mẫu:
```typescript
log_set_level(1); // Set log level minimum (LOG_DEBUG)

log_section("SERVER INITIALIZATION");
log_info("Starting Vito HTTP Server on port 8080...");
log_field("Port", "8080");
log_field("Worker Threads", "8");

log_debug("Database connection pool initialized");
log_warn("High memory usage detected (85%)");
log_error("Failed to connect to Redis queue");
```

---

### 🔒 1.7. Module Mã Hóa & Crypto (`std/crypto` & `std/encoding`)

```typescript
import { sha256, md5 } from "std/crypto";
import { base64Encode, base64Decode, urlEncode, urlDecode } from "std/encoding";
```

#### API Reference & Code Mẫu:
```typescript
// Hash chuỗi
let hashSha: string = sha256("my_secret_password");
let hashMd5: string = md5("hello_world");

// Base64 Encoding / Decoding
let b64: string = base64Encode("Vit Ecosystem");
let original: string = base64Decode(b64);

// URL Encoding / Decoding
let safeUrl: string = urlEncode("https://vito.dev/search?q=vit language");
```

---

### 🧮 1.8. Module Toán Học (`std/math`)

```typescript
import { abs, sqrt, max, min, pow, floor, ceil } from "std/math";
```

#### API Reference & Code Mẫu:
```typescript
let root: number = sqrt(144.0);     // 12.0
let absolute: number = abs(-42.5);  // 42.5
let maximum: number = max(10, 25);   // 25
let minimum: number = min(10, 25);   // 10
let power: number = pow(2, 10);      // 1024.0
let roundedDown = floor(4.9);       // 4.0
let roundedUp = ceil(4.1);          // 5.0
```

---

### 🔤 1.9. Module Chuỗi (`std/string` & `std/str`)

```typescript
import { String, trim, split, join } from "std/string";
```

#### API Reference & Code Mẫu:
```typescript
let raw: string = "   Vit Language Ecosystem   ";
let clean: string = trim(raw); // "Vit Language Ecosystem"
let len: number = clean.length;
let words: string[] = split(clean, " ");
let combined: string = join(words, "-");
```

---

### 🖥️ 1.10. Module Nhập Xuất Chuẩn (`std/io`)

```typescript
import { print, println, readLine } from "std/io";
```

#### API Reference & Code Mẫu:
```typescript
print("Enter your username: ");
let name: string = readLine();
println("Welcome to Vit, " + name + "!");
```

---

### ⚙️ 1.11. Module Môi Trường & Hệ Thống (`std/env` & `std/sys`)

```typescript
import { getEnv, setEnv, osName, cpuCores, totalMemory } from "std/env";
```

#### API Reference & Code Mẫu:
```typescript
let port: string = getEnv("PORT") || "8080";
setEnv("APP_ENV", "production");
println("OS: " + osName());          
println("CPU Cores: " + cpuCores());
```

---

### 🧪 1.12. Module Kiểm Thử Automated Testing (`std/testing`)

```typescript
import { 
    describe, assertEqual, assertEq, assertNe, 
    assertTrue, assertFalse, assertGt, assertLt, 
    assertApprox, assertNull, assertNotNull, test_summary 
} from "std/testing";
```

#### API Reference & Code Mẫu:
```typescript
function testSuite() {
    describe("Core Logic Suite");

    let val = 50;
    assertEq(val, 50, "Val phai la 50");
    assertNe(val, 100, "Val khac 100");
    assertGt(val, 10, "Val > 10");
    assertLt(val, 100, "Val < 100");
    assertTrue(val > 0, "Val > 0");
    assertFalse(val < 0, "Val không được âm");
    assertEqual("Vit", "Vit", "Chuỗi khớp");
}

function main() {
    testSuite();
    test_summary();
}
```

---

### 🧵 1.13. Module Đa Luồng & Bất Đồng Bộ (`std/thread` & `std/async`)

```typescript
import { Thread, Channel } from "std/thread";
```

#### API Reference & Code Mẫu:
```typescript
function workerTask(ch: Channel<string>) {
    ch.send("Task finished!");
}

function main() {
    let channel = Channel.new<string>();
    let t = Thread.spawn(() => workerTask(channel));
    let result = channel.recv();
    println("Received: " + result);
    t.join();
}
```

---

### 🌐 1.14. Module TCP Socket (`std/net`)

```typescript
import { TcpServer, Socket } from "std/net";
```

#### API Reference & Code Mẫu:
```typescript
let server = TcpServer.bind("127.0.0.1", 9000);
while (true) {
    let client: Socket = server.accept();
    client.write("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK");
    client.close();
}
```

---

## 🛠️ 2. Chi Tiết Các Package Cốt Lõi Của Vito Framework (`vito/packages`)

---

### 🧱 2.1. Router Package `vito/router`
```typescript
import { Vito, Request, Response } from "vito";

let app = Vito.new();
app.get("/users/:id", (req: Request, res: Response) => {
    let userId = req.param("id");
    let search = req.query("search") || "";
    res.json({ id: userId, query: search });
});
```

### 🗄️ 2.2. ORM & CSDL Package `vito/orm`
```typescript
import { ORM, DB } from "vito/orm";

DB.connect({ driver: "postgres", url: "postgres://localhost/db", poolSize: 10 });
let users = ORM.table("users").where("active", "=", true).limit(5).get();
```

### 🔒 2.3. Auth & Hash Package `vito/auth`
```typescript
import { Auth, Hash } from "vito/auth";

let hashed = Hash.make("MyPassword");
let token = Auth.sign({ id: 1 }, "SECRET_KEY", 3600);
let payload = Auth.verify(token, "SECRET_KEY");
```

### 📡 2.4. WebSockets Package `vito/websocket`
```typescript
import { WebSocket } from "vito/websocket";

app.ws("/stream", (ws: WebSocket) => {
    ws.onConnect(() => ws.joinRoom("live"));
    ws.onMessage((msg: string) => ws.broadcastToRoom("live", msg));
});
```

### ⚙️ 2.5. Job Queue Package `vito/queue`
```typescript
import { Queue } from "vito/queue";

Queue.process("send_sms", (data) => {
    println("Sending SMS to " + data.phone);
    return true;
});
Queue.dispatch("send_sms", { phone: "+8499999999" });
```
