#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	//옵션을 해석하고 처리할 주체?
	// 소켓코드 -> SOL_SOCKET


	//SO_KEPPALIVE = 주기적으로 연결상태 확인 여부(tcp만 가능)
	// 상대방이 소리소문없이 연결을 끊는다면?
	
	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET,SO_KEEPALIVE,(char*)&enable,sizeof(enable));


	//SO_LINGER = 지연하다.
	// 송신 버퍼에있는 값들을 보낼것인가? 아니며 ㄴ날릴것인가?
	
	//onoff  = 0이면 closesocket()이 바로 리턴. 아니면 linger 초만큼 대기(default 0)
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;// linger는 대기시간.
	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

	//half-close
	//SD_SEND : send 막는다.
	//SD_RECEIVE : recv 막
	//SD_BOTH
	//::shutdown(serverSocket, SD_SEND);//나는 할말 다했어
	
	//::closesocket(serverSocket);
	
	//SO_SNDBUF = 송신버퍼의 크기
	//SO_RCVBUF  = 수신버퍼의 크기

	int32 sendBufferSize;
	int32 optionLen = sizeof(sendBufferSize);
	::getsockopt(serverSocket,SOL_SOCKET,SO_SNDBUF,(char*)&sendBufferSize, &optionLen);
	cout << "송신 버퍼 크기 : " << sendBufferSize << endl;
	
	int32 recvBufferSize;
	optionLen = sizeof(recvBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, &optionLen);
	cout << "수신 버퍼 크기 : " << recvBufferSize << endl;//기본 64KB


	//SO_REUSEADDR
	//IP주소 및 PORT 재사용
	{
		bool enable = true;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	}

	//IPPROTO_TCP
	//TCP_NODELAY = Nagle 네이글 알고리즘 작동여부
	//네이글? 데이터가 충분히 크면 보내고, 데이터가 크지않으면 쌓일때까지 대기
	//장점: 불필요한 작은 데이터 패킷이 안생김.
	// 단점:반응시간에 손해.
	
	{
		bool enable = true;
		::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
	}

	// 윈속 종료
	::WSACleanup();
}