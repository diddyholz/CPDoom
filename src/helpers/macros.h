#ifndef MACROS_H
#define MACROS_H

#define DIRECTORY_DOOM  "\\fls0\\" APPNAME_STRING
#define DIRECTORY_WAD   DIRECTORY_DOOM "\\wad"
#define DIRECTORY_BIN   DIRECTORY_DOOM "\\bin"

// Dummy define, so that editor does not annoy me
// Set the actual appname in makefile
#ifndef APPNAME_STRING
#define APPNAME_STRING ""
#endif

#define RETURN_FAILURE(val) \
    if (val) \
    { \
        return -1; \
    }   

#endif

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)