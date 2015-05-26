#ifndef IDEBUGWIDGET_H
#define IDEBUGWIDGET_H
#include <Windows.h>
namespace Engine
{
	class IDebugWidget
	{
	public:
		virtual void updateWidget(bool i_Increment, HWND i_mainWindow) = 0;
		virtual ~IDebugWidget() {};
	};
}
#endif