/********************************************************************
	Created:	2014/07/15
	Filename:	geVector.h
	Author:		Samuel Prince

	Purpose:	Definición de una estructura para manejo de vectores 3D
*********************************************************************/
#pragma once

/************************************************************************************************************************/
/* Declaración de la clase geVector																						*/
/************************************************************************************************************************/
class geVector 
{
	/************************************************************************************************************************/
	/* Declaración de variables miembro																						*/
	/************************************************************************************************************************/
public:
	float X,Y,Z;	//Este es un vector 3D (3x1 Float)

	/************************************************************************************************************************/
	/* Constructores																										*/
	/************************************************************************************************************************/
	FORCEINLINE geVector();											//Constructor standard
	explicit FORCEINLINE geVector(float InF);						//Constructor explícito InF se utiliza para rellenar todos los valores del vector
	FORCEINLINE geVector(float InX, float InY, float InZ);			//Constructor con parámetros para cada componente del objeto
	explicit FORCEINLINE geVector(const geVector2D V, float InZ);	//Constructor que copia la información de un vector 2D y un componente Z
	FORCEINLINE geVector(const geVector4& V);						//Constructor que obtiene la información de un vector 4D
	explicit geVector(const geLinearColor& InColor);				//Constructor que toma la información de un color lineal
	explicit geVector(geIntVector InVector);

	/**
	 * Constructor
	 *
	 * @param A Int Point used to set X and Y coords, Z is set to zero.
	 */
	explicit geVector( FIntPoint A );

	/**
	 * Constructor
	 *
	 * @param EForceInit Force Init Enum
	 */
	explicit FORCEINLINE geVector(EForceInit);

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
	/**
	* Copy another FVector into this one
	*
	* @param Other The other vector.
	*
	* @return Reference to vector after copy.
	*/
	FORCEINLINE geVector& operator=(const geVector& Other);
#endif

	/**
	 * Calculate Cross product between this and another vector.
	 *
	 * @param V The other vector.
	 *
	 * @return The Cross product.
	 */
	FORCEINLINE geVector operator^( const geVector& V ) const;

	/**
	 * Calculate the Cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 *
	 * @return The Cross product.
	 */
	FORCEINLINE static geVector CrossProduct( const geVector& A, const geVector& B );

	/**
	 * Calculate the Dot Product between this and another vector.
	 *
	 * @param V The other vector.
	 *
	 * @return The Dot Product.
	 */
	FORCEINLINE float operator|( const geVector& V ) const;

	/**
	 * Calculate the Dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 *
	 * @return The Dot product.
	 */
	FORCEINLINE static float DotProduct( const geVector& A, const geVector& B );

	/**
	 * Gets the result of adding a vector to this.
	 *
	 * @param V The vector to add.
	 *
	 * @return The result of vector addition.
	 */
	FORCEINLINE geVector operator+( const geVector& V ) const;

	/**
	 * Gets the result of subtracting a vector from this.
	 *
	 * @param V The vector to subtract.
	 *
	 * @return The result of vector subtraction.
	 */
	FORCEINLINE geVector operator-( const geVector& V ) const;

	/**
	 * Gets the result of subtracting from each component of the vector.
	 *
	 * @param Bias What to subtract.
	 *
	 * @return The result of subtraction.
	 */
	FORCEINLINE geVector operator-( float Bias ) const;

	/**
	 * Gets the result of adding to each component of the vector.
	 *
	 * @param Bias What to add.
	 *
	 * @return The result of addition.
	 */
	FORCEINLINE geVector operator+( float Bias ) const;

	/**
	 * Gets the result of multiplying each component of the vector.
	 *
	 * @param Scale What to multiply by.
	 *
	 * @return The result of multiplication.
	 */
	FORCEINLINE geVector operator*( float Scale ) const;

	/**
	 * Gets the result of dividing each component of the vector.
	 *
	 * @param Scale What to divide by.
	 *
	 * @return The result of division.
	 */
	geVector operator/( float Scale ) const;

	/**
	 * Gets the result of multiplying vector with this.
	 *
	 * @param V The vector to multiply with.
	 *
	 * @return The result of multiplication.
	 */
	FORCEINLINE geVector operator*( const geVector& V ) const;

	/**
	 * Gets the result of dividing this vector by another.
	 *
	 * @param V The vector to divide by.
	 *
	 * @return The result of division.
	 */
	FORCEINLINE geVector operator/( const geVector& V ) const;

	// Binary comparison operators.

	/**
	 * Check against another vector for equality.
	 *
	 * @param V The vector to check against.
	 *
	 * @return true if the vectors are equal, otherwise false.
	 */
	bool operator==( const geVector& V ) const;

	/**
	 * Check against another vector for inequality.
	 *
	 * @param V The vector to check against.
	 *
	 * @return true if the vectors are not equal, otherwise false.
	 */
	bool operator!=( const geVector& V ) const;

	/**
	 * Check against another vector for equality, within specified error limits.
	 *
	 * @param V The vector to check against.
	 * @param Tolerance Error tolerance.
	 *
	 * @return true if the vectors are equal within tolerance limits, otherwise false.
	 */
	bool Equals(const geVector& V, float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether all components of the vector are the same, within a tolerance.
	 *
	 * @param Tolerance Error Tolerance.
	 *
	 * @return true if the vectors are equal within tolerance limits, otherwise false.
	 */
	bool AllComponentsEqual(float Tolerance=KINDA_SMALL_NUMBER) const;


	/**
	 * Get a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE geVector operator-() const;


	/**
	 * Adds another vector to this.
	 *
	 * @param V Vector to add.
	 *
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE geVector operator+=( const geVector& V );

	/**
	 * Subtracts another vector from this.
	 *
	 * @param V Vector to subtract.
	 *
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE geVector operator-=( const geVector& V );

	/**
	 * Scales the vector.
	 *
	 * @param Scale What to scale vector by.
	 *
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE geVector operator*=( float Scale );

	/**
	 * Divides the vector by a number.
	 *
	 * @param V What to divide the vector by.
	 *
	 * @return Copy of the vector after division.
	 */
	geVector operator/=( float V );

	/**
	 * Multiplies the vector with another vector.
	 *
	 * @param V What to multiply vector with.
	 *
	 * @return Copy of the vector after multiplication.
	 */
	geVector operator*=( const geVector& V );

	/**
	 * Divides the vector by another vector.
	 *
	 * @param V What to divide vector by.
	 *
	 * @return Copy of the vector after division.
	 */
	geVector operator/=( const geVector& V );

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 *
	 * @return reference to component.
	 */
	float& operator[]( int32 Index );

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 *
	 * @return Copy of the component.
	 */
	float operator[]( int32 Index )const;

	// Simple functions.

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 * @param InZ New Z coordinate.
	 */
	void Set( float InX, float InY, float InZ );

	/**
	 * Get the maximum of the vectors coordinates.
	 *
	 * @return The maximum of the vectors coordinates.
	 */
	float GetMax() const;

	/**
	 * Get the absolute maximum of the vectors coordinates.
	 *
	 * @return The absolute maximum of the vectors coordinates.
	 */
	float GetAbsMax() const;

	/**
	 * Get the minimum of the vectors coordinates.
	 *
	 * @return The minimum of the vectors coordinates.
	 */
	float GetMin() const;

	/**
	 * Get the absolute minimum of the vectors coordinates.
	 *
	 * @return The absolute minimum of the vectors coordinates.
	 */
	float GetAbsMin() const;

	/** Gets the component-wise min of two vectors. */
	geVector ComponentMin(const geVector& Other) const;

	/** Gets the component-wise max of two vectors. */
	geVector ComponentMax(const geVector& Other) const;

	/**
	 * Get a copy of this vector with absolute components.
	 *
	 * @return A copy of this vector with absolute components.
	 */
	geVector GetAbs() const;

	/**
	 * Get the length of this vector.
	 *
	 * @return The length of this vector.
	 */
	float Size() const;

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	float SizeSquared() const;

	/**
	 * Get the length of the 2D components of this vector.
	 *
	 * @return The 2D length of this vector.
	 */
	float Size2D() const ;

	/**
	 * Get the squared length of the 2D components of this vector.
	 *
	 * @return The squared 2D length of this vector.
	 */
	float SizeSquared2D() const ;

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 *
	 * @return true if the vector is near to zero, otherwise false.
	 */
	bool IsNearlyZero(float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether vector is exactly zero.
	 *
	 * @return true if the vector is exactly zero, otherwise false.
	 */
	bool IsZero() const;

	/**
	 * Normalize this vector if it is large enough.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 *
	 * @return true if the vector was normalized correctly, otherwise false.
	 */
	bool Normalize(float Tolerance=SMALL_NUMBER);

	/**
	 * Checks whether vector is normalized.
	 *
	 * @return true if Normalized, false otherwise.
	 */
	bool IsNormalized() const;

	/**
	 * Util to convert this vector into a unit direction vector, and its original length
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(geVector &OutDir, float &OutLength);

	/**
	 * Get a copy of the vector as sign only.
	 *
	 * @param A copy of the vector with each component set to 1 or -1
	 */
	FORCEINLINE geVector GetSignVector();

	/**
	 * Projects 2D components of vector based on Z.
	 *
	 * @return Projected version of vector.
	 */
	geVector Projection() const;

	/**
	 * Calculates normalized version of vector without checking if it is non-zero.
	 *
	 * @return Normalized version of vector.
	 */
	FORCEINLINE geVector UnsafeNormal() const;

	/**
	 * Gets a copy of this vector snapped to a grid.
	 *
	 * @param GridSz Grid dimension.
	 *
	 * @return A copy of this vector snapped to a grid.
	 */
	geVector GridSnap( const float& GridSz ) const;

	/**
	 * Get a copy of this vector, clamped inside of a cube.
	 *
	 * @param Radius Half size of the cube.
	 *
	 * @return A copy of this vector, bound by cube.
	 */
	geVector BoundToCube( float Radius ) const;

	/**
	 * Add a vector to this and clamp the result in a cube.
	 *
	 * @param V Vector to add.
	 * @param Radius Half size of the cube.
	 */
	void AddBounded( const geVector& V, float Radius=MAX_int16 );

	/**
	 * Gets a specific component of the vector.
	 *
	 * @param Index The index of the component required.
	 *
	 * @return Reference to the specified component.
	 */
	float& Component( int32 Index );

	/**
	 * Gets a specific component of the vector.
	 *
	 * @param Index The index of the component required.
	 *
	 * @return Copy of the specified component.
	 */
	float Component( int32 Index ) const;

	/**
	 * Gets the reciprocal of this vector, avoiding division by zero.
	 * Zero components set to BIG_NUMBER.
	 *
	 * @return Reciprocal of this vector.
	 */
	geVector Reciprocal() const;

	/**
	 * Check whether X, Y and Z are nearly equal.
	 *
	 * @param Tolerance Specified Tolerance.
	 *
	 * @return true if X == Y == Z within the specified tolerance.
	 */
	bool IsUniform(float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Mirror a vector about a normal vector.
	 *
	 * @param MirrorNormal Normal vector to mirror about.
	 *
	 * @return Mirrored vector.
	 */
	geVector MirrorByVector( const geVector& MirrorNormal ) const;

	/**
	 * Mirrors a vector about a plane.
	 *
	 * @param Plane Plane to mirror about.
	 *
	 * @return Mirrored vector.
	 */
	geVector MirrorByPlane( const FPlane& Plane ) const;

	/**
	 * Rotates around Axis (assumes Axis.Size() == 1).
	 *
	 * @param Angle Angle to rotate (in degrees).
	 * @param Axis Axis to rotate around.
	 *
	 * @return Rotated Vector.
	 */
	geVector RotateAngleAxis( const float AngleDeg, const geVector& Axis ) const;

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so.
	 *
	 * @param Tolerance Minimum squared vector length.
	 *
	 * @return Normalized copy if safe, otherwise returns zero vector.
	 */
	geVector SafeNormal(float Tolerance=SMALL_NUMBER) const;

	/**
	 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so.
	 *
	 * @param Tolerance Minimum squared vector length.
	 *
	 * @return Normalized copy if safe, otherwise returns zero vector.
	 */
	geVector SafeNormal2D(float Tolerance=SMALL_NUMBER) const;

	/**
	 * Performs a 2D dot product (no z)
	 *
	 * @param B the vector to perform the dot product with
	 *
	 * @return The 2D dot product.
	 */
	FORCEINLINE float Dot2d(geVector B) const;

	/**
	 * Projects this vector onto the input vector.
	 *
	 * @param A Vector to project onto, does not assume it is unnormalized.
	 *
	 * @return Projected vector.
	 */
	FORCEINLINE geVector ProjectOnTo( const geVector& A ) const ;

	/**
	 * Return the FRotator corresponding to the direction that the vector
	 * is pointing in.  Sets Yaw and Pitch to the proper numbers, and sets
	 * roll to zero because the roll can't be determined from a vector.
	 *
	 * @return The FRotator from the vector's direction.
	 */
	CORE_API FRotator Rotation() const;

	/**
	 * Find good arbitrary axis vectors to represent U and V axes of a plane,
	 * given just the normal.
	 *
	 * @param Axis1 Reference to first axis.
	 * @param Axis2 Reference to second axis.
	 */
	CORE_API void FindBestAxisVectors( geVector& Axis1, geVector& Axis2 ) const;

	/** When this vector contains Euler angles (degrees), ensure that angles are between +/-180 */
	CORE_API void UnwindEuler();

	/**
	 * Utility to check if there are any NaNs in this vector.
	 *
	 * @return true if there are any NaNs in this vector, otherwise false.
	 */
	bool ContainsNaN() const;

	/**
	 * Check if the vector is of unit length, with specified tolerance.
	 *
	 * @param LengthSquaredTolerance Tolerance against squared length.
	 *
	 * @return true if the vector is a unit vector within the specified tolerance.
	 */
	FORCEINLINE bool IsUnit(float LengthSquaredTolerance = KINDA_SMALL_NUMBER ) const;

	/**
	 * Get a textual representation of this vector.
	 *
	 * @return A string describing the vector.
	 */
	FString ToString() const;

	/**
	 * Initialize this Vector based on an FString. The String is expected to contain X=, Y=, Z=.
	 * The FVector will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the vector values.
	 *
	 * @return true if the X,Y,Z values were read successfully; false otherwise.
	 */
	bool InitFromString( const FString & InSourceString );

	/** 
	 * Converts a cartesian unit vector into spherical coordinates on the unit sphere.
	 * @return Output Theta will be in the range [0, PI], and output Phi will be in the range [-PI, PI]. 
	 */
	FVector2D UnitCartesianToSpherical() const;

	/**
	 * Convert a direction vector into a 'heading' angle.
	 *
	 * @return 'Heading' angle between +/-PI. 0 is pointing down +X.
	 */
	float HeadingAngle() const;

	/**
	 * Create an orthonormal basis from a basis with at least two orthogonal vectors.
	 * It may change the directions of the X and Y axes to make the basis orthogonal,
	 * but it won't change the direction of the Z axis.
	 * All axes will be normalized.
	 *
	 * @param XAxis - The input basis' XAxis, and upon return the orthonormal basis' XAxis.
	 * @param YAxis - The input basis' YAxis, and upon return the orthonormal basis' YAxis.
	 * @param ZAxis - The input basis' ZAxis, and upon return the orthonormal basis' ZAxis.
	 */
	static CORE_API void CreateOrthonormalBasis(geVector& XAxis,geVector& YAxis,geVector& ZAxis);

	/**
	 * Compare two points and see if they're the same, using a threshold.
	 *
	 * @param P First vector.
	 * @param Q Second vector.
	 *
	 * @return 1=yes, 0=no.  Uses fast distance approximation.
	 */
	static bool PointsAreSame( const geVector &P, const geVector &Q );
	
	/**
	 * Compare two points and see if they're within specified distance.
	 *
	 * @param Point1 First vector.
	 * @param Point2 Second vector.
	 * @param Dist Specified distance.
	 *
	 * @return 1=yes, 0=no.  Uses fast distance approximation.
	 */
	static bool PointsAreNear( const geVector &Point1, const geVector &Point2, float Dist );

	/**
	 * Calculate the signed distance (in the direction of the normal) between
	 * a point and a plane.
	 *
	 * @param Point The Point we are checking.
	 * @param PlaneBase The Base Point in the plane.
	 * @param PlaneNormal The Normal of the plane.
	 *
	 * @return Signed distance  between point and plane.
	 */
	static float PointPlaneDist( const geVector &Point, const geVector &PlaneBase, const geVector &PlaneNormal );

	/**
	 * Calculate a the projection of a point on the plane defined by CCW points A,B,C
	 *
	 * @param Point - the point to project onto the plane
	 * @param A,B,C - three points in CCW order defining the plane 
	 *
	 * @return Projection of Point onto plane ABC
	 */
	static geVector PointPlaneProject(const geVector& Point, const geVector& A, const geVector& B, const geVector& C);

	/**
	* Calculate a the projection of a point on the plane defined by PlaneBase, and PlaneNormal
	*
	* @param Point - the point to project onto the plane
	* @param PlaneBase - point on the plane
	* @param PlaneNorm - normal of the plane
	*
	* @return Projection of Point onto plane ABC
	*/
	static geVector PointPlaneProject(const geVector& Point, const geVector& PlaneBase, const geVector& PlaneNorm);

	/**
	 * Euclidean distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 *
	 * @return The distance between two points.
	 */
	static FORCEINLINE float Dist( const geVector &V1, const geVector &V2 );

	/**
	 * Squared distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 *
	 * @return The squared distance between two points.
	 */
	static FORCEINLINE float DistSquared( const geVector &V1, const geVector &V2 );

	/**
	 * Compute pushout of a box from a plane.
	 *
	 * @param Normal The plane normal.
	 * @param Size The size of the box.
	 *
	 * @return Pushout required.
	 */
	static FORCEINLINE float BoxPushOut( const geVector & Normal, const geVector & Size );

	/**
	 * See if two normal vectors (or plane normals) are nearly parallel.
	 *
	 * @param Normal1 First normalized vector.
	 * @param Normal1 Second normalized vector.
	 *
	 * @return true if vectors are parallel, otherwise false.
	 */
	static bool Parallel( const geVector &Normal1, const geVector &Normal2 );

	/**
	 * See if two planes are coplanar.
	 *
	 * @param Base1 The base point in the first plane.
	 * @param Normal1 The normal of the first plane.
	 * @param Base2 The base point in the second plane.
	 * @param Normal2 The normal of the second plane.
	 *
	 * @return true if the planes are coplanar, otherwise false.
	 */
	static bool Coplanar( const geVector &Base1, const geVector &Normal1, const geVector &Base2, const geVector &Normal2 );

	/**
	 * Triple product of three vectors.
	 *
	 * @param X The first vector.
	 * @param Y The second vector.
	 * @param Z The third vector.
	 *
	 * @return The triple product.
	 */
	static float Triple( const geVector& X, const geVector& Y, const geVector& Z );

	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param	ControlPoints	Array of 4 FVectors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param	NumPoints		Number of samples.
	 * @param	OutPoints		Receives the output samples.
	 *
	 * @return					Path length.
	 */
	static CORE_API float EvaluateBezier(const geVector* ControlPoints, int32 NumPoints, TArray<geVector>& OutPoints);

	/**
	 * Given a current set of cluster centers, a set of points, iterate N times to move clusters to be central. 
	 *
	 * @param Clusters Reference to array of Clusters.
	 * @param Points Set of points.
	 * @param NumIterations Number of iterations.
	 * @param NumConnectionsToBeValid Sometimes you will have long strings that come off the mass of points
	 * which happen to have been chosen as Cluster starting points.  You want to be able to disregard those.
	 */
	static CORE_API void GenerateClusterCenters(TArray<geVector>& Clusters, const TArray<geVector>& Points, int32 NumIterations, int32 NumConnectionsToBeValid);

	/**
	 * Serializer.
	 *
	 * @param Ar Serialization Archive.
	 * @param V Vector to serialize.
	 *
	 * @return Reference to Archive after serialization.
	 */
	friend FArchive& operator<<( FArchive& Ar, geVector& V )
	{
		// @warning BulkSerialize: FVector is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		return Ar << V.X << V.Y << V.Z;
	}

	CORE_API bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	static CORE_API const geVector ZeroVector;
	static CORE_API const geVector UpVector;
};

/**
 * Multiplies a vector by a scaling factor.
 *
 * @param Scale Scaling factor.
 * @param V Vector to scale.
 *
 * @return Result of multiplication.
 */
FORCEINLINE geVector operator*( float Scale, const geVector& V )
{
	return V.operator*( Scale );
}

/**
 * Creates a hash value from a FVector. Uses pointers to the elements to
 * bypass any type conversion. This is a simple hash that just ORs the
 * raw 32bit data together
 *
 * @param Vector the vector to create a hash value for
 *
 * @return The hash value from the components
 */
FORCEINLINE uint32 GetTypeHash(const geVector& Vector)
{
	// Note: this assumes there's no padding in FVector that could contain uncompared data.
	return FCrc::MemCrc(&Vector,sizeof(geVector));
}

/**
 * Creates a hash value from a FVector2D. Uses pointers to the elements to
 * bypass any type conversion. This is a simple hash that just ORs the
 * raw 32bit data together
 *
 * @param Vector the vector to create a hash value for
 *
 * @return The hash value from the components
 */
FORCEINLINE uint32 GetTypeHash(const FVector2D& Vector)
{
	// Note: this assumes there's no padding in FVector2D that could contain uncompared data.
	return FCrc::MemCrc(&Vector,sizeof(FVector2D));
}


#if PLATFORM_LITTLE_ENDIAN
	#define INTEL_ORDER_VECTOR(x) (x)
#else
	static FORCEINLINE geVector INTEL_ORDER_VECTOR(geVector v)
	{
		return geVector(INTEL_ORDERF(v.X), INTEL_ORDERF(v.Y), INTEL_ORDERF(v.Z));
	}
#endif

/** 
 * Util to calculate distance from a point to a bounding box 
 *
 * @param Mins 3D Point defining the lower values of the axis of the bound box
 * @param Max 3D Point defining the lower values of the axis of the bound box
 * @param Point 3D position of interest
 * 
 * @return the distance from the Point to the bounding box.
 */
FORCEINLINE float ComputeSquaredDistanceFromBoxToPoint( const geVector& Mins, const geVector& Maxs, const geVector& Point )
{
	// Accumulates the distance as we iterate axis
	float DistSquared = 0.f;

	// Check each axis for min/max and add the distance accordingly
	// NOTE: Loop manually unrolled for > 2x speed up
	if (Point.X < Mins.X)
	{
		DistSquared += FMath::Square(Point.X - Mins.X);
	}
	else if (Point.X > Maxs.X)
	{
		DistSquared += FMath::Square(Point.X - Maxs.X);
	}
	
	if (Point.Y < Mins.Y)
	{
		DistSquared += FMath::Square(Point.Y - Mins.Y);
	}
	else if (Point.Y > Maxs.Y)
	{
		DistSquared += FMath::Square(Point.Y - Maxs.Y);
	}
	
	if (Point.Z < Mins.Z)
	{
		DistSquared += FMath::Square(Point.Z - Mins.Z);
	}
	else if (Point.Z > Maxs.Z)
	{
		DistSquared += FMath::Square(Point.Z - Maxs.Z);
	}
	
	return DistSquared;
}

FORCEINLINE geVector::geVector( const FVector2D V, float InZ )
	: X(V.X), Y(V.Y), Z(InZ)
{
}



inline geVector geVector::RotateAngleAxis( const float AngleDeg, const geVector& Axis ) const
{
	const float S	= FMath::Sin(AngleDeg * PI / 180.f);
	const float C	= FMath::Cos(AngleDeg * PI / 180.f);

	const float XX	= Axis.X * Axis.X;
	const float YY	= Axis.Y * Axis.Y;
	const float ZZ	= Axis.Z * Axis.Z;

	const float XY	= Axis.X * Axis.Y;
	const float YZ	= Axis.Y * Axis.Z;
	const float ZX	= Axis.Z * Axis.X;

	const float XS	= Axis.X * S;
	const float YS	= Axis.Y * S;
	const float ZS	= Axis.Z * S;

	const float OMC	= 1.f - C;

	return geVector(
		(OMC * XX + C ) * X + (OMC * XY - ZS) * Y + (OMC * ZX + YS) * Z,
		(OMC * XY + ZS) * X + (OMC * YY + C ) * Y + (OMC * YZ - XS) * Z,
		(OMC * ZX - YS) * X + (OMC * YZ + XS) * Y + (OMC * ZZ + C ) * Z
		);
}

inline bool geVector::PointsAreSame( const geVector &P, const geVector &Q )
{
	float Temp;
	Temp=P.X-Q.X;
	if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
	{
		Temp=P.Y-Q.Y;
		if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
		{
			Temp=P.Z-Q.Z;
			if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
			{
				return 1;
			}
		}
	}
	return 0;
}

inline bool geVector::PointsAreNear( const geVector &Point1, const geVector &Point2, float Dist )
{
	float Temp;
	Temp=(Point1.X - Point2.X); if (FMath::Abs(Temp)>=Dist) return 0;
	Temp=(Point1.Y - Point2.Y); if (FMath::Abs(Temp)>=Dist) return 0;
	Temp=(Point1.Z - Point2.Z); if (FMath::Abs(Temp)>=Dist) return 0;
	return 1;
}

inline float geVector::PointPlaneDist
(
	const geVector &Point,
	const geVector &PlaneBase,
	const geVector &PlaneNormal
)
{
	return (Point - PlaneBase) | PlaneNormal;
}


inline geVector geVector::PointPlaneProject(const geVector& Point, const geVector& PlaneBase, const geVector& PlaneNorm)
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - geVector::PointPlaneDist(Point,PlaneBase,PlaneNorm) * PlaneNorm;
}

inline bool geVector::Parallel( const geVector &Normal1, const geVector &Normal2 )
{
	const float NormalDot = Normal1 | Normal2;
	return (FMath::Abs(NormalDot - 1.f) <= THRESH_VECTORS_ARE_PARALLEL);
}

inline bool geVector::Coplanar( const geVector &Base1, const geVector &Normal1, const geVector &Base2, const geVector &Normal2 )
{
	if      (!geVector::Parallel(Normal1,Normal2)) return 0;
	else if (geVector::PointPlaneDist (Base2,Base1,Normal1) > THRESH_POINT_ON_PLANE) return 0;
	else    return 1;
}

inline float geVector::Triple( const geVector& X, const geVector& Y, const geVector& Z )
{
	return
	(	(X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
	+	(X.Y * (Y.Z * Z.X - Y.X * Z.Z))
	+	(X.Z * (Y.X * Z.Y - Y.Y * Z.X)) );
}

FORCEINLINE geVector::geVector()
{}

FORCEINLINE geVector::geVector(float InF)
	: X(InF), Y(InF), Z(InF)
{}

FORCEINLINE geVector::geVector( float InX, float InY, float InZ )
	: X(InX), Y(InY), Z(InZ)
{}

FORCEINLINE geVector::geVector(const geLinearColor& InColor)
	: X(InColor.R), Y(InColor.G), Z(InColor.B)
{}

FORCEINLINE geVector::geVector(FIntVector InVector)
	: X(InVector.X), Y(InVector.Y), Z(InVector.Z)
{}

FORCEINLINE geVector::geVector( FIntPoint A )
	: X(A.X), Y(A.Y), Z(0.f)
{}

FORCEINLINE geVector::geVector(EForceInit)
	: X(0.0f), Y(0.0f), Z(0.0f)
{}

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
FORCEINLINE geVector& geVector::operator=(const geVector& Other)
{
	this->X = Other.X;
	this->Y = Other.Y;
	this->Z = Other.Z;

	return *this;
}
#endif

FORCEINLINE geVector geVector::operator^( const geVector& V ) const
{
	return geVector
		(
		Y * V.Z - Z * V.Y,
		Z * V.X - X * V.Z,
		X * V.Y - Y * V.X
		);
}

FORCEINLINE geVector geVector::CrossProduct( const geVector& A, const geVector& B )
{
	return A ^ B;
}

FORCEINLINE float geVector::operator|( const geVector& V ) const
{
	return X*V.X + Y*V.Y + Z*V.Z;
}

FORCEINLINE float geVector::DotProduct( const geVector& A, const geVector& B )
{
	return A | B;
}

FORCEINLINE geVector geVector::operator+( const geVector& V ) const
{
	return geVector( X + V.X, Y + V.Y, Z + V.Z );
}

FORCEINLINE geVector geVector::operator-( const geVector& V ) const
{
	return geVector( X - V.X, Y - V.Y, Z - V.Z );
}

FORCEINLINE geVector geVector::operator-( float Bias ) const
{
	return geVector( X - Bias, Y - Bias, Z - Bias );
}

FORCEINLINE geVector geVector::operator+( float Bias ) const
{
	return geVector( X + Bias, Y + Bias, Z + Bias );
}

FORCEINLINE geVector geVector::operator*( float Scale ) const
{
	return geVector( X * Scale, Y * Scale, Z * Scale );
}

FORCEINLINE geVector geVector::operator/( float Scale ) const
{
	const float RScale = 1.f/Scale;
	return geVector( X * RScale, Y * RScale, Z * RScale );
}

FORCEINLINE geVector geVector::operator*( const geVector& V ) const
{
	return geVector( X * V.X, Y * V.Y, Z * V.Z );
}

FORCEINLINE geVector geVector::operator/( const geVector& V ) const
{
	return geVector( X / V.X, Y / V.Y, Z / V.Z );
}

FORCEINLINE bool geVector::operator==( const geVector& V ) const
{
	return X==V.X && Y==V.Y && Z==V.Z;
}

FORCEINLINE bool geVector::operator!=( const geVector& V ) const
{
	return X!=V.X || Y!=V.Y || Z!=V.Z;
}

FORCEINLINE bool geVector::Equals(const geVector& V, float Tolerance) const
{
	return FMath::Abs(X-V.X) < Tolerance && FMath::Abs(Y-V.Y) < Tolerance && FMath::Abs(Z-V.Z) < Tolerance;
}

FORCEINLINE bool geVector::AllComponentsEqual(float Tolerance) const
{
	return FMath::Abs( X - Y ) < Tolerance && FMath::Abs( X - Z ) < Tolerance && FMath::Abs( Y - Z ) < Tolerance;
}


FORCEINLINE geVector geVector::operator-() const
{
	return geVector( -X, -Y, -Z );
}


FORCEINLINE geVector geVector::operator+=( const geVector& V )
{
	X += V.X; Y += V.Y; Z += V.Z;
	return *this;
}

FORCEINLINE geVector geVector::operator-=( const geVector& V )
{
	X -= V.X; Y -= V.Y; Z -= V.Z;
	return *this;
}

FORCEINLINE geVector geVector::operator*=( float Scale )
{
	X *= Scale; Y *= Scale; Z *= Scale;
	return *this;
}

FORCEINLINE geVector geVector::operator/=( float V )
{
	const float RV = 1.f/V;
	X *= RV; Y *= RV; Z *= RV;
	return *this;
}

FORCEINLINE geVector geVector::operator*=( const geVector& V )
{
	X *= V.X; Y *= V.Y; Z *= V.Z;
	return *this;
}

FORCEINLINE geVector geVector::operator/=( const geVector& V )
{
	X /= V.X; Y /= V.Y; Z /= V.Z;
	return *this;
}

FORCEINLINE float& geVector::operator[]( int32 Index )
{
	check(Index >= 0 && Index < 3);
	if( Index == 0 )
	{
		return X;
	}
	else if( Index == 1)
	{
		return Y;
	}
	else
	{
		return Z;
	}
}

FORCEINLINE float geVector::operator[]( int32 Index )const
{
	check(Index >= 0 && Index < 3);
	if( Index == 0 )
	{
		return X;
	}
	else if( Index == 1)
	{
		return Y;
	}
	else
	{
		return Z;
	}
}

FORCEINLINE void geVector::Set( float InX, float InY, float InZ )
{
	X = InX;
	Y = InY;
	Z = InZ;
}

FORCEINLINE float geVector::GetMax() const
{
	return FMath::Max(FMath::Max(X,Y),Z);
}

FORCEINLINE float geVector::GetAbsMax() const
{
	return FMath::Max(FMath::Max(FMath::Abs(X),FMath::Abs(Y)),FMath::Abs(Z));
}

FORCEINLINE float geVector::GetMin() const
{
	return FMath::Min(FMath::Min(X,Y),Z);
}

FORCEINLINE float geVector::GetAbsMin() const
{
	return FMath::Min(FMath::Min(FMath::Abs(X),FMath::Abs(Y)),FMath::Abs(Z));
}

FORCEINLINE geVector geVector::ComponentMin(const geVector& Other) const
{
	return geVector(FMath::Min(X, Other.X), FMath::Min(Y, Other.Y), FMath::Min(Z, Other.Z));
}

FORCEINLINE geVector geVector::ComponentMax(const geVector& Other) const
{
	return geVector(FMath::Max(X, Other.X), FMath::Max(Y, Other.Y), FMath::Max(Z, Other.Z));
}

FORCEINLINE geVector geVector::GetAbs() const
{
	return geVector(FMath::Abs(X), FMath::Abs(Y), FMath::Abs(Z));
}

FORCEINLINE float geVector::Size() const
{
	return FMath::Sqrt( X*X + Y*Y + Z*Z );
}

FORCEINLINE float geVector::SizeSquared() const
{
	return X*X + Y*Y + Z*Z;
}

FORCEINLINE float geVector::Size2D() const 
{
	return FMath::Sqrt( X*X + Y*Y );
}

FORCEINLINE float geVector::SizeSquared2D() const 
{
	return X*X + Y*Y;
}

FORCEINLINE bool geVector::IsNearlyZero(float Tolerance) const
{
	return
		FMath::Abs(X)<Tolerance
		&&	FMath::Abs(Y)<Tolerance
		&&	FMath::Abs(Z)<Tolerance;
}

FORCEINLINE bool geVector::IsZero() const
{
	return X==0.f && Y==0.f && Z==0.f;
}

FORCEINLINE bool geVector::Normalize(float Tolerance)
{
	const float SquareSum = X*X + Y*Y + Z*Z;
	if( SquareSum > Tolerance )
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}
	return false;
}

FORCEINLINE bool geVector::IsNormalized() const
{
	return (FMath::Abs(1.f - SizeSquared()) <= 0.01f);
}

FORCEINLINE void geVector::ToDirectionAndLength(geVector &OutDir, float &OutLength)
{
	OutLength = Size();
	if (OutLength > SMALL_NUMBER)
	{
		float OneOverLength = 1.0f/OutLength;
		OutDir = geVector(X*OneOverLength, Y*OneOverLength,
			Z*OneOverLength);
	}
	else
	{
		OutDir = geVector::ZeroVector;
	}
}

FORCEINLINE geVector geVector::GetSignVector()
{
	return geVector
		(
		FMath::FloatSelect(X, 1.f, -1.f), 
		FMath::FloatSelect(Y, 1.f, -1.f), 
		FMath::FloatSelect(Z, 1.f, -1.f)
		);
}

FORCEINLINE geVector geVector::Projection() const
{
	const float RZ = 1.f/Z;
	return geVector( X*RZ, Y*RZ, 1 );
}

FORCEINLINE geVector geVector::UnsafeNormal() const
{
	const float Scale = FMath::InvSqrt(X*X+Y*Y+Z*Z);
	return geVector( X*Scale, Y*Scale, Z*Scale );
}

FORCEINLINE geVector geVector::GridSnap( const float& GridSz ) const
{
	return geVector( FMath::GridSnap(X, GridSz),FMath::GridSnap(Y, GridSz),FMath::GridSnap(Z, GridSz) );
}

FORCEINLINE geVector geVector::BoundToCube( float Radius ) const
{
	return geVector
		(
		FMath::Clamp(X,-Radius,Radius),
		FMath::Clamp(Y,-Radius,Radius),
		FMath::Clamp(Z,-Radius,Radius)
		);
}

FORCEINLINE void geVector::AddBounded( const geVector& V, float Radius )
{
	*this = (*this + V).BoundToCube(Radius);
}

FORCEINLINE float& geVector::Component( int32 Index )
{
	return (&X)[Index];
}

FORCEINLINE float geVector::Component( int32 Index ) const
{
	return (&X)[Index];
}

FORCEINLINE geVector geVector::Reciprocal() const
{
	geVector RecVector;
	if (X!=0.f)
	{
		RecVector.X = 1.f/X;
	}
	else 
	{
		RecVector.X = BIG_NUMBER;
	}
	if (Y!=0.f)
	{
		RecVector.Y = 1.f/Y;
	}
	else 
	{
		RecVector.Y = BIG_NUMBER;
	}
	if (Z!=0.f)
	{
		RecVector.Z = 1.f/Z;
	}
	else 
	{
		RecVector.Z = BIG_NUMBER;
	}

	return RecVector;
}

FORCEINLINE bool geVector::IsUniform(float Tolerance) const
{
	return (FMath::Abs(X-Y) < Tolerance) && (FMath::Abs(Y-Z) < Tolerance);
}

FORCEINLINE geVector geVector::MirrorByVector( const geVector& MirrorNormal ) const
{
	return *this - MirrorNormal * (2.f * (*this | MirrorNormal));
}

FORCEINLINE geVector geVector::SafeNormal(float Tolerance) const
{
	const float SquareSum = X*X + Y*Y + Z*Z;

	// Not sure if it's safe to add tolerance in there. Might introduce too many errors
	if( SquareSum == 1.f )
	{
		return *this;
	}		
	else if( SquareSum < Tolerance )
	{
		return geVector::ZeroVector;
	}
	const float Scale = FMath::InvSqrt(SquareSum);
	return geVector(X*Scale, Y*Scale, Z*Scale);
}

FORCEINLINE geVector geVector::SafeNormal2D(float Tolerance) const
{
	const float SquareSum = X*X + Y*Y;

	// Not sure if it's safe to add tolerance in there. Might introduce too many errors
	if( SquareSum == 1.f )
	{
		if( Z == 0.f )
		{
			return *this;
		}
		else
		{
			return geVector(X, Y, 0.f);
		}
	}
	else if( SquareSum < Tolerance )
	{
		return geVector::ZeroVector;
	}

	const float Scale = FMath::InvSqrt(SquareSum);
	return geVector(X*Scale, Y*Scale, 0.f);
}

FORCEINLINE float geVector::Dot2d(geVector B) const
{
	geVector A(*this);
	A.Z = 0.0f;
	B.Z = 0.0f;
	A.Normalize();
	B.Normalize();
	return A | B;
}

FORCEINLINE geVector geVector::ProjectOnTo( const geVector& A ) const 
{ 
	return (A * ((*this | A) / (A | A))); 
}


FORCEINLINE bool geVector::ContainsNaN() const
{
	return (FMath::IsNaN(X) || !FMath::IsFinite(X) || 
		FMath::IsNaN(Y) || !FMath::IsFinite(Y) ||
		FMath::IsNaN(Z) || !FMath::IsFinite(Z));
}

FORCEINLINE bool geVector::IsUnit(float LengthSquaredTolerance ) const
{
	return FMath::Abs(1.0f - SizeSquared()) < LengthSquaredTolerance;
}

FORCEINLINE FString geVector::ToString() const
{
	return FString::Printf(TEXT("X=%3.3f Y=%3.3f Z=%3.3f"), X, Y, Z);
}

FORCEINLINE bool geVector::InitFromString( const FString & InSourceString )
{
	X = Y = Z = 0;

	// The initialization is only successful if the X, Y, and Z values can all be parsed from the string
	const bool bSuccessful = FParse::Value( *InSourceString, TEXT("X=") , X ) && FParse::Value( *InSourceString, TEXT("Y="), Y ) && FParse::Value( *InSourceString, TEXT("Z="), Z );

	return bSuccessful;
}

FORCEINLINE FVector2D geVector::UnitCartesianToSpherical() const
{
	checkSlow(IsUnit());
	const float Theta = FMath::Acos(Z / Size());
	const float Phi = FMath::Atan2(Y, X);
	return FVector2D(Theta, Phi);
}

FORCEINLINE float geVector::HeadingAngle() const
{
	// Project Dir into Z plane.
	geVector PlaneDir = *this;
	PlaneDir.Z = 0.f;
	PlaneDir = PlaneDir.SafeNormal();

	float Angle = FMath::Acos(PlaneDir.X);

	if(PlaneDir.Y < 0.0f)
	{
		Angle *= -1.0f;
	}

	return Angle;
}



FORCEINLINE float geVector::Dist( const geVector &V1, const geVector &V2 )
{
	return FMath::Sqrt( FMath::Square(V2.X-V1.X) + FMath::Square(V2.Y-V1.Y) + FMath::Square(V2.Z-V1.Z) );
}

FORCEINLINE float geVector::DistSquared( const geVector &V1, const geVector &V2 )
{
	return FMath::Square(V2.X-V1.X) + FMath::Square(V2.Y-V1.Y) + FMath::Square(V2.Z-V1.Z);
}

FORCEINLINE float geVector::BoxPushOut( const geVector & Normal, const geVector & Size )
{
	return FMath::Abs(Normal.X*Size.X) + FMath::Abs(Normal.Y*Size.Y) + FMath::Abs(Normal.Z*Size.Z);
}

template <> struct TIsPODType<geVector> { enum { Value = true }; };
