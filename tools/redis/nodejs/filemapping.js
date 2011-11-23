var redis = require("redis");
var async = require("async");

var client = redis.createClient();

var global_next_fileid = "global:nextFileId";

module.exports = filemapping;

client.on("error", function(err) {
    console.log("redis client error: " + err);
});

client.on("end", function() {
    console.log("redis server close conn, reconnect ..");
    client = redis.createClient();
});

function filemapping() {
}

filemapping.prototype.addfile = function(lpath, rpath) {
    var fid;
    var pfid, plpath;
    var is_parent_exist = false;
    var is_vnode_found = false;

    var is_root = is_root_dir(lpath);
    var plpath = lpath.substring(0, lpath.lastIndexOf("/"));

    var multi = client.multi();

    async.series([
        function(callback) {
            client.incr(global_next_fileid, function(err, result) {
                fid = result;
                callback(null, 1);
            });
        },
        function(callback) {
            if(!is_root) {
                client.exists(get_lpath_set_key(plpath), function(err, result) {
                    if(err) {
                        console.log("client exists error", err);
                        return;
                    }

                    if(result == 1)
                        is_parent_exist = true;

                    callback(null, 1);
                });
            }
            callback(null, 1);
        },
        function(callback) {
            if(!is_root && (!is_parent_exist || !is_vnode_found)) {
                client.incr(global_next_fileid, function(err, result) {
                    if(err) { console.log("Error", err); }
                    pfid = result;
                    callback(null, 1);
                });
            }

            callback(null, 1);
        },
        function(callback) {
            if(!is_root && is_parent_exist) {
                client.smembers(get_lpath_set_key(plpath), function(err, pfids) {
                    pfids.forEach(function(id, i) {
                        if(is_vnode(id)) {
                            multi.sadd(get_file_children_key(id), fid);
                            is_vnode_found = true;
                        }
                    });
                    callback(null, 1);
                });
            }
            callback(null, 1);
        },
        function(callback) {
            if(!is_root && (!is_parent_exist || !is_vnode_found)) {
                multi.set(get_file_type_key(pfid), 0);
                multi.sadd(get_file_children_key(pfid), fid);
                multi.sadd(get_lpath_set_key(plpath), pfid);
            }

            multi.set(get_file_lpath_key(fid), lpath);
            multi.set(get_file_rpath_key(fid), rpath);
            multi.sadd(get_lpath_set_key(lpath), fid);

            multi.exec(function(err, replies) {
                console.log(replies);
            });
        }
    ], function(err, result) {
        if(err) console.log("Final Error:", err);
        console.log("add file succeed");
        client.quit();
    });
};

function is_vnode(fid)
{
    var rtn = false;
    var done = false;
    client.exists(get_file_type_key(fid), function(err, result) {
        if(err) {
            console.log("client exist error", err);
            return;
        }

        rtn = (result == 1) ? true : false;
        done = true;
    });

    while(!done) {}

    return rtn;
}

function get_new_fileid()
{
    var fid;
    var done = false;

    client.incr(global_next_fileid, function(err, result) {
        fid = result;
        done = true;
    });

    return fid;
}

// assume file lpath format is:
//   == //host/home/iwss ==
function is_root_dir(lpath)
{
    var str = lpath.substring(2);
    str = str.substring(str.indexOf("/") + 1);

    if(str.indexOf("/") == -1)
        return true;
    else
        return false;
}

function get_file_lpath_key(fid)
{
    var rtn = "file:" + fid + ":lpath";
    return rtn;
}

function get_file_rpath_key(fid)
{
    return "file:" + fid + ":rpath";
}

function get_lpath_set_key(lpath)
{
    return "lpath:" + lpath + ":fid";
}

function get_file_type_key(fid)
{
    return "file:" + fid + ":type";
}

function get_file_children_key(fid)
{
    return "file:" + fid + ":children";
}
