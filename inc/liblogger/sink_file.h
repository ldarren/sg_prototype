// TODO: change to unicode
#ifndef _LOGGER_SINK_FILE_H_
#define _LOGGER_SINK_FILE_H_

#include <liblogger/sink.h>
#include <fstream>

namespace meta {
namespace logger {

class fsink : public sink
{
public:
	fsink(const std::string& name, const std::string& fname, level lvl)
		:	sink(name, NULL, lvl),
			_pfstream(NULL)
	{
		_pfstream = new std::fstream;
		_pfstream->open( fname.c_str(), std::ios_base::binary | std::ios_base::out );
		if ( !*_pfstream )
		{   
			delete _pfstream;
			return;
		}
		_pstream = _pfstream;
	}

	virtual ~fsink()
	{
		if (_pfstream) 
		{
			_pfstream->close();
			delete _pfstream;
		}
	}

protected:
	std::fstream * _pfstream;
};

} // namespace logger
} // namespace meta

#endif // _LOGGER_SINK_FILE_H_