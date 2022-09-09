/*
* Shortcuts for typing.
*/
typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

/*
* Helps visualize which variables are global or internal.
*/
#define global_variable static
#define internal static

/**
* Clamps the window to make sure the game doesn't
* crash if the objects are not in view.
*/
inline int clamp(int min, int val, int max)
{
	if (val < min)
	{
		return min;
	}

	if (val > max)
	{
		return max;
	}

	return val;
}