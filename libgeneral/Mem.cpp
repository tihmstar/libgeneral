//
//  Mem.cpp
//  libgeneral
//
//  Created by erd on 08.08.23.
//  Copyright © 2023 tihmstar. All rights reserved.
//

#include "../include/libgeneral/Mem.hpp"
#include "../include/libgeneral/macros.h"

using namespace tihmstar;

#pragma mark class Mem
Mem::Mem(size_t size)
: _mem(NULL), _memSize(size), _allocatedSize(0)
{
    if (_memSize){
        assure(_mem = (uint8_t*)calloc(1,_allocatedSize = _memSize));
    }
}

Mem::Mem(const Mem &obj)
: _mem(NULL), _memSize(obj._memSize), _allocatedSize(0)
{
    if (_memSize){
        assure(_mem = (uint8_t*)malloc(_allocatedSize = _memSize));
        memcpy(_mem, obj._mem, _memSize);
    }
}

Mem::Mem(Mem &&old) noexcept{
    _mem = old._mem; old._mem = NULL;
    _memSize = old._memSize; old._memSize = 0;
    _allocatedSize = old._allocatedSize; old._allocatedSize = 0;
}

Mem::Mem(void *mem, size_t memSize) noexcept
: _mem((uint8_t*)mem), _memSize(memSize), _allocatedSize(memSize)
{
    //
}

Mem::Mem(const void *mem, size_t memSize)
: _mem(NULL), _memSize(memSize), _allocatedSize(memSize)
{
    if (_memSize){
        _mem = (uint8_t*)malloc(_allocatedSize = _memSize);
        memcpy(_mem, mem, _memSize);
    }
}

Mem::~Mem(){
    safeFree(_mem); _memSize = 0; _allocatedSize = 0;
}

Mem& Mem::operator=(const Mem &obj){
    safeFree(_mem);
    _memSize = obj._memSize;
    if (_memSize){
        assure(_mem = (uint8_t*)malloc(_allocatedSize = _memSize));
        memcpy(_mem, obj._mem, _memSize);
    }
    return *this;
}

Mem& Mem::operator=(Mem &&old) noexcept{
    safeFree(_mem);
    _mem = old._mem; old._mem = NULL;
    _memSize = old._memSize; old._memSize = 0;
    _allocatedSize = old._allocatedSize; old._allocatedSize = 0;
    return *this;
}

#pragma mark function members

void Mem::resize(size_t size){
    if (size == _memSize) return;
    if (size) {
        if (_memSize) {
            if (size > _memSize) {
                //grow
                if (size > _allocatedSize) {
                    //actually grow, because our allocation is smaller than what is requested
                    if (size > _allocatedSize*2) {
                        /*
                             User requested an allocation bigger than twice what we already have.
                             No optimizazions here, just alloc what was requested
                         */
                        _allocatedSize = size;
                    }else{
                        /*
                            User wants to increase allocation "just a little bit".
                            We will double our current size to have enough room for further small increases
                         */
                        _allocatedSize *= 2;
                    }
                    assure(_mem = (uint8_t*)realloc(_mem, _allocatedSize));
                }
            }else{
                //shrink
                if (size < _allocatedSize/4) {
                    /*
                        Only shrink the allocation if the user needs significantly less space than what we have allocated
                     */
                    _allocatedSize /= 2;
                    assure(_mem = (uint8_t*)realloc(_mem, _allocatedSize));
                }
            }
            _memSize = size;
        }else{
            assure(_mem = (uint8_t*)malloc(_allocatedSize = _memSize = size));
        }
    }else{
        safeFree(_mem); _memSize = 0; _allocatedSize = 0;
    }
}

void Mem::appendSize(size_t size){
    if (size) {
        size_t oldSize = _memSize;
        resize(oldSize + size);
        memset(&_mem[oldSize], 0, size);
    }
}

void Mem::append(const void *mem, size_t size){
    if (size) {
        size_t oldSize = _memSize;
        resize(oldSize + size);
        memcpy(&_mem[oldSize], mem, size);
    }
}

Mem Mem::copy() const{
    Mem ret(_memSize);
    memcpy(ret.data(), _mem, _memSize);
    return ret;
}

void Mem::disown(){
    _mem = NULL;
    _memSize = 0;
    _allocatedSize = 0;
}
