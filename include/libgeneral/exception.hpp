//
//  exception.hpp
//  libgeneral
//
//  Created by tihmstar on 09.03.18.
//  Copyright © 2018 tihmstar. All rights reserved.
//

#ifndef exception_hpp
#define exception_hpp

#include <libgeneral/macros.h>

#include <string>
#include <stdarg.h>

#if defined(WIN32) && !__clang__
#   define LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO __declspec( dllexport )
#else
#   ifdef HAVE_FVISIBILITY
#       define LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO __attribute__((visibility("default")))
#   else
#       define LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO
#   endif
#endif

#define EASY_BASE_EXCEPTION(name) \
class LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO name : public tihmstar::exception{ \
public: \
    using exception::exception; \
    /*this makes sure typeinfo works correcty in other libs!*/ \
    virtual const char *what() const noexcept override; \
}

#define EASY_EXCEPTION(name, base) \
class LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO name : public tihmstar::base{ \
    using base::base; \
}

#define EASY_BASE_EXCEPTION_DEF(name) \
LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO const char *tihmstar::name::what() const noexcept{ \
    /*this makes sure typeinfo works correcty in other libs!*/\
    return tihmstar::exception::what();\
}

namespace tihmstar {
    class LIBGENERAL_VISIBLE_EXCEPTION_TYPEINFO exception : public std::exception{
        const char *_commit_count_str;
        const char *_commit_sha_str;
        int _line;
        std::string _filename;
        char *_err;
        
    public:
        exception(const char *commit_count_str, const char *commit_sha_str, int line, const char *filename, const char *err ...);
        exception(const char *commit_count_str, const char *commit_sha_str, int line, const char *filename, va_list ap, const char *err);

        exception(const exception &e); //copy constructor
        
        //custom error can be used
        virtual const char *what() const noexcept override;
        
        /*
         -first lowest two bytes of code is sourcecode line
         -next two bytes is strlen of filename in which error happened
         */
        int code() const;
        
        virtual void dump() const;
        virtual std::string dumpStr() const;

        //Information about build
        virtual std::string build_commit_count() const;
        virtual std::string build_commit_sha() const;
        
        ~exception();
    };
};

#endif /* exception_hpp */
