#pragma once
#ifndef _VSPORE_INC_PLUGIN_INTERFACE_H_
#define _VSPORE_INC_PLUGIN_INTERFACE_H_

#include "./types.h"
#include <meta/platform.h>

namespace meta {

struct IHost
{
	virtual const Platform::EventHnd*	getPluginQuitEvents() const = 0;
	virtual const Platform::EventHnd*	getHostQuitEvents() const = 0;
	virtual U32	getQuitEventCount() const = 0;
};

struct IModule
{
	virtual bool		init(ModuleCfg *cfg, IHost *host) = 0;
	virtual bool		deinit() = 0;
	virtual bool		requestToQuit() = 0;
};

struct PluginInfo
{
};

struct IPlugin
{
	virtual IModule*			load() = 0;
	virtual	void				unload(IModule *mod) = 0;

	virtual const PluginInfo*	getInfo() const = 0;

protected:
	PluginInfo			mInfo_;
};

typedef IPlugin* (*GetModuleFunc)(void);

} // namespace meta

#endif // _VSPORE_INC_HOST_INTERFACE_H_