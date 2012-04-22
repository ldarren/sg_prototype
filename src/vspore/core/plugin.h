#pragma once
#ifndef _VSPORE_LOGIC_PLUGIN_H_
#define _VSPORE_LOGIC_PLUGIN_H_

#include "./thread.h"

#include <meta/i_plugin.h>
#include <meta/platform.h>

namespace meta {

class Module : public IModule, public Platform
{
public:
	Module();
	virtual ~Module();

	virtual bool			init(ModuleCfg *cfg, IHost *host);
	virtual bool			deinit();
	virtual bool			requestToQuit();

	ModuleCfg&				getConfig();
	IHost*					getHost();
	ThreadHnd				getThreadHnd();

private:
	ModuleCfg		mCfg__;
	ThreadHnd		hThread__;
	IHost			*pHost__;
};

class Plugin : public IPlugin
{
public:
	Plugin(Platform::InstanceHnd inst);
	virtual ~Plugin();

	virtual IModule*			load();
	virtual	void				unload(IModule *mod);

	virtual const PluginInfo*	getInfo() const;

protected:
	Platform::InstanceHnd		hInstance_;
};

} // namespace meta

#endif // _VSPORE_LOGIC_PLUGIN_H_