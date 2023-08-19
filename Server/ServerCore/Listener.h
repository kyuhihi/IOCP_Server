#pragma once

#include "IocpCore.h"
#include "NetworkAddress.h"

class AcceptEvent;


//-------------------
//		Listener
//-------------------
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	// 외부에서 사용
	bool StartAccept(NetworkAddress netAddress);
	void CloseSocket();

public:
	// interpace 구현
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	// 수신관련
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;// 이게 리스너 소켓 
	Vector<AcceptEvent*> _acceptEvents;
};

