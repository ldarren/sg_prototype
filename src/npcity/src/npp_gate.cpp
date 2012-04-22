#include <pluginbase.h>

// here the plugin creates a plugin instance object which 
// will be associated with this newly created NPP instance and 
// will do all the necessary job
NPError NPP_New(NPMIMEType pluginType, NPP instance,
                          uint16 mode, int16 argc, char* argn[],
                          char* argv[], NPSavedData* saved)
{
	if(instance == NULL)
		return NPERR_INVALID_INSTANCE_ERROR;

	// create a new plugin instance object
	// initialization will be done when the associated window is ready
	nsPluginCreateData ds;

	ds.instance = instance;
	ds.type     = pluginType; 
	ds.mode     = mode; 
	ds.argc     = argc; 
	ds.argn     = argn; 
	ds.argv     = argv; 
	ds.saved    = saved;

	// associate the plugin instance object with NPP instance
	instance->pdata = reinterpret_cast<void*>(NS_NewPluginInstance(&ds));
	if(instance->pdata == NULL)
		return NPERR_OUT_OF_MEMORY_ERROR;

	return NPERR_NO_ERROR;
}

// here is the place to clean up and destroy the nsPluginInstance object
NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
	if(instance == NULL)
		return NPERR_INVALID_INSTANCE_ERROR;

	nsPluginInstanceBase *plugin = reinterpret_cast<nsPluginInstanceBase*>(instance->pdata);
	if(plugin) 
	{
		plugin->shut();
		NS_DestroyPluginInstance(plugin);
	}

	return NPERR_NO_ERROR;
}

// during this call we know when the plugin window is ready or
// is about to be destroyed so we can do some gui specific
// initialization and shutdown
NPError NPP_SetWindow(NPP instance, NPWindow* win)
{
	if (!instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	if (!win)
		return NPERR_GENERIC_ERROR;

	nsPluginInstanceBase *plugin = (nsPluginInstanceBase *)instance->pdata;
	if (!plugin) 
		return NPERR_GENERIC_ERROR;

	// window just created
	if (!plugin->isInitialized() && win->window)
	{ 
		if(!plugin->init(win)) 
		{
			NS_DestroyPluginInstance(plugin);
			return NPERR_MODULE_LOAD_FAILED_ERROR;
		}
	}

	// window goes away
	if (plugin->isInitialized() && !win->window)
		return plugin->SetWindow(win);

	// window resized?
	else if (plugin->isInitialized() && win->window)
		return plugin->SetWindow(win);

	// this should not happen, nothing to do
	else if (!plugin->isInitialized() && !win->window)
		return plugin->SetWindow(win);

	return NPERR_NO_ERROR;
}

NPError NPP_NewStream(NPP instance, NPMIMEType type,
                      NPStream* stream, NPBool seekable,
                      uint16* stype)
{
	if (!instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if (!plugin) 
		return NPERR_GENERIC_ERROR;

	return plugin->NewStream(type, stream, seekable, stype);
}

NPError NPP_DestroyStream(NPP instance, NPStream* stream,
                                    NPReason reason)
{
	if(!instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return NPERR_GENERIC_ERROR;

	return plugin->DestroyStream(stream, reason);
}

void    NPP_StreamAsFile(NPP instance, NPStream* stream,
                                   const char* fname)
{
	if(!instance)
		return;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return;

	return plugin->StreamAsFile(stream, fname);
}

int32   NPP_WriteReady(NPP instance, NPStream* stream)
{
	if(!instance)
		return 0x0fffffff;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return 0x0fffffff;

	return plugin->WriteReady(stream);
}

int32   NPP_Write(NPP instance, NPStream* stream, int32 offset,
                            int32 len, void* buffer)
{
	if(!instance)
		return len;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return len;

	return plugin->Write(stream, offset, len, buffer);
}

void    NPP_Print(NPP instance, NPPrint* platformPrint)
{
	if(!instance)
		return;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return;

	return plugin->Print(platformPrint);
}

int16   NPP_HandleEvent(NPP instance, void* evt)
{
	if(!instance)
		return 0;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return 0;

	return plugin->HandleEvent(evt);
}

void    NP_LOADDS NPP_URLNotify(NPP instance, const char* url,
                                NPReason reason, void* notifyData)
{
	if(!instance)
		return;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return;

	return plugin->URLNotify(url, reason, notifyData);
}

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
	if(!instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return NPERR_GENERIC_ERROR;

	return plugin->GetValue(variable, value);
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
	if(!instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	nsPluginInstanceBase * plugin = (nsPluginInstanceBase *)instance->pdata;
	if(!plugin) 
		return NPERR_GENERIC_ERROR;

	return plugin->SetValue(variable, value);
}

jref    NPP_GetJavaClass(void)
{
	return 0;
}
