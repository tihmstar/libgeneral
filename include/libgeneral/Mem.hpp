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

#define LIBGENERAL_MEM_DO_INLINE inline __attribute__((always_inline))

namespace tihmstar {

class Mem{
    uint8_t *_mem;
    size_t _memSize;
    size_t _allocatedSize;
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
    LIBGENERAL_MEM_DO_INLINE uint8_t *data() noexcept{return _mem;}
    LIBGENERAL_MEM_DO_INLINE const uint8_t *data() const noexcept{return _mem;}
    LIBGENERAL_MEM_DO_INLINE size_t size() const noexcept{return _memSize;}
    
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
    Typed_Mem(const T val) noexcept :_mem(&val, sizeof(val)){} //copy from T
    Typed_Mem(const void *data, size_t size):_mem(data,size){}
    Typed_Mem(Mem &&old) noexcept :_mem(old){} //move assignment

    template <typename U>
    Typed_Mem(Typed_Mem<U> &&old) noexcept :_mem(old.mem()){} //move assignment
    LIBGENERAL_MEM_DO_INLINE operator const T*() const {return (const T*)_mem.data();}
    LIBGENERAL_MEM_DO_INLINE operator T*() {return (T*)_mem.data();}
    LIBGENERAL_MEM_DO_INLINE const T *operator->() const {return (const T*)_mem.data();}
    LIBGENERAL_MEM_DO_INLINE T *operator->() {return (T*)_mem.data();}
    LIBGENERAL_MEM_DO_INLINE Mem &mem() {return _mem;}
};


#undef LIBGENERAL_MEM_DO_INLINE

};
#endif /* Mem_hpp */
