# Standard Library (STD) & Official Packages 📦

This document provides a **comprehensive guide**, API Function Signatures, primitive types, and practical code examples for all **24 Modules in the VIT Standard Library (`vit/std`)** as well as official **Vito Framework Packages**.

---

## 📚 1. Detailed Guide for 24 VIT Standard Library Modules (`std`)

All modules in `std` are machine-code optimized via **LLVM**, with Automatic Reference Counting (ARC) memory cleanup and C/C++ tier execution performance.

---

### 📁 1.1. File System Module (`std/fs`)

```typescript
import { 
    readFile, writeFile, appendFile, exists, 
    removeFile, fileSize, createDir, readDir 
} from "std/fs";
```

| Function | Return Type | Description |
| :--- | :--- | :--- |
| `readFile(path: string)` | `string` | Read entire text file content as a UTF-8 string. |
| `writeFile(path: string, content: string)` | `boolean` | Write content to file, overwriting existing data or creating a new file. |
| `appendFile(path: string, content: string)`| `boolean` | Append text data to the end of a file. |
| `exists(path: string)` | `boolean` | Check if a file or directory exists. |
| `removeFile(path: string)` | `boolean` | Remove file from disk. |
| `fileSize(path: string)` | `number` | Get file size in bytes. |
| `createDir(path: string)` | `boolean` | Create a new directory. |
| `readDir(path: string)` | `string` | Read directory entries. |

#### Code Example:
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

### 🛣️ 1.2. Path Utilities Module (`std/path`)

```typescript
import { pathJoin, pathBasename, pathExtname, pathDirname } from "std/path";
```

#### API Reference & Code Example:
```typescript
let fullPath = pathJoin("var/www", "index.html"); // "var/www/index.html"
let base = pathBasename("/usr/local/bin/vit.exe"); // "vit.exe"
let ext = pathExtname("document.pdf");              // ".pdf"
let dir = pathDirname("/a/b/c/file.txt");          // "/a/b/c"
```

---

### 🔢 1.3. JSON Module (`std/json`)

```typescript
import { parseJSON, stringifyString, stringifyBoolean } from "std/json";
```

#### API Reference & Code Example:
```typescript
let rawJson: string = '{"name": "Vito", "version": "1.0.0"}';
let data = parseJSON(rawJson);

let safeString: string = stringifyString('Hello "Vit"');
let jsonBool: string = stringifyBoolean(true); // "true"
```

---

### 📦 1.4. Collections & Data Structures (`std/collections`)

Provides high-speed **HashMap** and **Set** implementations.

```typescript
import { HashMap, Set } from "std/collections";
```

#### API Reference & Code Example:
```typescript
// 1. HashMap Usage
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
map.free(); // Free allocated memory

// 2. Set Usage
let set = Set {};
set.init();
set.add("admin");
set.add("editor");
println("Has admin? " + set.has("admin")); // true
set.free();
```

---

### 🗓️ 1.5. Date & Time Module (`std/datetime` & `std/time`)

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

#### API Reference & Code Example:
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

### 📋 1.6. Structured Logging Module (`std/log`)

Leveled logging with ANSI color coding and key-value fields.

```typescript
import { 
    log_set_level, log_trace, log_debug, 
    log_info, log_warn, log_error, log_fatal, 
    log_section, log_field 
} from "std/log";
```

#### API Reference & Code Example:
```typescript
log_set_level(1); // Minimum log level: LOG_DEBUG

log_section("SERVER INITIALIZATION");
log_info("Starting Vito HTTP Server on port 8080...");
log_field("Port", "8080");
log_field("Worker Threads", "8");

log_debug("Database connection pool initialized");
log_warn("High memory usage detected (85%)");
log_error("Failed to connect to Redis queue");
```

---

### 🔒 1.7. Cryptography & Encoding (`std/crypto` & `std/encoding`)

```typescript
import { sha256, md5 } from "std/crypto";
import { base64Encode, base64Decode, urlEncode, urlDecode } from "std/encoding";
```

#### API Reference & Code Example:
```typescript
// Hashing
let hashSha: string = sha256("my_secret_password");
let hashMd5: string = md5("hello_world");

// Base64 Encoding / Decoding
let b64: string = base64Encode("Vit Ecosystem");
let original: string = base64Decode(b64);

// URL Encoding / Decoding
let safeUrl: string = urlEncode("https://vito.dev/search?q=vit language");
```

---

### 🧮 1.8. Math Module (`std/math`)

```typescript
import { abs, sqrt, max, min, pow, floor, ceil } from "std/math";
```

#### API Reference & Code Example:
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

### 🔤 1.9. String Module (`std/string` & `std/str`)

```typescript
import { String, trim, split, join } from "std/string";
```

#### API Reference & Code Example:
```typescript
let raw: string = "   Vit Language Ecosystem   ";
let clean: string = trim(raw); // "Vit Language Ecosystem"
let len: number = clean.length;
let words: string[] = split(clean, " ");
let combined: string = join(words, "-");
```

---

### 🖥️ 1.10. Input / Output Module (`std/io`)

```typescript
import { print, println, readLine } from "std/io";
```

#### API Reference & Code Example:
```typescript
print("Enter your username: ");
let name: string = readLine();
println("Welcome to Vit, " + name + "!");
```

---

### ⚙️ 1.11. Environment & System Module (`std/env` & `std/sys`)

```typescript
import { getEnv, setEnv, osName, cpuCores, totalMemory } from "std/env";
```

#### API Reference & Code Example:
```typescript
let port: string = getEnv("PORT") || "8080";
setEnv("APP_ENV", "production");
println("OS: " + osName());          
println("CPU Cores: " + cpuCores());
```

---

### 🧪 1.12. Automated Testing Module (`std/testing`)

```typescript
import { 
    describe, assertEqual, assertEq, assertNe, 
    assertTrue, assertFalse, assertGt, assertLt, 
    assertApprox, assertNull, assertNotNull, test_summary 
} from "std/testing";
```

#### API Reference & Code Example:
```typescript
function testSuite() {
    describe("Core Logic Suite");

    let val = 50;
    assertEq(val, 50, "Value must be 50");
    assertNe(val, 100, "Value must not be 100");
    assertGt(val, 10, "Value > 10");
    assertLt(val, 100, "Value < 100");
    assertTrue(val > 0, "Value > 0");
    assertFalse(val < 0, "Value must not be negative");
    assertEqual("Vit", "Vit", "Strings match");
}

function main() {
    testSuite();
    test_summary();
}
```

---

### 🧵 1.13. Threads & Concurrency (`std/thread` & `std/async`)

```typescript
import { Thread, Channel } from "std/thread";
```

#### API Reference & Code Example:
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

### 🌐 1.14. TCP Socket Module (`std/net`)

```typescript
import { TcpServer, Socket } from "std/net";
```

#### API Reference & Code Example:
```typescript
let server = TcpServer.bind("127.0.0.1", 9000);
while (true) {
    let client: Socket = server.accept();
    client.write("HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK");
    client.close();
}
```

---

## 🛠️ 2. Core Vito Framework Packages (`vito/packages`)

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

### 🗄️ 2.2. ORM & Database Package `vito/orm`
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
