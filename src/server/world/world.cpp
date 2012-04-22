#include "./world.h"
#include <Ice/Ice.h>
#include <string.h>

using namespace meta;
using namespace std;

//////////////// Player ////////////////

Player::Player(const string &name)
: sName_(name), pPlayerProxy_(0), pZoneCBProxy_(0)
{
	memset(&mDPos_, 0, sizeof(mDPos_));
	memset(&mDRot_, 0, sizeof(mDRot_));
	memset(&mPos_, 0, sizeof(mPos_));
	memset(&mRot_, 0, sizeof(mRot_));
}

void Player::subscribe(const Ice::Identity& ident, const Ice::Current &c)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    IPlayerCBPrx cb = IPlayerCBPrx::uncheckedCast(c.con->createProxy(ident));
	if (cb)
		mSubscribers_.insert(cb);
}

void Player::unsubscribe(const Ice::Identity& ident, const Ice::Current &c)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    IPlayerCBPrx cb = IPlayerCBPrx::uncheckedCast(c.con->createProxy(ident));
	if (cb)
	{
		//cb->ice_getConnection()->close(false);
		mSubscribers_.erase(cb);
	}
}

void Player::move(const Vector3F& pos, const Vector3F& rot, const Ice::Current& c)
{
	mDPos_ = pos;
	mDRot_ = rot;
	broadcastMovement();
}

void Player::stop(const Vector3F& pos, const Vector3F& rot, const Ice::Current&)
{
	mPos_ = pos;
	mRot_ = rot;
	broadcastLocation();
}

void Player::chat(const std::string& msg, const Ice::Current& c)
{
	broadcastChat(msg);
}

void Player::setProxies(const IZoneCBPrx &zn, const IPlayerPrx &pl)
{
	pPlayerProxy_ = pl;
	pZoneCBProxy_ = zn;
}

const IZoneCBPrx Player::getZoneCBProxy() const
{
	return pZoneCBProxy_;
}

const IPlayerPrx Player::getPlayerProxy() const
{
	return pPlayerProxy_;
}

const std::string& Player::getName() const
{
	return sName_;
}

const Vector3F& Player::getDeltaPos() const
{
	return mPos_;
}

const Vector3F& Player::getDeltaRot() const
{
	return mRot_;
}

const Vector3F& Player::getPos() const
{
	return mPos_;
}

const Vector3F& Player::getRot() const
{
	return mRot_;
}

void Player::broadcastMovement()
{
	SubscriberListType::iterator iter;

	for (iter = mSubscribers_.begin(); iter != mSubscribers_.end();)
	{
		try
		{
			iter->get()->onMove(mDPos_, mDRot_);
			 ++iter;
		}
		catch (const IceUtil::Exception &)
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
			iter = mSubscribers_.erase(iter);
		}
	}
}

void Player::broadcastLocation()
{
	SubscriberListType::iterator iter;

	for (iter = mSubscribers_.begin(); iter != mSubscribers_.end();)
	{
		try
		{
			iter->get()->onStop(mPos_, mRot_);
			 ++iter;
		}
		catch (const IceUtil::Exception &)
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
			iter = mSubscribers_.erase(iter);
		}
	}
}

void Player::broadcastChat(const std::string &msg)
{
	SubscriberListType::iterator iter;
	for (iter = mSubscribers_.begin(); iter != mSubscribers_.end();)
	{
		try
		{
			iter->get()->onChat(msg);
			 ++iter;
		}
		catch (const IceUtil::Exception &)
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
			iter = mSubscribers_.erase(iter);
		}
	}
}

//////////////// Zone ////////////////
Zone::~Zone()
{
	ClientListType::iterator iter, temp;
	iter = mClientList_.begin();
	while (iter != mClientList_.end())
	{
		broadcastRemovePlayer(iter->first);
		++iter;
	}
	mClientList_.clear();
}

IPlayerPrx Zone::login(const Ice::Identity &id, const ::Ice::Current &c)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
	if (found(id.name))
		return 0;

	Player *player = new Player(id.name);
	IPlayerPrx proxy = IPlayerPrx::uncheckedCast(c.adapter->addWithUUID(player));
    IZoneCBPrx cb = IZoneCBPrx::uncheckedCast(c.con->createProxy(id));
	player->setProxies(cb, proxy);

	broadcastAddPlayer(proxy, player);

	mClientList_[id.name] = player;
	cout<<"login: "<<id.name<<endl;
	return proxy;
}

bool Zone::logout(const Ice::Identity &id, const Ice::Current& c)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
	if (found(id.name))
	{
		//mClientList_[id.name]->getZoneCBProxy()->ice_getConnection()->close(false);
		mClientList_.erase(id.name);

		broadcastRemovePlayer(id.name);

		return true;
	}
	return false;
}

void Zone::broadcastAddPlayer(IPlayerPrx proxy, Player *player)
{
	ClientListType::iterator iter;
	const std::string &name = player->getName();
	const Vector3F &pos = player->getPos();
	const Vector3F &rot = player->getRot();

	for (iter = mClientList_.begin(); iter != mClientList_.end();)
	{
		Player *ex = iter->second;
		try
		{
			player->getZoneCBProxy()->onPlayerAdd(ex->getName(), ex->getPos(), ex->getRot(), ex->getPlayerProxy());
			ex->getZoneCBProxy()->onPlayerAdd(name, pos, rot, proxy);
			 ++iter;
		}
		catch (const IceUtil::Exception &)
		{
			//IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
			iter = mClientList_.erase(iter);
			//delete ex;
		}
	}
}

void Zone::broadcastRemovePlayer(const std::string &name)
{
	ClientListType::iterator iter;

	for (iter = mClientList_.begin(); iter != mClientList_.end(); ++iter)
	{
		Player *ex = iter->second;
		try
		{
			ex->getZoneCBProxy()->onPlayerRemove(name);
			 ++iter;
		}
		catch (const IceUtil::Exception &)
		{
			//IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
			iter = mClientList_.erase(iter);
			//delete ex;
		}
	}
}

bool Zone::found(const string &name)
{
	ClientListType::iterator iter = mClientList_.find(name);
	if (iter == mClientList_.end())
		return false;
	return true;
}

//////////////// World ////////////////
World::World()
{
}

World::~World()
{
}

IZonePrx World::authenticate(const Ice::Current &c)
{
	if (mZoneList_.empty())
	{
		IZonePtr zn = new Zone();
		IZonePrx proxy = IZonePrx::uncheckedCast(c.adapter->addWithUUID(zn));
		mZoneList_.push_back(proxy);
	}
	return mZoneList_.front();
}

void World::destroy(const Ice::Current &c)
{
	ZoneListType::iterator iter;
	iter = mZoneList_.begin();
	while (iter != mZoneList_.end())
	{
		c.adapter->remove((*iter)->ice_getIdentity());
		++iter;
	}
	mZoneList_.clear();
}
