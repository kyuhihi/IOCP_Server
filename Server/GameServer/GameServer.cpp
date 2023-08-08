﻿#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 0;
	}

	
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode :" << errCode << endl;
		return 0;
	}



	SOCKADDR_IN serverAddr; 
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;

	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "BIND ErrorCode :" << errCode << endl;
		return 0;
	}

	if (::listen(listenSocket, 10))
	{
		int32 errCode = ::WSAGetLastError();
		cout << "LISTEN ErrorCode :" << errCode << endl;
		return 0;
	}

	//---------------------------------
	while (true)
	{
		SOCKADDR_IN clientAddr; 
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

		if (clientSocket == INVALID_SOCKET)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "ACCEPT ErrorCode :" << errCode << endl;
			return 0;
		}

		// 손님입장!
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "client Connected!!! IP" << ipAddress<<endl;
	
		while (true)
		{
			char recvBuffer[1000];

			this_thread::sleep_for(1s);

			int32 recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				int32 errCode = WSAGetLastError();
				cout << "RECV ErrorCode :" << errCode << endl;
				return 0;
			}
			cout << "Recv Data! Data " << recvBuffer << endl;
			cout << "Recv Data! Len " << recvLen << endl;
			


			/*int32 resultCode = send(clientSocket, recvBuffer, recvLen, 0);

			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "SEND ErrorCode :" << errCode << endl;
				return 0;
			}*/

		
		}
	
	}

	//---------------------------------

	::WSACleanup();
}

