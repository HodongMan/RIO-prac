#pragma once

#include "TNumber.h"

class TMessage : public std::unordered_map<std::string, std::string>
{
public:
	TMessage( void );

	bool					read( std::string& buffer, size_t startOffset, size_t* endOffset ) noexcept;
	void					append( std::string& buffer ) noexcept;
	void					softClear( void ) noexcept;
	void					merge( TMessage& merge ) noexcept;
	bool					tryGet( const std::string& key, _Out_ std::string& value ) const noexcept;
	void					dump( std::ostream& out ) noexcept;


private:
	TNumber					_number;
	std::string				_key;
	std::string				_value;
};