http = require("http");

var options = {
    host: "localhost",
    port: 81,
    path: "/proxy.pac",
    method: "GET"
};

process.on('message', function(m) {
    console.log('Received from parent:', m);
});

var stat_request = 0;
var stat_success = 0;
var stat_failure = 0;
var handle_time = 0;

var configs = { send_interval : 2,
                stat_interval : 1000
                };

setInterval(sendRequest, configs['send_interval']);
setInterval(sendStat, configs['stat_interval']);
//setInterval(printStat, 5000);

function sendStat()
{
    process.send({ request_num: stat_request,
                   success_num: stat_success,
                   failure_num: stat_failure,
                   handle_time: handle_time });
}

function printStat()
{
    var aver_time = handle_time / stat_success;

    console.log("Total requests: " + stat_request);
    console.log("Total success:  " + stat_success);
    console.log("Total failure:  " + stat_failure);
    console.log("Average time:   " + aver_time);
}

function getCurrentTime()
{
    return new Date().valueOf();
}

function sendRequest()
{
    var dataLen = 0;

    stat_request += 1;
    var send_start = getCurrentTime();
    var send_end;
    var req = http.request(options, function(res) {
        //console.log("status code: " + res.statusCode);

        res.on("data", function(chunk) {
            dataLen += chunk.length;
        });
        res.on("end", function() {
            stat_success += 1;
            //console.log("data len: " + dataLen);
            send_end = getCurrentTime();
            handle_time += (send_end - send_start);
            this.connection.end();
        });
    });

    req.on("error", function(e) {
        console.log('problem while send the request');
        stat_failure += 1;
    });

    req.end();
}

