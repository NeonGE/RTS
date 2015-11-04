/********************************************************************
	Created:	2014/07/06
	Filename:	Initialization.h
	Author:		Samuel Prince

	Purpose:	Funciones de inicialización y detección de hardware
				Tambien nos encargamos de algunas funciones para la
				obtención de opciones a través de archivos de
				configuración
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de funciones de chequeo																					*/
/************************************************************************************************************************/
extern bool			CheckStorage(const uint64 diskSpaceNeeded);
extern uint32		ReadCPUSpeed();
extern bool			CheckMemory(const uint64 physicalRAMNeeded, const uint64 virtualRAMNeeded);

/************************************************************************************************************************/
/* Declaración de funciones de uso para la aplicación																	*/
/************************************************************************************************************************/
extern bool			IsOnlyInstance(LPCTSTR gameTitle);
extern const TCHAR*	GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory);
extern bool			CheckForJoystick(HWND hWnd);
