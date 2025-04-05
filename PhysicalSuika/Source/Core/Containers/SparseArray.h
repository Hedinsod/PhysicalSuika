#pragma once

#include "Core/Utility.h"
#include <cstdlib>
#include <string>

template <class ElementType>
class TSparseArray
{
public:
	class Iterator;

	inline TSparseArray()
		: Capacity(16)
		, ArraySize(0)
	{
		Data = std::malloc(sizeof(ElementType) * Capacity);
		GAssert(Data);

		BitWordsNum = Capacity / BitsPerWord + 1;
		Bitmap = new uint32_t[BitWordsNum](~0u);
	}

	// TODO: Temporary solution, implement rule of five
	TSparseArray(const TSparseArray&) = delete;

	inline ~TSparseArray()
	{
		Clear();

		std::free(Data);

		delete[] Bitmap;
	}

	// Add functions
	template <class ...Args>
	int32_t Emplace(Args&&... args)
	{
		int32_t NewIndex = AddEmptyElement();
		new (&GetData()[NewIndex]) ElementType(std::forward<Args>(args)...);

		return NewIndex;
	}
	int32_t Push(const ElementType& Element)
	{
		int32_t NewIndex = AddEmptyElement();
		new (&GetData()[NewIndex]) ElementType(Element);

		return NewIndex;
	}
	int32_t Push(ElementType&& Element)
	{
		int32_t NewIndex = AddEmptyElement();
		new (&GetData()[NewIndex]) ElementType(std::move(Element));

		return NewIndex;
	}

	// Remove
	void Remove(int32_t Index)
	{
		// manual destructor on allocated memory
		GetData()[Index].~ElementType();
		SetBitMapValue(Index, false);
	}
	void Remove(Iterator It)
	{
		Remove(It.GetIndex());
	}
	void Clear()
	{
		for (auto It = begin(); It != end(); It++)
		{
			Remove(It);
		}
	}

	// Accessors
	inline int32_t Size() const
	{
		return ArraySize;
	}
	ElementType& operator[](int32_t Index)
	{
		GAssert(Index >= 0 && Index < ArraySize);
		GAssert(GetBitMapValue(Index));
		return GetData()[Index];
	}
	const ElementType& operator[](int32_t Index) const
	{
		GAssert(Index >= 0 && Index < ArraySize);
		GAssert(GetBitMapValue(Index));
		return GetData()[Index];
	}

	// Iterator
	class Iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ElementType;
		using pointer = ElementType*;
		using reference = ElementType&;

		Iterator(TSparseArray& Array, int32_t Index)
			: Array(Array), Index(Index)
		{
			SkipInvalid();
		}

		reference operator*() const { return Array[Index]; }
		pointer operator->() { return &Array[Index]; }

		int32_t GetIndex() { return Index; }

		// Prefix increment
		Iterator& operator++()
		{
			++Index;
			SkipInvalid();
			return *this;
		}

		// Postfix increment
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.Index == b.Index; }
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.Index != b.Index; }

		
	private:
		void SkipInvalid()
		{
			while (Index < Array.ArraySize && !Array.GetBitMapValue(Index))
			{
				++Index;
			}
		}

		TSparseArray& Array;
		int32_t Index;
	};


	Iterator begin() { return Iterator(*this, 0); }
	Iterator end() { return Iterator(*this, ArraySize); }

	

private:
	//
	int32_t FindNextFreeIndex()
	{
		int32_t WordIndex = 0;
		while (WordIndex < BitWordsNum && !(Bitmap[WordIndex] & ~0u))
			WordIndex++;

		if (WordIndex == BitWordsNum)
		{
			return ArraySize;
		}

		uint32_t BitIndex = 0;
		while (BitIndex < BitsPerWord && !((Bitmap[WordIndex] >> BitIndex) & 1u))
			BitIndex++;

		GAssert(BitIndex < BitsPerWord); // Should be impossible

		return WordIndex * BitsPerWord + BitIndex;
	}

	//
	int32_t AddEmptyElement()
	{
		int32_t NewIndex = FindNextFreeIndex();
		if (NewIndex >= ArraySize)
		{
			if ((ArraySize + 1) > Capacity)
			{
 				Realloc();
			}
			NewIndex = ArraySize++;
		}
		SetBitMapValue(NewIndex, true);

		return NewIndex;
	}

	// 
	ElementType* GetData()
	{
		return static_cast<ElementType*>(Data);
	}

	// 
	void Realloc()
	{
		// 0. Calculate
		int32_t NewCapacity = static_cast<int32_t>(Capacity * 1.5);  // Some arbitrary multiplier
		int32_t NewBitWordsNum = NewCapacity / BitsPerWord + 1;

		if (NewCapacity > Capacity)
		{
			// 1. Allocate
			void* NewData = std::malloc(sizeof(ElementType) * NewCapacity);

			// 2. Copy
			for (Iterator It = begin(); It != end(); It++)
			{
				ElementType* Array = static_cast<ElementType*>(NewData);
				new (&Array[It.GetIndex()]) ElementType(std::move(*It));

				// Destroy element before freeing its memory 
				(*It).~ElementType();
			}


			// 3. Free
			std::free(Data); // Call destructor

			// 4. Swap
			Data = NewData;
			Capacity = NewCapacity;
		}

		if (NewBitWordsNum > BitWordsNum)
		{
			// 1. Allocate
			uint32_t* NewBitmap = new uint32_t[NewBitWordsNum](~0u);

			// 2. Copy
			errno_t ErrorCode = memcpy_s(NewBitmap, NewBitWordsNum * sizeof(uint32_t), Bitmap, BitWordsNum * sizeof(uint32_t));
			GAssert(ErrorCode == 0);

			// 3. Free
			delete[] Bitmap;

			// 4. Swap
			Bitmap = NewBitmap;
			BitWordsNum = NewBitWordsNum;
		}
	}

	//
	bool GetBitMapValue(int32_t Index)
	{
		int32_t WordIndex = Index / BitsPerWord;
		int32_t BitIndex = Index - WordIndex * BitsPerWord;

		return !(Bitmap[WordIndex] & (1 << BitIndex));
	}

	//
	void SetBitMapValue(int32_t Index, bool Value)
	{
		int32_t WordIndex = Index / BitsPerWord;
		int32_t BitIndex = Index - WordIndex * BitsPerWord;

		Value = !Value; // Store free as 1 and 0 as taken

		Bitmap[WordIndex] = (Bitmap[WordIndex] & ~(1 << BitIndex)) | (((uint32_t)Value) << BitIndex);
	}


	// *** 

	void* Data;
	uint32_t* Bitmap;

	int32_t ArraySize;
	size_t Capacity;
	size_t BitWordsNum;

	static constexpr size_t BitsPerWord = sizeof(uint32_t) * 8;

};
