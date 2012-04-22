#ifdef LINUX

#include <meta/platform/win32.h>
#include <meta/platform.h>
#include <meta/i_plugin.h>

#include <math.h>

using namespace meta;

Platform::Platform()
:	fFreq__(0), bPerfCounterAvail__(false), nCountNew__(0), nCountOld__(0),	nRemainder__(0)
{
}

Platform::~Platform()
{
}

/*	Quoted "http://msdn.microsoft.com/en-us/library/ms644904(VS.85).aspx"
	On a multiprocessor computer, it should not matter which processor is called. 
	However, you can get different results on different processors due to bugs in 
	the basic input/output system (BIOS) or the hardware abstraction layer (HAL). 
	To specify processor affinity for a thread, use the SetThreadAffinityMask function. */
void Platform::init()
{
	// Timer
	SetProcessAffinityMask( GetCurrentProcess(), 1 ); // first processor

	nRemainder__ = 0;
	bPerfCounterAvail__ = QueryPerformanceFrequency((LARGE_INTEGER *) &fFreq__);
	if (bPerfCounterAvail__)
		bPerfCounterAvail__ = QueryPerformanceCounter((LARGE_INTEGER *) &nCountOld__);
	if (!bPerfCounterAvail__)
		nCountOld__ = GetTickCount();

	// event

	// thread
}

void Platform::deinit()
{
}

U32 Platform::timeElapse()
{
	U32 elapsed;

	if(bPerfCounterAvail__)
	{
		QueryPerformanceCounter( (LARGE_INTEGER *) &nCountNew__);
		F64 elapsed64 = (1000.0 * F64(nCountNew__ - nCountOld__) / F64(fFreq__));
		elapsed64 += nRemainder__;
		elapsed = (U32)floor(elapsed64);
		nRemainder__ = elapsed64 - F64(elapsed);
		return elapsed;
	}
	else
	{
		nCountNew__ = GetTickCount();
		elapsed = (U32)(nCountNew__ - nCountOld__);
	}

	nCountOld__ = nCountNew__;
	return elapsed;
}

Platform::EventHnd Platform::eventCreate(bool manual, bool set)
{
	return CreateEventA(0, manual, set, 0); 
}

bool Platform::eventDestroy(const EventHnd &evt)
{
	return CloseHandle(evt) == TRUE;
}

bool Platform::eventSet(const EventHnd &evt)
{
	return SetEvent(evt) == TRUE;
}

bool Platform::eventReset(const EventHnd &evt)
{
	return ResetEvent(evt) == TRUE;
}

U32 Platform::eventSelect(U32 count, const EventHnd *list, U32 period, bool wait)
{
	return WaitForMultipleObjects(count, list, wait, period);
}

IPlugin* Platform::pluginLoad(char *pluginName, PluginHnd &handle)
{
	handle = LoadLibraryA(pluginName);
	if (!handle) return 0;
	
	GetModuleFunc	loader = (GetModuleFunc)GetProcAddress(handle, "GetInterface");

	if(!loader)	return 0;
	IPlugin *plugin = loader();
	if (!plugin) return 0;

	return plugin;
}

bool Platform::pluginUnload(const PluginHnd &handle)
{
	return FreeLibrary(handle) == TRUE;
}

#endif LINUX