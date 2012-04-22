#include <liblogger/console.h>
#include <liblogger/format_date.h>

#include <cstdarg>

namespace meta {
namespace logger {

log _instance(new logger::dateformat());

void debugf(const char* fmt,...)
{
	va_list marker = 0;
	va_start(marker, fmt);

	_instance.format(logger::ELOGL_Debug, fmt, marker);

	va_end(marker);
}

void infof(const char* fmt,...)
{
	va_list marker = 0;
	va_start(marker, fmt);

	_instance.format(logger::ELOGL_Info, fmt, marker);

	va_end(marker);
}

void warnf(const char* fmt,...)
{
	va_list marker = 0;
	va_start(marker, fmt);

	_instance.format(logger::ELOGL_Warning, fmt, marker);

	va_end(marker);
}

void errorf(const char* fmt,...)
{
	va_list marker = 0;
	va_start(marker, fmt);

	_instance.format(logger::ELOGL_Error, fmt, marker);

	va_end(marker);
}

} // namespace logger
} // namespace meta