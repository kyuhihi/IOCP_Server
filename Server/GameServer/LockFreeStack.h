#pragma once

//==================================
//		1�� �õ� - �̱۽��������
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
//		2�� �õ� - ��Ƽ���������(Compare and Swap)
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
//		3�� �õ� - ��Ƽ���������(ABA���� �ذ� - ī��Ʈ���д�)
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
	{// ����ü �ΰ��� �̸����� �θ�������
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		struct
		{//�Ʒ��� 128 ���� 128
			uint64 depth : 16; //uint64���ƴ϶� ��Ʈ������ ������
			uint64 sequence : 48;
			uint64 reserved: 4;
			uint64 next : 60;
		} HeaderX64;
	};

};

void InitializeHead(SListHeader* header);

void PushEntrySList(SListHeader* header, SListEntry* entry);

SListEntry* PopEntrySList(SListHeader* header);