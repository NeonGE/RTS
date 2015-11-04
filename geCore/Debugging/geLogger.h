/********************************************************************
	Created:	2014/01/31
	Filename:	geLogger.h
	Author:		Samuel Prince

	Purpose:	Definición de clases y objetos relacionados con
				el manejo de Loggeo y notificación de Mensajes.
				En estas clases encontraremos lo necesario para
				hacer el log a un archivo o a la consola de debug
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Definimos según la configuración del sistema, el tipo de string que utilizaremos										*/
/************************************************************************************************************************/
#ifdef DEBUG

#endif // DEBUG


/************************************************************************************************************************/
/* Definición de banderas de visualización (donde se mostrarán los logs)												*/
/************************************************************************************************************************/
const uint8 LOGFLAG_WRITE_TO_LOG_FILE =		1 << 0;		//Si esta bandera es establecida, el logger escribirá a un archivo
const uint8 LOGFLAG_WRITE_TO_DEBUGGER =		1 << 1;		//Si esta bandera es establecida, el logger escribirá a la consola de debug de Visual Studio

/************************************************************************************************************************/
/* Esta es la interface pública de geLogger. Debes llamar a Init() antes de que cualquier loggeo pueda llevarse a cabo	*/
/* y Destroy() cuando hayas terminado para hacer limpieza. Llama SetDisplayFlags() para establecer las banderas de		*/
/* visualización para un tag de logging en particular. Por default, todas están desactivadas. Aunque se puede,	no		*/
/* debería llamarse a Log directamente. Usa el macro GEE_LOG() en su lugar ya que puede ser eliminado por el			*/
/* compilador en construcciones de Release.																				*/
/************************************************************************************************************************/
namespace geLogger
{
	class ErrorMessenger
	{//NOTA.- Esta clase es utilizada por los macros de debug y no debería ser accedida externamente
		//Definición de variables miembro
	private:
		bool m_enabled;		//Indica que el objeto ya fue inicializado
		//Definición de Funciones
	public:
		ErrorMessenger(void);	//Constructor
		void Show(const geString& errorMessage, bool isFatal, const ANSICHAR* funcName, const ANSICHAR* sourceFile, uint32 lineNum);	//Muestra el error al usuario
	};

	//Funciones de contrucción / destrucción
	void Init(const TCHAR* loggingConfigFilename);	//Esta función DEBE ser llamada al inicio del programa antes de utilizar el logger
	void Destroy(void);								//Esta función debe llamarse cuando quiera destruirse el objeto, normalmente al salir del programa

	//Funciones de Loggeo
	void Log(const geString& tag, const geString& message, const ANSICHAR* funcName, const ANSICHAR* sourceFile, uint32 lineNum);	//Envía un mensaje al log
	void SetDisplayFlags(const geString& tag, uint8 flags);	//Establece las banderas de visualización para un TAG específico
}

/************************************************************************************************************************/
/* Macros de Debug																										*/
/************************************************************************************************************************/

/************************************************************************************************************************/
/* Los errores fatales obviamente son fatales y siempre se le presentan	al usuario										*/
/************************************************************************************************************************/
#define GEE_FATAL(str) \
	MULTI_LINE_MACRO_BEGIN \
		static geLogger::ErrorMessenger* pErrorMessenger = GEE_NEW geLogger::ErrorMessenger; \
		geString s((str)); \
		pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
	MULTI_LINE_MACRO_END

#if defined(_DEBUG)	//Si estamos construyendo en modo de DEBUG
/************************************************************************************************************************/
/* Los errores son malos y potencialmente fatales. Estos son presentados con un diálogo con opciones de Abort, Retry e	*/
/* Ignore. Abort hará un break en el debugger, Retry continuará con la ejecución e Ignore continuará la ejecución e		*/
/* ignorará cualquier subsecuente llamada a ese específico error. Este Macro es completamente ignorado en modo Release	*/
/************************************************************************************************************************/
#define GEE_ERROR(str) \
	MULTI_LINE_MACRO_BEGIN \
		static geLogger::ErrorMessenger* pErrorMessenger = GEE_NEW geLogger::ErrorMessenger; \
		geString s((str)); \
		pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
	MULTI_LINE_MACRO_END

/************************************************************************************************************************/
/* Los Warnings son recuperables. Solo son logs con el tag "WARNING" que muestran la información de la llamada.			*/
/* Las banderas son inicialmente establecidas a WARNINGFLAG_DEFAULT (definido en geLogger.cpp), pero pueden ser			*/
/* sobreescritas normalmente.																							*/
/************************************************************************************************************************/
#define GEE_WARNING(str) \
	MULTI_LINE_MACRO_BEGIN \
		geString s((str)); \
		geLogger::Log(TEXT("WARNING"), s, __FUNCTION__, __FILE__, __LINE__); \
	MULTI_LINE_MACRO_END

/************************************************************************************************************************/
/* Este es solo un macro conveniente para hacer logging si no sientes que quieras lidiar con Tags. Este llama Log()	con	*/
/* un Tag "INFO". Las banderas son inicialmente establecidas a LOGFLAG_DEFAULT (definido en geLogger.cpp), pero pueden	*/
/* ser sobreescritas normalmente																						*/
/************************************************************************************************************************/
#define GEE_INFO(str) \
	MULTI_LINE_MACRO_BEGIN \
		geString s((str)); \
		geLogger::Log(TEXT("INFO"), s, NULL, NULL, 0); \
	MULTI_LINE_MACRO_END

/************************************************************************************************************************/
/* Este macro es utilizado para hacer logging y debe ser el método preferido de "printf debugging". Puedes usar			*/
/* cualquier Tag que quieras, solo asegurate de activarlos en algún lado en tu inicialización							*/
/************************************************************************************************************************/
#define GEE_LOG(tag, str) \
	MULTI_LINE_MACRO_BEGIN \
		geString s((str)); \
		geLogger::Log(tag, s, NULL, NULL, 0); \
	MULTI_LINE_MACRO_END

/************************************************************************************************************************/
/* Este macro reemplaza ASSERT()																						*/
/************************************************************************************************************************/
#define GEE_ASSERT(expr) \
	MULTI_LINE_MACRO_BEGIN \
		if (!(expr)) \
		{ \
			static geLogger::ErrorMessenger* pErrorMessenger = GEE_NEW geLogger::ErrorMessenger; \
			pErrorMessenger->Show(TEXT(#expr), false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	MULTI_LINE_MACRO_END

#else	//Estamos construyendo en modo release

/************************************************************************************************************************/
/* Estas son las definiciones para el modo Release de los macros anteriores. Todos están definidos de tal manera que el	*/
/* compilador los ignorará																								*/
/************************************************************************************************************************/
#define GEE_ERROR(str)		MULTI_LINE_MACRO_BEGIN (void)sizeof(str);MULTI_LINE_MACRO_END
#define GEE_WARNING(str)	MULTI_LINE_MACRO_BEGIN (void)sizeof(str);MULTI_LINE_MACRO_END
#define GEE_INFO(str)		MULTI_LINE_MACRO_BEGIN (void)sizeof(str);MULTI_LINE_MACRO_END
#define GEE_LOG(tag, str)	MULTI_LINE_MACRO_BEGIN (void)sizeof(tag); (void)sizeof(str);MULTI_LINE_MACRO_END
#define GEE_ASSERT(expr)	MULTI_LINE_MACRO_BEGIN (void)sizeof(expr);MULTI_LINE_MACRO_END

#endif  //#if defined(_DEBUG)