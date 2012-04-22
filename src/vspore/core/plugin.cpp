#include "./plugin.h"
#include "./thread.h"

#include <liblogger/console.h>

#include <stdio.h> // for sprintf

using namespace meta;

///////////////////////// Module /////////////////////////

Module::Module()
:	pHost__(0), hThread__(0)
{
	memset(&mCfg__, 0, sizeof(mCfg__));
	logger::_instance.add_sink(new logger::fsink("dfsink", "./vspore.log", logger::ELOGL_Info));
}

Module::~Module()
{
}

bool Module::init(ModuleCfg *cfg, IHost *host)
{
	Platform::init();

	pHost__ = host;

	memcpy(&mCfg__, cfg, sizeof(ModuleCfg));
	// TODO: check if host set these values already
	//mCfg__.hInstance = inst;
	mCfg__.nColorDepth = 32;
	mCfg__.nZbufferDepth = 16;
	mCfg__.nFreq = 60;
	sprintf_s(mCfg__.sName, sizeof(mCfg__.sName), "vspore%d", mCfg__.nId);
	mCfg__.sTitle = ".: Twinity - Singapore :.";

	hThread__ = threadCreate(Thread::main, this);

	return true;
}

bool Module::deinit()
{
	//Platform::EventHnd evt[1];
	//evt[0] = pHost__->getPluginQuitEvent(mCfg__.nId);
	//eventSelect(1, evt, 20000, false);
	windowClose(mCfg__);
	pHost__ = 0;
	
	Platform::deinit();

	return true;
}

bool Module::requestToQuit()
{
	eventSet(mCfg__.hHostQuitEvt);
	MessageBoxA(mCfg__.hWindow, "Close without saving?", "Metaversum Singapore", MB_OK | MB_ICONINFORMATION);
	return true;
}

meta::ModuleCfg& Module::getConfig()
{
	return mCfg__;
}

IHost* Module::getHost()
{
	return pHost__;
}

Platform::ThreadHnd Module::getThreadHnd()
{
	return hThread__;
}

///////////////////////// Plugin /////////////////////////

Plugin::Plugin(HINSTANCE inst)
:	hInstance_(inst)
{
}

Plugin::~Plugin()
{
	hInstance_ = 0;
}

IModule* Plugin::load()
{
	return new Module();
}

void Plugin::unload(IModule *mod)
{
	delete mod;
}

const PluginInfo* Plugin::getInfo() const
{
	return &mInfo_;
}
