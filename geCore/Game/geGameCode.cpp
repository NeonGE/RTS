/********************************************************************
	created:	2013/09/06
	created:	6:9:2013   18:23
	filename: 	D:\Trabajo\geEngine\geCore\Game\geGameCode.cpp
	file path:	D:\Trabajo\geEngine\geCore\Game
	file base:	geGameCode
	file ext:	cpp
	author:		Samuel Prince
	
	purpose:	The entry code for the Game Engine
*********************************************************************/

#include "../geCoreStd.h"
#include "geGameCode.h"

#pragma comment(lib, "tinyxml.lib")

INT WINAPI geGameCode(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	geLogger::Init("Logging.xml");

	GEE_LOG("geEngine", "***** Logger Initiated *****");

	return 0;
}