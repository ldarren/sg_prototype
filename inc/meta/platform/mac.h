/** 
 * @file	platform/mac.h
 * @brief	Win32 Platform interfaces.
 * @author	Darren Liew<br>
 *			Copyright (C) 2009 Metaversum Asia Pte Ltd.
 *
 * Machintosh platform definitions (placeholder)
 */
#ifdef MAC

#pragma once
#ifndef _META_PLATFORM_MAC_H_
#define _META_PLATFORM_MAC_H_

#include <meta/types.h>

#include <windows.h>

namespace meta {

class Platform
{
public:
	typedef HANDLE			EventHnd;
	typedef LARGE_INTEGER	TimeType;
	typedef HMODULE			PluginHnd;
	typedef HANDLE			ThreadHnd;

	enum EMsgId
	{
		EMI_TimeOut = WAIT_TIMEOUT,
	};

public:
	Platform();
	virtual ~Platform();

	void	init();
	void	deinit();

	/// return time elapsed since last call
	/// @retval U32 elapsed time in milli seconds
	U32		timeElapse();

	/// create a system  event
	/// @retval EventHnd HANDLE of the newly created system event
	/// @param manual (in) true if event reset will be handled manually
	/// @param set (in) true if event set at the start
	EventHnd	eventCreate(bool manual, bool set);

	/// close a created system event
	/// @param evt (in) handle of system event to be destroyed
	bool		eventDestroy(const EventHnd &evt);

	/// set a created system event
	/// @retval bool true if set
	/// @param evt (in) handle of system event to be set
	bool		eventSet(const EventHnd &evt);

	/// reset a created system event
	/// @retval bool true if reset
	/// @param evt (in) handle of system event to be resetted
	bool		eventReset(const EventHnd &evt);

	/// wrapper of WaitForMultipleObjects
	/// @retval U32 return invoke event's array index count or WAIT_TIMEOUT
	/// @param count (in) number of event count in <em>list</em>
	/// @param list (in) Event list
	/// @param period (in) waiting period in milliseconds, defaut = 1 milli second
	/// @param wait (in) if true, it will wait for all event to be true before returing
	U32		eventSelect(U32 count, const EventHnd *list, U32 period=1, bool wait=false);

	/// load plugin (dll) module, module loading function assumed to be LoadModule()
	/// @retval IPlugin* pointer to IPlugin: parent to all other plugin class
	/// @param pluginName (in) path of targeted dll
	/// @param handle (out) reference of plugin handle
	IPlugin*	pluginLoad(char *pluginName, PluginHnd &handle);

	/// unload a loaded plugin (dll) module
	/// @retval bool true if plugin has been unloaded
	/// @param handle (in) unloading plugin's handle
	bool		pluginUnload(const PluginHnd &handle);

private:
	U64		fFreq__;
	BOOL	bPerfCounterAvail__;
	U64		nCountNew__;
	U64		nCountOld__;
	F64		nRemainder__;
};

} //namespace meta

#endif // _META_PLATFORM_MAC_H_

#endif // MAC