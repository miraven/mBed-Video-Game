#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.filled_rectangle (3, 101, 123, 127, WHITE); 
    
}

void erase_speech_bubble()
{
    uLCD.filled_rectangle (3, 117, 123, 127, BLACK);
}

void draw_speech_line(const char* line, int which)
{
    if (which == 0) {
        uLCD.locate(1,13);
        uLCD.printf("%s\n", line); 
    }
    else {
        uLCD.locate(1, 14);
        uLCD.printf("%s\n", line);
    }
}

void speech_bubble_wait()
{
    wait_ms(3000); 
}

// Sequence of speech bubble
void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{
}
