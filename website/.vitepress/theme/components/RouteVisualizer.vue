<template>
  <div class="route-visualizer">
    <div class="vis-header">
      <h4>⚡ Interactive Vito Router Testbed (Radix Trie Engine)</h4>
      <p class="subtitle">Thử gõ một URL bất kỳ bên dưới để trực quan hóa cách Vito Router giải mã và phân tích Route Param!</p>
    </div>

    <div class="preset-buttons">
      <span>Thử nhanh:</span>
      <button 
        v-for="p in presets" 
        :key="p.url"
        class="preset-btn"
        @click="testUrl = p.url"
      >
        {{ p.label }}
      </button>
    </div>

    <div class="input-group">
      <span class="method-tag">GET</span>
      <input 
        v-model="testUrl" 
        type="text" 
        class="url-input"
        placeholder="e.g. /users/42 or /products/laptop/macbook-m3"
      />
    </div>

    <div class="result-card">
      <div class="result-row">
        <span class="label">Matching Strategy:</span>
        <span :class="['badge', matchedResult.strategyClass]">
          {{ matchedResult.strategy }}
        </span>
      </div>

      <div class="result-row">
        <span class="label">Matched Route Pattern:</span>
        <span class="route-pattern">{{ matchedResult.pattern }}</span>
      </div>

      <div class="result-row align-start">
        <span class="label">Extracted Params (`req.param`):</span>
        <pre class="params-json"><code>{{ JSON.stringify(matchedResult.params, null, 2) }}</code></pre>
      </div>

      <div class="result-row align-start">
        <span class="label">Query Strings (`req.query`):</span>
        <pre class="params-json"><code>{{ JSON.stringify(matchedResult.query, null, 2) }}</code></pre>
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
  if (!fullUrl) return { strategy: 'N/A', pattern: 'None', params: {}, query: {}, strategyClass: 'gray' }

  // Extract path and query
  const [pathPart, queryPart] = fullUrl.split('?')
  
  // Parse query
  const query = {}
  if (queryPart) {
    const pairs = queryPart.split('&')
    for (const p of pairs) {
      const [k, v] = p.split('=')
      if (k) query[k] = decodeURIComponent(v || '')
    }
  }

  // Matching logic simulation
  if (pathPart === '/health' || pathPart === '/' || pathPart === '/api/v1/metrics') {
    return {
      strategy: '⚡ Static Fast Path O(1)',
      strategyClass: 'green',
      pattern: pathPart,
      params: {},
      query
    }
  }

  if (pathPart.startsWith('/users/')) {
    const id = pathPart.replace('/users/', '')
    return {
      strategy: '🌲 Radix Trie Dynamic Segment',
      strategyClass: 'purple',
      pattern: '/users/:id',
      params: { id },
      query
    }
  }

  if (pathPart.startsWith('/products/')) {
    const parts = pathPart.replace('/products/', '').split('/')
    return {
      strategy: '🌲 Radix Trie Multi Segment',
      strategyClass: 'purple',
      pattern: '/products/:category/:id',
      params: { category: parts[0] || '', id: parts[1] || '' },
      query
    }
  }

  if (pathPart.startsWith('/files/')) {
    const filepath = pathPart.replace('/files/', '')
    return {
      strategy: '🌐 Radix Trie Wildcard Path',
      strategyClass: 'blue',
      pattern: '/files/*filepath',
      params: { filepath },
      query
    }
  }

  return {
    strategy: '❌ 404 Not Found (Fallback)',
    strategyClass: 'red',
    pattern: 'Custom NotFoundHandler',
    params: {},
    query
  }
})
</script>

<style scoped>
.route-visualizer {
  background: var(--vp-c-bg-soft, #f8fafc);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 16px;
  padding: 1.5rem;
  margin: 2rem 0;
  box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.05);
}

.vis-header h4 {
  margin: 0 0 0.25rem 0;
  font-size: 1.1rem;
  font-weight: 700;
  color: var(--vp-c-text-1, #0f172a);
}

.subtitle {
  margin: 0 0 1rem 0;
  font-size: 0.88rem;
  color: var(--vp-c-text-2, #475569);
}

.preset-buttons {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  flex-wrap: wrap;
  margin-bottom: 1.25rem;
  font-size: 0.85rem;
  color: var(--vp-c-text-2, #475569);
}

.preset-btn {
  background: var(--vp-c-bg-alt, #ffffff);
  border: 1px solid var(--vp-c-divider, #cbd5e1);
  border-radius: 6px;
  padding: 0.3rem 0.7rem;
  color: var(--vp-c-text-1, #0f172a);
  font-size: 0.82rem;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
}

.preset-btn:hover {
  background: var(--vp-c-brand-soft, rgba(79, 70, 229, 0.1));
  border-color: var(--vp-c-brand-1, #4f46e5);
  color: var(--vp-c-brand-1, #4f46e5);
}

.input-group {
  display: flex;
  align-items: center;
  background: #0f172a;
  border: 1px solid #334155;
  border-radius: 10px;
  overflow: hidden;
  margin-bottom: 1.25rem;
}

.method-tag {
  background: #059669;
  color: #ffffff;
  font-family: var(--vp-font-family-mono, monospace);
  font-weight: 800;
  font-size: 0.85rem;
  padding: 0.75rem 1.2rem;
}

.url-input {
  flex: 1;
  background: transparent;
  border: none;
  padding: 0.75rem 1rem;
  color: #f8fafc;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.95rem;
  outline: none;
}

.result-card {
  background: var(--vp-c-bg-alt, #ffffff);
  border: 1px solid var(--vp-c-divider, #e2e8f0);
  border-radius: 12px;
  padding: 1.25rem;
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.result-row {
  display: flex;
  align-items: center;
  gap: 1rem;
  font-size: 0.9rem;
}

.result-row.align-start {
  align-items: flex-start;
}

.result-row .label {
  min-width: 220px;
  color: var(--vp-c-text-1, #0f172a);
  font-weight: 700;
}

.badge {
  padding: 0.25rem 0.75rem;
  border-radius: 6px;
  font-weight: 800;
  font-size: 0.82rem;
}

.badge.green { background: #d1fae5; color: #047857; }
.badge.purple { background: #f3e8ff; color: #6b21a8; }
.badge.blue { background: #dbeafe; color: #1e40af; }
.badge.red { background: #fee2e2; color: #b91c1c; }

.dark .badge.green { background: rgba(16, 185, 129, 0.2); color: #34d399; }
.dark .badge.purple { background: rgba(168, 85, 247, 0.2); color: #c084fc; }
.dark .badge.blue { background: rgba(59, 130, 246, 0.2); color: #60a5fa; }
.dark .badge.red { background: rgba(239, 68, 68, 0.2); color: #f87171; }

.route-pattern {
  font-family: var(--vp-font-family-mono, monospace);
  color: var(--vp-c-brand-1, #4f46e5);
  font-weight: 700;
}

.params-json {
  margin: 0;
  padding: 0.6rem 1rem;
  background: #0f172a;
  border-radius: 8px;
  font-family: var(--vp-font-family-mono, monospace);
  font-size: 0.85rem;
  color: #38bdf8;
  flex: 1;
}
</style>
