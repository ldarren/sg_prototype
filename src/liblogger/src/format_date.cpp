#include <liblogger/log.h>
#include <liblogger/format_date.h>
#include <string>
#include <time.h>

using namespace meta::logger;

dateformat::dateformat() :
	Parent()
{
}

dateformat::~dateformat()
{
}

std::string &	dateformat::prefix()
{
    struct tm *today;
    time_t ltime;

	char datestr[80];
	char timestr[80];

	time( &ltime );
	today = localtime( &ltime );
	sprintf(datestr, "%02d/%02d",
		today->tm_mon + 1,
		today->tm_mday);
	_prefix += datestr;
	sprintf(timestr, " %02d:%02d:%02d ",
		today->tm_hour,
		today->tm_min,
		today->tm_sec);
	_prefix += timestr;

	return Parent::prefix();
}

std::string &	dateformat::postfix()
{
	return Parent::postfix();
}
