/*
	This file manages user-configurable settings
*/

#ifndef EAE6320_USERSETTINGS_H
#define EAE6320_USERSETTINGS_H

// Header Files
//=============

// Interface
//==========

namespace eae6320
{
	namespace UserSettings
	{
		unsigned int GetHeight();
		unsigned int GetWidth();
		unsigned int IsFullScreenModeEnabled();
	}
}

#endif	// EAE6320_USERSETTINGS_H
