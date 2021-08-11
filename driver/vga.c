#include "vga.h"
#include "ports.h"

// Private API functions
int get_cur_offset();
void set_cur_offset(int offset);
int printch(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

void print(char *text){
    printpos(text, -1, -1);
}

void printpos(char *text, int x, int y){
    int offset = 0;

    if (x >= 0 && y >= 0){
        offset = get_offset(x, y);
    }  
    else{
        offset = get_cur_offset();
        x = get_offset_col(offset);
        y = get_offset_row(offset);
    }

    int i = 0;
    while (text[i] != 0){
        offset = printch(text[i], x, x, WHITE_ON_BLACK);
        x = get_offset_col(offset);
        y = get_offset_row(offset);

        i++;
    }
}

int printch(char c, int col, int row, char attr){
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    if (col >= MAX_COLS || row >= MAX_ROWS){
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = '?';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;

    if (col >= 0 && row >= 0){
        offset = get_offset(col, row);
    }
    else{
        offset = get_cur_offset();
    }

    if (c == '\n'){
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } 
    else{
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    set_cur_offset(offset);
    
    return offset;
}

int get_cursor_offset(){
    port_byte_out(REG_CTRL, 14);
    
    int offset = port_byte_in(REG_DATA) << 8;

    port_byte_out(REG_CTRL, 15);
    
    offset += port_byte_in(REG_DATA);
    
    // return offset / 2;
    return offset * 2;
}

void set_cur_offset(int offset){
    offset = offset / 2;
    port_byte_out(REG_CTRL, 14);

    unsigned char b1 = (unsigned char) (offset >> 8);
    port_byte_out(REG_DATA, b1);

    port_byte_out(REG_CTRL, 15);

    unsigned char b2 = (unsigned char) (offset & 0xff);
    port_byte_out(REG_DATA, b2);
}

void clear_screen(){
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++){
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }

    set_cur_offset(get_offset(0, 0));
}


int get_offset(int col, int row){ 
    return 2 * (row * MAX_COLS + col); 
}

int get_offset_row(int offset){
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset){
    int xf = get_offset_row(offset) * 2 * MAX_COLS;
    
    return (offset - xf) / 2;
}
