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
          { text: 'Hướng dẫn', link: '/guide/getting-started' },
          { text: 'Routing', link: '/guide/routing' },
          { text: 'Middleware', link: '/guide/middleware' },
          { text: 'CLI', link: '/guide/cli' },
          { text: 'API Reference', link: '/api/overview' }
        ],
        sidebar: [
          {
            text: '📖 Giới Thiệu',
            items: [
              { text: 'Bắt Đầu', link: '/guide/getting-started' },
              { text: 'Định Tuyến & Parameters', link: '/guide/routing' },
              { text: 'Hệ Thống Middleware', link: '/guide/middleware' },
              { text: 'CLI & Công Cụ Phát Triển', link: '/guide/cli' }
            ]
          },
          {
            text: '📦 Hệ Sinh Thái',
            items: [
              { text: 'Tổng Quan Middleware', link: '/guide/middleware' },
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
          { text: 'Routing', link: '/en/guide/routing' },
          { text: 'Middleware', link: '/en/guide/middleware' },
          { text: 'CLI', link: '/en/guide/cli' },
          { text: 'API Reference', link: '/en/api/overview' }
        ],
        sidebar: [
          {
            text: '📖 Introduction',
            items: [
              { text: 'Getting Started', link: '/en/guide/getting-started' },
              { text: 'Routing & Parameters', link: '/en/guide/routing' },
              { text: 'Middleware System', link: '/en/guide/middleware' },
              { text: 'CLI & Developer Tooling', link: '/en/guide/cli' }
            ]
          },
          {
            text: '📦 Ecosystem',
            items: [
              { text: 'Official Middlewares', link: '/en/guide/middleware' },
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
