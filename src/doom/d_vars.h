#ifndef D_VARS_H
#define D_VARS_H

#include <stdio.h>

#include "am_map.h"
#include "m_fixed.h"
#include "d_player.h"
#include "r_defs.h"
#include "d_net.h"
#include "d_event.h"
#include "hu_stuff.h"
#include "hu_lib.h"
#include "m_misc.h"
#include "p_spec.h"
#include "p_local.h"
#include "r_bsp.h"
#include "r_data.h"
#include "st_stuff.h"
#include "st_lib.h"
#include "wi_stuff.h"
#include "w_wad.h"
#include "d_main.h"
#include "z_zone.h"

#define AM_NUMMARKPOINTS 10
#define NUMKEYS		256 
#define	BODYQUESIZE	32
#define SAVESTRINGSIZE 	24
#define MAXSPECIALCROSS		8
#define MAX_DEATHMATCH_STARTS	10
#define MAXANIMS                32
#define MAXLINEANIMS            64
#define MAXSEGS		32
#define MAXWIDTH			1120
#define MAXHEIGHT			832
#define MAXVISPLANES	128
#define MAXOPENINGS	SCREENWIDTH*64

#define DEFAULTS_MOVEMENTKEY_START 13
#define DEFAULTS_ACTIONSKEY_START  21

// A struct that holds all global, uninitialized doom variables, 
// that would normaly go into the .bss section.
// I do not have more fixed RAM locations to spare, so I will just relocate every global variable
// that I find into this fancy struct and malloc it before calling D_DoomMain().
typedef struct
{
    struct
    {
        // location of window on screen
        int 	f_x;
        int	f_y;

        // size of window on screen
        int 	f_w;
        int	f_h;

        int 	lightlev; 		// used for funky strobing effect
        byte*	fb; 			// pseudo-frame buffer
        int 	amclock;

        mpoint_t m_paninc; // how far the window pans each tic (map coords)
        fixed_t 	mtof_zoommul; // how far the window zooms in each tic (map coords)
        fixed_t 	ftom_zoommul; // how far the window zooms in each tic (fb coords)

        fixed_t 	m_x, m_y;   // LL x,y where the window is on the map (map coords)
        fixed_t 	m_x2, m_y2; // UR x,y where the window is on the map (map coords)

        //
        // width/height of window on map (map coords)
        //
        fixed_t 	m_w;
        fixed_t	m_h;

        // based on level size
        fixed_t 	min_x;
        fixed_t	min_y; 
        fixed_t 	max_x;
        fixed_t  max_y;

        fixed_t 	max_w; // max_x-min_x,
        fixed_t  max_h; // max_y-min_y

        // based on player size
        fixed_t 	min_w;
        fixed_t  min_h;


        fixed_t 	min_scale_mtof; // used to tell when to stop zooming out
        fixed_t 	max_scale_mtof; // used to tell when to stop zooming in

        // old stuff for recovery later
        fixed_t old_m_w, old_m_h;
        fixed_t old_m_x, old_m_y;

        // old location used by the Follower routine
        mpoint_t f_oldloc;

        // used by FTOM to scale from frame-buffer-to-map coords (=1/scale_mtof)
        fixed_t scale_ftom;

        player_t *plr; // the player represented by an arrow

        patch_t *marknums[10]; // numbers used for marking by the automap
        mpoint_t markpoints[AM_NUMMARKPOINTS]; // where the points are

    } am_map;

    struct 
    {
        doomcom_t	doomcom;	
        doomdata_t*	netbuffer;		// points inside doomcom

        ticcmd_t	localcmds[BACKUPTICS];

        ticcmd_t        netcmds[MAXPLAYERS][BACKUPTICS];
        int         	nettics[MAXNETNODES];
        boolean		nodeingame[MAXNETNODES];		// set false as nodes leave game
        boolean		remoteresend[MAXNETNODES];		// set when local needs tics
        int		resendto[MAXNETNODES];			// set when remote needs tics
        int		resendcount[MAXNETNODES];

        int		nodeforplayer[MAXPLAYERS];

        int             maketic;
        int		lastnettic;
        int		skiptics;
        int		ticdup;		
        int		maxsend;	// BACKUPTICS/(2*ticdup)-1
 
        boolean		reboundpacket;
        doomdata_t	reboundstore;
    } d_net;

    struct
    {
        char*		wadfiles[MAXWADFILES];

        boolean		devparm;	// started game with -devparm
        boolean         nomonsters;	// checkparm of -nomonsters
        boolean         respawnparm;	// checkparm of -respawn
        boolean         fastparm;	// checkparm of -fast

        boolean         drone;

        skill_t		startskill;
        int             startepisode;
        int		startmap;
        boolean		autostart;

        FILE*		debugfile;

        boolean		advancedemo;

        char		wadfile[1024];		// primary wad file
        char		mapdir[1024];           // directory of development maps
        char		basedefault[1024];      // default file

        event_t         events[MAXEVENTS];
        int             eventhead;
        int 		eventtail;

        int             demosequence;
        int             pagetic;
        char                    *pagename;

        //      print title for every printed line
        char            title[128];

    } d_main;

    struct
    {
        boolean	modifiedgame; // Set if homebrew PWAD stuff has been added.
    } doomstat;

    struct 
    {
        // Stage of animation:
        //  0 = text, 1 = art screen, 2 = character cast
        int		finalestage;
        int		finalecount;

        char*	finaletext;
        char*	finaleflat;
    } f_finale;

    struct
    {
        byte*	wipe_scr_start;
        byte*	wipe_scr_end;
        byte*	wipe_scr;
    } f_wipe;

    struct
    {
        gameaction_t    gameaction; 
        gamestate_t     gamestate; 
        skill_t         gameskill; 
        boolean		respawnmonsters;
        int             gameepisode; 
        int             gamemap; 
        
        boolean         paused; 
        boolean         sendpause;             	// send a pause event next tic 
        boolean         sendsave;             	// send a save event next tic 
        boolean         usergame;               // ok to save / end game 
        
        boolean         timingdemo;             // if true, exit with report on completion 
        boolean         nodrawers;              // for comparative timing purposes 
        boolean         noblit;                 // for comparative timing purposes 
        int             starttime;          	// for comparative timing purposes  	 
        
        boolean         viewactive; 
        
        boolean         deathmatch;           	// only if started as net death 
        boolean         netgame;                // only true if packets are broadcast 
        boolean         playeringame[MAXPLAYERS]; 
        player_t        players[MAXPLAYERS]; 
        
        int             consoleplayer;          // player taking events and displaying 
        int             displayplayer;          // view being displayed 
        int             gametic; 
        int             levelstarttic;          // gametic at level start 
        int             totalkills, totalitems, totalsecret;    // for intermission 
        
        char            demoname[32]; 
        boolean         demorecording; 
        boolean         demoplayback; 
        boolean		netdemo; 
        byte*		demobuffer;
        byte*		demo_p;
        byte*		demoend; 
        boolean         singledemo;            	// quit after playing a demo from cmdline 
 
        wbstartstruct_t wminfo;               	// parms for world map / intermission 
        
        short		consistancy[MAXPLAYERS][BACKUPTICS]; 
        
        byte*		savebuffer;

        // 
        // controls (have defaults) 
        // 
        int             key_right;
        int		key_left;

        int		key_up;
        int		key_down; 
        int             key_strafeleft;
        int		key_straferight; 
        int             key_fire;
        int		key_use;
        int		key_wpnext;
        int		key_wpprev;
        int		key_strafe;
        int		key_speed; 
        
        int             mousebfire; 
        int             mousebstrafe; 
        int             mousebforward; 
        
        int             joybfire; 
        int             joybstrafe; 
        int             joybuse; 
        int             joybspeed; 

        boolean         gamekeydown[NUMKEYS]; 
        int             turnheld;				// for accelerative turning 
        
        boolean		mousearray[4]; 
        boolean*	mousebuttons;		// allow [-1] INITILIAZED

        // mouse values are used once 
        int             mousex;
        int		mousey;         

        int             dclicktime;
        boolean		dclickstate;
        int		dclicks; 
        int             dclicktime2;
        boolean		dclickstate2;
        int		dclicks2;

        // joystick values are repeated 
        int             joyxmove;
        int		joyymove;
        boolean         joyarray[5]; 
        boolean*	joybuttons;		// allow [-1] INITILIAZED
 
        int		savegameslot; 
        char		savedescription[32]; 

        mobj_t*		bodyque[BODYQUESIZE]; 
        int		bodyqueslot; 
        
        void*		statcopy;				// for statistics driver

        boolean		secretexit; 
    } g_game;

    struct
    {
        char			chat_char; // remove later.
        player_t*	plr;
        patch_t*		hu_font[HU_FONTSIZE];
        hu_textline_t	w_title;
        boolean			chat_on;
        hu_itext_t	w_chat;

        char		chat_dest[MAXPLAYERS];
        hu_itext_t w_inputbuffer[MAXPLAYERS];

        boolean		message_on;
        boolean			message_dontfuckwithme;
        boolean		message_nottobefuckedwith;

        hu_stext_t	w_message;
        int		message_counter;

        const char*	shiftxform;
    } hu_stuff;

    struct
    {
        ticcmd_t	emptycmd;
    } i_system;

    struct
    {
        unsigned short cas_colors[256];
        boolean shmFinished;
    } i_video;

    struct
    {
        int    myargc;
        char** myargv;
    } m_argv;

    struct
    {
        unsigned char cheat_xlate_table[256];
    } m_cheat;

    struct
    {
        //
        // defaulted values
        //
        int			mouseSensitivity;       // has default

        // Show messages has default, 0 = off, 1 = on
        int			showMessages;
            

        // Blocky mode, has default, 0 = high, 1 = normal
        int			detailLevel;		
        int			screenblocks;		// has default

        // temp for screenblocks (0-9)
        int			screenSize;		

        // -1 = no quicksave slot picked!
        int			quickSaveSlot;          

        // 1 = message to be printed
        int			messageToPrint;
        // ...and here is the message string!
        char*			messageString;		

        // message x & y
        int			messx;			
        int			messy;
        int			messageLastMenuActive;

        // timed message = no input from user
        boolean			messageNeedsInput;     

        // we are going to be entering a savegame string
        int			saveStringEnter;              
        int             	saveSlot;	// which slot to save in
        int			saveCharIndex;	// which char we're editing
        // old save description before edit
        char			saveOldString[SAVESTRINGSIZE];  

        boolean			inhelpscreens;
        boolean			menuactive;

        char			savegamestrings[10][SAVESTRINGSIZE];

        char	endstring[160];

        short		itemOn;			// menu item skull is on
        short		skullAnimCounter;	// skull animation counter
        short		whichSkull;		// which skull to draw
    } m_menu;

    struct
    {
        int		usemouse;
        int		usejoystick;
        default_t	defaults[39];
        int	numdefaults;
        char*	defaultfile;
    } m_misc;

    struct
    {
        //
        // CEILINGS
        //
        ceiling_t*	activeceilings[MAXCEILINGS];
    } p_ceiling;

    struct 
    {
        mobj_t*		soundtarget;
        mobj_t*		corpsehit;
        mobj_t*		vileobj;
        fixed_t		viletryx;
        fixed_t		viletryy;
        mobj_t*		braintargets[32];
        int		numbraintargets;
        int		braintargeton;
    } p_enemy;

    struct 
    {
        fixed_t		tmbbox[4];
        mobj_t*		tmthing;
        int		tmflags;
        fixed_t		tmx;
        fixed_t		tmy;

        // If "floatok" true, move would be ok
        // if within "tmfloorz - tmceilingz".
        boolean		floatok;

        fixed_t		tmfloorz;
        fixed_t		tmceilingz;
        fixed_t		tmdropoffz;

        // keep track of the line that lowers the ceiling,
        // so missiles don't explode against sky hack walls
        line_t*		ceilingline;

        line_t*		spechit[MAXSPECIALCROSS];
        int		numspechit;

        mobj_t*		linetarget;	// who got hit (or NULL)
        mobj_t*		shootthing;

        // Height if not aiming up or down
        // ???: use slope for monsters?
        fixed_t		shootz;	

        int		la_damage;
        fixed_t		attackrange;

        fixed_t		aimslope;

        //
        // USE LINES
        //
        mobj_t*		usething;

        //
        // RADIUS ATTACK
        //
        mobj_t*		bombsource;
        mobj_t*		bombspot;
        int		bombdamage;

        //
        // SECTOR HEIGHT CHANGING
        // After modifying a sectors floor or ceiling height,
        // call this routine to adjust the positions
        // of all things that touch the sector.
        //
        // If anything doesn't fit anymore, true will be returned.
        // If crunch is true, they will take damage
        //  as they are being crushed.
        // If Crunch is false, you should set the sector height back
        //  the way it was and call P_ChangeSector again
        //  to undo the changes.
        //
        boolean		crushchange;
        boolean		nofit;


        //
        // SLIDE MOVE
        // Allows the player to slide along any angled walls.
        //
        fixed_t		bestslidefrac;
        fixed_t		secondslidefrac;

        line_t*		bestslideline;
        line_t*		secondslideline;

        mobj_t*		slidemo;

        fixed_t		tmxmove;
        fixed_t		tmymove;
    } p_map;

    struct 
    {
        //
        // P_LineOpening
        // Sets opentop and openbottom to the window
        // through a two sided line.
        // OPTIMIZE: keep this precalculated
        //
        fixed_t opentop;
        fixed_t openbottom;
        fixed_t openrange;
        fixed_t	lowfloor;

        //
        // INTERCEPT ROUTINES
        //
        intercept_t	intercepts[MAXINTERCEPTS];
        intercept_t*	intercept_p;

        divline_t 	trace;
        boolean 	earlyout;
        int		ptflags;
    } p_maputl;

    struct
    {
        int test;

        mapthing_t	itemrespawnque[ITEMQUESIZE];
        int		itemrespawntime[ITEMQUESIZE];
        int		iquehead;
        int		iquetail;
    } p_mobj;
    
    struct
    {
        plat_t* activeplats[MAXPLATS];
    } p_plats;
    
    struct
    {
        fixed_t		swingx;
        fixed_t		swingy;
        fixed_t		bulletslope;
    } p_pspr;

    struct
    {
        byte* save_p;

    } p_saveg;

    struct
    {
        //
        // MAP related Lookup tables.
        // Store VERTEXES, LINEDEFS, SIDEDEFS, etc.
        //
        int		numvertexes;
        vertex_t*	vertexes;

        int		numsegs;
        seg_t*		segs;

        int		numsectors;
        sector_t*	sectors;

        int		numsubsectors;
        subsector_t*	subsectors;

        int		numnodes;
        node_t*		nodes;

        int		numlines;
        line_t*		lines;

        int		numsides;
        side_t*		sides;


        // BLOCKMAP
        // Created from axis aligned bounding box
        // of the map, a rectangular array of
        // blocks of size ...
        // Used to speed up collision detection
        // by spatial subdivision in 2D.
        //
        // Blockmap size.
        int		bmapwidth;
        int		bmapheight;	// size in mapblocks
        short*		blockmap;	// int for larger maps
        // offsets in blockmap are from here
        short*		blockmaplump;		
        // origin of block map
        fixed_t		bmaporgx;
        fixed_t		bmaporgy;
        // for thing chains
        mobj_t**	blocklinks;		

        // REJECT
        // For fast sight rejection.
        // Speeds up enemy AI by skipping detailed
        //  LineOf Sight calculation.
        // Without special effect, this could be
        //  used as a PVS lookup as well.
        //
        byte*		rejectmatrix;

        // Maintain single and multi player starting spots.
        mapthing_t	deathmatchstarts[MAX_DEATHMATCH_STARTS];
        mapthing_t*	deathmatch_p;
        mapthing_t	playerstarts[MAXPLAYERS];
    } p_setup;

    struct
    {
        //
        // P_CheckSight
        //
        fixed_t		sightzstart;		// eye z of looker
        fixed_t		topslope;
        fixed_t		bottomslope;		// slopes to top and bottom of target

        divline_t	strace;			// from t1 to t2
        fixed_t		t2x;
        fixed_t		t2y;

        int		sightcounts[2];
    } p_sight;

    struct
    {
        p_anim_t		anims[MAXANIMS];
        p_anim_t*		lastanim;

        //
        //      Animating line specials
        //

        short		numlinespecials;
        line_t*		linespeciallist[MAXLINEANIMS];

        boolean		levelTimer;
        int		levelTimeCount;
    } p_spec;

    struct
    {
        int		switchlist[MAXSWITCHES * 2];
        int		numswitches;
        button_t        buttonlist[MAXBUTTONS];
    } p_switch;

    struct
    {
        int	leveltime;
        // Both the head and tail of the thinker list.
        thinker_t	thinkercap;
    } p_tick;

    struct
    {
        boolean onground;
    } p_user;

    struct
    {
        seg_t*		curline;
        side_t*		sidedef;
        line_t*		linedef;
        sector_t*	frontsector;
        sector_t*	backsector;

        drawseg_t	drawsegs[MAXDRAWSEGS];
        drawseg_t*	ds_p;

        // newend is one past the last valid seg
        cliprange_t* newend;
        cliprange_t	solidsegs[MAXSEGS];
    } r_bsp;

    struct
    {
        int		firstflat;
        int		lastflat;
        int		numflats;

        int		firstpatch;
        int		lastpatch;
        int		numpatches;

        int		firstspritelump;
        int		lastspritelump;
        int		numspritelumps;

        int		numtextures;
        texture_t**	textures;


        int*			texturewidthmask;
        // needed for texture pegging
        fixed_t*		textureheight;		
        int*			texturecompositesize;
        short**			texturecolumnlump;
        unsigned short**	texturecolumnofs;
        byte**			texturecomposite;

        // for global animation
        int*		flattranslation;
        int*		texturetranslation;

        // needed for pre rendering
        fixed_t*	spritewidth;	
        fixed_t*	spriteoffset;
        fixed_t*	spritetopoffset;

        lighttable_t	*colormaps;

        int		flatmemory;
        int		texturememory;
        int		spritememory;
    } r_data;

    struct 
    {
        byte*		viewimage; 
        int		viewwidth;
        int		scaledviewwidth;
        int		viewheight;
        int		viewwindowx;
        int		viewwindowy; 
        byte*		ylookup[MAXHEIGHT]; 
        int		columnofs[MAXWIDTH]; 

        // Color tables for different players,
        //  translate a limited part to another
        //  (color ramps used for  suit colors).
        //
        byte		translations[3][256];	

        lighttable_t*		dc_colormap; 
        int			dc_x; 
        int			dc_yl; 
        int			dc_yh; 
        fixed_t			dc_iscale; 
        fixed_t			dc_texturemid;

        // first pixel in a column (possibly virtual) 
        byte*			dc_source;		

        // just for profiling 
        int			dccount;

        byte*	dc_translation;
        byte*	translationtables;

        int			ds_y; 
        int			ds_x1; 
        int			ds_x2;

        lighttable_t*		ds_colormap; 

        fixed_t			ds_xfrac; 
        fixed_t			ds_yfrac; 
        fixed_t			ds_xstep; 
        fixed_t			ds_ystep;

        // start of a 64*64 tile image 
        byte*			ds_source;	

        // just for profiling
        int			dscount;
    } r_draw;

    struct 
    {
        int			viewangleoffset;
        lighttable_t*		fixedcolormap;

        int			centerx;
        int			centery;

        fixed_t			centerxfrac;
        fixed_t			centeryfrac;
        fixed_t			projection;

        // just for profiling purposes
        int			framecount;	

        int			sscount;
        int			linecount;
        int			loopcount;

        fixed_t			viewx;
        fixed_t			viewy;
        fixed_t			viewz;

        angle_t			viewangle;

        fixed_t			viewcos;
        fixed_t			viewsin;

        player_t*		viewplayer;

        // Blocky/low detail mode.
        //B remove this?
        //  0 = high, 1 = low
        int			detailshift;	

        //
        // precalculated math tables
        //
        angle_t			clipangle;

        // The viewangletox[viewangle + FINEANGLES/4] lookup
        // maps the visible view angles to screen X coordinates,
        // flattening the arc to a flat projection plane.
        // There will be many angles mapped to the same X. 
        int			viewangletox[FINEANGLES/2];

        // The xtoviewangleangle[] table maps a screen pixel
        // to the lowest viewangle that maps back to x ranges
        // from clipangle to -clipangle.
        angle_t			xtoviewangle[SCREENWIDTH+1];


        lighttable_t*		scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
        lighttable_t*		scalelightfixed[MAXLIGHTSCALE];
        lighttable_t*		zlight[LIGHTLEVELS][MAXLIGHTZ];

        // bumped light from gun blasts
        int			extralight;		

        boolean		setsizeneeded;
        int		setblocks;
        int		setdetail;	
    } r_main;

    struct
    {
        planefunction_t		floorfunc;
        planefunction_t		ceilingfunc;

        //
        // opening
        //

        // Here comes the obnoxious "visplane".
        visplane_t		visplanes[MAXVISPLANES];
        visplane_t*		lastvisplane;
        visplane_t*		floorplane;
        visplane_t*		ceilingplane;

        // ?
        short			openings[MAXOPENINGS];
        short*			lastopening;


        //
        // Clip values are the solid pixel bounding the range.
        //  floorclip starts out SCREENHEIGHT
        //  ceilingclip starts out -1
        //
        short			floorclip[SCREENWIDTH];
        short			ceilingclip[SCREENWIDTH];

        //
        // spanstart holds the start of a plane span
        // initialized to 0 at start
        //
        int			spanstart[SCREENHEIGHT];
        int			spanstop[SCREENHEIGHT];

        //
        // texture mapping
        //
        lighttable_t**		planezlight;
        fixed_t			planeheight;

        fixed_t			yslope[SCREENHEIGHT];
        fixed_t			distscale[SCREENWIDTH];
        fixed_t			basexscale;
        fixed_t			baseyscale;

        fixed_t			cachedheight[SCREENHEIGHT];
        fixed_t			cacheddistance[SCREENHEIGHT];
        fixed_t			cachedxstep[SCREENHEIGHT];
        fixed_t			cachedystep[SCREENHEIGHT];
    } r_plane;

    struct
    {
        // OPTIMIZE: closed two sided lines as single sided

        // True if any of the segs textures might be visible.
        boolean		segtextured;	

        // False if the back side is the same plane.
        boolean		markfloor;	
        boolean		markceiling;

        boolean		maskedtexture;
        int		toptexture;
        int		bottomtexture;
        int		midtexture;


        angle_t		rw_normalangle;
        // angle to line origin
        int		rw_angle1;	

        //
        // regular wall
        //
        int		rw_x;
        int		rw_stopx;
        angle_t		rw_centerangle;
        fixed_t		rw_offset;
        fixed_t		rw_distance;
        fixed_t		rw_scale;
        fixed_t		rw_scalestep;
        fixed_t		rw_midtexturemid;
        fixed_t		rw_toptexturemid;
        fixed_t		rw_bottomtexturemid;

        int		worldtop;
        int		worldbottom;
        int		worldhigh;
        int		worldlow;

        fixed_t		pixhigh;
        fixed_t		pixlow;
        fixed_t		pixhighstep;
        fixed_t		pixlowstep;

        fixed_t		topfrac;
        fixed_t		topstep;

        fixed_t		bottomfrac;
        fixed_t		bottomstep;


        lighttable_t**	walllights;

        short*		maskedtexturecol;
    } r_segs;

    struct
    {
        //
        // sky mapping
        //
        int			skyflatnum;
        int			skytexture;
        int			skytexturemid;
    } r_sky;

    struct
    {
        //
        // Sprite rotation 0 is facing the viewer,
        //  rotation 1 is one angle turn CLOCKWISE around the axis.
        // This is not the same as the angle,
        //  which increases counter clockwise (protractor).
        // There was a lot of stuff grabbed wrong, so I changed it...
        //
        fixed_t		pspritescale;
        fixed_t		pspriteiscale;

        lighttable_t**	spritelights;

        // constant arrays
        //  used for psprite clipping and initializing clipping
        short		negonearray[SCREENWIDTH];
        short		screenheightarray[SCREENWIDTH];


        //
        // INITIALIZATION FUNCTIONS
        //

        // variables used to look up
        //  and range check thing_t sprites patches
        spritedef_t*	sprites;
        int		numsprites;

        spriteframe_t	sprtemp[29];
        int		maxframe;
        char*		spritename;

        vissprite_t	vissprites[MAXVISSPRITES];
        vissprite_t*	vissprite_p;
        int		newvissprite;

        vissprite_t	overflowsprite;

        short*		mfloorclip;
        short*		mceilingclip;

        fixed_t		spryscale;
        fixed_t		sprtopscreen;

        vissprite_t	vsprsortedhead;
    } r_things;

    struct
    {
        patch_t* sttminus;
    } st_lib;

    struct
    { 
        // main player in game
        player_t* plyr; 

        // ST_Start() has just been called
        boolean st_firsttime;

        // lump number for PLAYPAL
        int		lu_palette;

        // used for timing
        unsigned int	st_clock;
        
        // used when in chat 
        st_chatstateenum_t	st_chatstate;

        // whether in automap or first-person
        st_stateenum_t	st_gamestate;

        // whether left-side main status bar is active
        boolean		st_statusbaron;

        // whether status bar chat is active
        boolean		st_chat;

        // value of st_chat before message popped up
        boolean		st_oldchat;

        // whether chat window has the cursor on
        boolean		st_cursoron;

        // !deathmatch
        boolean		st_notdeathmatch; 

        // !deathmatch && st_statusbaron
        boolean		st_armson;

        // !deathmatch
        boolean		st_fragson; 

        // main bar left
        patch_t*		sbar;

        // 0-9, tall numbers
        patch_t*		tallnum[10];

        // tall % sign
        patch_t*		tallpercent;

        // 0-9, short, yellow (,different!) numbers
        patch_t*		shortnum[10];

        // 3 key-cards, 3 skulls
        patch_t*		keys[NUMCARDS]; 

        // face status patches
        patch_t*		faces[ST_NUMFACES];

        // face background
        patch_t*		faceback;

        // main bar right
        patch_t*		armsbg;

        // weapon ownership patches
        patch_t*		arms[6][2]; 

        // ready-weapon widget
        st_number_t	w_ready;

        // in deathmatch only, summary of frags stats
        st_number_t	w_frags;

        // health widget
        st_percent_t	w_health;

        // arms background
        st_binicon_t	w_armsbg; 


        // weapon ownership widgets
        st_multicon_t	w_arms[6];

        // face status widget
        st_multicon_t	w_faces; 

        // keycard widgets
        st_multicon_t	w_keyboxes[3];

        // armor widget
        st_percent_t	w_armor;

        // ammo widgets
        st_number_t	w_ammo[4];

        // max ammo widgets
        st_number_t	w_maxammo[4]; 

        // number of frags so far in deathmatch
        int	st_fragscount;

        // used for evil grin
        boolean	oldweaponsowned[NUMWEAPONS]; 

        // holds key-type for each key box on bar
        int	keyboxes[3]; 

        // a random number per tick
        int	st_randomnumber;  
    } st_stuff;

    struct
    {
        // Each screen is [SCREENWIDTH*SCREENHEIGHT]; 
        // Screen 0 is the screen updated by I_Update screen.
        // Screen 1 is an extra buffer.
        byte* screens[5];	
        
        int dirtybox[4]; 
        
        int	usegamma;
    } v_video;

    struct
    {
        // Location of each lump on disk.
        lumpinfo_t*		lumpinfo;		
        int			numlumps;

        void**			lumpcache;

        int			reloadlump;
        char*			reloadname;

        int		w_info[2500][10];
        int		profilecount;
    } w_wad;

    struct
    {
        // used to accelerate or skip a stage
        int		acceleratestage;

        // wbs->pnum
        int		me;

        // specifies current state
        stateenum_t	wi_state;

        // contains information passed into intermission
        wbstartstruct_t*	wbs;

        wbplayerstruct_t* plrs;  // wbs->plyr[]

        // used for general timing
        int 		wi_cnt;  

        // used for timing of background animation
        int 		bcnt;

        // signals to refresh everything for one frame
        int 		firstrefresh; 

        int		cnt_kills[MAXPLAYERS];
        int		cnt_items[MAXPLAYERS];
        int		cnt_secret[MAXPLAYERS];
        int		cnt_time;
        int		cnt_par;
        int		cnt_pause;

        // # of commercial levels
        int		NUMCMAPS; 


        //
        //	GRAPHICS
        //

        // background (map of levels).
        patch_t*		wi_bg;

        // You Are Here graphic
        patch_t*		yah[2]; 

        // splat
        patch_t*		splat;

        // %, : graphics
        patch_t*		percent;
        patch_t*		colon;

        // 0-9 graphic
        patch_t*		wi_num[10];

        // minus sign
        patch_t*		wi_minus;

        // "Finished!" graphics
        patch_t*		finished;

        // "Entering" graphic
        patch_t*		entering; 

        // "secret"
        patch_t*		sp_secret;

        // "Kills", "Scrt", "Items", "Frags"
        patch_t*		kills;
        patch_t*		secret;
        patch_t*		items;
        patch_t*		wi_frags;

        // Time sucks.
        patch_t*		time;
        patch_t*		par;
        patch_t*		sucks;

        // "killers", "victims"
        patch_t*		killers;
        patch_t*		victims; 

        // "Total", your face, your dead face
        patch_t*		wi_total;
        patch_t*		star;
        patch_t*		bstar;

        // "red P[1..MAXPLAYERS]"
        patch_t*		wi_p[MAXPLAYERS];

        // "gray P[1..MAXPLAYERS]"
        patch_t*		wi_bp[MAXPLAYERS];

        // Name graphics of each level (centered)
        patch_t**	lnames;

        int		dm_state;
        int		dm_frags[MAXPLAYERS][MAXPLAYERS];
        int		dm_totals[MAXPLAYERS];

        int	cnt_frags[MAXPLAYERS];
        int	dofrags;
        int	ng_state;

        int	sp_state;
    } wi_stuff;

    struct
    {
        memzone_t*	mainzone;
    } z_zone;
} d_vars;

extern d_vars *md_vars;

int D_VarsInit();
void D_VarsCleanup();

#define VAR_REPLACE(scope, var) (md_vars->scope.var)

// am_map
#define m_x            VAR_REPLACE(am_map, m_x)
#define m_y            VAR_REPLACE(am_map, m_y)
#define m_w            VAR_REPLACE(am_map, m_w)
#define m_h            VAR_REPLACE(am_map, m_h)
#define m_x2           VAR_REPLACE(am_map, m_x2)
#define m_y2           VAR_REPLACE(am_map, m_y2)
#define old_m_x        VAR_REPLACE(am_map, old_m_x)
#define old_m_y        VAR_REPLACE(am_map, old_m_y)
#define old_m_w        VAR_REPLACE(am_map, old_m_w)
#define old_m_h        VAR_REPLACE(am_map, old_m_h)
#define f_x            VAR_REPLACE(am_map, f_x)
#define f_y            VAR_REPLACE(am_map, f_y)
#define f_w            VAR_REPLACE(am_map, f_w)
#define f_h            VAR_REPLACE(am_map, f_h)
#define scale_ftom     VAR_REPLACE(am_map, scale_ftom)
#define plr            VAR_REPLACE(am_map, plr)
#define markpoints     VAR_REPLACE(am_map, markpoints)
#define min_x          VAR_REPLACE(am_map, min_x)
#define min_y          VAR_REPLACE(am_map, min_y)
#define max_x          VAR_REPLACE(am_map, max_x)
#define max_y          VAR_REPLACE(am_map, max_y)
#define min_w          VAR_REPLACE(am_map, min_w)
#define min_h          VAR_REPLACE(am_map, min_h)
#define max_w          VAR_REPLACE(am_map, max_w)
#define max_h          VAR_REPLACE(am_map, max_h)
#define min_scale_mtof VAR_REPLACE(am_map, min_scale_mtof)
#define max_scale_mtof VAR_REPLACE(am_map, max_scale_mtof)
#define m_paninc       VAR_REPLACE(am_map, m_paninc)
#define f_oldloc       VAR_REPLACE(am_map, f_oldloc)
#define fb             VAR_REPLACE(am_map, fb)
#define amclock        VAR_REPLACE(am_map, amclock)
#define lightlev       VAR_REPLACE(am_map, lightlev)
#define ftom_zoommul   VAR_REPLACE(am_map, ftom_zoommul)
#define mtof_zoommul   VAR_REPLACE(am_map, mtof_zoommul)
#define marknums       VAR_REPLACE(am_map, marknums)
// end of am_map

// d_net
#define skiptics       VAR_REPLACE(d_net, skiptics)
#define nodeingame     VAR_REPLACE(d_net, nodeingame)
#define resendto       VAR_REPLACE(d_net, resendto)
#define remoteresend   VAR_REPLACE(d_net, remoteresend)
#define nodeforplayer  VAR_REPLACE(d_net, nodeforplayer)
#define maketic  VAR_REPLACE(d_net, maketic)
#define ticdup  VAR_REPLACE(d_net, ticdup)
#define netcmds  VAR_REPLACE(d_net, netcmds)
#define netbuffer  VAR_REPLACE(d_net, netbuffer)
#define doomcom  VAR_REPLACE(d_net, doomcom)
#define nettics  VAR_REPLACE(d_net, nettics)
#define localcmds  VAR_REPLACE(d_net, localcmds)
#define maxsend  VAR_REPLACE(d_net, maxsend)
#define reboundstore  VAR_REPLACE(d_net, reboundstore)
#define reboundpacket  VAR_REPLACE(d_net, reboundpacket)
#define resendcount  VAR_REPLACE(d_net, resendcount)
// end of d_net

// d_main
#define advancedemo     VAR_REPLACE(d_main, advancedemo)
#define pagetic     VAR_REPLACE(d_main, pagetic)
#define pagename     VAR_REPLACE(d_main, pagename)
#define demosequence     VAR_REPLACE(d_main, demosequence)
#define wadfiles     VAR_REPLACE(d_main, wadfiles)
#define title     VAR_REPLACE(d_main, title)
#define eventhead     VAR_REPLACE(d_main, eventhead)
#define eventtail     VAR_REPLACE(d_main, eventtail)
#define events     VAR_REPLACE(d_main, events)
#define basedefault     VAR_REPLACE(d_main, basedefault)
#define devparm     VAR_REPLACE(d_main, devparm)
#define respawnparm     VAR_REPLACE(d_main, respawnparm)
#define nomonsters     VAR_REPLACE(d_main, nomonsters)
#define debugfile     VAR_REPLACE(d_main, debugfile)
#define fastparm     VAR_REPLACE(d_main, fastparm)
#define startskill     VAR_REPLACE(d_main, startskill)
#define startmap     VAR_REPLACE(d_main, startmap)
#define startepisode     VAR_REPLACE(d_main, startepisode)
#define autostart     VAR_REPLACE(d_main, autostart)
// end of d_main

// doomstat
#define modifiedgame   VAR_REPLACE(doomstat, modifiedgame)
// end of doomstat

// f_finale
#define finaleflat   VAR_REPLACE(f_finale, finaleflat)
#define finaletext   VAR_REPLACE(f_finale, finaletext)
#define finalestage   VAR_REPLACE(f_finale, finalestage)
#define finalecount   VAR_REPLACE(f_finale, finalecount)
// end of f_finale

// f_wipe
#define wipe_scr   VAR_REPLACE(f_wipe, wipe_scr)
#define wipe_scr_start   VAR_REPLACE(f_wipe, wipe_scr_start)
#define wipe_scr_end   VAR_REPLACE(f_wipe, wipe_scr_end)
// end of f_wipe

// g_game
#define consistancy   VAR_REPLACE(g_game, consistancy)
#define gamekeydown   VAR_REPLACE(g_game, gamekeydown)
#define mousebuttons   VAR_REPLACE(g_game, mousebuttons)
#define joybuttons   VAR_REPLACE(g_game, joybuttons)
#define key_strafe   VAR_REPLACE(g_game, key_strafe)
#define mousebstrafe   VAR_REPLACE(g_game, mousebstrafe)
#define joybstrafe   VAR_REPLACE(g_game, joybstrafe)
#define key_speed   VAR_REPLACE(g_game, key_speed)
#define joybspeed   VAR_REPLACE(g_game, joybspeed)
#define joyxmove   VAR_REPLACE(g_game, joyxmove)
#define key_right   VAR_REPLACE(g_game, key_right)
#define key_left   VAR_REPLACE(g_game, key_left)
#define turnheld   VAR_REPLACE(g_game, turnheld)
#define key_up   VAR_REPLACE(g_game, key_up)
#define key_down   VAR_REPLACE(g_game, key_down)
#define joyymove   VAR_REPLACE(g_game, joyymove)
#define key_straferight   VAR_REPLACE(g_game, key_straferight)
#define key_strafeleft   VAR_REPLACE(g_game, key_strafeleft)
#define key_fire   VAR_REPLACE(g_game, key_fire)
#define mousebfire   VAR_REPLACE(g_game, mousebfire)
#define joybfire   VAR_REPLACE(g_game, joybfire)
#define key_use   VAR_REPLACE(g_game, key_use)
#define key_wpnext   VAR_REPLACE(g_game, key_wpnext)
#define key_wpprev   VAR_REPLACE(g_game, key_wpprev)
#define joybuse   VAR_REPLACE(g_game, joybuse)
#define bodyqueslot   VAR_REPLACE(g_game, bodyqueslot)
#define bodyque   VAR_REPLACE(g_game, bodyque)
#define dclicks   VAR_REPLACE(g_game, dclicks)
#define mousebforward   VAR_REPLACE(g_game, mousebforward)
#define dclickstate   VAR_REPLACE(g_game, dclickstate)
#define dclicktime   VAR_REPLACE(g_game, dclicktime)
#define dclickstate2   VAR_REPLACE(g_game, dclickstate2)
#define dclicktime2   VAR_REPLACE(g_game, dclicktime2)
#define dclicks2   VAR_REPLACE(g_game, dclicks2)
#define mousey   VAR_REPLACE(g_game, mousey)
#define mousex   VAR_REPLACE(g_game, mousex)
#define sendpause   VAR_REPLACE(g_game, sendpause)
#define sendsave   VAR_REPLACE(g_game, sendsave)
#define savegameslot   VAR_REPLACE(g_game, savegameslot)
#define starttime   VAR_REPLACE(g_game, starttime)
#define netdemo   VAR_REPLACE(g_game, netdemo)
#define savedescription   VAR_REPLACE(g_game, savedescription)
#define statcopy   VAR_REPLACE(g_game, statcopy)
#define savebuffer   VAR_REPLACE(g_game, savebuffer)
#define demo_p   VAR_REPLACE(g_game, demo_p)
#define demoend   VAR_REPLACE(g_game, demoend)
#define demoname   VAR_REPLACE(g_game, demoname)
#define demobuffer   VAR_REPLACE(g_game, demobuffer)
#define demorecording   VAR_REPLACE(g_game, demorecording)
#define gamestate     VAR_REPLACE(g_game, gamestate)
#define singledemo     VAR_REPLACE(g_game, singledemo)
#define timingdemo   VAR_REPLACE(g_game, timingdemo)
#define demoplayback   VAR_REPLACE(g_game, demoplayback)
#define levelstarttic   VAR_REPLACE(g_game, levelstarttic)
#define secretexit   VAR_REPLACE(g_game, secretexit)
#define totalkills   VAR_REPLACE(g_game, totalkills)
#define totalitems   VAR_REPLACE(g_game, totalitems)
#define totalsecret   VAR_REPLACE(g_game, totalsecret)
#define consoleplayer   VAR_REPLACE(g_game, consoleplayer)
#define gamemap   VAR_REPLACE(g_game, gamemap)
#define deathmatch   VAR_REPLACE(g_game, deathmatch)
#define netgame   VAR_REPLACE(g_game, netgame)
#define gametic   VAR_REPLACE(g_game, gametic)
#define nodrawers   VAR_REPLACE(g_game, nodrawers)
#define gameaction   VAR_REPLACE(g_game, gameaction)
#define gameepisode   VAR_REPLACE(g_game, gameepisode)
#define playeringame   VAR_REPLACE(g_game, playeringame)
#define viewactive   VAR_REPLACE(g_game, viewactive)
#define players   VAR_REPLACE(g_game, players)
#define usergame   VAR_REPLACE(g_game, usergame)
#define wminfo   VAR_REPLACE(g_game, wminfo)
#define gameskill   VAR_REPLACE(g_game, gameskill)
#define paused   VAR_REPLACE(g_game, paused)
#define displayplayer   VAR_REPLACE(g_game, displayplayer)
#define noblit   VAR_REPLACE(g_game, noblit)
#define respawnmonsters   VAR_REPLACE(g_game, respawnmonsters)
// end of g_game

// hu_stuff
#define shiftxform   VAR_REPLACE(hu_stuff, shiftxform)
#define hu_font   VAR_REPLACE(hu_stuff, hu_font)
#define message_on              VAR_REPLACE(hu_stuff, message_on)
#define message_dontfuckwithme   VAR_REPLACE(hu_stuff, message_dontfuckwithme)
#define message_nottobefuckedwith   VAR_REPLACE(hu_stuff, message_nottobefuckedwith)
#define chat_on   VAR_REPLACE(hu_stuff, chat_on)
#define w_message   VAR_REPLACE(hu_stuff, w_message)
#define w_title   VAR_REPLACE(hu_stuff, w_title)
#define w_chat   VAR_REPLACE(hu_stuff, w_chat)
#define w_inputbuffer   VAR_REPLACE(hu_stuff, w_inputbuffer)
#define chat_dest   VAR_REPLACE(hu_stuff, chat_dest)
#define message_counter   VAR_REPLACE(hu_stuff, message_counter)
// end of hu_stuff

// i_system
#define emptycmd   VAR_REPLACE(i_system, emptycmd)
// end of i_system

// i_video
#define cas_colors   VAR_REPLACE(i_video, cas_colors)
// end of i_video

// m_argv
#define myargc   VAR_REPLACE(m_argv, myargc)
#define myargv   VAR_REPLACE(m_argv, myargv)
// end of m_argv

// m_cheat
#define cheat_xlate_table   VAR_REPLACE(m_cheat, cheat_xlate_table)
// end of m_cheat

// m_menu
#define savegamestrings   VAR_REPLACE(m_menu, savegamestrings)
#define saveStringEnter   VAR_REPLACE(m_menu, saveStringEnter)
#define saveSlot   VAR_REPLACE(m_menu, saveSlot)
#define quickSaveSlot   VAR_REPLACE(m_menu, quickSaveSlot)
#define saveOldString   VAR_REPLACE(m_menu, saveOldString)
#define saveCharIndex   VAR_REPLACE(m_menu, saveCharIndex)
#define inhelpscreens   VAR_REPLACE(m_menu, inhelpscreens)
#define detailLevel   VAR_REPLACE(m_menu, detailLevel)
#define showMessages   VAR_REPLACE(m_menu, showMessages)
#define screenSize   VAR_REPLACE(m_menu, screenSize)
#define itemOn   VAR_REPLACE(m_menu, itemOn)
#define endstring   VAR_REPLACE(m_menu, endstring)
#define screenblocks   VAR_REPLACE(m_menu, screenblocks)
#define messageLastMenuActive   VAR_REPLACE(m_menu, messageLastMenuActive)
#define messageToPrint   VAR_REPLACE(m_menu, messageToPrint)
#define messageString   VAR_REPLACE(m_menu, messageString)
#define messageNeedsInput   VAR_REPLACE(m_menu, messageNeedsInput)
#define whichSkull   VAR_REPLACE(m_menu, whichSkull)
#define skullAnimCounter   VAR_REPLACE(m_menu, skullAnimCounter)
#define menuactive   VAR_REPLACE(m_menu, menuactive)
#define mouseSensitivity   VAR_REPLACE(m_menu, mouseSensitivity)
// end of m_menu

// m_misc
#define defaultfile   VAR_REPLACE(m_misc, defaultfile)
#define numdefaults   VAR_REPLACE(m_misc, numdefaults)
#define defaults   VAR_REPLACE(m_misc, defaults)
// end of m_misc

// p_ceiling
#define activeceilings   VAR_REPLACE(p_ceiling, activeceilings)
// end of p_ceiling

// p_enemy
#define soundtarget   VAR_REPLACE(p_enemy, soundtarget)
#define viletryx   VAR_REPLACE(p_enemy, viletryx)
#define viletryy   VAR_REPLACE(p_enemy, viletryy)
#define corpsehit   VAR_REPLACE(p_enemy, corpsehit)
#define vileobj   VAR_REPLACE(p_enemy, vileobj)
#define numbraintargets   VAR_REPLACE(p_enemy, numbraintargets)
#define braintargeton   VAR_REPLACE(p_enemy, braintargeton)
#define braintargets   VAR_REPLACE(p_enemy, braintargets)
// end of p_enemy

// p_map
#define tmthing   VAR_REPLACE(p_map, tmthing)
#define tmx   VAR_REPLACE(p_map, tmx)
#define tmy   VAR_REPLACE(p_map, tmy)
#define tmflags   VAR_REPLACE(p_map, tmflags)
#define tmbbox   VAR_REPLACE(p_map, tmbbox)
#define tmdropoffz   VAR_REPLACE(p_map, tmdropoffz)
#define numspechit   VAR_REPLACE(p_map, numspechit)
#define spechit   VAR_REPLACE(p_map, spechit)
#define tmymove   VAR_REPLACE(p_map, tmymove)
#define tmxmove   VAR_REPLACE(p_map, tmxmove)
#define secondslidefrac   VAR_REPLACE(p_map, secondslidefrac)
#define secondslideline   VAR_REPLACE(p_map, secondslideline)
#define slidemo   VAR_REPLACE(p_map, slidemo)
#define bestslidefrac   VAR_REPLACE(p_map, bestslidefrac)
#define bestslideline   VAR_REPLACE(p_map, bestslideline)
#define attackrange   VAR_REPLACE(p_map, attackrange)
#define shootz   VAR_REPLACE(p_map, shootz)
#define shootthing   VAR_REPLACE(p_map, shootthing)
#define aimslope   VAR_REPLACE(p_map, aimslope)
#define la_damage   VAR_REPLACE(p_map, la_damage)
#define usething   VAR_REPLACE(p_map, usething)
#define bombspot   VAR_REPLACE(p_map, bombspot)
#define bombdamage   VAR_REPLACE(p_map, bombdamage)
#define bombsource   VAR_REPLACE(p_map, bombsource)
#define nofit   VAR_REPLACE(p_map, nofit)
#define crushchange   VAR_REPLACE(p_map, crushchange)
#define floatok   VAR_REPLACE(p_map, floatok)
#define tmceilingz   VAR_REPLACE(p_map, tmceilingz)
#define linetarget   VAR_REPLACE(p_map, linetarget)
#define ceilingline   VAR_REPLACE(p_map, ceilingline)
#define tmfloorz   VAR_REPLACE(p_map, tmfloorz)
// end of p_map

// p_maputl
#define earlyout   VAR_REPLACE(p_maputl, earlyout)
#define intercept_p   VAR_REPLACE(p_maputl, intercept_p)
#define intercepts   VAR_REPLACE(p_maputl, intercepts)
#define openrange   VAR_REPLACE(p_maputl, openrange)
#define opentop   VAR_REPLACE(p_maputl, opentop)
#define openbottom   VAR_REPLACE(p_maputl, openbottom)
#define lowfloor   VAR_REPLACE(p_maputl, lowfloor)
#define trace   VAR_REPLACE(p_maputl, trace)
// end of p_maputl

// p_mobj
#define itemrespawnque   VAR_REPLACE(p_mobj, itemrespawnque)
#define itemrespawntime   VAR_REPLACE(p_mobj, itemrespawntime)
#define iquehead   VAR_REPLACE(p_mobj, iquehead)
#define iquetail   VAR_REPLACE(p_mobj, iquetail)
// end of p_mobj

// p_plats
#define activeplats   VAR_REPLACE(p_plats, activeplats)
// end of p_plats

// p_pspr
#define swingx   VAR_REPLACE(p_pspr, swingx)
#define swingy   VAR_REPLACE(p_pspr, swingy)
#define bulletslope   VAR_REPLACE(p_pspr, bulletslope)
// end of p_pspr

// p_saveg
#define save_p   VAR_REPLACE(p_saveg, save_p)
// end of p_saveg

// p_setup
#define blockmaplump   VAR_REPLACE(p_setup, blockmaplump)
#define blockmap   VAR_REPLACE(p_setup, blockmap)
#define bmaporgx   VAR_REPLACE(p_setup, bmaporgx)
#define bmaporgy   VAR_REPLACE(p_setup, bmaporgy)
#define bmapwidth   VAR_REPLACE(p_setup, bmapwidth)
#define bmapheight   VAR_REPLACE(p_setup, bmapheight)
#define blocklinks   VAR_REPLACE(p_setup, blocklinks)
#define numvertexes   VAR_REPLACE(p_setup, numvertexes)
#define vertexes   VAR_REPLACE(p_setup, vertexes)
#define numsegs   VAR_REPLACE(p_setup, numsegs)
#define lines   VAR_REPLACE(p_setup, lines)
#define numsubsectors   VAR_REPLACE(p_setup, numsubsectors)
#define subsectors   VAR_REPLACE(p_setup, subsectors)
#define segs   VAR_REPLACE(p_setup, segs)
#define sides   VAR_REPLACE(p_setup, sides)
#define numsectors   VAR_REPLACE(p_setup, numsectors)
#define sectors   VAR_REPLACE(p_setup, sectors)
#define numnodes   VAR_REPLACE(p_setup, numnodes)
#define nodes   VAR_REPLACE(p_setup, nodes)
#define numlines   VAR_REPLACE(p_setup, numlines)
#define numsides   VAR_REPLACE(p_setup, numsides)
#define rejectmatrix   VAR_REPLACE(p_setup, rejectmatrix)
#define deathmatch_p   VAR_REPLACE(p_setup, deathmatch_p)
#define playerstarts   VAR_REPLACE(p_setup, playerstarts)
#define deathmatchstarts   VAR_REPLACE(p_setup, deathmatchstarts)
// end of p_setup

// p_sight
#define strace   VAR_REPLACE(p_sight, strace)
#define t2x   VAR_REPLACE(p_sight, t2x)
#define t2y   VAR_REPLACE(p_sight, t2y)
#define sightzstart   VAR_REPLACE(p_sight, sightzstart)
#define bottomslope   VAR_REPLACE(p_sight, bottomslope)
#define topslope   VAR_REPLACE(p_sight, topslope)
#define sightcounts   VAR_REPLACE(p_sight, sightcounts)
// end of p_sight

// p_spec
#define lastanim   VAR_REPLACE(p_spec, lastanim)
#define anims   VAR_REPLACE(p_spec, anims)
#define numlinespecials   VAR_REPLACE(p_spec, numlinespecials)
#define linespeciallist   VAR_REPLACE(p_spec, linespeciallist)
#define levelTimer   VAR_REPLACE(p_spec, levelTimer)
#define levelTimeCount   VAR_REPLACE(p_spec, levelTimeCount)
// end of p_spec

// p_switch
#define numswitches   VAR_REPLACE(p_switch, numswitches)
#define switchlist   VAR_REPLACE(p_switch, switchlist)
#define buttonlist   VAR_REPLACE(p_switch, buttonlist)
// end of p_switch

// p_tick
#define thinkercap   VAR_REPLACE(p_tick, thinkercap)
#define leveltime   VAR_REPLACE(p_tick, leveltime)
// end of p_tick

// p_user
#define onground   VAR_REPLACE(p_user, onground)
// end of p_user

// r_bsp
#define solidsegs   VAR_REPLACE(r_bsp, solidsegs)
#define newend      VAR_REPLACE(r_bsp, newend)
#define ds_p   VAR_REPLACE(r_bsp, ds_p)
#define drawsegs   VAR_REPLACE(r_bsp, drawsegs)
#define curline   VAR_REPLACE(r_bsp, curline)
#define backsector   VAR_REPLACE(r_bsp, backsector)
#define frontsector   VAR_REPLACE(r_bsp, frontsector)
#define sidedef   VAR_REPLACE(r_bsp, sidedef)
#define linedef   VAR_REPLACE(r_bsp, linedef)
// end of r_bsp

// r_data
#define textures   VAR_REPLACE(r_data, textures)
#define texturecompositesize   VAR_REPLACE(r_data, texturecompositesize)
#define texturecomposite   VAR_REPLACE(r_data, texturecomposite)
#define texturecolumnlump   VAR_REPLACE(r_data, texturecolumnlump)
#define texturecolumnofs   VAR_REPLACE(r_data, texturecolumnofs)
#define texturewidthmask   VAR_REPLACE(r_data, texturewidthmask)
#define numtextures   VAR_REPLACE(r_data, numtextures)
#define lastflat   VAR_REPLACE(r_data, lastflat)
#define numflats   VAR_REPLACE(r_data, numflats)
#define flatmemory   VAR_REPLACE(r_data, flatmemory)
#define texturememory   VAR_REPLACE(r_data, texturememory)
#define spritememory   VAR_REPLACE(r_data, spritememory)
#define textureheight   VAR_REPLACE(r_data, textureheight)
#define texturetranslation   VAR_REPLACE(r_data, texturetranslation)
#define firstflat   VAR_REPLACE(r_data, firstflat)
#define flattranslation   VAR_REPLACE(r_data, flattranslation)
#define firstspritelump   VAR_REPLACE(r_data, firstspritelump)
#define lastspritelump   VAR_REPLACE(r_data, lastspritelump)
#define numspritelumps   VAR_REPLACE(r_data, numspritelumps)
#define spritewidth   VAR_REPLACE(r_data, spritewidth)
#define spriteoffset   VAR_REPLACE(r_data, spriteoffset)
#define spritetopoffset   VAR_REPLACE(r_data, spritetopoffset)
#define colormaps   VAR_REPLACE(r_data, colormaps)
// end of r_data

// r_draw
#define dc_yh   VAR_REPLACE(r_draw, dc_yh)
#define dc_yl   VAR_REPLACE(r_draw, dc_yl)
#define dc_x   VAR_REPLACE(r_draw, dc_x)
#define ylookup   VAR_REPLACE(r_draw, ylookup)
#define columnofs   VAR_REPLACE(r_draw, columnofs)
#define dc_iscale   VAR_REPLACE(r_draw, dc_iscale)
#define dc_texturemid   VAR_REPLACE(r_draw, dc_texturemid)
#define dc_colormap   VAR_REPLACE(r_draw, dc_colormap)
#define dc_source   VAR_REPLACE(r_draw, dc_source)
#define dc_translation   VAR_REPLACE(r_draw, dc_translation)
#define translationtables   VAR_REPLACE(r_draw, translationtables)
#define ds_x2   VAR_REPLACE(r_draw, ds_x2)
#define ds_x1   VAR_REPLACE(r_draw, ds_x1)
#define ds_y   VAR_REPLACE(r_draw, ds_y)
#define ds_xfrac   VAR_REPLACE(r_draw, ds_xfrac)
#define ds_yfrac   VAR_REPLACE(r_draw, ds_yfrac)
#define ds_colormap   VAR_REPLACE(r_draw, ds_colormap)
#define ds_source   VAR_REPLACE(r_draw, ds_source)
#define ds_xstep   VAR_REPLACE(r_draw, ds_xstep)
#define ds_ystep   VAR_REPLACE(r_draw, ds_ystep)
#define viewwindowx   VAR_REPLACE(r_draw, viewwindowx)
#define viewwindowy   VAR_REPLACE(r_draw, viewwindowy)
#define scaledviewwidth   VAR_REPLACE(r_draw, scaledviewwidth)
#define viewheight   VAR_REPLACE(r_draw, viewheight)
#define viewwidth   VAR_REPLACE(r_draw, viewwidth)
// end of r_draw

// r_main
#define projection   VAR_REPLACE(r_main, projection)
#define centerxfrac   VAR_REPLACE(r_main, centerxfrac)
#define centeryfrac   VAR_REPLACE(r_main, centeryfrac)
#define centerx   VAR_REPLACE(r_main, centerx)
#define centery   VAR_REPLACE(r_main, centery)
#define zlight   VAR_REPLACE(r_main, zlight)
#define setsizeneeded   VAR_REPLACE(r_main, setsizeneeded)
#define setblocks   VAR_REPLACE(r_main, setblocks)
#define scalelight   VAR_REPLACE(r_main, scalelight)
#define framecount   VAR_REPLACE(r_main, framecount)
#define viewangleoffset   VAR_REPLACE(r_main, viewangleoffset)
#define setdetail   VAR_REPLACE(r_main, setdetail)
#define extralight   VAR_REPLACE(r_main, extralight)
#define viewsin   VAR_REPLACE(r_main, viewsin)
#define viewcos   VAR_REPLACE(r_main, viewcos)
#define fixedcolormap   VAR_REPLACE(r_main, fixedcolormap)
#define scalelightfixed   VAR_REPLACE(r_main, scalelightfixed)
#define viewx   VAR_REPLACE(r_main, viewx)
#define viewy   VAR_REPLACE(r_main, viewy)
#define viewz   VAR_REPLACE(r_main, viewz)
#define viewangle   VAR_REPLACE(r_main, viewangle)
#define viewangletox   VAR_REPLACE(r_main, viewangletox)
#define xtoviewangle   VAR_REPLACE(r_main, xtoviewangle)
#define sscount   VAR_REPLACE(r_main, sscount)
#define detailshift   VAR_REPLACE(r_main, detailshift)
#define viewplayer   VAR_REPLACE(r_main, viewplayer)
#define clipangle   VAR_REPLACE(r_main, clipangle)
// end of r_main

// r_plane
#define planeheight   VAR_REPLACE(r_plane, planeheight)
#define cachedheight   VAR_REPLACE(r_plane, cachedheight)
#define cacheddistance   VAR_REPLACE(r_plane, cacheddistance)
#define cachedxstep   VAR_REPLACE(r_plane, cachedxstep)
#define cachedystep   VAR_REPLACE(r_plane, cachedystep)
#define basexscale   VAR_REPLACE(r_plane, basexscale)
#define baseyscale   VAR_REPLACE(r_plane, baseyscale)
#define planezlight   VAR_REPLACE(r_plane, planezlight)
#define lastvisplane   VAR_REPLACE(r_plane, lastvisplane)
#define visplanes   VAR_REPLACE(r_plane, visplanes)
#define openings   VAR_REPLACE(r_plane, openings)
#define spanstart   VAR_REPLACE(r_plane, spanstart)
#define yslope   VAR_REPLACE(r_plane, yslope)
#define distscale   VAR_REPLACE(r_plane, distscale)
#define floorclip   VAR_REPLACE(r_plane, floorclip)
#define ceilingclip   VAR_REPLACE(r_plane, ceilingclip)
#define lastopening   VAR_REPLACE(r_plane, lastopening)
#define floorplane   VAR_REPLACE(r_plane, floorplane)
#define ceilingplane   VAR_REPLACE(r_plane, ceilingplane)
// end of r_plane

// r_segs
#define walllights   VAR_REPLACE(r_segs, walllights)
#define maskedtexturecol   VAR_REPLACE(r_segs, maskedtexturecol)
#define rw_scalestep   VAR_REPLACE(r_segs, rw_scalestep)
#define topfrac   VAR_REPLACE(r_segs, topfrac)
#define bottomfrac   VAR_REPLACE(r_segs, bottomfrac)
#define rw_centerangle   VAR_REPLACE(r_segs, rw_centerangle)
#define rw_offset   VAR_REPLACE(r_segs, rw_offset)
#define rw_scale   VAR_REPLACE(r_segs, rw_scale)
#define midtexture   VAR_REPLACE(r_segs, midtexture)
#define rw_midtexturemid   VAR_REPLACE(r_segs, rw_midtexturemid)
#define toptexture   VAR_REPLACE(r_segs, toptexture)
#define pixhigh   VAR_REPLACE(r_segs, pixhigh)
#define pixhighstep   VAR_REPLACE(r_segs, pixhighstep)
#define rw_toptexturemid   VAR_REPLACE(r_segs, rw_toptexturemid)
#define bottomtexture   VAR_REPLACE(r_segs, bottomtexture)
#define bottomtexture   VAR_REPLACE(r_segs, bottomtexture)
#define pixlow   VAR_REPLACE(r_segs, pixlow)
#define pixlowstep   VAR_REPLACE(r_segs, pixlowstep)
#define rw_bottomtexturemid   VAR_REPLACE(r_segs, rw_bottomtexturemid)
#define maskedtexture   VAR_REPLACE(r_segs, maskedtexture)
#define topstep   VAR_REPLACE(r_segs, topstep)
#define bottomstep   VAR_REPLACE(r_segs, bottomstep)
#define worldtop   VAR_REPLACE(r_segs, worldtop)
#define worldbottom   VAR_REPLACE(r_segs, worldbottom)
#define worldhigh   VAR_REPLACE(r_segs, worldhigh)
#define worldlow   VAR_REPLACE(r_segs, worldlow)
#define worldtop   VAR_REPLACE(r_segs, worldtop)
#define rw_angle1   VAR_REPLACE(r_segs, rw_angle1)
#define rw_x   VAR_REPLACE(r_segs, rw_x)
#define rw_normalangle   VAR_REPLACE(r_segs, rw_normalangle)
#define rw_stopx   VAR_REPLACE(r_segs, rw_stopx)
#define rw_distance   VAR_REPLACE(r_segs, rw_distance)
#define markceiling   VAR_REPLACE(r_segs, markceiling)
#define markfloor   VAR_REPLACE(r_segs, markfloor)
#define segtextured   VAR_REPLACE(r_segs, segtextured)
// end of r_segs

// r_sky
#define skytexturemid   VAR_REPLACE(r_sky, skytexturemid)
#define skytexture   VAR_REPLACE(r_sky, skytexture)
#define skyflatnum   VAR_REPLACE(r_sky, skyflatnum)
// end of r_sky

// r_things
#define maxframe   VAR_REPLACE(r_things, maxframe)
#define sprtemp   VAR_REPLACE(r_things, sprtemp)
#define spritename   VAR_REPLACE(r_things, spritename)
#define negonearray   VAR_REPLACE(r_things, negonearray)
#define vissprite_p   VAR_REPLACE(r_things, vissprite_p)
#define vissprites   VAR_REPLACE(r_things, vissprites)
#define overflowsprite   VAR_REPLACE(r_things, overflowsprite)
#define sprtopscreen   VAR_REPLACE(r_things, sprtopscreen)
#define spryscale   VAR_REPLACE(r_things, spryscale)
#define mfloorclip   VAR_REPLACE(r_things, mfloorclip)
#define mceilingclip   VAR_REPLACE(r_things, mceilingclip)
#define spritelights   VAR_REPLACE(r_things, spritelights)
#define pspritescale   VAR_REPLACE(r_things, pspritescale)
#define pspriteiscale   VAR_REPLACE(r_things, pspriteiscale)
#define screenheightarray   VAR_REPLACE(r_things, screenheightarray)
#define vsprsortedhead   VAR_REPLACE(r_things, vsprsortedhead)
#define sprites   VAR_REPLACE(r_things, sprites)
#define numsprites   VAR_REPLACE(r_things, numsprites)
// end of r_things

// st_lib
#define sttminus   VAR_REPLACE(st_lib, sttminus)
// end of st_lib

// st_stuff
#define st_statusbaron   VAR_REPLACE(st_stuff, st_statusbaron)
#define sbar   VAR_REPLACE(st_stuff, sbar)
#define faceback   VAR_REPLACE(st_stuff, faceback)
#define st_gamestate   VAR_REPLACE(st_stuff, st_gamestate)
#define st_firsttime   VAR_REPLACE(st_stuff, st_firsttime)
#define plyr   VAR_REPLACE(st_stuff, plyr)
#define oldweaponsowned   VAR_REPLACE(st_stuff, oldweaponsowned)
#define st_randomnumber   VAR_REPLACE(st_stuff, st_randomnumber)
#define w_ready   VAR_REPLACE(st_stuff, w_ready)
#define keyboxes   VAR_REPLACE(st_stuff, keyboxes)
#define st_notdeathmatch   VAR_REPLACE(st_stuff, st_notdeathmatch)
#define st_armson   VAR_REPLACE(st_stuff, st_armson)
#define st_fragson   VAR_REPLACE(st_stuff, st_fragson)
#define st_fragscount   VAR_REPLACE(st_stuff, st_fragscount)
#define st_chat   VAR_REPLACE(st_stuff, st_chat)
#define st_clock   VAR_REPLACE(st_stuff, st_clock)
#define st_oldchat   VAR_REPLACE(st_stuff, st_oldchat)
#define w_ammo   VAR_REPLACE(st_stuff, w_ammo)
#define w_maxammo   VAR_REPLACE(st_stuff, w_maxammo)
#define w_health   VAR_REPLACE(st_stuff, w_health)
#define w_armor   VAR_REPLACE(st_stuff, w_armor)
#define w_armsbg   VAR_REPLACE(st_stuff, w_armsbg)
#define w_arms   VAR_REPLACE(st_stuff, w_arms)
#define w_faces   VAR_REPLACE(st_stuff, w_faces)
#define w_keyboxes   VAR_REPLACE(st_stuff, w_keyboxes)
#define w_frags   VAR_REPLACE(st_stuff, w_frags)
#define lu_palette   VAR_REPLACE(st_stuff, lu_palette)
#define tallnum   VAR_REPLACE(st_stuff, tallnum)
#define shortnum   VAR_REPLACE(st_stuff, shortnum)
#define tallpercent   VAR_REPLACE(st_stuff, tallpercent)
#define keys   VAR_REPLACE(st_stuff, keys)
#define armsbg   VAR_REPLACE(st_stuff, armsbg)
#define arms   VAR_REPLACE(st_stuff, arms)
#define faces   VAR_REPLACE(st_stuff, faces)
#define st_chatstate   VAR_REPLACE(st_stuff, st_chatstate)
#define st_cursoron   VAR_REPLACE(st_stuff, st_cursoron)
#define lu_palette   VAR_REPLACE(st_stuff, lu_palette)
// end of st_stuff

// v_video
#define dirtybox   VAR_REPLACE(v_video, dirtybox)
#define screens   VAR_REPLACE(v_video, screens)
#define usegamma   VAR_REPLACE(v_video, usegamma)
// end of v_video

// w_wad
#define reloadname   VAR_REPLACE(w_wad, reloadname)
#define reloadlump   VAR_REPLACE(w_wad, reloadlump)
#define numlumps   VAR_REPLACE(w_wad, numlumps)
#define lumpinfo   VAR_REPLACE(w_wad, lumpinfo)
#define lumpcache   VAR_REPLACE(w_wad, lumpcache)
#define w_info   VAR_REPLACE(w_wad, w_info)
#define profilecount   VAR_REPLACE(w_wad, profilecount)
// end of w_wad

// wi_stuff
#define lnames   VAR_REPLACE(wi_stuff, lnames)
#define wi_cnt   VAR_REPLACE(wi_stuff, wi_cnt)
#define finished   VAR_REPLACE(wi_stuff, finished)
#define entering   VAR_REPLACE(wi_stuff, entering)
#define bcnt   VAR_REPLACE(wi_stuff, bcnt)
#define wi_state   VAR_REPLACE(wi_stuff, wi_state)
#define wi_num   VAR_REPLACE(wi_stuff, wi_num)
#define lnames   VAR_REPLACE(wi_stuff, lnames)
#define wbs   VAR_REPLACE(wi_stuff, wbs)
#define sucks   VAR_REPLACE(wi_stuff, sucks)
#define percent   VAR_REPLACE(wi_stuff, percent)
#define colon   VAR_REPLACE(wi_stuff, colon)
#define acceleratestage   VAR_REPLACE(wi_stuff, acceleratestage)
#define wi_minus   VAR_REPLACE(wi_stuff, wi_minus)
#define splat   VAR_REPLACE(wi_stuff, splat)
#define yah   VAR_REPLACE(wi_stuff, yah)
#define plrs   VAR_REPLACE(wi_stuff, plrs)
#define dm_state   VAR_REPLACE(wi_stuff, dm_state)
#define dm_state   VAR_REPLACE(wi_stuff, dm_state)
#define cnt_pause   VAR_REPLACE(wi_stuff, cnt_pause)
#define dm_frags   VAR_REPLACE(wi_stuff, dm_frags)
#define dm_totals   VAR_REPLACE(wi_stuff, dm_totals)
#define wi_total   VAR_REPLACE(wi_stuff, wi_total)
#define killers   VAR_REPLACE(wi_stuff, killers)
#define victims   VAR_REPLACE(wi_stuff, victims)
#define wi_p   VAR_REPLACE(wi_stuff, wi_p)
#define me   VAR_REPLACE(wi_stuff, me)
#define bstar   VAR_REPLACE(wi_stuff, bstar)
#define star   VAR_REPLACE(wi_stuff, star)
#define ng_state   VAR_REPLACE(wi_stuff, ng_state)
#define cnt_kills   VAR_REPLACE(wi_stuff, cnt_kills)
#define cnt_items   VAR_REPLACE(wi_stuff, cnt_items)
#define cnt_secret   VAR_REPLACE(wi_stuff, cnt_secret)
#define cnt_frags   VAR_REPLACE(wi_stuff, cnt_frags)
#define dofrags   VAR_REPLACE(wi_stuff, dofrags)
#define kills   VAR_REPLACE(wi_stuff, kills)
#define items   VAR_REPLACE(wi_stuff, items)
#define secret   VAR_REPLACE(wi_stuff, secret)
#define wi_frags   VAR_REPLACE(wi_stuff, wi_frags)
#define sp_state   VAR_REPLACE(wi_stuff, sp_state)
#define cnt_time   VAR_REPLACE(wi_stuff, cnt_time)
#define cnt_par   VAR_REPLACE(wi_stuff, cnt_par)
#define sp_secret   VAR_REPLACE(wi_stuff, sp_secret)
#define time   VAR_REPLACE(wi_stuff, time)
#define par   VAR_REPLACE(wi_stuff, par)
#define wi_bg   VAR_REPLACE(wi_stuff, wi_bg)
#define NUMCMAPS   VAR_REPLACE(wi_stuff, NUMCMAPS)
#define wi_bp   VAR_REPLACE(wi_stuff, wi_bp)
#define firstrefresh   VAR_REPLACE(wi_stuff, firstrefresh)
#define secret   VAR_REPLACE(wi_stuff, secret)
#define secret   VAR_REPLACE(wi_stuff, secret)
#define secret   VAR_REPLACE(wi_stuff, secret)
// end of wi_stuff

// z_zone
#define mainzone   VAR_REPLACE(z_zone, mainzone)
// end of z_zone
#endif