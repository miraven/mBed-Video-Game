#include "graphics.h"

#include "globals.h"



void draw_player(int u, int v, int key)
{
    uLCD.filled_rectangle(u, v, u+11, v+11, RED);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = WHITE;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}

void draw_plant(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_debri (int u, int v) 
{ 
    uLCD.filled_rectangle(u, v, u+10, v+10, LGREY);
}

void draw_survivor (int u, int v) 
{ 
    uLCD.filled_rectangle(u, v, u+10, v+10, BLUE);
}

void draw_door (int u, int v) 
{ 
    uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
}

    
void draw_medkit (int u, int v) 
{ 
    
    char str [121]; 
    for (int i = 0; i < 121; i++) {
        switch (i) {
            case 5: 
            case 16: 
            case 27: 
            case 38: 
            case 49: 
            case 60: 
            case 71: 
            case 82: 
            case 93: 
            case 104: 
            case 115:
            case 55: 
            case 56: 
            case 57: 
            case 58: 
            case 59: 
            case 61: 
            case 62: 
            case 63: 
            case 64: 
            case 65:
                str[i] = 'R';
                break;
            default: 
                str[i] = 'W'; 
        }
    }
    
    draw_img(u, v, str); 
}

void draw_axe (int u, int v) 
{ 
    char str [121]; 
    for (int i = 0; i < 121; i++) {
        switch (i) {
            case 25: 
            case 15: 
            case 5: 
            case 17: 
            case 29: 
                str[i] = '3';
                break;
            case 16:
            case 27:
            case 38:
            case 49:
            case 60:
            case 71:
            case 82: 
            case 93:
            case 104: 
            case 115: 
                str[i] = 'D';
                break;
            default: 
                str[i] = 'B';
                break;
        }
    }
    
    draw_img(u, v, str);
}

void draw_upper_status()
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);
    
    // Add other status info drawing code here
}

void draw_lower_status()
{
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);
    
    // Add other status info drawing code here
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


