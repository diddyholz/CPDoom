#pragma once

#include <stdint.h>

#define TMU_TICKS_PER_SEC 7000000

enum tmu_tcr_tpsc 
{
  PHI_DIV_4     = 0x0,
  PHI_DIV_16    = 0x1,
  PHI_DIV_64    = 0x2,
  PHI_DIV_256   = 0x3,
  PHI_DIV_1024  = 0x4
};

union tmu_tstr
{
  struct 
  {
    uint8_t _reserved0  : 5;
    uint8_t STR2        : 1;
    uint8_t STR1        : 1;
    uint8_t STR0        : 1;
  };

  uint8_t raw;
};

union tmu_tcr
{
  struct 
  {
    uint16_t _reserved0 : 7;
    uint16_t UNF        : 1; // Underflow Flag     
    uint16_t _reserved1 : 2;
    uint16_t UNIE       : 1; // Underflow Interrupt Enable
    uint16_t _reserved2 : 2;
    tmu_tcr_tpsc TPSC   : 3; // Timer Prescaler
  };

  uint16_t raw;
};

// General register
#define TMU_TSTR    ((volatile tmu_tstr *) 0xA4490004)

// Channel 0
#define TMU_TCOR_0  ((volatile uint32_t *) 0xA4490008)
#define TMU_TCNT_0  ((volatile uint32_t *) 0xA449000C)
#define TMU_TCR_0   ((volatile tmu_tcr *)  0xA4490010)

// Channel 1
#define TMU_TCOR_1  ((volatile uint32_t *) 0xA4490014)
#define TMU_TCNT_1  ((volatile uint32_t *) 0xA4490018)
#define TMU_TCR_1   ((volatile tmu_tcr *)  0xA449001C)

// Channel 2
#define TMU_TCOR_2  ((volatile uint32_t *) 0xA4490020)
#define TMU_TCNT_2  ((volatile uint32_t *) 0xA4490024)
#define TMU_TCR_2   ((volatile tmu_tcr *)  0xA4490028)