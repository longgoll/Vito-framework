import DefaultTheme from 'vitepress/theme'
import BenchmarkVisualizer from './components/BenchmarkVisualizer.vue'
import CodeComparison from './components/CodeComparison.vue'
import QuickStartFlow from './components/QuickStartFlow.vue'
import RouteVisualizer from './components/RouteVisualizer.vue'
import './custom.css'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }: { app: any }) {
    app.component('BenchmarkVisualizer', BenchmarkVisualizer)
    app.component('CodeComparison', CodeComparison)
    app.component('QuickStartFlow', QuickStartFlow)
    app.component('RouteVisualizer', RouteVisualizer)
  }
}
