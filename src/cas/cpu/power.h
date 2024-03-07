#pragma once

#include <stdint.h>

union power_mstpcr0 
{
  struct
  {
    uint32_t TLB        : 1;
    uint32_t IC         : 1;
    uint32_t OC         : 1;
    uint32_t _reserved0 : 1;
    uint32_t IL         : 1;
    uint32_t _reserved1 : 2;
    uint32_t FPU        : 1;
    uint32_t _reserved2 : 1;
    uint32_t INTC       : 1;
    uint32_t DMAC       : 1;
    uint32_t _reserved3 : 1;
    uint32_t HUDI       : 1;
    uint32_t DBG        : 1;
    uint32_t UBC        : 1;
    uint32_t SUBC       : 1;
    uint32_t TMU        : 1;
    uint32_t CMT        : 1;
    uint32_t RWDT       : 1;
    uint32_t _reserved4 : 3;
    uint32_t SCIF4      : 1;
    uint32_t SCIF5      : 1;
    uint32_t SCIF0      : 1;
    uint32_t SCIF1      : 1;
    uint32_t SCIF2      : 1;
    uint32_t SCIF3      : 1;
    uint32_t _reserved5 : 1;
    uint32_t SIOF       : 1;
    uint32_t _reserved6 : 2;
  };
  
  uint32_t raw;
};

#define POWER_MSTPCR0 ((volatile power_mstpcr0 *)0xA4150030)
