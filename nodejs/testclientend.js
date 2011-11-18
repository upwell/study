var net = require('net');

var options = {
    host: 'localhost',
    port: 8888
};

function create_socket()
{
    var sock = new net.Socket();

    sock.connect(options['port'], options['host'], function() {
        console.log('connected');
    });

    sock.on('error', function(e) {
        console.log('sock error', e);
    });

    sock.on('end', function() {
        console.log('sock end: FIN received');
        sock.end();
    });

    sock.on('data', function(data) {
        console.log(data.toString());
    });
}

create_socket()
