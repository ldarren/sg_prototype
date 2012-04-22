#include <liblogger/log.h>
#include <liblogger/format.h>
#include <string>
#include <sstream>

using namespace meta::logger;

basicformat::basicformat()
:	_plog(NULL),
	_repeated_msg_count(0)
{
}

basicformat::~basicformat()
{
	if (_repeated_msg_count > 0)
		print_repeated();
}

void basicformat::setLog( log * plog )
{
	if (plog) _plog = plog;
}

std::string &	basicformat::prefix()
{
	switch ( _plog->severity() )
	{
	default:
	case ELOGL_Debug:
		_prefix += "[debug]   ";
		break;
	case ELOGL_Info:
		_prefix += "[info]    ";
		break;
	case ELOGL_Warning:
		_prefix += "[warning] ";
		break;
	case ELOGL_Error:
		_prefix += "[error]   ";
		break;
	case ELOGL_Fatal:
		_prefix += "[fatal]   ";
		break;
	}

	return _prefix;
}

std::string &	basicformat::postfix()
{
#ifdef _WINDOWS
	_postfix += "\r\n";
#else
	_postfix += "\n";
#endif
	return _postfix;
}

std::basic_ios< char >::int_type basicformat::overflow( int_type c )
{
	std::basic_ios< char >::int_type ret = std::char_traits< char >::not_eof( c );

	if( !std::char_traits< char >::eq_int_type( c, std::char_traits< char >::eof() ) )
	{
		if( c == '\n')
		{
			if (_repeated_msg == _msg)
			{
				if (_msg == "") return ret;
				_repeated_msg_count++;
				_msg = "";
				return ret;
			}
			else if (_repeated_msg_count > 0) // repeated msg stop
			{
				print_repeated();
			}
			_repeated_msg = _msg;

			_prefix = "";
			_postfix = "";
			_plog->out( prefix() + _msg + postfix());
			_msg = "";
		}
		else
		{
			_msg += c;
		}
	}
	return ret;
}

void basicformat::print_repeated()
{
	_prefix = "";

	std::ostringstream oss;
	oss << "previous message repeated: " << _repeated_msg_count << " times";
	_postfix = oss.str();

	_plog->out( prefix() + postfix());
	_repeated_msg_count = 0;
}
