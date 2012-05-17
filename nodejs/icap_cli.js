var HOST = '10.64.75.194';
var PORT = 1344;
var URL = 'http://www.qq.com/';
var REQNUM = 1;

var net = require('net');

var count = 0;
var count_succeed = 0;

function sendIcapReq(conn, url)
{
    conn.write('REQMOD icap://' + HOST + ':' + PORT +
            '/REQ-Service ICAP/1.0\r\n');
    conn.write('Host: ' + HOST + ':' + PORT + '\r\n');
    conn.write('Allow: 204\r\n');
    conn.write('X-Session-ID: 11111111111111111\r\n');
    conn.write('X-Subscriber-Type: 1\r\n');
    conn.write('X-Subscriber-Data: user_id_1\r\n');

    var httpStr = 'GET ' + url + ' HTTP/1.0\r\n\r\n';
    var len = httpStr.length;

    conn.write('Encapsulated: req-hdr=0, null-body=' + len + '\r\n');
    conn.write('\r\n');
    conn.write(httpStr);
}

var client = net.connect(PORT, HOST, function() {
    console.log('connected');
    sendIcapReq(client, URL);
    count++;
});


client.on('data', function(data) {
    var resp = data.toString();
    console.log(resp);
    if(resp.indexOf("200 OK") != -1 &&
        resp.indexOf("X-Category-ID: 51") != -1)
        count_succeed++;

    if(count >= REQNUM)
        client.end();

    sendIcapReq(client, URL);
    count++;
});

client.on('end', function() {
    console.log('succeed ' + count_succeed);
    console.log('closed');
});
