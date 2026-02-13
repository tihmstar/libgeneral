//
//  Utils.hpp
//  libgeneral
//
//  Created by erd on 13.07.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdint.h>
#include <vector>
#include <libgeneral/Mem.hpp>

namespace tihmstar {
    
Mem readFile(const char *path);
void writeFile(const char *path, const void *mem, size_t memSize, int perm = 0644);
bool fileExists(const char *path) noexcept;
uint64_t parseNum(const char *num) noexcept;
int hexToBytes(const char *hexstr, uint8_t *output, size_t outbufSize) noexcept;
void bytesToHex(const uint8_t *bytes, size_t bytesSize, char *outStr, size_t *outStrSize) noexcept;
};

#endif /* Utils_hpp */
