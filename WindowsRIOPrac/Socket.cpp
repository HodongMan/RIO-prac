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

TWinsockExtensions::TWinsockExtensions( SOCKET socket )
	: _socket{ socket }
	, _lpfnTransmitFile{ nullptr }
	, _lpfnAcceptEx{ nullptr }
	, _lpfnGetAcceptExSockAddrs{ nullptr }
	, _lpfnTransmitPackets{ nullptr }
	, _lpfnConnectEx{ nullptr }
	, _lpfnDisconnectEx{ nullptr }
{
	HODONG_ASSERT( NULL != _socket, "SOCKET�� ������ �Դϴ�." );

	DWORD dwBytes								= 0;
	int check									= 0;

	GUID GUID_WSAID_TRANSMITFILE				= WSAID_TRANSMITFILE;

	// �Լ��� Socket ��带 ����
	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_TRANSMITFILE, sizeof( GUID_WSAID_TRANSMITFILE ), &_lpfnTransmitFile, sizeof( _lpfnTransmitFile ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl TRANSMITFILE ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );

	GUID GUID_WSAID_ACCEPTEX					= WSAID_ACCEPTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_ACCEPTEX, sizeof( GUID_WSAID_ACCEPTEX ), &_lpfnAcceptEx, sizeof( _lpfnAcceptEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl ACCEPTEX ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );

	GUID GUID_WSAID_GETACCEPTEXSOCKADDRS		= WSAID_GETACCEPTEXSOCKADDRS;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_GETACCEPTEXSOCKADDRS, sizeof( GUID_WSAID_GETACCEPTEXSOCKADDRS ), &_lpfnGetAcceptExSockAddrs, sizeof( _lpfnGetAcceptExSockAddrs ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl GETACCEPTEXSOCKADDRS ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );

	GUID GUID_WSAID_TRANSMITPACKETS				= WSAID_TRANSMITPACKETS;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_TRANSMITPACKETS, sizeof( GUID_WSAID_TRANSMITPACKETS ), &_lpfnTransmitPackets, sizeof( _lpfnTransmitPackets ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl TRANSMITPACKETS ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );

	GUID GUID_WSAID_CONNECTEX					= WSAID_CONNECTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_CONNECTEX, sizeof( GUID_WSAID_CONNECTEX ), &_lpfnConnectEx, sizeof( _lpfnConnectEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl CONNECTEX ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );

	GUID GUID_WSAID_DISCONNECTEX				= WSAID_DISCONNECTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_DISCONNECTEX, sizeof( GUID_WSAID_DISCONNECTEX ), &_lpfnDisconnectEx, sizeof( _lpfnDisconnectEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl DISCONNECTEX ��带 �����ϴµ� ���� �߽��ϴ�. ������ �Դϴ�." );
}

bool TWinsockExtensions::transmitFile( HANDLE hFile, DWORD nNumberOfBytesToWrite, DWORD nNumberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnTransmitFile, "TransmitFile �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnTransmitFile( _socket, hFile, nNumberOfBytesToWrite, nNumberOfBytesPerSend, lpOverlapped, lpTransmitBuffers, dwReserved );
}

bool TWinsockExtensions::transmitPacket( LPTRANSMIT_PACKETS_ELEMENT lpPacketArray, DWORD nElementCount, DWORD nSendSize, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnTransmitFile, "TransmitPackets �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnTransmitPackets( _socket, lpPacketArray, nElementCount, nSendSize, lpOverlapped, dwFlags );
}

bool TWinsockExtensions::acceptEx( SOCKET acceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnAcceptEx, "AcceptEx �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnAcceptEx( _socket, acceptSocket, lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped );
}

bool TWinsockExtensions::connectEx( const sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnConnectEx, "ConnectEx �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnConnectEx( _socket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped );
}

bool TWinsockExtensions::disconnectEx( LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD dwReserved ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnDisconnectEx, "DisconnectEx �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnDisconnectEx( _socket, lpOverlapped, dwFlags, dwReserved );
}

void TWinsockExtensions::getAcceptExSockAddrs( PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, sockaddr** LocalSockaddr, LPINT LocalSockaddrLength, sockaddr** RemoteSockaddr, LPINT RemoteSockaddrLength ) const noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	//HODONG_ASSERT( nullptr != _lpfnGetAcceptExSockAddrs, "GetAcceptExSockAddrs �Լ� ������ ���� ������ �Դϴ�." );

	return _lpfnGetAcceptExSockAddrs( lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, LocalSockaddr, LocalSockaddrLength, RemoteSockaddr, RemoteSockaddrLength );
}

int TWinsockExtensions::send( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );

	return ::WSASend( _socket, lpBuffers, dwBufferCount, NULL, 0, lpOverlapped, NULL );
}

int TWinsockExtensions::send(char* buffer, unsigned long len, LPOVERLAPPED lpOverlapped) noexcept
{
	HODONG_ASSERT( nullptr != buffer, "buffer ���� nullptr �Դϴ�.");
	HODONG_ASSERT( 0 < len, "buffer size ���� ������ �Դϴ�.");

	WSABUF wsaBuf;
	wsaBuf.buf									= buffer;
	wsaBuf.len									= len;

	return send( &wsaBuf, 1, lpOverlapped );
}

int TWinsockExtensions::recv( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET ���� ������ �Դϴ�." );
	
	DWORD flags									= 0;
	
	return ::WSARecv( _socket, lpBuffers, dwBufferCount, NULL, &flags, lpOverlapped, NULL );
}

int TWinsockExtensions::recv( char* buffer, unsigned long len, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != buffer, "buffer ���� nullptr �Դϴ�.");
	HODONG_ASSERT( 0 < len, "buffer size ���� ������ �Դϴ�.");

	WSABUF wsaBuf;
	wsaBuf.buf									= buffer;
	wsaBuf.len									= len;

	return recv( &wsaBuf, 1, lpOverlapped );
}
