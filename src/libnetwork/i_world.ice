#ifndef _WORLD_INTERFACE_ICE_
#define _WORLD_INTERFACE_ICE_

#include <Ice/Identity.ice>

module meta
{

struct Vector3F
{
	float x;
	float y;
	float z;
};

interface IPlayerCB
{
	void onMove(Vector3F pos, Vector3F rot);
	void onStop(Vector3F pos, Vector3F rot);
	void onChat(string msg);
};

interface IPlayer
{
	void subscribe(Ice::Identity id);
	void unsubscribe(Ice::Identity id);
	void move(Vector3F pos, Vector3F rot);
	void chat(string msg);
	void stop(Vector3F pos, Vector3F rot);
};

interface IZoneCB
{
	void onPlayerAdd(string name, Vector3F pos, Vector3F rot, IPlayer* proxy);
	void onPlayerRemove(string name);
};

interface IZone
{
	IPlayer*	login(Ice::Identity id);
	bool		logout(Ice::Identity id);
};

interface IWorld
{
	IZone* authenticate();
};

};

#endif // _WORLD_INTERFACE_ICE_
