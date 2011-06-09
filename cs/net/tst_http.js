// node js server exmple

var http = require('http');

http.createServer(function(req, res) {
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end('Hello World\n');
    }
    ).listen(8088, '10.64.75.55');

console.log('Server running at http://10.64.75.55:8088');
