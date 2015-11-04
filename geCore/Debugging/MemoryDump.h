/********************************************************************
	Created:	2014/08/16
	Filename:	MemoryDump.h
	Author:		Samuel Prince

	Purpose:	Clase de debug encargada de hacer un Dump de memoria
				compatible con el compilador utilizado para detectar
				fallas del programa e informarlas a desarrollo
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de constantes utilizadas por las clases de este archivo													*/
/************************************************************************************************************************/
#define USER_DATA_BUFFER_SIZE 4096												//Tamaño del tamaño del buffer de datos de usuario

/************************************************************************************************************************/
/* Declaración de la clase geMemoryDump																					*/
/************************************************************************************************************************/
class geMemoryDump
{
	/************************************************************************************************************************/
	/* Declaración de variables protegidas																					*/
	/************************************************************************************************************************/
protected:
	static geMemoryDump* g_pDumper;												//Usaremos esta variable estática para generar acceso como singleton
	static long WINAPI Handler(struct _EXCEPTION_POINTERS *pExceptionInfo);		//Función de manejo de escepción de memoria 

	bool m_bHeadless;															//Esta bandera indica si debe mostrarse un mensaje o solo hacer el dump sin preguntar
	time_t m_lTime;																//Información de tiempo en el que se genera este Dump
	_EXCEPTION_POINTERS *m_pExceptionInfo;										//Información de punteros de excepción

	TCHAR m_szDumpPath[MAX_PATH];												//Ruta donde se guardará el archivo de dump
	TCHAR m_szAppPath[MAX_PATH];												//Ruta de la aplicación
	TCHAR m_szAppBaseName[MAX_PATH];											//Nombre de base de la aplicación
	
	/************************************************************************************************************************/
	/* Declaración de funciones protegidas																					*/
	/************************************************************************************************************************/
protected:
	long WriteMemoryDump(_EXCEPTION_POINTERS *pExceptionInfo);					//Escribe el archivo de Dump de la aplicación

	virtual void SetDumpFileName(void);											//Establece el nombre del archivo de Dump
	virtual MINIDUMP_USER_STREAM_INFORMATION* GetUserStreamArray()				//Función virtual que regresa un stream de datos del ususario (por el momento no se utiliza)
	{
		return NULL;
	}
	virtual const TCHAR* GetUserMessage(){return TEXT("");}						//Obtiene el mensaje del usuario (la función de windows puede preguntar por comentarios del usaurio)

	/************************************************************************************************************************/
	/* Declaración de funciones públicas																					*/
	/************************************************************************************************************************/
public:
	geMemoryDump(bool headless);												//Constructor (el parámetro indica si en el caso de una escepción debe preguntar al usuario para hacer el dump)
};

/************************************************************************************************************************/
/* Declaración de la clase geModuleEntry usada para dar información de un módulo en particular							*/
/************************************************************************************************************************/
class geModuleEntry
{
	/************************************************************************************************************************/
	/* Declaración de variables protegidas																					*/
	/************************************************************************************************************************/
protected:
	MODULEENTRY32 m_me32;														//Información del módulo
	TCHAR* m_pFileVersionInfo;													//Puntero a la información de la versión del archivo
	TCHAR* m_pProductVersion;													//Puntero a la información de la versión del producto

	/************************************************************************************************************************/
	/* Declaración de funciones protegidas																					*/
	/************************************************************************************************************************/
protected:
	void ReadFileVersionInfo();													//Función que lee la información de versión del archivo
	void FindProductVersion();													//Función que lee la información de la versión del producto

	/************************************************************************************************************************/
	/* Declaración de funciones públicas																					*/
	/************************************************************************************************************************/
public:
	geModuleEntry(const MODULEENTRY32 &me);										//Constructor de la clase
	virtual ~geModuleEntry();													//Destructor virtual

	/************************************************************************************************************************/
	/* Accesores constantes																									*/
	/************************************************************************************************************************/
	const MODULEENTRY32* Get() const {return &m_me32;}
	const TCHAR* GetProductVersion() const {return m_pProductVersion;}
};