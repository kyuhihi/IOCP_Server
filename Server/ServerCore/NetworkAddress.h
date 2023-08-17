#pragma once

/*--------------
	NetAddress
---------------*/

class NetworkAddress
{
public:
	NetworkAddress() = default;
	NetworkAddress(SOCKADDR_IN sockAddr);
	NetworkAddress(wstring ip, uint16 port);

	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	wstring			GetIpAddress();
	uint16			GetPort() { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		_sockAddr = {};
};

