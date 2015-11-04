/********************************************************************
	Created:	2014/02/01
	Filename:	String.h
	Author:		Samuel Prince

	Purpose:	Definición y Clases para manejo de Strings
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Definición de Constantes                                             												*/
/************************************************************************************************************************/
#define MAX_DIGITS_IN_INT 12					//Numero máximo de dígitos en un int (-2147483647 = 11 dígitos, +1 por el '\0')
typedef std::vector<geString> StringVec;		//Tipo definido para listas de strings

/************************************************************************************************************************/
/* Funciones utilitarias                                                												*/
/************************************************************************************************************************/
extern void RemoveFirstLine(geString &src, geString &result);						//Elimina caracteres hasta que encuentra el primer corte de línea ('\n')
extern void TrimLeft(geString &s);													//Elimina los espacios blancos al inicio del string
extern int CountLines(const geString &s);											//Cuenta el número de líneas en un bloque de texto
extern bool WildcardMatch(const TCHAR *pat, const TCHAR *str);						//Hace el clásico matching de patrones * & ? en un nombre de archivo: Esto case sensitive!
extern void StringToWideString(const std::string& source, std::wstring& outDest);	//Convierte un string regular a un wide string
extern void ReplaceAll(geString &str, const geString& from, const geString& to);	//Reemplaza todas las instancias de un string con otro

/************************************************************************************************************************/
/* Funciones útiles durante el manejo de información de archivos														*/
/************************************************************************************************************************/
extern void CheckFolderSlashes(geString &inString, const geString &charDiagonal);				//Esta función revisa las diagonales en una ruta de archivo y la regresa con la forma pedida

/************************************************************************************************************************/
/* Funciones de conversión de tipos de string                           												*/
/************************************************************************************************************************/
extern HRESULT AnsiToWideCch( UNICHAR* dest, const ANSICHAR* src, int32 charCount);	//ANSI a WIDE
extern HRESULT WideToAnsiCch( ANSICHAR* dest, const UNICHAR* src, int32 charCount);	//WIDE a ANSI
extern HRESULT GenericToAnsiCch( ANSICHAR* dest, const TCHAR* src, int32 charCount);	//Genérico a ANSI
extern HRESULT GenericToWideCch( UNICHAR* dest, const TCHAR* src, int32 charCount);	//Genérico a WIDE
extern HRESULT AnsiToGenericCch( TCHAR* dest, const ANSICHAR* src, int32 charCount);	//ANSI a Genérico
extern HRESULT WideToGenericCch( TCHAR* dest, const UNICHAR* src, int32 charCount);	//WIDE a Genérico

/************************************************************************************************************************/
/* Funciones de conversión de Número a String                           												*/
/************************************************************************************************************************/
extern geString ToStr(int32 num, int32 base = 10);			//Conversión de Int a String
extern geString ToStr(uint32 num, int32 base = 10);			//Conversión de Unsigned Int a String
extern geString ToStr(unsigned long num, int32 base = 10);	//Conversión de Unsigned Long a String
extern geString ToStr(float num);							//Conversión de Float a String
extern geString ToStr(double num);							//Conversión de Double a String
extern geString ToStr(bool val);							//Conversión de Boolean a String

/************************************************************************************************************************/
/* Funciones de conversión de WIDE a String y de String a WIDE          												*/
/************************************************************************************************************************/
extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);

/************************************************************************************************************************/
/* Funciones de conversión de WIDE y ANSI a Genérico          															*/
/************************************************************************************************************************/
extern geString s2gs(const std::string &s);
extern geString ws2gs(const std::wstring &s);

/************************************************************************************************************************/
/* Divide un String por el delimitador y lo convierte en un vector de string. Por ejemplo, si tenemos el string:		*/
/* std::string test("uno,dos,tres");																					*/
/* Puedes llamar a Split() así:																							*/
/* Split(test, outVec, ',');																							*/
/* outVec contendrá al salir los siguientes valores:																	*/
/* "uno", "dos", "tres"																									*/
/* Parametros:																											*/
/*     str: String que será dividido																					*/
/*     vec: lista que contendrá los valores de salida																	*/
/*     delimeter: caracter que se utilizará como delimitador															*/
/************************************************************************************************************************/
void Split(const geString& str, StringVec& vec, TCHAR delimiter);

#pragma warning(push)				//Guardamos la configuración actual del compilador
#pragma warning(disable : 4311)		//Deshabilitamos la advertencia 4311 (intentar poner un puntero en un int de 32 bits)

/************************************************************************************************************************/
/* Definición de la clase HashedString, la cual retiene el string (ANSI) inicial, además de un hash para facil			*/
/* referencia																											*/
/************************************************************************************************************************/
class HashedString
{
public:
	//Constructor
	explicit HashedString( const TCHAR *pIdentString ) : m_ident( hash_name(pIdentString) ), m_identStr( pIdentString )
	{
	}

	//Regresa el valor Hash de este objeto
	uint32 getHashValue( void ) const
	{
		return reinterpret_cast<uint32>( m_ident );
	}

	//Regresa el string original
	const geString & getStr() const
	{
		return m_identStr;
	}

	//Regresa el Hash de el String
	static void * hash_name( const TCHAR *pIdentStr );

	//Declaración de operador Menor-Que
	bool operator< ( const HashedString &o ) const
	{
		bool r = ( getHashValue() < o.getHashValue() );
		return r;
	}

	//Declaración de operador de comparación
	bool operator== ( const HashedString &o ) const
	{
		bool r = ( getHashValue() == o.getHashValue() );
		return r;
	}

private:
	/************************************************************************************************************************/
	/* NOTA: m_ident es almacenado como un void* no como un int, para que en el debugger se muestre con valor hexadecimal	*/
	/* en lugar de valores enteros. Esto es un poco más representativo de lo que estamos haciendo aquí y hace más facil	el	*/
	/* permitir que el código externo asigne tipos de eventos como se desea													*/
	/************************************************************************************************************************/
	void*	 m_ident;
	geString m_identStr;
};

#pragma warning(pop)