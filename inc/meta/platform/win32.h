/** 
 * @file	platform/win32.h
 * @brief	Win32 Platform interfaces.
 * @author	Darren Liew<br>
 *			Copyleft (C) 2009 RaceHazard Pte Ltd.
 *
 * Windows 32 platform definitions
 */
#ifdef WIN32

#pragma once
#ifndef _META_PLATFORM_WIN32_H_
#define _META_PLATFORM_WIN32_H_

#include <meta/types.h>

#include <windows.h>

namespace meta {

struct IPlugin;
struct ModuleCfg;

class Platform
{
public:
	typedef LARGE_INTEGER		TimeType;
	typedef HANDLE				EventHnd;
	typedef HMODULE				PluginHnd;
	typedef HANDLE				ThreadHnd;
	typedef HWND				WindowHnd;
	typedef HINSTANCE			InstanceHnd;
	typedef HDC					ContextHnd;
	typedef HGLRC				RendererHnd;
	typedef CRITICAL_SECTION	MutexHnd;
	typedef DEVMODEA			DeviceMode;
	typedef WNDPROC				MessageFunc;
	typedef unsigned int (__stdcall *ThreadFunc)(void*);

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
	U32			timeElapse();

	/// sleep
	/// @param millisec milli seconds
	void		timeSleep(U32 millisec);

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
	U32			eventSelect(U32 count, const EventHnd *list, U32 period=1, bool wait=false);

	/// create a mutex
	/// @retval MutexHnd the created mutex handle, same as the <em>hnd</em> argument
	/// @param hnd the place to store the new mutex handle
	MutexHnd&	mutexCreate(MutexHnd &hnd);

	/// destroy a mutex
	/// @param hnd mutex handle to be destroyed
	void		mutexDestroy(MutexHnd &hnd);

	/// lock a thread
	/// @param hnd lock a thread with this mutex
	void		mutexLock(MutexHnd &hnd);

	/// unlock a mutex
	/// @retval hnd unlock a therad with this mutex
	void		mutexUnlock(MutexHnd &hnd);

	/// load plugin (dll) module, module loading function assumed to be LoadModule()
	/// @retval IPlugin* pointer to IPlugin: parent to all other plugin class
	/// @param pluginName (in) path of targeted dll
	/// @param handle (out) reference of plugin handle
	IPlugin*	pluginLoad(char *pluginName, PluginHnd &handle);

	/// unload a loaded plugin (dll) module
	/// @retval bool true if plugin has been unloaded
	/// @param handle (in) unloading plugin's handle
	bool		pluginUnload(const PluginHnd &handle);

	/// create a thread, wrapper of _beginthreadex
	/// @retval ThreadHnd handle of the thread
	/// @param func entry point of the thread
	/// @param arg argument list
	ThreadHnd	threadCreate(ThreadFunc func, void *args);

	/// destroy a thread
	/// @param handle handle of the thread
	/// @param code return code
	void		threadDestroy(U32 code);

	/// get thread status
	/// @retval bool true is thread is still active
	/// @param hnd thread handle
	bool Platform::threadStatus(Platform::ThreadHnd hnd);

	/// get windows application instance
	/// @retval InstanceHnd wrapper for HINSTANCE
	InstanceHnd	InstanceGet(const char *name=0);

	/// create a window
	/// @retval bool true if window open successfully
	/// @param info module information
	bool		windowOpen(ModuleCfg &info);

	/// destroy a window
	/// @retval bool true if window close successfully
	/// @param info module information
	bool		windowClose(ModuleCfg &info);

	/// process window messages
	/// @retval info false if window messaging process stop
	bool		windowMsgProc();

protected:
	/// set device mode, select a hardware supported mode that closest to user preference
	/// @retval bool true if mode selected
	/// @param info reference of application info
	bool		setDeviceMode(ModuleCfg &info);

private:
	U64		fFreq__;
	BOOL	bPerfCounterAvail__;
	U64		nCountNew__;
	U64		nCountOld__;
	F64		nRemainder__;

	MSG		mMsg__;
};

} //namespace meta

#endif // _META_PLATFORM_WIN32_H_

#endif // WIN32