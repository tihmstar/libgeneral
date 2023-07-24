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

namespace tihmstar {
    
std::vector<uint8_t> readFile(const char *path);
void writeFile(const char *path, std::vector<uint8_t> data);

};

#endif /* Utils_hpp */
