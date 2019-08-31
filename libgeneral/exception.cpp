//
//  exception.cpp
//  liboffsetfinder64
//
//  Created by tihmstar on 09.03.18.
//  Copyright © 2018 tihmstar. All rights reserved.
//

#include <libgeneral/macros.h>
#include <libgeneral/exception.hpp>
#include <string>
#include <stdarg.h>

using namespace tihmstar;

exception::exception(int code, const char *filename, const char *err ...) :
    _code(code),
    _filename(filename),
    _err(NULL)
    {
        va_list ap = {};
        va_start(ap, err);
        vasprintf(&_err, err, ap);
        va_end(ap);
    };

const char *exception::what(){
    return _err;
}

int exception::code() const{
    return (_code << 16) | (int)(_filename.size());
}

void exception::dump() const{
    printf("[exception]:\n");
    printf("what=%s\n",_err);
    printf("code=%d\n",code());
    printf("commit count=%s:\n",build_commit_count().c_str());
    printf("commit sha  =%s:\n",build_commit_sha().c_str());
}

std::string exception::build_commit_count() const {
    return "build_commit_count: override me!";
};

std::string exception::build_commit_sha() const{
    return "build_commit_sha: override me!";
};

exception::~exception(){
    safeFree(_err);
}
