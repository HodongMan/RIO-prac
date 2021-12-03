#include "pch.h"

#include "Event.h"

TEvent::TEvent( void )
	: _eventHandle{ INVALID_HANDLE_VALUE }
{
	_eventHandle				= ::CreateEvent( NULL, FALSE, FALSE, NULL );

	HODONG_ASSERT( INVALID_HANDLE_VALUE != _eventHandle, "CreateEvent Failed... ������ �Դϴ�." );
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

	HODONG_ASSERT( WAIT_OBJECT_0 == waitResult, "WaitForSingleObject�� ����� ������ �Դϴ�." );
}
