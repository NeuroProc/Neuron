//sudo make EVILVTE_CONF=$HOME/.evilvte/config.h reinstall clean
#define COLOR_STYLE 		LINUX

// #define BACKGROUND_TRANSPARENT  TRUE  /* Pseudo transparent background      */
#define BACKGROUND_OPACITY      TRUE  /* True transparent background        */
#define BACKGROUND_SATURATION   0.5

#define SCROLL_LINES            10000  /* Negative value means unlimited     */
#define SHOW_WINDOW_BORDER      FALSE
#define SHOW_WINDOW_DECORATED   FALSE
#define SHOW_WINDOW_ICON        FALSE

#define BELL_AUDIBLE            FALSE

#define COMMAND_EXEC_PROGRAM	TRUE  /* -e option, execute program         */
#define	COMMAND_SHOW_OPTIONS	TRUE  /* -o option, show build-time options */
#define	COMMAND_SHOW_VERSION	TRUE  /* -v option, show program version    */
#define COMMAND_TAB_NUMBERS	TRUE  /* -[2-9] options, to set tab numbers */
#define CURSOR_COLOR		"blue"
// #define COLOR_TEXT_DIM         	"blue"
#define COLOR_TEXT_HIGHLIGHTED  "blue"
// #define COLOR_TEXT_BOLD         "red"
#define COLOR_FOREGROUND       	"#FFFFFF" // "#00ff00"
#define FONT			"Courier 14"
#define	SCROLLBAR		OFF_R /* Options: LEFT, RIGHT, OFF_L, OFF_R */
#define STATUS_BAR           	FALSE
#define WORD_CHARS             	"-A-Za-z0-9_$.+!*(),;:@&=?/~#%"

#define MENU                   	TRUE
#define MATCH_STRING_L    	"firefox" /* left click to open without menu*/
#define MENU_CUSTOM             "Copy", "Paste"
#define TAB_NEW_PATH_EQUAL_OLD  TRUE
#define TAB_REORDERABLE         TRUE
#define TAB_BORDER              0
// #define TAB_LABEL               "tab %u" /* %u will be replaced by numbers */
// #define TABBAR_AUTOHIDE         TRUE
#define TAB_SHOW_INFO_AT_TITLE  TRUE
#define TABBAR                  FALSE
#define TAB                  	TRUE

#define HOTKEY               	TRUE
#define HOTKEY_COPY             CTRL(GDK_X) || CTRL(GDK_x)
#define HOTKEY_PASTE            CTRL(GDK_V) || CTRL(GDK_v)
#define HOTKEY_SELECT_ALL       CTRL(GDK_A) || CTRL(GDK_a)
#define HOTKEY_TAB_ADD          CTRL(GDK_T) || CTRL(GDK_t)
#define HOTKEY_TAB_REMOVE       CTRL(GDK_W) || CTRL(GDK_w)
#define HOTKEY_TAB_PREVIOUS     CTRL(GDK_Left)
#define HOTKEY_TAB_NEXT         CTRL(GDK_Right)


