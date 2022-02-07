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

	HODONG_ASSERT( 0 == result, "WSAStartup �Լ� Call�� �����߽��ϴ�. ������ �Դϴ�" );
	HODONG_ASSERT( 2 == LOBYTE( _wsaData.wVersion ) || 2 == HIBYTE( _wsaData.wVersion ), "WSAStartup �Լ� Call�� �����߽��ϴ�. �Է��� ���� ������ �����ʳ׿�" );
}

TWSAStartUp::~TWSAStartUp( void )
{
	const int result							= ::WSACleanup();

	HODONG_ASSERT( 0 == result, "WSACleanup �Լ� Call�� �����߽��ϴ�. ������ �Դϴ�" );
}
