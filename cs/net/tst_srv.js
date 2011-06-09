var net = require('net');

var socks = new Array();

function notifyAllClients(data)
{
    for(var i = 0; i < socks.length; i++)
    {
        socks[i].write(data + '> ');
    }
}

var srv = net.createServer(function(stream) {
    console.log('connection is accepted');
    stream.write("welcome\r\n");

    socks.push(stream); /* add to global socks array */

    stream.on('data', function(data) {
        notifyAllClients(data);
    });
}
);

srv.listen(3124, 'localhost');
