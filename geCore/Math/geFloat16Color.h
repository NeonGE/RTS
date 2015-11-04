/********************************************************************/
/**
 * @file   geFloat16Color.h
 * @Author Samuel Prince (samuel.prince.quezada@gmail.com)
 * @date   2014/08/19
 * @brief  
 *
 * Declaration of the geFloat16 class for use of colors with 
 * float 16 bits components
 *
 * @bug	   No known bugs.
 */
 /********************************************************************/
#pragma once

/** \class geFloat16Color
 *	RGBA Color made up of geFloat16 components
 */
class geFloat16Color
{
	/************************************************************************************************************************/
	/* Member variables definition																							*/
	/************************************************************************************************************************/
public:
	geFloat16 R;
	geFloat16 G;
	geFloat16 B;
	geFloat16 A;

	/************************************************************************************************************************/
	/* Member functions definition																							*/
	/************************************************************************************************************************/
public:
	/** Default constructor */
	geFloat16Color();

	/** Copy constructor. */
	geFloat16Color(const geFloat16Color& Src);

	/** Constructor from a linear color. */
	geFloat16Color(const geLinearColor& Src);

	/** Assignment operator */
	geFloat16Color& operator=(const geFloat16Color& Src);

 	/**
	 * Checks whether two colors are identical.
	 * @param Src The other color.
	 * @return true if the two colors are identical, otherwise false.
	 */
	bool operator==(const geFloat16Color& Src);
};


FORCEINLINE geFloat16Color::geFloat16Color()
{
}

FORCEINLINE geFloat16Color::geFloat16Color(const geFloat16Color& Src)
{
	R = Src.R;
	G = Src.G;
	B = Src.B;
	A = Src.A;
}

FORCEINLINE geFloat16Color::geFloat16Color(const geLinearColor& Src) :
	R(Src.R),
	G(Src.G),
	B(Src.B),
	A(Src.A)
{
}

FORCEINLINE geFloat16Color& geFloat16Color::operator=(const geFloat16Color& Src)
{
	R = Src.R;
	G = Src.G;
	B = Src.B;
	A = Src.A;
	return *this;
}

FORCEINLINE bool geFloat16Color::operator==(const geFloat16Color& Src)
{
	return ( (R == Src.R) && (G == Src.G) && (B == Src.B) && (A == Src.A) );
}
