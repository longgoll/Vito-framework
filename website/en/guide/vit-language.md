# The VIT Programming Language

**VIT** is a modern, high-performance compiled programming language designed with expressive syntax, strong static typing, and automatic memory management (ARC).

Vit powers the entire core ecosystem of the **Vito** web framework.

---

## ⚡ 1. Quick Start

### Installing the Vit Compiler

To compile and execute Vit source files (`.vit`), install the `vit` compiler CLI:

```bash
# Windows (PowerShell)
iwr -useb https://vit-lang.org/install.ps1 | iex

# Linux / macOS
curl -fsSL https://vit-lang.org/install.sh | sh
```

Verify your installation:
```bash
vit --version
```

---

## 📝 2. Variables & Primitive Types

Vit supports both explicit type annotations and type inference.

### Primitive Types

```vit
// Mutable variables with let, constants with const
let age: number = 25;
let pi = 3.14159;            // Inferred as 'number'
let isProduction: boolean = true;
let greeting: string = "Hello Vit Ecosystem!";

// Constants
const MAX_CONNECTIONS = 10000;
```

### Arrays

Arrays in Vit are dynamically resized and automatically managed:

```vit
let numbers: number[] = [10, 20, 30, 40];
numbers.push(50);

let first = numbers[0];
let totalLen = numbers.length;
```

---

## 🏗️ 3. Structs & Methods

Vit allows grouping data into structs and attaching methods.

```vit
struct Point {
    x: number,
    y: number,

    // Struct Method
    function distance(): number {
        return sqrt(this.x * this.x + this.y * this.y);
    }
}

// Instantiation
let p = Point { x: 3.0, y: 4.0 };
let dist = p.distance(); // Result: 5.0
```

---

## 🔀 4. Enums, Tagged Unions & Pattern Matching (`match`)

Vit provides expressive pattern matching with `match` and tagged unions.

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

// Pattern matching expression
function checkStatus(s: Status): string {
    return match (s) {
        Status.Pending => "Pending execution",
        Status.Active => "Currently active",
        Status.Completed => "Completed successfully"
    };
}
```

---

## 🎯 5. Functional Programming & Arrow Functions (Lambdas)

Functions in Vit are First-Class Citizens.

```vit
// Arrow Function
let double = (x: number): number => x * 2.0;

let values: number[] = [1, 2, 3, 4, 5];

// Higher-order array methods: map, filter, forEach
let doubledValues = values.map((n: number) => n * 2);
let evenValues = values.filter((n: number) => n % 2 == 0);
```

---

## 🚀 6. Asynchronous Programming (`async` / `await`)

Vit features lightweight concurrency backed by an event loop and worker thread pool.

```vit
async function fetchData(url: string): string {
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

Vit operates **without a Garbage Collector (GC)**, avoiding Stop-the-World pauses.

The Vit compiler automatically inserts **Automatic Reference Counting (ARC)** instructions alongside an **LLVM Escape Analysis Pass**.
- Heap objects are freed immediately when variables fall out of scope.
- Stack allocations are prioritized over Heap allocations.
- Delivers native execution speed comparable to C/C++ and Rust.

---

## 🔗 8. C FFI (Foreign Function Interface)

Vit natively calls C libraries without overhead:

```vit
// External C function declaration
extern function sqrt(x: number): number;
extern function clock(): number;

let result = sqrt(16.0); // Direct C call
```
