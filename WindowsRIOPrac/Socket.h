#pragma once

#include "pch.h"


class ISession;
class TIOCP;


class ISocket
{
public:
	ISocket( void );
	virtual ~ISocket( void );

	virtual HANDLE						getHandle( void ) const noexcept = 0;
	virtual SOCKET						getSocket( void ) const noexcept = 0;

	virtual std::string					getName( void ) const noexcept = 0;
	virtual std::string					getPeerName( void ) const noexcept = 0;

	virtual ISession*					getSessionConnected( void ) const noexcept = 0;


	virtual bool						transmitFile( HANDLE hFile, DWORD numberOfBytesToWrite, DWORD numberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved ) noexcept = 0;
	virtual bool						acceptEx( SOCKET acceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped ) noexcept = 0;
	virtual void						getAcceptExSocketAddress( PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, struct sockaddr** localSockAddr, LPINT localSockAddrLength, struct sockaddr** remoteSockAddr, LPINT remoteSockAddrLength ) noexcept = 0;
	virtual bool						transmitPacket( LPTRANSMIT_PACKETS_ELEMENT lpPacketArray, DWORD elementCount, DWORD sendSize, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept = 0;
	virtual bool						connectEx( const struct sockaddr FAR* name, int nameLength, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept = 0;

	virtual bool						disconnectEx( const struct sockaddr FAR* name ) noexcept = 0;

	virtual int							send( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept = 0;
	virtual int							recv( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept = 0;
};

class TWSAStartUp
{
public:
	TWSAStartUp( void );
	~TWSAStartUp( void );


private:
	WSADATA								_wsaData;
};

class TWinsockExtensions
{
public:
	TWinsockExtensions( void ) = delete;
	TWinsockExtensions( SOCKET socket );

	bool								transmitFile( HANDLE hFile, DWORD nNumberOfBytesToWrite, DWORD nNumberOfBytesPerSend, LPOVERLAPPED lpOverlapped, LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers, DWORD dwReserved ) noexcept;
	bool								transmitPacket( LPTRANSMIT_PACKETS_ELEMENT lpPacketArray, DWORD nElementCount, DWORD nSendSize, LPOVERLAPPED lpOverlapped, DWORD dwFlags ) noexcept;

	bool								acceptEx( SOCKET acceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped ) noexcept;
	bool								connectEx( const sockaddr FAR* name, int namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped ) noexcept;
	bool								disconnectEx( LPOVERLAPPED lpOverlapped, DWORD  dwFlags, DWORD  dwReserved ) noexcept;

	void								getAcceptExSockAddrs( PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, sockaddr** LocalSockaddr, LPINT LocalSockaddrLength, sockaddr** RemoteSockaddr, LPINT RemoteSockaddrLength ) const noexcept;
	
	int									send( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept;
	int									send( char* buffer, unsigned long len, LPOVERLAPPED lpOverlapped ) noexcept;

	int									recv( LPWSABUF lpBuffers, DWORD dwBufferCount, LPOVERLAPPED lpOverlapped ) noexcept;
	int									recv( char *buffer, unsigned long len, LPOVERLAPPED lpOverlapped ) noexcept;


private:
	SOCKET								_socket;
	LPFN_TRANSMITFILE					_lpfnTransmitFile;
	LPFN_ACCEPTEX						_lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS			_lpfnGetAcceptExSockAddrs;
	LPFN_TRANSMITPACKETS				_lpfnTransmitPackets;
	LPFN_CONNECTEX						_lpfnConnectEx;
	LPFN_DISCONNECTEX					_lpfnDisconnectEx;
};