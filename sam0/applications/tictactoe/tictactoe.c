/**
 * \file
 *
 * \brief Example application for GFX Monochrome System Font (sysfont)
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#include <asf.h>
#include <stdlib.h>
#include <stdio.h>

/* Size of a square */
#define SQUARE_SIZE  (LCD_HEIGHT_PIXELS / 3)

/* Size of a cross */
#define CROSS_SIZE  (SQUARE_SIZE / 3)
/* Size of a circle */
#define CIRCLE_SIZE (SQUARE_SIZE / 4)

/* Square coordinates */
#define SQUARE0_X 0
#define SQUARE0_Y 0
#define SQUARE1_X (SQUARE0_X + SQUARE_SIZE)
#define SQUARE1_Y SQUARE0_Y
#define SQUARE2_X (SQUARE1_X + SQUARE_SIZE)
#define SQUARE2_Y SQUARE0_Y
#define SQUARE3_X SQUARE0_X
#define SQUARE3_Y (SQUARE0_Y + SQUARE_SIZE)
#define SQUARE4_X SQUARE1_X
#define SQUARE4_Y SQUARE3_Y
#define SQUARE5_X SQUARE2_X
#define SQUARE5_Y SQUARE3_Y
#define SQUARE6_X SQUARE0_X
#define SQUARE6_Y (SQUARE3_Y + SQUARE_SIZE)
#define SQUARE7_X SQUARE1_X
#define SQUARE7_Y SQUARE6_Y
#define SQUARE8_X SQUARE2_X
#define SQUARE8_Y SQUARE6_Y

#define NUMBER_OF_SQUARES 9

#define STRING_X (SQUARE2_X + SQUARE_SIZE*2)

#define STRING_LENGTH 20

/* Occupied squares */
uint8_t occupied_squares[NUMBER_OF_SQUARES];

/* X and Y coordinates for squares */
const uint8_t square_coord[9][2] = {
		{SQUARE0_X, SQUARE0_Y,},
		{SQUARE1_X, SQUARE1_Y,},
		{SQUARE2_X, SQUARE2_Y,},
		{SQUARE3_X, SQUARE3_Y,},
		{SQUARE4_X, SQUARE4_Y,},
		{SQUARE5_X, SQUARE5_Y,},
		{SQUARE6_X, SQUARE6_Y,},
		{SQUARE7_X, SQUARE7_Y,},
		{SQUARE8_X, SQUARE8_Y,},
		};

/** 
 * \brief Enum for the different buttons
 */
enum button {
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_NONE,	
};

/* Variable to mark number of wins */
uint16_t wins = 0;

/* Variable to mark number of games */
uint16_t games = 0;

/* String to display number of wins */
char win_string[STRING_LENGTH];


/** 
 * \brief Draws the tic-tac-toe board
 *
 */
static void setup_board(void)
{
	/* Clear screen */
	gfx_mono_draw_filled_rect(0, 0, LCD_WIDTH_PIXELS, LCD_HEIGHT_PIXELS,
			GFX_PIXEL_CLR);

	/* Draw vertical lines */
	gfx_mono_draw_line(SQUARE_SIZE, 0, SQUARE_SIZE, SQUARE_SIZE * 3,
			GFX_PIXEL_SET);
	gfx_mono_draw_line(SQUARE_SIZE * 2, 0, SQUARE_SIZE * 2, SQUARE_SIZE * 3,
			GFX_PIXEL_SET);

	/* Draw horizontal lines */
	gfx_mono_draw_line(0, SQUARE_SIZE, SQUARE_SIZE*3, SQUARE_SIZE,
			GFX_PIXEL_SET);
	gfx_mono_draw_line(0, SQUARE_SIZE * 2, SQUARE_SIZE * 3, SQUARE_SIZE * 2,
			GFX_PIXEL_SET);

	/* Print number of games */
	snprintf(win_string, STRING_LENGTH, "Games: %d", games);
	gfx_mono_draw_string(win_string, STRING_X, SQUARE0_Y, &sysfont);

	/* Print number of wins */
	snprintf(win_string, STRING_LENGTH, "Wins: %d", wins);
	gfx_mono_draw_string(win_string, STRING_X, SQUARE3_Y, &sysfont);
	
	/* Clear occupied squares */
	for (uint8_t i = 0; i < NUMBER_OF_SQUARES; i++) {
		occupied_squares[i] = 0;
	}
}

/** 
 * \brief Gets button push
 */
static enum button get_button(void)
{
	if (port_pin_get_input_level(WING_BUTTON_1) == false) {
		return BUTTON_1;
	} else if (port_pin_get_input_level(WING_BUTTON_2) == false) {
		return BUTTON_2;
	} else if (port_pin_get_input_level(WING_BUTTON_3) == false) {
		return BUTTON_3;
	} else {
		/* No button pushed */
		return BUTTON_NONE;
	}
}

/** 
 * \brief Initalizes the display
 */
static void init_display(void)
{
	/* Draw buttons */
	gfx_mono_draw_circle(10, SQUARE3_Y, CIRCLE_SIZE, GFX_PIXEL_SET, GFX_WHOLE);
	gfx_mono_draw_circle((LCD_WIDTH_PIXELS / 3) + 10, SQUARE3_Y, CIRCLE_SIZE,
			GFX_PIXEL_SET, GFX_WHOLE);
	gfx_mono_draw_circle((LCD_WIDTH_PIXELS / 3) * 2 + 10, SQUARE3_Y,
			CIRCLE_SIZE, GFX_PIXEL_SET, GFX_WHOLE);

	/* Print text */
	gfx_mono_draw_string("LEFT", 0, SQUARE6_Y, &sysfont);
	gfx_mono_draw_string("OK", (LCD_WIDTH_PIXELS / 3), SQUARE6_Y, &sysfont);
	gfx_mono_draw_string("RIGHT", (LCD_WIDTH_PIXELS / 3) * 2, SQUARE6_Y,
			&sysfont);
}

/** 
 * \brief Draws a cross in selected square
 */
static void draw_cross(uint8_t square_num)
{
	uint8_t x = square_coord[square_num][0] + SQUARE_SIZE / 4;
	uint8_t y = square_coord[square_num][1] + SQUARE_SIZE / 4;

	/* Draw cross in selected square */
	gfx_mono_draw_line(x, y, x + CROSS_SIZE, y + CROSS_SIZE, GFX_PIXEL_SET);
	gfx_mono_draw_line(x + CROSS_SIZE, y, x, y + CROSS_SIZE, GFX_PIXEL_SET);
}

/** 
 * \brief Draws a circle at selected point
 */
static void draw_circle(uint8_t square_num)
{
	uint8_t x = square_coord[square_num][0] + SQUARE_SIZE / 4;
	uint8_t y = square_coord[square_num][1] + SQUARE_SIZE / 4;
	/* Draw circle in selected square */
	gfx_mono_draw_circle(x + CIRCLE_SIZE, y + CIRCLE_SIZE, CIRCLE_SIZE,
			GFX_PIXEL_SET, GFX_WHOLE);
}

/**
 * \brief Highlight a square
 */
static void highlight_square(uint8_t square_num)
{
	static uint8_t last_square = 0;

	/* Clear current highlighting */
	uint8_t x = square_coord[last_square][0];
	uint8_t y = square_coord[last_square][1];
	gfx_mono_draw_rect(x + 1, y + 1, SQUARE_SIZE - 2, SQUARE_SIZE - 2, GFX_PIXEL_CLR);

	last_square = square_num;

	/* Highlight new square */
	x = square_coord[square_num][0];
	y = square_coord[square_num][1];
	gfx_mono_draw_rect(x + 1, y + 1, SQUARE_SIZE - 2, SQUARE_SIZE - 2, GFX_PIXEL_SET);
}

/** 
 * \brief User turn
 * Waits for the user to push buttons and select a new square
 */
static void user_turn(void)
{
	static uint8_t square_num = 0;
	enum button button_pushed;
	highlight_square(square_num);

	while (true) {

		/* Wait for button interaction */
		do {
			button_pushed = get_button();
		} while (button_pushed == BUTTON_NONE);
		delay_ms(500);

		switch (button_pushed) {
		case BUTTON_1:
			/* Left */
			if (square_num > 0) {
				square_num--;
				highlight_square(square_num);
			}
			break;
		case BUTTON_2:
			if (occupied_squares[square_num] == 0) {
				/* Select square */
				occupied_squares[square_num] = 1;
				draw_circle(square_num);
				return;
			}
			/* Do not break, skip to next square */
		case BUTTON_3:
			/* Right */
			if (square_num < 8) {
				square_num++;
				highlight_square(square_num);
			}
			break;
		default:
			break;
		}
	}
}

/** 
 * \brief Checks for winner
 *
 * \return Number to say if there is a winner or not.
 * \retval 0 No winner
 * \retval 1 Opponent won
 * \retval 2 User won 
 */
static uint8_t we_have_a_winner(void)
{
	uint8_t winner = 0;

	if (occupied_squares[0] == occupied_squares[1] &&
			occupied_squares[1] == occupied_squares[2] &&
			occupied_squares[0] != 0) {

		/* Three in a row on top row */
		winner = occupied_squares[0];
	} else if (occupied_squares[0] == occupied_squares[3] &&
			occupied_squares[3] == occupied_squares[6] &&
			occupied_squares[0] != 0) {

		/* Three in a row on first vertical row */
		winner = occupied_squares[0];
	} else if (occupied_squares[0] == occupied_squares[4] &&
			occupied_squares[4] == occupied_squares[8] &&
			occupied_squares[0] != 0) {

		/* Three in a row on diagonal */
		winner = occupied_squares[0];
	} else if (occupied_squares[1] == occupied_squares[4] &&
			occupied_squares[4] == occupied_squares[7] &&
			occupied_squares[1] != 0) {
		/* Three in a row on second vertical row */
		winner = occupied_squares[1];
	} else if (occupied_squares[2] == occupied_squares[5] &&
			occupied_squares[5] == occupied_squares[8] &&
			occupied_squares[2] != 0) {

		/* Three in a row on third vertical row */
		winner = occupied_squares[2];
	} else if (occupied_squares[3] == occupied_squares[4] &&
			occupied_squares[4] == occupied_squares[5] &&
			occupied_squares[3] != 0) {

		/* Three in a row on second row */
		winner = occupied_squares[3];
	} else if (occupied_squares[6] == occupied_squares[7] &&
			occupied_squares[7] == occupied_squares[8] &&
			occupied_squares[6] != 0) {

		/* Three in a row on third row */
		winner = occupied_squares[6];
	} else if (occupied_squares[2] == occupied_squares[4] &&
			occupied_squares[4] == occupied_squares[6] &&
			occupied_squares[2] != 0) {

		/* Three in a row on diagonal */
		winner = occupied_squares[2];
	}
	/* Return winner or zero */
	return winner;
}

/**
 * \brief Selects a random, unoccupied square
 */
static void opponent_turn(void)
{
	uint8_t square_num;

	do {
		square_num = rand() % 9;
	} while (occupied_squares[square_num]);

	occupied_squares[square_num] = 2;
	draw_cross(square_num);
}

/**
 * \brief Configures buttons
 */
static void init_buttons(void)
{
	struct port_config conf;
	port_get_config_defaults(&conf);

	conf.direction = PORT_PIN_DIR_INPUT;
	conf.input_pull = PORT_PIN_PULL_UP;

	/* Configure all three buttons as inputs */
	port_pin_set_config(WING_BUTTON_1, &conf);
	port_pin_set_config(WING_BUTTON_2, &conf);
	port_pin_set_config(WING_BUTTON_3, &conf);
}

/**
 * \brief Main entry of example application
 */
int main(void)
{
	/* Variable to store the last winner */
	uint8_t winner;

	system_init();
	delay_init();
	gfx_mono_init();

	init_buttons();
	init_display();

	/* Start game */
	while (true) {
		winner = 0;
		/* Wait for button interaction */
		while (get_button() == BUTTON_NONE) {
		}
		delay_ms(500);
		setup_board();

		/* Start playing */
		for (int i = 0; i < 5; i++) {
			/* User's turn */
			user_turn();

			/* Check if the game is over */
			winner = we_have_a_winner();
			if (winner || i == 4) {
				break;
			}

			/* Opponent's turn */
			opponent_turn();

			/* Check if the game is over */
			winner = we_have_a_winner();
			if (winner) {
				break;
			}
		}

		/* Game over, print winner and get ready for restart */
		if (winner == 1) {
			/* User won */
			gfx_mono_draw_string("You won!", STRING_X, 0, &sysfont);
			wins++;
		} else if (winner == 2) {
			gfx_mono_draw_string("You lost!", STRING_X, 0, &sysfont);
		} else {
			gfx_mono_draw_string("No winner!", STRING_X, 0, &sysfont);
		}

		gfx_mono_draw_string("Press a button", STRING_X, SQUARE3_Y, &sysfont);
		games++;
	}
}
