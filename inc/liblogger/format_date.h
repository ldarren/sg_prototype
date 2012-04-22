#ifndef _LOGGER_DATE_STREAMBUF_H_
#define _LOGGER_DATE_STREAMBUF_H_

#include <iostream>

namespace meta {
namespace logger {

class log;

class dateformat : public basicformat
{
	typedef basicformat Parent;

public:
	dateformat();
	virtual ~dateformat();

protected:
	virtual std::string &	prefix();
	virtual std::string &	postfix();
};

} // namespace logger
} // namespace meta

#endif // _LOGGER_DATE_STREAMBUF_H_