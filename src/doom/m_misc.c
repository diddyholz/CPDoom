// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
//
// $Log:$
//
// DESCRIPTION:
//	Main loop menu stuff.
//	Default Config File.
//	PCX Screenshots.
//
//-----------------------------------------------------------------------------

//static const char
//rcsid[] = "$Id: m_misc.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>


#include "doomdef.h"

#include "d_vars.h"

#include "z_zone.h"

#include "m_swap.h"
#include "m_argv.h"

#include "w_wad.h"

#include "i_mcs.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#include "hu_stuff.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"

#include "m_misc.h"

#define CONFIG_STR_LEN  0x800
#define CONFIG_LINE_LEN 0xFF

//
// M_DrawText
// Returns the final X coordinate
// HU_Init must have been called to init the font
//
int
M_DrawText
( int		x,
  int		y,
  boolean	direct,
  char*		string )
{
    int 	c;
    int		w;

    while (*string)
    {
	c = toupper(*string) - HU_FONTSTART;
	string++;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    x += 4;
	    continue;
	}
		
	w = SHORT (hu_font[c]->width);
	if (x+w > SCREENWIDTH)
	    break;
	if (direct)
	    V_DrawPatchDirect(x, y, 0, hu_font[c]);
	else
	    V_DrawPatch(x, y, 0, hu_font[c]);
	x+=w;
    }

    return x;
}




//
// M_WriteFile
//
#ifndef O_BINARY
#define O_BINARY 0
#endif

// This wont actually write a file, but instead write to an mcs var
boolean
M_WriteFile
( char const*	name,
  void*		source,
  int		length )
{
    if (I_McsWrite(name, source, length) < 0)
    {
        return false;
    }

    return true;
}

//
// M_ReadFile
//
// Again, this won't actually read a file, but an MCS variable
int
M_ReadFile
( char const*	name,
  byte**	buffer )
{
    uint32_t size = 0;

    if (I_McsRead(name, (void**)buffer, &size))
    {
        I_Error("Couldn't read MCS variable: %s", name);
    }

    return size;
}

// UNIX hack, to be removed.
#ifdef SNDSERV
extern char*	sndserver_filename;
extern int	mb_used;
#endif

#ifdef LINUX
char*		mousetype;
char*		mousedev;
#endif

extern char*	chat_macros[];

//
// M_SaveDefaults
//
void M_SaveDefaults (void)
{
    int		i;
    int		v;
    char *configstr = malloc(CONFIG_STR_LEN);
    memset(configstr, 0, CONFIG_STR_LEN);

    if (!configstr)
	    return; // can't write the file, but don't complain

    configstr[0] = '\0';
		
    for (i=0 ; i<numdefaults ; i++)
    {
        char line[CONFIG_LINE_LEN];

        if (defaults[i].defaultvalue > -0xfff
            && defaults[i].defaultvalue < 0xfff)
        {
            v = *defaults[i].location;
            sprintf (line,"%s %i\n",defaults[i].name,v);
        } else {
            sprintf (line,"%s \"%s\"\n",defaults[i].name,
                * (char **) (defaults[i].location));
        }

        strcat (configstr, line);
    }

    I_McsInit();
    I_McsWrite(defaultfile, configstr, CONFIG_STR_LEN);
    free (configstr);
}

void M_Cleanup (void)
{
    printf("M_Cleanup: Free config memory\n");
    
    for (int i = 0; i < numdefaults; i++)
    {
        // Check if string or integer value
        if (*defaults[i].location > -0xFFF 
            && *defaults[i].location < 0xFFF)
        {
            continue;
        }

        free((void*)*defaults[i].location);
    }
}

//
// M_LoadDefaults
//
extern byte	scantokey[128];

void M_LoadDefaults (void)
{
    int		    i;
    int		    len;
    char	    def[80];
    char	    strparm[100];
    char*	    newstring;
    int		    parm;
    boolean	    isstring;
    const char* configstr;
    uint32_t    size;
    
    I_McsInit();
    
    // set everything to base values
    numdefaults = sizeof(defaults)/sizeof(defaults[0]);
    for (i=0 ; i<numdefaults ; i++)
	    *defaults[i].location = defaults[i].defaultvalue;
    
    // check for a custom default file
    i = M_CheckParm ("-config");
    if (i && i<myargc-1)
    {
        defaultfile = myargv[i+1];
        printf ("	default file: %s\n",defaultfile);
    }
    else
	    defaultfile = basedefault;

    // read the file in, overriding any set defaults
    if (I_McsRead(defaultfile, (void**)&configstr, &size) < 0)
    {
        return;
    }

    // Go through every line
    while (configstr[0])
    {
        isstring = false;
        if (sscanf (configstr, "%79s %[^\n]\n", def, strparm) == 2)
        {
            if (strparm[0] == '"')
            {
                // get a string default
                isstring = true;
                len = strlen(strparm);
                newstring = (char *) malloc(len);
                strparm[len-1] = 0;
                strcpy(newstring, strparm+1);
            }
            else if (strparm[0] == '0' && strparm[1] == 'x')
                sscanf(strparm+2, "%x", &parm);
            else
                sscanf(strparm, "%i", &parm);
            for (i=0 ; i<numdefaults ; i++)
                if (!strcmp(def, defaults[i].name))
                {
                    if (!isstring)
                        *defaults[i].location = parm;
                    else
                    {
                        *defaults[i].location =
                        (int) newstring;
                    }
                    break;
                }
        }

        configstr = strchr(configstr, '\n') + 1;
    }

    atexit(M_Cleanup);
}


//
// SCREEN SHOTS
//


typedef struct
{
    char		manufacturer;
    char		version;
    char		encoding;
    char		bits_per_pixel;

    unsigned short	xmin;
    unsigned short	ymin;
    unsigned short	xmax;
    unsigned short	ymax;
    
    unsigned short	hres;
    unsigned short	vres;

    unsigned char	palette[48];
    
    char		reserved;
    char		color_planes;
    unsigned short	bytes_per_line;
    unsigned short	palette_type;
    
    char		filler[58];
    unsigned char	data;		// unbounded
} pcx_t;


//
// WritePCXfile
//
void
WritePCXfile
( char*		filename,
  byte*		data,
  int		width,
  int		height,
  byte*		palette )
{
    int		i;
    int		length;
    pcx_t*	pcx;
    byte*	pack;
	
    pcx = Z_Malloc (width*height*2+1000, PU_STATIC, NULL);

    pcx->manufacturer = 0x0a;		// PCX id
    pcx->version = 5;			// 256 color
    pcx->encoding = 1;			// uncompressed
    pcx->bits_per_pixel = 8;		// 256 color
    pcx->xmin = 0;
    pcx->ymin = 0;
    pcx->xmax = SHORT(width-1);
    pcx->ymax = SHORT(height-1);
    pcx->hres = SHORT(width);
    pcx->vres = SHORT(height);
    memset (pcx->palette,0,sizeof(pcx->palette));
    pcx->color_planes = 1;		// chunky image
    pcx->bytes_per_line = SHORT(width);
    pcx->palette_type = SHORT(2);	// not a grey scale
    memset (pcx->filler,0,sizeof(pcx->filler));


    // pack the image
    pack = &pcx->data;
	
    for (i=0 ; i<width*height ; i++)
    {
	if ( (*data & 0xc0) != 0xc0)
	    *pack++ = *data++;
	else
	{
	    *pack++ = 0xc1;
	    *pack++ = *data++;
	}
    }
    
    // write the palette
    *pack++ = 0x0c;	// palette ID byte
    for (i=0 ; i<768 ; i++)
	*pack++ = *palette++;
    
    // write output file
    length = pack - (byte *)pcx;
    M_WriteFile (filename, pcx, length);

    Z_Free (pcx);
}


//
// M_ScreenShot
//
void M_ScreenShot (void)
{
    int		i;
    byte*	linear;
    char	lbmname[12];
    
    // munge planar buffer to linear
    linear = screens[2];
    I_ReadScreen (linear);
    
    // find a file name to save it to
    strcpy(lbmname,"DOOM00.pcx");
		
    for (i=0 ; i<=99 ; i++)
    {
	lbmname[4] = i/10 + '0';
	lbmname[5] = i%10 + '0';
	if (access(lbmname,0) == -1)
	    break;	// file doesn't exist
    }
    if (i==100)
	I_Error ("M_ScreenShot: Couldn't create a PCX");
    
    // save the pcx file
    WritePCXfile (lbmname, linear,
		  SCREENWIDTH, SCREENHEIGHT,
		  W_CacheLumpName ("PLAYPAL",PU_CACHE));
	
    players[consoleplayer].message = "screen shot";
}


