#include "../helpers/macros.h"

#define BIN_PATH DIRECTORY_BIN "\\main.bin"

.align 4
.section .bootstrap.data
.global _bin_path
_bin_path:
    .long __main_bin
    
__main_bin:
    .string BIN_PATH
