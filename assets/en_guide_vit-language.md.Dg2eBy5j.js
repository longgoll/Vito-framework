import{_ as s,o as n,c as e,a2 as t}from"./chunks/framework.CAlA1Nx7.js";const u=JSON.parse('{"title":"VIT Programming Language (VIT Language)","description":"","frontmatter":{},"headers":[],"relativePath":"en/guide/vit-language.md","filePath":"en/guide/vit-language.md"}'),i={name:"en/guide/vit-language.md"};function p(l,a,o,r,c,d){return n(),e("div",null,[...a[0]||(a[0]=[t(`<h1 id="vit-programming-language-vit-language" tabindex="-1">VIT Programming Language (VIT Language) <a class="header-anchor" href="#vit-programming-language-vit-language" aria-label="Permalink to &quot;VIT Programming Language (VIT Language)&quot;">​</a></h1><p><strong>VIT</strong> is a modern compiled language with extremely high performance, designed with an expressive syntax, a powerful Strong Static Typing system, and automatic memory management via ARC (Automatic Reference Counting).</p><p>Vit is the core foundation used to build the <strong>Vito</strong> web framework ecosystem.</p><hr><h2 id="⚡-1-rapid-start" tabindex="-1">⚡ 1. Rapid Start <a class="header-anchor" href="#⚡-1-rapid-start" aria-label="Permalink to &quot;⚡ 1. Rapid Start&quot;">​</a></h2><h3 id="installing-the-vit-compiler" tabindex="-1">Installing the Vit Compiler <a class="header-anchor" href="#installing-the-vit-compiler" aria-label="Permalink to &quot;Installing the Vit Compiler&quot;">​</a></h3><p>To run Vit source programs (<code>.vit</code>), you need the <code>vit</code> compiler:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Download and install automatically on Windows (PowerShell)</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">iwr</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -useb</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.ps1</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> iex</span></span>
<span class="line"></span>
<span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Download and install on Linux/macOS</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">curl</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -fsSL</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.sh</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> sh</span></span></code></pre></div><p>Verify version after installation:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">vit</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> --version</span></span></code></pre></div><hr><h2 id="📝-2-variables-data-types" tabindex="-1">📝 2. Variables &amp; Data Types <a class="header-anchor" href="#📝-2-variables-data-types" aria-label="Permalink to &quot;📝 2. Variables &amp; Data Types&quot;">​</a></h2><p>Vit supports automatic Type Inference as well as explicit type declarations.</p><h3 id="primitive-data-types" tabindex="-1">Primitive Data Types <a class="header-anchor" href="#primitive-data-types" aria-label="Permalink to &quot;Primitive Data Types&quot;">​</a></h3><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Define variables with let (mutable) and const (constant)</span></span>
<span class="line"><span>let age: number = 25;</span></span>
<span class="line"><span>let pi = 3.14159;            // Auto-inferred type &#39;number&#39;</span></span>
<span class="line"><span>let isProduction: boolean = true;</span></span>
<span class="line"><span>let greeting: string = &quot;Hello Vit Ecosystem!&quot;;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Constants</span></span>
<span class="line"><span>const MAX_CONNECTIONS = 10000;</span></span></code></pre></div><h3 id="arrays" tabindex="-1">Arrays <a class="header-anchor" href="#arrays" aria-label="Permalink to &quot;Arrays&quot;">​</a></h3><p>Arrays in Vit have flexible size and are automatically memory-managed:</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>let numbers: number[] = [10, 20, 30, 40];</span></span>
<span class="line"><span>numbers.push(50);</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let first = numbers[0];</span></span>
<span class="line"><span>let totalLen = numbers.length;</span></span></code></pre></div><hr><h2 id="🏗️-3-structs-methods" tabindex="-1">🏗️ 3. Structs &amp; Methods <a class="header-anchor" href="#🏗️-3-structs-methods" aria-label="Permalink to &quot;🏗️ 3. Structs &amp; Methods&quot;">​</a></h2><p>Vit allows grouping data using <code>struct</code> and defining associated methods.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>struct Point {</span></span>
<span class="line"><span>    x: number,</span></span>
<span class="line"><span>    y: number,</span></span>
<span class="line"><span></span></span>
<span class="line"><span>    // Method belonging to the Struct</span></span>
<span class="line"><span>    function distance(): number {</span></span>
<span class="line"><span>        return sqrt(this.x * this.x + this.y * this.y);</span></span>
<span class="line"><span>    }</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Instantiate the Struct</span></span>
<span class="line"><span>let p = Point { x: 3.0, y: 4.0 };</span></span>
<span class="line"><span>let dist = p.distance(); // Result: 5.0</span></span></code></pre></div><hr><h2 id="🔀-4-enums-tagged-unions-pattern-matching-match" tabindex="-1">🔀 4. Enums, Tagged Unions &amp; Pattern Matching (<code>match</code>) <a class="header-anchor" href="#🔀-4-enums-tagged-unions-pattern-matching-match" aria-label="Permalink to &quot;🔀 4. Enums, Tagged Unions &amp; Pattern Matching (\`match\`)&quot;">​</a></h2><p>Vit strongly supports safe programming with <code>enum</code> types and <code>match</code> pattern matching.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>enum Status {</span></span>
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
<span class="line"><span>// Match expression pattern matching</span></span>
<span class="line"><span>function checkStatus(s: Status): string {</span></span>
<span class="line"><span>    return match (s) {</span></span>
<span class="line"><span>        Status.Pending =&gt; &quot;Awaiting processing&quot;,</span></span>
<span class="line"><span>        Status.Active =&gt; &quot;Currently active&quot;,</span></span>
<span class="line"><span>        Status.Completed =&gt; &quot;Completed&quot;</span></span>
<span class="line"><span>    };</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🎯-5-functional-programming-arrow-functions-lambdas" tabindex="-1">🎯 5. Functional Programming &amp; Arrow Functions (Lambdas) <a class="header-anchor" href="#🎯-5-functional-programming-arrow-functions-lambdas" aria-label="Permalink to &quot;🎯 5. Functional Programming &amp; Arrow Functions (Lambdas)&quot;">​</a></h2><p>Vit treats Functions as First-Class Citizens. You can easily use Lambda functions and advanced array processing methods.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Arrow Function</span></span>
<span class="line"><span>let double = (x: number): number =&gt; x * 2.0;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let values: number[] = [1, 2, 3, 4, 5];</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Higher-order methods: map, filter, forEach</span></span>
<span class="line"><span>let doubledValues = values.map((n: number) =&gt; n * 2);</span></span>
<span class="line"><span>let evenValues = values.filter((n: number) =&gt; n % 2 == 0);</span></span></code></pre></div><hr><h2 id="🚀-6-async-programming-async-await" tabindex="-1">🚀 6. Async Programming (<code>async</code> / <code>await</code>) <a class="header-anchor" href="#🚀-6-async-programming-async-await" aria-label="Permalink to &quot;🚀 6. Async Programming (\`async\` / \`await\`)&quot;">​</a></h2><p>Vit integrates an ultra-lightweight Concurrency mechanism based on a high-performance Event Loop and Thread Pool.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>async function fetchData(url: string): string {</span></span>
<span class="line"><span>    // Execute async HTTP request</span></span>
<span class="line"><span>    let response = await http.get(url);</span></span>
<span class="line"><span>    return response.body;</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>async function main() {</span></span>
<span class="line"><span>    let data = await fetchData(&quot;https://api.vito.dev/status&quot;);</span></span>
<span class="line"><span>    println(&quot;Response data: &quot; + data);</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🛡️-7-automatic-memory-management-arc" tabindex="-1">🛡️ 7. Automatic Memory Management (ARC) <a class="header-anchor" href="#🛡️-7-automatic-memory-management-arc" aria-label="Permalink to &quot;🛡️ 7. Automatic Memory Management (ARC)&quot;">​</a></h2><p>Vit <strong>does not use a Garbage Collector (GC)</strong> that causes Stop-the-World pauses.</p><p>Instead, the Vit compiler analyzes data flow and inserts <strong>Automatic Reference Counting (ARC)</strong> instructions combined with an <strong>LLVM Escape Analysis Pass</strong>. As a result:</p><ul><li>Heap memory is freed immediately when a variable goes out of scope.</li><li>Stack allocation (<code>alloca</code>) is optimized over Heap for local objects.</li><li>Achieves execution speed comparable to C/C++ and Rust.</li></ul><hr><h2 id="🔗-8-c-ffi-foreign-function-interface" tabindex="-1">🔗 8. C FFI (Foreign Function Interface) <a class="header-anchor" href="#🔗-8-c-ffi-foreign-function-interface" aria-label="Permalink to &quot;🔗 8. C FFI (Foreign Function Interface)&quot;">​</a></h2><p>Vit can directly call native C libraries with zero intermediate overhead.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Declare external C functions</span></span>
<span class="line"><span>extern function sqrt(x: number): number;</span></span>
<span class="line"><span>extern function clock(): number;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let result = sqrt(16.0); // Calls native C function directly</span></span></code></pre></div>`,43)])])}const g=s(i,[["render",p]]);export{u as __pageData,g as default};
