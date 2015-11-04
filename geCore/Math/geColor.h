/********************************************************************/
/**
 * @file   geColor.h
 * @Author Samuel Prince (samuel.prince.quezada@gmail.com)
 * @date   2014/07/17
 * @brief  Color structures for use on the geEngine
 * @bug	   No known bugs.
 */
 /********************************************************************/
#pragma once

/************************************************************************************************************************/
/** \struct geLinearColor 
 * @brief Linear color struct with 32 float components
 *
 * Linear color array that represents an RGBA color with float 32 components
 * 
 */
 /************************************************************************************************************************/
struct geLinearColor
{
	/************************************************************************************************************************/
	/* Member variables declaration																							*/
	/************************************************************************************************************************/
	float R, G, B, A;							/*!< R,G,B,A 32 bits components  */
	static float PowOneOver255Table[256];		/*!< Static search table used for geColor -> geLinearColor conversions */

	/************************************************************************************************************************/
	/* Constructors																											*/
	/************************************************************************************************************************/
	/** Standard Constructor */
	FORCEINLINE geLinearColor(){}
	
	/** Explicit Constructor with forced initialization */
	FORCEINLINE explicit geLinearColor(geForceInit) : R(0), G(0), B(0), A(0){}
	
	/** \fn FORCEINLINE geLinearColor(float InR,float InG,float InB,float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA)
	 * Constructor with parameters
	 * @param [in] InR float Red channel (0-1)
	 * @param [in] InR float Green channel (0-1)
	 * @param [in] InR float Blue channel (0-1)
	 * @param [in] InA Optional float Alpha channel (0-1) Default = 1.0
	 */
	FORCEINLINE geLinearColor(float InR,float InG,float InB,float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA){}

	/** Create this color based on the data of a geColor object */
	geLinearColor(const geColor& C);

	/** Create this color based on the data of a geVector X=R, Y=G, Z=B, W=A */
	geLinearColor(const class geVector& Vector);																	
	
	/** Explicit constructor created based on a  */
	explicit geLinearColor(const class FFloat16Color& C);															/** Color a partir de un color de flotantes de 16 bits */

	/************************************************************************************************************************/
	/* Conversion functions																									*/
	/************************************************************************************************************************/
	geColor ToRGBE() const;

	/************************************************************************************************************************/
	/* Accessors																											*/
	/************************************************************************************************************************/
	FORCEINLINE float& Component(int32 Index)
	{
		return (&R)[Index];
	}

	FORCEINLINE const float& Component(int32 Index) const
	{
		return (&R)[Index];
	}

	/************************************************************************************************************************/
	/* Addition operators																									*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator+(const geLinearColor& ColorB) const
	{
		return geLinearColor(this->R + ColorB.R, this->G + ColorB.G, this->B + ColorB.B, this->A + ColorB.A);
	}

	FORCEINLINE geLinearColor& operator+=(const geLinearColor& ColorB)
	{
		R += ColorB.R;
		G += ColorB.G;
		B += ColorB.B;
		A += ColorB.A;
		return *this;
	}

	/************************************************************************************************************************/
	/* Substraction operators																								*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator-(const geLinearColor& ColorB) const
	{
		return geLinearColor(this->R - ColorB.R, this->G - ColorB.G, this->B - ColorB.B, this->A - ColorB.A);
	}
	FORCEINLINE geLinearColor& operator-=(const geLinearColor& ColorB)
	{
		R -= ColorB.R;
		G -= ColorB.G;
		B -= ColorB.B;
		A -= ColorB.A;
		return *this;
	}

	/************************************************************************************************************************/
	/* Operadores de multiplicación por otro componente geLinearColor														*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator*(const geLinearColor& ColorB) const
	{
		return geLinearColor(this->R * ColorB.R, this->G * ColorB.G, this->B * ColorB.B, this->A * ColorB.A);
	}

	FORCEINLINE geLinearColor& operator*=(const geLinearColor& ColorB)
	{
		R *= ColorB.R;
		G *= ColorB.G;
		B *= ColorB.B;
		A *= ColorB.A;
		return *this;
	}

	/************************************************************************************************************************/
	/* Operadores de multiplicación por escalar																				*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator*(float Scalar) const
	{
		return geLinearColor(this->R * Scalar, this->G * Scalar, this->B * Scalar, this->A * Scalar);
	}

	FORCEINLINE geLinearColor& operator*=(float Scalar)
	{
		R *= Scalar;
		G *= Scalar;
		B *= Scalar;
		A *= Scalar;
		return *this;
	}

	/************************************************************************************************************************/
	/* Operadores de división por otro componente geLinearColor																*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator/(const geLinearColor& ColorB) const
	{
		return geLinearColor(this->R / ColorB.R, this->G / ColorB.G, this->B / ColorB.B, this->A / ColorB.A);
	}

	FORCEINLINE geLinearColor& operator/=(const geLinearColor& ColorB)
	{
		R /= ColorB.R;
		G /= ColorB.G;
		B /= ColorB.B;
		A /= ColorB.A;
		return *this;
	}

	/************************************************************************************************************************/
	/* Operadores de división por escalar																					*/
	/************************************************************************************************************************/
	FORCEINLINE geLinearColor operator/(float Scalar) const
	{
		const float	InvScalar = 1.0f / Scalar;
		return geLinearColor(this->R * InvScalar, this->G * InvScalar, this->B * InvScalar, this->A * InvScalar);
	}

	FORCEINLINE geLinearColor& operator/=(float Scalar)
	{
		const float	InvScalar = 1.0f / Scalar;
		R *= InvScalar;
		G *= InvScalar;
		B *= InvScalar;
		A *= InvScalar;
		return *this;
	}

	/************************************************************************************************************************/
	/* Operadores de comparación																							*/
	/************************************************************************************************************************/
	FORCEINLINE bool operator==(const geLinearColor& ColorB) const
	{
		return this->R == ColorB.R && this->G == ColorB.G && this->B == ColorB.B && this->A == ColorB.A;
	}
	FORCEINLINE bool operator!=(const geLinearColor& Other) const
	{
		return this->R != Other.R || this->G != Other.G || this->B != Other.B || this->A != Other.A;
	}

	/************************************************************************************************************************/
	/* Comparación tolerante a errores																						*/
	/************************************************************************************************************************/
	FORCEINLINE bool Equals(const geLinearColor& ColorB, float Tolerance=KINDA_SMALL_NUMBER) const
	{
		return Abs(this->R - ColorB.R) < Tolerance && Abs(this->G - ColorB.G) < Tolerance && Abs(this->B - ColorB.B) < Tolerance && Abs(this->A - ColorB.A) < Tolerance;
	}

	/************************************************************************************************************************/
	/* Función de copia con cambio de alfa																					*/
	/************************************************************************************************************************/
	geLinearColor CopyWithNewOpacity(float NewOpacicty)
	{
		geLinearColor NewCopy = *this;
		NewCopy.A = NewOpacicty;
		return NewCopy;
	}

	/************************************************************************************************************************/
	/* Función que convierte hue-saturation-brightness a un RGB de punto flotante											*/
	/************************************************************************************************************************/
	static geLinearColor FGetHSV(uint8 H,uint8 S,uint8 V);

	/************************************************************************************************************************/
	/* Calcula la distancia Euclideana entre dos colores																	*/
	/************************************************************************************************************************/
	static inline float Dist(const geLinearColor &V1, const geLinearColor &V2)
	{
		return Sqrt( Square(V2.R-V1.R) + Square(V2.G-V1.G) + Square(V2.B-V1.B) + Square(V2.A-V1.A) );
	}

	/************************************************************************************************************************/
	/* Genera una lista de puntos sampleados en una curva de Bezier definida por 2 puntos									*/
	/* @param	ControlPoints		Array de 4  Colores Lineales (vert1, controlpoint1, controlpoint2, vert2)				*/
	/* @param	NumPoints			Número de samples																		*/
	/* @param	OutPoints			Recibe los samples de salida															*/
	/* @return						Longitud de la ruta																		*/
	/************************************************************************************************************************/
	//static float EvaluateBezier(const geLinearColor* ControlPoints, int32 NumPoints, TArray<geLinearColor>& OutPoints);

	/** Converts a linear space RGB color to an HSV color */
	geLinearColor LinearRGBToHSV() const;

	/** Converts an HSV color to a linear space RGB color */
	geLinearColor HSVToLinearRGB() const;

	/** Quantizes the linear color and returns the result as a FColor.  This bypasses the SRGB conversion. */
	geColor Quantize() const;

	/** Quantizes the linear color and returns the result as a FColor with optional sRGB conversion and quality as goal. */
	geColor ToFColor(const bool bSRGB) const;

	/**
	 * Returns a desaturated color, with 0 meaning no desaturation and 1 == full desaturation
	 *
	 * @param	Desaturation	Desaturation factor in range [0..1]
	 * @return	Desaturated color
	 */
	geLinearColor Desaturate( float Desaturation ) const;

	/** Computes the perceptually weighted luminance value of a color. */
	float ComputeLuminance() const;

	/**
	 * Returns the maximum value in this color structure
	 *
	 * @return	The maximum color channel value
	 */
	FORCEINLINE float GetMax() const
	{
		return Max( Max( Max( R, G ), B ), A );
	}

	/** useful to detect if a light contribution needs to be rendered */
	bool IsAlmostBlack() const
	{
		return Square(R) < DELTA && Square(G) < DELTA && Square(B) < DELTA;
	}

	/**
	 * Returns the minimum value in this color structure
	 *
	 * @return	The minimum color channel value
	 */
	FORCEINLINE float GetMin() const
	{
		return Min( Min( Min( R, G ), B ), A );
	}

	FORCEINLINE float GetLuminance() const 
	{ 
		return R * 0.3f + G * 0.59f + B * 0.11f; 
	}

	//Colores comunes
	static const geLinearColor White;
	static const geLinearColor Gray;
	static const geLinearColor Black;
	static const geLinearColor Transparent;
	static const geLinearColor Red;
	static const geLinearColor Green;
	static const geLinearColor Blue;
	static const geLinearColor Yellow;
};

FORCEINLINE geLinearColor operator*(float Scalar,const geLinearColor& Color)
{
	return Color.operator*( Scalar );
}

//
//	geColor
//

class geColor
{
public:
	// Variables.
#if PLATFORM_LITTLE_ENDIAN
    #if _MSC_VER
		// Win32 x86
	    union { struct{ uint8 B,G,R,A; }; uint32 AlignmentDummy; };
    #else
		// Linux x86, etc
	    uint8 B GCC_ALIGN(4);
	    uint8 G,R,A;
    #endif
#else // PLATFORM_LITTLE_ENDIAN
	union { struct{ uint8 A,R,G,B; }; uint32 AlignmentDummy; };
#endif

	uint32& DWColor(void) {return *((uint32*)this);}
	const uint32& DWColor(void) const {return *((uint32*)this);}

	// Constructors.
	FORCEINLINE geColor() {}
	FORCEINLINE explicit geColor(geForceInit)
	{
		// put these into the body for proper ordering with INTEL vs non-INTEL_BYTE_ORDER
		R = G = B = A = 0;
	}
	FORCEINLINE geColor( uint8 InR, uint8 InG, uint8 InB, uint8 InA = 255 )
	{
		// put these into the body for proper ordering with INTEL vs non-INTEL_BYTE_ORDER
		R = InR;
		G = InG;
		B = InB;
		A = InA;

	}
	
	// fast, for more accuracy use FLinearColor::ToFColor()
	// TODO: doesn't handle negative colors well, implicit constructor can cause
	// accidental conversion (better use .ToFColor(true))
	geColor(const geLinearColor& C)
		// put these into the body for proper ordering with INTEL vs non-INTEL_BYTE_ORDER
	{
		R = (uint8)Clamp(Trunc(Pow(C.R, 1.0f / 2.2f) * 255.0f),0,255);
		G = (uint8)Clamp(Trunc(Pow(C.G, 1.0f / 2.2f) * 255.0f),0,255);
		B = (uint8)Clamp(Trunc(Pow(C.B, 1.0f / 2.2f) * 255.0f),0,255);
		A = (uint8)Clamp(Trunc(    C.A               * 255.0f),0,255);
	}

	FORCEINLINE explicit geColor( uint32 InColor )
	{ 
		DWColor() = InColor; 
	}

	// Operators.
	FORCEINLINE bool operator==( const geColor &C ) const
	{
		return DWColor() == C.DWColor();
	}
	FORCEINLINE bool operator!=( const geColor& C ) const
	{
		return DWColor() != C.DWColor();
	}
	FORCEINLINE void operator+=(const geColor& C)
	{
		R = (uint8)Min((int32) R + (int32) C.R,255);
		G = (uint8)Min((int32) G + (int32) C.G,255);
		B = (uint8)Min((int32) B + (int32) C.B,255);
		A = (uint8)Min((int32) A + (int32) C.A,255);
	}
	
	geLinearColor FromRGBE() const;

	/**
	 * Makes a random but quite nice color.
	 */
	static geColor MakeRandomColor();

	/**
	 * Makes a color red->green with the passed in scalar (e.g. 0 is red, 1 is green)
	 */
	static geColor MakeRedToGreenColorFromScalar(float Scalar);

	/** Reinterprets the color as a linear color. */
	FORCEINLINE geLinearColor ReinterpretAsLinear() const
	{
		return geLinearColor(R/255.f,G/255.f,B/255.f,A/255.f);
	}

	/** Some pre-inited colors, useful for debug code */
	static const geColor White;
	static const geColor Black;
	static const geColor Red;
	static const geColor Green;
	static const geColor Blue;
	static const geColor Yellow;
	static const geColor Cyan;
	static const geColor Magenta;
};

FORCEINLINE uint32 GetTypeHash( const geColor& Color )
{
	return Color.DWColor();
}

/** Computes a brightness and a fixed point color from a floating point color. */
extern void ComputeAndFixedColorAndIntensity(const geLinearColor& InLinearColor,geColor& OutColor,float& OutIntensity);

/**
 * Helper struct for a 16 bit 565 color of a DXT1/3/5 block.
 */
struct FDXTColor565
{
	/** Blue component, 5 bit. */
	uint16 B:5;
	/** Green component, 6 bit. */
	uint16 G:6;
	/** Red component, 5 bit */
	uint16 R:5;
};

/**
 * Helper struct for a 16 bit 565 color of a DXT1/3/5 block.
 */
struct FDXTColor16
{
	union 
	{
		/** 565 Color */
		FDXTColor565 Color565;
		/** 16 bit entity representation for easy access. */
		uint16 Value;
	};
};

/**
 * Structure encompassing single DXT1 block.
 */
struct FDXT1
{
	/** Color 0/1 */
	union
	{
		FDXTColor16 Color[2];
		uint32 Colors;
	};
	/** Indices controlling how to blend colors. */
	uint32 Indices;
};

/**
 * Structure encompassing single DXT5 block
 */
struct FDXT5
{
	/** Alpha component of DXT5 */
	uint8	Alpha[8];
	/** DXT1 color component. */
	FDXT1	DXT1;
};
