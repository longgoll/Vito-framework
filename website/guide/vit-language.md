# Ngôn Ngữ Lập Trình VIT (VIT Language)

**VIT** là một ngôn ngữ lập trình biên dịch (compiled language) hiện đại, hiệu năng cực cao, được thiết kế với cú pháp biểu cảm, hệ thống kiểu tĩnh mạnh mẽ (Strong Static Typing) và cơ chế quản lý bộ nhớ tự động ARC (Automatic Reference Counting).

Vit là nền tảng cốt lõi được sử dụng để xây dựng hệ sinh thái web framework **Vito**.

---

## ⚡ 1. Bắt Đầu Rapid Start

### Cài Đặt Trình Biên Dịch Vit

Để chạy các chương trình mã nguồn Vit (`.vit`), bạn cần trình biên dịch `vit`:

```bash
# Tải và cài đặt tự động trên Windows (PowerShell)
iwr -useb https://vit-lang.org/install.ps1 | iex

# Tải và cài đặt trên Linux/macOS
curl -fsSL https://vit-lang.org/install.sh | sh
```

Kiểm tra phiên bản sau khi cài đặt:
```bash
vit --version
```

---

## 📝 2. Biến & Kiểu Dữ Liệu

Vit hỗ trợ suy luận kiểu tự động (Type Inference) cũng như khai báo kiểu rõ ràng.

### Các Kiểu Dữ Liệu Cơ Bản (Primitives)

```vit
// Định nghĩa biến với let (có thể thay đổi) và const (hằng số)
let age: number = 25;
let pi = 3.14159;            // Tự suy luận kiểu 'number'
let isProduction: boolean = true;
let greeting: string = "Hello Vit Ecosystem!";

// Hằng số
const MAX_CONNECTIONS = 10000;
```

### Mảng Dữ Liệu (Array)

Mảng trong Vit có kích thước linh hoạt và được quản lý bộ nhớ tự động:

```vit
let numbers: number[] = [10, 20, 30, 40];
numbers.push(50);

let first = numbers[0];
let totalLen = numbers.length;
```

---

## 🏗️ 3. Cấu Trúc Dữ Liệu (Struct) & Phương Thức (Methods)

Vit cho phép nhóm dữ liệu bằng `struct` và định nghĩa các phương thức xử lý đi kèm.

```vit
struct Point {
    x: number,
    y: number,

    // Phương thức thuộc về Struct
    function distance(): number {
        return sqrt(this.x * this.x + this.y * this.y);
    }
}

// Khởi tạo Struct
let p = Point { x: 3.0, y: 4.0 };
let dist = p.distance(); // Kết quả: 5.0
```

---

## 🔀 4. Enums, Tagged Unions & Pattern Matching (`match`)

Vit hỗ trợ mạnh mẽ lập trình an toàn với kiểu `enum` và khớp mẫu `match`.

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

// Khớp mẫu biểu thức match
function checkStatus(s: Status): string {
    return match (s) {
        Status.Pending => "Đang chờ xử lý",
        Status.Active => "Đang hoạt động",
        Status.Completed => "Đã hoàn thành"
    };
}
```

---

## 🎯 5. Functional Programming & Arrow Functions (Lambdas)

Vit xem Hàm là giá trị loại một (First-Class Citizens). Bạn có thể dễ dàng sử dụng Lambda functions và các phương thức xử lý mảng cao cấp.

```vit
// Arrow Function
let double = (x: number): number => x * 2.0;

let values: number[] = [1, 2, 3, 4, 5];

// Higher-order methods: map, filter, forEach
let doubledValues = values.map((n: number) => n * 2);
let evenValues = values.filter((n: number) => n % 2 == 0);
```

---

## 🚀 6. Lập Trình Bất Đồng Bộ (`async` / `await`)

Vit tích hợp cơ chế Concurrency siêu nhẹ dựa trên Event Loop và Thread Pool hiệu năng cao.

```vit
async function fetchData(url: string): string {
    // Thực thi HTTP request bất đồng bộ
    let response = await http.get(url);
    return response.body;
}

async function main() {
    let data = await fetchData("https://api.vito.dev/status");
    println("Response data: " + data);
}
```

---

## 🛡️ 7. Quản Lý Bộ Nhớ Tự Động (ARC)

Vit **không sử dụng Garbage Collector (GC)** gây dừng chương trình (Stop-the-World pauses). 

Thay vào đó, trình biên dịch Vit phân tích luồng dữ liệu và chèn các chỉ thị **Automatic Reference Counting (ARC)** combined với **LLVM Escape Analysis Pass**. Nhờ đó:
- Bộ nhớ Heap được giải phóng ngay lập tức khi biến vượt ra ngoài phạm vi (Out of Scope).
- Tối ưu hóa phân bổ trên Stack (`alloca`) thay vì Heap cho các đối tượng cục bộ.
- Đạt tốc độ thực thi tương đương C/C++ và Rust.

---

## 🔗 8. Giao Tiếp C FFI (Foreign Function Interface)

Vit có thể trực tiếp gọi các thư viện C gốc mà không phát sinh chi phí trung gian.

```vit
// Khai báo hàm C bên ngoài
extern function sqrt(x: number): number;
extern function clock(): number;

let result = sqrt(16.0); // Gọi trực tiếp hàm C native
```
