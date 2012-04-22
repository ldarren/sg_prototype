#include <libclient/application.h>

using namespace meta;

int main (int argc, char **argv)
{
	Application app;

	app.init();

	IModule *mod = app.createModule();
	app.initModule(mod);

	const Platform::EventHnd *evts = app.getPluginQuitEvents();
	while (app.eventSelect(Application::MODULE_COUNT, evts, 16, false) == Platform::EMI_TimeOut)
	{
		app.update();
	}

	app.deinitModule(mod);
	app.destroyModule(mod);

	app.deinit();

	return 0;
}