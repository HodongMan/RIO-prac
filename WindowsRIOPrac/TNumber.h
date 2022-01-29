#pragma once


class TNumber
{
public:
	TNumber();

	bool					read( std::string& buffer, size_t startOffset, size_t* endOffset, __int64* num ) noexcept;
	bool					append( std::string& buffer, __int64 val ) noexcept;

private:
	enum
	{
		RESERVE_CAPACITY = 128
	};
private:

	std::string				_outBuffer;
	std::string				_outWriterCache;
	std::string				_inBuffer;
};