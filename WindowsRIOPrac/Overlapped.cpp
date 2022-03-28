#include "pch.h"

#include "Overlapped.h"
#include "IEvent.h"
#include "TCPSocket.h"


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
	: _completionCount{ 0 }
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
		unsigned __int64 tick	= ::GetTickCount64();
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

IIOCPEvent::IIOCPEvent( void )
{

}

TIOCPEvent::TIOCPEvent( IEvent* completionsWaiting )
	: _completionsWaiting{ completionsWaiting }
{
	HODONG_ASSERT( nullptr != _completionsWaiting, "해당 Event를 nullptr 로 전달하는 것은 비정상 입니다." );
}

IEvent* TIOCPEvent::getHandle( void ) const noexcept
{
	return _completionsWaiting;
}

const TIOCP& TIOCPEvent::getCompletionPort( void ) const noexcept
{
	return _completionPort;
}

TIOCP& TIOCPEvent::getCompletionPort( void ) noexcept
{
	return _completionPort;
}

TOverlappedRecv::TOverlappedRecv( ICompletionResult* completionResult, HANDLE event )
	: TOverlapped( completionResult, event )
{

}

TOverlappedSend::TOverlappedSend( ICompletionResult* completionResult, HANDLE event )
	: TOverlapped( completionResult, event )
{

}

TOverlappedListener::TOverlappedListener( ICompletionResult* completionResult, HANDLE event )
	: TOverlapped( completionResult, event )
	, _byteCount{ 0 }
	, _addressBuffer{ 2 * AddressReserve }
{
	_acceptee									= new TCPSocket();
}

TOverlappedListener::~TOverlappedListener( void )
{
	delete _acceptee;
}

TAcceptEx::TAcceptEx(IIOCPEvent* iocp, std::string& intfc, short port, int depth)
	:_iocp( iocp )
{
	_accepter									= new TCPSocket();

	TIOCP completionPort = iocp->getCompletionPort();
	completionPort.attach( _accepter->getHandle() );

	SOCKADDR_IN addr;
	::memset( &addr, 0, sizeof( addr ) );
	
	addr.sin_addr.S_un.S_addr					= ::inet_addr( intfc.c_str() );
	addr.sin_family								= AF_INET;
	addr.sin_port								= ::htons( port );

	int result									= ::bind( _accepter->getSocket(), reinterpret_cast<LPSOCKADDR>( &addr ), sizeof( addr ) );
	HODONG_ASSERT( SOCKET_ERROR != result, "Socket Bind Error" );

	result										= ::listen( _accepter->getSocket(), SOMAXCONN );
	HODONG_ASSERT( SOCKET_ERROR != result, "Socket Listen Error" );

	for ( int ii = 0; ii < depth; ++ii )
	{
		postAccept();
	}
}

void TAcceptEx::postAccept( void ) noexcept
{
	TOverlappedListener* overlappedListener		= new TOverlappedListener( this, _iocp->getHandle()->getHandle() );

	BOOL result									= _accepter->acceptEx( overlappedListener->_acceptee->getSocket(), &overlappedListener->_addressBuffer[0], 0, TOverlappedListener::AddressReserve, TOverlappedListener::AddressReserve, &overlappedListener->_byteCount, overlappedListener );
	
	if ( FALSE == result )
	{
		HODONG_ASSERT( ERROR_IO_PENDING != ::WSAGetLastError(), "Socket AcceptEX Error" );
	}
	else
	{
		HODONG_ASSERT( 0 == overlappedListener->_byteCount, "Socket AcceptEX Error" );
	}
}

void TAcceptEx::completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) noexcept
{
	HODONG_ASSERT( 0 == byteCount, "Socket AcceptEX Error" );

	std::unique_ptr<TOverlappedListener> preOverlappedListener = std::make_unique<TOverlappedListener>( reinterpret_cast<TOverlappedListener*>( overlapped ) );
	
	TIOCP completionPort = _iocp->getCompletionPort();
	completionPort.attach( preOverlappedListener->_acceptee->getHandle() );

	accepted( status, preOverlappedListener->_acceptee );
	postAccept();
}

TConnectionEx::TConnectionEx( IIOCPEvent* iocp, std::string& intfc, std::string& remote, short port )
{

}

ISocket* TConnectionEx::getSocket( void ) const noexcept
{
	return nullptr;
}

void TConnectionEx::completed( BOOL status, DWORD byteCount, TOverlapped* overlapped ) noexcept
{

}
