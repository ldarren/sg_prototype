#include "./thread.h"
#include <world/vspore.h>
#include "./plugin.h"

using namespace meta;

U32 __stdcall Thread::main(void *args)
{
	Module *mod = reinterpret_cast<Module*>(args);
	ModuleCfg &cfg = mod->getConfig();
	IHost *host = mod->getHost();

	int ret = cfg.nId;
	bool selfInitiated=false;
	VSpore *spore = new VSpore(cfg, mod);

	if (!spore->init())
	{
		MessageBox(0, L"Possible reasons:\r\n 1) server not running\r\n 2) network problem\r\n 3) irrlicht dll not found", L"Initialization error", MB_OK | MB_ICONINFORMATION);
		ret = -1;
		goto ln_clear_and_threadend;
	}

	while (mod->eventSelect(1, &cfg.hHostQuitEvt, 17, true) == Platform::EMI_TimeOut)
	{
		if (!spore->update())
		{
			selfInitiated=true;
			break;
		}
	}

ln_clear_and_threadend:

	spore->deinit(selfInitiated);
	// courtesy of ICE//delete spore;

	mod->eventSet(cfg.hPluginQuitEvt);

	//mod->threadDestroy(cfg.nId);
	return ret;
}
