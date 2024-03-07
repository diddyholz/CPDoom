#pragma once

#include <stdint.h>

#define MMU_AREA_P0 ((void *)0x00000000)
#define MMU_AREA_P1 ((void *)0x80000000)
#define MMU_AREA_P2 ((void *)0xA0000000)
#define MMU_AREA_P3 ((void *)0xC0000000)
#define MMU_AREA_P4 ((void *)0xE0000000)

inline void *virt_to_phys_addr(void *addr)
{
  uint32_t mask = (addr < MMU_AREA_P4)? 0x1FFFFFFF : 0xFFFFFFFF;

  return (void *)((uint32_t)addr & mask);
}
