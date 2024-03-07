#include "bootstrap.h"

#include <sdk/os/file.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include <sdk/calc/calc.hpp>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpu/dmac.h"
#include "../helpers/macros.h"

#define LCD_WIDTH       320
#define LCD_HEIGHT      528
#define LCD_FRAMEBUFFER ((uint16_t *)0x8C000000)

#define BIN_ADDRESS ((void *)0x8C052800) // VRAM  Backup (Yeah this is not good)

extern const char *bin_path;

extern uint8_t actualbssstart;
extern uint8_t actualbssend;

uint16_t *vram_bak __attribute__((section(".bootstrap.data")));

int __attribute__((section(".bootstrap.text"))) CAS_LoadBin() 
{
    struct File_Stat st;

    int tmp = File_Stat(bin_path, &st);

    RETURN_FAILURE(tmp < 0)

    int fd = File_Open(bin_path, FILE_OPEN_READ);

    RETURN_FAILURE(fd < 0)

    tmp = File_Read(fd, BIN_ADDRESS, st.fileSize);

    if (tmp < 0)
    {
        File_Close(fd);
        return -1;
    }

    File_Close(fd);

    return 0;
}

void CAS_ClearBss(void)
{
    Mem_Memset(&actualbssstart, 0, &actualbssend - &actualbssstart);
}

int CAS_BackupAndFillScreen(void)
{
    vram_bak = (uint16_t *)Mem_Malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(*vram_bak));
    
    if (!vram_bak)
    {
        return -1;
    }
    
    Mem_Memcpy(vram_bak, LCD_FRAMEBUFFER, LCD_WIDTH * LCD_HEIGHT * sizeof(*vram_bak));
    Mem_Memset(LCD_FRAMEBUFFER, 0, LCD_WIDTH * LCD_HEIGHT * sizeof(*LCD_FRAMEBUFFER));

    return 0;
}

void __attribute__((section(".bootstrap.text"))) CAS_Cleanup(void) 
{
    Mem_Memcpy(LCD_FRAMEBUFFER, vram_bak, LCD_WIDTH * LCD_HEIGHT * sizeof(*LCD_FRAMEBUFFER));
    LCD_Refresh();
    Mem_Free(vram_bak);    
}

int __attribute__((section(".bootstrap.text"))) CAS_HardwareSetup(void)
{
    RETURN_FAILURE(CAS_LoadBin())

    CAS_ClearBss();

    RETURN_FAILURE(CAS_BackupAndFillScreen())

    atexit(CAS_Cleanup);

    return 0;
}
