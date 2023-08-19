#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"

//-------------------
//		Session
//-------------------
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();


public:
	// ���� ��������
	void SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }


public:
	// interpace ����
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;


public:
	//TEMP
	char _recvBuffer[1000] = {};

private:
	SOCKET _socket = INVALID_SOCKET;// Ŭ�����
	NetworkAddress _netAddress = {};
	Atomic<bool> _connected = false;

};

