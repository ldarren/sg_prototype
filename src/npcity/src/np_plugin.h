#pragma once
#ifndef _NETSCAPE_CITY_PLUGIN_H_
#define _NETSCAPE_CITY_PLUGIN_H_

#include <libclient/application.h>
#include <pluginbase.h>

class CityInstance : public nsPluginInstanceBase
{
public:
	CityInstance(NPP inst);
	virtual ~CityInstance();

	virtual NPBool	init(NPWindow* win);
	virtual void	shut();
	virtual NPBool	isInitialized();
	virtual uint16  HandleEvent(void* evt);

	const char*			getVersion();
	meta::Application*	getApp();

private:
	NPP					mInstance__;
	meta::IModule		*pModule__;
};

#endif // _NETSCAPE_CITY_PLUGIN_H_