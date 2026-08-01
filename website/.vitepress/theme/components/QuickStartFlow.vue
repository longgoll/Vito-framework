<template>
  <div class="quick-start-flow">
    <div class="flow-nav">
      <button 
        v-for="(step, index) in steps" 
        :key="index"
        :class="['flow-tab', { active: currentStep === index }]"
        @click="currentStep = index"
      >
        <span class="step-num">{{ index + 1 }}</span>
        <span class="step-label">{{ step.shortTitle }}</span>
      </button>
    </div>

    <div class="flow-content">
      <div class="step-info">
        <h3 class="step-heading">
          <span class="icon">{{ activeStep.icon }}</span>
          {{ activeStep.title }}
        </h3>
        <p class="step-desc">{{ activeStep.description }}</p>
      </div>

      <div class="step-body">
        <div class="code-header">
          <span class="file-name">{{ activeStep.fileName }}</span>
          <span class="badge">{{ activeStep.tag }}</span>
        </div>
        <pre class="code-preview"><code>{{ activeStep.code }}</code></pre>
      </div>

      <div v-if="activeStep.terminalOutput" class="terminal-box">
        <div class="terminal-header">
          <span class="dot red"></span>
          <span class="dot yellow"></span>
          <span class="dot green"></span>
          <span class="term-title">Terminal Output</span>
        </div>
        <pre class="term-body"><code>{{ activeStep.terminalOutput }}</code></pre>
      </div>

      <div class="flow-actions">
        <button 
          v-if="currentStep > 0" 
          class="btn-prev"
          @click="currentStep--"
        >
          ← Bước Trước
        </button>
        <div class="spacer"></div>
        <button 
          v-if="currentStep < steps.length - 1" 
          class="btn-next"
          @click="currentStep++"
        >
          Bước Tiếp Theo →
        </button>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

const currentStep = ref(0)

const steps = [
  {
    shortTitle: 'Cài Đặt',
    icon: '⚡',
    title: '1. Cài Đặt Toolchain Vito (1-Line Install)',
    description: 'Chạy duy nhất 1 câu lệnh để cài đặt bộ dịch Vit Compiler Engine & Vito CLI tự động cấu hình PATH.',
    fileName: 'Terminal (PowerShell / Bash)',
    tag: 'CLI Command',
    code: `# Windows PowerShell:
iwr -useb https://raw.githubusercontent.com/longgoll/vit/main/install.ps1 | iex

# Linux / macOS:
curl -fsSL https://raw.githubusercontent.com/longgoll/vit/main/install.sh | bash`,
    terminalOutput: `[✓] Downloading Vit Compiler Engine v2.0.0...
[✓] Installing Vito CLI to PATH...
[✓] Clang/LLVM toolchain verified.
⚡ Vito Toolchain is ready! Run 'vit setup' to verify.`
  },
  {
    shortTitle: 'Khởi Tạo',
    icon: '📦',
    title: '2. Khởi Tạo Dự Án Mới (vit init)',
    description: 'Dùng lệnh vit init để tự động tạo cấu trúc thư mục chuẩn và file main.vit.',
    fileName: 'Terminal',
    tag: 'Project Setup',
    code: `vit init my-web-app
cd my-web-app`,
    terminalOutput: `✨ Created project my-web-app/
├── vit.json          (App Config)
└── src/main.vit      (Entrypoint)

🚀 Done! Run 'cd my-web-app' and 'vit dev' to start developing.`
  },
  {
    shortTitle: 'Viết Code',
    icon: '💻',
    title: '3. Viết HTTP Server (src/main.vit)',
    description: 'Viết Web Server với cú pháp cực kỳ đơn giản, mạch lạc và hỗ trợ TypeScript/VIT Native.',
    fileName: 'src/main.vit',
    tag: 'VIT Language Code',
    code: `import { Vito, Request, Response } from "vito";

function main(): number {
    let app = Vito.new();

    // 1. Route HTML
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    // 2. Route JSON API
    app.get("/api/health", (req: Request, res: Response) => {
        res.json({ status: "UP", engine: "Vit Native Engine", uptime: "ok" });
    });

    app.listen(3000);
    print("⚡ Server running at http://localhost:3000");
    return 0;
}`,
    terminalOutput: null
  },
  {
    shortTitle: 'Chạy App',
    icon: '🔥',
    title: '4. Khởi Chạy Live-Reload (vit dev)',
    description: 'Chạy dev server với tính năng Hot-Reloading tự động biên dịch lại cực nhanh mỗi khi bạn lưu file.',
    fileName: 'Terminal',
    tag: 'Live Reloading',
    code: `vit dev`,
    terminalOutput: `⚡ Vito Dev Engine v2.0 (Watcher active)
[Compiler] Compiling src/main.vit... (build time: 14ms)
[Server] Listening on http://localhost:3000
[Watcher] Ready for file changes.`
  }
]

const activeStep = computed(() => steps[currentStep.value])
</script>

<style scoped>
.quick-start-flow {
  background: var(--vp-c-bg-soft, #f8fafc);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 16px;
  overflow: hidden;
  margin: 2rem 0;
  box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.05);
}

.flow-nav {
  display: flex;
  background: var(--vp-c-bg-alt, #f1f5f9);
  border-bottom: 1px solid var(--vp-c-divider, #e2e8f0);
}

.flow-tab {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.6rem;
  padding: 0.9rem 0.5rem;
  background: transparent;
  border: none;
  border-bottom: 3px solid transparent;
  color: var(--vp-c-text-2, #64748b);
  font-weight: 600;
  font-size: 0.9rem;
  cursor: pointer;
  transition: all 0.2s ease;
}

.flow-tab:hover {
  color: var(--vp-c-text-1, #0f172a);
  background: rgba(0, 0, 0, 0.03);
}

.flow-tab.active {
  color: var(--vp-c-brand-1, #4f46e5);
  border-bottom-color: var(--vp-c-brand-1, #4f46e5);
  background: var(--vp-c-bg-soft, #ffffff);
}

.step-num {
  width: 22px;
  height: 22px;
  border-radius: 50%;
  background: var(--vp-c-divider, #cbd5e1);
  color: var(--vp-c-text-2, #475569);
  font-size: 0.75rem;
  font-weight: 700;
  display: flex;
  align-items: center;
  justify-content: center;
}

.flow-tab.active .step-num {
  background: var(--vp-c-brand-1, #4f46e5);
  color: #ffffff;
}

.flow-content {
  padding: 1.5rem;
}

.step-heading {
  margin: 0 0 0.5rem 0;
  font-size: 1.15rem;
  font-weight: 700;
  color: var(--vp-c-text-1, #0f172a);
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.step-desc {
  margin: 0 0 1.25rem 0;
  color: var(--vp-c-text-2, #475569);
  font-size: 0.95rem;
  line-height: 1.5;
}

.code-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: #1e293b;
  padding: 0.6rem 1rem;
  border-top-left-radius: 10px;
  border-top-right-radius: 10px;
  border: 1px solid #334155;
  border-bottom: none;
}

.file-name {
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.85rem;
  color: #38bdf8;
  font-weight: 600;
}

.badge {
  font-size: 0.7rem;
  padding: 0.15rem 0.5rem;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 4px;
  color: #94a3b8;
}

.code-preview {
  margin: 0 0 1.25rem 0;
  padding: 1.25rem;
  background: #0f172a;
  border-bottom-left-radius: 10px;
  border-bottom-right-radius: 10px;
  border: 1px solid #334155;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.9rem;
  color: #f8fafc;
  overflow-x: auto;
  line-height: 1.6;
}

.terminal-box {
  background: #090d16;
  border: 1px solid #1e293b;
  border-radius: 10px;
  overflow: hidden;
  margin-bottom: 1.25rem;
}

.terminal-header {
  display: flex;
  align-items: center;
  gap: 6px;
  padding: 0.5rem 0.8rem;
  background: #1e293b;
  border-bottom: 1px solid #334155;
}

.dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
}
.dot.red { background: #ef4444; }
.dot.yellow { background: #f59e0b; }
.dot.green { background: #10b981; }

.term-title {
  margin-left: 8px;
  font-size: 0.75rem;
  color: #94a3b8;
  font-family: var(--vp-font-family-mono, monospace);
  font-weight: 600;
}

.term-body {
  margin: 0;
  padding: 1rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.85rem;
  color: #34d399;
  line-height: 1.6;
  background: #090d16;
}

.flow-actions {
  display: flex;
  align-items: center;
  margin-top: 1rem;
}

.spacer { flex: 1; }

.btn-next, .btn-prev {
  padding: 0.65rem 1.3rem;
  border-radius: 8px;
  font-weight: 600;
  font-size: 0.9rem;
  cursor: pointer;
  transition: all 0.2s ease;
}

.btn-next {
  background: #4f46e5;
  color: #ffffff;
  border: none;
  box-shadow: 0 4px 12px rgba(79, 70, 229, 0.3);
}

.btn-next:hover {
  background: #4338ca;
  transform: translateX(2px);
}

.btn-prev {
  background: transparent;
  color: var(--vp-c-text-2, #475569);
  border: 1px solid var(--vp-c-divider, #cbd5e1);
}

.btn-prev:hover {
  color: var(--vp-c-text-1, #0f172a);
  border-color: var(--vp-c-text-2, #64748b);
}
</style>
