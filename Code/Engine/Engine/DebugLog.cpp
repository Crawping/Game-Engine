#include "DebugLog.h"

void DebugOutput(const char* i_str,...)
{
	const int buf_length = 1000;
	char buf[buf_length];
	va_list args;
	va_start(args, i_str);
	vsprintf_s(buf, buf_length, i_str, args);
	OutputDebugString(buf);


}