#include "pch.h"

#include "Socket.h"


ISocket::ISocket( void )
{

}

ISocket::~ISocket( void )
{

}

TWSAStartUp::TWSAStartUp( void )
{
	const WORD winsockVersion					= MAKEWORD( 2, 2 );
	const int result							= ::WSAStartup( winsockVersion, &_wsaData );

	HODONG_ASSERT( 0 == result, "WSAStartup 함수 Call에 실패했습니다. 비정상 입니다" );
	HODONG_ASSERT( 2 == LOBYTE( _wsaData.wVersion ) || 2 == HIBYTE( _wsaData.wVersion ), "WSAStartup 함수 Call에 실패했습니다. 입력한 버전 정보가 맞지않네요" );
}

TWSAStartUp::~TWSAStartUp( void )
{
	const int result							= ::WSACleanup();

	HODONG_ASSERT( 0 == result, "WSACleanup 함수 Call에 실패했습니다. 비정상 입니다" );
}
