var HOST = '10.64.75.194';
var PORT = 1344;
var URL = 'http://www.qq.com/';
var REQNUM = 1000000;

var net = require('net');

var count = 0;
var count_succeed = 0;

function sendIcapReq(conn, url)
{
    var reqStr = 'REQMOD icap://' + HOST + ':' + PORT +
            '/REQ-Service ICAP/1.0\r\n';
    reqStr += 'Host: ' + HOST + ':' + PORT + '\r\n';
    reqStr += 'Allow: 204\r\n';
    reqStr += 'X-Session-ID: 11111111111111111\r\n';
    reqStr += 'X-Subscriber-Type: 1\r\n';
    reqStr += 'X-Subscriber-Data: user_id_1\r\n';

    var httpStr = 'GET ' + url + ' HTTP/1.0\r\n\r\n';
    var len = httpStr.length;

    reqStr += 'Encapsulated: req-hdr=0, null-body=' + len + '\r\n\r\n';
    reqStr += httpStr;
    conn.write(reqStr);
}

var client = net.connect(PORT, HOST, function() {
    console.log('connected');

    while(count <= REQNUM)
    {
        sendIcapReq(client, URL);
        count++;
    }
});

var dataStr = "";

client.on('data', function(data) {
    var resp = data.toString();
    //console.log(resp);

    while((pos = resp.indexOf("200 OK")) != -1)
    {
        resp = resp.substring(pos+7);
        count_succeed++;
    }

    console.log(count_succeed);

    if(count_succeed >= REQNUM)
        client.end();
});

client.on('end', function() {
    console.log('succeed ' + count_succeed);
    console.log('closed');
});
