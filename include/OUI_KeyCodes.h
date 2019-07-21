#ifndef OUI_KEYCODES_H
#define OUI_KEYCODES_H

//These were taken right out of Java's java.awt.event.KeyEvent

static const int KEY_ENTER = 13;
static const int KEY_BACKSPACE = 8;
static const int KEY_TAB = 9;
static const int KEY_SHIFT_LEFT = 1073742049;
static const int KEY_SHIFT_RIGHT = 1073742053;
static const int KEY_CONTROL_LEFT = 1073742048;
static const int KEY_CONTROL_RIGHT = 1073742052;
static const int KEY_ALT_LEFT = 1073742050;
static const int KEY_ALT_RIGHT = 1073742054;
static const int KEY_META = 157;

static const int KEY_PAUSE = 1073741896;
static const int KEY_CAPS_LOCK = 9;
static const int KEY_ESCAPE = 27;
static const int KEY_SPACE = 32;
static const int KEY_PAGE_UP = 1073741899;
static const int KEY_PAGE_DOWN = 1073741902;
static const int KEY_END = 1073741902;
static const int KEY_HOME = 1073741898;

static const int KEY_LEFT = 1073741904;
static const int KEY_UP = 1073741906;
static const int KEY_RIGHT = 1073741903;
static const int KEY_DOWN = 1073741905;
static const int KEY_COMMA = 44;
static const int KEY_DASH = 45;
static const int KEY_PERIOD = 46;
static const int KEY_SLASH = 47;
static const int KEY_0 = 48;
static const int KEY_1 = 49;
static const int KEY_2 = 50;
static const int KEY_3 = 51;
static const int KEY_4 = 52;
static const int KEY_5 = 53;
static const int KEY_6 = 54;
static const int KEY_7 = 55;
static const int KEY_8 = 56;
static const int KEY_9 = 57;

static const int KEY_SEMICOLON = 59;
static const int KEY_EQUALS = 61;

/** KEY_A thru KEY_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
static const int KEY_A = 97;
static const int KEY_B = 98;
static const int KEY_C = 99;
static const int KEY_D = 100;
static const int KEY_E = 101;
static const int KEY_F = 102;
static const int KEY_G = 103;
static const int KEY_H = 104;
static const int KEY_I = 105;
static const int KEY_J = 106;
static const int KEY_K = 107;
static const int KEY_L = 108;
static const int KEY_M = 109;
static const int KEY_N = 110;
static const int KEY_O = 111;
static const int KEY_P = 112;
static const int KEY_Q = 113;
static const int KEY_R = 114;
static const int KEY_S = 115;
static const int KEY_T = 116;
static const int KEY_U = 117;
static const int KEY_V = 118;
static const int KEY_W = 119;
static const int KEY_X = 120;
static const int KEY_Y = 121;
static const int KEY_Z = 122;
static const int KEY_OPEN_BRACKET = 91;//[
static const int KEY_BACK_SLASH = 92;
static const int KEY_CLOSE_BRACKET = 93;//]
static const int KEY_NUMPAD0 = 1073741922;
static const int KEY_NUMPAD1 = 1073741913;
static const int KEY_NUMPAD2 = 1073741914;
static const int KEY_NUMPAD3 = 1073741915;
static const int KEY_NUMPAD4 = 1073741916;
static const int KEY_NUMPAD5 = 1073741917;
static const int KEY_NUMPAD6 = 1073741918;
static const int KEY_NUMPAD7 = 1073741919;
static const int KEY_NUMPAD8 = 1073741920;
static const int KEY_NUMPAD9 = 1073741921;
static const int KEY_MULTIPLY = 1073741909;
static const int KEY_ADD = 1073741911;

static const int KEY_SUBTRACT = 1073741910;
static const int KEY_DECIMAL = 1073741923;
static const int KEY_DIVIDE = 1073741908;
static const int KEY_DELETE = 127; /* ASCII DEL */
static const int KEY_NUM_LOCK = 1073741907;
static const int KEY_SCROLL_LOCK = 1073741895;

static const int KEY_F1 = 1073741882;
static const int KEY_F2 = 1073741883;
static const int KEY_F3 = 1073741884;
static const int KEY_F4 = 1073741885;
static const int KEY_F5 = 1073741886;
static const int KEY_F6 = 1073741887;
static const int KEY_F7 = 1073741888;
static const int KEY_F8 = 1073741889;
static const int KEY_F9 = 1073741890;
static const int KEY_F10 = 1073741891;
static const int KEY_F11 = 1073741892;
static const int KEY_F12 = 1073741893;

static const int KEY_PRINTSCREEN = 1073741894;
static const int KEY_INSERT = 1073741897;

static const int KEY_BACK_QUOTE = 0xC0;
static const int KEY_QUOTE = 0xDE;

static const int KEY_KP_UP = 1073741920;
static const int KEY_KP_DOWN = 1073741914;
static const int KEY_KP_LEFT = 1073741916;
static const int KEY_KP_RIGHT = 1073741918;

static char getChar(int keyCode, bool shift) {

    if (keyCode == KEY_ENTER) return '\n';
    if (keyCode == KEY_TAB) return '\t';
    if (keyCode == KEY_SPACE) return ' ';

    if (keyCode == KEY_COMMA) return shift ? '<' : ',';
    if (keyCode == KEY_DASH) return shift ? '_' : '-';
    if (keyCode == KEY_PERIOD) return shift ? '>' : '.';
    if (keyCode == KEY_SLASH) return shift ? '?' : '/';

    if (keyCode == KEY_0) return shift ? ')' : '0';
    if (keyCode == KEY_1) return shift ? '!' : '1';
    if (keyCode == KEY_2) return shift ? '@' : '2';
    if (keyCode == KEY_3) return shift ? '#' : '3';
    if (keyCode == KEY_4) return shift ? '$' : '4';
    if (keyCode == KEY_5) return shift ? '%' : '5';
    if (keyCode == KEY_6) return shift ? '^' : '6';
    if (keyCode == KEY_7) return shift ? '&' : '7';
    if (keyCode == KEY_8) return shift ? '*' : '8';
    if (keyCode == KEY_9) return shift ? '(' : '9';
    if (keyCode == KEY_SEMICOLON) return shift ? ':' : ';';
    if (keyCode == KEY_EQUALS) return shift ? '+' : '=';
    if (keyCode == KEY_A) return shift ? 'A' : 'a';
    if (keyCode == KEY_B) return shift ? 'B' : 'b';
    if (keyCode == KEY_C) return shift ? 'C' : 'c';
    if (keyCode == KEY_D) return shift ? 'D' : 'd';
    if (keyCode == KEY_E) return shift ? 'E' : 'e';
    if (keyCode == KEY_F) return shift ? 'F' : 'f';
    if (keyCode == KEY_G) return shift ? 'G' : 'g';
    if (keyCode == KEY_H) return shift ? 'H' : 'h';
    if (keyCode == KEY_I) return shift ? 'I' : 'i';
    if (keyCode == KEY_J) return shift ? 'J' : 'j';
    if (keyCode == KEY_K) return shift ? 'K' : 'k';
    if (keyCode == KEY_L) return shift ? 'L' : 'l';
    if (keyCode == KEY_M) return shift ? 'M' : 'm';
    if (keyCode == KEY_N) return shift ? 'N' : 'n';
    if (keyCode == KEY_O) return shift ? 'O' : 'o';
    if (keyCode == KEY_P) return shift ? 'P' : 'p';
    if (keyCode == KEY_Q) return shift ? 'Q' : 'q';
    if (keyCode == KEY_R) return shift ? 'R' : 'r';
    if (keyCode == KEY_S) return shift ? 'S' : 's';
    if (keyCode == KEY_T) return shift ? 'T' : 't';
    if (keyCode == KEY_U) return shift ? 'U' : 'u';
    if (keyCode == KEY_V) return shift ? 'V' : 'v';
    if (keyCode == KEY_W) return shift ? 'W' : 'w';
    if (keyCode == KEY_X) return shift ? 'X' : 'x';
    if (keyCode == KEY_Y) return shift ? 'Y' : 'y';
    if (keyCode == KEY_Z) return shift ? 'Z' : 'z';
    if (keyCode == KEY_OPEN_BRACKET) return shift ? '{' : '[';
    if (keyCode == KEY_BACK_SLASH) return shift ? '|' : '\\';
    if (keyCode == KEY_CLOSE_BRACKET) return shift ? '}' : ']';
    if (keyCode == KEY_BACK_QUOTE) return shift ? '~' : '`';
    if (keyCode == KEY_QUOTE) return shift ? '"' : '\'';
    return ' ';
}

#endif
