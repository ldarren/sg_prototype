#include <libclient/application.h>

#include <assert.h>

using namespace meta;

Application::Application()
:	hPlugin__(0), pPlugin__(0), nCurrId__(0)
{
	memset(hHostQuitEvent__, 0, sizeof(hHostQuitEvent__));
	memset(hPluginQuitEvents__, 0, sizeof(hPluginQuitEvents__));
}

Application::~Application()
{
}

bool Application::init()
{
	Platform::init();

	for (int i=0; i<MODULE_COUNT; ++i)
	{
		hHostQuitEvent__[i] = eventCreate(true, false);
		hPluginQuitEvents__[i] = eventCreate(true, false);
	}

	pPlugin__ = pluginLoad("vspore.dll", hPlugin__);

	nCurrId__ = 0;

	return pPlugin__ != 0;
}

bool Application::deinit()
{
	nCurrId__ = 0;

	pluginUnload(hPlugin__);

	for (int i=0; i<MODULE_COUNT; ++i)
	{
		eventDestroy(hHostQuitEvent__[i]);
		hHostQuitEvent__[i]=0;
		eventDestroy(hPluginQuitEvents__[i]);
		hPluginQuitEvents__[i]=0;
	}

	Platform::deinit();

	return true;
}

IModule* Application::createModule()
{
	return pPlugin__->load();
}

bool Application::initModule(IModule *mod, void *wnd, U32 w, U32 h, bool fs)
{
	ModuleCfg cfg;
	memset(&cfg, 0, sizeof(ModuleCfg));

	cfg.hParentWindow = (Platform::WindowHnd)wnd;
	cfg.bFullscreen = fs;
	cfg.nWidth=w;
	cfg.nHeight=h;
	cfg.nId=nCurrId__++;
	cfg.hHostQuitEvt = hHostQuitEvent__[cfg.nId];
	cfg.hPluginQuitEvt = hPluginQuitEvents__[cfg.nId];
	// TODO: better id management
	if (nCurrId__ > MODULE_COUNT) 
		nCurrId__=0;

	return mod->init(&cfg, this);
}

bool Application::deinitModule(IModule *mod)
{
	return mod->deinit();
}

bool Application::destroyModule(IModule *mod)
{
	pPlugin__->unload(mod);

	return true;
}

bool Application::update()
{
	return true;
}

const Platform::EventHnd*	Application::getPluginQuitEvents() const
{
	return hPluginQuitEvents__;
}

const Platform::EventHnd*	Application::getHostQuitEvents() const
{
	return hHostQuitEvent__;
}

U32	Application::getQuitEventCount() const
{
	return MODULE_COUNT;
}
