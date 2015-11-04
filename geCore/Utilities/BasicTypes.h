/********************************************************************
	Created:	2014/07/14
	Filename:	BasicTypes.h
	Author:		Samuel Prince

	Purpose:	Definición de tipos de objeto básicos
				(Point y Rect)
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Incluimos archivos necesarios para la compilación																	*/
/************************************************************************************************************************/

/************************************************************************************************************************/
/* Clase que representa un punto en un espacio 2D (números enteros, para flotantes usar geVector2D)						*/
/************************************************************************************************************************/
class gePoint
{
public:
	/************************************************************************************************************************/
	/* Declaración de variables miembro																						*/
	/************************************************************************************************************************/
	int32 X, Y;

	/************************************************************************************************************************/
	/* Constructores																										*/
	/************************************************************************************************************************/
	gePoint(void) { X = Y = 0; }
	gePoint(const int32 newX, const int32 newY) { X = newX; Y = newY; }
	gePoint(const gePoint& newPoint) { X = newPoint.X; Y = newPoint.Y; }
	gePoint(const gePoint* pNewPoint) { X = pNewPoint->X; Y = pNewPoint->Y; }
	
	/************************************************************************************************************************/
	/* Operadores de Asignación																								*/
	/************************************************************************************************************************/
	gePoint& operator=(const gePoint& newPoint) { X = newPoint.X; Y = newPoint.Y; return (*this); }
	gePoint& operator=(const gePoint* pNewPoint) { X = pNewPoint->X; Y = pNewPoint->Y; return (*this); }

	/************************************************************************************************************************/
	/* Operadores de adición y substracción																					*/
	/************************************************************************************************************************/
	gePoint& operator+=(const gePoint& newPoint) { X += newPoint.X; Y += newPoint.Y; return (*this); }
	gePoint& operator-=(const gePoint& newPoint) { X -= newPoint.X; Y -= newPoint.Y; return (*this); }
	gePoint& operator+=(const gePoint* pNewPoint) { X += pNewPoint->X; Y += pNewPoint->Y; return (*this); }
	gePoint& operator-=(const gePoint* pNewPoint) { X -= pNewPoint->X; Y -= pNewPoint->Y; return (*this); }
	gePoint operator+(const gePoint& other) { gePoint temp(this); temp += other; return temp; }
	gePoint operator-(const gePoint& other) { gePoint temp(this); temp -= other; return temp; }

	/************************************************************************************************************************/
	/* Operadores de comparación																							*/
	/************************************************************************************************************************/
	bool operator==(const gePoint& other) const { return ((X == other.X) && (Y == other.Y)); }
	bool operator!=(const gePoint& other) const { return (!((X == other.Y) && (Y == other.Y))); }

	/************************************************************************************************************************/
	/* Accesores (Generados para el uso con lenguajes de scripting)															*/
	/************************************************************************************************************************/
	int32 GetX() const { return X; }
	int32 GetY() const { return Y; }
	void SetX(const int32 newX) { X = newX; }
	void SetY(const int32 newY) { Y = newY; }
	void Set(const int32 newX, const int32 newY) { X = newX; Y = newY; }

	/************************************************************************************************************************/
	/* Una pequeña función un poco inutil ya que existe la versión del vector pero por si acaso	alguien la utiliza			*/
	/************************************************************************************************************************/
	float Size() const { return Sqrt((float)(X*X+Y*Y)); }
};

/************************************************************************************************************************/
/* Operador de negación																									*/
/************************************************************************************************************************/
FORCEINLINE gePoint operator-(const gePoint& left, const gePoint& right) { gePoint temp(left); temp -= right; return temp; }


/************************************************************************************************************************/
/* Clase que representa un rectángulo en un espacio 2D																	*/
/************************************************************************************************************************/
class geRect
{
public:
	/************************************************************************************************************************/
	/* Declaración de variables miembro																						*/
	/************************************************************************************************************************/
	int32 top, left, right, bottom;

	/************************************************************************************************************************/
	/* Constructores																										*/
	/************************************************************************************************************************/
	geRect(void) { left = top = right = bottom = 0; }
	geRect(int32 newLeft, int32 newTop, int32 newRight, int32 newBottom) { Set(newLeft, newTop, newRight, newBottom); }
	geRect(const geRect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }
	geRect(geRect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; }
	geRect(const gePoint& topLeft, const gePoint& bottomRight) { top = topLeft.Y; left = topLeft.X; right = bottomRight.X; bottom = bottomRight.Y; }

	/************************************************************************************************************************/
	/* Operadores de Asignación																								*/
	/************************************************************************************************************************/
	geRect& operator=(const geRect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; return (*this); }
	geRect& operator=(const geRect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; return (*this); }

	/************************************************************************************************************************/
	/* Operadores de adición y substracción																					*/
	/************************************************************************************************************************/
	geRect& operator+=(const geRect& newRect) { left += newRect.left; top += newRect.top; right += newRect.right; bottom += newRect.bottom; return (*this); }
	geRect& operator-=(const geRect& newRect) { left -= newRect.left; top -= newRect.top; right -= newRect.right; bottom -= newRect.bottom; return (*this); }
	geRect& operator+=(const geRect* pNewRect) { left += pNewRect->left; top += pNewRect->top; right += pNewRect->right; bottom += pNewRect->bottom; return (*this); }
	geRect& operator-=(const geRect* pNewRect) { left -= pNewRect->left; top -= pNewRect->top; right -= pNewRect->right; bottom -= pNewRect->bottom; return (*this); }
	geRect operator+(geRect& other) { geRect temp(this); temp += other; return temp; }
	geRect operator-(geRect& other) { geRect temp(this); temp -= other; return temp; }

	// move a rectangle by an amount defined by a Point, keeping the rectanle's size constant
	geRect& operator+=(const gePoint& delta) { left += delta.X; top += delta.Y; right += delta.X; bottom += delta.Y; return (*this); }
	geRect& operator-=(const gePoint& delta) { left -= delta.X; top -= delta.Y; right -= delta.X; bottom -= delta.Y; return (*this); }
	geRect& operator+=(const gePoint* pDelta) { left += pDelta->X; top += pDelta->Y; right += pDelta->X; bottom += pDelta->Y; return (*this); }
	geRect& operator-=(const gePoint* pDelta) { left -= pDelta->X; top -= pDelta->Y; right -= pDelta->X; bottom -= pDelta->Y; return (*this); }
	geRect operator+(gePoint& delta) { geRect temp(this); temp += delta; return temp; }
	geRect operator-(gePoint& delta) { geRect temp(this); temp -= delta; return temp; }

	/************************************************************************************************************************/
	/* Operadores de comparación																							*/
	/************************************************************************************************************************/
	bool operator==(const geRect& other) const { return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom)); }
	bool operator!=(const geRect& other) const { return (!((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom))); }

	/************************************************************************************************************************/
	/* Funciones de revisión de colisiones																					*/
	/************************************************************************************************************************/
	bool Collide(const geRect& other) const { if (other.bottom < top || other.right < left || other.left > right || other.top > bottom) {return false;} else {return true;} }
	bool Collide(const gePoint& p) const { if( p.X > left && p.X < right && p.Y < bottom && p.Y > top ){return true;} else {return false;} }
	bool IsWithin(const geRect& other) const { return ( (left >= other.left && top >= other.top && right <= other.right && bottom <= other.bottom) || (other.left >= left && other.top >= top && other.right <= right && other.bottom <= bottom) ); }
	bool IsWithin(const gePoint& other) const { return (other.X >= left && other.X <= right && other.Y >= top && other.Y <= bottom); }
	bool IsValid(void) const { return (left <= right && top <= bottom); }
	bool IsNull(void) const { return (left == 0 && right == 0 && top == 0 && bottom == 0); }

	/************************************************************************************************************************/
	/* Funciones de conveniencia																							*/
	/************************************************************************************************************************/
	void ShiftX(int dx) { left += dx; right += dx; }
	void ShiftY(int dy) { top += dy; bottom += dy; }
	void SetX(int x) { int dx = x - left; ShiftX(dx); }
	void SetY(int y) { int dy = y - top; ShiftY(dy); }

	/************************************************************************************************************************/
	/* Accesores de uso para el lenguaje de script																			*/
	/************************************************************************************************************************/
	int32 GetTop(void) const { return top; }
	int32 GetLeft(void) const { return left; }
	int32 GetRight(void) const { return right; }
	int32 GetBottom(void) const { return bottom; }
	gePoint GetCenter(void) const
	{
		if( IsValid() )
		{
			return (gePoint(left + ((right - left) / 2), top + ((bottom - top) / 2)));
		}
		GEE_ERROR( TEXT("gePoint::GetCenter() - Se intentó obtener el centro de un geRect inválido") );
		return gePoint();
	}
	gePoint TopLeft(void) const { return gePoint(left, top); }
	gePoint TopRight(void) const { return gePoint(right, top); }
	gePoint BottomLeft(void) const { return gePoint(left, bottom); }
	gePoint BottomRight(void) const { return gePoint(right, bottom); }
	long GetWidth(void) const { return right - left; }
	long GetHeight(void) const { return bottom - top; }
	void Set(int32 newLeft, int32 newTop, int32 newRight, int32 newBottom) { left = newLeft; top = newTop; right = newRight; bottom = newBottom; }
	void MoveDelta(int32 dx, int32 dy) { left += dx; top += dy; right += dx; bottom += dy; }
	void MoveDelta(const gePoint deltaPoint) { MoveDelta(deltaPoint.X, deltaPoint.Y); }
	void MoveTo(int32 x, int32 y)
	{
		int32 width = right - left;
		int32 height = bottom - top;
		left = x;
		right = left + width;
		top = y;
		bottom = top + height;
	}
	void MoveTo(const gePoint& point) { MoveTo(point.X, point.Y); }
};

