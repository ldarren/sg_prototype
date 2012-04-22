#include <world/world.h>
#include <Ice/Ice.h>

using namespace meta;
using namespace std;

class GameServer : public Ice::Application
{
public:
	virtual int run(int, char**)
	{
		int status = EXIT_SUCCESS;

		IWorldPtr world = new World();

		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("VSpore.Server");
		adapter->add(world, communicator()->stringToIdentity("world"));
		adapter->activate();

		communicator()->waitForShutdown();

		return status;
	}

protected:

};

int main(int argc, char **argv)
{
	GameServer server;
	return server.main(argc, argv, "config.server");
}