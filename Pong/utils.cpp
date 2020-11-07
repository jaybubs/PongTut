typedef char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef long long s64;
typedef unsigned long long u64;
typedef int s32;
typedef unsigned int u32;

#define global_variable static
#define internal static

inline int clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
