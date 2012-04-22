/**
 * @brief Netscape helper functions
 */
#include <npplat.h>

extern NPNetscapeFuncs gNPNFuncs;

void    NPN_Version(int* plugin_major, int* plugin_minor,
                              int* netscape_major, int* netscape_minor)
{
	*plugin_major   = NP_VERSION_MAJOR;
	*plugin_minor   = NP_VERSION_MINOR;
	*netscape_major = HIBYTE(gNPNFuncs.version);
	*netscape_minor = LOBYTE(gNPNFuncs.version);
}

NPError NPN_GetURLNotify(NPP instance, const char* url,
                         const char* target, void* notifyData)
{
	int navMinorVers = gNPNFuncs.version & 0xFF;
	NPError rv = NPERR_NO_ERROR;

	if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
		rv = CallNPN_GetURLNotifyProc(gNPNFuncs.geturlnotify, instance, url, target, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

	return rv;
}

NPError NPN_GetURL(NPP instance, const char* url,
                             const char* target)
{
	return CallNPN_GetURLProc(gNPNFuncs.geturl, instance, url, target);
}

NPError NPN_PostURLNotify(NPP instance, const char* url,
                                    const char* target, uint32 len,
                                    const char* buf, NPBool file,
                                    void* notifyData)
{
	int navMinorVers = gNPNFuncs.version & 0xFF;
	NPError rv = NPERR_NO_ERROR;

	if( navMinorVers >= NPVERS_HAS_NOTIFICATION )
		rv = CallNPN_PostURLNotifyProc(gNPNFuncs.posturlnotify, instance, url, target, len, buf, file, notifyData);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

	return rv;
}

NPError NPN_PostURL(NPP instance, const char* url,
                              const char* target, uint32 len,
                              const char* buf, NPBool file)
{
	return CallNPN_PostURLProc(gNPNFuncs.posturl, instance, url, target, len, buf, file);
}

NPError NPN_RequestRead(NPStream* stream, NPByteRange* rangeList)
{
  return CallNPN_RequestReadProc(gNPNFuncs.requestread, stream, rangeList);
}

NPError NPN_NewStream(NPP instance, NPMIMEType type,
                                const char* target, NPStream** stream)
{
	int navMinorVersion = gNPNFuncs.version & 0xFF;
	NPError rv = NPERR_NO_ERROR;

	if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_NewStreamProc(gNPNFuncs.newstream, instance, type, target, stream);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

	return rv;
}

int32   NPN_Write(NPP instance, NPStream* stream, int32 len, void* buffer)
{
	int navMinorVersion = gNPNFuncs.version & 0xFF;
	int32 rv = 0;

	if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_WriteProc(gNPNFuncs.write, instance, stream, len, buffer);
	else
		rv = -1;

	return rv;
}

NPError NPN_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	int navMinorVersion = gNPNFuncs.version & 0xFF;
	NPError rv = NPERR_NO_ERROR;

	if( navMinorVersion >= NPVERS_HAS_STREAMOUTPUT )
		rv = CallNPN_DestroyStreamProc(gNPNFuncs.destroystream, instance, stream, reason);
	else
		rv = NPERR_INCOMPATIBLE_VERSION_ERROR;

	return rv;
}

void    NPN_Status(NPP instance, const char* message)
{
	CallNPN_StatusProc(gNPNFuncs.status, instance, message);
}

const char* NP_LOADDS	NPN_UserAgent(NPP instance)
{
	return CallNPN_UserAgentProc(gNPNFuncs.uagent, instance);
}

void*   NPN_MemAlloc(uint32 size)
{
  return CallNPN_MemAllocProc(gNPNFuncs.memalloc, size);
}

void    NPN_MemFree(void* ptr)
{
	CallNPN_MemFreeProc(gNPNFuncs.memfree, ptr);
}

uint32  NPN_MemFlush(uint32 size)
{
	return CallNPN_MemFlushProc(gNPNFuncs.memflush, size);
}

void    NPN_ReloadPlugins(NPBool reloadPages)
{
	CallNPN_ReloadPluginsProc(gNPNFuncs.reloadplugins, reloadPages);
}

#ifdef OJI
JRIEnv* NPN_GetJavaEnv(void)
{
	return CallNPN_GetJavaEnvProc(gNPNFuncs.getJavaEnv);
}

jref    NPN_GetJavaPeer(NPP instance)
{
	return CallNPN_GetJavaPeerProc(gNPNFuncs.getJavaPeer, instance);
}

#endif
NPError NPN_GetValue(NPP instance, NPNVariable variable, void *value)
{
	return CallNPN_GetValueProc(gNPNFuncs.getvalue, instance, variable, value);
}

NPError NPN_SetValue(NPP instance, NPPVariable variable, void *value)
{
	return CallNPN_SetValueProc(gNPNFuncs.setvalue, instance, variable, value);
}

void    NPN_InvalidateRect(NPP instance, NPRect *invalidRect)
{
	CallNPN_InvalidateRectProc(gNPNFuncs.invalidaterect, instance, invalidRect);
}

void    NPN_InvalidateRegion(NPP instance, NPRegion invalidRegion)
{
	CallNPN_InvalidateRegionProc(gNPNFuncs.invalidateregion, instance, invalidRegion);
}

void    NPN_ForceRedraw(NPP instance)
{
	CallNPN_ForceRedrawProc(gNPNFuncs.forceredraw, instance);
}

void    NPN_PushPopupsEnabledState(NPP instance, NPBool enabled)
{
}

void    NPN_PopPopupsEnabledState(NPP instance)
{
}

void    NPN_PluginThreadAsyncCall(NPP instance,
                                            void (*func) (void *),
                                            void *userData)
{
}

