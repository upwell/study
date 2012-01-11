var net = require('net');
var end_ptn = '\r\n\r\n';
var response = 'ICAP/1.0 204 No Content\r\nX-Result: OK\r\n\r\n';

var server = net.createServer(function(c) {
    console.log('connected');
    var buff = "";
    c.on('data', function(data) {
        buff = buff + data;
        if((pos = buff.search(end_ptn)) > 0)
        {
            console.log('received all request');
            c.write(response);
            buff = buff.substring(pos+4);
        }
    });
    
    c.on('end', function() {
        console.log('connection closed');
    });
});

server.listen(1344);
