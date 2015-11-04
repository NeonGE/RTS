/********************************************************************
	Created:	2014/08/16
	Filename:	MemoryDump.cpp
	Author:		Samuel Prince

	Purpose:	Clase de debug que genera un Dump de memoria a un
				archivo para el futuro análisis de errores
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de archivos necesarios para la compilación																	*/
/************************************************************************************************************************/
#pragma region Includes, Namespaces y librerías necesarias
#include "geCoreStd.h"
#include "MemoryDump.h"

#pragma comment(lib, "version.lib")

#pragma endregion

#pragma region Constantes, Estáticos y Globales
/************************************************************************************************************************/
/* Definición de Constantes, Estáticos y Globales																		*/
/************************************************************************************************************************/

/************************************************************************************************************************/
/* Prototipo de función basado en dbghelp.h																				*/
/************************************************************************************************************************/
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

geMemoryDump *geMemoryDump::g_pDumper = NULL;			//Inicializamos 


#pragma endregion

#pragma region Declaración de la Clase LogMgr
/************************************************************************************************************************/
/* Clase geMemoryDump.- Clase de debug encargada de hacer un Dump de memoria a archivo									*/
/************************************************************************************************************************/
geMemoryDump::geMemoryDump(bool headless)
{
	//Detectamos si existe más de un objeto geMemoryDump
	GEE_ASSERT(!g_pDumper);

	if( !g_pDumper )
	{//Si aún no existe un objeto Dumper en la aplicación
		::SetUnhandledExceptionFilter(Handler);		//Establecemos el manejador local para ser utilizado como función de filtro de excepciones
		g_pDumper = this;							//Establecemos este objeto de clase como el Dumper en uso
		m_bHeadless = headless;						//Establecemos la bandera que indica si debe mostrarse un diálogo preguntando al usuario por la creación del archivo de Dump
	}
}

/************************************************************************************************************************/
/* Función Handler() - Esta función se encarga de manejar las excepciones de memoria									*/
/************************************************************************************************************************/
long geMemoryDump::Handler(_EXCEPTION_POINTERS *pExceptionInfo)
{
	long retval = EXCEPTION_CONTINUE_SEARCH;

	if( !g_pDumper )
	{//Si no se ha establecido un dumper, simplemente regresa sin hacer nada
		return retval;	//TODO: Esto indica un error y debería ser revisado
	}

	//Mandamos a escribir el archivo del Dump
	return g_pDumper->WriteMemoryDump(pExceptionInfo);
}

/************************************************************************************************************************/
/* Función WriteMemoryDump() - Escribe el archivo con la información del Memory Dump									*/
/************************************************************************************************************************/
long geMemoryDump::WriteMemoryDump(_EXCEPTION_POINTERS *pExceptionInfo)
{
	//Creamos variables temporales
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HMODULE hDll = NULL;
	TCHAR szDbgHelpPath[MAX_PATH];
	
	//Guardamos la información de la excepción en los miembros de la clase
	m_pExceptionInfo = pExceptionInfo;
	
	//Obtenemos la información de tiempo y fecha actual
	time( &m_lTime );

	//Debemos buscar la librería dbghelp.dll correcta
	//Buscamos primero en la misma ruta del EXE ya que el de System32 puede ser una versión antigua (Win2k)
	if( GetModuleFileName(NULL, m_szAppPath, MAX_PATH) )
	{
		//Buscamos la última ocurrencia de la diagonal en la ruta de la aplicación
		TCHAR* pSlash = _tcsrchr( m_szAppPath, '\\' );
		if( pSlash )
		{//Si la encontramos, copiamos la ruta limpia sin el nombre del archivo
#ifdef _VS2005_
			_tcscpy_s( m_szAppBaseName, pSlash + 1);
#else
			_tcscpy( m_szAppBaseName, pSlash + 1 );
#endif
			*(pSlash+1) = 0;	//Terminamos el string
		}

		//Creamos la ruta de búsqueda de la librería usando la ruta de la aplicación
#ifdef _VS2005_
		_tcscpy_s( szDbgHelpPath, m_szAppPath );
		_tcscat_s( szDbgHelpPath, TEXT("DBGHELP.DLL") );
#else
		_tcscpy( szDbgHelpPath, m_szAppPath );
		_tcscat( szDbgHelpPath, TEXT("DBGHELP.DLL") );
#endif

		//Cargamos la librería DLL
		hDll = ::LoadLibrary(szDbgHelpPath);
	}


	if( hDll == NULL )
	{//Si la librería no se encontró
		//Intentamos una vez más utilizando las rutas preestablecidas por las variables de entorno
		hDll = ::LoadLibrary( TEXT("DBGHELP.DLL") );
	}

	LPCTSTR szResult = NULL;
	if( hDll )
	{//Si encontramos la librería
		//Obtenemos un puntero al procedimiento "MiniDumpWriteDump" de la DLL
		MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump");
		if( pMiniDumpWriteDump )
		{//Si obtuvimos el puntero al procedimiento correctamente
			//Creamos un array para contener la información de usuario
			TCHAR szScratch[USER_DATA_BUFFER_SIZE];

			//Establece el nombre final del archivo de Dump
			SetDumpFileName();

			//Le pregunta al usuario si quiere guardar un archivo Dump (esto solo si )
			_tcssprintf(szScratch, TEXT("Ocurrió un error inesperado:\n\n%s\nQuieres guardar un archivo de diagnóstico?\n\nNombre del Archivo: %s"), GetUserMessage(), m_szDumpPath);
			if( m_bHeadless || (::MessageBox( NULL, szScratch, NULL, MB_YESNO ) == IDYES) )
			{//Se accedió a la creación del archivo de Dump
				//Creamos el archivo vacío con los atributos necesarios
				HANDLE hFile = ::CreateFile(m_szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				if( hFile != INVALID_HANDLE_VALUE)
				{//El archivo se creó exitosamente
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;				//Esta estructura contenrdrá la información de la excepción

					ExInfo.ThreadId = ::GetCurrentThreadId();			//Obtenemos el identificador del thread de la aplicación
					ExInfo.ExceptionPointers = pExceptionInfo;			//Guardamos los punteros de excepción
					ExInfo.ClientPointers = NULL;						//Limpiamos la información de punteros a clientes

					//Escribimos el Dump
					int32 bOK = pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, GetUserStreamArray(), NULL);
					if( bOK )
					{//Si se escribió el archivo correctamente
						szResult = NULL;					//No enviaremos mensajes de error
						retval = EXCEPTION_EXECUTE_HANDLER;	//Establecemos el valor de regreso
					}
					else
					{//Falló al escribir el archivo
						//Establecemos un mensaje de error
						_tcssprintf(szScratch, TEXT("Fallo al escribir el archivo Dump a '%s' (error: %d)"), m_szDumpPath, GetLastError());
						szResult = szScratch;
					}

					//Cerramos el manejador al archivo
					::CloseHandle(hFile);
				}
				else
				{//Fallo al crear el archivo
					_tcssprintf( szScratch, TEXT("Fallo al crear el archivo Dump en '%s' (error: %d)"), m_szDumpPath, GetLastError() );
					szResult = szScratch;
				}
			}
		}
		else
		{//La versión de la librería es muy vieja
			szResult = TEXT("DBGHELP.DLL es una versión muy vieja");
		}
	}
	else
	{//No se encontró ninguna versión de la librería de debug
		szResult = TEXT("No se encontró - DBGHELP.DLL");
	}

	if( szResult && !m_bHeadless )
	{//Si hubo algún mensaje de error, y debe mostrarse
		//Lanzamos un díalogo con el mensaje
		::MessageBox(NULL, szResult, NULL, MB_OK);
	}

	//Terminamos el proceso de la aplicación
	TerminateProcess( GetCurrentProcess(), 0);

	/************************************************************************************************************************/
	/* NOTA: ExitThread funcionaría, y permite al Dumper matar un thread crasheado sin afectar al resto de la aplicación.	*/
	/* Ahora bien... la pregunta del día: Es esa una buena idea??? Respuesta: DEFINITIVAMENTE NO!!!!!!!!					*/
	/************************************************************************************************************************/
	//ExitThread(0);

	return retval;
}

/************************************************************************************************************************/
/* SetDumpFileName - Establece el nombre que llevará el archivo de Dump en esta instancia								*/
/************************************************************************************************************************/
void geMemoryDump::SetDumpFileName(void)
{
	//Creamos el nombre a partir del nombre de la aplicación, y la fecha y hora de ejecución
	_tcssprintf(m_szDumpPath, _T("%s%s.%ld.dmp"), m_szAppPath, m_szAppBaseName, m_lTime);
}

#pragma endregion
