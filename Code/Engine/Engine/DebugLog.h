#ifndef DEBUGLOG_H
#define DEBUGLOG_H
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>
void DebugOutput(const char*, ...);
#ifdef _DEBUG
#define DEBUG_OUTPUT(i_str,...) DebugOutput(i_str ## "File:%s , Line:%d \n",__FILE__, __LINE__,__VA_ARGS__)
#else
#define DEBUG_OUTPUT(i_str,...) void(0)
#endif

#endif