import{_ as a,o as s,c as i,a2 as t}from"./chunks/framework.CAlA1Nx7.js";const g=JSON.parse('{"title":"Ngôn Ngữ Lập Trình VIT (VIT Language)","description":"","frontmatter":{},"headers":[],"relativePath":"guide/vit-language.md","filePath":"guide/vit-language.md"}'),e={name:"guide/vit-language.md"};function p(l,n,c,h,o,r){return s(),i("div",null,[...n[0]||(n[0]=[t(`<h1 id="ngon-ngu-lap-trinh-vit-vit-language" tabindex="-1">Ngôn Ngữ Lập Trình VIT (VIT Language) <a class="header-anchor" href="#ngon-ngu-lap-trinh-vit-vit-language" aria-label="Permalink to &quot;Ngôn Ngữ Lập Trình VIT (VIT Language)&quot;">​</a></h1><p><strong>VIT</strong> là một ngôn ngữ lập trình biên dịch (compiled language) hiện đại, hiệu năng cực cao, được thiết kế với cú pháp biểu cảm, hệ thống kiểu tĩnh mạnh mẽ (Strong Static Typing) và cơ chế quản lý bộ nhớ tự động ARC (Automatic Reference Counting).</p><p>Vit là nền tảng cốt lõi được sử dụng để xây dựng hệ sinh thái web framework <strong>Vito</strong>.</p><hr><h2 id="⚡-1-bat-đau-rapid-start" tabindex="-1">⚡ 1. Bắt Đầu Rapid Start <a class="header-anchor" href="#⚡-1-bat-đau-rapid-start" aria-label="Permalink to &quot;⚡ 1. Bắt Đầu Rapid Start&quot;">​</a></h2><h3 id="cai-đat-trinh-bien-dich-vit" tabindex="-1">Cài Đặt Trình Biên Dịch Vit <a class="header-anchor" href="#cai-đat-trinh-bien-dich-vit" aria-label="Permalink to &quot;Cài Đặt Trình Biên Dịch Vit&quot;">​</a></h3><p>Để chạy các chương trình mã nguồn Vit (<code>.vit</code>), bạn cần trình biên dịch <code>vit</code>:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Tải và cài đặt tự động trên Windows (PowerShell)</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">iwr</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -useb</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.ps1</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> iex</span></span>
<span class="line"></span>
<span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Tải và cài đặt trên Linux/macOS</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">curl</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -fsSL</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.sh</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> sh</span></span></code></pre></div><p>Kiểm tra phiên bản sau khi cài đặt:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">vit</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> --version</span></span></code></pre></div><hr><h2 id="📝-2-bien-kieu-du-lieu" tabindex="-1">📝 2. Biến &amp; Kiểu Dữ Liệu <a class="header-anchor" href="#📝-2-bien-kieu-du-lieu" aria-label="Permalink to &quot;📝 2. Biến &amp; Kiểu Dữ Liệu&quot;">​</a></h2><p>Vit hỗ trợ suy luận kiểu tự động (Type Inference) cũng như khai báo kiểu rõ ràng.</p><h3 id="cac-kieu-du-lieu-co-ban-primitives" tabindex="-1">Các Kiểu Dữ Liệu Cơ Bản (Primitives) <a class="header-anchor" href="#cac-kieu-du-lieu-co-ban-primitives" aria-label="Permalink to &quot;Các Kiểu Dữ Liệu Cơ Bản (Primitives)&quot;">​</a></h3><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Định nghĩa biến với let (có thể thay đổi) và const (hằng số)</span></span>
<span class="line"><span>let age: number = 25;</span></span>
<span class="line"><span>let pi = 3.14159;            // Tự suy luận kiểu &#39;number&#39;</span></span>
<span class="line"><span>let isProduction: boolean = true;</span></span>
<span class="line"><span>let greeting: string = &quot;Hello Vit Ecosystem!&quot;;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Hằng số</span></span>
<span class="line"><span>const MAX_CONNECTIONS = 10000;</span></span></code></pre></div><h3 id="mang-du-lieu-array" tabindex="-1">Mảng Dữ Liệu (Array) <a class="header-anchor" href="#mang-du-lieu-array" aria-label="Permalink to &quot;Mảng Dữ Liệu (Array)&quot;">​</a></h3><p>Mảng trong Vit có kích thước linh hoạt và được quản lý bộ nhớ tự động:</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>let numbers: number[] = [10, 20, 30, 40];</span></span>
<span class="line"><span>numbers.push(50);</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let first = numbers[0];</span></span>
<span class="line"><span>let totalLen = numbers.length;</span></span></code></pre></div><hr><h2 id="🏗️-3-cau-truc-du-lieu-struct-phuong-thuc-methods" tabindex="-1">🏗️ 3. Cấu Trúc Dữ Liệu (Struct) &amp; Phương Thức (Methods) <a class="header-anchor" href="#🏗️-3-cau-truc-du-lieu-struct-phuong-thuc-methods" aria-label="Permalink to &quot;🏗️ 3. Cấu Trúc Dữ Liệu (Struct) &amp; Phương Thức (Methods)&quot;">​</a></h2><p>Vit cho phép nhóm dữ liệu bằng <code>struct</code> và định nghĩa các phương thức xử lý đi kèm.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>struct Point {</span></span>
<span class="line"><span>    x: number,</span></span>
<span class="line"><span>    y: number,</span></span>
<span class="line"><span></span></span>
<span class="line"><span>    // Phương thức thuộc về Struct</span></span>
<span class="line"><span>    function distance(): number {</span></span>
<span class="line"><span>        return sqrt(this.x * this.x + this.y * this.y);</span></span>
<span class="line"><span>    }</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Khởi tạo Struct</span></span>
<span class="line"><span>let p = Point { x: 3.0, y: 4.0 };</span></span>
<span class="line"><span>let dist = p.distance(); // Kết quả: 5.0</span></span></code></pre></div><hr><h2 id="🔀-4-enums-tagged-unions-pattern-matching-match" tabindex="-1">🔀 4. Enums, Tagged Unions &amp; Pattern Matching (<code>match</code>) <a class="header-anchor" href="#🔀-4-enums-tagged-unions-pattern-matching-match" aria-label="Permalink to &quot;🔀 4. Enums, Tagged Unions &amp; Pattern Matching (\`match\`)&quot;">​</a></h2><p>Vit hỗ trợ mạnh mẽ lập trình an toàn với kiểu <code>enum</code> và khớp mẫu <code>match</code>.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>enum Status {</span></span>
<span class="line"><span>    Pending,</span></span>
<span class="line"><span>    Active,</span></span>
<span class="line"><span>    Completed</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>enum Option&lt;T&gt; {</span></span>
<span class="line"><span>    Some(val: T),</span></span>
<span class="line"><span>    None</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Khớp mẫu biểu thức match</span></span>
<span class="line"><span>function checkStatus(s: Status): string {</span></span>
<span class="line"><span>    return match (s) {</span></span>
<span class="line"><span>        Status.Pending =&gt; &quot;Đang chờ xử lý&quot;,</span></span>
<span class="line"><span>        Status.Active =&gt; &quot;Đang hoạt động&quot;,</span></span>
<span class="line"><span>        Status.Completed =&gt; &quot;Đã hoàn thành&quot;</span></span>
<span class="line"><span>    };</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🎯-5-functional-programming-arrow-functions-lambdas" tabindex="-1">🎯 5. Functional Programming &amp; Arrow Functions (Lambdas) <a class="header-anchor" href="#🎯-5-functional-programming-arrow-functions-lambdas" aria-label="Permalink to &quot;🎯 5. Functional Programming &amp; Arrow Functions (Lambdas)&quot;">​</a></h2><p>Vit xem Hàm là giá trị loại một (First-Class Citizens). Bạn có thể dễ dàng sử dụng Lambda functions và các phương thức xử lý mảng cao cấp.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Arrow Function</span></span>
<span class="line"><span>let double = (x: number): number =&gt; x * 2.0;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let values: number[] = [1, 2, 3, 4, 5];</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Higher-order methods: map, filter, forEach</span></span>
<span class="line"><span>let doubledValues = values.map((n: number) =&gt; n * 2);</span></span>
<span class="line"><span>let evenValues = values.filter((n: number) =&gt; n % 2 == 0);</span></span></code></pre></div><hr><h2 id="🚀-6-lap-trinh-bat-đong-bo-async-await" tabindex="-1">🚀 6. Lập Trình Bất Đồng Bộ (<code>async</code> / <code>await</code>) <a class="header-anchor" href="#🚀-6-lap-trinh-bat-đong-bo-async-await" aria-label="Permalink to &quot;🚀 6. Lập Trình Bất Đồng Bộ (\`async\` / \`await\`)&quot;">​</a></h2><p>Vit tích hợp cơ chế Concurrency siêu nhẹ dựa trên Event Loop và Thread Pool hiệu năng cao.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>async function fetchData(url: string): string {</span></span>
<span class="line"><span>    // Thực thi HTTP request bất đồng bộ</span></span>
<span class="line"><span>    let response = await http.get(url);</span></span>
<span class="line"><span>    return response.body;</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>async function main() {</span></span>
<span class="line"><span>    let data = await fetchData(&quot;https://api.vito.dev/status&quot;);</span></span>
<span class="line"><span>    println(&quot;Response data: &quot; + data);</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🛡️-7-quan-ly-bo-nho-tu-đong-arc" tabindex="-1">🛡️ 7. Quản Lý Bộ Nhớ Tự Động (ARC) <a class="header-anchor" href="#🛡️-7-quan-ly-bo-nho-tu-đong-arc" aria-label="Permalink to &quot;🛡️ 7. Quản Lý Bộ Nhớ Tự Động (ARC)&quot;">​</a></h2><p>Vit <strong>không sử dụng Garbage Collector (GC)</strong> gây dừng chương trình (Stop-the-World pauses).</p><p>Thay vào đó, trình biên dịch Vit phân tích luồng dữ liệu và chèn các chỉ thị <strong>Automatic Reference Counting (ARC)</strong> combined với <strong>LLVM Escape Analysis Pass</strong>. Nhờ đó:</p><ul><li>Bộ nhớ Heap được giải phóng ngay lập tức khi biến vượt ra ngoài phạm vi (Out of Scope).</li><li>Tối ưu hóa phân bổ trên Stack (<code>alloca</code>) thay vì Heap cho các đối tượng cục bộ.</li><li>Đạt tốc độ thực thi tương đương C/C++ và Rust.</li></ul><hr><h2 id="🔗-8-giao-tiep-c-ffi-foreign-function-interface" tabindex="-1">🔗 8. Giao Tiếp C FFI (Foreign Function Interface) <a class="header-anchor" href="#🔗-8-giao-tiep-c-ffi-foreign-function-interface" aria-label="Permalink to &quot;🔗 8. Giao Tiếp C FFI (Foreign Function Interface)&quot;">​</a></h2><p>Vit có thể trực tiếp gọi các thư viện C gốc mà không phát sinh chi phí trung gian.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Khai báo hàm C bên ngoài</span></span>
<span class="line"><span>extern function sqrt(x: number): number;</span></span>
<span class="line"><span>extern function clock(): number;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let result = sqrt(16.0); // Gọi trực tiếp hàm C native</span></span></code></pre></div>`,43)])])}const d=a(e,[["render",p]]);export{g as __pageData,d as default};
