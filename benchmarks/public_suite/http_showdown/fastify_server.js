const fastify = require('fastify')({ logger: false });
const port = 8083;

fastify.get('/json', async (request, reply) => {
    return { message: 'Hello, World!' };
});

fastify.listen({ port, host: '0.0.0.0' }, (err, address) => {
    if (err) {
        console.error(err);
        process.exit(1);
    }
    console.log(`Fastify server listening on ${address}`);
});
