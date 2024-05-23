#include <appdef.hpp>
#include <initializer_list>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "cas/bootstrap.h"
#include "helpers/macros.h"

extern "C" {
    #include "doom/m_argv.h"
    #include "doom/d_main.h"
    #include "doom/d_vars.h"
}

APP_NAME(APPNAME_STRING)
APP_DESCRIPTION("Doom port for the ClassPad II")
APP_AUTHOR("diddyholz")
APP_VERSION("1.0.0")

void Main_SetupDoomArgv(int *argc, char ***argv);
void Main_SetupDoomEnvVars(void);
void test() ;

extern "C" 
int __attribute__((section(".bootstrap.text"))) main(void)
{
    if (CAS_HardwareSetup() == -1)
    {
        // If we failed to copy the binaries, the following lines will propably lead to a crash
        fprintf(stderr, "Error setting up cas hardware struct\n");
        exit(EXIT_FAILURE);
    }

    if (D_VarsInit() == -1)
    {
        fprintf(stderr, "Error initializing d_vars struct\n");
        exit(EXIT_FAILURE);
    }

    atexit(D_VarsCleanup);

    Main_SetupDoomArgv(&myargc, &myargv);
    Main_SetupDoomEnvVars();

    D_DoomMain();
    
    exit(EXIT_SUCCESS);
}

void Main_SetupDoomArgv(int *argc, char ***argv)
{
    static char cas_argv0[] = APPNAME_STRING;
    static char *cas_argv[] = {
        cas_argv0
    };

    *argc = sizeof(cas_argv) / sizeof(*cas_argv);
    *argv = cas_argv;
}

void Main_SetupDoomEnvVars(void)
{
    setenv("HOME", DIRECTORY_DOOM, true);
    setenv("DOOMWADDIR", DIRECTORY_WAD, true);
}