#include <npplat.h>
#include <pluginbase.h>

/// helper functions and datastructure

NPNetscapeFuncs gNPNFuncs;

/// Set up the plugin function table that Netscape will use to
/// call us. Netscape needs to know about our version and size   
/// and have a UniversalProcPointer for every function we implement.
NPError writePluginFunctionTable(NPPluginFuncs *nppFuncs)
{
	if (!nppFuncs)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	nppFuncs->version		= (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

#ifdef XP_MAC
#else
	nppFuncs->newp			= NPP_New;
	nppFuncs->destroy		= NPP_Destroy;
	nppFuncs->setwindow		= NPP_SetWindow;
	nppFuncs->newstream		= NPP_NewStream;
	nppFuncs->destroystream	= NPP_DestroyStream;
	nppFuncs->asfile		= NPP_StreamAsFile;
	nppFuncs->writeready	= NPP_WriteReady;
	nppFuncs->write			= NPP_Write;
	nppFuncs->print			= NPP_Print;
	nppFuncs->event			= NPP_HandleEvent;
	nppFuncs->urlnotify		= NPP_URLNotify;
	nppFuncs->getvalue		= NPP_GetValue;
	nppFuncs->setvalue		= NPP_SetValue;
#endif

#ifdef OJI
	nppFuncs->javaClass		= NPP_GetJavaClass;
#endif

	return NPERR_NO_ERROR;
}

NPError readNetscapeFunctionTable(NPNetscapeFuncs *npnFuncs)
{
	if (!npnFuncs || npnFuncs->size < sizeof(NPNetscapeFuncs))
		return NPERR_INVALID_FUNCTABLE_ERROR;
	if (HIBYTE(npnFuncs->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	memcpy(&gNPNFuncs, npnFuncs, sizeof(NPNetscapeFuncs));

	return NPERR_NO_ERROR;
}

/// DLL entry point

NPError OSCALL NP_Shutdown()
{
	NS_PluginShutdown();
	return NPERR_NO_ERROR;
}

#ifdef XP_WIN

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppFuncs)
{
	return writePluginFunctionTable(nppFuncs);
}

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnFuncs)
{
	NPError ret = readNetscapeFunctionTable(npnFuncs);
	if (ret != NPERR_NO_ERROR)
		return ret;

	return NS_PluginInitialize();
}

#elif XP_UNIX
#elif XP_MAC
#endif