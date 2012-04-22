#ifndef _LOGGER_SINK_H_
#define _LOGGER_SINK_H_

#include <iostream>
#include <string>
#include <liblogger/log.h>

namespace meta {
namespace logger {

class sink
{
public:

	sink( const std::string& name, std::ostream* pstream, level lvl ) :
		_name( name ),
		_pstream( pstream ), 
		_level( lvl )
	{}
                                            
	virtual	~sink() {}

	virtual void out( const std::string & msg) 
	{
        *( _pstream ) << msg;
        _pstream->flush();
	}

	std::string		_name;

	std::ostream *	_pstream;

	level			_level;
};

} // namespace logger
} // namespace meta

#endif // _LOGGER_SINK_H_