var net = require('net');

var server = net.createServer(function(c) {
    c.on('end', function() {
        console.log('server in end event');
    });

    c.on('data', function() {
        console.log('server received data');
    });

    console.log('close client : send FIN');
    c.end();
});

server.listen(8888);
