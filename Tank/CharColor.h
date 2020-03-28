#ifndef CHAR_COLOR_H
#define CHAR_COLOR_H

/*------------------------------------------------------------------------------*/
#define COLOR_NONE      0

#define COLOR_FG_RED    41
#define COLOR_FG_GREEN  42
#define COLOR_FG_YELLOW 43
#define COLOR_FG_BLUE   44
#define COLOR_FG_PURPLE 45
#define COLOR_FG_CYAN   46
#define COLOR_FG_WHITE  47

#define COLOR_BG_RED    31
#define COLOR_BG_GREEN  32
#define COLOR_BG_YELLOW 33
#define COLOR_BG_BLUE   34
#define COLOR_BG_PURPLE 35
#define COLOR_BG_CYAN   36
#define COLOR_BG_WHITE  37

typedef struct charColor{
  char c;
  int color;
} charColor_t;

#endif /* CHAR_COLOR_H */
