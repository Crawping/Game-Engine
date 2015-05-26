#pragma once

#ifdef DEBUG_MENU_ENABLED	// This wraps the whole file
#include "DebugMenu.h"
#include "Graphics.h"
#include "Sprite.h"
#include "DebugCheckBox.h"
#include "DebugButton.h"

DebugMenu::DebugMenu(float i_screenXLocation, float i_screenYStartLocation, float i_menuItemWidth, float i_menuItemHeight) :m_screenXLocation(i_screenXLocation),
m_screenYStartLocation(i_screenYStartLocation),
m_menuItemWidth(i_menuItemWidth),
m_menuItemHeight(i_menuItemHeight),
m_menuItemCount(0),
m_currentSelection(0),
m_active(false)
{
	Engine::Quad indicatorQuad = Engine::Quad(m_screenXLocation - m_menuItemWidth*0.65f, m_screenYStartLocation - m_menuItemHeight / 2, m_menuItemHeight / 5.0f, m_menuItemHeight / 1.5f);
	m_currentItemPointer.SetQuad(indicatorQuad);
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", &m_currentItemPointer);
	Engine::Graphics::GetInstance()->addDebugSprite(&m_currentItemPointer);
}

void DebugMenu::Activate(bool active)
{
	m_active = active;
}


void DebugMenu::Update(MenuAction i_action, HWND i_window)
{
	if (i_action == EUp)
	{
		if (m_currentSelection == 0)
		{
			m_currentSelection = m_widgets.size()-1;
		}
		else
			m_currentSelection--;
	}
	if (i_action == EDown)
	{
		if (m_currentSelection == (m_widgets.size()-1))
		{
			m_currentSelection = 0;
		}
		else
			m_currentSelection++;
	}
	m_currentItemPointer.SetQuad(Engine::Quad(m_screenXLocation - m_menuItemWidth *0.65f, m_screenYStartLocation - m_menuItemHeight / 2 - m_currentSelection*m_menuItemHeight, m_menuItemHeight /5.0f, m_menuItemHeight / 1.5f));
	m_currentItemPointer.FillQuadMeshBuffer(i_window);
	if (i_action == ELeft)
	{
		m_widgets[m_currentSelection]->updateWidget(false, i_window);
	}
	if (i_action == ERight)
	{
		m_widgets[m_currentSelection]->updateWidget(true, i_window);
	}
}


void DebugMenu::CreateSlider(char *name, float* value, float min, float max)
{
	Engine::DebugSlider* sliderInstance = new Engine::DebugSlider(std::string(name), m_screenXLocation, m_screenYStartLocation - m_menuItemHeight*m_menuItemCount,
		m_menuItemWidth, m_menuItemHeight,value, min,max);
	Engine::Sprite** sliderSprites = new Engine::Sprite*[2];
	sliderInstance->GetSliderSprites(sliderSprites);
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", sliderSprites[0]);
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", sliderSprites[1]);
	Engine::Graphics::GetInstance()->addDebugSprite(sliderSprites[0]);
	Engine::Graphics::GetInstance()->addDebugSprite(sliderSprites[1]);
	m_widgets.push_back(sliderInstance);


	CreateWidgetText(name);
	m_menuItemCount++;
	
}
void DebugMenu::SetRectInScreenCoordinates(Engine::Quad i_quad, RECT* o_rect)
{
	Engine::NativeResolution resolution = Engine::Sprite::GetNativeResolution();
	float left = (1 + i_quad.getTopLeftX())*eae6320::UserSettings::GetWidth() / 2;
	float top = (1 - i_quad.getTopLeftY())*eae6320::UserSettings::GetHeight() / 2;
	float right = left + i_quad.getWidth()*eae6320::UserSettings::GetWidth() / 2;;
	float bottom = top + i_quad.getHeight()*eae6320::UserSettings::GetHeight()/2;;
	SetRect(o_rect, (int)left, (int)top, (int)right, (int)bottom);
}

void DebugMenu::CreateWidgetText(char* i_name)
{
	Engine::Quad textQuad(m_screenXLocation - m_menuItemWidth / 2, m_screenYStartLocation - m_menuItemHeight*m_menuItemCount - m_menuItemHeight / 2, m_menuItemWidth, m_menuItemHeight);
	RECT textRect;
	SetRectInScreenCoordinates(textQuad, &textRect);
	char* textName = _strdup(i_name);
	Engine::Graphics::GetInstance()->addDebugText(textName, textRect);
}

void DebugMenu::CreateCheckBox(char *name, bool* value, void(*myfunc)(bool, void*), void* i_param)
{
	Engine::DebugCheckBox* checkBoxInstance = new Engine::DebugCheckBox(std::string(name), m_screenXLocation, m_screenYStartLocation - m_menuItemHeight*m_menuItemCount,
		m_menuItemHeight / 2, m_menuItemHeight, value, myfunc, i_param);
	Engine::Sprite** boxSprites = new Engine::Sprite*[2];
	checkBoxInstance->GetBoxSprites(boxSprites);
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", boxSprites[0]);
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", boxSprites[1]);
	Engine::Graphics::GetInstance()->addDebugSprite(boxSprites[0]);
	Engine::Graphics::GetInstance()->addDebugSprite(boxSprites[1]);
	m_widgets.push_back(checkBoxInstance);


	CreateWidgetText(name);
	m_menuItemCount++;
}


void DebugMenu::CreateText(char *name, char* value)
{
	Engine::Quad textQuad(m_screenXLocation, m_screenYStartLocation - m_menuItemHeight*m_menuItemCount - m_menuItemHeight / 2, m_menuItemWidth, m_menuItemHeight);
	RECT textRect;
	SetRectInScreenCoordinates(textQuad, &textRect);
	Engine::Graphics::GetInstance()->addDebugText(value, textRect);
	CreateWidgetText(name);
	m_menuItemCount++;
}


void DebugMenu::CreateButton(char *name, void(*myfunc)(void))
{
	Engine::DebugButton* buttonInstance = new Engine::DebugButton(std::string(name), m_screenXLocation, m_screenYStartLocation - m_menuItemHeight*m_menuItemCount,
		m_menuItemWidth, m_menuItemHeight,myfunc);
	Engine::Sprite* buttonSprite = buttonInstance->GetButtonSprite();
	Engine::Graphics::GetInstance()->AssignMaterial("data/spriteMaterial_RedBar.txt", buttonSprite);
	Engine::Graphics::GetInstance()->addDebugSprite(buttonSprite);
	m_widgets.push_back(buttonInstance);


	CreateWidgetText(name);
	m_menuItemCount++;
}


#endif	// DEBUG_MENU_ENABLED