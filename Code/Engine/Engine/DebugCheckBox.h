#ifndef DEBUGCHECKBOX_H
#define DEBUGCHECKBOX_H
#include "IDebugWidget.h"
#include "Sprite.h"
#include <string>
namespace Engine
{
	class DebugCheckBox :public IDebugWidget
	{
		Sprite m_outerBoxSprite;
		Sprite m_innerBoxSprite;
		std::string m_name;
		bool* m_value;
		void(*m_callbackFunction)(bool,void* i_param);
		void* m_param;
	public:
		DebugCheckBox(std::string i_name, float i_screenPositionx, float i_screenPositiony,
			float i_width, float i_height, bool* i_value, void(*i_callbackFunction)(bool, void*), void* i_param);

		void GetBoxSprites(Sprite** o_spriteArray);
		void updateWidget(bool i_Increment, HWND i_mainWindow);
	};
}
#endif