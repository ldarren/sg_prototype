#include "./plugin.h"
#include <windows.h>

meta::Plugin *gpPlugin=0;

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_DETACH:
		delete gpPlugin;
		gpPlugin = 0;
		break;
	case DLL_PROCESS_ATTACH:
		gpPlugin = new meta::Plugin(inst);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

extern "C"
{
	meta::IPlugin* GetInterface()
	{
		return gpPlugin;
	}
}
