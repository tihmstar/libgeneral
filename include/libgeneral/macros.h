//
//  macros.h
//  libgeneral
//
//  Created by tihmstar on 03.05.19.
//  Copyright © 2019 tihmstar. All rights reserved.
//

#ifndef macros_h
#define macros_h

#ifdef DEBUG //versioning
#   define TOOL_NAME "DEBUG: TOOL_NAME_NOT_SET!"
#   define VERSION_COMMIT_COUNT "Debug"
#   define VERSION_COMMIT_SHA "Build: " __DATE__ " " __TIME__
#else
#   ifndef TOOL_NAME
#       error TOOL_NAME_NOT_SET
#   endif
#   ifndef EXPECTIONNAME
#       error EXPECTIONNAME_NAME_NOT_SET
#   endif
#endif

#define info(a ...) ({printf(a),printf("\n");})
#define log(a ...) ({printf(a),printf("\n");})
#define warning(a ...) ({printf("[WARNING] "), printf(a),printf("\n");})
#define error(a ...) ({printf("[Error] "),printf(a),printf("\n");})

#define safeFree(ptr) ({if (ptr) free(ptr),ptr=NULL;})

#ifdef __cplusplus
#   define assure(cond) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__, "assure failed", __FILE__)
#   define retassure(cond, err) if ((cond) == 0) throw tihmstar::EXPECTIONNAME(__LINE__,err,__FILE__)
#   define reterror(err) throw tihmstar::EXPECTIONNAME(__LINE__, err, LOCAL_FILENAME)
#   define retcustomerror(err,except) throw tihmstar::EXPECTIONNAME(__LINE__, err, __FILE__)
#   define doassure(cond,code) do {if (!(cond)){(code);assure(cond);}} while(0)
#   define assureclean(cond) do {if (!(cond)){clean();assure(cond);}} while(0)
#else
#   define assure(a) do{ if ((a) == 0){err=__LINE__; goto error;} }while(0)
#   define retassure(retcode, a) do{ if ((a) == 0){err=retcode; goto error;} }while(0)
#endif

#endif /* macros_h */
