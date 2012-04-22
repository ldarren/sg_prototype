#pragma once
#ifndef _SERVER_WORLD_H_
#define _SERVER_WORLD_H_

#include <libnetwork/i_world.h>
#include <IceUtil/IceUtil.h>

#include <string>
#include <map>
#include <list>
#include <set>

namespace meta {

class Player : public IPlayer, public IceUtil::Monitor<IceUtil::Mutex>
{
	typedef std::set<IPlayerCBPrx> SubscriberListType;

public:
	Player(const std::string&);

	virtual void subscribe(const Ice::Identity&, const Ice::Current&);
    virtual void unsubscribe(const Ice::Identity&, const Ice::Current&);
	virtual void move(const Vector3F& pos, const Vector3F& rot, const Ice::Current&);
	virtual void stop(const Vector3F& pos, const Vector3F& rot, const Ice::Current&);
    virtual void chat(const std::string& msg, const Ice::Current&);

	void setProxies(const IZoneCBPrx &, const IPlayerPrx &);
	const IZoneCBPrx getZoneCBProxy() const;
	const IPlayerPrx getPlayerProxy() const;

	const std::string&	getName() const;
	const Vector3F&		getDeltaPos() const;
	const Vector3F&		getDeltaRot() const;
	const Vector3F&		getPos() const;
	const Vector3F&		getRot() const;

protected:
	void broadcastMovement();
	void broadcastLocation();
	void broadcastChat(const std::string &msg);

	SubscriberListType			mSubscribers_;
	IPlayerPrx			pPlayerProxy_;		///< for new player
	IZoneCBPrx			pZoneCBProxy_;		///< to call client's zone

	std::string					sName_;
	Vector3F					mDPos_;
	Vector3F					mDRot_;
	Vector3F					mPos_;
	Vector3F					mRot_;
};

class Zone : public IZone, public IceUtil::Monitor<IceUtil::Mutex>
{
	typedef std::map<std::string, Player*> ClientListType;

public:
	virtual ~Zone();

	virtual IPlayerPrx	login(const Ice::Identity&, const ::Ice::Current&);
	virtual bool		logout(const Ice::Identity&, const Ice::Current& ctx);

protected:
	void broadcastAddPlayer(IPlayerPrx proxy, Player *);
	void broadcastRemovePlayer(const std::string &name);
	bool found(const std::string &name);

	ClientListType mClientList_;
};

class World : public IWorld, public IceUtil::Monitor<IceUtil::Mutex>
{
	typedef std::list<IZonePrx> ZoneListType;

public:
	World();
	virtual ~World();

	virtual IZonePrx	authenticate(const Ice::Current&);
	virtual void		destroy(const Ice::Current&);

protected:
	ZoneListType mZoneList_;
};

} // namespace meta

#endif // _SERVER_WORLD_H_