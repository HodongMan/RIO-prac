#pragma once

#include "IEvent.h"


class TEvent : public IEvent
{
public:
	TEvent( void );
	virtual ~TEvent( void );

	HANDLE					getHandle( void ) const noexcept override;
	void					waitOne( void ) noexcept override;

private:

	HANDLE					_eventHandle;
};