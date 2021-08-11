#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

// Port Registers
#define REG_CTRL 0x3d4
#define REG_DATA 0x3d5

// API Functions
void printpos(char *text, int x, int y);
void print(char *text);
void clearscr();
