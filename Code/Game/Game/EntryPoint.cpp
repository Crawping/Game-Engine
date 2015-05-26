/*
	Unlike a standard C/C++ program,
	WinMain() is the function where Windows programs start execution
*/

// Header Files
//=============

#include "precompiled.h"
#include "WindowsProgram.h"
#include "Game.h"

// Entry Point
//============

int WINAPI WinMain(
	HINSTANCE i_thisInstanceOfTheProgram,
	HINSTANCE,
	char* i_commandLineArguments,
	int i_initialWindowDisplayState )
{
	
	Game GameInstance;
	GameInstance.InitializeGame(i_thisInstanceOfTheProgram, i_initialWindowDisplayState);
	GameInstance.UpdateGame();
	return GameInstance.ShutdownGame();

}
