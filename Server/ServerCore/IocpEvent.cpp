#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType eType) : eventType( eType)
{
	Init();
}

void IocpEvent::Init()
{
	//�� �Ʒ��͵��� �ü���� ����ϴ� ������.
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal= 0;
	OVERLAPPED::InternalHigh  = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
