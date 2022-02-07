#pragma once


class TMessage;
class ISession;
class Socket;


class ISessionManager
{
public:
	ISessionManager( void );

	virtual ISession*				makeNewSession( const bool status, const bool isServerSession, Socket* socket ) noexcept = 0;
};

class IProcessMessage
{
public:
	IProcessMessage( void );
	virtual void					process( __int64 sessionId, TMessage& message ) noexcept = 0;
};