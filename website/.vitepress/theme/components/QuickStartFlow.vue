<template>
  <div class="vertical-timeline-flow">
    <div class="timeline-container">
      <div 
        v-for="(step, index) in steps" 
        :key="index"
        class="timeline-step"
      >
        <!-- Vertical dashed line & Node marker -->
        <div class="timeline-track">
          <div class="step-node">
            <span>{{ index + 1 }}</span>
          </div>
          <div v-if="index < steps.length - 1" class="dashed-line"></div>
        </div>

        <!-- Step content block -->
        <div class="step-content">
          <div class="step-header">
            <span class="step-tag">Bước {{ index + 1 }}</span>
            <h3 class="step-title">{{ step.title }}</h3>
            <p class="step-desc">{{ step.description }}</p>
          </div>

          <!-- Code Box -->
          <div v-if="Array.isArray(step.code)" class="code-block-group">
            <div v-for="(block, blockIndex) in step.code" :key="blockIndex" class="code-box code-block-item">
              <div class="code-header">
                <span class="file-name">{{ block.label }}</span>
                <span class="tag-badge">{{ step.tag }}</span>
              </div>
              <pre class="code-preview"><code>{{ block.content }}</code></pre>
            </div>
          </div>
          <div v-else class="code-box">
            <div class="code-header">
              <span class="file-name">{{ step.fileName }}</span>
              <span class="tag-badge">{{ step.tag }}</span>
            </div>
            <pre class="code-preview"><code>{{ step.code }}</code></pre>
          </div>

          <!-- Terminal Output Box -->
          <div v-if="step.terminalOutput" class="terminal-box">
            <div class="terminal-header">
              <div class="window-dots">
                <span class="dot red"></span>
                <span class="dot yellow"></span>
                <span class="dot green"></span>
              </div>
              <span class="term-title">Terminal Console Output</span>
            </div>
            <pre class="term-body"><code>{{ step.terminalOutput }}</code></pre>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
const steps = [
  {
    title: 'Cài Đặt Toolchain Vito (1-Line Install)',
    description: 'Chọn câu lệnh phù hợp với hệ điều hành của bạn để tự động tải và cấu hình bộ dịch Vit Compiler Engine & Vito CLI.',
    fileName: 'Terminal',
    tag: 'Install Command',
    code: [
      {
        label: 'Windows (PowerShell)',
        content: `iwr -useb https://raw.githubusercontent.com/longgoll/vit/main/install.ps1 | iex`
      },
      {
        label: 'Linux / macOS (Bash)',
        content: `curl -fsSL https://raw.githubusercontent.com/longgoll/vit/main/install.sh | bash`
      }
    ],
    terminalOutput: `[✓] Downloading Vit Compiler Engine v2.0.0...
[✓] Installing Vito CLI binary to PATH...
[✓] Clang/LLVM toolchain runtime verified.
⚡ Vito Toolchain is ready! Run 'vit setup' to verify.`
  },
  {
    title: 'Khởi Tạo Dự Án Mới (vit init)',
    description: 'Tự động tạo cấu trúc thư mục tiêu chuẩn, file cấu hình vit.json và entrypoint main.vit.',
    fileName: 'Terminal',
    tag: 'Project Setup',
    code: `vit init my-web-app
cd my-web-app`,
    terminalOutput: `✨ Created project directory my-web-app/
├── vit.json          (App Infrastructure Config)
└── src/main.vit      (Application Entrypoint)

🚀 Project ready! Run 'cd my-web-app' and 'vit dev' to start developing.`
  },
  {
    title: 'Viết Code HTTP Server (src/main.vit)',
    description: 'Xây dựng Web API với cú pháp vô cùng đơn giản, mạch lạc và chạy trực tiếp trên VIT Native Engine.',
    fileName: 'src/main.vit',
    tag: 'VIT Code',
    code: `import { Vito, Request, Response } from "vito";

function main(): number {
    let app = Vito.new();

    // 1. Route trả về Web UI HTML
    app.get("/", (req: Request, res: Response) => {
        res.html("<h1>Welcome to Vito Web Framework! 🚀</h1>");
    });

    // 2. Route JSON REST API
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
    title: 'Khởi Chạy Dev Server (vit dev)',
    description: 'Chạy dev server với tính năng Live-Reloading tự động biên dịch lại cực nhanh mỗi khi bạn lưu file.',
    fileName: 'Terminal',
    tag: 'Live Reloading',
    code: `vit dev`,
    terminalOutput: `⚡ Vito Dev Engine v2.0 (Watcher active)
[Compiler] Compiling src/main.vit... (build time: 14ms)
[Server] Listening on http://localhost:3000
[Watcher] Ready for file changes.`
  }
]
</script>

<style scoped>
.vertical-timeline-flow {
  margin: 2rem 0;
}

.timeline-container {
  display: flex;
  flex-direction: column;
  gap: 2rem;
}

.timeline-step {
  display: grid;
  grid-template-columns: 44px 1fr;
  gap: 1.25rem;
  position: relative;
}

/* Vertical Dashed Line Track */
.timeline-track {
  display: flex;
  flex-direction: column;
  align-items: center;
  position: relative;
}

.step-node {
  width: 36px;
  height: 36px;
  border-radius: 50%;
  background: var(--vp-c-brand-1, #6366f1);
  color: #ffffff;
  font-weight: 800;
  font-size: 0.95rem;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 4px 12px rgba(99, 102, 241, 0.3);
  z-index: 2;
}

.dashed-line {
  width: 0;
  flex: 1;
  border-left: 2px dashed var(--vp-c-brand-1, #6366f1);
  opacity: 0.4;
  margin-top: 8px;
  margin-bottom: -1.5rem;
}

/* Step Content Card */
.step-content {
  background: var(--vp-c-bg-soft, #f8fafc);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 14px;
  padding: 1.5rem;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.03);
}

.step-header {
  margin-bottom: 1rem;
}

.step-tag {
  display: inline-block;
  font-size: 0.75rem;
  font-weight: 700;
  color: var(--vp-c-brand-1, #6366f1);
  background: var(--vp-c-brand-soft, rgba(99, 102, 241, 0.12));
  padding: 0.15rem 0.55rem;
  border-radius: 4px;
  margin-bottom: 0.4rem;
  text-transform: uppercase;
  letter-spacing: 0.04em;
}

.step-title {
  margin: 0 0 0.4rem 0;
  font-size: 1.2rem;
  font-weight: 800;
  color: var(--vp-c-text-1, #0f172a);
}

.step-desc {
  margin: 0;
  font-size: 0.92rem;
  color: var(--vp-c-text-2, #475569);
  line-height: 1.5;
}

/* Code Box */
.code-block-group {
  display: grid;
  gap: 0.75rem;
  margin-top: 1rem;
}

.code-box {
  background: #0f172a;
  border-radius: 10px;
  border: 1px solid #1e293b;
  overflow: hidden;
  margin-top: 0;
}

.code-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0.55rem 1rem;
  background: #1e293b;
  border-bottom: 1px solid #334155;
}

.file-name {
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.82rem;
  color: #38bdf8;
  font-weight: 600;
}

.tag-badge {
  font-size: 0.7rem;
  padding: 0.12rem 0.45rem;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 4px;
  color: #94a3b8;
}

.code-preview {
  margin: 0;
  padding: 1.1rem 1.25rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.88rem;
  color: #f8fafc;
  line-height: 1.6;
  white-space: pre-wrap;
  word-break: break-word;
  background: #0f172a;
}

/* Terminal Box */
.terminal-box {
  background: #090d16;
  border: 1px solid #1e293b;
  border-radius: 10px;
  overflow: hidden;
  margin-top: 0.85rem;
}

.terminal-header {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 0.45rem 0.8rem;
  background: #1e293b;
  border-bottom: 1px solid #334155;
}

.window-dots {
  display: flex;
  gap: 5px;
}

.dot {
  width: 9px;
  height: 9px;
  border-radius: 50%;
}
.dot.red { background: #ef4444; }
.dot.yellow { background: #f59e0b; }
.dot.green { background: #10b981; }

.term-title {
  font-size: 0.73rem;
  color: #94a3b8;
  font-family: var(--vp-font-family-mono, monospace);
  font-weight: 600;
}

.term-body {
  margin: 0;
  padding: 0.9rem 1.1rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.83rem;
  color: #34d399;
  line-height: 1.5;
  white-space: pre-wrap;
  word-break: break-word;
  background: #090d16;
}

@media (max-width: 640px) {
  .timeline-step {
    grid-template-columns: 32px 1fr;
    gap: 0.75rem;
  }
  .step-node {
    width: 28px;
    height: 28px;
    font-size: 0.82rem;
  }
}
</style>
