/*
* Clears the screen.
*/
internal void clear_screen(unsigned int color)
{
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

/*
* Helper function for draw_rect()
* Draws a rectangle in pixels.
*/
internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

/*
* Scale multiplyer for drawing rectangles.
*/
global_variable float render_scale = 0.01f;

/*
* Draws a rectangle.
*/
internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	//Change to pixels
	int x0 = x - half_size_x;
	int y0 = y - half_size_y;
	int x1 = x + half_size_x;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

/*
* All possible characters for the game.
*/
const char* letters[][7] = {
	//Alphabet		
	" 000",
	"0   0",
	"0   0",
	"00000",
	"0   0",
	"0   0",
	"0   0",

	"0000",
	"0   0",
	"0   0",
	"0000",
	"0   0",
	"0   0",
	"0000",

	" 0000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 0000",

	"0000",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0000",

	"00000",
	"0",
	"0",
	"0000",
	"0",
	"0",
	"00000",

	"00000",
	"0",
	"0",
	"0000",
	"0",
	"0",
	"0",

	" 0000",
	"0",
	"0",
	"0 000",
	"0   0",
	"0   0",
	" 000",

	"0   0",
	"0   0",
	"0   0",
	"00000",
	"0   0",
	"0   0",
	"0   0",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
	"00000",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"0 0",
	" 0",

	"0   0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0   0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"00000",

	"0   0",
	"00 00",
	"00 00",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",

	"0   0",
	"00  0",
	"00  0",
	"0 0 0",
	"0  00",
	"0  00",
	"0   0",

	" 000",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 000",

	"0000",
	"0   0",
	"0   0",
	"0000",
	"0",
	"0",
	"0",

	" 000",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	" 000 0",

	"0000",
	"0   0",
	"0   0",
	"0000",
	"00",
	"0 00",
	"0   0",

	" 0000",
	"0",
	"0",
	" 000",
	"    0",
	"    0",
	"0000",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 000",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0 ",
	"  0 ",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"00000",
	"    0",
	"   0",
	"  0",
	" 0",
	"0",
	"00000",

	//Symbols
	"",
	"",
	"",
	"0000",
	"",
	"",
	"",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"    0",
	"   0",
	"   0",
	"  0",
	" 0",
	" 0",
	"0",

	" 000 ",
	"0   0",
	"   0",
	"  0",
	"  0",
	"",
	"  0",

	//Numbers
	"  0",
	" 00",
	"0 0",
	"  0",
	"  0",
	"  0",
	"00000",

	" 000 ",
	"0   0",
	"   0",
	"  0",
	" 0",
	"0",
	"00000",

	//Arrows
	"  0",
	" 000",
	"0 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"  0",
	"  0",
	"  0",
	"  0",
	"0 0 0",
	" 000",
	"  0"
};

/*
* Draws text.
*/
internal void draw_text(const char* text, float x, float y, float size, u32 color)
{
	float half_size = size * .5f;
	float original_y = y;

	while (*text)
	{
		if (*text != 32)//"Space" character
		{
			float original_x = x;

			const char** letter;
			if (*text == 45) letter = letters[26];//"-" character
			else if (*text == 46) letter = letters[27];//"." character
			else if (*text == 47) letter = letters[28];//"/" character
			else if (*text == 63) letter = letters[29];//"?" character
			else if (*text == 49) letter = letters[30];//"1" character
			else if (*text == 50) letter = letters[31];//"2" character
			else if (*text == 94) letter = letters[32];//"UP ARROW" character
			else if (*text == 118) letter = letters[33];//"DOWN ARROW" character
			else letter = letters[*text - 'A'];//alphabetical character

			for (int i = 0; i < 7; i++)
			{
				const char* row = letter[i];
				while (*row)
				{
					if (*row == '0')
					{
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		x += size * 6.f;//Space between letters
		y = original_y;
		text++;
	}
}

/*
* Draws numbers.
*/
internal void draw_number(int number, float x, float y, float size, u32 color)
{
	float half_size = size * .5f;

	//----------------------------------------------------//
	//                  **Draw Numbers**                  //
	//----------------------------------------------------//
	{
		bool drew_number = false;
		while (number || !drew_number)
		{
			int digit = number % 10;
			number = number / 10;

			drew_number = true;

			switch (digit)
			{
				//Draws a 0
			case 0:
			{
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x, y + size * 2.f, half_size, half_size, color);
				draw_rect(x, y - size * 2.f, half_size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 1
			case 1:
			{
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 2
			case 2:
			{
				draw_rect(x - size, y - size, half_size, half_size, color);
				draw_rect(x + size, y + size, half_size, half_size, color);
				draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y, 1.5f * size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 3
			case 3:
			{
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x - half_size, y, size, half_size, color);
				draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 4
			case 4:
			{
				draw_rect(x - size, y + size, half_size, 1.5f * size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x, y, half_size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 5
			case 5:
			{
				draw_rect(x - size, y + size, half_size, half_size, color);
				draw_rect(x + size, y - size, half_size, half_size, color);
				draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y, 1.5f * size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 6
			case 6:
			{
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y - size, half_size, half_size, color);
				draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x + half_size, y, size, half_size, color);
				draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 7
			case 7:
			{
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws an 8
			case 8:
			{
				draw_rect(x, y - size * 2.f, half_size, half_size, color);
				draw_rect(x, y + size * 2.f, half_size, half_size, color);
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x, y, half_size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;

			//Draws a 9
			case 9:
			{
				draw_rect(x - size, y + size, half_size, half_size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x - half_size, y, size, half_size, color);
				draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
				x -= size * 4.f;//Space between digits
			}break;
			}
		}
	}
}