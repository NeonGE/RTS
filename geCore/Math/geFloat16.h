/********************************************************************
	Created:	2014/07/17
	Filename:	geFloat32.h
	Author:		Samuel Prince

	Purpose:	Declaración de una clase para el uso de flotantes
				de 16 bits con acceso a sus elementos

				IEEE float 16

				Representado por 10 bits de mantissa M, 5 bits de
				exponente E, y 1 bit de signo S.

				Especiales:

				E=0, M=0		== 0.0
				E=0, M!=0		== Valor denormalizado (M/2^10)*2^-14
				0<E<31, M=any	== (1 + M / 2^10) * 2^(E-15)
				E=32, M=0		== Infinito
				E=31, M!=0		== NAN
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase geFloat16																					*/
/************************************************************************************************************************/
class geFloat16
{
public:
	union
	{
		struct
		{
#if PLATFORM_LITTLE_ENDIAN
			uint16 Mantissa : 10;
			uint16 Exponent : 5;
			uint16 Sign : 1;
#else
			uint16 Sign : 1;
			uint16 Exponent : 5;
			uint16 Mantissa : 10;			
#endif
		}Components;
		uint16 Encoded;
	};

	/************************************************************************************************************************/
	/* Constructores																										*/
	/************************************************************************************************************************/
	geFloat16();								//Constructor standard
	geFloat16(const geFloat16& FP16Value);		//Constructor de copia
	geFloat16(float FP32Value);					//Constructor de conversión (convierte un geFloat32 a geFloat16)

	/************************************************************************************************************************/
	/* Operadores de asignación																								*/
	/************************************************************************************************************************/
	geFloat16& operator=(float FP32Value);				//Iguala el valor con el de un geFloat32
	geFloat16& operator=(const geFloat16& FP16Value);	//Copia el valor de otro geFloat16

	/************************************************************************************************************************/
	/* Operadores de conversión																								*/
	/************************************************************************************************************************/
	operator float() const;								//Convierte el valor de un FP16 a FP32
	void Set(float FP32Value);							//Establece el valor de un FP16 a partir de un FP32
	float GetFloat() const;								//Convierte de un FP16 a un FP32
};

/************************************************************************************************************************/
/* Implementación de funciones																							*/
/************************************************************************************************************************/
FORCEINLINE geFloat16::geFloat16() : Encoded(0)
{
}

FORCEINLINE geFloat16::geFloat16(const geFloat16& FP16Value)
{
	Encoded = FP16Value.Encoded;
}

FORCEINLINE geFloat16::geFloat16(float FP32Value)
{
	Set(FP32Value);
}	

FORCEINLINE geFloat16& geFloat16::operator=(float FP32Value)
{
	Set(FP32Value);
	return *this;
}

FORCEINLINE geFloat16& geFloat16::operator=(const geFloat16& FP16Value)
{
	Encoded = FP16Value.Encoded;
	return *this;
}

FORCEINLINE geFloat16::operator float() const
{
	return GetFloat();
}

FORCEINLINE void geFloat16::Set(float FP32Value)
{
	geFloat32 FP32(FP32Value);

	//Copiamos el bit del signo
	Components.Sign = FP32.Components.Sign;

	//Revisamos si es un valor en zero, denormalizado o demaciado pequeño
	if( FP32.Components.Exponent <= 112 )			//Exponente muy pequeño? (0+127-15)
	{//Lo establecemos en 0
		Components.Exponent = 0;
		Components.Mantissa = 0;
	}
	else if ( FP32.Components.Exponent >= 143 )		//Esponente muy grande? (31+127-15)
	{//Revisamos si es un valor infinito o NAN o un valor muy elevado
		//lo ponemos como un valor hasta 65504.0 (valor máximo)
		Components.Exponent = 30;
		Components.Mantissa = 1023;
	}
	else
	{//Lo manejamos como un número normal
		Components.Exponent = int32(FP32.Components.Exponent) - 127 + 15;
		Components.Mantissa = uint16(FP32.Components.Mantissa >> 13);
	}
}

FORCEINLINE float geFloat16::GetFloat() const
{
	//Creamos variables temporales
	geFloat32 Result;	//El valor de salida

	//Copiamos el bit del signo
	Result.Components.Sign = Components.Sign;
	if( Components.Exponent == 0 )
	{//Si el exponente es 0
		uint32 Mantissa = Components.Mantissa;
		if( Mantissa == 0 )
		{//Si la mantissa es igual a zero
			Result.Components.Exponent = 0;
			Result.Components.Mantissa = 0;
		}
		else
		{//Valor denormalizado
			uint32 MantissaShift = 10 - (uint32)Trunc(Log2((float)Mantissa));
			Result.Components.Exponent = 127 - (15 - 1) - MantissaShift;
			Result.Components.Mantissa = Mantissa << (MantissaShift + 23 - 10);
		}
	}
	else if( Components.Exponent == 31 )	//2^5 - 1
	{//Si es infinito o NAN. Establecemos a 65504.0
		Result.Components.Exponent = 142;
		Result.Components.Mantissa = 8380416;
	}
	else
	{//Es un número normal
		Result.Components.Exponent = int32(Components.Exponent) - 15 + 127;	//Exponentes almacenados son  Stored exponents are biased by half their range.
		Result.Components.Mantissa = uint32(Components.Mantissa) << 13;
	}

	return Result.FloatValue;
}
