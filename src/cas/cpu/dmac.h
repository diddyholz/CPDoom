#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../../helpers/macros.h"

// Configuration Enums
typedef enum  
{
  CMS_NORMAL        = 0x0,
  INTERMITTENT_16   = 0x2,
  INTERMITTENT_64   = 0x3,
  INTERMITTENT_256  = 0x4,
} dmac_dmaor_cms;

typedef enum  
{
  PRIORITY_0  = 0x0, // CH0 > CH1 > CH2 > CH3 > CH4 > CH5
  PRIORITY_1  = 0x1, // CH0 > CH2 > CH3 > CH1 > CH4 > CH5
  ROUND_ROBIN = 0x3,
} dmac_dmaor_pr;

// CHCR
typedef enum  
{
  REPEAT_NORMAL       = 0x0,
  REPEAT_SAR_DAR_TCR  = 0x1,
  REPEAT_DAR_TCR      = 0x2,
  REPEAT_SAR_TCR      = 0x3,
  RELOAD_SAR_DAR_TCR  = 0x5,
  RELOAD_DAR_TCR      = 0x6,
  RELOAD_SAR_TCR      = 0x7,
} dmac_chcr_rpt;

typedef enum  
{
  READ_CYCLE = 0x0,
  WRITE_CYCLE = 0x1,
} dmac_chcr_am;

typedef enum  
{
  ACTIVE_LOW = 0x0,
  ACTIVE_HIGH = 0x1,
} dmac_chcr_al;

typedef enum  
{
  SIZE_1_0    = 0x0,
  SIZE_2_0    = 0x1,
  SIZE_4_0    = 0x2,
  SIZE_16_0   = 0x3,
  SIZE_32_0   = 0x0,
  SIZE_8_0    = 0x3,
  SIZE_8x2_0  = 0x3,
  SIZE_16x2_0 = 0x0,
} dmac_chcr_ts_0;

typedef enum  
{
  SIZE_1_1    = 0x0,
  SIZE_2_1    = 0x0,
  SIZE_4_1    = 0x0,
  SIZE_16_1   = 0x0,
  SIZE_32_1   = 0x1,
  SIZE_8_1    = 0x1,
  SIZE_8x2_1  = 0x2,
  SIZE_16x2_1 = 0x3,
} dmac_chcr_ts_1;

typedef enum  
{
  DAR_FIXED_SOFT  = 0x0, // Address is fixed, but will be incremented in 16/32-byte division  
                         // transfer mode. Check SH7730 User's Manual Figure 12.3.7  
  DAR_INCREMENT   = 0x1,
  DAR_DECREMENT   = 0x2, // Prohibited in 8/16/32-byte transfer mode
  DAR_FIXED_HARD  = 0x3, // Address is fixed, even in 16/32-byte division transfer mode. 
} dmac_chcr_dm;

typedef enum  
{
  SAR_FIXED_SOFT  = 0x0, // Address is fixed, but will be incremented in 16/32-byte division  
                         // transfer mode. Check SH7730 User's Manual Figure 12.3.7  
  SAR_INCREMENT   = 0x1,
  SAR_DECREMENT   = 0x2, // Prohibited in 8/16/32-byte transfer mode
  SAR_FIXED_HARD  = 0x3, // Address is fixed, even in 16/32-byte division transfer mode. 
} dmac_chcr_sm;

typedef enum  
{
  EXTERNAL  = 0x0,  
  AUTO      = 0x4,
  DMARS     = 0x8,
} dmac_chcr_rs;

typedef enum  
{
  LOW_LEVEL     = 0x0,  
  FALLING_EDGE  = 0x1,
  HIGH_LEVEL    = 0x2,
  RISING_EDGE   = 0x3,
} dmac_chcr_dlds;

typedef enum  
{
  CYCLE_STEAL = 0x0,  
  BURST       = 0x1,
} dmac_chcr_tb;

typedef union 
{
  struct 
  {
    uint32_t        _reserved0  : 1;
    uint32_t        LCKN        : 1; // Bus Release Enable in Cycle Steal Mode
    uint32_t        _reserved1  : 2;
    dmac_chcr_rpt   RPT         : 3; // DMA Settings Renewal Specify
    uint32_t        _reserved2  : 1;
    uint32_t        DO          : 1; // DMA Overrun
    uint32_t        _reserved3  : 1;
    dmac_chcr_ts_1  TS_1        : 2; // DMA Transfer Size Specify (MS 2 bits)
    uint32_t        HE          : 1; // Half End Flag
    uint32_t        HIE         : 1; // Half End Interrupt Enable
    dmac_chcr_am    AM          : 1; // Acknowledge Mode
    dmac_chcr_al    AL          : 1; // Acknowledge Level
    dmac_chcr_dm    DM          : 2; // Destination Address Mode
    dmac_chcr_sm    SM          : 2; // Source Address Mode
    dmac_chcr_rs    RS          : 4; // Resource Select
    dmac_chcr_dlds  DLDS        : 2; // DREQ Level and Edge Select
    dmac_chcr_tb    TB          : 1; // Transfer Bus Mode
    dmac_chcr_ts_0  TS_0        : 2; // DMA Transfer Size Specify (LS 2 bits)
    uint32_t        IE          : 1; // Interrupt Enable
    uint32_t        TE          : 1; // Transfer End Flag
    uint32_t        DE          : 1; // DMA Enable
  };
  uint32_t raw;
} dmac_chcr;

typedef union 
{
  struct 
  {
    dmac_dmaor_cms CMS        : 4; // Cycle Steal Mode
    uint16_t       _reserved0 : 2;               
    dmac_dmaor_pr  PR         : 2; // Priority Mode
    uint16_t       _reserved1 : 5;
    uint16_t       AE         : 1; // Address Error Flag
    uint16_t       NMIF       : 1; // NMI Flag
    uint16_t       DME        : 1; // DMA Master Enable
  };
  uint16_t raw;
} dmac_dmaor;

// Channel 0
#define DMAC_SAR_0  ((volatile uint32_t *)0xFE008020)
#define DMAC_DAR_0  ((volatile uint32_t *)0xFE008024)
#define DMAC_TCR_0  ((volatile uint32_t *)0xFE008028)
#define DMAC_CHCR_0 ((volatile dmac_chcr *)0xFE00802C)

// Channel 1
#define DMAC_SAR_1  ((volatile uint32_t *)0xFE008030)
#define DMAC_DAR_1  ((volatile uint32_t *)0xFE008034)
#define DMAC_TCR_1  ((volatile uint32_t *)0xFE008038)
#define DMAC_CHCR_1 ((volatile dmac_chcr *)0xFE00803C)

// Common
#define DMAC_DMAOR  ((volatile dmac_dmaor *)0xFE008060)

// Channel 0 B-Registers
#define DMAC_SARB_0 ((volatile uint32_t *)0xFE008120)
#define DMAC_DARB_0 ((volatile uint32_t *)0xFE008124)
#define DMAC_TCRB_0 ((volatile uint32_t *)0xFE008128)

// Channel 1 B-Registers
#define DMAC_SARB_1 ((volatile uint32_t *)0xFE008130)
#define DMAC_DARB_1 ((volatile uint32_t *)0xFE008134)
#define DMAC_TCRB_1 ((volatile uint32_t *)0xFE008138)

/**
 * Waits for a DMA operation to complete on a channel.
 *   
 * @param chcr The control register of the channel to wait for.
 * 
 * @return Returns true on successful operation and false for an address error.
*/
inline bool dma_wait(volatile dmac_chcr *chcr) 
{
  // Check if DMA was never running
  if (unlikely(!chcr->DE || !DMAC_DMAOR->DME)) 
  {
    return true;
  }

  for (;;)
  {
    if (DMAC_DMAOR->AE) 
    {
      // Address error
      DMAC_DMAOR->AE = 0;
      return false;
    }

    if (chcr->TE) 
    {
      // Success
      return true;
    }
  }
}
