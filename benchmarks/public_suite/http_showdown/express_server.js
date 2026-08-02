const express = require('express');
const app = express();
const port = 8082;

app.get('/json', (req, res) => {
    res.json({ message: 'Hello, World!' });
});

app.listen(port, () => {
    console.log(`Express server listening on port ${port}`);
});
