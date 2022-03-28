#pragma once


#include "TypeMagicNumber.h"


class TOverlapped;
class IEvent;
class ISocket;


class ICompletionResult
{
public:
	virtual void completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) noexcept = 0;
};


class TOverlapped : public OVERLAPPED
{
public:
	TOverlapped( ICompletionResult* iCompletion , HANDLE hEvent );

	void						reset( void ) noexcept;

public:
	ICompletionResult*			_result;
};


class TIOCP
{
public:
	TIOCP( void );
	virtual ~TIOCP( void );

	void						attach( HANDLE hChild ) noexcept;
	void						flush( void ) noexcept;
private:

	void						doStats( void ) noexcept;
	bool						flushQueue( void ) noexcept;

private:
	HANDLE						_hIOCP;
	unsigned __int64			_lastTick;
	__int64						_completionCount;

};

class IIOCPEvent
{
public:
	IIOCPEvent( void );

	virtual IEvent*						getHandle( void ) const noexcept = 0;
	virtual const TIOCP&				getCompletionPort( void ) const noexcept = 0;
};


class TIOCPEvent : public IIOCPEvent
{
public:
	TIOCPEvent( void ) = delete;
	TIOCPEvent( IEvent* completionsWaiting );


	virtual IEvent*						getHandle( void ) const noexcept;
	virtual const TIOCP&				getCompletionPort( void ) const noexcept override;
	TIOCP& getCompletionPort( void ) noexcept;

private:
	TIOCP								_completionPort;
	IEvent*								_completionsWaiting;
};


class TOverlappedRecv : public TOverlapped
{
public:
	TOverlappedRecv( void ) = delete;
	TOverlappedRecv( ICompletionResult* completionResult, HANDLE event );
};


class TOverlappedSend : public TOverlapped
{
public:
	TOverlappedSend( void ) = delete;
	TOverlappedSend( ICompletionResult* completionResult, HANDLE event );
};

class TOverlappedListener : public TOverlapped
{
public:
	static const unsigned char			AddressReserve			= sizeof( SOCKADDR_IN ) + 16;

public:
	TOverlappedListener( void ) = delete;
	TOverlappedListener( ICompletionResult* completionResult, HANDLE event );
	~TOverlappedListener( void );

public:
	TBytes								_addressBuffer;
	DWORD								_byteCount;
	ISocket*							_acceptee;
};


class TAcceptEx : public ICompletionResult 
{
public:
	TAcceptEx( void ) = delete;
	TAcceptEx( IIOCPEvent* iocp, std::string& intfc, short port, int depth );

	void								postAccept( void ) noexcept;
	virtual void						accepted( BOOL status, ISocket* socket ) noexcept = 0;
private:
	virtual void						completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) noexcept override;
private:

	ISocket*							_accepter;
	IIOCPEvent*							_iocp;
};


class TConnectionEx : public ICompletionResult
{
public:
	TConnectionEx( void ) = delete;
	TConnectionEx( IIOCPEvent* iocp, std::string& intfc, std::string& remote, short port );

	ISocket*							getSocket( void ) const noexcept;

private:
	virtual void						completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) noexcept override;
protected:
	virtual void						connected( BOOL status, ISocket* socket ) noexcept = 0;

private:
	ISocket*							_connector;
	IIOCPEvent*							_iocp;
	TOverlapped*						_connectNotify;
};