// TODO: log file rotating, allow log file record non-stop
#include <liblogger/log.h>
#include <string>
#include <vector>
#include <assert.h>

using namespace meta::logger;

log::log(basicformat * format) 
:	Parent(format),
	_severity(ELOGL_Debug), _pformat(format)
{
	_buffer.push_back('\0');
    format->setLog( this );
}

log::~log()
{
	if (_pformat) delete _pformat; _pformat = 0;
    std::vector< sink* >::iterator i = _sinks.begin(), end = _sinks.end();
    for ( ; i != end; ++i )
    {
        delete *i;
    }

	_sinks.clear();
}

std::ostream& log::operator <<(level lvl)
{
	_severity = severity(lvl);
	return *this;
}

level log::severity(level lvl)
{
	if ((lvl > ELOGL_Debug) || (lvl < ELOGL_Fatal)) return lvl;

	return ELOGL_Debug;
}

bool log::add_sink(sink * s)
{
	if (!s) return false;

	s->_level = severity(s->_level);
    
    // check if there is already a sink with requested sink name
    std::vector< sink * >::iterator i = _sinks.begin(), end = _sinks.end();
    for ( ; i != end; ++i )
    {
        if ( ( *i )->_name == s->_name )
        {
			assert( NULL && "sink name already exist!" );
            //log_error << "sink name '" << sinkname << "' already exists!"  << std::endl;
            return false;
        }
    }

    _sinks.push_back( s );

	return true;
}

bool log::remove_sink(std::string name)
{
    std::vector< sink* >::iterator i = _sinks.begin(), end = _sinks.end();
    for ( ; i != end; ++i )
    {
        if ( ( *i )->_name == name )
        {
            break;
        }
    }

	sink *s = *i;
	_sinks.erase(i);
	delete s;

    assert( NULL && "sink name does not exist!" );
	return false;
}

void log::out( const std::string& msg )
{
    std::vector< sink* >::iterator i = _sinks.begin(), end = _sinks.end();
    for ( ; i != end; ++i )
    {
        if ( ( *i )->_level <= _severity )
        {
			( *i )->out(msg);
        }
    }
}

void  log::format(level lvl, const char* fmt, va_list marker)
{
	_severity = severity(lvl);

	// Get formatted string length adding one for the NULL
	size_t len = _vscprintf(fmt, marker) + 1;

	// Create a char vector to hold the formatted string.
	_buffer.reserve(len);
	if ( _vsnprintf_s(&_buffer[0], _buffer.capacity(), len, fmt, marker) > 0 )
	{
	//	out(&_buffer[0]);
		*this << &_buffer[0];
	}
}
