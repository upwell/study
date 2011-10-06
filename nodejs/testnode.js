http = require("http");

var options = {
    host: "localhost",
    port: 81,
    path: "/",
    method: "GET"
};

var start_time = 0;
var stat_request = 0;
var stat_success = 0;
var stat_failure = 0;

start_time = new Date().valueOf();

setInterval(sendRequest, 10);
setInterval(printStat, 5000);

function printStat()
{
    var curr_time = new Date().valueOf();
    var aver_time = (curr_time - start_time) / stat_success;

    console.log("Total requests: " + stat_request);
    console.log("Total success:  " + stat_success);
    console.log("Total failure:  " + stat_failure);
    console.log("Average time:   " + aver_time);

}

function sendRequest()
{
    var dataLen = 0;

    stat_request += 1;
    var req = http.request(options, function(res) {
        //console.log("status code: " + res.statusCode);

        res.on("data", function(chunk) {
            dataLen += chunk.length;
        });
        res.on("end", function() {
            stat_success += 1;
            //console.log("data len: " + dataLen);
            this.connection.end();
        });
    });

    req.on("error", function(e) {
        console.log('problem while send the request');
        stat_failure += 1;
    });

    req.end();
}
