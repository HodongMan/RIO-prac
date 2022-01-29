#include "pch.h"

#include "TNumber.h"


TNumber::TNumber()
{
	_outWriterCache.reserve( 16 );
	_outBuffer.reserve( RESERVE_CAPACITY );
}

bool TNumber::read( std::string& buffer, size_t startOffset, size_t* endOffset, __int64* num ) noexcept
{
	_inBuffer.clear();

	if ( buffer.size() <= startOffset )
	{
		return false;
	}

	const size_t len	= buffer[startOffset] - '0';
	startOffset			+= 1;

	if ( buffer.size() <= ( startOffset + len ) )
	{
		return false;
	}

	*endOffset			= startOffset + len;
	_inBuffer.insert( _inBuffer.begin(), buffer.begin() + startOffset, buffer.begin() + *endOffset );

	int val				= 0;

	std::stringstream inReader( _inBuffer );

	inReader >> val;
	*num				= val;

	return true;
}

bool TNumber::append( std::string& buffer, __int64 val ) noexcept
{
	_outWriterCache.clear();

	std::stringstream outWriter( _outWriterCache );

	outWriter << val;

	HODONG_ASSERT( outWriter.str().length() <= 9, "데이터 길이가 비정상 입니다." );
	HODONG_ASSERT( 0 < outWriter.str().length(), "데이터 길이가 비정상 입니다." );

	_outBuffer.clear();

	_outBuffer			+= static_cast<char>( outWriter.str().length() + '0' );
	_outBuffer			+= outWriter.str();

	buffer.insert( buffer.end(), _outBuffer.begin(), _outBuffer.end() );

	return true;
}
