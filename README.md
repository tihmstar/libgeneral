# libgeneral
libgeneral is (in its core) a collection of macros, which aid in employing a
certain coding style used throughout my projects.
This developed over several years and aims to:
- reduce bugs
- improve readability/maintainability
- help debugging
- deploy a basic automatic version number tracking for git base projects
- unify logging (lots of DEBUG info vs. option custom RELEASE stripping)

In the following i will present the coding guidelines which should be considered when using **libgeneral**:

# Coding guidelines
There are some differences in C vs. C++ which will be noted accordingly.

When using C++ the general policy should be to not use raw pointers or raw memory in first place,
however since this is rarely possible in practice here are some guidelines on how to do so properly.

## Coding Style / Functions
Put opening curly brackets `{` in the same line as closing brackets `)`.

Thus prefer this:
```C
void empty(int a){ //Good: opening curly bracket in same line as closing bracket

}
```

Over this:
```C
void empty(int a)
{ //Bad: opening curly backet in different line than closing bracket

}
```

If a function guarantees not to throw, this should be explicitly marked as `noexcept`, otherwise the function should be expected to throw.

Using of `const` keyword is encouraged where possible. For example if a function passes a string, which it reads, but never modifies consider declaring the argument as `const char *`.

If a class member function does not modify the class state in any ways the keyword `const` can be added to allow calling the function on a const object.   
Example:
```C++
class testclass{
public:
  testclass();
  ~testclass();
  void printHello() const;
};
```


## Good and Bad C++ features
C++ offers a variety of features. If you try to use every single of them in all of your projects,
chances are that people will build up a deep hatred and aggression towards you (at least so i was told).
Here is a list of do's and dont's:

### C++ discouraged features
#### Streams (eg. `std::cout << "start" << std::endl;`) -> **Use `printf` instead!**  
While this is probably one of the first and biggest things to C++ to learn for people coming from C,
i personally don't really like streams.
The idea is of putting data in and out a stream might be a fun gimmick on a abstraction level,
but i feel like it is often not really practical.

These things tend to be confusing really quickly:  
eg. How do i read from `b` to `a`? Is it `a << b;` or `a >> b;`?    
Furthermore `a << b;` is usually not the same as `b >> a;`.

Also formatting is usually a pita, here `printf("0x%016llx %zu %lld %d\n\n%s\n",...)` is just easier.

##### Thus, use printf instead of streams !

### C++ encouraged features
#### Exceptions
While there are a lot of split opinions about exceptions, i think that if used properly they are a great tool. That being said, in **libgeneral** wrappers are used, so that the programmer doesn't need to do raw constructing/throwing of exception.   
However the programmer needs to be aware of the fact that an exception can be thrown at any point in time, which could be at any function invocation.
I view this as an advantage, since it allows to do implicit error handling.
Generally speaking a programmer should error-check every function that is called and handle possible failures accordingly (not every function error is fatal). When a programmer expects that a function
could fail, then a `try{}catch(...){}` should be used to account for the fact.
However sometimes a programmer might forget to check whether a function invocation failed, which might be at a fatal point in time. In this case the only sane response is to terminate the program and show the error for debugging purposes. If not handled this way, subtle failures may lead to deeply nested, hard to find and debug, bugs; which may end up compromising security of a system.

#### Traditional C error checking
In **C** a function which can fail should always return an integer to indicate success return value of `0` or an error code which is any value other than zero (usually negative values).
Functions which usually return values other than `0` (or `NULL`) should use this to indicate and error
(for example like `malloc()`).   
When no special values can be reserved for error checking because all return values might be valid,
a function shall fall back to indicate success(or failure) by returning and `int` and return the actual value by a pointer reference instead.   

Example:
```C
int somefunction(int arg1, char* args, ssize_t *out1){
    /*
      do something here...
    */
    if (/*failure*/){
        return -errorcode; //indicate failure with error code
    }
    /*success*/
    if (out1) *out1 = 123; //1) check if out1 isn't NULL. 2) write return value
    return 0; //indicate success
}
```
When **exceptions** are use for error checking, the following can be used instead:
```C++
ssize_t somefunction(int arg1, char* args){
    /*
      do something here...
    */
    if (/*failure*/){
        reterror("Error message here"); //macro from libgeneral
    }
    /*success*/
    return 123; //returning a value implicitly means success
}
```
or alternatively:
```C++
ssize_t somefunction(int arg1, char* args){
    /*
      do something here...
    */
    retassure(!(/*failure*/),"Error message here"); //macro from libgeneral
    /*success*/
    return 123; //returning a value implicitly means success
}
```


## Variable declaration & initialization
Variables should always be declared at the beginning of a function / scope and immediately
initialized to sane values.  
Most of the time a sane value is:
- `0` for integers
- `NULL` for raw C-style pointers
- `nullptr` for C++ object pointers (to indicate that the variable is a C++ object)
- `-1` for filedescriptors (since 0 is a valid filedescriptor)

Example:
```C
ASN1DERElement ASN1DERElement::makeASN1Integer(uint64_t num){
    uint64_t bigEndian = num;   //Initializing with function argument
    int bytes = 0;              //Initializing with specifiy value or a sane default value

    while (num) {
        bigEndian <<=8;
        bigEndian |= num & 0xff;
        num >>=8;
        bytes++;
    }

    return ASN1DERElement({ASN1DERElement::TagNumber::TagINTEGER, ASN1DERElement::Universal}, &bigEndian, bytes);
}
```

## Error checking
Programs should be written in a way that every line in every function is guaranteed to succeed before the next line executes. For example we need to guarantee that a buffer allocation succeeded, before we use the buffer for reading/writing. It should be assumed that every function can fail and throw, unless `noexcept` is specified, in which case we can safely expect a function not to throw.
When `noexcept` is used, a function may still fail and indicate failure through other means (eg. C-style error return codes). Although it is discouraged to fallback to such error checking mechanism, unless there is a very good reason to do so (eg in C-style callbacks which don't except a function to throw).   
For a clean and readable error checking the macro `assure` and its variants should be used.
The macro `assure` takes a condition and throws an error, when the condition is false.

Example:
```C++
void function(char *path, char *numstr){
  struct stat st = {};
  int num = 0;
  assure(!stat(&st, path)); //check that stat succeded and throw a generic error exception otherwise
  retassure(num = atoi(numstr), "atoi failed or got unexpected 0 value on str '%s'",numstr); //error checking with custom printf style error message
  reterror("shouldn't be reached"); //always throws an error when reached
}
```

Example2:
```C++
insn vmem::operator++(){
    try {
        return ++_segments.at(_segNum);
    } catch (tihmstar::out_of_range &e) { //catch custom exception
        //
    }
    /*
      An empty "//" in the catch case above
      indicates that we intentionally want to no nothing in the catch case
      and ignore the exception
    */
    retcustomassure(out_of_range, _segNum+1<_segments.size(), "overflow reached end of vmem"); //throw custom exception to allow catching very specific errors
    _segNum++;
    auto &seg = _segments.at(_segNum);
    seg = seg.base();
    return seg();
}
```

### C-style error checking
Since we can't use exceptions in C, the following construct should be used.
At first initialize a reserved variable `int err = 0;`, which will be used for error handling by the macros. At the bottom of the function declare a label `error:` which will be jumped to in case of failure, or reached on function end. Here all cleanup should be performed.
You should **not** return in the middle of the function since then resources might not get cleaned up properly, thus **always** return **only** by reaching the end of the function.
If you need to skip some part of the function, use a `goto error;` instead.
Optionally you can check `err` for a value other than `0` and perform custom error logging and do additional error case handling.   
Finally return `err` if the return value is used solely for error checking.
If the function returns a pointer on success and `err` is != `0` you shall
perform proper error cleanup and return `NULL` instead.

You should **NOT** use `err` within the function to determine cause of the error!

Example:
```C
  int somefunc(int arg1){
    int err = 0; //reserved variable named err for error checking
    char *somebuf = NULL;
    size_t somebufSize = 100;

    cassure(somebuf = malloc(somebufSize)); //c-style assure macro
    cretassure(arg1 == 123, "Error: arg1 isnt 123"); //c-style retassure macro
    creterror("we should not get here"); //c-style reterror macro

  error: //error goto label at the end of the function
    if (err){
      error("function somefunc failed with error=%d",err); //optional error logging with error() macro
    }
    safeFree(somebuf); //cleanup in every case
    return err; //return error code in any case
  }
```


## Raw memory (pointers) lifetime
Similar to regular variables, pointers should be initialized with `NULL` at the beginning of a function/scope.
Immediately after, the `cleanup` construct should be used to free/cleanup the memory.
Pointers should be declared in the order in which the allocation/resource aquisition is planned and
should be released in the **reverse** order.
All pointers which are allocated/aquired within the current scope, should be delared **above** the `cleanup` construct, while all pointers and variables which do not require cleanup (eg. constant pointers or scalar variables) should be declared **below** the `cleanup` construct.
An exception to this are variables which itself do not required cleanup, but are required to properly cleanup other variables (eg. for `void kfree(void *ptr, size_t size)`).
For cleanup the `safeFree` macro (or its corresponding variations) should be used whenever possible.

Example:
```C++
char *somefunction(int arg1){
  char *buffer = NULL; //a raw buffer
  plist_t pDict = NULL; //a special type of pointer (from libplist) which requires special cleanup
    /*
      the cleanup construct,
      which automatically executes when the current scope is left
    */
    cleanup([&]{
    //cleanup in reverse order the variables declared above

    safeFreeCustom(pDict, plist_free); //pDict needs to be freed using custom plist_free(plist_t) function
    safeFree(buffer); //buffer will be freed using default free(void*) function
  });
  char *bufCopy = NULL; //a copy of rawBuffer, which however does NOT require to be freed
  const char *someConstat = NULL; //a constant char* which doesn't need to be freed either
  size_t bufferSize = 0; //size of the raw buffer

  bufferSize = arg1*4; //assigning bufferSize after it was initialized with a default value is sometimes the prefered way

  //buffer and bufferSize are in sync!
  assure(buffer = malloc(bufferSize)); //error checking of malloc call, which throws with a generic error message when allocation fails

  retassure(pDict = plist_new_dict(), "Failed to allocate plist dict"); //error checking with custom error message

  /*
    some code working with buffer and pDict
  */

  { //special return scope, which is guaranteed not to throw
    char *ret = buffer; buffer = NULL; //transfer ownership of buffer to ret. Make sure to NULL buffer, so that i will not get freed when leaving the scope
    return ret; //return ret safely, without the cleanup function freeing the underlying buffer
  }
}
```

## Scopes
Variables which are used throughout the whole function/scope, should be declared at the top of the function/scope conforming to what was described in _Raw memory (pointers) lifetime_.
If a variable is only relevant for a few lines of code within a function, consider encapsulating it into a dedicated scope in order to not pollute the functions namespace.   
This lowers the risk for certain types of bugs (eg. where the wrong variable was assigned/used), improves readability (as it is easier to find variable declarations for relevant code)
and also helps debugging as variables which are out of scope, are not shown in the debugger and thus fewer variables need to be worried about.

Example:
```C++
plist_t Muxer::getClientPlist(Client *client) noexcept{
    plist_t p_ret = NULL;
    cleanup([&]{
        safeFreeCustom(p_ret, plist_free);
    });
    p_ret = plist_new_dict();
    /*
    ...
    */
    { //unconditional scope within a function
        /*
          variable idstring only needed for next 3 lines,
          thus declared within a dedicated scope so that
          it doesn't pollute the outer scope's namespace
        */
        std::string idstring;
        idstring = std::to_string(client->_number) +"-";
        idstring += client->getClientInfo().progName;
        plist_dict_set_item(p_ret,"ID String", plist_new_string(idstring.c_str()));
    }
    /*
    ...
    */
    {
        plist_t ret = p_ret; p_ret = NULL;
        return ret;
    }
}
```

### Emtpy scopes
Purposely empty scopes such as class constructors with no code in the function body, or empty (try-)catch constructs should be explicitly annotated with `//`.

Example:
```C++
try{
  function_that_may_throw()
}catch(...){
  //
}
/*
  Emtpy catch construct marked with "//" explicitly states that
  we purposefully ignore the case where function_that_may_throw() fails.
*/
```


## Classes
When creating classes, the access should be generally defined in the following order:
- private
- protected
- public  
If the class defines types such as `enum` or `struct`, those should be defined as `public` (or with the appropriate access) even before that.

Instance variables should be prefixed with "\_" to indicate it is a class instance variable.
These should be declared (usually) `private` or `protected`. It is discouraged to
declare instance variables `public`, instead proper getters and setters should be used when neccessary. Make sure that those can never bring your class in an *unexpected* state.

Inside the `private` (or `protected`) section variables should be declared first,
then (if applicable) member functions.

Inside the `public` section the first member function declared should be the constructor,
or static functions responsible for constructing a class instance, then the destructor.
If either of those are not `public`, these should be declared as first functions under
`private` or `protected` **after** variable declaration.

Then declare the member functions as usual, followed by `static` functions, followed by `friend`s.

Member functions which override virtual functions should use both keywords `virtual` and `override`.

Example:
```C++
class Device{
public: //class types at the very top of a class declaration declared as public
    enum mux_conn_type{
        MUXCONN_UNAVAILABLE = 0,
        MUXCONN_USB  = 1 << 0,
        MUXCONN_WIFI = 1 << 1
    };
protected://private, then protected, then public class members
    /*
      First all instance variables, which should be prefixed with "_".
      Then private/protected functions
    */
    Muxer *_muxer; //unmanaged
    mux_conn_type _conntype;
    std::atomic_bool _killInProcess;
    int _id;
    char _serial[256];

    virtual ~Device();
public:
    /*
      The first declared member function should be constructors, then destructor (if public)
    */
    Device(Muxer *mux, mux_conn_type conntype);
    Device(const Device &cpy) =delete; //delete copy constructor
    Device(Device &&mov) = delete; //move constructor

    virtual void start_connect(uint16_t dport, Client *cli) = 0;
    const char *getSerial() noexcept {return _serial;} //inline implementation ONLY for getters. In general these should be avoided as much as possible
    void kill() noexcept; //noexcept with no arguments indicates fail-safe function

    friend Muxer; //finally static functions and friends. Be careful with friends!!!
};
```

In the cpp file the functions should be implemented in the order of declaration of their corresponding header file, with the following exceptions:
- always implement the constructors first
- implement destructors immediately after constructors
- then implement private functions
- then implement protected functions
- then implement public functions

The constructor should always initialize **all** instance variables with a sane value
in the initializer part using the `:`.
The following style should be used:
- class definition then newline
- colon `:` (for noting variable initialization) followed by all the variables from header in order of declaration then newline
- opening curly bracket `{`
- constructor body or `//` when empty, then newline
- closing curly bracket `}`

Note: This is an exception of the general rule of putting the opening curly bracket `{` at the same line as the preceding closing bracket `)`.

Example:
```C++
Device::Device(Muxer *mux, mux_conn_type conntype)
    : _muxer(mux), _conntype(conntype), _killInProcess(false), _id(0), _serial{}
{
    //
}
```


## Debugging
The assure macro is defined as following:
```C++
#define assure(cond) \
  do{ \
    if ((cond) == 0)\
      throw tihmstar::EXPECTIONNAME(VERSION_COMMIT_COUNT, VERSION_COMMIT_SHA, __LINE__, __FILE__, "assure failed"); \
  } while(0)
```

With the default exception being defined as following:
```C++
namespace tihmstar {
    class exception : public std::exception{
        const char *_commit_count_str;
        const char *_commit_sha_str;
        int _line;
        std::string _filename;
        char *_err;
    public:
        exception(const char *commit_count_str, const char *commit_sha_str, int line, const char *filename, const char *err ...);
        exception(const exception &e); //copy constructor
        //custom error can be used
        virtual const char *what() const noexcept override;
        /*
         -first lowest two bytes of code is sourcecode line
         -next two bytes is strlen of filename in which error happened
         */
        int code() const;        
        virtual void dump() const;        
        //Information about build
        virtual std::string build_commit_count() const;
        virtual std::string build_commit_sha() const;        
        ~exception();
    };
};
```

The `assure` macros take care of correctly populating the exception and throwing it, in order to provide the following debug information:
- the git commit of the current version (VERSION_COMMIT_SHA)
- the total number of git commits up until this commit (VERSION_COMMIT_COUNT)
- the name of the source code file, which caused this exception
- the line in the source code file which caused this exception
- a (custom) error message of the reason this exception was thrown

It is recommended to catch the exception as `tihmstar::exception &e`.

Example:
```C++
try{
  function_that_throws();
}catch (tihmstar::exception &e){
  printf("error=%s\n", e.what()); //print error string
  printf("code=%d\n", e.code()); //print error code

  printf("build_commit_count=%s\n", e.build_commit_count().c_str()); //print number of commits up until this commit
  printf("build_commit_sha=%s\n", e.build_commit_sha().c_str()); //print current commit hash

  e.dump(); //Dumps all of the available information using printf
}
```

```C++
void exception::dump() const{
    printf("[exception]:\n");
    printf("what=%s\n",_err);
    printf("code=%d\n",code());
    printf("line=%d\n",_line);
    printf("file=%s\n",_filename.c_str());
    printf("commit count=%s:\n",build_commit_count().c_str());
    printf("commit sha  =%s:\n",build_commit_sha().c_str());
}
int exception::code() const{
    return (_line << 16) | (int)(_filename.size());
}
```

The exception can also be catched as `std::exception`, however then only `const char *what()` and `int code()` are available.
The code is expected to be 4 bytes in size and tells you the line of failure in the sourcecode in the upper 2 bytes and the length of the source filename in the lower 2 bytes.

When in a debugger it is recommended to turn on exception breakpoints, so that every failed assure which isn't caught (i.e. isn't expected to fail) causes the debugger to break at that exact line.

For standalone tools with a main function the following construct is recommended, with `main_r` being the *real* main function.
```C++
int main(int argc, const char * argv[]) {
#ifdef DEBUG
    return main_r(argc, argv);
#else
    try {
        return main_r(argc, argv);
    } catch (tihmstar::exception &e) {
        printf("%s: failed with exception:\n",PACKAGE_NAME); //package name defined by libgeneral in DEBUG builds and by autotools in RELEASE builds
        e.dump();
        return e.code();
    }
#endif
}
```
This construct aids debugging when in DEBUG builds and prints a meaningful error in RELEASE builds.
Optionally debug information can be stripped in RELEASE builds altogether, a unified concept for that might be provided in a future revision of **libgeneral**.


## Logging

The following macros are provided by **libgeneral** in the default configuration:
```C
// -- logging --
#ifndef CUSTOM_LOGGING
#define info(a ...) ({printf(a),printf("\n");})
#define warning(a ...) ({printf("[WARNING] "), printf(a),printf("\n");})
#define error(a ...) ({printf("[Error] "),printf(a),printf("\n");})
#   ifdef DEBUG
#       define debug(a ...) ({printf("[DEBUG] "),printf(a),printf("\n");})
#   else
#       define debug(a ...)
#   endif
#else
#   include CUSTOM_LOGGING
#endif
```

`info` shall be used as a general replacement for `printf` whenever *permanent* logging occurs.   
`warning` shall be used whenever a non-critical issue occured.   
`error` shall be used whenever a semi-critical or critical issue occurs.
If you want to print a critical failure message before terminating a program, this macro should be used.   
`debug` shall be used for debugging purposes only. These are disabled on RELEASE builds.

It is possible to overwrite these macros by specifying `CUSTOM_LOGGING` compiler macro,
which will be included as a file.
Thus `CUSTOM_LOGGING` should point to a header file implementing these macros.
