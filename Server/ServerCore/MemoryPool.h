#pragma once

enum
{
	SLIST_ALIGNMENT = 16,
};

/*-------------------
	MemoryHeader
-------------------*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{// [MemoryHeader][Data]
	MemoryHeader(int32 size): allocSize(size){}

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); //placement new
		return reinterpret_cast<void*>(++header);// c++의 특성상 Data전의 메모리 주소를 반환할거임.
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
	
	int32 allocSize;
};

/*-------------------
	MemeoryPool
-------------------*/
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();

private:
	SLIST_HEADER _header;// 첫번째 데이터를 가리키게 될것.
	int32 _allocSize = 0;// 메모리풀의 단일 노드 사이즈
	atomic<int32> _useCount = 0;
	atomic<int32> _reserveCount = 0;
};

