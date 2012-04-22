#include "./net_player.h"

using namespace meta;

NetPlayer::NetPlayer(const meta::AvatarProp &prop, NetClient *net, const IPlayerPrx &proxy)
:	Parent(prop), pNet_(net), pProxy_(proxy)
{
}

bool NetPlayer::create(irr::IrrlichtDevice *device)
{
	Parent::create(device);

	mId_.name = mProp_.sName;
	mId_.category = "player";
	Ice::ObjectAdapterPtr adapter = pNet_->getCBAdapter();
	adapter->add(this, mId_);

	pProxy_->subscribe(mId_);

	return true;
}

void NetPlayer::update(F32 elapsed)
{
	Parent::update(elapsed);
}

bool NetPlayer::destroy()
{
	pProxy_->unsubscribe(mId_);
	Parent::destroy();

	return true;
}

void NetPlayer::onMove(const Vector3F &pos, const Vector3F &rot, const Ice::Current &c)
{
	mTarget_.X = 0;
	mTarget_.Y = rot.y;
	mTarget_.Z = pos.z;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EACTID_Move;
	evt.UserEvent.UserData2 = reinterpret_cast<irr::s32>(this);
	pDevice_->postEventFromUser(evt);
}

void NetPlayer::onStop(const Vector3F &pos, const Vector3F &rot, const Ice::Current &)
{
	mTarget_.X = 0;
	mTarget_.Y = 0;
	mTarget_.Z = 0;

	pNode_->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
	pNode_->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
}

void NetPlayer::onChat(const std::string &msg, const Ice::Current &)
{
	sMsg_ = msg;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EACTID_Chat;
	evt.UserEvent.UserData2 = reinterpret_cast<irr::s32>(this);
	pDevice_->postEventFromUser(evt);
}

void NetPlayer::move(float adv, float rot)
{
	Vector3F p, r;
	p.x = 0;
	p.y = 0;
	p.z = adv;
	r.x = 0;
	r.y = rot;
	r.z = 0;
	pProxy_->move(p, r);
}

void NetPlayer::stop()
{
	if (nCurrAnim_ == EMAID_Idle)
		return;

	Vector3F p, r;
	const irr::core::vector3df &pos = pNode_->getPosition();
	const irr::core::vector3df &rot = pNode_->getRotation();
	p.x = pos.X;
	p.y = pos.Y;
	p.z = pos.Z;
	r.x = rot.X;
	r.y = rot.Y;
	r.z = rot.Z;
	pProxy_->stop(p, r);
}

void NetPlayer::chat(const char *msg)
{
	pProxy_->chat(msg);
}

const std::string& NetPlayer::getChatMsg() const
{
	return sMsg_;
}
