#pragma once

//==================================
//		1차 시도 - 싱글스레드기준
//==================================


//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//
//SListEntry* PopEntrySList(SListHeader* header);

//==================================================
//		2차 시도 - 멀티스레드기준(Compare and Swap)
//==================================================

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//
//SListEntry* PopEntrySList(SListHeader* header);

//==================================================
//		3차 시도 - 멀티스레드기준(ABA문제 해결 - 카운트를둔다)
//==================================================

DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	union
	{// 구조체 두개의 이름으로 부를수있음
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		struct
		{//아래도 128 위도 128
			uint64 depth : 16; //uint64가아니라 비트단위로 나눈것
			uint64 sequence : 48;
			uint64 reserved: 4;
			uint64 next : 60;
		} HeaderX64;
	};

};

void InitializeHead(SListHeader* header);

void PushEntrySList(SListHeader* header, SListEntry* entry);

SListEntry* PopEntrySList(SListHeader* header);