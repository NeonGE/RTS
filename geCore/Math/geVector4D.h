// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once


/**
* A 4D homogeneous vector, 4x1 FLOATs, 16-byte aligned.
*/
MS_ALIGN(16) struct FVector4
{
public:


public:

	

public:

	

public:

	

	/**
	* Return the FRotator corresponding to the direction that the vector
	* is pointing in.  Sets Yaw and Pitch to the proper numbers, and sets
	* roll to zero because the roll can't be determined from a vector.
	*
	* @return The FRotator of the vector's direction.
	*/
	CORE_API FRotator Rotation() const;

	

public:

} GCC_ALIGN(16);


/**
* Creates a hash value from a FVector4.
*
* @param Vector the vector to create a hash value for
*
* @return The hash value from the components
*/
FORCEINLINE uint32 GetTypeHash(const FVector4& Vector)
{
	// Note: this assumes there's no padding in FVector that could contain uncompared data.
	return FCrc::MemCrc_DEPRECATED(&Vector, sizeof(Vector));
}


/* FVector4 inline functions
*****************************************************************************/


