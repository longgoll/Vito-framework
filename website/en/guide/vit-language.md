# VIT Programming Language (VIT Language)

**VIT** is a modern compiled language with extremely high performance, designed with an expressive syntax, a powerful Strong Static Typing system, and automatic memory management via ARC (Automatic Reference Counting).

Vit is the core foundation used to build the **Vito** web framework ecosystem.

---

## ⚡ 1. Rapid Start

### Installing the Vit Compiler

To run Vit source programs (`.vit`), you need the `vit` compiler:

```bash
# Download and install automatically on Windows (PowerShell)
iwr -useb https://vit-lang.org/install.ps1 | iex

# Download and install on Linux/macOS
curl -fsSL https://vit-lang.org/install.sh | sh
```

Verify version after installation:
```bash
vit --version
```

---

## 📝 2. Variables & Data Types

Vit supports automatic Type Inference as well as explicit type declarations.

### Primitive Data Types

```vit
// Define variables with let (mutable) and const (constant)
let age: number = 25;
let pi = 3.14159;            // Auto-inferred type 'number'
let isProduction: boolean = true;
let greeting: string = "Hello Vit Ecosystem!";

// Constants
const MAX_CONNECTIONS = 10000;
```

### Arrays

Arrays in Vit have flexible size and are automatically memory-managed:

```vit
let numbers: number[] = [10, 20, 30, 40];
numbers.push(50);

let first = numbers[0];
let totalLen = numbers.length;
```

---

## 🏗️ 3. Structs & Methods

Vit allows grouping data using `struct` and defining associated methods.

```vit
struct Point {
    x: number,
    y: number,

    // Method belonging to the Struct
    function distance(): number {
        return sqrt(this.x * this.x + this.y * this.y);
    }
}

// Instantiate the Struct
let p = Point { x: 3.0, y: 4.0 };
let dist = p.distance(); // Result: 5.0
```

---

## 🔀 4. Enums, Tagged Unions & Pattern Matching (`match`)

Vit strongly supports safe programming with `enum` types and `match` pattern matching.

```vit
enum Status {
    Pending,
    Active,
    Completed
}

enum Option<T> {
    Some(val: T),
    None
}

// Match expression pattern matching
function checkStatus(s: Status): string {
    return match (s) {
        Status.Pending => "Awaiting processing",
        Status.Active => "Currently active",
        Status.Completed => "Completed"
    };
}
```

---

## 🎯 5. Functional Programming & Arrow Functions (Lambdas)

Vit treats Functions as First-Class Citizens. You can easily use Lambda functions and advanced array processing methods.

```vit
// Arrow Function
let double = (x: number): number => x * 2.0;

let values: number[] = [1, 2, 3, 4, 5];

// Higher-order methods: map, filter, forEach
let doubledValues = values.map((n: number) => n * 2);
let evenValues = values.filter((n: number) => n % 2 == 0);
```

---

## 🚀 6. Async Programming (`async` / `await`)

Vit integrates an ultra-lightweight Concurrency mechanism based on a high-performance Event Loop and Thread Pool.

```vit
async function fetchData(url: string): string {
    // Execute async HTTP request
    let response = await http.get(url);
    return response.body;
}

async function main() {
    let data = await fetchData("https://api.vito.dev/status");
    println("Response data: " + data);
}
```

---

## 🛡️ 7. Automatic Memory Management (ARC)

Vit **does not use a Garbage Collector (GC)** that causes Stop-the-World pauses.

Instead, the Vit compiler analyzes data flow and inserts **Automatic Reference Counting (ARC)** instructions combined with an **LLVM Escape Analysis Pass**. As a result:
- Heap memory is freed immediately when a variable goes out of scope.
- Stack allocation (`alloca`) is optimized over Heap for local objects.
- Achieves execution speed comparable to C/C++ and Rust.

---

## 🔗 8. C FFI (Foreign Function Interface)

Vit can directly call native C libraries with zero intermediate overhead.

```vit
// Declare external C functions
extern function sqrt(x: number): number;
extern function clock(): number;

let result = sqrt(16.0); // Calls native C function directly
```
