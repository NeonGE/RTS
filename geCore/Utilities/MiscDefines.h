/********************************************************************
	Created:	2014/07/17
	Filename:	MiscDefines.h
	Author:		Samuel Prince

	Purpose:	Definición de constantes miscelaneas
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Valores máximos y mínimos de los diferentes tipos de variable														*/
/************************************************************************************************************************/
enum {MIN_UINT8		= 0x00       };
enum {MIN_UINT16	= 0x0000U    };
enum {MIN_UINT32	= 0x00000000U};
enum {MIN_INT8		= 0x80       };
enum {MIN_INT16		= 0x8000     };
enum {MIN_INT32		= 0x80000000 };

enum {MAX_UINT8		= 0xff       };
enum {MAX_UINT16	= 0xffffU    };
enum {MAX_UINT32	= 0xffffffffU};
enum {MAX_INT8		= 0x7f       };
enum {MAX_INT16		= 0x7fff     };
enum {MAX_INT32		= 0x7fffffff };

enum {INDEX_NONE	= -1         };
enum {UNICODE_BOM   = 0xfeff     };

/************************************************************************************************************************/
/* Enums utilizados para marcar definiciones con inizialización forzada e inicialización no hecha						*/
/************************************************************************************************************************/
enum geForceInit 
{
	ForceInit,
	ForceInitToZero
};
enum geNoInit {NoInit};