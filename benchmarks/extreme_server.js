const http = require('http');

const server = http.createServer({ keepAlive: true }, (req, res) => {
  if (req.url === '/plaintext') {
    res.writeHead(200, { 'Content-Type': 'text/plain', 'Server': 'Vito-Extreme' });
    res.end('Hello, World!');
  } else if (req.url === '/json') {
    res.writeHead(200, { 'Content-Type': 'application/json', 'Server': 'Vito-Extreme' });
    res.end(JSON.stringify({ message: 'Hello, World!' }));
  } else {
    res.writeHead(404);
    res.end();
  }
});

server.listen(8080, '127.0.0.1', () => {
  console.log('Vito Extreme Engine Server started on port 8080');
});
