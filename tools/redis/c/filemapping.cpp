#include "filemapping.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#define LogPrintf printf

using namespace std;

const string FileMapping::dsPath_ = "/tmp/redis.sock";
const string FileMapping::globalFileIdKey_ = "global:nextFileId";

const struct timeval FileMapping::connTimeout_ = { 1, 500000 };

FileMapping::FileMapping() :
    rContext_(NULL)
{
}

FileMapping::~FileMapping()
{
    if(rContext_)
        redisFree(rContext_);
}

bool FileMapping::Init()
{
    if(!rContext_)
    {
        rContext_ = redisConnectUnixWithTimeout(dsPath_.c_str(), connTimeout_);
    }

    if(!rContext_)
    {
        LogPrintf("initliaze failure - connect to redis fail\n");
        return false;
    }

    return true;
}

/*
 * Add a *NEW* File object
 * it doesn't check the existence of lpath, just create a new one.
 *
 */
bool FileMapping::AddFile(const string &lpath, const string &rpath)
{
    fid_t fid = 0;
    fid_t pvfid = 0;
    string parentLPath;
    bool isParentVnodeFound = false;

    fid = getNextFileId();

    if(!isRootDir(lpath))
    {
        parentLPath = lpath.substr(0, lpath.find_last_of("/"));

        isParentVnodeFound = processVnodeCheck(parentLPath, pvfid);

        if(!isParentVnodeFound)
        {
            AddVnode(parentLPath, fid);
        } else {
            AddVnodeChild(pvfid, fid);
        }
    }

    AddFileNode(lpath, rpath, fid);
}

bool FileMapping::checkRedisContext()
{
    // TODO check the redis context before calling
    return true;
}

bool FileMapping::AddVnodeChild(const fid_t pvfid, const fid_t fid)
{
    checkRedisContext();

    redisReply* reply;
    string tmpStr = getFileChildrenKey(pvfid);

    reply = (redisReply*) redisCommand(rContext_, "SADD %s %lld", tmpStr.c_str(), fid);

    freeReplyObject(reply);

    return true;
}

bool FileMapping::AddFileNode(const string &lpath, const string &rpath,
                                fid_t fid)
{
    checkRedisContext();

    redisReply *reply;
    int count = 0;
    string tmpStr;
    string command;

    tmpStr = getFileLPathKey(fid);
    command = "SET " + tmpStr + " " + lpath;
    redisAppendCommand(rContext_, command.c_str());

    tmpStr = getFileRPathKey(fid);
    command = "SET " + tmpStr + " " + rpath;
    redisAppendCommand(rContext_, command.c_str());

    tmpStr = getLPathFidKey(lpath);
    command = "SET " + tmpStr + " " + fidToString(fid);
    redisAppendCommand(rContext_, command.c_str());

    count += 3;

    for(int i = 0; i < count; i++)
    {
        redisGetReply(rContext_, (void**) &reply);
        freeReplyObject(reply);
    }

    return true;
}

bool FileMapping::AddVnode(const string &lpath, fid_t childFid)
{
    checkRedisContext();

    redisReply *reply;
    fid_t fid = getNextFileId();
    int count = 0;

    string tmpStr;
    string command;

    tmpStr = getFileChildrenKey(fid);
    command = "SADD " + tmpStr + " " + fidToString(childFid);
    redisAppendCommand(rContext_, command.c_str());
    count++;

    tmpStr = getLPathVFidKey(lpath);
    command = "SET " + tmpStr + " " + fidToString(fid);
    redisAppendCommand(rContext_, command.c_str());
    count++;

    for(int i = 0; i < count; i++)
    {
        redisGetReply(rContext_, (void**) &reply);
        freeReplyObject(reply);
    }

    return true;
}

bool FileMapping::processVnodeCheck(const string &lpath, fid_t &fid)
{
    checkRedisContext();

    string lpathVFidKey = getLPathVFidKey(lpath);
    redisReply *reply;

    reply = (redisReply*) redisCommand(rContext_, "GET %s", lpathVFidKey.c_str());

    if(reply->type == REDIS_REPLY_ERROR)
    {
        // TODO throw
        LogPrintf("redis command failed\n");
        return false;
    } else
    {
        if(reply->type == REDIS_REPLY_NIL)
        {
            // no key found
            return false;
        }

        fid = atoi(reply->str);
        return true;
    }
}

bool FileMapping::isVnode(const fid_t fid)
{
    checkRedisContext();

    string fileTypeKey = getFileTypeKey(fid);
    bool rtn = false;

    redisReply *reply = (redisReply*) redisCommand(rContext_, "EXIST %s", fileTypeKey.c_str());

    if(reply->type == REDIS_REPLY_ERROR)
    {
        // TODO throw
        return rtn;
    } else {
        rtn = reply->integer == 1 ? true : false;
        freeReplyObject(reply);

        return rtn;
    }
}

bool FileMapping::isLPathNodeExist(const string &lpath)
{
    checkRedisContext();

    string lpathFidKey;
    redisReply *reply;
    bool rtn = false;

    lpathFidKey = getLPathFidKey(lpath);
    reply = (redisReply*) redisCommand(rContext_, "EXISTS %s", lpathFidKey.c_str());

    if(reply->type == REDIS_REPLY_ERROR)
    {
       // TODO throw exception here
       return rtn;
    } else
    {
        rtn = (reply->integer == 1) ? true : false;
        freeReplyObject(reply);

        return rtn;
    }
}

fid_t FileMapping::getNextFileId()
{
    checkRedisContext();

    redisReply *reply;
    fid_t rtn = -1;

    reply = (redisReply*) redisCommand(rContext_, "INCR %s", globalFileIdKey_.c_str());
    if(reply->type == REDIS_REPLY_ERROR)
    {
        // TODO throw exception here
        return rtn;
    }
    else
    {
        rtn = reply->integer;
        freeReplyObject(reply);

        return rtn;
    }
}

bool FileMapping::isRootDir(const string &lpath)
{
    string str = lpath.substr(2);
    size_t pos;

    pos = str.find("/");
    str = str.substr(pos+1);

    if(str.find("/") == string::npos)
        return true;
    else
        return false;
}

string FileMapping::getLPathFidKey(const string &lpath)
{
    return "lpath:" + lpath + ":fid";
}

string FileMapping::getLPathVFidKey(const string &lpath)
{
    return "lpath:" + lpath + ":vfid";
}

string FileMapping::getFileTypeKey(const fid_t fid)
{
    return "file:" + fidToString(fid) + ":type";
}

string FileMapping::getFileChildrenKey(const fid_t fid)
{
    return "file:" + fidToString(fid) + ":children";
}

string FileMapping::getFileLPathKey(const fid_t fid)
{
    return "file:" + fidToString(fid) + ":lpath";
}

string FileMapping::getFileRPathKey(const fid_t fid)
{
    return "file:" + fidToString(fid) + ":rpath";
}

string FileMapping::fidToString(const fid_t fid)
{
    char buf[128];

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lld", fid);

    string tmpStr(buf);

    return tmpStr;
}
