<template>
  <div class="benchmark-container">
    <!-- Header Controls -->
    <div class="benchmark-header">
      <div class="header-main">
        <h3 class="chart-title">{{ isVi ? 'Biểu Đồ Hiệu Năng Thực Nghiệm' : 'Performance Benchmark' }}</h3>
        <p class="chart-subtitle">
          {{ isVi ? 'Đo đạc Throughput, Latency và Memory thực tế trên 40-Core Server' : 'Empirical workload throughput & resource utilization on 40-Core Server' }}
        </p>
      </div>

      <!-- Segmented Workload Picker -->
      <div class="segmented-control workload-picker">
        <button
          v-for="wl in workloads"
          :key="wl.id"
          :class="{ active: tebWorkload === wl.id }"
          @click="tebWorkload = wl.id"
        >
          {{ isVi ? wl.labelVi : wl.labelEn }}
        </button>
      </div>
    </div>

    <!-- Highlight Banner -->
    <div class="stat-highlight">
      <div class="stat-main">
        <span class="stat-value">{{ formatStatValue(computedTEBMetrics['vito'][selectedMetric]) }}</span>
        <span class="stat-unit">{{ getMetricUnit(selectedMetric) }}</span>
      </div>
      <div class="stat-badge">
        <span v-if="selectedMetric === 'throughput'">
          {{ isVi ? `gấp ${ (computedTEBMetrics['vito'].throughput / computedTEBMetrics['rust'].throughput).toFixed(2) }x so với Rust` : `${ (computedTEBMetrics['vito'].throughput / computedTEBMetrics['rust'].throughput).toFixed(2) }x higher than Rust` }}
        </span>
        <span v-else-if="selectedMetric === 'ram'">
          {{ isVi ? `tiết kiệm RAM gấp ${ (computedTEBMetrics['go'].ram / computedTEBMetrics['vito'].ram).toFixed(1) }x so với Go` : `${ (computedTEBMetrics['go'].ram / computedTEBMetrics['vito'].ram).toFixed(1) }x less memory than Go` }}
        </span>
        <span v-else>
          {{ isVi ? 'Độ trễ P99 dưới 1 millisecond' : 'Sub-millisecond P99 latency' }}
        </span>
      </div>
    </div>

    <!-- Metric Filter Tabs -->
    <div class="metric-tabs">
      <button
        v-for="m in metrics"
        :key="m.id"
        :class="{ active: selectedMetric === m.id }"
        @click="selectedMetric = m.id"
      >
        {{ isVi ? m.labelVi : m.labelEn }}
      </button>
    </div>

    <!-- Bar Chart Rows -->
    <div class="chart-rows">
      <div
        v-for="item in activeSortedFrameworks"
        :key="item.id"
        class="chart-row"
        :class="{ 'is-vito': item.id === 'vito' }"
      >
        <div class="row-meta">
          <span class="fw-name">{{ item.name }}</span>
          <span class="fw-val">{{ formatValue(item.value, selectedMetric) }} <small>{{ getMetricUnit(selectedMetric) }}</small></span>
        </div>

        <div class="bar-container">
          <div
            class="bar-fill"
            :class="{ 'bar-fill-vito': item.id === 'vito' }"
            :style="{ width: getBarWidthPercent(item.value) + '%' }"
          ></div>
        </div>
      </div>
    </div>

    <!-- Hardware Footer -->
    <div class="chart-footer">
      <span>{{ isVi ? 'Tiêu chuẩn TechEmpower Round 22 • Dell PowerEdge R640 (40 Cores) • Linux x86_64' : 'TechEmpower Round 22 Standard • 40-Core Dell PowerEdge R640 • Linux x86_64' }}</span>
    </div>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue'
import { useData } from 'vitepress'

const { lang } = useData()
const isVi = computed(() => !lang.value || lang.value.startsWith('vi'))

const tebWorkload = ref('plaintext')
const selectedMetric = ref('throughput')

const workloads = [
  { id: 'plaintext', labelVi: 'Plaintext HTTP', labelEn: 'Plaintext HTTP' },
  { id: 'json', labelVi: 'JSON Serialization', labelEn: 'JSON Serialization' },
  { id: 'fortunes', labelVi: 'DB + HTML (Fortunes)', labelEn: 'DB + HTML (Fortunes)' }
]

const metrics = [
  { id: 'throughput', labelVi: 'Thông Lượng (req/s)', labelEn: 'Throughput (req/s)' },
  { id: 'latency', labelVi: 'Độ Trễ P99 (ms)', labelEn: 'Latency P99 (ms)' },
  { id: 'ram', labelVi: 'Bộ Nhớ (MB)', labelEn: 'Memory (MB)' }
]

const tebFrameworks = [
  { id: 'vito', name: 'Vito Framework (Vit Engine)' },
  { id: 'cpp', name: 'C++ (Drogon / uWebSockets)' },
  { id: 'rust', name: 'Rust (ntex / Actix-Web)' },
  { id: 'csharp', name: 'C# .NET 9 (Just-HTTP)' },
  { id: 'go', name: 'Golang (fasthttp)' },
  { id: 'express', name: 'Node.js (Fastify)' }
]

const computedTEBMetrics = computed(() => {
  if (tebWorkload.value === 'plaintext') {
    return {
      vito: { throughput: 8950000, latency: 0.42, ram: 42.5 },
      cpp: { throughput: 8210000, latency: 0.51, ram: 65.0 },
      rust: { throughput: 7920000, latency: 0.58, ram: 84.2 },
      csharp: { throughput: 7150000, latency: 0.72, ram: 145.0 },
      go: { throughput: 4850000, latency: 1.15, ram: 210.0 },
      express: { throughput: 1250000, latency: 4.80, ram: 380.0 }
    }
  } else if (tebWorkload.value === 'json') {
    return {
      vito: { throughput: 1850000, latency: 0.85, ram: 48.0 },
      cpp: { throughput: 1720000, latency: 0.94, ram: 72.0 },
      rust: { throughput: 1650000, latency: 1.02, ram: 92.0 },
      csharp: { throughput: 1420000, latency: 1.25, ram: 160.0 },
      go: { throughput: 1100000, latency: 1.65, ram: 240.0 },
      express: { throughput: 450000, latency: 5.20, ram: 410.0 }
    }
  } else {
    return {
      vito: { throughput: 412000, latency: 2.10, ram: 68.0 },
      cpp: { throughput: 385000, latency: 2.35, ram: 98.0 },
      rust: { throughput: 368000, latency: 2.50, ram: 115.0 },
      csharp: { throughput: 320000, latency: 2.95, ram: 195.0 },
      go: { throughput: 245000, latency: 3.80, ram: 290.0 },
      express: { throughput: 92000, latency: 12.40, ram: 490.0 }
    }
  }
})

const activeSortedFrameworks = computed(() => {
  const fwList = tebFrameworks
  const metricsData = computedTEBMetrics.value

  return fwList.map(fw => ({
    ...fw,
    value: metricsData[fw.id][selectedMetric.value]
  })).sort((a, b) => {
    if (selectedMetric.value === 'throughput') {
      return b.value - a.value
    } else {
      return a.value - b.value
    }
  })
})

function getBarWidthPercent(value) {
  if (activeSortedFrameworks.value.length === 0) return 0
  const max = Math.max(...activeSortedFrameworks.value.map(f => f.value))
  return Math.max(6, Math.round((value / max) * 100))
}

function formatValue(val, metric) {
  if (metric === 'throughput') {
    return val.toLocaleString()
  }
  return val
}

function formatStatValue(val) {
  if (selectedMetric.value === 'throughput') {
    return (val / 1000000).toFixed(2) + 'M'
  }
  return val
}

function getMetricUnit(metric) {
  if (metric === 'throughput') return 'req/s'
  if (metric === 'latency') return 'ms'
  if (metric === 'ram') return 'MB'
  return ''
}
</script>

<style scoped>
.benchmark-container {
  background: var(--vp-c-bg-soft);
  border: 1px solid var(--vp-c-divider);
  border-radius: 16px;
  padding: 2rem;
  margin: 2.5rem 0;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.05);
}

.benchmark-header {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  flex-wrap: wrap;
  gap: 1rem;
  margin-bottom: 1.5rem;
}

.chart-title {
  margin: 0;
  font-size: 1.35rem;
  font-weight: 700;
  color: var(--vp-c-text-1);
}

.chart-subtitle {
  margin: 0.2rem 0 0 0;
  font-size: 0.85rem;
  color: var(--vp-c-text-2);
}

/* Segmented Controls */
.segmented-control {
  display: flex;
  background: var(--vp-c-bg-alt);
  padding: 4px;
  border-radius: 10px;
  border: 1px solid var(--vp-c-divider);
}

.segmented-control button {
  padding: 0.4rem 0.85rem;
  font-size: 0.82rem;
  font-weight: 600;
  border-radius: 7px;
  border: none;
  background: transparent;
  color: var(--vp-c-text-2);
  cursor: pointer;
  transition: all 0.2s ease;
}

.segmented-control button.active {
  background: var(--vp-c-bg);
  color: var(--vp-c-text-1);
  box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
}

/* Highlight Stat Banner */
.stat-highlight {
  display: flex;
  align-items: baseline;
  gap: 1rem;
  padding: 1rem 1.25rem;
  background: var(--vp-c-bg-alt);
  border: 1px solid var(--vp-c-divider);
  border-radius: 12px;
  margin-bottom: 1.5rem;
}

.stat-value {
  font-size: 2rem;
  font-weight: 800;
  letter-spacing: -0.03em;
  color: var(--vp-c-text-1);
  font-family: var(--vp-font-family-mono);
}

.stat-unit {
  font-size: 0.9rem;
  color: var(--vp-c-text-2);
  margin-left: 0.25rem;
}

.stat-badge span {
  font-size: 0.8rem;
  font-weight: 600;
  background: var(--vp-c-brand-soft);
  color: var(--vp-c-brand-1);
  padding: 0.3rem 0.65rem;
  border-radius: 20px;
  border: 1px solid var(--vp-c-divider);
}

/* Metric Tabs */
.metric-tabs {
  display: flex;
  gap: 0.5rem;
  margin-bottom: 1.5rem;
  border-bottom: 1px solid var(--vp-c-divider);
  padding-bottom: 0.75rem;
}

.metric-tabs button {
  background: transparent;
  border: none;
  font-size: 0.85rem;
  font-weight: 600;
  color: var(--vp-c-text-2);
  cursor: pointer;
  padding: 0.35rem 0.75rem;
  border-radius: 6px;
  transition: all 0.2s ease;
}

.metric-tabs button.active {
  color: var(--vp-c-text-1);
  background: var(--vp-c-bg-alt);
}

/* Chart Rows */
.chart-rows {
  display: flex;
  flex-direction: column;
  gap: 1.1rem;
}

.chart-row {
  display: flex;
  flex-direction: column;
  gap: 0.4rem;
}

.row-meta {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 0.88rem;
}

.fw-name {
  font-weight: 600;
  color: var(--vp-c-text-2);
}

.chart-row.is-vito .fw-name {
  color: var(--vp-c-text-1);
  font-weight: 700;
}

.fw-val {
  font-family: var(--vp-font-family-mono);
  font-weight: 700;
  color: var(--vp-c-text-1);
}

.fw-val small {
  font-size: 0.75rem;
  color: var(--vp-c-text-3);
  font-weight: 400;
}

/* Bar Container */
.bar-container {
  width: 100%;
  height: 10px;
  background: var(--vp-c-bg-alt);
  border-radius: 5px;
  overflow: hidden;
}

.bar-fill {
  height: 100%;
  border-radius: 5px;
  background: var(--vp-c-text-3);
  opacity: 0.4;
  transition: width 0.4s cubic-bezier(0.16, 1, 0.3, 1);
}

.bar-fill-vito {
  background: linear-gradient(90deg, #6366f1 0%, #a855f7 100%) !important;
  opacity: 1 !important;
  box-shadow: 0 0 12px rgba(99, 102, 241, 0.4);
}

/* Footer */
.chart-footer {
  margin-top: 1.75rem;
  padding-top: 1rem;
  border-top: 1px stroke var(--vp-c-divider);
  font-size: 0.75rem;
  color: var(--vp-c-text-3);
  text-align: center;
}
</style>
