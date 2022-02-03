#pragma once


class ISession;
class Socket;


class ISessionManager
{
public:
	ISessionManager( void );

	virtual ISession*				makeNewSession( const bool status, const bool isServerSession, Socket* socket ) noexcept = 0;
};