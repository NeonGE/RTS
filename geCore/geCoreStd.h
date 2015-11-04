/********************************************************************
	Created:	2014/02/01
	Filename:	gecorestd.h
	Author:		Samuel Prince

	Purpose:	Cabecera precompilada de la librería geCore
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Constantes de configuración en tiempo de compilación                                               					*/
/************************************************************************************************************************/
#define _VS2012		1			//Esto solo determina que estamos utilizando VS2012 para compilar

/************************************************************************************************************************/
/* Archivos de Cabeceras de Windows                                     												*/
/************************************************************************************************************************/
#include "Platforms/Windows/MinWindows.h"

/************************************************************************************************************************/
/* Características de Debugging de la librería RunTime de C             												*/
/************************************************************************************************************************/
#include <crtdbg.h>

/************************************************************************************************************************/
/* Archivos de Cabeceras de RunTime de C	                               												*/
/************************************************************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

#include <Tlhelp32.h>
#include <dbghelp.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

#include <string.h>
#include <stdio.h>

/************************************************************************************************************************/
/* Declaración de la clase GEE_noncopyable, la cual se utilizará como clase base para objetos que no deben ser			*/
/* duplicados, por lo que solo serán pasadas referencias																*/
/************************************************************************************************************************/
class GEE_noncopyable
{
private:
	GEE_noncopyable(const GEE_noncopyable& x);
	GEE_noncopyable& operator=(const GEE_noncopyable& x);
public:
	GEE_noncopyable(){};	//Constructor
};  

/************************************************************************************************************************/
/* Macros para alojamiento de memoria en versiones DEBUG y RELEASE      												*/
/************************************************************************************************************************/
#if defined(_DEBUG)
#	define GEE_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#	define GEE_DELETE delete
#	define GEE_DELETE_ARRAY delete []
#else
#	define GEE_NEW new
#	define GEE_DELETE delete
#	define GEE_DELETE_ARRAY delete []
#endif

/************************************************************************************************************************/
/* Includes de librerías externas                                                                  						*/
/************************************************************************************************************************/
#include <tinyxml.h>

/************************************************************************************************************************/
/* Forward Declaration de los objetos de la librería																	*/
/************************************************************************************************************************/
class	geColor;
class	geFloat16;
class	geFloat32;
class	gePoint;
class	geRect;
struct	geVector2D;
class	geVector;
class	geVector4;

/************************************************************************************************************************/
/* Inclusión de archivos requeridos para el uso de viewback																*/
/************************************************************************************************************************/
#include "viewback.h"
#include "viewback_shared.h"
#include "viewback_internal.h"
#include "viewback_util.h"

/************************************************************************************************************************/
/* Inclusión de Pthreads para creación de procesos en paralelo															*/
/************************************************************************************************************************/
//#include <pthread.h>

/************************************************************************************************************************/
/* Requerimientos para el uso de Fast Delegate                          												*/
/************************************************************************************************************************/
#include "../Externals/FastDelegate/FastDelegate.h"
using fastdelegate::MakeDelegate;
#pragma warning( disable : 4996 )		//'function' declared deprecated - elimina todos los warnings 2005...

//Includes de utilidades (la parte más básica pero tambien la más utilizada)
#include "Utilities/MiscDefines.h"		//Definición de constantes miscelaneas
#include "Utilities/PlatformTypes.h"	//Definimos los tipos para la plataforma actual
#include "Utilities/PlatformMath.h"		//Definimos las operaciones matemáticas optimizadas para esta plataforma

//Clases de Debug
#include "Debugging/geLogger.h"			//Este debe ser el primero de los includes de GEE ya que define GEE_ASSERT()
#include "Debugging/MemoryDump.h"		//Clase utilitaria para generar dumps de memoria

//Clases de objetos contenedores
//#include "Containers/Array.h"

//Más utilidades dependientes de otros tipos
#include "Utilities/BasicTypes.h"		//Incluimos las clases de tipos básicos
#include "Utilities/String.h"			//Incluimos las clases de ayuda para manejo de Strings

//Clases matemáticas
#include "Math/geFloat32.h"				//Definimos un tipo flotante de 32 bits
#include "Math/geFloat16.h"				//Definimos un tipo flotante de 16 bits
#include "Math/geColor.h"				//Color class with 32 bits components
#include "Math/geFloat16Color.h"		//Color class with 16 bits components
#include "Math/geVector2D.h"			//Vectores 2D
//#include "Math/geVector3D.h"			//Vectores 3D
//#include "Math/geVector4D.h"			//Vectores 3D

/************************************************************************************************************************/
/* Estructura de mensajes de la aplicación                              												*/
/************************************************************************************************************************/
struct AppMsg
{
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
};

/************************************************************************************************************************/
/* Definición de algunas constantes útiles																				*/
/************************************************************************************************************************/
extern const int MEGABYTE;			//Número de Bytes en un Megabyte
extern const float SIXTY_HERTZ;		//Tiempo de un ciclo a una frecuencia de 60 Hertz

/************************************************************************************************************************/
/* Declaración de macros útiles                                         												*/
/************************************************************************************************************************/
#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(x) if(x) GEE_DELETE x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
	#define SAFE_DELETE_ARRAY(x) if (x) GEE_DELETE_ARRAY x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
	#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#ifdef UNICODE
	#define _tcssprintf wsprintf
	#define tcsplitpath _wsplitpath
#else
	#define _tcssprintf sprintf
	#define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

//#include "Game/geGameCode.h"
//extern INT WINAPI geGameCode(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow);
