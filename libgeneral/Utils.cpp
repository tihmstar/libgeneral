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

#ifndef O_BINARY
#define O_BINARY 0
#endif

tihmstar::Mem tihmstar::readFile(const char *path){
    int fd = -1;
    cleanup([&]{
        safeClose(fd);
    });
    struct stat st = {};
    Mem ret;
    retassure((fd = open(path, O_RDONLY | O_BINARY)) != -1, "Failed to open file at path '%s'",path);
    retassure(!fstat(fd, &st), "Failed to fstat file at path '%s'",path);
    ret.resize(st.st_size);
    retassure(read(fd, ret.data(), ret.size()) == ret.size(), "Failed to read file at path '%s'",path);
    return ret;
}

void tihmstar::writeFile(const char *path, const void *mem, size_t memSize, int perm){
    int fd = -1;
    cleanup([&]{
        safeClose(fd);
    });
    retassure((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, perm)) != -1, "Failed to open file at path '%s'",path);
    retassure(write(fd, mem, memSize) == memSize, "Failed to write file at path '%s'",path);
}

bool tihmstar::fileExists(const char *path) noexcept{
    struct stat st{};
    return stat(path, &st) == 0;
}

uint64_t tihmstar::parseNum(const char *num) noexcept{
    if (num[0] == '0' && (num[1] == 'x' || num[1] == 'X')) {
        return strtoul(num, NULL, 16);
    }else{
        return strtoul(num, NULL, 10);
    }
}

int tihmstar::hexToBytes(const char *hexstr, uint8_t *outbuf, size_t outbufSize) noexcept {
  int err = 0;
  int res = 0;

  while (*hexstr && res/2 < outbufSize) {
    char c = *hexstr++;
    if (c == '\n') break;
    if (c == ' ') break;
    uint8_t *v = &outbuf[(res++)/2];
    *v <<= 4;
    if (c >= '0' && c<='9') {
        *v += c - '0';
    }else if (c >= 'a' && c <= 'f'){
        *v += 10 + c - 'a';
    }else if (c >= 'A' && c <= 'F'){
        *v += 10 + c - 'A';
    }else{
        creterror("Got unexpected char '%c'",c);
    }
  }

error:
  if (err){
    return -err;
  }
  return res/2;
}
