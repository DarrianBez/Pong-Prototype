/**
* A simple Windows Pong application.
*
* @author Darrian Bezayiff
* @date 09/09/22
*/

#include "utilities.cpp"
#include <windows.h>

/**
* Creates a boolean that tells whether the game is running or not.
*/
global_variable bool running = true;

/**
* A struct for the window size.
*/
struct Render_State
{
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

/**
* Creates a global variable of the render state.
*/
global_variable Render_State render_state;

#include "button_events.cpp"
#include "renderer.cpp"
#include "game.cpp"

/**
* The callback function for a Windows application.
* Sets the window size, destroys, or quits depending on the message.
*/
LRESULT CALLBACK window_callback (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
		}
		break;

		case WM_CLOSE:
		{
			running = false;
			DestroyWindow(hwnd);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			result = 0;
		}
		break;

		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

/**
* Main for a Windows application. Creates a window class,
* registers it, and creates a new window for the game to use.
* Also simulates and renders the game.
*/
int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	FreeConsole();
	ShowCursor(FALSE);

	//Creates a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	//Registers window class
	RegisterClass(&window_class);

	//Creates a window
	HWND window = CreateWindow(window_class.lpszClassName, L"Pong Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		//Full Screen
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO monitor_info = { sizeof(monitor_info) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
		SetWindowPos(window, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top, monitor_info.rcMonitor.right - monitor_info.rcMonitor.left, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	HDC hdc = GetDC(window);

	//Input array for the possible valid User Inputs
	Input input = {};
	
	//Starts the timer for each frame
	float delta_time = 0.f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	//Finds how many cycles the CPU can run in one second
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	//This is where the game takes place!
	while (running)
	{
		//User Input
		MSG message;

		//Resets the buttons changed state to false for each frame
		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}

		//Listens to User Input
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
				u32 vk_code = (u32)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk:\
{\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}\
break;

					switch (vk_code)
					{
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_ENTER, VK_RETURN);
						process_button(BUTTON_PAUSE, 'P');
						process_button(BUTTON_ESCAPE, VK_ESCAPE);
					}
				}
				break;

				default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}
		//Simulates the game
		simulate_game(&input, delta_time);//delta_time);

		//Renders the game
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		//Ends the timer and gives the delta-time in seconds per frame
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart)/ performance_frequency;

		//Resets the timer
		frame_begin_time = frame_end_time;
	}
}