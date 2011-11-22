var redis = require("redis");
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

filemapping.prototype.close = function() {
    client.end();
}


filemapping.prototype.addfile = function(lpath, rpath) {
    console.log("inside module");
    var fid = get_new_fileid();
    var pfid, plpath;

    if(!is_root_dir(lpath))
    {
        console.log("inside root dir");
        // check parent dir existence
        var is_vnode_found = false;
        plpath = lpath.substring(0, lpath.lastIndexOf("/"));
        console.log(plpath);

        client.exists(get_lpath_set_key(plpath), function(err, results) {
            if(err) {
                console.log("client exists error", err);
                return;
            }

            console.log("debug ", results);
            var parent_multi = client.multi();

            if(results == 1) {
                client.smembers(get_lpath_set_key(plpath), function(err, pfids) {
                    if(err) {
                        console.log("smember error", err);
                        return;
                    }

                    pfids.forEach(function(id, i) {
                        if(is_vnode(id)) {
                            parent_multi.sadd(get_file_children_key(id), fid);
                            is_vnode_found = true;
                        }
                    });
                });
            }

            if(results == 0 || !is_vnode_found) {
                // add a cache type parent directory
                console.log("debug add parent vnode");
                pfid = get_new_fileid();
                parent_multi.set(get_file_type_key(pfid), 0);
                parent_multi.sadd(get_file_children_key(pfid), fid);
                parent_multi.sadd(get_lpath_set_key(plpath), pfid);
            }

            parent_multi.exec(function(err, result) {
                console.log(result);
            });
        });
    }

    console.log("debug add vnode");

    var multi = client.multi();
    multi.set(get_file_lpath_key(fid), lpath);
    multi.set(get_file_rpath_key(fid), rpath);
    multi.sadd(get_lpath_set_key(lpath), fid);
    multi.exec(function(err, replies) {
        if(err)
        {
            console.log("multi exec error" + err);
            return false;
        }
        console.log(replies);
    });
};

function is_vnode(fid)
{
    var rtn = false;
    client.exists(get_file_type_key(fid), function(err, result) {
        if(err) {
            console.log("client exist error", err);
            return;
        }

        rtn = (result == 1) ? true : false;
    });

    return rtn;
}

function get_new_fileid()
{
    var fid;
    client.incr(global_next_fileid, function(err, result) {
        fid = result;
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
    console.log(rtn);
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
