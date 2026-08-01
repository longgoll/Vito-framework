import DefaultTheme from 'vitepress/theme'
import BenchmarkVisualizer from './components/BenchmarkVisualizer.vue'
import CodeComparison from './components/CodeComparison.vue'
import './custom.css'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }: { app: any }) {
    app.component('BenchmarkVisualizer', BenchmarkVisualizer)
    app.component('CodeComparison', CodeComparison)
  }
}
