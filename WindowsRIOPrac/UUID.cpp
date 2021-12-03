#include "pch.h"

#include "UUID.h"

TUUID::TUUID(void)
	: _uuid{ 0 }
{
	RPC_STATUS status			= ::UuidCreate( &_uuid );

	HODONG_ASSERT( RPC_S_OK == status, "RPC status가 비정상 입니다. 실패한듯?" );
}

UUID TUUID::getUUID( void ) const noexcept
{
	return _uuid;
}

std::string TUUID::toString( void ) const noexcept
{
	RPC_CSTR buffer				= nullptr;
	RPC_STATUS status			= ::UuidToStringA( &_uuid, &buffer );

	HODONG_ASSERT( RPC_S_OK == status, "RPC status가 비정상 입니다. 실패한듯?" );

	std::string outValue( reinterpret_cast<char*>( buffer ) );
	status						= ::RpcStringFreeA( &buffer );

	HODONG_ASSERT( RPC_S_OK == status, "RPC status가 비정상 입니다. 실패한듯?" );

	return outValue;
}
