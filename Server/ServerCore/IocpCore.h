#pragma once

//--------------------
//		IOCP OBJECT
//--------------------

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent,int32 numOfBytes = 0) abstract;
};


//--------------------
//		IOCP CORE
//--------------------

class IocpCore
{
public:
	IocpCore();
	~IocpCore();


	HANDLE GetHandle() { return _iocpHandle; }

	bool Register(class IocpObject* iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);// 일감이 없나 두리번

private:
	HANDLE _iocpHandle;
};

//TEMP
extern IocpCore GIocpCore;