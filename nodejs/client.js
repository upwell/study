var net = require('net');
var sock = net.createConnection(3120);

function createConn()
{
    var sock = net.createConnection(3120);
    sock.end();
}

setInterval(createConn, 200);
