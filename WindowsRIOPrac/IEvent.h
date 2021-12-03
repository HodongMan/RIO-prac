#pragma once

#include "pch.h"

class IEvent
{
public:

	virtual ~IEvent( void );

	virtual HANDLE getHandle( void ) const noexcept = 0;
	virtual void waitOne( void ) noexcept = 0;
};