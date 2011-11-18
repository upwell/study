net = require('net');
NodeQueue = require('./nodequeue');

var configs = {
    conn: 500,
    trans: 10
};

var options = {
    host: "10.64.75.58",
    port: 80,
    path: "/",
    method: "GET",
};

var stat_trans = 0;

var requests = "GET " + options['path'] + " HTTP/1.1\r\n"
             + "Host: " + options['host'] + "\r\n\r\n";

var queue = new NodeQueue();
var sock_pool = [];

var start_time = getCurrentTime();

setInterval(printStat, 5000);

//main routing here
for(var i = 0, j = configs['conn']; i < j; i++)
    create_socket();
//    init_socket();

//for(var i = 0, j = configs['conn']; i < j; i++)
//    queue.enqueue(sock_pool.pop());

queue.on('ready', function()
{
//    if(queue.size() < configs['conn']) return;

    while(queue.size() > 0)
    {
        var sock = queue.dequeue();
        sock.write(requests);
        stat_trans++;
    }
});

function init_socket()
{
    var sock = new net.Socket();

    sock.connect(options['port'], options['host'], function() {
        sock_pool.push(sock);
    });

    sock.on('error', function(e) {
        queue.remove(sock);
        console.log('sock error', e);
        create_socket();
    });

    sock.on('end', function() {
        queue.remove(sock);
        create_socket();
    });

    sock.on('data', function(data) {
        parse_http_resp(data, sock);
    });
}

function create_socket()
{
    var sock = new net.Socket();

    sock.connect(options['port'], options['host'], function() {
        queue.enqueue(sock);
    });

    sock.on('error', function(e) {
        queue.remove(sock);
        console.log('sock error', e);
        create_socket();
    });

    sock.on('end', function() {
        queue.remove(sock);
        create_socket();
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
        queue.enqueue(sock);
    }
}

function getCurrentTime()
{
    return new Date().valueOf();
}

function printStat()
{
    var average_trans = stat_trans / ((getCurrentTime() - start_time) / 1000);

    console.log("Total Requests: ", stat_trans);
    console.log("Average Requests: ", average_trans);
}
