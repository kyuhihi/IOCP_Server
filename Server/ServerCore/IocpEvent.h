#pragma once

//--------------------
//		EventType
//--------------------
// enum class는 namespace를 갖게한다.
class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	//PreRecv,
	Recv,
	Send
};

//--------------------
//		IOCP EVENT
//--------------------

class IocpEvent : public OVERLAPPED
{// 이경우는 소멸자에 virtual 키워드를 사용불가함.
	//overlapped를 상속받는 순간 첫번째주소에 overlapped변수가 들어가야하기때문에
public:
	IocpEvent(EventType eType);

	void Init();

public:
	EventType		eventType;
	IocpObjectRef	owner;
};

//--------------------
//		CONNECT EVENT
//--------------------

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};


//--------------------
//		Accept EVENT
//--------------------

class AcceptEvent : public IocpEvent
{// Accept evnet는 추가적으로 멤버변수가 필요할수도
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	SessionRef session = nullptr;
};


//--------------------
//		Recv EVENT
//--------------------
class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};


//--------------------
//		Send EVENT
//--------------------
class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}
};