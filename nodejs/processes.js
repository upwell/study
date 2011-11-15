var cp = require('child_process');

var node_num = 4;
var nodes = [];

var i = 0;

for(i = 0; i < node_num; i++)
{
    var n = cp.fork(__dirname + '/testnode.js');
    nodes.push(n);
}

var all_requests = 0;
var all_handle_time = 0;
var all_success = 0;
var all_failure = 0;

for(i = 0; i < nodes.length; i++)
{
    nodes[i].on('message', function(m) {
        all_requests += m['request_num'];
        all_handle_time += m['handle_time'];
        all_success += m['success_num'];
        all_failure += m['failure_num'];
    });
}

setInterval(printStat, 5000);

function printStat()
{
    var aver_time = all_handle_time / all_success;

    console.log("Total requests: " + all_requests);
    console.log("Total success:  " + all_success);
    console.log("Total failure:  " + all_failure);
    console.log("Average time:   " + aver_time);
}
