#ifndef FILE_MAPPING_H
#define FILE_MAPPING_H

#include "hiredis/hiredis.h"
#include <string>

#include "searchresult.h"

using namespace std;

typedef long long fid_t;

class FileMapping {
public:
    FileMapping();
    ~FileMapping();

    bool Init();
    bool AddFile(const string &lpath, const string &rpath);
    bool GetRPath(const string &lpath, SearchResult &result);

    static const string dsPath_;
    static const string globalFileIdKey_;
    static const struct timeval connTimeout_;

private:

    redisContext *rContext_;

    bool checkRedisContext();

    fid_t getNextFileId();
    bool isLPathNodeExist(const string &lpath, fid_t &fid);
    bool isLPathVnodeExist(const string &lpath, fid_t &fid);

    bool AddVnode(const string &lpath, fid_t childFid);
    bool AddFileNode(const string &lpath, const string &rpath,
                        fid_t fid);
    bool AddVnodeChild(const fid_t pvfid, const fid_t fid);

    string getRPathFromFid(const fid_t fid);


    // utils
    bool isRootDir(const string &lpath);
    string getLPathFidKey(const string &lpath);
    string getLPathVFidKey(const string &lpath);
    string getFileTypeKey(const fid_t fid);
    string getFileChildrenKey(const fid_t fid);
    string getFileLPathKey(const fid_t fid);
    string getFileRPathKey(const fid_t fid);
    string fidToString(const fid_t fid);
};

#endif
