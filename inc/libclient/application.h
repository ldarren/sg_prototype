#pragma once
#ifndef _CITY_APPLICATION_H_
#define _CITY_APPLICATION_H_

#include <meta/i_plugin.h>
#include <meta/platform.h>

namespace meta {

class Application : public IHost, public Platform
{
public:
	static const U32 PLUGIN_COUNT=1;
	static const U32 MODULE_COUNT=32;
public:
	Application();
	virtual ~Application();

	bool init();
	bool deinit();
	IModule* createModule();
	bool initModule(IModule *mod, void *wnd=0, U32 w=800, U32 h=600, bool fs=false);
	bool deinitModule(IModule *mod);
	bool destroyModule(IModule *mod);
	bool update();

	virtual const Platform::EventHnd*	getPluginQuitEvents() const;
	virtual const Platform::EventHnd*	getHostQuitEvents() const;
	virtual U32	getQuitEventCount() const;

private:
	Platform::EventHnd	hHostQuitEvent__[MODULE_COUNT];
	Platform::EventHnd	hPluginQuitEvents__[MODULE_COUNT];
	Platform::PluginHnd	hPlugin__;
	IPlugin				*pPlugin__;
	U32					nCurrId__;
};

} // namespace meta

#endif // _CITY_APPLICATION_H_