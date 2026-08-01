const cluster = require('cluster');
const http = require('http');
const numCPUs = require('os').cpus().length;

if (cluster.isPrimary) {
  console.log(`Master cluster setting up ${numCPUs} CPU cores...`);
  for (let i = 0; i < numCPUs; i++) {
    cluster.fork();
  }
} else {
  http.createServer({ keepAlive: true }, (req, res) => {
    if (req.url === '/plaintext') {
      res.writeHead(200, { 'Content-Type': 'text/plain', 'Connection': 'keep-alive' });
      res.end('Hello, World!');
    } else {
      res.writeHead(404);
      res.end();
    }
  }).listen(8080);
}
