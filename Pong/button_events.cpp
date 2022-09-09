/*
* A struct for button states.
*/
struct Button_State
{
	bool is_down;
	bool changed;
};

/*
* List of possible button User Inputs.
*/
enum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_PAUSE,
	BUTTON_ESCAPE,
	BUTTON_COUNT
};

/*
* Array that holds each button and their state.
*/
struct Input
{
	Button_State buttons[BUTTON_COUNT];
};