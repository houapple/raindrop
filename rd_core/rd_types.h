#ifndef _RD_TYPES_H
#define _RD_TYPES_H

#ifndef BYTE
typedef unsigned char		BYTE;
#endif

#ifndef WORD
typedef unsigned short		WORD;
#endif

#ifndef DWORD
typedef unsigned long		DWORD;
#endif

#ifndef QWORD
typedef unsigned __int64	QWORD;
#endif


template <class T>	struct Vec2_T
{
	T x, y;

	Vec2_T() : x(0), y(0) {}
	Vec2_T(T _x, T _y) : x(_x), y(_y) {}
	Vec2_T(const Vec2_T<T>& rhs) { x = rhs.x; y = rhs.y; }
	Vec2_T(const Vec2_T<T>* p) { x = p->x; y = p->y; }

	// op
	Vec2_T& operator=(const Vec2_T<T>& rhs) { x = rhs.x; y = rhs.y; return *this; }
	Vec2_T operator-() const { return Vec2_T(-x, -y); }
	Vec2_T operator*(T k) const { return Vec2_T(x * k, y * k); }
	Vec2_T& operator*=(T k) { x *= k; y *= k; return *this }
	Vec2_T operator/(T k) const { return *this * (T)1.0 / k; }
	Vec2_T& operator/=(T k) { return *this *= (T)1.0 / k; }


	void Set(T _x, T _y) { x = _x; y = _y; }
};

template<class T>
Vec2_T<T> operator*(T k, const Vec2_T<T>& rhs)
{
	return Vec2_T(k * rhs.x, k * rhs.y);
}

template<class T1, class T2>
Vec2_T<T1> operator+(const Vec2_T<T1>& lhs,const Vec2_T<T2>& rhs)
{
	return Vec2_T<T1>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<class T1, class T2>
Vec2_T<T1> operator-(const Vec2_T<T1>& lhs,const Vec2_T<T2>& rhs)
{
	return Vec2_T<T1>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<class T1, class T2>
Vec2_T<T1>& operator+=(const Vec2_T<T1>& lhs,const Vec2_T<T2>& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

template<class T1, class T2>
Vec2_T<T1>& operator-=(const Vec2_T<T1>& lhs,const Vec2_T<T2>& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

template<class T>
bool operator==(const Vec2_T<T>& lhs,const Vec2_T<T>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<class T>
bool operator!=(const Vec2_T<T>& lhs,const Vec2_T<T>& rhs)
{
	return ! (lhs == rhs);
}

typedef Vec2_T<int>		Vec2I;
typedef Vec2_T<long>	Vec2L;
typedef Vec2_T<float>	Vec2F;

//////////////////////////////////////////////////////////////////////////

template <class T> struct Rect_T
{
	T left, top, right, bottom;

	Rect_T() : left(0), top(0), right(0), bottom(0) {}
	Rect_T(T l, T t, T r, T b) : left(l), top(t), right(r), bottom(b) {}
	Rect_T(const Rect_T<T>& rhs) { CopyRect(&rhs); }
	Rect_T(const Rect_T<T>* p) { CopyRect(p); }

	// op
	Rect_T& operator=(const Rect_T<T>& rhs) { CopyRect(&rhs); return *this; }
	Rect_T& operator&=(const Rect_T<T>& rhs) { left = max(left, rhs.left); top = max(top, rhs.top); right = min(right, rhs.right); bottom = min(bottom, rhs.bottom); }
	Rect_T& operator|=(const Rect_T<T>& rhs) { left = min(left, rhs.left); top = min(top, rhs.top); right = max(right, rhs.right); bottom = max(bottom, rhs.bottom); }

	void CopyRect(Rect_T<T>* pSrc) { memcpy(this, pSrc, sizeof(Rect_T<T>)); }
	void SetRect(T l, T t, T r, T b) { left = l; top = t; right = r; bottom = b; }

	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	Vec2_T<T> CenterPoint() const { return Vec2_T<T>((left + right) / (T)2.0, (top + bottom) / (T)2.0); }

	bool IsRectEmpty() const { return left >= right || top >= bottom; }
	template<class T1> bool PtInRect (const Vec2_T<T1>& pt) const { return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom; }
	
	template<class T1> void OffsetRect(T1 x, T1 y) { left += (T)x; right += (T)x; top += (T)y; bottom += (T)y; }
	template<class T1> void OffsetRect(const Vec2_T<T1>& pt) { OffsetRect(pt.x, pt.y); }
	template<class T1> void InflateRect(T1 l, T1 t, T1 r, T1 b) { left -= (T)l; right += (T)r; top -= (T)t; bottom += (T)t; }
};

template<class T>
bool operator==(const Rect_T<T>& lhs, const Rect_T<T>& rhs)
{
	return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom;
}

template<class T>
bool operator!=(const Rect_T<T>& lhs, const Rect_T<T>& rhs)
{
	return ! (lhs == rhs);
}

template<class T>
Rect_T<T> operator&(const Rect_T<T>& lhs, const Rect_T<T>& rhs)
{
	Rect_T<T> rect;
	rect.left = max(lhs.left, rhs.left);
	rect.right = min(lhs.right, lhs.right);
	rect.top = max(lhs.top, rhs.top);
	rect.bottom = min(lhs.bottom, rhs.bottom);
	return rect;
}

typedef Rect_T<int>		RectI;
typedef Rect_T<long>	RectL;
typedef Rect_T<float>	RectF;

#endif