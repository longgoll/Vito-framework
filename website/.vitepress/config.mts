import { defineConfig } from 'vitepress'

export default defineConfig({
  base: process.env.GITHUB_ACTIONS ? '/Vito-farrmwork/' : '/',
  title: "Vito Framework",
  description: "High-performance, lightweight, expressive Web Server Framework for the VIT Language ecosystem.",
  head: [
    ['link', { rel: 'icon', type: 'image/svg+xml', href: '/logo.svg' }]
  ],
  locales: {
    root: {
      label: 'Tiếng Việt',
      lang: 'vi-VN',
      title: "Vito Framework",
      description: "Web Server Framework hiệu năng cao cho hệ sinh thái ngôn ngữ VIT.",
      themeConfig: {
        logo: '/logo.svg',
        nav: [
          { text: 'Trang chủ', link: '/' },
          { text: 'Hướng dẫn', link: '/guide/getting-started' },
          {
            text: 'Tính Năng & Mô-đun',
            items: [
              { text: '🎯 Định Tuyến (Routing)', link: '/guide/routing' },
              { text: '🧱 Hệ Thống Middleware', link: '/guide/middleware' },
              { text: '🗄️ CSDL & Vito ORM', link: '/guide/database-orm' },
              { text: '🔒 Xác Thực & Bảo Mật', link: '/guide/auth-security' },
              { text: '⚡ Realtime & Jobs', link: '/guide/realtime-jobs' },
              { text: '📊 Metrics & DX', link: '/guide/observability-dx' },
              { text: '🛠️ Bộ Công Cụ CLI', link: '/guide/cli' }
            ]
          },
          { text: 'Benchmark', link: '/guide/benchmarks' },
          { text: 'Playground', link: '/playground' },
          { text: 'API Reference', link: '/api/overview' }
        ],
        sidebar: [
          {
            text: '🚀 Bắt Đầu (Getting Started)',
            items: [
              { text: 'Khởi Đầu Siêu Tốc (Quick Start)', link: '/guide/getting-started' },
              { text: 'Báo Cáo Benchmark Hiệu Năng', link: '/guide/benchmarks' }
            ]
          },
          {
            text: '🧱 Cốt Lõi Framework (Core)',
            items: [
              { text: 'Định Tuyến & Route Params', link: '/guide/routing' },
              { text: 'Hệ Thống Middleware', link: '/guide/middleware' },
              { text: 'CLI & Công Cụ Phát Triển', link: '/guide/cli' }
            ]
          },
          {
            text: '🗄️ Dữ Liệu & Bảo Mật (Data & Auth)',
            items: [
              { text: 'Quản Lý CSDL & Vito ORM', link: '/guide/database-orm' },
              { text: 'Xác Thực & Bảo Mật (Auth)', link: '/guide/auth-security' }
            ]
          },
          {
            text: '⚡ Tính Năng Nâng Cao (Advanced)',
            items: [
              { text: 'Realtime WebSockets & Jobs', link: '/guide/realtime-jobs' },
              { text: 'Giám Sát Metrics & DX', link: '/guide/observability-dx' },
              { text: 'Tra Cứu API Reference', link: '/api/overview' }
            ]
          }
        ]
      }
    },
    en: {
      label: 'English',
      lang: 'en-US',
      link: '/en/',
      title: "Vito Framework",
      description: "High-performance, lightweight, expressive Web Server Framework for the VIT Language ecosystem.",
      themeConfig: {
        logo: '/logo.svg',
        nav: [
          { text: 'Home', link: '/en/' },
          { text: 'Guide', link: '/en/guide/getting-started' },
          {
            text: 'Features & Modules',
            items: [
              { text: '🎯 Routing System', link: '/en/guide/routing' },
              { text: '🧱 Middleware System', link: '/en/guide/middleware' },
              { text: '🗄️ Database & Vito ORM', link: '/en/guide/database-orm' },
              { text: '🔒 Auth & Security', link: '/en/guide/auth-security' },
              { text: '⚡ Realtime & Job Queue', link: '/en/guide/realtime-jobs' },
              { text: '📊 Metrics & DX Observability', link: '/en/guide/observability-dx' },
              { text: '🛠️ CLI Tooling', link: '/en/guide/cli' }
            ]
          },
          { text: 'Benchmark', link: '/en/guide/benchmarks' },
          { text: 'Playground', link: '/playground' },
          { text: 'API Reference', link: '/en/api/overview' }
        ],
        sidebar: [
          {
            text: '🚀 Getting Started',
            items: [
              { text: 'Quick Start Guide', link: '/en/guide/getting-started' },
              { text: 'Performance Benchmark', link: '/en/guide/benchmarks' }
            ]
          },
          {
            text: '🧱 Framework Core',
            items: [
              { text: 'Routing & Parameters', link: '/en/guide/routing' },
              { text: 'Middleware System', link: '/en/guide/middleware' },
              { text: 'CLI & Dev Tools', link: '/en/guide/cli' }
            ]
          },
          {
            text: '🗄️ Data & Security',
            items: [
              { text: 'Database & Vito ORM', link: '/en/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/en/guide/auth-security' }
            ]
          },
          {
            text: '⚡ Advanced Features',
            items: [
              { text: 'Realtime WebSockets & Jobs', link: '/en/guide/realtime-jobs' },
              { text: 'Observability & DX', link: '/en/guide/observability-dx' },
              { text: 'API Reference', link: '/en/api/overview' }
            ]
          }
        ]
      }
    }
  },
  themeConfig: {
    socialLinks: [
      { icon: 'github', link: 'https://github.com/vit-lang/vito' }
    ],
    search: {
      provider: 'local'
    },
    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2026 VIT Language Community'
    }
  }
})
