#pragma once

inline void HODONG_ASSERT( const bool condition, const char* errorMessage ) noexcept
{
	if ( false == condition )
	{
		::__debugbreak();
	}	
}