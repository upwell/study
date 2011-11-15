var io = require('socket.io');
var express = require('express');

var app = express.createServer(),
    io = io.listen(app);

app.listen(80);

app.get('/', function(req, res) {
    res.sendfile(__dirname + '/index.html');
});

io.sockets.on('connection', function(sock) {
    sock.emit('news', {hello: "world"});
    sock.on('my other event', function(data) {
        console.log(data);
        console.log('hello');
    });
});
