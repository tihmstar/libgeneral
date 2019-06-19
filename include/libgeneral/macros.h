//
//  macros.h
//  libgeneral
//
//  Created by tihmstar on 03.05.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#ifndef macros_h
#define macros_h

#include <assert.h>

#ifdef DEBUG //versioning
#   define VERSION_COMMIT_COUNT "Debug"
#   define VERSION_COMMIT_SHA "Build: " __DATE__ " " __TIME__
#else
#include <config.h>
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "PACKAGE_NAME_not_set"
#endif

#ifndef VERSION_MAJOR
#define VERSION_MAJOR "0"
#endif

#define VERSION_STRING PACKAGE_NAME " version: " VERSION_MAJOR "." VERSION_COMMIT_COUNT "-" VERSION_COMMIT_SHA


// ---- functions ----

// -- logging --
#ifndef CUSTOM_LOGGING
#define info(a ...) ({printf(a),printf("\n");})
#define log(a ...) ({printf(a),printf("\n");})
#define warning(a ...) ({printf("[WARNING] "), printf(a),printf("\n");})
#define error(a ...) ({printf("[Error] "),printf(a),printf("\n");})
#endif

#define NOTIMPLEMENTED() __assert_rtn(__func__, __FILE__, __LINE__, "not implemented")

#define safeFree(ptr) ({if (ptr) free(ptr),ptr=NULL;})
#define safeFreeCustom(ptr,func) ({if (ptr) func(ptr),ptr=NULL;})

#ifdef __cplusplus
#include <functional>
#   ifndef NO_EXCEPT_ASSURE
#       define EXCEPT_ASSURE
#   endif
#endif


// -- assure --

#   define cassure(a) do{ if ((a) == 0){err=__LINE__; goto error;} }while(0)
#   define cretassure(cond, errstr ...) do{ if ((cond) == 0){err=__LINE__;error(errstr); goto error;} }while(0)
#   define creterror(estr ...) do{error(estr);err=__LINE__; goto error; }while(0)


#ifdef EXCEPT_ASSURE
#include "exception.hpp"
//assure cpp
#   define assure(cond) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__, __FILE__, "assure failed")
#   define retassure(cond, errstr ...) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__,__FILE__,errstr)
#   define reterror(errstr ...) throw tihmstar::EXPECTIONNAME(__LINE__, __FILE__, errstr)
#   define retcustomerror(custom_except,errstr ...) throw tihmstar::custom_except(__LINE__, __FILE__, errstr)
#   define doassure(cond,code) do {if (!(cond)){(code);assure(cond);}} while(0)
//mach assures
//#   define assureMach(kernRet) if (kernRet) throw tihmstar::EXPECTIONNAME(__LINE__, __FILE__, "assure failed")
#   define assureMachclean(kernRet) do {if (kernRet){clean();assureMach(kernRet);}} while(0)
#   define assureCatchClean(code) do {try { code; } catch (EXPECTIONNAME &e) { clean(); throw; }} while (0)
#   define assureNoDoublethrow(code) \
        do{try {code;} catch (EXPECTIONNAME &e) {if (isInException) {error("Double exception! Error in line=%d",__LINE__);}else{throw;}}}while (0)


// //more cpp assure
#   ifndef EXPECTIONNAME
#       define EXPECTIONNAME exception
#   endif


class guard{
    std::function<void()> _f;
public:
    guard(std::function<void()> cleanup) : _f(cleanup) {}
    guard(const guard&) = delete; //delete copy constructor
    guard(guard &&o) = delete; //move constructor
    
    ~guard(){_f();}
};
#define cleanup(f) guard _cleanup(f);

#else
//assure c
#   define assure(a) cassure(a)
#   define retassure(cond, errstr ...) cretassure(cond, errstr)
#   define reterror(estr ...) creterror(estr)

#endif

#endif /* macros_h */
