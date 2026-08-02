<template>
  <div class="code-comparison">
    <div class="comparison-header">
      <h3 class="comparison-title">💡 Cú Pháp Mạch Lạc - Hiệu Năng Vượt Trỗi</h3>
      <p class="subtitle">So sánh cú pháp Vito với Express, Gin và Actix-Web khi định tuyến API & gom nhóm route.</p>
      
      <!-- Responsive Wrapped Tab Buttons -->
      <div class="tabs-list">
        <button
          v-for="tab in tabs"
          :key="tab.id"
          :class="['tab-btn', { active: activeTab === tab.id }]"
          @click="activeTab = tab.id"
        >
          <span class="tab-icon">{{ tab.icon }}</span>
          <span class="tab-label">{{ tab.label }}</span>
        </button>
      </div>
    </div>

    <!-- Code Preview Card -->
    <div class="code-body">
      <div class="code-meta">
        <div class="lang-info">
          <span class="lang-tag">{{ currentTab.lang }}</span>
          <span class="fw-fullname">{{ currentTab.label }}</span>
        </div>
        <div class="perf-stats">
          <span class="stat-badge throughput">⚡ {{ currentTab.throughput }}</span>
          <span class="stat-badge ram">💾 {{ currentTab.ram }}</span>
        </div>
      </div>
      <pre class="code-block"><code>{{ currentTab.code }}</code></pre>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

const activeTab = ref('vito')

const tabs = [
  {
    id: 'vito',
    label: 'Vito (VIT Native)',
    icon: '🔴',
    lang: 'vit',
    throughput: '245,100 req/s',
    ram: '18.69 MB RAM (100k conns)',
    code: `import { createApp, Request, Response } from "vito/src/vito.vit";

function main(): number {
    let app = createApp();

    // 1. Dynamic path parameter
    app.get("/users/:id", (req: Request, res: Response) => {
        let userId = req.param("id");
        res.json("{\\"status\\":\\"ok\\", \\"user_id\\":\\"" + userId + "\\"}");
    });

    // 2. Modular Route Grouping
    app.group("/api/v1", (v1) => {
        v1.get("/products/:category", (req: Request, res: Response) => {
            let cat = req.param("category");
            res.json("{\\"category\\":\\"" + cat + "\\"}");
        });
    });

    app.listen(8888);
    return 0;
}`
  },
  {
    id: 'express',
    label: 'Node.js (Express)',
    icon: '🟨',
    lang: 'javascript',
    throughput: '38,500 req/s',
    ram: '320.10 MB RAM (100k conns)',
    code: `const express = require('express');
const app = express();

app.get('/users/:id', (req, res) => {
    res.json({ status: 'ok', user_id: req.params.id });
});

const v1 = express.Router();
v1.get('/products/:category', (req, res) => {
    res.json({ category: req.params.category });
});
app.use('/api/v1', v1);

app.listen(8888);`
  },
  {
    id: 'gin',
    label: 'Golang (Gin)',
    icon: '🔵',
    lang: 'go',
    throughput: '94,200 req/s',
    ram: '254.80 MB RAM (100k conns)',
    code: `package main

import "github.com/gin-gonic/gin"

func main() {
    r := gin.Default()

    r.GET("/users/:id", func(c *gin.Context) {
        c.JSON(200, gin.H{"status": "ok", "user_id": c.Param("id")})
    })

    v1 := r.Group("/api/v1")
    {
        v1.GET("/products/:category", func(c *gin.Context) {
            c.JSON(200, gin.H{"category": c.Param("category")})
        })
    }

    r.Run(":8888")
}`
  },
  {
    id: 'actix',
    label: 'Rust (Actix-Web)',
    icon: '🦀',
    lang: 'rust',
    throughput: '188,300 req/s',
    ram: '185.40 MB RAM (100k conns)',
    code: `use actix_web::{web, App, HttpServer, HttpResponse, Responder};

async fn get_user(path: web::Path<String>) -> impl Responder {
    let user_id = path.into_inner();
    HttpResponse::Ok().json(serde_json::json!({ "status": "ok", "user_id": user_id }))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .route("/users/{id}", web::get().to(get_user))
    })
    .bind(("127.0.0.1", 8888))?
    .run()
    .await
}`
  }
]

const currentTab = computed(() => {
  return tabs.find(t => t.id === activeTab.value) || tabs[0]
})
</script>

<style scoped>
.code-comparison {
  background: var(--vp-c-bg-soft, #f8fafc);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 16px;
  padding: 1.5rem;
  margin: 2.5rem 0;
  box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.05);
}

.comparison-title {
  margin: 0;
  font-size: 1.25rem;
  font-weight: 800;
  color: var(--vp-c-text-1, #0f172a);
}

.subtitle {
  margin: 0.25rem 0 1.25rem 0;
  font-size: 0.9rem;
  color: var(--vp-c-text-2, #475569);
}

/* Wrapped Tabs (No horizontal scrolling) */
.tabs-list {
  display: flex;
  flex-wrap: wrap;
  gap: 0.6rem;
  margin-bottom: 1.25rem;
}

.tab-btn {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.55rem 1rem;
  font-size: 0.88rem;
  font-weight: 700;
  border-radius: 10px;
  border: 1px solid var(--vp-c-divider, #cbd5e1);
  background: var(--vp-c-bg-alt, #ffffff);
  color: var(--vp-c-text-1, #0f172a);
  cursor: pointer;
  transition: all 0.2s ease;
}

.tab-btn:hover {
  background: var(--vp-c-brand-soft, rgba(99, 102, 241, 0.08));
  border-color: var(--vp-c-brand-1, #6366f1);
  transform: translateY(-1px);
}

.tab-btn.active {
  background: var(--vp-c-brand-1, #6366f1);
  border-color: var(--vp-c-brand-1, #6366f1);
  color: #ffffff;
  box-shadow: 0 4px 12px rgba(99, 102, 241, 0.25);
}

.code-body {
  background: #0f172a;
  border-radius: 12px;
  border: 1px solid #1e293b;
  overflow: hidden;
}

.code-meta {
  display: flex;
  justify-content: space-between;
  align-items: center;
  flex-wrap: wrap;
  gap: 0.5rem;
  padding: 0.65rem 1rem;
  background: #1e293b;
  border-bottom: 1px solid #334155;
}

.lang-info {
  display: flex;
  align-items: center;
  gap: 0.6rem;
}

.lang-tag {
  text-transform: uppercase;
  font-weight: 800;
  font-size: 0.75rem;
  letter-spacing: 0.05em;
  color: #38bdf8;
  background: rgba(56, 189, 248, 0.12);
  padding: 0.15rem 0.5rem;
  border-radius: 4px;
}

.fw-fullname {
  font-size: 0.85rem;
  font-weight: 700;
  color: #e2e8f0;
}

.perf-stats {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.stat-badge {
  font-size: 0.75rem;
  font-weight: 700;
  padding: 0.2rem 0.6rem;
  border-radius: 6px;
}

.stat-badge.throughput {
  background: rgba(16, 185, 129, 0.15);
  color: #34d399;
  border: 1px solid rgba(16, 185, 129, 0.3);
}

.stat-badge.ram {
  background: rgba(99, 102, 241, 0.15);
  color: #818cf8;
  border: 1px solid rgba(99, 102, 241, 0.3);
}

.code-block {
  margin: 0;
  padding: 1.25rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.9rem;
  line-height: 1.6;
  color: #f8fafc;
  background: #0f172a;
  white-space: pre-wrap;
  word-break: break-word;
}
</style>
