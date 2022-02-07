#pragma once


class TOverlapped;
class IEvent;



class ICompletionResult
{
public:
	virtual void completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) = 0;
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

	//virtual IEvent						getHandle( void ) const noexcept = 0;
	virtual const TIOCP&				getCompletionPort( void ) const noexcept = 0;
};


class TIOCPEvent : public IIOCPEvent
{
public:
	TIOCPEvent( void ) = delete;
	TIOCPEvent( IEvent* completionsWaiting );


	IEvent*								getHandle( void ) const noexcept;
	virtual const TIOCP&				getCompletionPort( void ) const noexcept override;

private:
	TIOCP								_completionPort;
	IEvent*								_completionsWaiting;
};