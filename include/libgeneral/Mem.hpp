//
//  Mem.hpp
//  libgeneral
//
//  Created by erd on 08.08.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#ifndef Mem_hpp
#define Mem_hpp

#include <stdint.h>
#include <stdlib.h>

namespace tihmstar {

class Mem{
    uint8_t *_mem;
    size_t _memSize;
public:
    Mem(size_t size = 0);
    Mem(const Mem &obj); //copy constructor
    Mem(Mem &&old) noexcept; //move constructor
    Mem(void *mem, size_t memSize) noexcept; //transfer ownership to this obj
    Mem(const void *mem, size_t memSize); //copy mem to this obj
    ~Mem();
    
    Mem& operator=(const Mem &obj); //copy assignment
    Mem& operator=(Mem &&old) noexcept; //move assignment

#pragma mark quick access members
    inline __attribute__((always_inline)) uint8_t *data() noexcept{return _mem;}
    inline __attribute__((always_inline)) const uint8_t *data() const noexcept{return _mem;}
    inline __attribute__((always_inline)) size_t size() const noexcept{return _memSize;}
    
#pragma mark function members
    void resize(size_t size);
    void appendSize(size_t size);
    void append(const void *mem, size_t size);
    Mem copy() const;
    void disown();
};

template <typename T>
class Typed_Mem {
    Mem _mem;
public:
    Typed_Mem() :_mem(sizeof(T)){}
    Typed_Mem(const void *data, size_t size):_mem(data,size){}
    inline __attribute__((always_inline)) operator const T*() const {return (const T*)_mem.data();}
    inline __attribute__((always_inline)) operator T*() {return (T*)_mem.data();}

    inline __attribute__((always_inline)) const T *operator->() const {return (const T*)_mem.data();}
    inline __attribute__((always_inline)) T *operator->() {return (T*)_mem.data();}
    inline __attribute__((always_inline)) Mem &mem() {return _mem;}
};

};
#endif /* Mem_hpp */
