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
#   ifndef NO_EXCEPT_ASSURE
#       define EXCEPT_ASSURE
#   endif
#endif


// -- assure --
#ifdef EXCEPT_ASSURE
#include "exception.hpp"
//assure cpp
#   define assure(cond) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__, "assure failed", __FILE__)
#   define retassure(cond, err) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__,err,__FILE__)
#   define reterror(err) throw tihmstar::EXPECTIONNAME(__LINE__, err, __FILE__)
#   define retcustomerror(err,custom_except) throw tihmstar::custom_except(__LINE__, err, __FILE__)
#   define doassure(cond,code) do {if (!(cond)){(code);assure(cond);}} while(0)
#   define assureclean(cond) do {if (!(cond)){clean();assure(cond);}} while(0)
//mach assures
#   define assureMach(kernRet) if (kernRet) throw tihmstar::EXPECTIONNAME(__LINE__, "assure failed", __FILE__,kernRet)
#   define assureMachclean(kernRet) do {if (kernRet){clean();assureMach(kernRet);}} while(0)
#   define assureCatchClean(code) do {try { code; } catch (EXPECTIONNAME &e) { clean(); throw; }} while (0)
#   define assureNoDoublethrow(code) \
        do{try {code;} catch (EXPECTIONNAME &e) {if (isInException) {error("Double exception! Error in line=%d",__LINE__);}else{throw;}}}while (0)



// //more cpp assure
#   ifndef EXPECTIONNAME
#       define EXPECTIONNAME exception
#   endif

#else
//assure c
#   define assure(a) do{ if ((a) == 0){err=__LINE__; goto error;} }while(0)
#   define retassure(cond, errstr ...) do{ if ((cond) == 0){error(errstr); goto error;} }while(0)
#   define reterror(estr ...) do{error(estr);err=__LINE__; goto error; }while(0)

#endif

#endif /* macros_h */
