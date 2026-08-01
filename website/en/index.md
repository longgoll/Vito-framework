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
      text: Get Started 🚀
      link: /en/guide/getting-started
    - theme: alt
      text: API Reference 📚
      link: /en/api/overview
    - theme: alt
      text: View on GitHub 🐙
      link: https://github.com/vit-lang/vito

features:
  - icon: ⚡
    title: Fast HTTP Request Engine
    details: Zero-dependency parser for GET, POST, PUT, DELETE with automatic query parameters, header, and payload body extraction.
  - icon: 🎯
    title: Dynamic Routing & Grouping
    details: Segment matching (:id), multi-level path parameters, and modular prefix grouping with app.group().
  - icon: 🔗
    title: Rich Middleware Ecosystem
    details: Official chainable plugins for CORS, Logger, Static Files, Rate Limiting, JWT Auth, SSE, and Security Headers.
  - icon: 🔒
    title: Native TLS 1.3 & Protocols
    details: Built-in TLS encryption, HTTP/2 multiplexing, and HTTP/3 QUIC (UDP) transport engine.
  - icon: 🌐
    title: Edge Runtime & WASM
    details: Serverless WebAssembly adapter for Cloudflare Workers & AWS Lambda with < 1ms cold start time.
  - icon: 🔌
    title: Real-Time WebSockets & Job Queue
    details: Full-Duplex WebSockets with Rooms, Heartbeats, Deflate compression, Async Event Bus, and Redis Job Queue with Retries & DLQ.
  - icon: 📊
    title: Prometheus Metrics & DX Tooling
    details: Native Prometheus /metrics endpoint, W3C OpenTelemetry Tracing, ultra-fast in-memory test injector, DB isolation fixtures, and vit doctor diagnostics.
  - icon: 🛠 Developer Tooling & CLI
    details: Hot Module Reloading (vit dev), code generators (vit g controller), project scaffolding (vit create), and single binary compilation (< 5MB).
---

<BenchmarkVisualizer />

<CodeComparison />

