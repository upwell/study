var async = require("async");

var redis = require("redis");
var client = redis.createClient();

var id;

async.series([
    function(callback) {
        client.incr("test", function(err, result) {
            console.log("in 1st function", result);
            id = result;
            callback(null, 1);
        });
    },
    function(callback) {
        client.incr("test", function(err, result) {
            if(err) {
                console.log("Error", err);
                return;
            }
            console.log("in 2nd function", result);
            id = result;
            callback(null, 2);
        });
    }
    ], function(err, result) {
        if(err) {
            console.log("Error", err);
            return;
        }
        console.log(result);
        client.quit();
    });

console.log("after async call");

