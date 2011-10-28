var net = require('net');

var server = net.createServer(function (socket) {
    socket.write("echo server\r\n");
    socket.pipe(socket);
});

server.listen(3120);
