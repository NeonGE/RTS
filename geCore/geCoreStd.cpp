/********************************************************************
	Created:	2014/02/01
	Filename:	geCoreStd.cpp
	Author:		Samuel Prince

	Purpose:	geCoreStd.pch será la cabecera precompilada
				Aquí definimos algunas variables e implementamos
				funciones de alojamiento de memoria
*********************************************************************/

#include "geCoreStd.h"

/************************************************************************************************************************/
/* Descomentar esta línea para hacer un dump de calla llamada a new a la ventana de salida								*/
/************************************************************************************************************************/
#define MEM_LOG
#if defined(_DEBUG) && defined(MEM_LOG)

void* operator new(size_t size, int memType, const char* filename, int lineNum)
{
	//Tenemos que hacer el dump al estilo old-school ya que no se nos permite alojar memoria dinámicamente en este lugar
	char buffer[2048];
	SIZE_T index = 0;

	index += strlen(_ultoa((unsigned long)size, buffer, 10));
	strcpy(buffer+index, " -> ");
	index += 4;
	strcpy(buffer+index, filename);
	index += strlen(filename);
	buffer[index] = ':';
	++index;
	index += strlen(_itoa(lineNum, buffer+index, 10));
	buffer[index] = '\n';
	++index;
	buffer[index] = '\0';
	++index;
	OutputDebugStringA(buffer);

	//Alojamos la memoria
	memType = _NORMAL_BLOCK;	//Hacemos esto para eliminar el warning que indica que no estamos usando la variable, pero en Debug, queremos utilizar el bloque normal
	return _malloc_dbg(size, memType, filename, lineNum);
}

void operator delete(void* pMemory)
{
	//Liberamos la memoria
	_free_dbg(pMemory, _NORMAL_BLOCK);
}

void* operator new[](size_t size, int memType, const char* filename, int lineNum)
{
	//Tenemos que hacer el dump al estilo old-school ya que no se nos permite alojar memoria dinámicamente en este lugar
	char buffer[2048];
	SIZE_T index = 0;
	index += strlen(ultoa((unsigned long)size, buffer, 10));
	strcpy(buffer+index, " -> ");
	index += 4;
	strcpy(buffer+index, filename);
	index += strlen(filename);
	buffer[index] = ':';
	++index;
	index += strlen(itoa(lineNum, buffer+index, 10));
	buffer[index] = '\n';
	++index;
	buffer[index] = '\0';
	++index;
	OutputDebugStringA(buffer);

	//Alojamos la memoria
	memType = _NORMAL_BLOCK;	//Hacemos esto para eliminar el warning que indica que no estamos usando la variable, pero en Debug, queremos utilizar el bloque normal
	return _malloc_dbg(size, memType, filename, lineNum);
}

void operator delete[](void* pMemory)
{
	//Liberamos la memoria
	_free_dbg(pMemory, _NORMAL_BLOCK);
}

#endif