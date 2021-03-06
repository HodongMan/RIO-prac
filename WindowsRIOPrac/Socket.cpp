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

TWinsockExtensions::TWinsockExtensions( SOCKET socket )
	: _socket{ socket }
	, _lpfnTransmitFile{ nullptr }
	, _lpfnAcceptEx{ nullptr }
	, _lpfnGetAcceptExSockAddrs{ nullptr }
	, _lpfnTransmitPackets{ nullptr }
	, _lpfnConnectEx{ nullptr }
	, _lpfnDisconnectEx{ nullptr }
{
	HODONG_ASSERT( NULL != _socket, "SOCKET이 비정상 입니다." );

	DWORD dwBytes								= 0;
	int check									= 0;

	GUID GUID_WSAID_TRANSMITFILE				= WSAID_TRANSMITFILE;

	// 함수가 Socket 모드를 제어
	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_TRANSMITFILE, sizeof( GUID_WSAID_TRANSMITFILE ), &_lpfnTransmitFile, sizeof( _lpfnTransmitFile ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl TRANSMITFILE 모드를 제어하는데 실패 했습니다. 비정상 입니다." );

	GUID GUID_WSAID_ACCEPTEX					= WSAID_ACCEPTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_ACCEPTEX, sizeof( GUID_WSAID_ACCEPTEX ), &_lpfnAcceptEx, sizeof( _lpfnAcceptEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl ACCEPTEX 모드를 제어하는데 실패 했습니다. 비정상 입니다." );

	GUID GUID_WSAID_GETACCEPTEXSOCKADDRS		= WSAID_GETACCEPTEXSOCKADDRS;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_GETACCEPTEXSOCKADDRS, sizeof( GUID_WSAID_GETACCEPTEXSOCKADDRS ), &_lpfnGetAcceptExSockAddrs, sizeof( _lpfnGetAcceptExSockAddrs ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl GETACCEPTEXSOCKADDRS 모드를 제어하는데 실패 했습니다. 비정상 입니다." );

	GUID GUID_WSAID_TRANSMITPACKETS				= WSAID_TRANSMITPACKETS;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_TRANSMITPACKETS, sizeof( GUID_WSAID_TRANSMITPACKETS ), &_lpfnTransmitPackets, sizeof( _lpfnTransmitPackets ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl TRANSMITPACKETS 모드를 제어하는데 실패 했습니다. 비정상 입니다." );

	GUID GUID_WSAID_CONNECTEX					= WSAID_CONNECTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_CONNECTEX, sizeof( GUID_WSAID_CONNECTEX ), &_lpfnConnectEx, sizeof( _lpfnConnectEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl CONNECTEX 모드를 제어하는데 실패 했습니다. 비정상 입니다." );

	GUID GUID_WSAID_DISCONNECTEX				= WSAID_DISCONNECTEX;

	check										= ::WSAIoctl( _socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GUID_WSAID_DISCONNECTEX, sizeof( GUID_WSAID_DISCONNECTEX ), &_lpfnDisconnectEx, sizeof( _lpfnDisconnectEx ), &dwBytes, NULL, NULL );
	HODONG_ASSERT( SOCKET_ERROR != check, "WSAIoctl DISCONNECTEX 모드를 제어하는데 실패 했습니다. 비정상 입니다." );
}

bool TWinsockExtensions::transmitFile( HANDLE hFile, DWORD nNumberOfBytesToWrite, DWORD nNumberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnTransmitFile, "TransmitFile 함수 포인터 값이 비정상 입니다." );

	return _lpfnTransmitFile( _socket, hFile, nNumberOfBytesToWrite, nNumberOfBytesPerSend, lpOverlapped, lpTransmitBuffers, dwReserved );
}

bool TWinsockExtensions::transmitPacket( LPTRANSMIT_PACKETS_ELEMENT lpPacketArray, DWORD nElementCount, DWORD nSendSize, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnTransmitFile, "TransmitPackets 함수 포인터 값이 비정상 입니다." );

	return _lpfnTransmitPackets( _socket, lpPacketArray, nElementCount, nSendSize, lpOverlapped, dwFlags );
}

bool TWinsockExtensions::acceptEx( SOCKET acceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnAcceptEx, "AcceptEx 함수 포인터 값이 비정상 입니다." );

	return _lpfnAcceptEx( _socket, acceptSocket, lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped );
}

bool TWinsockExtensions::connectEx( const sockaddr* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnConnectEx, "ConnectEx 함수 포인터 값이 비정상 입니다." );

	return _lpfnConnectEx( _socket, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped );
}

bool TWinsockExtensions::disconnectEx( LPOVERLAPPED lpOverlapped, DWORD dwFlags, DWORD dwReserved ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnDisconnectEx, "DisconnectEx 함수 포인터 값이 비정상 입니다." );

	return _lpfnDisconnectEx( _socket, lpOverlapped, dwFlags, dwReserved );
}

void TWinsockExtensions::getAcceptExSockAddrs( PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, sockaddr** LocalSockaddr, LPINT LocalSockaddrLength, sockaddr** RemoteSockaddr, LPINT RemoteSockaddrLength ) const noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	//HODONG_ASSERT( nullptr != _lpfnGetAcceptExSockAddrs, "GetAcceptExSockAddrs 함수 포인터 값이 비정상 입니다." );

	return _lpfnGetAcceptExSockAddrs( lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, LocalSockaddr, LocalSockaddrLength, RemoteSockaddr, RemoteSockaddrLength );
}

int TWinsockExtensions::send( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );

	return ::WSASend( _socket, lpBuffers, dwBufferCount, NULL, 0, lpOverlapped, NULL );
}

int TWinsockExtensions::send(char* buffer, unsigned long len, LPOVERLAPPED lpOverlapped) noexcept
{
	HODONG_ASSERT( nullptr != buffer, "buffer 값이 nullptr 입니다.");
	HODONG_ASSERT( 0 < len, "buffer size 값이 비정상 입니다.");

	WSABUF wsaBuf;
	wsaBuf.buf									= buffer;
	wsaBuf.len									= len;

	return send( &wsaBuf, 1, lpOverlapped );
}

int TWinsockExtensions::recv( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( INVALID_SOCKET != _socket, "SOCKET 값이 비정상 입니다." );
	
	DWORD flags									= 0;
	
	return ::WSARecv( _socket, lpBuffers, dwBufferCount, NULL, &flags, lpOverlapped, NULL );
}

int TWinsockExtensions::recv( char* buffer, unsigned long len, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != buffer, "buffer 값이 nullptr 입니다.");
	HODONG_ASSERT( 0 < len, "buffer size 값이 비정상 입니다.");

	WSABUF wsaBuf;
	wsaBuf.buf									= buffer;
	wsaBuf.len									= len;

	return recv( &wsaBuf, 1, lpOverlapped );
}

TSocket::TSocket( int type, int protocol, int flags )
{
	_socket										= ::WSASocket( AF_INET, type, protocol, NULL, 0, flags );
	HODONG_ASSERT( INVALID_SOCKET != _socket, "WSASocket이 실패해서 값이 비정상 입니다." );

	_winsockExtensions							= new TWinsockExtensions( _socket );
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	BOOL value									= TRUE;

	const int error								= ::setsockopt( _socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>( &value ), sizeof( value ) );
	HODONG_ASSERT( SOCKET_ERROR != error, "setsockopt이 실패해서 값이 비정상 입니다." );
}

TSocket::~TSocket( void )
{
	delete _winsockExtensions;

	_winsockExtensions							= nullptr;

	const int error								= ::closesocket( _socket );
	HODONG_ASSERT( 0 == error, "closesocket이 실패해서 값이 비정상 입니다." );

	_socket										= NULL;
}

HANDLE TSocket::getHandle( void)  const noexcept
{
	return reinterpret_cast<HANDLE>( _socket );
}

SOCKET TSocket::getSocket( void ) const noexcept
{
	return _socket;
}

std::string TSocket::getName( void ) const noexcept
{
	return std::string();
}

std::string TSocket::getPeerName( void ) const noexcept
{
	return std::string();
}

ISession* TSocket::getSessionConnected( void ) const noexcept
{
	return _session;
}

bool TSocket::transmitFile( HANDLE hFile, DWORD numberOfBytesToWrite, DWORD numberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->transmitFile( hFile, numberOfBytesToWrite, numberOfBytesPerSend, lpOverlapped, lpTransmitBuffers, dwReserved );
}

bool TSocket::acceptEx( SOCKET acceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->acceptEx( acceptSocket, lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped );
}

void TSocket::getAcceptExSocketAddress( PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, sockaddr** localSockAddr, LPINT localSockAddrLength, sockaddr** remoteSockAddr, LPINT remoteSockAddrLength ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->getAcceptExSockAddrs( lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, localSockAddr, localSockAddrLength, remoteSockAddr, remoteSockAddrLength );
}

bool TSocket::transmitPacket( LPTRANSMIT_PACKETS_ELEMENT lpPacketArray, DWORD elementCount, DWORD sendSize, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->transmitPacket( lpPacketArray, elementCount, sendSize, lpOverlapped, dwFlags );
}

bool TSocket::connectEx( const sockaddr* name, int nameLength, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->connectEx( name, nameLength, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped );
}

bool TSocket::disconnectEx( LPOVERLAPPED lpOverlapped, DWORD  dwFlags, DWORD  dwReserved ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->disconnectEx( lpOverlapped, dwFlags, dwReserved );
}

int TSocket::send( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->send( lpBuffers, dwBufferCount, lpOverlapped );
}

int TSocket::recv( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept
{
	HODONG_ASSERT( nullptr != _winsockExtensions, "TWinsockExtensions 메모리 할당이 실패해서 비정상 입니다." );

	return _winsockExtensions->recv( lpBuffers, dwBufferCount, lpOverlapped );
}
