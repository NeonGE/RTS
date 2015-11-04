/********************************************************************
	Created:	2014/07/17
	Filename:	geFloat32.h
	Author:		Samuel Prince

	Purpose:	Declaración de una clase para el uso de flotantes
				de 32 bits con acceso a sus elementos
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase geFloat32																					*/
/************************************************************************************************************************/
class geFloat32
{
public:
	union
	{
		struct
		{
#if PLATFORM_LITTLE_ENDIAN
			uint32 Mantissa : 23;
			uint32 Exponent : 8;
			uint32 Sign : 1;			
#else
			uint32 Sign : 1;
			uint32 Exponent : 8;
			uint32 Mantissa : 23;			
#endif
		}Components;
		float FloatValue;
	};

	/************************************************************************************************************************/
	/* Constructor con la entrada del valor de inicialización																*/
	/************************************************************************************************************************/
	geFloat32(float InValue=0.0f);
};

FORCEINLINE geFloat32::geFloat32(float InValue) : FloatValue(InValue)
{

}