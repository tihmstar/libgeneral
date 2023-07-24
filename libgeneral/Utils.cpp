//
//  Utils.cpp
//  libgeneral
//
//  Created by erd on 13.07.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#include "../include/libgeneral/Utils.hpp"
#include "../include/libgeneral/macros.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

std::vector<uint8_t> tihmstar::readFile(const char *path){
    int fd = -1;
    cleanup([&]{
        safeClose(fd);
    });
    struct stat st = {};
    std::vector<uint8_t> ret;
    retassure((fd = open(path, O_RDONLY)) != -1, "Failed to open file at path '%s'",path);
    retassure(!fstat(fd, &st), "Failed to fstat file at path '%s'",path);
    ret.resize(st.st_size);
    retassure(read(fd, ret.data(), ret.size()) == ret.size(), "Failed to read file at path '%s'",path);
    return ret;
}

void tihmstar::writeFile(const char *path, std::vector<uint8_t> data){
    int fd = -1;
    cleanup([&]{
        safeClose(fd);
    });
    struct stat st = {};
    std::vector<uint8_t> ret;
    retassure((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) != -1, "Failed to open file at path '%s'",path);
    retassure(write(fd, data.data(), data.size()) == data.size(), "Failed to write to file at path '%s'",path);
}
