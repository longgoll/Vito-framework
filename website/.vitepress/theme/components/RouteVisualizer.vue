<template>
  <div class="route-testbed-container">
    <!-- Header Section -->
    <div class="testbed-header">
      <div class="header-title-group">
        <div class="header-icon">⚡</div>
        <div>
          <h3 class="header-title">Interactive Vito Router Testbed</h3>
          <p class="header-subtitle">Trực quan hóa thuật toán <strong>Radix Trie (O(1) Fast-Path)</strong> giải mã URL và phân tích Params real-time.</p>
        </div>
      </div>
      <span class="live-status-pill">
        <span class="pulse-dot"></span> Trie Engine Active
      </span>
    </div>

    <!-- Quick Presets -->
    <div class="preset-section">
      <span class="preset-label">Thử nhanh URL mẫu:</span>
      <div class="preset-chips">
        <button 
          v-for="p in presets" 
          :key="p.url"
          :class="['preset-chip', { active: testUrl === p.url }]"
          @click="testUrl = p.url"
        >
          <span class="chip-dot"></span>
          {{ p.label }}
        </button>
      </div>
    </div>

    <!-- Input Bar -->
    <div class="url-input-card">
      <div class="method-badge-glow">GET</div>
      <div class="input-wrapper">
        <input 
          v-model="testUrl" 
          type="text" 
          class="url-input-field"
          placeholder="Gõ URL (ví dụ: /users/108?tab=profile)..."
          spellcheck="false"
        />
        <button v-if="testUrl" class="clear-btn" title="Clear" @click="testUrl = ''">✕</button>
      </div>
    </div>

    <!-- Results Display Panel -->
    <div class="analysis-panel">
      <!-- Strategy & Pattern Grid -->
      <div class="meta-grid">
        <div class="meta-card">
          <span class="meta-label">Matching Strategy</span>
          <div :class="['strategy-badge', matchedResult.strategyClass]">
            <span class="badge-icon">{{ matchedResult.icon }}</span>
            <span>{{ matchedResult.strategy }}</span>
          </div>
        </div>

        <div class="meta-card">
          <span class="meta-label">Matched Route Pattern</span>
          <div class="pattern-code">
            <code>{{ matchedResult.pattern }}</code>
          </div>
        </div>
      </div>

      <!-- Params and Query Strings -->
      <div class="data-grid">
        <!-- Extracted Params -->
        <div class="data-card">
          <div class="data-card-header">
            <span class="data-title">Extracted Params (<code>req.param</code>)</span>
            <span class="data-count">{{ Object.keys(matchedResult.params).length }} keys</span>
          </div>
          <div class="data-body">
            <template v-if="Object.keys(matchedResult.params).length > 0">
              <div v-for="(val, key) in matchedResult.params" :key="key" class="kv-row">
                <span class="kv-key">:{{ key }}</span>
                <span class="kv-arrow">➔</span>
                <span class="kv-val">"{{ val }}"</span>
              </div>
            </template>
            <div v-else class="empty-state">Không có URL Params</div>
          </div>
        </div>

        <!-- Query Strings -->
        <div class="data-card">
          <div class="data-card-header">
            <span class="data-title">Query Strings (<code>req.query</code>)</span>
            <span class="data-count">{{ Object.keys(matchedResult.query).length }} keys</span>
          </div>
          <div class="data-body">
            <template v-if="Object.keys(matchedResult.query).length > 0">
              <div v-for="(val, key) in matchedResult.query" :key="key" class="kv-row">
                <span class="kv-key">{{ key }}</span>
                <span class="kv-arrow">=</span>
                <span class="kv-val">"{{ val }}"</span>
              </div>
            </template>
            <div v-else class="empty-state">Không có Query Parameters</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'

const testUrl = ref('/users/108?tab=profile&theme=dark')

const presets = [
  { label: '/health (Static)', url: '/health' },
  { label: '/users/108 (Param :id)', url: '/users/108?tab=profile' },
  { label: '/products/laptop/macbook-m3', url: '/products/laptop/macbook-m3' },
  { label: '/files/images/logo.png (Wildcard)', url: '/files/images/logo.png' }
]

const matchedResult = computed(() => {
  const fullUrl = testUrl.value.trim()
  if (!fullUrl) return { strategy: 'N/A', pattern: 'None', params: {}, query: {}, strategyClass: 'gray', icon: '❓' }

  const [pathPart, queryPart] = fullUrl.split('?')
  
  const query = {}
  if (queryPart) {
    const pairs = queryPart.split('&')
    for (const p of pairs) {
      const [k, v] = p.split('=')
      if (k) query[k] = decodeURIComponent(v || '')
    }
  }

  if (pathPart === '/health' || pathPart === '/' || pathPart === '/api/v1/metrics') {
    return {
      strategy: 'Static Fast Path O(1)',
      strategyClass: 'green',
      icon: '⚡',
      pattern: pathPart,
      params: {},
      query
    }
  }

  if (pathPart.startsWith('/users/')) {
    const id = pathPart.replace('/users/', '')
    return {
      strategy: 'Radix Trie Dynamic Segment',
      strategyClass: 'purple',
      icon: '🌲',
      pattern: '/users/:id',
      params: { id },
      query
    }
  }

  if (pathPart.startsWith('/products/')) {
    const parts = pathPart.replace('/products/', '').split('/')
    return {
      strategy: 'Radix Trie Multi Segment',
      strategyClass: 'purple',
      icon: '🌲',
      pattern: '/products/:category/:id',
      params: { category: parts[0] || '', id: parts[1] || '' },
      query
    }
  }

  if (pathPart.startsWith('/files/')) {
    const filepath = pathPart.replace('/files/', '')
    return {
      strategy: 'Radix Trie Wildcard Path',
      strategyClass: 'blue',
      icon: '🌐',
      pattern: '/files/*filepath',
      params: { filepath },
      query
    }
  }

  return {
    strategy: '404 Not Found (Fallback)',
    strategyClass: 'red',
    icon: '❌',
    pattern: 'Custom NotFoundHandler',
    params: {},
    query
  }
})
</script>

<style scoped>
.route-testbed-container {
  background: var(--vp-c-bg-soft, #f8fafc);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 16px;
  padding: 1.5rem;
  margin: 2rem 0;
  box-shadow: 0 12px 32px -8px rgba(0, 0, 0, 0.06);
  transition: all 0.25s ease;
}

.testbed-header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  margin-bottom: 1.25rem;
  gap: 1rem;
  flex-wrap: wrap;
}

.header-title-group {
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.header-icon {
  font-size: 1.5rem;
  background: var(--vp-c-brand-soft, rgba(99, 102, 241, 0.12));
  width: 42px;
  height: 42px;
  border-radius: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.header-title {
  margin: 0;
  font-size: 1.15rem;
  font-weight: 800;
  color: var(--vp-c-text-1);
}

.header-subtitle {
  margin: 0.2rem 0 0 0;
  font-size: 0.88rem;
  color: var(--vp-c-text-2);
}

.live-status-pill {
  display: inline-flex;
  align-items: center;
  gap: 0.4rem;
  background: rgba(16, 185, 129, 0.12);
  border: 1px solid rgba(16, 185, 129, 0.3);
  color: #10b981;
  font-size: 0.78rem;
  font-weight: 700;
  padding: 0.35rem 0.75rem;
  border-radius: 20px;
}

.pulse-dot {
  width: 8px;
  height: 8px;
  background: #10b981;
  border-radius: 50%;
  box-shadow: 0 0 8px #10b981;
}

.preset-section {
  margin-bottom: 1.25rem;
}

.preset-label {
  font-size: 0.82rem;
  font-weight: 700;
  color: var(--vp-c-text-2);
  display: block;
  margin-bottom: 0.5rem;
}

.preset-chips {
  display: flex;
  gap: 0.5rem;
  flex-wrap: wrap;
}

.preset-chip {
  display: inline-flex;
  align-items: center;
  gap: 0.4rem;
  background: var(--vp-c-bg-alt, #ffffff);
  border: 1px solid var(--vp-c-divider, #cbd5e1);
  border-radius: 8px;
  padding: 0.4rem 0.8rem;
  font-size: 0.82rem;
  font-weight: 600;
  color: var(--vp-c-text-1);
  cursor: pointer;
  transition: all 0.2s ease;
}

.chip-dot {
  width: 6px;
  height: 6px;
  border-radius: 50%;
  background: var(--vp-c-brand-1);
  opacity: 0.6;
}

.preset-chip:hover, .preset-chip.active {
  background: var(--vp-c-brand-soft);
  border-color: var(--vp-c-brand-1);
  color: var(--vp-c-brand-1);
}

.preset-chip.active .chip-dot {
  opacity: 1;
}

/* Input Card */
.url-input-card {
  display: flex;
  align-items: center;
  background: var(--vp-c-bg-alt, #ffffff);
  border: 2px solid var(--vp-c-brand-1, #6366f1);
  border-radius: 12px;
  overflow: hidden;
  box-shadow: 0 4px 14px rgba(99, 102, 241, 0.12);
  margin-bottom: 1.25rem;
}

.method-badge-glow {
  background: linear-gradient(135deg, #10b981 0%, #059669 100%);
  color: white;
  font-family: var(--vp-font-family-mono, monospace);
  font-weight: 800;
  font-size: 0.9rem;
  padding: 0.8rem 1.25rem;
  letter-spacing: 0.05em;
}

.input-wrapper {
  flex: 1;
  display: flex;
  align-items: center;
  padding: 0 1rem;
}

.url-input-field {
  width: 100%;
  background: transparent;
  border: none;
  color: var(--vp-c-text-1);
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.95rem;
  font-weight: 600;
  outline: none;
  padding: 0.75rem 0;
}

.clear-btn {
  background: none;
  border: none;
  color: var(--vp-c-text-3);
  font-size: 0.9rem;
  cursor: pointer;
  padding: 0.2rem 0.5rem;
}

.clear-btn:hover {
  color: var(--vp-c-text-1);
}

/* Analysis Panel */
.analysis-panel {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.meta-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
  gap: 1rem;
}

.meta-card {
  background: var(--vp-c-bg-alt, #ffffff);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 12px;
  padding: 1rem 1.25rem;
}

.meta-label {
  font-size: 0.78rem;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 0.05em;
  color: var(--vp-c-text-2);
  display: block;
  margin-bottom: 0.5rem;
}

.strategy-badge {
  display: inline-flex;
  align-items: center;
  gap: 0.4rem;
  padding: 0.4rem 0.8rem;
  border-radius: 8px;
  font-weight: 700;
  font-size: 0.85rem;
}

.strategy-badge.green { background: rgba(16, 185, 129, 0.15); color: #047857; }
.strategy-badge.purple { background: rgba(168, 85, 247, 0.15); color: #7e22ce; }
.strategy-badge.blue { background: rgba(59, 130, 246, 0.15); color: #1d4ed8; }
.strategy-badge.red { background: rgba(239, 68, 68, 0.15); color: #b91c1c; }

.dark .strategy-badge.green { color: #34d399; }
.dark .strategy-badge.purple { color: #c084fc; }
.dark .strategy-badge.blue { color: #60a5fa; }
.dark .strategy-badge.red { color: #f87171; }

.pattern-code code {
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.95rem;
  font-weight: 700;
  color: var(--vp-c-brand-1, #6366f1);
  background: var(--vp-c-brand-soft);
  padding: 0.3rem 0.7rem;
  border-radius: 6px;
}

/* Data Cards */
.data-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 1rem;
}

.data-card {
  background: var(--vp-c-bg-alt, #ffffff);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 12px;
  overflow: hidden;
}

.data-card-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: var(--vp-c-bg-soft);
  padding: 0.6rem 1rem;
  border-bottom: 1px solid var(--vp-c-divider);
}

.data-title {
  font-size: 0.82rem;
  font-weight: 700;
  color: var(--vp-c-text-1);
}

.data-count {
  font-size: 0.75rem;
  color: var(--vp-c-text-2);
  font-family: var(--vp-font-family-mono);
}

.data-body {
  padding: 0.8rem 1rem;
  min-height: 70px;
  display: flex;
  flex-direction: column;
  justify-content: center;
}

.kv-row {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.85rem;
  padding: 0.3rem 0;
  border-bottom: 1px dashed var(--vp-c-divider);
}

.kv-row:last-child {
  border-bottom: none;
}

.kv-key {
  color: #ec4899;
  font-weight: 700;
}

.kv-arrow {
  color: var(--vp-c-text-3);
  font-size: 0.75rem;
}

.kv-val {
  color: #10b981;
  font-weight: 600;
}

.dark .kv-key { color: #f472b6; }
.dark .kv-val { color: #34d399; }

.empty-state {
  font-size: 0.82rem;
  color: var(--vp-c-text-3);
  font-style: italic;
  text-align: center;
}
</style>
