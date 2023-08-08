// DummyClient.cpp 

#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


int main()
{
	// 윈socket 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 0;
	}

	// af: Address family( AF_INET - ipv4, AF_INET6 - ipv6)
	// type: TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
	// protocol : 0 자동선택
	// return : socket desc
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode :" << errCode << endl;
		return 0;
	}

	SOCKADDR_IN serverAddr; //IPv4 기준 이거 사용.
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;

	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");// 구식 방법임
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	//	host to network short
	//	Little- Endian vs Big-Endian
	// ex) 0x12345678 4byte 정수
	// low [0x78][0x56][0x34][0x12] high < little
	// low [0x12][0x34][0x56][0x78] high > big = network
	//네트워크에서 공식적으로 사용하는 endian방식은 big이다.
	serverAddr.sin_port = ::htons(7777);


	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode :" << errCode << endl;
		return 0;
	}

	//-----------------------------------
	// 데이터 송수신 가능!

	cout << "Connected To Server!!" << endl;
	while (true)
	{
		this_thread::sleep_for(1s);
	}

	//-----------------------------------

	//소켓 리소스 반환
	::closesocket(clientSocket);

	//윈솤 종료
	::WSACleanup();
}

 