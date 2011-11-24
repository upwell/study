#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

using namespace std;

class SearchResult
{
public:
    SearchResult() : isExactFound_(false),
                    isParentFound_(false),
                    parentRPath_(""),
                    exactRPath_("") { }
    ~SearchResult() { }

    bool IsFound() { return isExactFound_ || isParentFound_; }
    bool IsExactFound() { return isExactFound_; }
    bool IsParentFound() { return isParentFound_; }

    string GetParentRPath() { return parentRPath_; }
    string GetExactRPath() { return exactRPath_; }

    void SetExactFound(bool set) { isExactFound_ = set; }
    void SetParentFound(bool set) { isParentFound_ = set; }

    void SetParentRPath(string &path) { parentRPath_ = path; }
    void SetExactRPath(string &path) { exactRPath_ = path; }

private:
    bool isExactFound_;
    bool isParentFound_;

    string parentRPath_;
    string exactRPath_;
};

#endif
