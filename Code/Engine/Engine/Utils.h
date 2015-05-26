#ifndef UTILS_H
#define UTILS_H
#include <assert.h>
#include <math.h>

namespace Engine
{
	inline bool isNAN(const float);
	inline bool isApproxZero(const float);
	inline bool compareFloats(float, float, const int);
	inline unsigned int RoundUpToMultipleOf(unsigned int, unsigned int);
	inline unsigned int RoundDownToMultipleOf(unsigned int, unsigned int);
	inline bool IsMultipleOf(unsigned int, unsigned int);
	inline float MetersToPixels();
	inline float clamp(const float, const float);
	inline float degreesToRadians(float);
	void utilsUnitTest();
}
#include "Utils.inl"
#endif 
