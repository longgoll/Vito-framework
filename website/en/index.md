---
layout: home

hero:
  name: "Vito Framework"
  text: "Ultra-Fast Web Framework for VIT"
  tagline: "High-performance, lightweight, and expressive Web Server Framework built on the native VIT Language runtime."
  image:
    src: /logo.svg
    alt: Vito Framework Logo
  actions:
    - theme: brand
      text: Get Started
      link: /en/guide/getting-started
    - theme: alt
      text: Playground
      link: /playground
    - theme: alt
      text: API Reference
      link: /en/api/overview
    - theme: alt
      text: GitHub
      link: https://github.com/longgoll/Vito-framework

features:
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon></svg>'
    title: Fast HTTP Request Engine
    details: Zero-dependency parser for GET, POST, PUT, DELETE with automatic query parameters, header, and payload body extraction.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 16 14"></polyline></svg>'
    title: Dynamic Routing & Grouping
    details: Segment matching (:id), multi-level path parameters, and modular prefix grouping with app.group().
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M10 13a5 5 0 0 0 7.54.54l3-3a5 5 0 0 0-7.07-7.07l-1.72 1.71"></path><path d="M14 11a5 5 0 0 0-7.54-.54l-3 3a5 5 0 0 0 7.07 7.07l1.71-1.71"></path></svg>'
    title: Rich Middleware Ecosystem
    details: Official chainable plugins for CORS, Logger, Static Files, Rate Limiting, JWT Auth, SSE, and Security Headers.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="11" width="18" height="11" rx="2" ry="2"></rect><path d="M7 11V7a5 5 0 0 1 10 0v4"></path></svg>'
    title: Native TLS 1.3 & Protocols
    details: Built-in TLS encryption, HTTP/2 multiplexing, and HTTP/3 QUIC (UDP) transport engine.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="2" y1="12" x2="22" y2="12"></line><path d="M12 2a15.3 15.3 0 0 1 4 10 15.3 15.3 0 0 1-4 10 15.3 15.3 0 0 1-4-10 15.3 15.3 0 0 1 4-10z"></path></svg>'
    title: Edge Runtime & WASM
    details: Serverless WebAssembly adapter for Cloudflare Workers & AWS Lambda with < 1ms cold start time.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M18 8A6 6 0 0 0 6 8c0 7-3 9-3 9h18s-3-2-3-9"></path><path d="M13.73 21a2 2 0 0 1-3.46 0"></path></svg>'
    title: Real-Time WebSockets & Job Queue
    details: Full-Duplex WebSockets with Rooms, Heartbeats, Deflate compression, Async Event Bus, and Redis Job Queue with Retries & DLQ.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><line x1="18" y1="20" x2="18" y2="10"></line><line x1="12" y1="20" x2="12" y2="4"></line><line x1="6" y1="20" x2="6" y2="14"></line></svg>'
    title: Prometheus Metrics & DX Tooling
    details: Native Prometheus /metrics endpoint, W3C OpenTelemetry Tracing, ultra-fast in-memory test injector, DB isolation fixtures, and vit doctor diagnostics.
  - icon: '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="4 17 10 11 4 5"></polyline><line x1="12" y1="19" x2="20" y2="19"></line></svg>'
    title: Developer Tooling & CLI
    details: Hot Module Reloading (vit dev), code generators (vit g controller), project scaffolding (vit create), and single binary compilation (< 5MB).
---

<div class="strategic-pillars" style="margin: 2.5rem 0; padding: 2rem; background: linear-gradient(135deg, rgba(16, 185, 129, 0.08), rgba(99, 102, 241, 0.08)); border: 1px solid var(--vp-c-brand-1); border-radius: 16px;">
<h2 style="margin-top: 0; text-align: center; font-size: 1.8rem; font-weight: 800; background: linear-gradient(120deg, #10b981, #6366f1); -webkit-background-clip: text; -webkit-text-fill-color: transparent;">🚀 4 Strategic Pillars of Vito</h2>

<div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 1.5rem; margin-top: 1.5rem;">

<div style="padding: 1.2rem; background: var(--vp-c-bg-soft); border-radius: 12px; border: 1px solid var(--vp-c-divider);">
  <h3 style="margin-top: 0; color: var(--vp-c-brand-1);">⚡ 1. LLVM Native Benchmark</h3>
  <p style="font-size: 0.95rem; color: var(--vp-c-text-2); margin-bottom: 0;">Direct LLVM Native Machine Code compilation. Completely eliminates GC pauses & V8 runtime overhead, outperforming Node.js (Express/Fastify), Go (Gin), and approaching C++20.</p>
</div>

<div style="padding: 1.2rem; background: var(--vp-c-bg-soft); border-radius: 12px; border: 1px solid var(--vp-c-divider);">
  <h3 style="margin-top: 0; color: #8b5cf6;">🤖 2. AI-Native Token Streaming</h3>
  <p style="font-size: 0.95rem; color: var(--vp-c-text-2); margin-bottom: 0;">Built-in <code>vito/sse</code> package with specialized Server-Sent Events helpers for OpenAI, Claude, and Local LLM token streaming out-of-the-box.</p>
</div>

<div style="padding: 1.2rem; background: var(--vp-c-bg-soft); border-radius: 12px; border: 1px solid var(--vp-c-divider);">
  <h3 style="margin-top: 0; color: #3b82f6;">🛡️ 3. Auto Swagger UI & OpenAPI</h3>
  <p style="font-size: 0.95rem; color: var(--vp-c-text-2); margin-bottom: 0;">Automatically generates <code>/openapi.json</code> spec and interactive Swagger UI at <code>/docs</code> directly from route definitions without manual annotations.</p>
</div>

<div style="padding: 1.2rem; background: var(--vp-c-bg-soft); border-radius: 12px; border: 1px solid var(--vp-c-divider);">
  <h3 style="margin-top: 0; color: #f59e0b;">📦 4. Batteries-Included Ecosystem</h3>
  <p style="font-size: 0.95rem; color: var(--vp-c-text-2); margin-bottom: 0;">Ships with 27 official packages (<code>vito/orm</code>, <code>vito/auth</code>, <code>vito/upload</code>, <code>vito/websocket</code>...). Zero dependency rot and unified code quality.</p>
</div>

</div>
</div>

<BenchmarkVisualizer />

<CodeComparison />
