/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static unsigned int borderpx        = 1;         /* border pixel of windows */
static const unsigned int gappx     = 35;        /* gaps between windows */
static unsigned int snap            = 4;         /* snap pixel */
static int showbar                  = 1;         /* 0 means no bar */
static int topbar                   = 1;         /* 0 means bottom bar */
static char font[]                  = "RobotoMono Nerd Font Mono:style=Bold:size=11:antialias=true:autohint=true";
static char dmenufont[]             = "monospace:size=10";
static const char *fonts[]          = { font, "JetBrainsMonoNL NF:style=Regular:size=11:antialias=true:autohint=true" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,        50,50,500,500,        5 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,        50,50,500,500,        5 },
	{ NULL,	      NULL,	  "Picture in picture", (1 << 8) - 1, 1, -1,	    50,50,500,500,	  0 },

	// float rules
	{ "flt",      NULL,	  NULL,	      0,	    1,		 -1,	    -1,-1,-1,-1,	  -1 },
	{ "nflt",     NULL,	  NULL,	      0,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

	// nvim*
	// 160x42+100+100
	//{ "neovide",  NULL,	  NULL,	      0,	    1,		 -1,	    100,100,-1,-1,	  -1 },

	// Minecraft 
    	{ "Minecraft Launcher", NULL, NULL,   1 << 2,	    0,		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 3
	{ "Modrinth-app", NULL,	  NULL,	      1 << 2,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

   	// Steam 
    	{ "steam",    NULL, 	  NULL,       1 << 2, 	    0, 		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 3
	{ NULL,	      NULL,	  "Steam",    1 << 2,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },
	{ "steamwebhelper", NULL, NULL,	      1 << 2,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

   	// Vivaldi & Vivaldi Stable
  	{ "Vivaldi",  NULL,	  NULL,	      1 << 1,	    0,           -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 2
	{ "Vivaldi-stable", NULL, NULL,	      1 << 1,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

   	// Discord
	{ "discord",  NULL,	  NULL,	      1 << 8,	    0,		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 9
   	{ "vesktop",  NULL, 	  NULL,	      1 << 8,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

   	// Media
  	{ "vlc",      NULL,	  NULL,	      1 << 4,	    0,		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 5
	{ NULL,	      NULL,	  "player",   1 << 4,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },
   	{ "Spotify",  NULL,	  NULL,	      1 << 7, 	    0,		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 8
	{ "Sxiv",     NULL,	  NULL,	      0,	    1,		 -1,	    520,150,800,800,	  -1 },
	
	{ "jetbrains-toolbox", NULL, NULL,    1 << 0,	    1,		 -1,	    -1,-1,-1,-1,	  -1 }, //  Tag 1
	{ "jetbrains-idea", NULL, NULL,	      1 << 0,	    0,		 -1,	    -1,-1,-1,-1,	  -1 },

	// etc programs/windows
	{ "swatch",  NULL,	 NULL,	      0,	    1,		 -1,	    150,400,1400,300,	  -1 },
	{ NULL,	     NULL,	 "MiniPlayer", 0,	    1,		 -1,	    -1,-1,-1,-1,	  -1 },
	{ "scrcpy",  NULL,	 NULL,	      0,	    1,		 -1,	    -1,-1,-1,-1,	  -1 },
	{ "Gedit",   NULL,	 NULL,	      0,	    1,		 -1,	    -1,-1,-1,-1,	  -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };

// terminal
static const char *termcmd[]  = { "st", NULL };
static const char *termfloat[] = { "st", "-c", "flt", "-g", "80x24+677+65", NULL };

// terminal++
//  neovide
//static const char *flttermnvim[] = { "st", "-c", "flt", "-g", "160x42+100+100", "-e", "nvim", NULL };
//static const char *flttermnvim[] = { "neovide", "--x11-wm-class", "flt", "--size", "160x42", NULL };
static const char *termnvim[] = { "neovide", "--x11-wm-class", "nflt", NULL };
// --x11-wm-class-instance is probably better than --x11-wm-class, but the latter works a lot better
// NVM, just straight up borked I guess
//  colorpicker (WIP(?)(BORKED ASF RN))
//static const char *colorpick[] = { "st", "-c", "flt", "-g", "42x8+48-47", "-e", "\"PS1='' && colorpicker && read\"" };

// clipboard
static const char *clipcmd[] = { "clipmenu", "-b", "-fn", dmenufont, NULL };
static const char *scrclip[] = { "sh", "-c", "maim -s | xclip -selection clipboard -t image/png" };

// volume
static const char *vol_up[] = { "amixer", "-q", "sset", "Master", "5%+", NULL };
static const char *vol_down[] = { "amixer", "-q", "sset", "Master", "5%-", NULL };
static const char *media_mute[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *media_toggle[] = { "playerctl", "play-pause", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#include "shiftview.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	
	// dmenu
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },

	// Terminal
	{ MODKEY|ShiftMask|ControlMask, XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,		XK_Return, spawn,	   {.v = termfloat } },
	// borked with rendering on dwm, insane lag, may be nvidia related.
	//{ MODKEY|ShiftMask,		XK_backslash, spawn,	   {.v = flttermnvim } },
	{ MODKEY|ShiftMask|ControlMask,	XK_backslash, spawn,	   {.v = termnvim } },
	{ MODKEY,                       XK_semicolon, togglebar,   {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ 0, 				XF86XK_AudioRaiseVolume,   spawn,	{.v = vol_up } },
	{ 0,				XF86XK_AudioLowerVolume,   spawn,	{.v = vol_down } },
	{ 0,				XF86XK_AudioMute,	   spawn,	{.v = media_mute } },
	{ 0,				XF86XK_AudioPlay,	   spawn,	{.v = media_toggle } },
	{ MODKEY,			XK_b,	   shiftview,	   {.i = -1 } },
	{ MODKEY,			XK_n,	   shiftview,	   {.i = +1 } },
	{ MODKEY,			XK_v,	   spawn,	   {.v = clipcmd} },
	{ 0,				XK_Print,  spawn,	   {.v = scrclip} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

