#include "pch.h"

#include "Event.h"

TEvent::TEvent( void )
	: _eventHandle{ INVALID_HANDLE_VALUE }
{
	_eventHandle				= ::CreateEvent( NULL, FALSE, FALSE, NULL );

	HODONG_ASSERT( INVALID_HANDLE_VALUE != _eventHandle, "CreateEvent Failed... 비정상 입니다." );
}

TEvent::~TEvent( void )
{
	const BOOL result			= ::CloseHandle( _eventHandle );
}

HANDLE TEvent::getHandle( void ) const noexcept
{
	return _eventHandle;
}

void TEvent::waitOne( void ) noexcept
{
	const DWORD waitResult		= ::WaitForSingleObject( _eventHandle, INFINITE );

	HODONG_ASSERT( WAIT_OBJECT_0 == waitResult, "WaitForSingleObject의 결과가 비정상 입니다." );
}
