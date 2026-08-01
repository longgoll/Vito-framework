FROM node:22-slim

WORKDIR /app

COPY packages /app/packages
COPY std /app/std
COPY benchmarks /app/benchmarks

EXPOSE 8080

CMD ["node", "benchmarks/cluster_server.js"]
