#ifndef __I_MCS_H__
#define __I_MCS_H__

#include <stdint.h>

int I_McsWrite(const char *name, const void *data, uint32_t size);
int I_McsRead(const char *name, void **data, uint32_t *size);
void I_McsInit(void);

#endif