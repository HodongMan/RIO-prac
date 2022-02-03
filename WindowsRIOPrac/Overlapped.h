#pragma once

class TOverlapped;


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