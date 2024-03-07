#pragma once

#include <stdint.h>

// SH7724 Clock Pulse Generator 

enum cpg_frqcra_stc
{
  PLL_MUL_12 = 0x05,
  PLL_MUL_16 = 0x07,
  PLL_MUL_24 = 0x0B,
  PLL_MUL_30 = 0x0E,
  PLL_MUL_32 = 0x0F,
  PLL_MUL_36 = 0x11,
  PLL_MUL_48 = 0x17
};

enum cpg_frqcra_ifc
{
  IFC_DIV_2  = 0x0,
  IFC_DIV_3  = 0x1,
  IFC_DIV_4  = 0x2,
  IFC_DIV_6  = 0x3,
  IFC_DIV_8  = 0x4,
  IFC_DIV_12 = 0x5,
  IFC_DIV_16 = 0x6,
  IFC_DIV_24 = 0x8,
  IFC_DIV_32 = 0x9,
  IFC_DIV_36 = 0xA,
  IFC_DIV_48 = 0xB,
  IFC_DIV_72 = 0xD
};

enum cpg_frqcra_sfc
{
  SFC_DIV_4  = 0x2,
  SFC_DIV_6  = 0x3,
  SFC_DIV_8  = 0x4,
  SFC_DIV_12 = 0x5,
  SFC_DIV_16 = 0x6,
  SFC_DIV_24 = 0x8,
  SFC_DIV_32 = 0x9,
  SFC_DIV_36 = 0xA,
  SFC_DIV_48 = 0xB,
  SFC_DIV_72 = 0xD
};

enum cpg_frqcra_bfc
{
  BFC_DIV_4  = 0x2,
  BFC_DIV_6  = 0x3,
  BFC_DIV_8  = 0x4,
  BFC_DIV_12 = 0x5,
  BFC_DIV_16 = 0x6,
  BFC_DIV_24 = 0x8,
  BFC_DIV_32 = 0x9,
  BFC_DIV_36 = 0xA,
  BFC_DIV_48 = 0xB,
  BFC_DIV_72 = 0xD
};

enum cpg_frqcra_p1fc
{
  P1FC_DIV_4  = 0x2,
  P1FC_DIV_6  = 0x3,
  P1FC_DIV_8  = 0x4,
  P1FC_DIV_12 = 0x5,
  P1FC_DIV_16 = 0x6,
  P1FC_DIV_24 = 0x8,
  P1FC_DIV_32 = 0x9,
  P1FC_DIV_36 = 0xA,
  P1FC_DIV_48 = 0xB,
  P1FC_DIV_72 = 0xD
};

// Frequency control register A
union cpg_frqcra
{
  struct
  {
    uint32_t KICK        : 1;
    uint32_t _reserved0  : 1;
    cpg_frqcra_stc STC   : 6; // PLL Circuit Multiplication Ratio
    cpg_frqcra_ifc IFC   : 4; // CPU Clock (I-Phi) Frequency Division Ratio
    uint32_t _reserved1  : 4;
    cpg_frqcra_sfc SFC   : 4; // SuperHy Clock (S-Phi) Frequency Division Ratio
    cpg_frqcra_bfc BFC   : 4; // Bus Clock (B-Phi) Frequency Division Ratio
    uint32_t _reserved2  : 4;
    cpg_frqcra_p1fc P1FC : 4; // Peripheral Clock (P-Phi) Frequency Division Ratio
  };

  uint32_t raw;
};

// Frequency change status register
union cpg_lstatus
{
  struct
  {
    uint32_t _reserved0 : 31;
    uint32_t FRQF       : 1; // Frequency changing status flag
  };

  uint32_t raw;
};

#define CPG_FRQCRA          ((volatile cpg_frqcra *) 0xA4150000)
#define CPG_LSTATUS         ((volatile cpg_lstatus *)0xA4150060)

#define CPG_PLL_MUL_DEFAULT PLL_MUL_32

inline void cpg_frqf_wait()
{
  while (CPG_LSTATUS->FRQF == 1) {}
}

inline void cpg_set_pll_mul(cpg_frqcra_stc multiplier)
{
  // Set new PLL multiplier and activate it
  CPG_FRQCRA->STC = multiplier;
  CPG_FRQCRA->KICK = 1;

  // Wait until frequency change is completed
  cpg_frqf_wait();
}
