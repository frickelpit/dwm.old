/* See LICENSE file for copyright and license details. */

/* includes */
#include "bstack.c"
#include "fibonacci.c"
#include "movestack.c"
#include "gaplessgrid.c"

/* appearance */
static const char font[]            = "-*-stlarch-medium-r-*-*-11-*-*-*-*-*-*-*" "," "-*-termsyn-medium-*-normal-*-14-*-*-*-*-*-*-*";
#define NUMCOLORS	8	
static const char colors[NUMCOLORS][ColLast][8] = {
   /* border	foreground  background */
   { "#1F1F1F", "#B7B7B7",  "#0E0F12" },  // 1 = normal
   { "#B7B7B7", "#2865E0",  "#0E0F12" },  // 2 = selected
   { "#202020", "#202020",  "#F0E84D" },  // 3 = urgent/warning
   { "#ff0000", "#ADADAD",  "#ff0000" },  // 4 = error
   { "#202020", "#3DE32D",  "#0E0F12" },  // 5 = green
   { "#202020", "#E7EB17",  "#0E0F12" },  // 6 = yellow
   { "#202020", "#CC1818",  "#0E0F12" },  // 7 = red
   { "#202020", "#2865E0",  "#0E0F12" },  // 8 = blue
   // add more here
   };
static const unsigned int borderpx  		= 1;        	/* border pixel of windows */
static const unsigned int snap      		= 10;       	/* snap pixel */
static const Bool showbar           		= True;     	/* False means no bar */
static const Bool topbar            		= True;     	/* False means bottom bar */
static const Bool showsystray	    		= True;		/* True means show systray */
static const unsigned int systrayspacing 	= 1;		/* space between systray icons */
static const Bool clicktofocus      		= True;     	/* Change focus only on click */

/* tagging */
static const char *tags[] = { "web", "main", "mail<>news", "media", "other" };

static const Rule rules[] = {
	/* class	instance	title		tags mask	isfloating	monitor */
	{ "Firefox",	NULL,		NULL,		1,		False,		-1 },
	{ "Firefox",	NULL,		"Firefox-Einstellungen",	1,	True,		-1 },
	{ "Pcmanfm",	NULL,		NULL,		1 << 4,		True,		-1 },
	{ NULL,		NULL,		"mutt",		1 << 2,		False,		-1 },
	{ NULL,		NULL,		"newsbeuter",	1 << 2,		False,		-1 },
	{ NULL,		NULL,		"irssi",	1 << 1,		False,		-1 },
	{ NULL,		NULL,		"ncmpcpp",	1 << 1,		False,		-1 },
	{ NULL,		NULL,		"jabber",	1 << 1,		False,		-1 },
	{ NULL,		NULL,		"ranger",	1 << 1,		False,		-1 },
	{ NULL,		NULL,		"mpv",		1 << 3,		True,		-1 },
};

/* layout(s) */
static const float mfact      			= 0.56; 	/* factor of master area size [0.05..0.95] */
static const int nmaster      			= 1;    	/* number of clients in master area */
static const Bool resizehints 			= False; 	/* True means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol	arrange 	function */
	{ "",		tile },    	/* first entry is default */
	{ "",		NULL },    	/* no layout function means floating behavior */
	{ "",		monocle },	
	{ "",		spiral },
	{ "",		dwindle },
	{ "",		bstack },
	{ "",		gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] 		= { "dmenu_run", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG], "-sb", colors[1][ColBG], "-sf", colors[1][ColFG], NULL };
static const char *termcmd[]  		= { "urxvtc", NULL };

/* my commands*/
static const char *irccmd[]		= { "urxvtc", "-title", "irssi", "-e", "irssi", NULL };
static const char *volup[]		= { "amixer", "-q", "sset", "Master", "4%+", "unmute", NULL };
static const char *voldown[]		= { "amixer", "-q", "sset", "Master", "4%-", "unmute", NULL };
static const char *volmute[]		= { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *screenshotcmd[]	= { "scrot", "-e", "mv $f ~/Bilder/Screenshots 2>/dev/null", NULL };
static const char *rangercmd[]		= { "urxvtc", "-title", "ranger", "-e", "ranger", NULL };
static const char *quitcmd[]		= { "killall", "startdwm", NULL };
static const char *imcmd[]		= { "urxvtc", "-title", "jabber", "-e", "profanity", NULL };
static const char *newscmd[]		= { "urxvtc", "-title", "newsbeuter", "-e", "newsbeuter", NULL };
static const char *musiccmd[]		= { "urxvtc", "-title", "ncmpcpp", "-e", "ncmpcpp", NULL };
static const char *browsercmd[]		= { "firefox", NULL };
static const char *mailcmd[]		= { "urxvtc", "-title", "mutt", "-e", "mutt", NULL };
static const char *filecmd[]		= { "pcmanfm", NULL };
static const char *rebootcmd[]		= { "systemctl", "reboot", NULL };
static const char *shutdowncmd[]	= { "systemctl", "poweroff", NULL };

/* keybindings */
static Key keys[] = {
	/* modifier                     key        	function        argument */
	{ MODKEY,                       XK_p,      	spawn,          {.v = dmenucmd } },
	{ MODKEY,	                XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      	incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      	movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      	movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, 	zoom,           {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      	killclient,     {0} },
	{ MODKEY,                       XK_t,      	setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      	setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      	setlayout,      {.v = &layouts[2]} },
	{ MODKEY,			XK_s,		setlayout,	{.v = &layouts[3]} },
	{ MODKEY,			XK_a,		setlayout,	{.v = &layouts[4]} },
	{ MODKEY,			XK_b,		setlayout,	{.v = &layouts[5]} },
	{ MODKEY,			XK_g,		setlayout,	{.v = &layouts[6]} },
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_Left,    	cycle,          {.i = -1 } },
        { MODKEY,                       XK_Right,   	cycle,          {.i = +1 } },
        { MODKEY|ControlMask,           XK_Left,    	tagcycle,       {.i = -1 } },
        { MODKEY|ControlMask,           XK_Right,   	tagcycle,       {.i = +1 } },

/* my keybindings*/
	{ MODKEY|ControlMask,		XK_i,      	spawn,          {.v = irccmd } },
	{ 0,				0x1008ff13,	spawn,		{.v = volup } },
	{ 0,				0x1008ff11,	spawn,		{.v = voldown } },
	{ 0,				0x1008ff12,	spawn,		{.v = volmute } },
	{ 0,				0xff61,		spawn,		{.v = screenshotcmd } },
	{ MODKEY|ControlMask,		XK_r,		spawn,		{.v = rangercmd } },
	{ MODKEY|ControlMask,		XK_q,		spawn,		{.v = quitcmd } },
	{ MODKEY|ControlMask,		XK_p,		spawn,		{.v = imcmd } },
	{ MODKEY|ControlMask,		XK_n,		spawn,		{.v = newscmd } },
	{ MODKEY|ControlMask,		XK_m,		spawn,		{.v = musiccmd } },
	{ MODKEY|ControlMask,		XK_f,		spawn,		{.v = browsercmd } },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		{.v = mailcmd } },
	{ MODKEY|ShiftMask,		XK_p,		spawn,		{.v = filecmd } },
	{ MODKEY|ShiftMask,		XK_r,		spawn,		{.v = rebootcmd } },
	{ MODKEY|ShiftMask,		XK_s,		spawn,		{.v = shutdowncmd } },

	TAGKEYS(                        XK_1,                      	0)
	TAGKEYS(                        XK_2,                      	1)
	TAGKEYS(                        XK_3,                      	2)
	TAGKEYS(                        XK_4,                      	3)
	TAGKEYS(                        XK_5,                      	4)
	TAGKEYS(                        XK_6,                      	5)
	TAGKEYS(                        XK_7,                      	6)
	TAGKEYS(                        XK_8,                      	7)
	TAGKEYS(                        XK_9,                      	8)
	{ MODKEY|ShiftMask,             XK_q,      	quit,           {0} },

};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

