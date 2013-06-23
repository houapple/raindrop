#ifndef _RD_MATH_H
#define _RD_MATH_H

// a - b µÄËæ»úÊı
template <class T>
inline T random(T a, T b)
{
	return a + rand() % (b - a + 1);
}

#endif