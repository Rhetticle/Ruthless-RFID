/*
 * terminal.c
 *
 *  Created on: Jul 1, 2024
 *      Author: rhett
 */

#include "stm32f4xx.h"
#include "terminal.h"
#include <stdio.h>

/**
 * Move terminal cursor
 * @param x - X location to move to
 * @param y - Y locationto move to
 * */
void move_terminal_cursor(int x, int y) {
	printf("\x1b[%d;%dH", x, y);
}

/**
 * Clear entire terminal
 * */
void clear_terminal(void) {
	printf("\x1b[2J");
}

/**
 * Clear to end of line
 * */
void clear_to_end_of_line(void) {
	printf("\x1b[K");
}

/**
 * Set terminal colour (text or background)
 * @param colour - Colour to set to (See terminal.h)
 * */
void set_colour (TerminalColour colour) {
	printf("\x1b[%dm", colour);
}
