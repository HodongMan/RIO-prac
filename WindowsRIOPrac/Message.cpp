#include "pch.h"

#include "Message.h"

TMessage::TMessage( void )
{
	_key.clear();
	_value.clear();
}

bool TMessage::read( std::string& buffer, size_t startOffset, size_t* endOffset ) noexcept
{
	__int64 count					= 0;

	if ( false == _number.read( buffer, startOffset, &startOffset, &count ) )
	{
		return false;
	}

	__int64 keyLength				= 0;
	__int64 valueLength				= 0;

	for ( __int64 ii = 0; ii < count; ++ii )
	{
		keyLength					= 0;

		if ( false == _number.read( buffer, startOffset, &startOffset, &keyLength ) )
		{
			return false;
		}

		_key.clear();

		_key.insert( _key.begin(), buffer.begin() + startOffset, buffer.begin() + ( startOffset + static_cast<size_t>( keyLength ) ) );

		startOffset					+= static_cast<size_t>( keyLength );
		if ( buffer.size() <= startOffset )
		{
			return false;
		}

		valueLength					= 0;

		if ( false == _number.read( buffer, startOffset, &startOffset, &valueLength ) )
		{
			return false;
		}

		_value.clear();

		_value.insert( _value.begin(), buffer.begin() + startOffset, buffer.begin() + ( startOffset + static_cast<size_t>( valueLength ) ) );

		startOffset					+= static_cast<size_t>( valueLength );
		if ( buffer.size() <= startOffset )
		{
			return false;
		}

		( *this )[ _key ]			= _value;

	}

	*endOffset						= startOffset;

	return true;
}

void TMessage::append( std::string& buffer ) noexcept
{
	_number.append( buffer, size() );

	for ( const_iterator iter = begin(); end() != iter; ++iter )
	{
		const std::string& key		= iter->first;

		_number.append( buffer, key.length() );
		buffer.insert( buffer.end(), key.begin(), key.end() );

		const std::string& value	= iter->second;

		_number.append( buffer, value.length() );
		buffer.insert( buffer.end(), value.begin(), value.end() );
	}
}

void TMessage::softClear( void ) noexcept
{
	for ( iterator iter = begin(); end() != iter; ++iter )
	{
		iter->second.clear();
	}
}

void TMessage::merge( TMessage& merge ) noexcept
{
	for (iterator iter = begin(); end() != iter; ++iter)
	{
		( *this )[ iter->first ]	= iter->second;
	}
}

bool TMessage::tryGet( const std::string& key, std::string& value ) const noexcept
{
	auto iter						= find( key );
	
	if ( 0 < iter->second.length() ) 
	{
		value						= iter->second;
		return true;
	}

	return false;
}

void TMessage::dump( std::ostream& out ) noexcept
{
	for ( const_iterator iter = begin(); end() != iter; ++iter ) 
	{
		out << iter->first << ": " << iter->second << std::endl;
	}

	out << std::endl;
}
