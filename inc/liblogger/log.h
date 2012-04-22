#ifndef _LOGGER_LOG_H_
#define _LOGGER_LOG_H_

namespace meta {
namespace logger {

enum level
{
	ELOGL_Debug		= 0x10,
	ELOGL_Info		= 0x20,
	ELOGL_Warning	= 0x30,
	ELOGL_Error		= 0x40,
	ELOGL_Fatal		= 0x50
};

} // namespace logger
} // namespace meta

#include "./format.h"
#include "./sink.h"
#include <iostream>
#include <vector>

namespace meta {
namespace logger {

class log : public std::basic_ostream< char >
{
	typedef std::basic_ostream< char > Parent;

public:
	explicit log(basicformat * format);
	virtual ~log();

	std::ostream& operator <<(level lvl);

	bool add_sink(sink * s);

	bool remove_sink(std::string name);
	// 
    void out( const std::string& msg );
	void format(level lvl, const char* fmt, va_list marker);

	level severity() const { return _severity; }

protected:
	level				_severity;
	std::vector<sink *>	_sinks;
	basicformat			*_pformat;
	std::vector<char>	_buffer;
	level severity(level lvl);
};

} // namespace logger
} // namespace meta;

#endif // _LOGGER_LOG_H_