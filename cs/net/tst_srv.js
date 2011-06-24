var net = require('net');

var socks = new Array();

function notifyAllClients(data, loginName)
{
    for(var i = 0; i < socks.length; i++)
    {
        if(loginName != socks[i][0])
            socks[i][1].write(data);
    }
}

var srv = net.createServer(function(stream) {
    console.log('connection is accepted');
    stream.write("welcome, please enter your name:");

    var isName = true;
    var loginName = "";

    stream.on('data', function(data) {
        if(isName)
        {
            var sockNode = new Array();
            data = new String(data).trim();
            loginName = data;
            sockNode[0] = data;
            sockNode[1] = stream;
            socks.push(sockNode);
            isName = false;
        } else
        {
            data = loginName + " said : " + data;
            notifyAllClients(data, loginName);
        }
    });
}
);

srv.listen(3124, 'localhost');
