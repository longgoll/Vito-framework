# Vito Web Framework 🚀

> High-performance, lightweight, expressive Web Server Framework for the **VIT Language ecosystem**.

## 🌟 Overview
**Vito** is an open-source, modern Web Framework built on top of the native **VIT Compiler runtime**. Designed for simplicity, speed, and DX excellence.

## 📁 Repository Architecture (Monorepo)

```text
vito/
├── vit.json            # Framework package config
├── README.md           # Documentation
├── src/                # Core Web Framework Engine
│   └── vito.vit        # HTTP Router & App Server
├── packages/           # Official Middleware Plugins
│   ├── cors/           # CORS header middleware
│   └── logger/         # Request logger middleware
└── examples/           # Web app examples
    └── app_demo.vit    # App demo
```

## 🚀 Quick Start

### 1. Installation via `vit pm`
```bash
vit add github.com/longgoll/Vito-farrmwork
```

### 2. Basic Web Server Example
```javascript
import { runVitoApp } from "vito/src/vito.vit";
import { logRequest } from "vito/packages/logger/logger.vit";

function main(): number {
    logRequest("GET", "/api/status");
    runVitoApp(8080);
    return 0;
}
```

### 3. Run Application
```bash
vit run main.vit
```

## 📜 License
MIT License - Open-source & Free for the VIT Community.
