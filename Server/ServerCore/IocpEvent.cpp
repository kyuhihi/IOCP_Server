#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType eType) : eventType( eType)
{
	Init();
}

void IocpEvent::Init()
{
	//이 아래것들은 운영체제가 사용하는 값들임.
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal= 0;
	OVERLAPPED::InternalHigh  = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}
