// TODO: fix this, current gabage was sent to syslog
#ifndef _LOGGER_SINK_SYSLOG_H_
#define _LOGGER_SINK_SYSLOG_H_

#include <liblogger/sink.h>
#include <windows.h>

namespace meta {
namespace logger {

class sys_sink : public sink
{
public:
	sys_sink(const std::string& name, level lvl)
		:	sink(name, NULL, lvl),
			bIsOpen(false)
	{
		mStrSource = std::string(__FILE__);
	}

	virtual ~sys_sink()
	{
		if(true == bIsOpen) deRegisterEventLog();
	}

	virtual void out( const std::string & msg) 
	{
		if(false == bIsOpen) 
		{
			if(! registerEventLog(mStrSource)) 
			{
				return;
			}
		}

		const char * pStr;
		char * pOutString;
		
		pOutString = new char[msg.length() + 1];
		
		strcpy(pOutString, msg.c_str());
		
		pStr = pOutString;
		
		if(! ReportEvent(hEventLogHandle,
				EVENTLOG_ERROR_TYPE,
				1500,
				0,
				NULL,
				1,
				0,
				(LPCTSTR *) &pStr,
				NULL)) 
		{
			return;
		}

		return;
	}

protected:

	bool registerEventLog(std::string source) 
	{
		if(NULL == (hEventLogHandle = RegisterEventSourceA(NULL, mStrSource.c_str()))) 
		{
			return false;
		}

		bIsOpen = true;

		return true;
	}

	bool deRegisterEventLog() 
	{
		if(bIsOpen == false || NULL == hEventLogHandle) 
		{
			return false;
		}

		DeregisterEventSource(hEventLogHandle);
		hEventLogHandle = NULL;
		bIsOpen = false;

		return true;
	}

private:
	HANDLE hEventLogHandle;
	bool bIsOpen;
	std::string mStrSource;
};

} // namespace logger
} // namespace meta

#endif // _LOGGER_SINK_SYSLOG_H_