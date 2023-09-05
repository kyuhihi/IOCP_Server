#pragma once

//--------------------
//		EventType
//--------------------
// enum class�� namespace�� �����Ѵ�.
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
{// �̰��� �Ҹ��ڿ� virtual Ű���带 ���Ұ���.
	//overlapped�� ��ӹ޴� ���� ù��°�ּҿ� overlapped������ �����ϱ⶧����
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
{// Accept evnet�� �߰������� ��������� �ʿ��Ҽ���
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