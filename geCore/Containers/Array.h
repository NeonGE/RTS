#pragma once

#define ARRAY_PARENS 0
#define ARRAY_BRACKETS 1
#define DEBUG_HEAP 0


/**
 * Generic non-const iterator which can operate on types that expose the following:
 * - A type called ElementType representing the contained type.
 * - A method IndexType Num() const that returns the number of items in the container.
 * - A method bool IsValidIndex(IndexType index) which returns whether a given index is valid in the container.
 * - A method T& operator(IndexType index) which returns a reference to a contained object by index.
 */
template< typename ContainerType, typename IndexType = int32 >
class TIndexedContainerIterator
{
public:
	typedef typename ContainerType::ElementType ElementType;

	TIndexedContainerIterator(ContainerType& InContainer)
		:	Container( InContainer )
		,	Index(0)
	{
	}

	TIndexedContainerIterator(const TIndexedContainerIterator& Other)
		:	Container( Other.Container )
		,	Index( Other.Index )
	{
	}

	/** Advances iterator to the next element in the container. */
	TIndexedContainerIterator& operator++()
	{
		++Index;
		return *this;
	}
	TIndexedContainerIterator operator++(int)
	{
		TIndexedContainerIterator Tmp(*this);
		++Index;
		return Tmp;
	}

	/** Moves iterator to the previous element in the container. */
	TIndexedContainerIterator& operator--()
	{
		--Index;
		return *this;
	}
	TIndexedContainerIterator operator--(int)
	{
		TIndexedContainerIterator Tmp(*this);
		--Index;
		return Tmp;
	}

	/** pointer arithmetic support */
	TIndexedContainerIterator& operator+=(int32 Offset)
	{
		Index += Offset;
		return *this;
	}

	TIndexedContainerIterator operator+(int32 Offset) const
	{
		TIndexedContainerIterator Tmp(*this);
		return Tmp += Offset;
	}

	TIndexedContainerIterator& operator-=(int32 Offset)
	{
		return *this += -Offset;
	}

	TIndexedContainerIterator operator-(int32 Offset) const
	{
		TIndexedContainerIterator Tmp(*this);
		return Tmp -= Offset;
	}

	TIndexedContainerIterator operator()(int32 Offset) const
	{
		return *this + Offset;
	}

	/** @name Element access */
	//@{
	ElementType& operator* () const
	{
		return Container[ Index ];
	}

	ElementType* operator-> () const
	{
		return &Container[ Index ];
	}
	//@}

	/** conversion to "bool" returning true if the iterator has not reached the last element. */
	typedef bool PrivateBooleanType;
	operator PrivateBooleanType() const
	{
		return Container.IsValidIndex(Index) ? &TIndexedContainerIterator::Index : false;
	}

	/** inverse of the "bool" operator */
	bool operator !() const
	{
		return !operator PrivateBooleanType();
	}

	/** Returns an index to the current element. */
	IndexType GetIndex() const
	{
		return Index;
	}

	/** Resets the iterator to the first element. */
	void Reset()
	{
		Index = 0;
	}
private:
	ContainerType&	Container;
	IndexType		Index;
};

/** operator + */
template< typename ContainerType, typename IndexType >
TIndexedContainerIterator<ContainerType, IndexType> operator+(
	int32 Offset,
	TIndexedContainerIterator<ContainerType, IndexType> RHS)
{
	return RHS + Offset;
}



/**
* Generic const iterator which can operate on types that expose the following:
* - A type called ElementType representing the contained type.
* - A method IndexType Num() const that returns the number of items in the container.
* - A method bool IsValidIndex(IndexType index) which returns whether a given index is valid in the container.
* - A method T& operator(IndexType index) const which returns a reference to a contained object by index.
*/
template< typename ContainerType, typename IndexType = int32 >
class TIndexedContainerConstIterator
{
public:
	typedef typename ContainerType::ElementType ElementType;

	TIndexedContainerConstIterator(const ContainerType& InContainer)
		:	Container( InContainer )
		,	Index(0)
	{
	}

	TIndexedContainerConstIterator(const TIndexedContainerConstIterator& Other)
		:	Container( Other.Container )
		,	Index( Other.Index )
	{
	}

	/** Advances iterator to the next element in the container. */
	TIndexedContainerConstIterator& operator++()
	{
		++Index;
		return *this;
	}
	TIndexedContainerConstIterator operator++(int)
	{
		TIndexedContainerConstIterator Tmp(*this);
		++Index;
		return Tmp;
	}

	/** Moves iterator to the previous element in the container. */
	TIndexedContainerConstIterator& operator--()
	{
		--Index;
		return *this;
	}
	TIndexedContainerConstIterator operator--(int)
	{
		TIndexedContainerConstIterator Tmp(*this);
		--Index;
		return Tmp;
	}

	/** iterator arithmetic support */
	TIndexedContainerConstIterator& operator+=(int32 Offset)
	{
		Index += Offset;
		return *this;
	}

	TIndexedContainerConstIterator operator+(int32 Offset) const
	{
		TIndexedContainerConstIterator Tmp(*this);
		return Tmp += Offset;
	}

	TIndexedContainerConstIterator& operator-=(int32 Offset)
	{
		return *this += -Offset;
	}

	TIndexedContainerConstIterator operator-(int32 Offset) const
	{
		TIndexedContainerConstIterator Tmp(*this);
		return Tmp -= Offset;
	}

	TIndexedContainerConstIterator operator()(int32 Offset) const
	{
		return *this + Offset;
	}

	/** @name Element access */
	//@{
	const ElementType& operator* () const
	{
		return Container[ Index ];
	}

	const ElementType* operator-> () const
	{
		return &Container[ Index ];
	}
	//@}

	/** conversion to "bool" returning true if the iterator has not reached the last element. */
	typedef bool PrivateBooleanType;
	operator PrivateBooleanType() const
	{
		return Container.IsValidIndex(Index) ? &TIndexedContainerConstIterator::Index : false;
	}

	/** inverse of the "bool" operator */
	bool operator !() const
	{
		return !operator PrivateBooleanType();
	}

	/** Returns an index to the current element. */
	IndexType GetIndex() const
	{
		return Index;
	}

	/** Resets the iterator to the first element. */
	void Reset()
	{
		Index = 0;
	}
private:
	const ContainerType&	Container;
	IndexType				Index;
};

/** operator + */
template< typename ContainerType, typename IndexType >
TIndexedContainerConstIterator<ContainerType, IndexType> operator+(
	int32 Offset,
	TIndexedContainerConstIterator<ContainerType, IndexType> RHS)
{
	return RHS + Offset;
}

template<typename InElementType, typename Allocator>
class TArray
{
public:
	typedef InElementType ElementType;

	TArray()
	:   ArrayNum( 0 )
	,	ArrayMax( 0 )
	{}
	/** Caution, this will create elements without calling the constructor and this is not appropriate for element types that require a constructor to function properly. */
	explicit TArray( int32 InNum )
	:   ArrayNum( InNum )
	,	ArrayMax( InNum )
	{
		AllocatorInstance.ResizeAllocation(0,ArrayMax,sizeof(ElementType));
	}

	/**
	 * Copy constructor. Use the common routine to perform the copy
	 *
	 * @param Other the source array to copy
	 */
	template<typename OtherAllocator>
	explicit TArray(const TArray<ElementType,OtherAllocator>& Other)
	:   ArrayNum( 0 )
	,	ArrayMax( 0 )
	{
		Copy(Other);
	}

	TArray(const TArray<ElementType,Allocator>& Other)
	:   ArrayNum( 0 )
	,	ArrayMax( 0 )
	{
		Copy(Other);
	}

	~TArray()
	{
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);
		DestructItems(0,ArrayNum);
		ArrayNum = ArrayMax = 0;

		#if defined(_MSC_VER)
			// ensure that DebugGet gets instantiated.
			//@todo it would be nice if we had a cleaner solution for DebugGet
			volatile const ElementType* Dummy = &DebugGet(0);
		#endif
	}

	/**
	 * Helper function for returning a typed pointer to the first array entry.
	 *
	 * @return pointer to first array entry or NULL if ArrayMax==0
	 */
	FORCEINLINE ElementType* GetTypedData()
	{
		return (ElementType*)AllocatorInstance.GetAllocation();
	}
	FORCEINLINE ElementType* GetData()
	{
		return (ElementType*)AllocatorInstance.GetAllocation();
	}
	/**
	 * Helper function for returning a typed pointer to the first array entry.
	 *
	 * @return pointer to first array entry or NULL if ArrayMax==0
	 */
	FORCEINLINE const ElementType* GetTypedData() const
	{
		return (const ElementType*)AllocatorInstance.GetAllocation();
	}
	FORCEINLINE const ElementType* GetData() const
	{
		return (const ElementType*)AllocatorInstance.GetAllocation();
	}
	/** 
	 * Helper function returning the size of the inner type
	 *
	 * @return size in bytes of array type
	 */
	FORCEINLINE uint32 GetTypeSize() const
	{
		return sizeof(ElementType);
	}

	/** 
	 * Helper function to return the amount of memory allocated by this container 
	 *
	 * @return number of bytes allocated by this container
	 */
	FORCEINLINE uint32 GetAllocatedSize( void ) const
	{
		return AllocatorInstance.GetAllocatedSize(ArrayMax, sizeof(ElementType));
	}

	/**
	 * Returns the amount of slack in this array in elements.
	 */
	int32 GetSlack() const
	{
		return ArrayMax - ArrayNum;
	}

	FORCEINLINE void CheckInvariants() const
	{
		checkSlow((ArrayNum >= 0) & (ArrayMax >= ArrayNum)); // & for one branch
	}
	FORCEINLINE void RangeCheck(int32 Index) const
	{
		CheckInvariants();
		checkf((Index >= 0) & ((Index < ArrayNum) | ((Index == 0) & (ArrayNum==0) )),TEXT("Array index out of bounds: %i from an array of size %i"),Index,ArrayNum); // & and | for one branch
	}

	FORCEINLINE bool IsValidIndex( int32 i ) const
	{
		return i>=0 && i<ArrayNum;
	}
	FORCEINLINE int32 Num() const
	{
		return ArrayNum;
	}

#if ARRAY_PARENS
	FORCEINLINE ElementType& operator()( int32 i )
	{
		RangeCheck(i);
		return GetTypedData()[i];
	}
	FORCEINLINE const ElementType& operator()( int32 i ) const
	{
		RangeCheck(i);
		return GetTypedData()[i];
	}
#endif
#if ARRAY_BRACKETS
	FORCEINLINE ElementType& operator[]( int32 i )
	{
		RangeCheck(i);
		return GetTypedData()[i];
	}
	FORCEINLINE const ElementType& operator[]( int32 i ) const
	{
		RangeCheck(i);
		return GetTypedData()[i];
	}
#endif
	ElementType Pop()
	{
		RangeCheck(0);
		ElementType Result = GetTypedData()[ArrayNum-1];
		RemoveAt( ArrayNum-1 );
		return Result;
	}
	void Push( const ElementType& Item )
	{
		Add(Item);
	}
	ElementType& Top()
	{
		return Last();
	}
	const ElementType& Top() const
	{
		return Last();
	}
	ElementType& Last( int32 c=0 )
	{
		RangeCheck(ArrayNum-c-1);
		return GetTypedData()[ArrayNum-c-1];
	}
	const ElementType& Last( int32 c=0 ) const
	{
		RangeCheck(ArrayNum-c-1);
		return GetTypedData()[ArrayNum-c-1];
	}
	void Shrink()
	{
		CheckInvariants();
		if( ArrayMax != ArrayNum )
		{
			ArrayMax = ArrayNum;
			AllocatorInstance.ResizeAllocation(ArrayNum,ArrayMax,sizeof(ElementType));
		}
	}
	bool Find( const ElementType& Item, int32& Index ) const
	{
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Item )
			{
				Index = (int32)(Data - GetTypedData());
				return true;
			}
		}
		return false;
	}
	int32 Find( const ElementType& Item ) const
	{
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Item )
			{
				return (int32)(Data - GetTypedData());
			}
		}
		return INDEX_NONE;
	}
	template<typename MatchFunctorType>
	int32 FindMatch( const MatchFunctorType& Matcher ) const
	{
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( Matcher.Matches(*Data) )
			{
				return (int32)(Data - GetTypedData());
			}
		}
		return INDEX_NONE;
	}
	/**
	 * Finds an item by key (assuming the ElementType overloads operator== for the comparison).
	 * @param Key	The key to search by
	 * @return		Index to the first matching element, or INDEX_NONE if none is found
	 */
	template <typename KeyType>
	int32 IndexOfByKey( const KeyType& Key ) const
	{
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Key )
			{
				return (int32)(Data - GetTypedData());
			}
		}
		return INDEX_NONE;
	}
	/**
	 * Finds an item by key (assuming the ElementType overloads operator== for the comparison).
	 * @param Key	The key to search by
	 * @return		Pointer to the first matching element, or NULL if none is found
	 */
	template <typename KeyType>
	const ElementType* FindByKey( const KeyType& Key ) const
	{
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Key )
			{
				return Data;
			}
		}
		return NULL;
	}
	/**
	 * Finds an item by key (assuming the ElementType overloads operator== for the comparison).
	 * @param Key	The key to search by
	 * @return		Pointer to the first matching element, or NULL if none is found
	 */
	template <typename KeyType>
	ElementType* FindByKey( const KeyType& Key )
	{
		ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Key )
			{
				return Data;
			}
		}
		return NULL;
	}
	/**
	 * @return		true if found
	 */
	bool Contains( const ElementType& Item ) const
	{
		// this function is performance critical
		// not calling Find() saves a subtract and divide (or shift)
		const ElementType* const RESTRICT DataEnd = GetTypedData() + ArrayNum;
		for(const ElementType* RESTRICT Data = GetTypedData();
			Data < DataEnd;
			Data++
			)
		{
			if( *Data==Item )
			{
				return true;
			}
		}
		return false;
	}
	bool operator==(const TArray& OtherArray) const
	{
		if(Num() != OtherArray.Num())
			return false;
		for(int32 Index = 0;Index < Num();Index++)
		{
			if(!((*this)[Index] == OtherArray[Index]))
				return false;
		}
		return true;
	}
	bool operator!=(const TArray& OtherArray) const
	{
		if(Num() != OtherArray.Num())
			return true;
		for(int32 Index = 0;Index < Num();Index++)
		{
			if(!((*this)[Index] == OtherArray[Index]))
				return true;
		}
		return false;
	}

	// Add, Insert, Remove, Empty interface.
	/** Caution, Add() will create elements without calling the constructor and this is not appropriate for element types that require a constructor to function properly. */
	int32 AddUninitialized( int32 Count=1 )
	{
		CheckInvariants();
		checkSlow(Count>=0);

		const int32 OldNum = ArrayNum;
		if( (ArrayNum+=Count)>ArrayMax )
		{
			ArrayMax = AllocatorInstance.CalculateSlack( ArrayNum, ArrayMax, sizeof(ElementType) );
			AllocatorInstance.ResizeAllocation(OldNum,ArrayMax, sizeof(ElementType));
		}

		return OldNum;
	}
	/** Caution, Insert() will create elements without calling the constructor and this is not appropriate for element types that require a constructor to function properly. */
	void InsertUninitialized( int32 Index, int32 Count=1 )
	{
		CheckInvariants();
		checkSlow((Count>=0) & (Index>=0) & (Index<=ArrayNum));

		const int32 OldNum = ArrayNum;
		if( (ArrayNum+=Count)>ArrayMax )
		{
			ArrayMax = AllocatorInstance.CalculateSlack( ArrayNum, ArrayMax, sizeof(ElementType) );
			AllocatorInstance.ResizeAllocation(OldNum,ArrayMax,sizeof(ElementType));
		}
		FMemory::Memmove
		(
			(uint8*)AllocatorInstance.GetAllocation() + (Index+Count )*sizeof(ElementType),
			(uint8*)AllocatorInstance.GetAllocation() + (Index       )*sizeof(ElementType),
			                                               (OldNum-Index)*sizeof(ElementType)
		);
	}
	/** Caution, InsertZeroed() will create elements without calling the constructor and this is not appropriate for element types that require a constructor to function properly. */
	void InsertZeroed( int32 Index, int32 Count=1 )
	{
		InsertUninitialized( Index, Count );
		FMemory::Memzero( (uint8*)AllocatorInstance.GetAllocation()+Index*sizeof(ElementType), Count*sizeof(ElementType) );
	}
	int32 Insert( const ElementType& Item, int32 Index )
	{
		// It isn't valid to specify an Item that is in the array, since adding an item might resize the array, which would make the item invalid
		check( ((&Item) < GetTypedData()) || ((&Item) >= GetTypedData()+ArrayMax) );
		// construct a copy in place at Index (this new operator will insert at 
		// Index, then construct that memory with Item)
		InsertUninitialized(Index,1);
		new(GetTypedData() + Index) ElementType(Item);
		return Index;
	}
	void RemoveAt( int32 Index, int32 Count=1, bool bAllowShrinking=true )
	{
		CheckInvariants();
		checkSlow((Count >= 0) & (Index >= 0) & (Index+Count <= ArrayNum));

		DestructItems(Index,Count);

		// Skip memmove in the common case that there is nothing to move.
		int32 NumToMove = ArrayNum - Index - Count;
		if( NumToMove )
		{
			FMemory::Memmove
			(
				(uint8*)AllocatorInstance.GetAllocation() + (Index      ) * sizeof(ElementType),
				(uint8*)AllocatorInstance.GetAllocation() + (Index+Count) * sizeof(ElementType),
				NumToMove * sizeof(ElementType)
			);
		}
		ArrayNum -= Count;
		
		const int32 NewArrayMax = AllocatorInstance.CalculateSlack(ArrayNum,ArrayMax,sizeof(ElementType));
		if(NewArrayMax != ArrayMax && bAllowShrinking)
		{
			ArrayMax = NewArrayMax;
			AllocatorInstance.ResizeAllocation(ArrayNum,ArrayMax,sizeof(ElementType));
		}
	}
	// RemoveSwap, this version is much more efficient O(Count) instead of O(ArrayNum), but does not preserve the order
	void RemoveAtSwap( int32 Index, int32 Count=1, bool bAllowShrinking=true)
	{
		CheckInvariants();
		checkSlow((Count >= 0) & (Index >= 0) & (Index+Count <= ArrayNum));

		DestructItems(Index,Count);
		
		// Replace the elements in the hole created by the removal with elements from the end of the array, so the range of indices used by the array is contiguous.
		const int32 NumElementsInHole = Count;
		const int32 NumElementsAfterHole = ArrayNum - (Index + Count);
		const int32 NumElementsToMoveIntoHole = FMath::Min(NumElementsInHole,NumElementsAfterHole);
		if(NumElementsToMoveIntoHole)
		{
			FMemory::Memcpy(
				(uint8*)AllocatorInstance.GetAllocation() + (Index                             ) * sizeof(ElementType),
				(uint8*)AllocatorInstance.GetAllocation() + (ArrayNum-NumElementsToMoveIntoHole) * sizeof(ElementType),
				NumElementsToMoveIntoHole * sizeof(ElementType)
				);
		}
		ArrayNum -= Count;

		const int32 NewArrayMax = AllocatorInstance.CalculateSlack(ArrayNum,ArrayMax,sizeof(ElementType));
		if(NewArrayMax != ArrayMax && bAllowShrinking)
		{
			ArrayMax = NewArrayMax;
			AllocatorInstance.ResizeAllocation(ArrayNum,ArrayMax,sizeof(ElementType));
		}
	}

	void Empty( int32 Slack=0 )
	{
		DestructItems(0,ArrayNum);

		checkSlow(Slack>=0);
		ArrayNum = 0;
		// only reallocate if we need to, I don't trust realloc to the same size to work
		if (ArrayMax != Slack)
		{
			ArrayMax = Slack;
			AllocatorInstance.ResizeAllocation(0,ArrayMax,sizeof(ElementType));
		}
	}

	void SetNum( int32 NewNum )
	{
		if (NewNum > Num())
		{
			AddZeroed(NewNum-Num());
		}
		else if (NewNum < Num())
		{
			RemoveAt(NewNum, Num() - NewNum);
		}
	}

	/**
	 * Appends the specified array to this array.
	 * Cannot append to self.
	 */
	template<typename OtherAllocator>
	FORCEINLINE void Append(const TArray<InElementType,OtherAllocator>& Source)
	{
		// Do nothing if the source and target match, or the source is empty.
		if ( (void*)this != (void*)&Source && Source.Num() > 0 )
		{
			// Allocate memory for the new elements.
			Reserve( ArrayNum + Source.Num() );

			if ( TContainerTraits<ElementType>::NeedsConstructor )
			{
				// Construct each element.
				for ( int32 Index = 0 ; Index < Source.Num() ; ++Index )
				{
					new(GetTypedData() + ArrayNum + Index) ElementType(Source[Index]);
				}
			}
			else
			{
				// Do a bulk copy.
				FMemory::Memcpy( (uint8*)(GetTypedData() + Num()), (uint8*)Source.GetTypedData(), sizeof(ElementType) * Source.Num() );
			}
			ArrayNum += Source.Num();
		}
	}

	/**
	 * Appends the specified array to this array.
	 * Cannot append to self.
	 */
	TArray& operator+=( const TArray& Other )
	{
		Append( Other );
		return *this;
	}

	/**
	 * Copies the source array into this one. Uses the common copy method
	 *
	 * @param Other the source array to copy
	 */
	template<typename OtherAllocator>
	TArray& operator=( const TArray<ElementType,OtherAllocator>& Other )
	{
		Copy(Other);
		return *this;
	}

	TArray& operator=( const TArray<ElementType,Allocator>& Other )
	{
		Copy(Other);
		return *this;
	}

	/**
	 * Adds a new item to the end of the array, possibly reallocating the whole array to fit.
	 *
	 * @param Item	The item to add
	 * @return		Index to the new item
	 */
	int32 Add( const ElementType& Item )
	{
		// It isn't valid to specify an Item that is in the array, since adding an item might resize the array, which would make the item invalid
		check( ((&Item) < GetTypedData()) || ((&Item) >= GetTypedData()+ArrayMax) );
		const int32 Index = AddUninitialized(1);
		new(GetTypedData() + Index) ElementType(Item);
		return Index;
	}
	/** Caution, AddZeroed() will create elements without calling the constructor and this is not appropriate for element types that require a constructor to function properly. */
	int32 AddZeroed( int32 Count=1 )
	{
		const int32 Index = AddUninitialized( Count );
		FMemory::Memzero( (uint8*)AllocatorInstance.GetAllocation()+Index*sizeof(ElementType), Count*sizeof(ElementType) );
		return Index;
	}
	int32 AddUnique( const ElementType& Item )
	{
		for( int32 Index=0; Index<ArrayNum; Index++ )
			if( (*this)[Index]==Item )
				return Index;
		return Add( Item );
	}

	/**
	 * Reserves memory such that the array can contain at least Number elements.
	 */
	void Reserve(int32 Number)
	{
		if (Number > ArrayMax)
		{
			ArrayMax = Number;
			AllocatorInstance.ResizeAllocation(ArrayNum,ArrayMax,sizeof(ElementType));
		}
	}
	
	/** Sets the size of the array. */
	void Init(int32 Number)
	{
		Empty(Number);
		AddUninitialized(Number);
	}

	/** Sets the size of the array, filling it with the given element. */
	void Init(const ElementType& Element,int32 Number)
	{
		Empty(Number);
		for(int32 Index = 0;Index < Number;++Index)
		{
			new(*this) ElementType(Element);
		}
	}

	/**
	 * Removes the first occurrence of the specified item in the array, maintaining order but not indices.
	 *
	 * @param	Item	The item to remove
	 *
	 * @return	The number of items removed.  For RemoveSingleItem, this is always either 0 or 1.
	 */
	int32 RemoveSingle( const ElementType& Item )
	{
		// It isn't valid to specify an Item that is in the array, since removing that item will change Item's value.
		check( ((&Item) < GetTypedData()) || ((&Item) >= GetTypedData()+ArrayMax) );

		for( int32 Index=0; Index<ArrayNum; Index++ )
		{
			if( GetTypedData()[Index] == Item )
			{
				// Destruct items that match the specified Item.
				DestructItems(Index,1);
				const int32 NextIndex = Index + 1;
				if( NextIndex < ArrayNum )
				{
					const int32 NumElementsToMove = ArrayNum - NextIndex;
					FMemory::Memmove(&GetTypedData()[Index],&GetTypedData()[NextIndex],sizeof(ElementType) * NumElementsToMove);
				}

				// Update the array count
				--ArrayNum;

				// Removed one item
				return 1;
			}
		}

		// Specified item was not found.  Removed zero items.
		return 0;
	}

	/** Removes as many instances of Item as there are in the array, maintaining order but not indices. */
	int32 Remove( const ElementType& Item )
	{
		// It isn't valid to specify an Item that is in the array, since removing that item will change Item's value.
		check( ((&Item) < GetTypedData()) || ((&Item) >= GetTypedData()+ArrayMax) );

		const int32 OriginalNum = ArrayNum;
		if (!OriginalNum)
		{
			return 0; // nothing to do, loop assumes one item so need to deal with this edge case here
		}

		int32 WriteIndex = 0;
		int32 ReadIndex = 0;
		bool NotMatch = !(GetTypedData()[ReadIndex] == Item); // use a ! to guarantee it can't be anything other than zero or one
		do
		{
			int32 RunStartIndex = ReadIndex++;
			while (ReadIndex < OriginalNum && NotMatch == !(GetTypedData()[ReadIndex] == Item))
			{
				ReadIndex++;
			}
			int32 RunLength = ReadIndex - RunStartIndex;
			checkSlow(RunLength > 0);
			if (NotMatch)
			{
				// this was a non-matching run, we need to move it
				if (WriteIndex != RunStartIndex)
				{
					FMemory::Memmove( &GetTypedData()[ WriteIndex ], &GetTypedData()[ RunStartIndex ], sizeof(ElementType) * RunLength );
				}
				WriteIndex += RunLength;
			}
			else
			{
				// this was a matching run, delete it
				DestructItems( RunStartIndex, RunLength );
			}
			NotMatch = !NotMatch;
		} while (ReadIndex < OriginalNum);

		ArrayNum = WriteIndex;
		return OriginalNum - ArrayNum;
	}


	/**
	 * Remove all instances that match the predicate
	 *
	 * @param Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void RemoveAll( const PREDICATE_CLASS& Predicate )
	{
		for ( int32 ItemIndex=0; ItemIndex < Num(); )
		{
			if ( Predicate( (*this)[ItemIndex] ) )
			{
				RemoveAt(ItemIndex);
			}
			else
			{
				++ItemIndex;
			}
		}
	}

	/**
	 * Removes the first occurrence of the specified item in the array.  This version is much more efficient
	 * O(Count) instead of O(ArrayNum), but does not preserve the order
	 *
	 * @param	Item	The item to remove
	 *
	 * @return	The number of items removed.  For RemoveSingleItem, this is always either 0 or 1.
	 */
	int32 RemoveSingleSwap( const ElementType& Item )
	{
		check( ((&Item) < (ElementType*)AllocatorInstance.GetAllocation()) || ((&Item) >= (ElementType*)AllocatorInstance.GetAllocation()+ArrayMax) );
		for( int32 Index=0; Index<ArrayNum; Index++ )
		{
			if( (*this)[Index]==Item )
			{
				RemoveAtSwap(Index);

				// Removed one item
				return 1;
			}
		}

		// Specified item was not found.  Removed zero items.
		return 0;
	}

	/** RemoveItemSwap, this version is much more efficient O(Count) instead of O(ArrayNum), but does not preserve the order */
	int32 RemoveSwap( const ElementType& Item )
	{
		check( ((&Item) < (ElementType*)AllocatorInstance.GetAllocation()) || ((&Item) >= (ElementType*)AllocatorInstance.GetAllocation()+ArrayMax) );
		const int32 OriginalNum=ArrayNum;
		for( int32 Index=0; Index<ArrayNum; Index++ )
		{
			if( (*this)[Index]==Item )
			{
				RemoveAtSwap( Index-- );
			}
		}
		return OriginalNum - ArrayNum;
	}

	void SwapMemory(int32 A, int32 B)
	{
		FMemory::Memswap(
			(uint8*)AllocatorInstance.GetAllocation()+(sizeof(ElementType)*A),
			(uint8*)AllocatorInstance.GetAllocation()+(sizeof(ElementType)*B),
			sizeof(ElementType)
			);
	}

	void Swap(int32 A, int32 B)
	{
		check((A >= 0) && (B >= 0));
		check((ArrayNum > A) && (ArrayNum > B));
		if (A != B)
		{
			SwapMemory(A,B);
		}
	}

	/**
	 * Same as empty, but doesn't change memory allocations, unless the new size is larger than
	 * the current array. It calls the destructors on held items if needed and then zeros the ArrayNum.
	 *
	 * @param NewSize the expected usage size
	 */
	void Reset(int32 NewSize = 0)
	{
		// If we have space to hold the excepted size, then don't reallocate
		if (NewSize <= ArrayMax)
		{
			DestructItems(0,ArrayNum);
			ArrayNum = 0;
		}
		else
		{
			Empty(NewSize);
		}
	}

	/**
	 * Searches for the first entry of the specified type, will only work
	 * with TArray<UObject*>.  Optionally return the item's index, and can
	 * specify the start index.
	 */
	template<typename SearchType> bool FindItemByClass(SearchType **Item = NULL, int32 *ItemIndex = NULL, int32 StartIndex = 0) const
	{
		UClass* SearchClass = SearchType::StaticClass();
		for (int32 Idx = StartIndex; Idx < ArrayNum; Idx++)
		{
			if ((*this)[Idx] != NULL && (*this)[Idx]->IsA(SearchClass))
			{
				if (Item != NULL)
				{
					*Item = (SearchType*)((*this)[Idx]);
				}
				if (ItemIndex != NULL)
				{
					*ItemIndex = Idx;
				}
				return true;
			}
		}
		return false;
	}

	// Iterators
	typedef TIndexedContainerIterator< TArray<ElementType,Allocator> >  TIterator;
	typedef TIndexedContainerConstIterator< TArray<ElementType,Allocator> >  TConstIterator;


	/** Creates an iterator for the contents of this array */
	TIterator CreateIterator()
	{
		return TIterator(*this);
	}

	/** Creates a const iterator for the contents of this array */
	TConstIterator CreateConstIterator() const
	{
		return TConstIterator(*this);
	}

	/** Sorts the array assuming < operator is defined for the item type */
	void Sort()
	{
		::Sort( GetTypedData(), Num() );
	}

	/**
	 * Sorts the array using user define predicate class.
	 *
	 * @param Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void Sort( const PREDICATE_CLASS& Predicate )
	{
		::Sort( GetTypedData(), Num(), Predicate );
	}

#if defined(_MSC_VER)
private:
	/**
	* Helper function that can be used inside the debuggers watch window to debug TArrays. E.g. "*Class->Defaults.DebugGet(5)". 
	*
	* @param	i	Index
	* @return		pointer to type T at Index i
	*/
	FORCENOINLINE const ElementType& DebugGet( int32 i ) const
	{
		return GetTypedData()[i];
	}
#endif

protected:

	/**
	 * Copies data from one array into this array. Uses the fast path if the
	 * data in question does not need a constructor.
	 *
	 * @param Source the source array to copy
	 */
	template<typename OtherAllocator>
	void Copy(const TArray<ElementType,OtherAllocator>& Source)
	{
		if ((void*)this != (void*)&Source)
		{
			// Just empty our array if there is nothing to copy
			if (Source.Num() > 0)
			{
				// Presize the array so there are no extra allocs/memcpys
				Empty(Source.Num());
				// Determine whether we need per element construction or bulk
				// copy is fine
				if (TContainerTraits<ElementType>::NeedsConstructor)
				{
					// Use the inplace new to copy the element to an array element
					for (int32 Index = 0; Index < Source.Num(); Index++)
					{
						new(GetTypedData() + Index) ElementType(Source[Index]);
					}
				}
				else
				{
					// Use the much faster path for types that allow it
					FMemory::Memcpy(AllocatorInstance.GetAllocation(),&Source[0],sizeof(ElementType) * Source.Num());
				}
				ArrayNum = Source.Num();
			}
			else
			{
				Empty(0);
			}
		}
	}
	
	/** Destructs a range of items in the array. */
	FORCEINLINE void DestructItems(int32 Index,int32 Count)
	{
		if( TContainerTraits<ElementType>::NeedsDestructor )
		{
			for( int32 i=Index; i<Index+Count; i++ )
			{
				(&(*this)[i])->~ElementType();
			}
		}
	}

	enum { AllocatorNeedsElementType = Allocator::NeedsElementType };

	typedef typename TChooseClass<
		AllocatorNeedsElementType,
		typename Allocator::template ForElementType<ElementType>,
		typename Allocator::ForAnyElementType
		>::Result ElementAllocatorType;

	ElementAllocatorType AllocatorInstance;
	int32	  ArrayNum;
	int32	  ArrayMax;

	/**
	 * Implicit heaps
	 */

public:

	/** 
	 * Builds an implicit heap from the array
	 *
	 * @pram Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void Heapify(const PREDICATE_CLASS& Predicate)
	{
		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper( Predicate );
		for(int32 Index=HeapGetParentIndex(Num()-1); Index>=0; Index--)
		{
			SiftDown(Index, Num(), PredicateWrapper);
		}

#if DEBUG_HEAP
		VerifyHeap(PredicateWrapper);
#endif
	}

	void Heapify()
	{
		Heapify( TLess<ElementType>() );
	}

	/** 
	 * Adds a new element to the heap
	 *
	 * @pram InIntem Item to be added
	 * @pram Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void HeapPush( const ElementType& InItem, const PREDICATE_CLASS& Predicate )
	{
		// Add at the end, then sift up
		Add(InItem);
		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper(Predicate);
		SiftUp(0, Num()-1, PredicateWrapper);

#if DEBUG_HEAP
		VerifyHeap(PredicateWrapper);
#endif
	}

	void HeapPush( const ElementType& InItem )
	{
		HeapPush( InItem, TLess<ElementType>() );
	}

	/** 
	 * Removes the top element from the heap.
	 *
	 * @pram OutItem The removed item
	 * @pram Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void HeapPop( ElementType& OutItem, const PREDICATE_CLASS& Predicate )
	{
		OutItem = (*this)[ 0 ];
		RemoveAtSwap( 0 );

		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper(Predicate);
		SiftDown(0, Num(), PredicateWrapper);

#if DEBUG_HEAP
		VerifyHeap(PredicateWrapper);
#endif
	}

	void HeapPop( ElementType& OutItem )
	{
		HeapPop( OutItem, TLess<ElementType>() );
	}

	template <class PREDICATE_CLASS>
	void VerifyHeap(const PREDICATE_CLASS& Predicate )
	{
		// Verify Predicate
		ElementType* Heap = GetTypedData();
		for( int32 Index=1; Index<Num(); Index++ )
		{
			int32 ParentIndex = HeapGetParentIndex(Index);
			if( Predicate(Heap[Index], Heap[ParentIndex]) )
			{
				check( false );
			}
		}
	}

	/** 
	 * Removes the top element from the heap.
	 *
	 * @pram Predicate Predicate class instance
	 */
	template <class PREDICATE_CLASS>
	void HeapPopDiscard(const PREDICATE_CLASS& Predicate )
	{
		RemoveAtSwap( 0 );
		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper(Predicate);
		SiftDown(0, Num(), PredicateWrapper);

#if DEBUG_HEAP
		VerifyHeap(PredicateWrapper);
#endif
	}

	void HeapPopDiscard()
	{
		HeapPopDiscard( TLess<ElementType>() );
	}

	/** 
	 * Returns the top element from the heap (does not remove the element).
	 */
	const ElementType& HeapTop() const
	{
		return (*this)[ 0 ];
	}

	/** 
	 * Returns the top element from the heap (does not remove the element).
	 */
	ElementType& HeapTop()
	{
		return (*this)[ 0 ];
	}

	/**
	 * Removes an element from the heap.
	 *
	 * @param Index at which to remove item.
	 * @param Predicate Predicate class.
	 */
	template <class PREDICATE_CLASS>
	void HeapRemoveAt( int32 Index, const PREDICATE_CLASS& Predicate )
	{
		RemoveAtSwap(Index);

		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper(Predicate);
		SiftDown(Index, Num(), PredicateWrapper);
		SiftUp(0, Index, PredicateWrapper);

#if DEBUG_HEAP
		VerifyHeap(PredicateWrapper);
#endif
	}

	void HeapRemoveAt( int32 Index )
	{
		HeapRemoveAt( Index, TLess< ElementType >() );
	}

	/**
	 * Performs heap sort on the array.
	 *
	 * @param Predicate Predicate class instance.
	 */
	template <class PREDICATE_CLASS>
	void HeapSort( const PREDICATE_CLASS& Predicate )
	{
 		TReversePredicateWrapper< ElementType, PREDICATE_CLASS > ReversePredicateWrapper(Predicate);
		Heapify(ReversePredicateWrapper);

		ElementType* Heap = GetTypedData();
		for(int32 Index=Num()-1; Index>0; Index--)
		{
			Exchange(Heap[0], Heap[Index]);
			SiftDown(0, Index, ReversePredicateWrapper);
		}

#if DEBUG_HEAP
		TDereferenceWrapper< ElementType, PREDICATE_CLASS> PredicateWrapper(Predicate);

		// Verify Heap Property
		VerifyHeap(PredicateWrapper);

		// Also verify Array is properly sorted
		for(int32 Index=1; Index<Num(); Index++)
		{
			if( PredicateWrapper(Heap[Index], Heap[Index-1]) )
			{
				check( false );
			}
		}
#endif
	}

	void HeapSort()
	{
		HeapSort( TLess<ElementType>() );
	}

private:

	/**
	 * Gets the index of the left child of node at Index.
	 *
	 * @param Index Node for which the left child index is to be returned.
	 * @return Index of the left child.
	 */
	FORCEINLINE int32 HeapGetLeftChildIndex( int32 Index ) const
	{
		return Index * 2 + 1;
	}

	/** 
	 * Checks if node located at Index is a leaf or not .
	 *
	 * @param Index - Node index.
	 * @return true if node is a leaf.
	 */
	FORCEINLINE bool HeapIsLeaf( int32 Index, int32 Count ) const
	{
		return HeapGetLeftChildIndex( Index ) >= Count;
	}

	/**
	 * Gets the parent index for node at Index.
	 *
	 * @param Index node index.
	 * @return Parent index.
	 */
	FORCEINLINE int32 HeapGetParentIndex( int32 Index ) const
	{
		return ( Index - 1 ) / 2;
	}

	/**
	 * Fixes a possible violation of order property between node at Index and a child.
	 *
	 * @param Index node index.
	 * @param Count Size of the heap (to avoid using Num()).
	 * @param Predicate Predicate class instance.
	 */
	template <class PREDICATE_CLASS>
	FORCEINLINE void SiftDown(int32 Index, const int32 Count, const PREDICATE_CLASS& Predicate)
	{
		ElementType* Heap = GetTypedData();
		while( !HeapIsLeaf(Index, Count) )
		{
			const int32 LeftChildIndex = HeapGetLeftChildIndex(Index);
			const int32 RightChildIndex = LeftChildIndex + 1;

			int32 MinChildIndex = LeftChildIndex;
			if( RightChildIndex < Count )
			{
				MinChildIndex = Predicate(Heap[LeftChildIndex], Heap[RightChildIndex]) ? LeftChildIndex : RightChildIndex;
			}

			if( Predicate(Heap[MinChildIndex], Heap[Index]) )
			{
				Exchange(Heap[Index], Heap[MinChildIndex]);
				Index = MinChildIndex;
			}
			else
			{
				break;
			}
		}
	}

	/**
	 * Fixes a possible violation of order property between node at NodeIndex and a parent.
	 *
	 * @param RootIndex, how far to go up
	 * @param NodeIndex node index.
	 * @param Predicate Predicate class instance.
	 */
	template <class PREDICATE_CLASS>
	FORCEINLINE void SiftUp(int32 RootIndex, const int32 NodeIndex, const PREDICATE_CLASS& Predicate)
	{
		ElementType* Heap = GetTypedData();
		int32 Index = FMath::Min(NodeIndex, Num() - 1);
		while(Index > RootIndex)
		{
			int32 ParentIndex = HeapGetParentIndex(Index);
			if( Predicate(Heap[Index], Heap[ParentIndex]) )
			{
				Exchange(Heap[Index], Heap[ParentIndex]);
				Index = ParentIndex; 
			}
			else
			{
				break;
			}
		}
	}
};

//
// Array operator news.
//
template <typename T,typename Allocator> void* operator new( size_t Size, TArray<T,Allocator>& Array )
{
	check(Size == sizeof(T));
	const int32 Index = Array.AddUninitialized(1);
	return &Array[Index];
}
template <typename T,typename Allocator> void* operator new( size_t Size, TArray<T,Allocator>& Array, int32 Index )
{
	check(Size == sizeof(T));
	Array.InsertUninitialized(Index,1);
	return &Array[Index];
}

/** A specialization of the exchange macro that avoids reallocating when exchanging two arrays. */
template <typename T> inline void Exchange( TArray<T>& A, TArray<T>& B )
{
	FMemory::Memswap( &A, &B, sizeof(TArray<T>) );
}

/** A specialization of the exchange macro that avoids reallocating when exchanging two arrays. */
template<typename ElementType,typename Allocator>
inline void Exchange( TArray<ElementType,Allocator>& A, TArray<ElementType,Allocator>& B )
{
	FMemory::Memswap( &A, &B, sizeof(TArray<ElementType,Allocator>) );
}