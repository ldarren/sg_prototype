#pragma once
#ifndef _VSPORE_PLAYER_NET_CONTROLLED_H_
#define _VSPORE_PLAYER_NET_CONTROLLED_H_

#include "./avatar.h"
#include <network/netclient.h>
#include <string>

namespace meta {

class NetPlayer : public Avatar, public IPlayerCB
{
	typedef Avatar Parent;

public:
	NetPlayer(const meta::AvatarProp &prop, NetClient *net, const IPlayerPrx &proxy);

	virtual bool create(irr::IrrlichtDevice *device);
	virtual void update(F32 elapsed);
	virtual bool destroy();

	virtual void onMove(const Vector3F &pos, const Vector3F &rot, const Ice::Current &);
	virtual void onStop(const Vector3F &pos, const Vector3F &rot, const Ice::Current &);
	virtual void onChat(const std::string &msg, const Ice::Current &);

	void move(float adv, float rot);
	void stop();
	void chat(const char *msg);

	const std::string& getChatMsg() const;

protected:
	NetClient		*pNet_;
	IPlayerPrx		pProxy_;
	std::string		sMsg_;
	Ice::Identity	mId_;
};

} // namespace meta

#endif // _VSPORE_PLAYER_NET_CONTROLLED_H_