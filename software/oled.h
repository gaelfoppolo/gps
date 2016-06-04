
/**
 *	@file oled.h
 *	@author Gaël Foppolo (gaelfoppolo)
 *
 *	@brief File containing the OLED functions
 */

#ifndef OLED_H
#define OLED_H

/** @brief Current */
#define CURRENT 1
/** @brief OLD */
#define OLD 0

/** @brief Mode selected on display is compass */
#define MD_COMPASS 0
/** @brief Mode selected on display is naviguation */
#define MD_NAVIG 1
/** @brief Mode selected on display is record */
#define MD_RECORD 2
/** @brief Mode selected on display is shutdown */
#define MD_SHUTDOWN 3

/** @brief Direction is none */
#define NONE ""
/** @brief Direction is north */
#define NORTH "N"
/** @brief Direction is north-east */
#define NE "NE"
/** @brief Direction is east */
#define EAST "E"
/** @brief Direction is south-east */
#define SE "SE"
/** @brief Direction is south */
#define SOUTH "S"
/** @brief Direction is south-west */
#define SW "SO"
/** @brief Direction is west */
#define WEST "O"
/** @brief Direction is north-west */
#define NW "NO"

/** @brief OLED respond is ACK */
#define OLED_ANSWER_ACK 6

/** @brief Color */
#define ALICEBLUE               0xF7DF 
/** @brief Color */
#define ANTIQUEWHITE            0xFF5A 
/** @brief Color */
#define AQUA                    0x07FF 
/** @brief Color */
#define AQUAMARINE              0x7FFA 
/** @brief Color */
#define AZURE                   0xF7FF 
/** @brief Color */
#define BEIGE                   0xF7BB 
/** @brief Color */
#define BISQUE                  0xFF38 
/** @brief Color */
#define BLACK                   0x0000 
/** @brief Color */
#define BLANCHEDALMOND          0xFF59 
/** @brief Color */
#define BLUE                    0x001F 
/** @brief Color */
#define BLUEVIOLET              0x895C 
/** @brief Color */
#define BROWN                   0xA145 
/** @brief Color */
#define BURLYWOOD               0xDDD0 
/** @brief Color */
#define CADETBLUE               0x5CF4 
/** @brief Color */
#define CHARTREUSE              0x7FE0 
/** @brief Color */
#define CHOCOLATE               0xD343 
/** @brief Color */
#define CORAL                   0xFBEA 
/** @brief Color */
#define CORNFLOWERBLUE          0x64BD 
/** @brief Color */
#define CORNSILK                0xFFDB 
/** @brief Color */
#define CRIMSON                 0xD8A7 
/** @brief Color */
#define CYAN                    0x07FF 
/** @brief Color */
#define DARKBLUE                0x0011 
/** @brief Color */
#define DARKCYAN                0x0451 
/** @brief Color */
#define DARKGOLDENROD           0xBC21 
/** @brief Color */
#define DARKGRAY                0xAD55 
/** @brief Color */
#define DARKGREEN               0x0320 
/** @brief Color */
#define DARKKHAKI               0xBDAD 
/** @brief Color */
#define DARKMAGENTA             0x8811 
/** @brief Color */
#define DARKOLIVEGREEN          0x5345 
/** @brief Color */
#define DARKORANGE              0xFC60 
/** @brief Color */
#define DARKORCHID              0x9999 
/** @brief Color */
#define DARKRED                 0x8800 
/** @brief Color */
#define DARKSALMON              0xECAF 
/** @brief Color */
#define DARKSEAGREEN            0x8DF1 
/** @brief Color */
#define DARKSLATEBLUE           0x49F1 
/** @brief Color */
#define DARKSLATEGRAY           0x2A69 
/** @brief Color */
#define DARKTURQUOISE           0x067A 
/** @brief Color */
#define DARKVIOLET              0x901A 
/** @brief Color */
#define DEEPPINK                0xF8B2 
/** @brief Color */
#define DEEPSKYBLUE             0x05FF 
/** @brief Color */
#define DIMGRAY                 0x6B4D 
/** @brief Color */
#define DODGERBLUE              0x1C9F 
/** @brief Color */
#define FIREBRICK               0xB104 
/** @brief Color */
#define FLORALWHITE             0xFFDE 
/** @brief Color */
#define FORESTGREEN             0x2444 
/** @brief Color */
#define FUCHSIA                 0xF81F 
/** @brief Color */
#define GAINSBORO               0xDEFB 
/** @brief Color */
#define GHOSTWHITE              0xFFDF 
/** @brief Color */
#define GOLD                    0xFEA0 
/** @brief Color */
#define GOLDENROD               0xDD24 
/** @brief Color */
#define GRAY                    0x8410 
/** @brief Color */
#define GREEN                   0x0400 
/** @brief Color */
#define GREENYELLOW             0xAFE5 
/** @brief Color */
#define HONEYDEW                0xF7FE 
/** @brief Color */
#define HOTPINK                 0xFB56 
/** @brief Color */
#define INDIANRED               0xCAEB 
/** @brief Color */
#define INDIGO                  0x4810 
/** @brief Color */
#define IVORY                   0xFFFE 
/** @brief Color */
#define KHAKI                   0xF731 
/** @brief Color */
#define LAVENDER                0xE73F 
/** @brief Color */
#define LAVENDERBLUSH           0xFF9E 
/** @brief Color */
#define LAWNGREEN               0x7FE0 
/** @brief Color */
#define LEMONCHIFFON            0xFFD9 
/** @brief Color */
#define LIGHTBLUE               0xAEDC 
/** @brief Color */
#define LIGHTCORAL              0xF410 
/** @brief Color */
#define LIGHTCYAN               0xE7FF 
/** @brief Color */
#define LIGHTGOLD               0xFFDA 
/** @brief Color */
#define LIGHTGREEN              0x9772 
/** @brief Color */
#define LIGHTGREY               0xD69A 
/** @brief Color */
#define LIGHTPINK               0xFDB8 
/** @brief Color */
#define LIGHTSALMON             0xFD0F 
/** @brief Color */
#define LIGHTSEAGREEN           0x2595 
/** @brief Color */
#define LIGHTSKYBLUE            0x867F 
/** @brief Color */
#define LIGHTSLATEGRAY          0x7453 
/** @brief Color */
#define LIGHTSTEELBLUE          0xB63B 
/** @brief Color */
#define LIGHTYELLOW             0xFFFC 
/** @brief Color */
#define LIME                    0x07E0 
/** @brief Color */
#define LIMEGREEN               0x3666 
/** @brief Color */
#define LINEN                   0xFF9C 
/** @brief Color */
#define MAGENTA                 0xF81F 
/** @brief Color */
#define MAROON                  0x8000 
/** @brief Color */
#define MEDIUMAQUAMARINE        0x6675 
/** @brief Color */
#define MEDIUMBLUE              0x0019 
/** @brief Color */
#define MEDIUMORCHID            0xBABA 
/** @brief Color */
#define MEDIUMPURPLE            0x939B 
/** @brief Color */
#define MEDIUMSEAGREEN          0x3D8E 
/** @brief Color */
#define MEDIUMSLATEBLUE         0x7B5D 
/** @brief Color */
#define MEDIUMSPRINGGREEN       0x07D3 
/** @brief Color */
#define MEDIUMTURQUOISE         0x4E99 
/** @brief Color */
#define MEDIUMVIOLETRED         0xC0B0 
/** @brief Color */
#define MIDNIGHTBLUE            0x18CE 
/** @brief Color */
#define MINTCREAM               0xF7FF 
/** @brief Color */
#define MISTYROSE               0xFF3C 
/** @brief Color */
#define MOCCASIN                0xFF36 
/** @brief Color */
#define NAVAJOWHITE             0xFEF5 
/** @brief Color */
#define NAVY                    0x0010 
/** @brief Color */
#define OLDLACE                 0xFFBC 
/** @brief Color */
#define OLIVE                   0x8400 
/** @brief Color */
#define OLIVEDRAB               0x6C64 
/** @brief Color */
#define ORANGE                  0xFD20 
/** @brief Color */
#define ORANGERED               0xFA20 
/** @brief Color */
#define ORCHID                  0xDB9A 
/** @brief Color */
#define PALEGOLDENROD           0xEF55 
/** @brief Color */
#define PALEGREEN               0x9FD3 
/** @brief Color */
#define PALETURQUOISE           0xAF7D 
/** @brief Color */
#define PALEVIOLETRED           0xDB92 
/** @brief Color */
#define PAPAYAWHIP              0xFF7A 
/** @brief Color */
#define PEACHPUFF               0xFED7 
/** @brief Color */
#define PERU                    0xCC27 
/** @brief Color */
#define PINK                    0xFE19 
/** @brief Color */
#define PLUM                    0xDD1B 
/** @brief Color */
#define POWDERBLUE              0xB71C 
/** @brief Color */
#define PURPLE                  0x8010 
/** @brief Color */
#define RED                     0xF800 
/** @brief Color */
#define ROSYBROWN               0xBC71 
/** @brief Color */
#define ROYALBLUE               0x435C 
/** @brief Color */
#define SADDLEBROWN             0x8A22 
/** @brief Color */
#define SALMON                  0xFC0E 
/** @brief Color */
#define SANDYBROWN              0xF52C 
/** @brief Color */
#define SEAGREEN                0x2C4A 
/** @brief Color */
#define SEASHELL                0xFFBD 
/** @brief Color */
#define SIENNA                  0xA285 
/** @brief Color */
#define SILVER                  0xC618 
/** @brief Color */
#define SKYBLUE                 0x867D 
/** @brief Color */
#define SLATEBLUE               0x6AD9 
/** @brief Color */
#define SLATEGRAY               0x7412 
/** @brief Color */
#define SNOW                    0xFFDF 
/** @brief Color */
#define SPRINGGREEN             0x07EF 
/** @brief Color */
#define STEELBLUE               0x4416 
/** @brief Color */
#define TAN                     0xD5B1 
/** @brief Color */
#define TEAL                    0x0410 
/** @brief Color */
#define THISTLE                 0xDDFB 
/** @brief Color */
#define TOMATO                  0xFB08 
/** @brief Color */
#define TURQUOISE               0x471A 
/** @brief Color */
#define VIOLET                  0xEC1D 
/** @brief Color */
#define WHEAT                   0xF6F6 
/** @brief Color */
#define WHITE                   0xFFFF 
/** @brief Color */
#define WHITESMOKE              0xF7BE 
/** @brief Color */
#define YELLOW                  0xFFE0 
/** @brief Color */
#define YELLOWGREEN             0x9E66 

/**
 * What mode is selected right now
 */
extern unsigned int modeDisplay;
/**
 * What mode was selected just before
 */
extern unsigned int oldModeDisplay;
/**
 * Is the display has been updated, aka needed
 */
extern int displayHasBeenUpdated;
 
/**
 * @brief      Enable and config USART for OLED
 */
void enableUSARTforOLED();

/**
 * @brief Reset OLED
 */
void resetOLED();

/**
 * @brief Toggle OLED interrupt
 * @details 1 = interrupt enable for OLED, 0 = disable
 * 
 * @param state The new state
 */
void toggleOLEDInterrupt(unsigned int state);

/**
 * @brief Send char
 * 
 * @param c The int to send
 */
void sendChar(int c);

/**
 * @brief      
 * Receive function for OLED data (USART1, interrupt mode)
 */
void usart1_rx();

/**
 * @brief      Clean the screen
 */
void gfx_Clean();

/**
 * @brief      Set the background color
 *
 * @param[in]  color  The color
 */
void gfx_BGcolour(int color);

/**
 * @brief      Put a string on the screen
 *
 * @param      string  The string
 */
void gfx_PutString(char* string);

/**
 * @brief      Draw a rectangle filled with a color
 *
 * @param[in]  x1     The x pos of the top left corner
 * @param[in]  y1     The y pos of the top left corner
 * @param[in]  x2     The x pos of the bottom right corner
 * @param[in]  y2     The y pos of the bottom right corner
 * @param[in]  color  The color
 */
void gfx_RectangleFilled(int x1, int y1, int x2, int y2, int color);

/**
 * @brief      Screensave mode
 *
 * @param[in]  t     The mode
 */
void SSTimeout(int t);

/**
 * @brief      Set the baud rate.
 */
void setBaudRate();

/**
 * @brief      Calculate the (x,y) pos (orbit) from angle and distance
 *
 * @param[in]  angle     The angle
 * @param[in]  distance  The distance
 * @param      x         The x pos computed
 * @param      y         The y pos computed
 */
void gfx_CalculateOrbit(int angle, int distance, int* x, int* y);

/**
 * @brief      Draw a circle
 *
 * @param[in]  x       x pos of center of the circle
 * @param[in]  y       y pos of center of the circle
 * @param[in]  radius  The radius
 * @param[in]  color   The color
 */
void gfx_DrawCircle(int x, int y, int radius, int color);

/**
 * @brief      Draw a line
 *
 * @param[in]  x1     The x pos of the beginning of the line
 * @param[in]  y1     The y pos of the beginning of the line
 * @param[in]  x2     The x pos of the ending of the line
 * @param[in]  y2     The y pos of the ending of the line
 * @param[in]  color  The color
 */
void gfx_DrawLine(int x1, int y1, int x2, int y2, int color);

/**
 * @brief      Screen mode (portrait/landscape)
 *
 * @param[in]  mode  The mode
 */
void gfx_ScreenMode(int mode);

/**
 * @brief      Move to origin to a position
 *
 * @param[in]  x     The new x pos
 * @param[in]  y     The new y pos
 */
void gfx_MoveOrigin(int x, int y);

/**
 * @brief      Draw a rectangle
 *
 * @param[in]  x1     The x pos of the top left corner
 * @param[in]  y1     The y pos of the top left corner
 * @param[in]  x2     The x pos of the bottom right corner
 * @param[in]  y2     The y pos of the bottom right corner
 * @param[in]  color  The color
 */
void gfx_Rectangle(int x1, int y1, int x2, int y2, int color);

/**
 * @brief      Set the width of the text
 *
 * @param[in]  multi  The multi
 */
void txt_Width(int multi);

/**
 * @brief      Set the text color
 *
 * @param[in]  color  The color
 */
void txt_FGColor(int color);

/**
 * @brief      Set the text background color
 *
 * @param[in]  color  The color
 */
void txt_BGColor(int color);

/**
 * @brief      Configure OLED for proper using
 */
void initOLED();

/**
 * @brief      Display menu
 */
void displayMainMenu();

/**
 * @brief      Update the menu with currently selected
 */
void updateMenu();

/**
 * @brief      Display a string in the center of the screen
 *
 * @param      string  The string to display
 */
void displayMessage(char* string);

/**
 * @brief      Display the compass
 */
void displayCompass();

/**
 * @brief      Display message header
 */
void displayHeader();

/**
 * @brief      Calculate the direction (N, S, NE, etc.)
 *
 * @return     The direction.
 */
char* calculateDirection();

/**
 * @brief      Float to string conversion
 *
 * @param      p     The buffer (string)
 * @param[in]  x     The float
 */
void ftoa(char* p, float x);

#endif

