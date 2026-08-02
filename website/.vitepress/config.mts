import { defineConfig } from 'vitepress'

const getBase = () => {
  if (process.env.GITHUB_REPOSITORY) {
    const repoName = process.env.GITHUB_REPOSITORY.split('/')[1]
    return `/${repoName}/`
  }
  return process.env.GITHUB_ACTIONS ? '/Vito-farrmwork/' : '/'
}

export default defineConfig({
  base: getBase(),
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
          { text: 'Ngôn Ngữ Vit', link: '/guide/vit-language' },
          { text: 'Vito Framework', link: '/guide/getting-started' },
          { text: 'Thư Viện Chuẩn (STD)', link: '/guide/std-packages' },
          {
            text: 'Tính Năng & Mô-đun',
            items: [
              { text: 'Ngôn Ngữ Vit', link: '/guide/vit-language' },
              { text: 'Thư Viện Chuẩn & Packages', link: '/guide/std-packages' },
              { text: 'Định Tuyến (Routing)', link: '/guide/routing' },
              { text: 'Hệ Thống Middleware', link: '/guide/middleware' },
              { text: 'Vito ORM & CSDL', link: '/guide/database-orm' },
              { text: 'Xác Thực & Bảo Mật', link: '/guide/auth-security' },
              { text: 'Realtime & Jobs', link: '/guide/realtime-jobs' },
              { text: 'Metrics & Observability', link: '/guide/observability-dx' },
              { text: 'Bộ Công Cụ CLI', link: '/guide/cli' }
            ]
          },
          { text: 'Benchmark', link: '/guide/benchmarks' },
          { text: 'Playground', link: '/playground' },
          { text: 'API Reference', link: '/api/overview' }
        ],
        sidebar: [
          {
            text: '📘 Ngôn Ngữ Vit',
            items: [
              { text: 'Hướng Dẫn Ngôn Ngữ Vit', link: '/guide/vit-language' },
              { text: 'Thư Viện Chuẩn & Packages', link: '/guide/std-packages' }
            ]
          },
          {
            text: '⚡ Vito Framework',
            items: [
              { text: 'Khởi Đầu Siêu Tốc', link: '/guide/getting-started' },
              { text: 'Benchmark Hiệu Năng', link: '/guide/benchmarks' }
            ]
          },
          {
            text: 'Cốt Lõi Framework',
            items: [
              { text: 'Định Tuyến & Route Params', link: '/guide/routing' },
              { text: 'Hệ Thống Middleware', link: '/guide/middleware' },
              { text: 'Vito CLI & Dev Tools', link: '/guide/cli' }
            ]
          },
          {
            text: 'Dữ Liệu & Bảo Mật',
            items: [
              { text: 'Vito ORM & CSDL', link: '/guide/database-orm' },
              { text: 'Xác Thực & Bảo Mật (Auth)', link: '/guide/auth-security' }
            ]
          },
          {
            text: 'Tính Năng Nâng Cao',
            items: [
              { text: 'Realtime WebSockets & Jobs', link: '/guide/realtime-jobs' },
              { text: 'Metrics & Observability', link: '/guide/observability-dx' }
            ]
          },
          {
            text: 'API Reference',
            items: [
              { text: 'Tổng Quan API', link: '/api/overview' }
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
          { text: 'Vit Language', link: '/en/guide/vit-language' },
          { text: 'Vito Framework', link: '/en/guide/getting-started' },
          { text: 'Standard Library (STD)', link: '/en/guide/std-packages' },
          {
            text: 'Features & Modules',
            items: [
              { text: 'Vit Language Guide', link: '/en/guide/vit-language' },
              { text: 'Standard Library & Packages', link: '/en/guide/std-packages' },
              { text: 'Routing System', link: '/en/guide/routing' },
              { text: 'Middleware System', link: '/en/guide/middleware' },
              { text: 'Database & Vito ORM', link: '/en/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/en/guide/auth-security' },
              { text: 'Realtime & Job Queue', link: '/en/guide/realtime-jobs' },
              { text: 'Metrics & Observability', link: '/en/guide/observability-dx' },
              { text: 'CLI Tooling', link: '/en/guide/cli' }
            ]
          },
          { text: 'Benchmark', link: '/en/guide/benchmarks' },
          { text: 'Playground', link: '/playground' },
          { text: 'API Reference', link: '/en/api/overview' }
        ],
        sidebar: [
          {
            text: '📘 Vit Language',
            items: [
              { text: 'Vit Language Guide', link: '/en/guide/vit-language' },
              { text: 'Standard Library & Packages', link: '/en/guide/std-packages' }
            ]
          },
          {
            text: '⚡ Vito Framework',
            items: [
              { text: 'Quick Start Guide', link: '/en/guide/getting-started' },
              { text: 'Performance Benchmarks', link: '/en/guide/benchmarks' }
            ]
          },
          {
            text: 'Core Framework',
            items: [
              { text: 'Routing & Parameters', link: '/en/guide/routing' },
              { text: 'Middleware System', link: '/en/guide/middleware' },
              { text: 'Vito CLI Tooling', link: '/en/guide/cli' }
            ]
          },
          {
            text: 'Data & Security',
            items: [
              { text: 'Vito ORM & Database', link: '/en/guide/database-orm' },
              { text: 'Auth & Security Suite', link: '/en/guide/auth-security' }
            ]
          },
          {
            text: 'Advanced Features',
            items: [
              { text: 'WebSockets & Job Queue', link: '/en/guide/realtime-jobs' },
              { text: 'Metrics & Observability', link: '/en/guide/observability-dx' }
            ]
          },
          {
            text: 'API Reference',
            items: [
              { text: 'API Overview', link: '/en/api/overview' }
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
