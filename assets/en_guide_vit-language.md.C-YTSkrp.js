import{_ as s,o as n,c as e,a2 as t}from"./chunks/framework.CAlA1Nx7.js";const u=JSON.parse('{"title":"The VIT Programming Language","description":"","frontmatter":{},"headers":[],"relativePath":"en/guide/vit-language.md","filePath":"en/guide/vit-language.md"}'),i={name:"en/guide/vit-language.md"};function p(l,a,r,o,c,h){return n(),e("div",null,[...a[0]||(a[0]=[t(`<h1 id="the-vit-programming-language" tabindex="-1">The VIT Programming Language <a class="header-anchor" href="#the-vit-programming-language" aria-label="Permalink to &quot;The VIT Programming Language&quot;">​</a></h1><p><strong>VIT</strong> is a modern, high-performance compiled programming language designed with expressive syntax, strong static typing, and automatic memory management (ARC).</p><p>Vit powers the entire core ecosystem of the <strong>Vito</strong> web framework.</p><hr><h2 id="⚡-1-quick-start" tabindex="-1">⚡ 1. Quick Start <a class="header-anchor" href="#⚡-1-quick-start" aria-label="Permalink to &quot;⚡ 1. Quick Start&quot;">​</a></h2><h3 id="installing-the-vit-compiler" tabindex="-1">Installing the Vit Compiler <a class="header-anchor" href="#installing-the-vit-compiler" aria-label="Permalink to &quot;Installing the Vit Compiler&quot;">​</a></h3><p>To compile and execute Vit source files (<code>.vit</code>), install the <code>vit</code> compiler CLI:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Windows (PowerShell)</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">iwr</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -useb</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.ps1</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> iex</span></span>
<span class="line"></span>
<span class="line"><span style="--shiki-light:#6A737D;--shiki-dark:#6A737D;"># Linux / macOS</span></span>
<span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">curl</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> -fsSL</span><span style="--shiki-light:#032F62;--shiki-dark:#9ECBFF;"> https://vit-lang.org/install.sh</span><span style="--shiki-light:#D73A49;--shiki-dark:#F97583;"> |</span><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;"> sh</span></span></code></pre></div><p>Verify your installation:</p><div class="language-bash vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">bash</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span style="--shiki-light:#6F42C1;--shiki-dark:#B392F0;">vit</span><span style="--shiki-light:#005CC5;--shiki-dark:#79B8FF;"> --version</span></span></code></pre></div><hr><h2 id="📝-2-variables-primitive-types" tabindex="-1">📝 2. Variables &amp; Primitive Types <a class="header-anchor" href="#📝-2-variables-primitive-types" aria-label="Permalink to &quot;📝 2. Variables &amp; Primitive Types&quot;">​</a></h2><p>Vit supports both explicit type annotations and type inference.</p><h3 id="primitive-types" tabindex="-1">Primitive Types <a class="header-anchor" href="#primitive-types" aria-label="Permalink to &quot;Primitive Types&quot;">​</a></h3><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Mutable variables with let, constants with const</span></span>
<span class="line"><span>let age: number = 25;</span></span>
<span class="line"><span>let pi = 3.14159;            // Inferred as &#39;number&#39;</span></span>
<span class="line"><span>let isProduction: boolean = true;</span></span>
<span class="line"><span>let greeting: string = &quot;Hello Vit Ecosystem!&quot;;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Constants</span></span>
<span class="line"><span>const MAX_CONNECTIONS = 10000;</span></span></code></pre></div><h3 id="arrays" tabindex="-1">Arrays <a class="header-anchor" href="#arrays" aria-label="Permalink to &quot;Arrays&quot;">​</a></h3><p>Arrays in Vit are dynamically resized and automatically managed:</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>let numbers: number[] = [10, 20, 30, 40];</span></span>
<span class="line"><span>numbers.push(50);</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let first = numbers[0];</span></span>
<span class="line"><span>let totalLen = numbers.length;</span></span></code></pre></div><hr><h2 id="🏗️-3-structs-methods" tabindex="-1">🏗️ 3. Structs &amp; Methods <a class="header-anchor" href="#🏗️-3-structs-methods" aria-label="Permalink to &quot;🏗️ 3. Structs &amp; Methods&quot;">​</a></h2><p>Vit allows grouping data into structs and attaching methods.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>struct Point {</span></span>
<span class="line"><span>    x: number,</span></span>
<span class="line"><span>    y: number,</span></span>
<span class="line"><span></span></span>
<span class="line"><span>    // Struct Method</span></span>
<span class="line"><span>    function distance(): number {</span></span>
<span class="line"><span>        return sqrt(this.x * this.x + this.y * this.y);</span></span>
<span class="line"><span>    }</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Instantiation</span></span>
<span class="line"><span>let p = Point { x: 3.0, y: 4.0 };</span></span>
<span class="line"><span>let dist = p.distance(); // Result: 5.0</span></span></code></pre></div><hr><h2 id="🔀-4-enums-tagged-unions-pattern-matching-match" tabindex="-1">🔀 4. Enums, Tagged Unions &amp; Pattern Matching (<code>match</code>) <a class="header-anchor" href="#🔀-4-enums-tagged-unions-pattern-matching-match" aria-label="Permalink to &quot;🔀 4. Enums, Tagged Unions &amp; Pattern Matching (\`match\`)&quot;">​</a></h2><p>Vit provides expressive pattern matching with <code>match</code> and tagged unions.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>enum Status {</span></span>
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
<span class="line"><span>// Pattern matching expression</span></span>
<span class="line"><span>function checkStatus(s: Status): string {</span></span>
<span class="line"><span>    return match (s) {</span></span>
<span class="line"><span>        Status.Pending =&gt; &quot;Pending execution&quot;,</span></span>
<span class="line"><span>        Status.Active =&gt; &quot;Currently active&quot;,</span></span>
<span class="line"><span>        Status.Completed =&gt; &quot;Completed successfully&quot;</span></span>
<span class="line"><span>    };</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🎯-5-functional-programming-arrow-functions-lambdas" tabindex="-1">🎯 5. Functional Programming &amp; Arrow Functions (Lambdas) <a class="header-anchor" href="#🎯-5-functional-programming-arrow-functions-lambdas" aria-label="Permalink to &quot;🎯 5. Functional Programming &amp; Arrow Functions (Lambdas)&quot;">​</a></h2><p>Functions in Vit are First-Class Citizens.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// Arrow Function</span></span>
<span class="line"><span>let double = (x: number): number =&gt; x * 2.0;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let values: number[] = [1, 2, 3, 4, 5];</span></span>
<span class="line"><span></span></span>
<span class="line"><span>// Higher-order array methods: map, filter, forEach</span></span>
<span class="line"><span>let doubledValues = values.map((n: number) =&gt; n * 2);</span></span>
<span class="line"><span>let evenValues = values.filter((n: number) =&gt; n % 2 == 0);</span></span></code></pre></div><hr><h2 id="🚀-6-asynchronous-programming-async-await" tabindex="-1">🚀 6. Asynchronous Programming (<code>async</code> / <code>await</code>) <a class="header-anchor" href="#🚀-6-asynchronous-programming-async-await" aria-label="Permalink to &quot;🚀 6. Asynchronous Programming (\`async\` / \`await\`)&quot;">​</a></h2><p>Vit features lightweight concurrency backed by an event loop and worker thread pool.</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>async function fetchData(url: string): string {</span></span>
<span class="line"><span>    let response = await http.get(url);</span></span>
<span class="line"><span>    return response.body;</span></span>
<span class="line"><span>}</span></span>
<span class="line"><span></span></span>
<span class="line"><span>async function main() {</span></span>
<span class="line"><span>    let data = await fetchData(&quot;https://api.vito.dev/status&quot;);</span></span>
<span class="line"><span>    println(&quot;Response data: &quot; + data);</span></span>
<span class="line"><span>}</span></span></code></pre></div><hr><h2 id="🛡️-7-automatic-memory-management-arc" tabindex="-1">🛡️ 7. Automatic Memory Management (ARC) <a class="header-anchor" href="#🛡️-7-automatic-memory-management-arc" aria-label="Permalink to &quot;🛡️ 7. Automatic Memory Management (ARC)&quot;">​</a></h2><p>Vit operates <strong>without a Garbage Collector (GC)</strong>, avoiding Stop-the-World pauses.</p><p>The Vit compiler automatically inserts <strong>Automatic Reference Counting (ARC)</strong> instructions alongside an <strong>LLVM Escape Analysis Pass</strong>.</p><ul><li>Heap objects are freed immediately when variables fall out of scope.</li><li>Stack allocations are prioritized over Heap allocations.</li><li>Delivers native execution speed comparable to C/C++ and Rust.</li></ul><hr><h2 id="🔗-8-c-ffi-foreign-function-interface" tabindex="-1">🔗 8. C FFI (Foreign Function Interface) <a class="header-anchor" href="#🔗-8-c-ffi-foreign-function-interface" aria-label="Permalink to &quot;🔗 8. C FFI (Foreign Function Interface)&quot;">​</a></h2><p>Vit natively calls C libraries without overhead:</p><div class="language-vit vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">vit</span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span>// External C function declaration</span></span>
<span class="line"><span>extern function sqrt(x: number): number;</span></span>
<span class="line"><span>extern function clock(): number;</span></span>
<span class="line"><span></span></span>
<span class="line"><span>let result = sqrt(16.0); // Direct C call</span></span></code></pre></div>`,43)])])}const g=s(i,[["render",p]]);export{u as __pageData,g as default};
