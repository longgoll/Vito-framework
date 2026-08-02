import { Hono } from 'hono';
import { serve } from '@hono/node-server';

const app = new Hono();

app.get('/json', (c) => {
    return c.json({ message: 'Hello, World!' });
});

serve({
    fetch: app.fetch,
    port: 8084,
}, (info) => {
    console.log(`Hono server listening on http://localhost:${info.port}`);
});
