#include "d_vars.h"

#include <stdlib.h>
#include <string.h>

#include "doomstat.h"
#include "d_englsh.h"

d_vars *md_vars;

extern int numChannels;
extern char*	chat_macros[];

void D_SetSpecialInitialized(void)
{
    mousebuttons = &md_vars->g_game.mousearray[1];
    joybuttons = &md_vars->g_game.joyarray[1];
    
    const default_t _defaults[] =
    {
        {"mouse_sensitivity",&mouseSensitivity, 5},
        {"sfx_volume",&snd_SfxVolume, 8},
        {"music_volume",&snd_MusicVolume, 8},
        {"show_messages",&showMessages, 1},

        {"use_mouse",&md_vars->m_misc.usemouse, 0},
        {"mouseb_fire",&mousebfire,0},
        {"mouseb_strafe",&mousebstrafe,1},
        {"mouseb_forward",&mousebforward,2},

        {"use_joystick",&md_vars->m_misc.usejoystick, 0},
        {"joyb_fire",&joybfire,0},
        {"joyb_strafe",&joybstrafe,1},
        {"joyb_use",&joybuse,3},
        {"joyb_speed",&joybspeed,2},

        {"key_up",&key_up, '8'},
        {"key_down",&key_down, '5'},
        {"key_right",&key_right, '6'},
        {"key_left",&key_left, '4'},
        {"key_straferight",&key_straferight, '9'},
        {"key_strafeleft",&key_strafeleft, '7'},
        {"key_speed",&key_speed, '+'},
        {"key_strafe",&key_strafe, KEY_RCTRL},

        {"key_fire",&key_fire, ' '},
        {"key_use",&key_use, '-'},
        {"key_wpnext",&key_wpnext, '3'},
        {"key_wpprev",&key_wpprev, '1'},

        {"screenblocks",&screenblocks, 9},
        {"detaillevel",&detailLevel, 0},

        {"snd_channels",&numChannels, 3},

        {"usegamma",&usegamma, 0},

        {"chatmacro0", (int *) &chat_macros[0], (int) HUSTR_CHATMACRO0 },
        {"chatmacro1", (int *) &chat_macros[1], (int) HUSTR_CHATMACRO1 },
        {"chatmacro2", (int *) &chat_macros[2], (int) HUSTR_CHATMACRO2 },
        {"chatmacro3", (int *) &chat_macros[3], (int) HUSTR_CHATMACRO3 },
        {"chatmacro4", (int *) &chat_macros[4], (int) HUSTR_CHATMACRO4 },
        {"chatmacro5", (int *) &chat_macros[5], (int) HUSTR_CHATMACRO5 },
        {"chatmacro6", (int *) &chat_macros[6], (int) HUSTR_CHATMACRO6 },
        {"chatmacro7", (int *) &chat_macros[7], (int) HUSTR_CHATMACRO7 },
        {"chatmacro8", (int *) &chat_macros[8], (int) HUSTR_CHATMACRO8 },
        {"chatmacro9", (int *) &chat_macros[9], (int) HUSTR_CHATMACRO9 }
    };

    memcpy(defaults, _defaults, sizeof(_defaults));
}

int D_VarsInit()
{
    md_vars = malloc(sizeof(d_vars));
    
    if (!md_vars)
    {
        return -1;
    }

    memset(md_vars, 0, sizeof(d_vars));

    D_SetSpecialInitialized();

    return 0;
}

void D_VarsCleanup()
{
    free(md_vars);
}
