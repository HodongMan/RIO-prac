#pragma once


class TMessage;


class ISession
{
public:

	ISession( void );

	virtual __int64				getSessionID( void ) noexcept = 0;
	virtual void				send( TMessage& message ) noexcept = 0;

	virtual bool				isServerSession( void ) const noexcept = 0;
	virtual std::string			getDescription( void ) const noexcept = 0;
};

class ISessionConnected
{
public:
	virtual void				connected( BOOL status, ISession* session ) = 0;
};