#ifndef _LOGGER_STREAMBUF_H_
#define _LOGGER_STREAMBUF_H_

#include <iostream>

namespace meta {
namespace logger {

class log;

class basicformat : public std::basic_streambuf< char >
{
public:
	basicformat();
	virtual ~basicformat();

	void					setLog( log * plog );

protected:

	virtual int_type		overflow( int_type c );

	virtual std::string &	prefix();
	virtual std::string &	postfix();

	log *				_plog;

	std::string			_msg;

	std::string			_repeated_msg;
	int					_repeated_msg_count;

	std::string			_prefix;
	std::string			_postfix;

	virtual	void			print_repeated();
};

} // namespace logger
} // namespace meta;

#endif // _LOGGER_STREAMBUF_H_