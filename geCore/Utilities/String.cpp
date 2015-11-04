/********************************************************************
	Created:	2014/02/01
	Filename:	String.cpp
	Author:		Samuel Prince

	Purpose:	Implementación de las funciones de String y 
				HashedString
*********************************************************************/

/************************************************************************************************************************/
/* Inclusión de archivos necesarios                                     												*/
/************************************************************************************************************************/
#include "geCoreStd.h"
#include "String.h"

/************************************************************************************************************************/
/* Elimina todos los espacios blancos al inicio del string																*/
/*																														*/
/* Parametros:																											*/
/*     s: String al que se le hará el tratamiento de eliminación de espacios											*/
/************************************************************************************************************************/
void TrimLeft(geString &s)
{
	//Primero hacemos una búsqueda por el primer caracter que no sea un espacio en blanco
	SIZE_T i = 0;
	SIZE_T len = s.length();

	while( i <  len )
	{
		TCHAR ch = s[i];
		int32 white = 
		#ifdef UNICODE		//Si estamos utilizando UNICODE
			iswspace( ch );
		#else				//Estamos utilizando Multi-Byte
			isspace( ch );
		#endif

		//Si este caracter no es un espacio blanco
		if( !white )
		{//Detenemos la búsqueda
			break;
		}

		//Incrementamos la posición para buscar en el siguiente caracter
		++i;
	}

	//Si encontramos un caracter que no sea un espacio en blanco
	if( i < len )
	{//Recortamos el string
		s = s.substr(i);
	}
}

/************************************************************************************************************************/
/* La siguiente función fue encontrada en http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html donde fue		*/
/* atribuida al C/C++ Users Journal, escrita por Mike Cornelison. Es un tanto horrenda, pero es RAPIDA. Usa esto como	*/
/* un ejercicio de no reinventar la rueda, incluso si ves gotos															*/
/*																														*/
/* Parametros:																											*/
/*     pat: Patrón que será buscado en el string																		*/
/*     str: String en el que se buscará el patrón																		*/
/************************************************************************************************************************/
bool WildcardMatch(const TCHAR *pat, const TCHAR *str)
{
	int32 i, star;

new_segment:
	star = 0;
	if(*pat == '*')
	{
		star = 1;
		do
		{
			pat++;
		}while(*pat == '*');
	}

test_match:
	for(i=0; pat[i] && (pat[i] != '*'); ++i)
	{
		if(str[i] != pat[i])
		{
			if(!str[i])return false;
			if((pat[i] == '?') && (str[i] != '.'))continue;
			if(!star)return false;
			str++;
			goto test_match;
		}
	}
	if(pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}
	if(!str[i])return true;
	if(i && pat[i-1] == '*')return true;
	if(!star)return false;
	str++;
	goto test_match;
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings CHAR a strings WCHAR									*/
/*																														*/
/* Parametros:																											*/
/*     wstrDestination: UNICHAR string destino																				*/
/*     strSource: CHAR string original																					*/
/*     cchDestChar: Es el tamaño en TCHARs de wstrDestination. Cuidado de no pasar sizeof(wstrDestination)				*/
/************************************************************************************************************************/
HRESULT AnsiToWideCch(UNICHAR* wstrDestination, const ANSICHAR* strSource, int32 cchDestChar)
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

	int32 nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1, wstrDestination, cchDestChar);
	wstrDestination[cchDestChar-1] = L'\0';

	if( nResult == 0 )
	{
		return E_FAIL;
	}
	return S_OK;
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings WCHAR a strings CHAR									*/
/*																														*/
/* Parametros:																											*/
/*     strDestination: ANSICHAR string destino																				*/
/*     wstrSource: WCHAR string original																				*/
/*     cchDestChar: Es el tamaño en TCHARs de strDestination															*/
/************************************************************************************************************************/
HRESULT WideToAnsiCch(ANSICHAR* strDestination, const UNICHAR* wstrSource, int32 cchDestChar)
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

	int32 nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, cchDestChar*sizeof(ANSICHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
	{
		return E_FAIL;
	}
	return S_OK;
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings TCHAR a strings CHAR									*/
/*																														*/
/* Parametros:																											*/
/*     strDestination: ANSICHAR string destino																				*/
/*     tstrSource: TCHAR string original																				*/
/*     cchDestChar: Es el tamaño en TCHARs de strDestination															*/
/************************************************************************************************************************/
HRESULT GenericToAnsiCch( ANSICHAR* strDestination, const TCHAR* tstrSource, int32 cchDestChar )
{
	if( strDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

#ifdef _UNICODE
	return WideToAnsiCch( strDestination, tstrSource, cchDestChar );
#else
	strncpy( strDestination, tstrSource, cchDestChar );
	strDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif   
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings TCHAR a strings WCHAR									*/
/*																														*/
/* Parametros:																											*/
/*     wstrDestination: UNICHAR string destino																			*/
/*     tstrSource: TCHAR string original																				*/
/*     cchDestChar: Es el tamaño en TCHARs de wstrDestination. Cuidado de no pasar sizeof(wstrDestination)				*/
/************************************************************************************************************************/
HRESULT GenericToWideCch( UNICHAR* wstrDestination, const TCHAR* tstrSource, int32 cchDestChar )
{
	if( wstrDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

#ifdef _UNICODE
	wcsncpy( wstrDestination, tstrSource, cchDestChar );
	wstrDestination[cchDestChar-1] = L'\0';
	return S_OK;
#else
	return AnsiToWideCch( wstrDestination, tstrSource, cchDestChar );
#endif
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings CHAR a strings TCHAR									*/
/*																														*/
/* Parametros:																											*/
/*     tstrDestination: TCHAR string destino																			*/
/*     strSource: ANSICHAR string original																					*/
/*     cchDestChar: Es el tamaño en TCHARs de tstrDestination. Cuidado de no pasar sizeof(tstrDestination)				*/
/*                  en builds UNICODE																					*/
/************************************************************************************************************************/
HRESULT AnsiToGenericCch( TCHAR* tstrDestination, const ANSICHAR* strSource, int32 cchDestChar )
{
	if( tstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

#ifdef UNICODE
	return AnsiToWideCch( tstrDestination, strSource, cchDestChar );
#else
	strncpy( tstrDestination, strSource, cchDestChar );
	tstrDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif    
}

/************************************************************************************************************************/
/* Esta es una utilidad para hacer conversión UNICODE de strings WCHAR a strings TCHAR									*/
/*																														*/
/* Parametros:																											*/
/*     tstrDestination: TCHAR string destino																			*/
/*     wstrSource: UNICHAR string original																				*/
/*     cchDestChar: Es el tamaño en TCHARs de tstrDestination. Cuidado de no pasar sizeof(tstrDestination)				*/
/*                  en builds UNICODE																					*/
/************************************************************************************************************************/
HRESULT WideToGenericCch( TCHAR* tstrDestination, const UNICHAR* wstrSource, int32 cchDestChar )
{
	if( tstrDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
	{
		return E_INVALIDARG;
	}

#ifdef UNICODE
	wcsncpy( tstrDestination, wstrSource, cchDestChar );
	tstrDestination[cchDestChar-1] = L'\0';    
	return S_OK;
#else
	return WideToAnsiCch( tstrDestination, wstrSource, cchDestChar );
#endif
}

/************************************************************************************************************************/
/* Funciónes para hacer conversión de y hacia widestring y string         												*/
/************************************************************************************************************************/
std::string ws2s(const std::wstring& s)
{
    int slength = (int)s.length() + 1;
    int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0)-1; 
    std::string r(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0); 
    return r;
}

std::wstring s2ws(const std::string &s)
{
    int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0)-1;
    std::wstring r(len, '\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
    return r;
}

/************************************************************************************************************************/
/* Funciones para convertir de ANSI o WIDE a Genérico                   												*/
/************************************************************************************************************************/
geString s2gs(const std::string &s)
{
	geString r;
	#ifdef UNICODE
		r = s2ws(s);
	#else
		r = s;
	#endif
	return r;
}

geString ws2gs(const std::wstring &s)
{
	geString r;
	#ifdef UNICODE
		r = s;
	#else
		r = ws2s(s);
	#endif
	return r;
}

/************************************************************************************************************************/
/* Reemplaza todas las instancias de un string con otro																	*/
/* Parametros:																											*/
/*     str - String original																							*/
/*     from - String a ser reemplazado																					*/
/*     to - String con el que se reemplazará																			*/
/************************************************************************************************************************/
void ReplaceAll(geString &str, const geString& from, const geString& to)
{
	SIZE_T start_pos = 0;
	while( (start_pos = str.find(from, start_pos)) != geString::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();	//Maneja el caso que sucede cuando 'to' es un substring de 'from'
	}
}

/************************************************************************************************************************/
/* Funciones útiles durante el manejo de información de archivos														*/
/************************************************************************************************************************/

/************************************************************************************************************************/
/* Esta función revisa las diagonales en una ruta de archivo y la regresa con la forma pedida de diagonales				*/
/************************************************************************************************************************/
void CheckFolderSlashes(geString &inString, const geString &charDiagonal)
{//Sin importar que tipo de diagonales vengan en el string enviado, las convertimos todas al estilo pedido
	ReplaceAll(inString, TEXT("\\"), charDiagonal);
	ReplaceAll(inString, TEXT("/"),  charDiagonal);
}

/************************************************************************************************************************/
/* Funciones para convertir de número a String                          												*/
/************************************************************************************************************************/
geString ToStr(int32 num, int32 base)
{
	TCHAR str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT*sizeof(TCHAR));
	_itot_s(num, str, MAX_DIGITS_IN_INT, base);
	return (geString(str));
}

geString ToStr(uint32 num, int32 base)
{
	TCHAR str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT*sizeof(TCHAR));
	_ultot_s((unsigned long)num, str, MAX_DIGITS_IN_INT, base);
	return (geString(str));
}

geString ToStr(unsigned long num, int32 base)
{
	TCHAR str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT*sizeof(TCHAR));
	_ultot_s(num, str, MAX_DIGITS_IN_INT, base);
	return (geString(str));
}

geString ToStr(float num)
{
	TCHAR str[64];  //Estoy seguro que esto es demaciado pero no importa
	memset(str, 0, sizeof(str));
	_stprintf_p(str, 64, TEXT("%f"), num);
	return (geString(str));
}

geString ToStr(double num)
{
	TCHAR str[64];  //Estoy seguro que esto es demaciado pero no importa
	memset(str, 0, sizeof(str));
	_stprintf_p(str, 64, TEXT("%fL"), num);
	return (geString(str));
}

geString ToStr(bool val)
{
	return (geString( (val == true ? TEXT("true") : TEXT("false")) ));
}

/************************************************************************************************************************/
/* Esta función es básicamente igual que split() de PERL. Divide str en substrings cortándolo usando el delimitador.	*/
/* El resultado es almacenado en un vector de strings																	*/
/*																														*/
/* Parametros:																											*/
/*     str: String original																								*/
/*     vec: Vector de Strings en el cual se almacenarán los cortes														*/
/*     delimeter: Caracter que será utilizado para saber donde cortar													*/
/************************************************************************************************************************/
void Split(const geString& str, StringVec& vec, TCHAR delimiter)
{
	vec.clear();
	SIZE_T strLen = str.size();
	if( strLen == 0 )
	{
		return;
	}

	SIZE_T startIndex = 0;
	SIZE_T indexOfDel = str.find_first_of(delimiter, startIndex);
	while( indexOfDel != geString::npos )
	{
		vec.push_back(str.substr(startIndex, indexOfDel-startIndex));
		startIndex=indexOfDel + 1;
		if( startIndex >= strLen )
		{
			break;
		}
		indexOfDel = str.find_first_of(delimiter, startIndex);
	}
	
	if( startIndex < strLen )
	{
		vec.push_back(str.substr(startIndex));
	}
}

/************************************************************************************************************************/
/* Hash relativamente simple de un string de texto arbitrario a un string de identificación de 32 bits. El valor de		*/
/* salida es valido determinísticamente al valor de entrada, pero no doy ninguna garantía sobre la singularidad de la	*/
/* salida por-entrada																									*/
/*																														*/
/* El valor de entrada es tratado como minusculas para reducir las falsas separaciones causadas por errores de escritura*/
/* humanos, y tambien reduce permutaciones, pero en uso en el mundo real, el hacer este texto case-sensitive con toda	*/
/* seguridad solo causaría dolor y sufrimiento																			*/
/*																														*/
/* Este código está ligeramente basado en el checksum adler32 de Mark Adler y publicado como parte del código fuente de	*/
/* la librería de compresión zlib.																						*/
/*																														*/
/* Parametros:																											*/
/*     pIdentStr: String del que obtendremos el hash																	*/
/************************************************************************************************************************/
void * HashedString::hash_name( const TCHAR* pIdentStr )
{
	uint32 BASE = 65521L;	//Número primo mas grande utilizable con 16 bits (por lo tanto menor a 65536)
	uint32 NMAX = 5552;		//NMAX es la n más grande de tal forma que 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1

	//Definimos unos macros para simplificar código
#define DO1(buf,i)  {s1 += _totlower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

    //Si no se envió un string
	if( pIdentStr == NULL )
	{//Regresamos sin hacer nada
		return NULL;
	}

    uint32 s1 = 0;
    uint32 s2 = 0;

	for( SIZE_T len = _tcslen(pIdentStr); len>0; )
	{
		uint32 k = len<NMAX?(uint32)len:NMAX;
		len -= k;

		while( k >= 16 )
		{
			DO16(pIdentStr);
			pIdentStr += 16;
			k -= 16;
		}

        if( k != 0 )
		{
			do
			{
				s1 += _totlower( *pIdentStr++ );
				s2 += s1;
			}while( --k );
		}
		
		s1 %= BASE;
		s2 %= BASE;
    }

#pragma warning(push)
#pragma warning(disable : 4312)
    return reinterpret_cast<void *>( (s2 << 16) | s1 );
#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}
