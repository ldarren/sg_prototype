#ifndef _LOGGER_CONSOLE_H_
#define _LOGGER_CONSOLE_H_

#include "./log.h"
#include "./sink_file.h"
#include "./sink_syslog.h"

namespace meta {
namespace logger {

extern log	_instance;

void	debugf(const char* fmt,...);
void	infof(const char* fmt,...);
void	warnf(const char* fmt,...);
void	errorf(const char* fmt,...);

} // namespace logger
} // namespace meta

#endif // _LOGGER_CONSOLE_H_