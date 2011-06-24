// node js server exmple

var http = require('http');

http.createServer(function(req, res) {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end('Hello World\n');
    }
    ).listen(8088, 'localhost');

console.log('Server running at http://localhost:8088');
