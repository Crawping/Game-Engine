#include <assert.h>
#include <math.h>
inline bool Engine::isNAN(const float i_value)
{
	volatile float x = i_value;
	return(x != x);
}

inline bool Engine::isApproxZero(const float i_value)
{
	if (i_value <= 0.00001f && i_value >= -0.00001f)
		return true;
	else
		return false;
}

inline bool Engine::compareFloats(float i_value1, float i_value2, const int maxUlps)
{
	assert(sizeof(float) == sizeof(int));
	if (i_value1 == i_value2)
		return true;
	int intDiff = abs(*(reinterpret_cast<int*> (&i_value1)) - *(reinterpret_cast<int*> (&i_value2)));
	if (intDiff <= maxUlps)
		return true;
	else
		return false;
}

inline unsigned int Engine::RoundUpToMultipleOf(unsigned int i_num, unsigned int i_roundto)
{
	assert(i_roundto != 0);

	return i_num == 0 ? 0 : ((i_num + (i_roundto - 1)) / i_roundto) * i_roundto;
}

inline unsigned int Engine::RoundDownToMultipleOf(unsigned int i_num, unsigned int i_roundto)
{
	return (i_num / i_roundto) * i_roundto;
}

inline bool Engine::IsMultipleOf(unsigned int i_number, unsigned int i_multiple)
{
	return (i_number % i_multiple) == 0;
}

inline float Engine::MetersToPixels()
{
	return 10.0f;
}

inline float Engine::clamp(const float i_arg1, const float i_arg2)
{
	return i_arg1<i_arg2 ? i_arg1 : i_arg2;
}

inline float Engine::degreesToRadians(float i_Degrees)
{
	return (i_Degrees*3.14159265f / 180.0f);
}