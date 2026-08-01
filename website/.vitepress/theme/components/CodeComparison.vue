<template>
  <div class="code-comparison">
    <div class="comparison-header">
      <h3>💡 Expressive Syntax Across Ecosystems</h3>
      <p class="subtitle">See how Vito maintains the clean simplicity of Express while outperforming C++ and Rust.</p>
      
      <div class="tabs-list">
        <button
          v-for="tab in tabs"
          :key="tab.id"
          :class="{ active: activeTab === tab.id }"
          @click="activeTab = tab.id"
        >
          <span>{{ tab.icon }}</span>
          <span>{{ tab.label }}</span>
          <span v-if="tab.badge" class="tab-badge">{{ tab.badge }}</span>
        </button>
      </div>
    </div>

    <div class="code-body">
      <div class="code-meta">
        <div class="lang-tag">{{ currentTab.lang }}</div>
        <div class="perf-stat">
          <span>⚡ {{ currentTab.throughput }}</span> | <span>💾 {{ currentTab.ram }}</span>
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
    badge: '245K req/s • 18MB RAM',
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
    badge: '38K req/s • 320MB RAM',
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
    badge: '94K req/s • 254MB RAM',
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
    badge: '188K req/s • 185MB RAM',
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

.comparison-header h3 {
  margin: 0;
  font-size: 1.25rem;
  font-weight: 700;
  color: var(--vp-c-text-1, #0f172a);
}

.subtitle {
  margin: 0.25rem 0 1rem 0;
  font-size: 0.88rem;
  color: var(--vp-c-text-2, #475569);
}

.tabs-list {
  display: flex;
  gap: 0.5rem;
  overflow-x: auto;
  padding-bottom: 0.5rem;
  margin-bottom: 1rem;
  border-bottom: 1px solid var(--vp-c-divider, #cbd5e1);
}

.tabs-list button {
  display: flex;
  align-items: center;
  gap: 0.4rem;
  padding: 0.55rem 0.9rem;
  font-size: 0.85rem;
  font-weight: 600;
  border-radius: 8px;
  border: 1px solid var(--vp-c-divider, #cbd5e1);
  background: var(--vp-c-bg-alt, #ffffff);
  color: var(--vp-c-text-1, #0f172a);
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.2s ease;
}

.tabs-list button:hover {
  background: var(--vp-c-brand-soft, rgba(79, 70, 229, 0.1));
  border-color: var(--vp-c-brand-1, #4f46e5);
}

.tabs-list button.active {
  background: #4f46e5;
  border-color: #4f46e5;
  color: #ffffff;
}

.tab-badge {
  font-size: 0.72rem;
  background: rgba(255, 255, 255, 0.2);
  padding: 0.15rem 0.45rem;
  border-radius: 4px;
  color: inherit;
}

.code-body {
  background: #0f172a;
  border-radius: 10px;
  border: 1px solid #334155;
  overflow: hidden;
}

.code-meta {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0.5rem 1rem;
  background: #1e293b;
  font-size: 0.8rem;
  color: #94a3b8;
  border-bottom: 1px solid #334155;
}

.lang-tag {
  text-transform: uppercase;
  font-weight: 800;
  letter-spacing: 0.5px;
  color: #38bdf8;
}

.perf-stat span {
  font-weight: 700;
  color: #34d399;
}

.code-block {
  margin: 0;
  padding: 1.25rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.9rem;
  line-height: 1.6;
  color: #f8fafc;
  overflow-x: auto;
  background: #0f172a;
}
</style>
