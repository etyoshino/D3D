#pragma once
#include<math.h>

constexpr float PI = 3.14149265f;
constexpr float PI_D = 3.1415926535897932;

template <typename T>

/*
* 平方
*/
constexpr auto sq(const T& x)
{
	return x * x;
}

/*
* 夹角
*/
template<typename T>
T wrap_angle(T theta)
{
	const T modded = fmod(theta, (T)2.0 * (T)PI_D);
	return (modded > (T)PI_D) ?
		(modded - (T)2.0 * (T)PI_D) :
		modded;
}

/*
* 插值
*/
template<typename T>
constexpr T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

/*
* 取角度
*/
template<typename T>
constexpr T to_rad(T deg)
{
	return deg * PI / (T)180.0;
}