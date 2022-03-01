#include "pch.h"

#include "TCPSocket.h"

TCPSocket::TCPSocket( void )
	: TSocket( SOCK_STREAM, IPPROTO_TCP, WSA_FLAG_OVERLAPPED )
{

}
