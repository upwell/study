net = require('net');
NodeQueue = require('./nodequeue');

var configs = {
    conn: 10,
    trans: 10
};

var options = {
    host: "localhost",
    port: 81,
    path: "/proxy.pac",
    method: "GET",
};

var stat_trans = 0;

var requests = "GET " + options['path'] + " HTTP/1.1\r\n"
             + "Host: " + options['host'] + "\r\n\r\n";

var queue = new NodeQueue();

//main routing here
for(var i = 0; i < configs['conn']; i++)
    create_socket();

queue.on('ready', function()
{
    if(queue.size() < configs['conn']) return;

    while(queue.size() > 0)
    {
        var sock = queue.dequeue();
        sock.write(requests);
        stat_trans++;
    }
});


function move_in_to_out()
{
    while(in_pool.length > 0)
    {
        out_pool.push(in_pool.pop());
    }
}

function create_socket()
{
    var sock = new net.Socket();

    sock.connect(options['port'], options['host'], function() {
        queue.enqueue(sock);
    });

    sock.on('error', function(e) {
        console.log('sock error', e);
        queue.remove(sock);
    });

    sock.on('end', function() {
        queue.remove(sock);
    });

    sock.on('data', function(data) {
        parse_http_resp(data, sock);
    });
}

function parse_http_resp(data, sock)
{
    var first_line;
    var headers;
    var content_len;
    var data_len;
    var resp = data.toString();

    // seek first line
    var index = resp.indexOf('\r\n');
    first_line = resp.substring(0, index-1);
    resp = resp.substring(index+2);

    index = resp.indexOf('\r\n\r\n');
    headers = resp.substring(0, index-1);
    resp = resp.substring(index+4);

    index = headers.indexOf('Content-Length');
    data_len = headers.substring(index+16);
    data_len = data_len.substring(0, data_len.indexOf('\r\n'));

    if(data_len == resp.length)
    {
        sock.end();
        create_socket();
    }
}

function rem_from_array(array, ele)
{
    return array.filter(function(x) { return x != ele });
}
