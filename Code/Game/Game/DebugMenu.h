#ifndef DEBUGMENU_H
#define DEBUGMENU_H

#include "DebugSlider.h"
#include "Graphics.h"
#include "Material.h"
#include "Sprite.h"
enum MenuAction
{
	EUp,
	EDown,
	ELeft,
	ERight,
};
#ifdef DEBUG_MENU_ENABLED

class DebugMenu
{
	std::vector<Engine::IDebugWidget*> m_widgets;
	Engine::Material* m_material;
	float m_screenXLocation;
	float m_screenYStartLocation;
	float m_menuItemWidth;
	float m_menuItemHeight;
	int m_menuItemCount;
	Engine::Sprite m_currentItemPointer;
	int m_currentSelection;
	void SetRectInScreenCoordinates(Engine::Quad i_quad, RECT* o_rect);
	bool m_active;
public:
	DebugMenu(){};
	DebugMenu(float i_screenXLocation, float i_screenYStartLocation, float i_menuItemWidth, float i_menuItemHeight);
	~DebugMenu();
	void Activate(bool active);
	void Update(MenuAction i_action, HWND i_window);
	void CreateWidgetText(char* i_name);
	void CreateSlider(char *name, float* value, float min, float max);
	void CreateCheckBox(char *name, bool* value, void(*myfunc)(bool, void*), void* i_param);
	void CreateText(char *name, char* value);
	void CreateButton(char *name, void(*myfunc)(void));
	bool GetState(){ return m_active; }
};

#else

class DebugMenu
{
public:
	// All of these functions will compile out to nothing.
	DebugMenu(float i_screenXLocation, float i_screenYStartLocation, float i_menuItemWidth, float i_menuItemHeight){}
	void Activate(bool active){}
	void Update(MenuAction i_action, HWND i_window){}
	void CreateWidgetText(char* i_name){}
	void CreateSlider(char *name, float* value, float min, float max){}
	void CreateCheckBox(char *name, bool* value, void(*i_callback)(bool, void*), void* i_param){}
	void CreateText(char *name, char* value){}
	void CreateButton(char *name, void(*myfunc)(void)){}
	bool GetState(){ return false; }

};
#endif // DEBUG_MENU_ENABLED

#endif // DEBUGMENU_H