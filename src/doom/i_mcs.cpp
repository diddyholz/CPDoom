#include <sdk/os/mcs.hpp>

#include "stdio.h"

// Dummy define, so that editor does not annoy me
// Set the actual appname in makefile
#ifndef APPNAME_STRING
#define APPNAME_STRING ""
#endif

#define MCS_DIRECTORY APPNAME_STRING

uint32_t align_val(uint32_t val, uint32_t at)
{
  if ((val % at) == 0) 
  {
    return val;
  }

  return val + (at - (val % at));
}

extern "C"
void
I_McsInit(void)
{
    MCS_CreateFolder(MCS_DIRECTORY, nullptr); 
}

extern "C"
int 
I_McsWrite(const char *name, 
    const void *data, 
    uint32_t size)
{
    size = align_val(size, 4);

    int ret = MCS_SetVariable(MCS_DIRECTORY, name, VARTYPE_STR, size, (void *)data);

    if (ret)
    {
        return -1;
    }

    return 0;
}

extern "C"
int 
I_McsRead(const char *name, 
    void **data, 
    uint32_t *size)
{
    uint8_t vartype;
    char *name2;
    void *tmpdata;
    uint32_t tmpsize;

    if (!data)
    {
        data = &tmpdata;
    }

    if (!size)
    {
        size = &tmpsize;
    }

    if (MCS_GetVariable(MCS_DIRECTORY, name, &vartype, &name2, data, size))
    {
        return -1;
    }

    return 0;
}
