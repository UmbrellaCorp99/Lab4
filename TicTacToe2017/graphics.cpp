//Alexander Young
//Lab 4

#include <allegro5/allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>	
#include <allegro5\allegro_native_dialog.h> 
#include "logic.h"
#include <time.h>



void set_graphics_x_o(int x, int y, logic &game_logic, int &turn);
void draw_board();
void draw_x(int x, int y);
void draw_o(int x, int y);
void game_message(bool &gameover, logic &game_logic);
void turn_xo(int x, int y, int &turn, int boardx, int boardy, logic  &game_logic);

int main(void)
{
	//instantiating the logic class as game_logic
	logic  game_logic;
	//integers for the x and y position of the screen when clicked on
	int posX = 0, posY = 0;
	//turns true once a game-ending condition is met
	bool gameover = false;
	//setting up display
	ALLEGRO_DISPLAY *Screen = NULL;
	int width = 640, height = 480;
	//controlls whos turn it is, 0 is the player and 1 is the bot. alternates between 0 and 1 after a move is made
	int turn = 0;
	srand(time(0));

	//error handling for Allegro initialization
	if (!al_init())
	{
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//creating display and error handling for it
	Screen = al_create_display(width, height);
	if (Screen == NULL)
	{
		al_show_native_message_box(Screen, "Error!", "Failed to create the display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//error handling for mouse installation
	if (!al_install_mouse()) {
		al_show_native_message_box(Screen, "Error!", "Failed to initialize the mouse!\n.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//initializing various Allegro addons needed
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	//draw indicates coltrols when it's time to draw to the screen, done indicates if player clicks the X button to close the display
	bool draw = false, done = false;;
	
	//creating Allegro event queue setup
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	event_queue = al_create_event_queue();


	al_register_event_source(event_queue, al_get_display_event_source(Screen));
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_register_event_source(event_queue, al_get_mouse_event_source());

	//setting up the board logic code and visual display
	game_logic.setup();
	draw_board();

	al_flip_display();
	while (!done && !gameover)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		//close the display if user clicks X button on window
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		//if user clicks the left mouse button, enable drawing X and O to the display
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			//if mouse was clicked, record X and Y position of the display that was clicked on
			if (ev.mouse.button & 1)
			{
				posX = ev.mouse.x;
				posY = ev.mouse.y;

				draw = true;
			}
		}
		draw_board();
		//if mouse was clicked on the display
		if (draw)
		{
			//user's X placement
			if (turn == 0) {
				set_graphics_x_o(posX, posY, game_logic, turn);
			}
			draw = false;
		}
		game_message(gameover, game_logic);
		al_flip_display();
		//bot's turn to place O, must be a valid entry (space is not taken already)
		if (turn == 1 && !gameover && !done) {
			while (turn == 1) {
				int posx = rand() % 640;
				int posy = rand() % 375;
				set_graphics_x_o(posx, posy, game_logic, turn);
			}
		}
	}
	//once game is over, wait 5 seconds and then destroy envent queue and display from memory
	al_rest(5.0);
	al_destroy_event_queue(event_queue);
	al_destroy_display(Screen);						//destroy our display object

	return 0;
}

//This function draws the board on the display for the user to see
//Takes no parameters
//No return
void draw_board()
{
	al_draw_line(0, 375, 640, 375, al_map_rgb(255, 0, 0), 2);
	al_draw_filled_rectangle(0, 376, 640, 480, al_map_rgb(200, 200, 200));

	al_draw_line(0, 125, 640, 125, al_map_rgb(255, 255, 255), 2);
	al_draw_line(0, 250, 640, 250, al_map_rgb(255, 255, 255), 2);
	al_draw_line(213, 0, 213, 375, al_map_rgb(255, 255, 255), 2);
	al_draw_line(426, 0, 426, 375, al_map_rgb(255, 255, 255), 2);


}

//This function draws a red X on the display
//Takes two integers as the parameters, they control where on the display to draw the X
//No return
void draw_x(int x, int y)
{
	al_draw_line(x - 106, y - 62, x + 106, y + 62, al_map_rgb(255, 0, 0), 2);
	al_draw_line(x - 106, y + 62, x + 106, y - 62, al_map_rgb(255, 0, 0), 2);
}

//This function draws a yellow O on the display
//Takes two integers as the parameters, they control where on the display to draw the O
//No return
void draw_o(int x, int y)
{
	al_draw_circle(x, y, 62, al_map_rgb(255, 255, 0), 4);

}

//This function determines whos turn it is and calls the draw_x or draw_o function accordingly, then flips the value of turn
//Takes an x integer, a y integer, the turn integer, boardx and boardy integers for multilevel array indexes, and the instantiation of the logic class as parameters
//No return
void turn_xo(int x, int y, int &turn, int boardx, int boardy, logic  &game_logic)
{
	ALLEGRO_FONT *font = al_load_font("GROBOLD.ttf", 24, 0);
	if (turn == 0)
	{
		if (game_logic.set_x(boardx, boardy) == true)
		{
			draw_x(x, y);
			turn = 1;
		}
	}
	else
	{
		if (game_logic.set_o(boardx, boardy) == true)
		{
			draw_o(x, y);
			turn = 0;
		}
	}
}

//This function determines if the user (or bot) is providing good x and y values before calling the turn_xo function
//Takes two integers for x and y values, the instantiation of the logic class, and the current value of turn
//No return
void set_graphics_x_o(int x, int y, logic &game_logic, int &turn)
{
	if ((x<213) && (y<125))
	{
		turn_xo(106, 62, turn, 0, 0, game_logic);
	}
	else if ((x>213) && (x<426) && (y<125))
	{
		turn_xo(319, 62, turn, 0, 1, game_logic);
	}
	else if ((x>426) && (y<125))
	{
		turn_xo(533, 62, turn, 0, 2, game_logic);
	}
	else if ((x<213) && (y>125) && (y<250))
	{
		turn_xo(106, 186, turn, 1, 0, game_logic);

	}
	else if ((x>213) && (x<426) && (y>125) && (y<250))
	{
		turn_xo(319, 186, turn, 1, 1, game_logic);
	}
	else if ((x>426) && (y>125) && (y<250))
	{
		turn_xo(533, 186, turn, 1, 2, game_logic);
	}
	else if ((x<213) && (y>250) && (y<375))
	{
		turn_xo(106, 314, turn, 2, 0, game_logic);

	}
	else if ((x>213) && (x<426) && (y>250) && (y<375))
	{
		turn_xo(319, 314, turn, 2, 1, game_logic);
	}
	else if ((x>426) && (y>250) && (y<375))
	{
		turn_xo(533, 314, turn, 2, 2, game_logic);
	}
}

//This function generates a message to display to the user based on if the user is still playing or if the game is over, and who won
//Takes a boolean value for if the game is over and the instantiation of the logic class
//No return
void game_message(bool &gameover, logic &game_logic)
{
	bool xwon = false, owon = false, tie = false;
	game_logic.done(tie, xwon, owon);
	ALLEGRO_FONT *font = al_load_font("GROBOLD.ttf", 24, 0);

	if (tie == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "The game is a tie -- screen will close shortly");
		gameover = true;
	}
	else if (xwon == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "X won the game-- screen will close shortly");
		gameover = true;
	}
	else if (owon == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "O won the game-- screen will close shortly");
		gameover = true;
	}
	else
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "Pick a Square");

}


