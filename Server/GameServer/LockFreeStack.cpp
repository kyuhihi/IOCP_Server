#include "pch.h"
#include "LockFreeStack.h"

//==================================
//		1차 시도 - 싱글스레드기준
//==================================

//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//	header->next = entry;
//}
//
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* first = header->next;
//
//	if (first != nullptr)
//		header->next = first->next;
//
//	return first;
//}

//==================================
//		2차 시도 - 멀티스레드기준
//==================================


//void InitializeHead(SListHeader* header)
//{
//	header->next = nullptr;
//}
//
//void PushEntrySList(SListHeader* header, SListEntry* entry)
//{
//	entry->next = header->next;
//
//	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) == 0)
//	{
//
//	}
//}
//
//SListEntry* PopEntrySList(SListHeader* header)
//{
//	SListEntry* expected = header->next;
//	
//	//ABA Problem
//
//	//Pop과 Push가 거의 그럴일은 없겠지만 사라진 주소에 다시 할당받아 새로운 값이 들어간다면
//	// 아래의 while문은 문제없이 통과할것..
//	//[5000]->[6000]->[7000]
//	//Header
//
//	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) == 0)
//	{
//
//	}
//
//	return expected;
//}

//==================================================
//		3차 시도 - 멀티스레드기준(ABA문제 해결)
//==================================================


void InitializeHead(SListHeader* header)
{
	header->alignment = 0;
	header->region = 0;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	SListHeader expected = {};
	SListHeader desired = {};

	//16바이트 정렬
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;
		//이 사이에 변경될수있다.

		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);// 4비트 날렸던거 복원
		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
		{
			break;
		}
	}

}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (true)
	{
		expected = *header;
		entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);// 4비트 날렸던거 복원
		if (entry == nullptr)
			break;

		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return entry;
}