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

using namespace tihmstar;

exception::exception(int code, std::string err, std::string filename) :
    _err(err),
    _code(code),
    _filename(filename)
    {
        //empty constructor
    };

const char *exception::what(){
    return _err.c_str();
}

int exception::code() const{
    return _code | (int)(_filename.size()<<16);
}

std::string exception::build_commit_count() const {
    return "build_commit_count: override me!";
};

std::string exception::build_commit_sha() const{
    return "build_commit_sha: override me!";
};
