#pragma once


class TUUID
{
public:
	TUUID( void );

	UUID				getUUID( void ) const noexcept;
	std::string			toString( void ) const noexcept;

private:
	UUID				_uuid;
};