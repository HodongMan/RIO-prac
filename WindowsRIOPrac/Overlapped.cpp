#include "pch.h"

#include "Overlapped.h"

TOverlapped::TOverlapped( ICompletionResult* iCompletion, HANDLE hEvent )
	: _result{ iCompletion }
{
	this->hEvent				= hEvent;

	reset();
}

void TOverlapped::reset( void ) noexcept
{
	HANDLE hEvent				= this->hEvent;
	LPOVERLAPPED clear			= this;

	::memset( clear, 0, sizeof( OVERLAPPED ) );

	this->hEvent				= hEvent;
}

TIOCP::TIOCP( void )
{
	_hIOCP						= ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	HODONG_ASSERT( NULL != _hIOCP, "CreateIoCompletionPort가 비정상 입니다. 실패한듯?" );

	_lastTick					= ::GetTickCount64();
}

TIOCP::~TIOCP( void )
{
	const BOOL result			= ::CloseHandle( _hIOCP );
	HODONG_ASSERT( TRUE == result, "CloseHandle가 비정상 입니다. 실패한듯?" );

	_hIOCP						= INVALID_HANDLE_VALUE;
}

void TIOCP::attach( HANDLE hChild ) noexcept
{
	HANDLE result				= ::CreateIoCompletionPort( hChild, _hIOCP, 0, 0 );
	HODONG_ASSERT( NULL != _hIOCP, "CreateIoCompletionPort가 비정상 입니다. 실패한듯?" );
}

void TIOCP::flush( void ) noexcept
{
	while ( false == flushQueue() );
}

void TIOCP::doStats( void ) noexcept
{
	_completionCount			+= 1;

	if ( 10000 < _completionCount )
	{
		unsigned __int64 tick	= ::GetTickCount();
		double rate				= static_cast<double>( _completionCount * 1000 );
		rate					/= static_cast<double>( tick - _lastTick );

		std::cout << rate << " completions/s" << std::endl;

		_lastTick				= tick;

		_completionCount		= 0;
	}
}

bool TIOCP::flushQueue( void ) noexcept
{
	LPOVERLAPPED lpOverlapped	= nullptr;
	ULONG_PTR completionKey		= 0;

	DWORD byteCount				= 0;

	BOOL status					= ::GetQueuedCompletionStatus( _hIOCP, &byteCount, &completionKey, &lpOverlapped, 0 );
	
	TOverlapped* overlapped		= reinterpret_cast<TOverlapped*>( lpOverlapped );

	if ( TRUE == status )
	{
		HODONG_ASSERT( nullptr != overlapped->_result, "해당 경우에 nullptr이 될 수 없습니다." );

		overlapped->_result->completed( status, byteCount, overlapped );

		doStats();
	}
	else
	{
		if ( nullptr != lpOverlapped )
		{
			HODONG_ASSERT( nullptr != overlapped->_result, "해당 경우에 nullptr이 될 수 없습니다." );

			overlapped->_result->completed( status, byteCount, overlapped );

			return true;
		}
		else
		{
			return false;
		}
	}
}
