#include <stdio.h>
#include <stdlib.h>

//Shortcut Macros
#define is_down(b) input -> buttons[b].is_down
#define pressed(b) input -> buttons[b].is_down && input -> buttons[b].changed
#define released(b) !input -> buttons[b].is_down && input -> buttons[b].changed

//Half-Size of Width (X) and Height (Y) of Arena
float arena_half_size_x = 85;
float arena_half_size_y = 45;

//Half-Size of Length (X) and Height (Y) of both Players 
float player_half_size_x = 2.5;
float player_half_size_y = 12;

//Half-Size of Ball ((X) and (Y) are equal, so we only need one variable)
float ball_half_size = 1;

//Player Y-Coordinate
float player1_y;
float player2_y;

//Player Velocity
float player1_dy;
float player2_dy;

//Ball X-Coordinate, Y-Coordinate, X-Velocity, and Y-Velocity
float ball_x;
float ball_y;
float ball_dx = 100;
float ball_dy;

//Scores for Plaeyr 1 and Player 2
int player1_score;
int player2_score;

//Different Game Modes
enum Game_Mode
{
	GM_MENU,
	GM_GAMEPLAY
};

//Different Pause Menu Options
enum Pause_Menu
{
	PM_MENU,
	PM_CONTROLS,
	PM_QUIT
};

Game_Mode current_game_mode;
Pause_Menu pause_menu_option;

int hot_button;
int pause_button;

bool enemy_is_ai;
bool continue_playing;

/*
* Runs a Pong Game!
*/
internal void simulate_game(Input* input, float dt)
{
	if (current_game_mode == GM_MENU)
	{
		//----------------------------------------------------//
		//                 **Screen Creation**                //
		//----------------------------------------------------//
		{
			//Create Screen
			clear_screen(0x600080);

			//Create Arena
			draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);
		}
		//----------------------------------------------------//
		//                    **Main Menu**                   //
		//----------------------------------------------------//
		{
			//Change Hot Button
			if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
			{
				hot_button = !hot_button;
			}

			//Select Game Mode
			if (pressed(BUTTON_ENTER))
			{
				current_game_mode = GM_GAMEPLAY;
				enemy_is_ai = hot_button ? 0 : 1;
				hot_button = 0;
			}

			//Highlight Game Mode
			if (hot_button == 0)
			{
				draw_text("SINGLE PLAYER", -80, -10, 1, 0xff00ff);
				draw_text("MULTIPLAYER", 16, -10, 1, 0xcccccc);
			}
			else
			{
				draw_text("SINGLE PLAYER", -80, -10, 1, 0xcccccc);
				draw_text("MULTIPLAYER", 16, -10, 1, 0xff00ff);
			}

			//Write Title, Sub-Title, and Links to Portfolio
			draw_text("PONG DEMO", -50, 40, 2, 0xcccccc);
			draw_text("PLAY AGAINST THE ALMIGHTY AI OR WITH A FRIEND", -81, 22, .6, 0xcccccc);
			draw_text("GITHUB AND ITCH IO HERE", -83, 15, 1.22, 0xffff00);
		}
	}
	else if (current_game_mode == GM_GAMEPLAY)
	{
		//----------------------------------------------------//
		//                 **Screen Creation**                //
		//----------------------------------------------------//
		{
			//Create Screen
			clear_screen(0x600080);

			//Create Arena
			draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);
		}
		//----------------------------------------------------//
		//                 **Object Creation**                //
		//----------------------------------------------------//
		{
			//Create Player 1
			draw_rect(80, player1_y, player_half_size_x, player_half_size_y, 0x0000ff);//Blue

			//Create Player 2
			draw_rect(-80, player2_y, player_half_size_x, player_half_size_y, 0xff0000);//Red

			//Create Ball
			draw_rect(ball_x, ball_y, ball_half_size, ball_half_size, 0xffffff);//White
		}
		//----------------------------------------------------//
		//                    **Gameplay**                    //
		//----------------------------------------------------//
		{
			float current_dt = dt;

			if (pressed(BUTTON_PAUSE) || pressed(BUTTON_ESCAPE))
			{
				pause_button = !pause_button;
			}

			if (pause_button)
			{
				//----------------------------------------------------//
				//                   **Pause Menu**                   //
				//----------------------------------------------------//
				{
					current_dt = 0.f;

					if (pause_menu_option == PM_MENU)
					{
						draw_rect(0, 0, 44.75, 47.75, 0xcccccc);

						//Pause Menu Border
						draw_rect(-45, 0, .25, 48, 0xff00ff);
						draw_rect(45, 0, .25, 48, 0xff00ff);
						draw_rect(0, 47.75, 44.75, .25, 0xff00ff);
						draw_rect(0, -47.75, 44.75, .25, 0xff00ff);

						draw_text("PAUSE MENU", -35.5, 35, 1.22, 0xff00ff);
						draw_rect(-.25, 25, 35, .7, 0xff00ff);

						//Change Pause Menu Option
						if (pressed(BUTTON_UP) || pressed(BUTTON_DOWN))
						{
							if (pressed(BUTTON_DOWN) && hot_button < 3)
							{
								hot_button += 1;
							}
							else if (pressed(BUTTON_UP) && hot_button > 0)
							{
								hot_button -= 1;
							}
							else if (pressed(BUTTON_DOWN) && hot_button == 3)
							{
								hot_button = 0;
							}
							else
							{
								hot_button = 3;
							}
						}

						//Select Pause Menu Option
						if ((pressed(BUTTON_ENTER) && hot_button == 0))
						{
							pause_button = false;
						}
						else if (pressed(BUTTON_ENTER) && hot_button == 1)
						{
							hot_button = 1;
							pause_menu_option = PM_CONTROLS;
						}
						else if (pressed(BUTTON_ENTER) && hot_button == 2)
						{
							player1_y = 0;
							player2_y = 0;

							player1_dy = 0;
							player2_dy = 0;

							ball_x = 0;
							ball_y = 0;
							ball_dx = 100;
							ball_dy = 0;
							
							player1_score = 0;
							player2_score = 0;
							
							pause_button = false;
							hot_button = 0;
							current_game_mode = GM_MENU;
						}
						else if (pressed(BUTTON_ENTER) && hot_button == 3)
						{
							hot_button = 1;
							pause_menu_option = PM_QUIT;
						}

						//Highlight Pause Menu Option
						if (hot_button == 0)
						{
							draw_text("CONTINUE", -16.25, 17, .66, 0xffff00);
							draw_text("CONTROLS", -16.25, 2, .66, 0xff00ff);
							draw_text("GAME MODE", -17.65, -13, .66, 0xff00ff);
							draw_text("QUIT GAME", -17.25, -28, .66, 0xff00ff);
						}
						else if (hot_button == 1)
						{
							draw_text("CONTINUE", -16.25, 17, .66, 0xff00ff);
							draw_text("CONTROLS", -16.25, 2, .66, 0xffff00);
							draw_text("GAME MODE", -17.65, -13, .66, 0xff00ff);
							draw_text("QUIT GAME", -17.25, -28, .66, 0xff00ff);
						}
						else if (hot_button == 2)
						{
							draw_text("CONTINUE", -16.25, 17, .66, 0xff00ff);
							draw_text("CONTROLS", -16.25, 2, .66, 0xff00ff);
							draw_text("GAME MODE", -17.65, -13, .66, 0xffff00);
							draw_text("QUIT GAME", -17.25, -28, .66, 0xff00ff);
						}
						else
						{
							draw_text("CONTINUE", -16.25, 17, .66, 0xff00ff);
							draw_text("CONTROLS", -16.25, 2, .66, 0xff00ff);
							draw_text("GAME MODE", -17.65, -13, .66, 0xff00ff);
							draw_text("QUIT GAME", -17.25, -28, .66, 0xffff00);
						}
					}
					else if (pause_menu_option == PM_CONTROLS)
					{
						draw_rect(0, 0, 44.75, 47.75, 0xcccccc);

						//Controls Menu Border
						draw_rect(-45, 0, .25, 48, 0xff00ff);
						draw_rect(45, 0, .25, 48, 0xff00ff);
						draw_rect(0, 47.75, 44.75, .25, 0xff00ff);
						draw_rect(0, -47.75, 44.75, .25, 0xff00ff);

						draw_text("CONTROLS", -28, 35, 1.22, 0xff00ff);
						draw_rect(-.25, 25, 35, .7, 0xff00ff);

						draw_text("PLAYER 1", -14.25, 17, .66, 0x0000ff);
						draw_text("UP..........^", -24.75, 9.5, .66, 0x0000ff);
						draw_text("DOWN........v", -24.75, 2, .66, 0x0000ff);

						draw_text("PLAYER 2", -14.25, -13, .66, 0xff0000);
						draw_text("UP..........W", -24.75, -20.5, .66, 0xff0000);
						draw_text("DOWN........S", -24.75, -28, .66, 0xff0000);

						//Select Controls Menu Option
						if (pressed(BUTTON_ENTER))
						{
							pause_menu_option = PM_MENU;
						}
					}
					else
					{
						draw_rect(0, 0, 44.75, 47.75, 0xcccccc);

						//Quit Menu Border
						draw_rect(-45, 0, .25, 48, 0xff00ff);
						draw_rect(45, 0, .25, 48, 0xff00ff);
						draw_rect(0, 47.75, 44.75, .25, 0xff00ff);
						draw_rect(0, -47.75, 44.75, .25, 0xff00ff);

						draw_text("ARE YOU SURE?", -35.5, 35, .95, 0xff00ff);
						draw_rect(-.25, 25, 35, .7, 0xff00ff);
						
						//Change Quit Menu Option
						if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT))
						{
							if (pressed(BUTTON_LEFT) && hot_button == 0)
							{
								hot_button = 1;
							}
							else if (pressed(BUTTON_RIGHT) && hot_button == 0)
							{
								hot_button = 1;
							}
							else if (pressed(BUTTON_LEFT) && hot_button == 1)
							{
								hot_button = 0;
							}
							else
							{
								hot_button = 0;
							}
						}

						//Select Quit Menu Option
						if ((pressed(BUTTON_ENTER) && hot_button == 0))
						{
							exit(0);
						}
						else if (pressed(BUTTON_ENTER) && hot_button == 1)
						{
							pause_menu_option = PM_MENU;
							hot_button = 3;
						}

						//Highlight Quit Menu Option
						if (hot_button == 0)
						{
							draw_text("YES", -17.25, -7.5, .66, 0xffff00);
							draw_text("NO", 9.75, -7.5, .66, 0xff00ff);
						}
						else
						{
							draw_text("YES", -17.25, -7.5, .66, 0xff00ff);
							draw_text("NO", 9.75, -7.5, .66, 0xffff00);
						}
					}
				}
			}
			else if (!pause_button || continue_playing)
			{
				//----------------------------------------------------//
				//                **Gameplay Physics**                //
				//----------------------------------------------------//
				{
					current_dt = dt;
					//Initial Player Acceleration
					float player1_ddy = 0.f;
					float player2_ddy = 0.f;

					//Speed in units per second (u/s)
					float speed = 50.f;

					//Player 1 Actions
					if (is_down(BUTTON_UP)) player1_ddy += 2000;
					if (is_down(BUTTON_DOWN)) player1_ddy -= 2000;

					//Player 2 Actions
					if (!enemy_is_ai)//Multiplayer
					{
						if (is_down(BUTTON_W)) player2_ddy += 2000;
						if (is_down(BUTTON_S)) player2_ddy -= 2000;
					}
					else if (enemy_is_ai)//Single Player
					{
						//if (ball_y > player2_y + 2.f) player2_ddy += 1000;
						//if (ball_y < player2_y - 2.f) player2_ddy -= 1000;
						player2_ddy = (ball_y - player2_y) * 100;
						if (player2_ddy > 1300) player2_ddy = 1300;
						if (player2_ddy > 1300) player2_ddy = 1300;
					}

					//Player Friction
					player1_ddy -= player1_dy * 8.f;
					player2_ddy -= player2_dy * 8.f;

					//Player Velocity
					player1_y = player1_y + player1_dy * current_dt + player1_ddy * current_dt * current_dt * .5f;
					player2_y = player2_y + player2_dy * current_dt + player2_ddy * current_dt * current_dt * .5f;

					//Player Acceleration
					player1_dy = player1_dy + player1_ddy * current_dt;
					player2_dy = player2_dy + player2_ddy * current_dt;

					//Ball Velocity
					ball_x += ball_dx * current_dt;
					ball_y += ball_dy * current_dt;
				}
			}
		}
		//----------------------------------------------------//
		//               **Collision Detection**              //
		//----------------------------------------------------//
		{
			//Up Collision for Player 1
			if (player1_y + player_half_size_y > arena_half_size_y)
			{
				player1_y = arena_half_size_y - player_half_size_y;
				player1_dy *= -.75f;
			}

			//Down Collision for Player 1
			if (player1_y - player_half_size_y < -arena_half_size_y)
			{
				player1_y = -arena_half_size_y + player_half_size_y;
				player1_dy *= -.75f;
			}

			//Up Collision for Player 2
			if (player2_y + player_half_size_y > arena_half_size_y)
			{
				player2_y = arena_half_size_y - player_half_size_y;
				player2_dy *= -.75f;
			}

			//Down Collision for Player 2
			if (player2_y - player_half_size_y < -arena_half_size_y)
			{
				player2_y = -arena_half_size_y + player_half_size_y;
				player2_dy *= -.75f;
			}

			//Player 1 Ball Collision
			if (ball_x + ball_half_size > 80 - player_half_size_x &&
				ball_x - ball_half_size < 80 + player_half_size_x &&
				ball_y + ball_half_size > player1_y - player_half_size_y &&
				ball_y + ball_half_size < player1_y + player_half_size_y)
			{
				ball_x = 80 - player_half_size_x - ball_half_size;
				ball_dx *= -1;
				ball_dy = player1_dy * .75f + (ball_y - player1_y) * 2;
			}

			//Player 2 Ball Collision
			if (ball_x + ball_half_size > -80 - player_half_size_x &&
				ball_x - ball_half_size < -80 + player_half_size_x &&
				ball_y + ball_half_size > player2_y - player_half_size_y &&
				ball_y + ball_half_size < player2_y + player_half_size_y)
			{
				ball_x = -80 + player_half_size_x + ball_half_size;
				ball_dx *= -1;
				ball_dy = player2_dy * .75f + (ball_y - player2_y) * 2;
			}

			//Arena Top Ball Collision
			if (ball_y + ball_half_size > arena_half_size_y)
			{
				ball_y = arena_half_size_y - ball_half_size;
				ball_dy *= -1;
			}

			//Arena Bottom Ball Collision
			if (ball_y - ball_half_size < -arena_half_size_y)
			{
				ball_y = -arena_half_size_y + ball_half_size;
				ball_dy *= -1;
			}

			//Ball Exits Arena Right
			if (ball_x + ball_half_size > arena_half_size_x)
			{
				ball_dx *= -1;
				ball_dy = 0;
				ball_x = 0;
				ball_y = 0;
				player2_score++;
			}

			//Ball Exits Arena Left
			if (ball_x - ball_half_size < -arena_half_size_x)
			{
				ball_dx *= -1;
				ball_dy = 0;
				ball_x = 0;
				ball_y = 0;
				player1_score++;
			}
		}
		//----------------------------------------------------//
		//                  **DisplayScore**                  //
		//----------------------------------------------------//
		{
			if (!pause_button)
			{
				//Player 1 Score
				draw_number(player1_score, 10, 40, 1.f, 0x00ff22);//Green

				//Player 2 Score
				draw_number(player2_score, -10, 40, 1.f, 0x00ff22);//0xbbffbb
			}
		}
	}
}