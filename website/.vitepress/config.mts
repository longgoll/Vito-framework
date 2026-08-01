import { defineConfig } from 'vitepress'

export default defineConfig({
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
          { text: 'Playground ⚡', link: '/playground' },
          { text: 'Hướng dẫn', link: '/guide/getting-started' },
          { text: 'Benchmark 🏆', link: '/guide/benchmarks' },
          { text: 'Routing', link: '/guide/routing' },
          { text: 'Middleware', link: '/guide/middleware' },
          { text: 'CSDL & ORM', link: '/guide/database-orm' },
          { text: 'Auth & Bảo Mật', link: '/guide/auth-security' },
          { text: 'Realtime & Jobs', link: '/guide/realtime-jobs' },
          { text: 'Metrics & DX', link: '/guide/observability-dx' },
          { text: 'CLI', link: '/guide/cli' },
          { text: 'API Reference', link: '/api/overview' }
        ],
        sidebar: [
          {
            text: '📖 Giới Thiệu',
            items: [
              { text: 'Bắt Đầu', link: '/guide/getting-started' },
              { text: 'Báo Cáo Benchmark 🏆', link: '/guide/benchmarks' },
              { text: 'Định Tuyến & Parameters', link: '/guide/routing' },
              { text: 'Hệ Thống Middleware', link: '/guide/middleware' },
              { text: 'Quản Lý CSDL & Vito ORM', link: '/guide/database-orm' },
              { text: 'Xác Thực & Bảo Mật (Auth)', link: '/guide/auth-security' },
              { text: 'Realtime & Job Queue ⚡', link: '/guide/realtime-jobs' },
              { text: 'Giám Sát Metrics & DX 📊', link: '/guide/observability-dx' },
              { text: 'CLI & Công Cụ Phát Triển', link: '/guide/cli' }
            ]
          },
          {
            text: '📦 Hệ Sinh Thái',
            items: [
              { text: 'Báo Cáo Benchmark 🏆', link: '/guide/benchmarks' },
              { text: 'Tổng Quan Middleware', link: '/guide/middleware' },
              { text: 'Database Pool & Vito ORM', link: '/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/guide/auth-security' },
              { text: 'Realtime WebSockets & Jobs', link: '/guide/realtime-jobs' },
              { text: 'Observability & DX Tooling', link: '/guide/observability-dx' },
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
          { text: 'Benchmark 🏆', link: '/en/guide/benchmarks' },
          { text: 'Routing', link: '/en/guide/routing' },
          { text: 'Middleware', link: '/en/guide/middleware' },
          { text: 'Database & ORM', link: '/en/guide/database-orm' },
          { text: 'Auth & Security', link: '/en/guide/auth-security' },
          { text: 'Realtime & Jobs', link: '/en/guide/realtime-jobs' },
          { text: 'Metrics & DX', link: '/en/guide/observability-dx' },
          { text: 'CLI', link: '/en/guide/cli' },
          { text: 'API Reference', link: '/en/api/overview' }
        ],
        sidebar: [
          {
            text: '📖 Introduction',
            items: [
              { text: 'Getting Started', link: '/en/guide/getting-started' },
              { text: 'Benchmark Report 🏆', link: '/en/guide/benchmarks' },
              { text: 'Routing & Parameters', link: '/en/guide/routing' },
              { text: 'Middleware System', link: '/en/guide/middleware' },
              { text: 'Database & Vito ORM', link: '/en/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/en/guide/auth-security' },
              { text: 'Realtime & Job Queue ⚡', link: '/en/guide/realtime-jobs' },
              { text: 'Observability & DX 📊', link: '/en/guide/observability-dx' },
              { text: 'CLI & Developer Tooling', link: '/en/guide/cli' }
            ]
          },
          {
            text: '📦 Ecosystem',
            items: [
              { text: 'Benchmark Report 🏆', link: '/en/guide/benchmarks' },
              { text: 'Official Middlewares', link: '/en/guide/middleware' },
              { text: 'Database Pool & Vito ORM', link: '/en/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/en/guide/auth-security' },
              { text: 'Realtime WebSockets & Jobs', link: '/en/guide/realtime-jobs' },
              { text: 'Observability & DX Tooling', link: '/en/guide/observability-dx' },
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
