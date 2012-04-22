#include "./local_player.h"

using namespace meta;
using namespace irr;
using namespace irr::core;
using namespace irr::scene;

LocalPlayer::LocalPlayer(const meta::AvatarProp &prop, NetClient *net, const IPlayerPrx &proxy)
: Parent(prop, net, proxy), pCam_(0), fAdvance_(0), fRotate_(0)
{
	mCamTargetOff_.setTranslation(vector3df(0, 5.0f, 0));
	mCamLocOff_.setTranslation(vector3df(0, 30.0f, -40.0f));
}

LocalPlayer::~LocalPlayer()
{
}

bool LocalPlayer::create(irr::IrrlichtDevice *device)
{
	if (!Parent::create(device)) return false;
	
	pCam_ = pScene_->addCameraSceneNode(0, getEyeLoc().getTranslation(), getEyeTarget().getTranslation());

	pNode_->setAnimationEndCallback(this);

	return true;
}

void LocalPlayer::update(F32 elapsed)
{
	Parent::update(elapsed);

	if (mTarget_.Z || mTarget_.Y)
	{
		pCam_->setPosition(getEyeLoc().getTranslation());
		pCam_->setTarget(getEyeTarget().getTranslation());
	}
}

bool LocalPlayer::onEvent(const irr::SEvent &evt)
{
	if (!pNode_ || !pScene_) return false;

	switch (evt.EventType)
	{
	case EET_GUI_EVENT:
		break;
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
		if (evt.KeyInput.PressedDown)
		{
			switch(evt.KeyInput.Key)
			{
			case KEY_KEY_A:
				fRotate_ += (fRotate_>-360.0f)?-30.0f:0;
				move(fAdvance_, fRotate_);
				return true;
			case KEY_KEY_D:
				fRotate_ += (fRotate_<360.0f)?30.0f:0;
				move(fAdvance_, fRotate_);
				return true;
			case KEY_KEY_S:
				fAdvance_ += (fAdvance_>-10.0f)?-5.0f:0;
				move(fAdvance_, fRotate_);
				return true;
			case KEY_KEY_W:
				fAdvance_ += (fAdvance_<10.0f)?5.0f:0;
				move(fAdvance_, fRotate_);
				return true;
			}
		}
		else
		{
			switch(evt.KeyInput.Key)
			{
			case KEY_KEY_A:
			case KEY_KEY_D:
				fRotate_ = 0;
				move(fAdvance_, fRotate_);
				break;
			case KEY_KEY_S:
			case KEY_KEY_W:
				fAdvance_ = 0;
				move(fAdvance_, fRotate_);
				break;
			default:
				return false;
			}

			if ((fRotate_ < 0.05f && fRotate_ > -0.05f) && (fAdvance_ < 0.05f && fAdvance_ > -0.05f))
			{
				stop();
				return true;
			}
		}
		break;
	case EET_JOYSTICK_INPUT_EVENT:
		break;
	case EET_LOG_TEXT_EVENT:
		break;
	case EET_USER_EVENT:
		break;
	}

	return false;
}

void LocalPlayer::OnAnimationEnd(IAnimatedMeshSceneNode* node)
{
}

void LocalPlayer::surf(const std::string &url)
{
	sURL_ = url;

	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EACTID_Surf;
	evt.UserEvent.UserData2 = reinterpret_cast<irr::s32>(this);
	pDevice_->postEventFromUser(evt);
}
	
irr::scene::ICameraSceneNode* LocalPlayer::getCam()
{
	return pCam_;
}

const std::string& LocalPlayer::getURL()
{
	return sURL_;
}

matrix4& LocalPlayer::getEyeLoc()
{
	static matrix4 loc;
	loc = pNode_->getRelativeTransformation()*mCamLocOff_;

	return loc;
}

matrix4& LocalPlayer::getEyeTarget()
{
	static matrix4 target;
	target = pNode_->getRelativeTransformation()*mCamTargetOff_;

	return target;
}
