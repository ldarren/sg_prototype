#include "./np_plugin.h"

#include <windows.h>
#include <windowsx.h>

using namespace meta;

namespace /* unnamed */ {
	meta::Application	gApp;
}

/// implement pluginbase.h functions

// global plugin initialization and shutdown
NPError NS_PluginInitialize()
{
	// WIN32 specific
	SetCurrentDirectoryA(getenv("VSPORE_DIR"));

	if (!gApp.init())
		return NPERR_MODULE_LOAD_FAILED_ERROR;
	return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
	gApp.deinit();
}

// creation and destruction of the object of the derived class
nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
	if (!aCreateDataStruct)
		return 0;

	return new CityInstance(aCreateDataStruct->instance);
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
	const Platform::EventHnd *evts = gApp.getPluginQuitEvents();
	gApp.eventSelect(meta::Application::MODULE_COUNT, evts, 20000, false);
	delete aPlugin;
}

/// implementation of twinity web plugin

CityInstance::CityInstance(NPP inst)
:	mInstance__(inst), pModule__(0)
{
}

CityInstance::~CityInstance()
{
	gApp.deinitModule(pModule__);
	gApp.destroyModule(pModule__);
	mInstance__ = 0;
	pModule__ = 0;
}

NPBool CityInstance::init(NPWindow* win)
{
	if (!win || !win->window)
		return FALSE;

	// WIN32 specific
	SetCurrentDirectoryA(getenv("VSPORE_DIR"));

	if (pModule__ = gApp.createModule())
	{
		gApp.initModule(pModule__, win->window, win->width, win->height);
		return TRUE;
	}

	return FALSE;
}

void CityInstance::shut()
{
	pModule__->requestToQuit();
}

NPBool CityInstance::isInitialized()
{
	return pModule__ != 0;
}

// not implement on WIN32 system
uint16 CityInstance::HandleEvent(void* evt)
{
	NPEvent *e = reinterpret_cast<_NPEvent*>(evt);
	switch (e->event)
	{
	case WM_DESTROY:
		break;
	}

	return 0;
}

const char *CityInstance::getVersion()
{
	return NPN_UserAgent(mInstance__);
}
