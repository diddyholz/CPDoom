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
// $Log:$
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------

//static const char
//rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>

#include "d_vars.h"
#include "i_system.h"
#include "d_event.h"
#include "d_net.h"
#include "m_argv.h"

#include "doomstat.h"

#ifdef __GNUG__
#pragma implementation "i_net.h"
#endif
#include "i_net.h"


//
// I_InitNetwork
//
void I_InitNetwork (void)
{	
    doomcom.netticdup = 1;
	
    if (M_CheckParm ("-extratic"))
	    doomcom.extratics = 1;
    else
	    doomcom.extratics = 0;

	netgame = false;
	doomcom.id = DOOMCOM_ID;
	doomcom.numplayers = doomcom.nnodes = 1;
	doomcom.netdeathmatch = false;
	doomcom.consoleplyr = 0;
}

