#pragma once
#ifndef _VSPORE_PLAYER_LOCAL_CONTROLLED_H_
#define _VSPORE_PLAYER_LOCAL_CONTROLLED_H_

#include "./net_player.h"

namespace meta {

class LocalPlayer : public NetPlayer, public irr::scene::IAnimationEndCallBack
{
	typedef NetPlayer Parent;

public:
	LocalPlayer(const meta::AvatarProp &prop, NetClient *net, const IPlayerPrx &proxy);
	virtual ~LocalPlayer();

	virtual bool create(irr::IrrlichtDevice *device);
	virtual void update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);
	virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node);

	void surf(const std::string &url);

	irr::scene::ICameraSceneNode* getCam();
	const std::string& getURL();

protected:
	irr::core::matrix4& getEyeLoc();
	irr::core::matrix4& getEyeTarget();

private:
	irr::scene::ICameraSceneNode	*pCam_;
	
	irr::core::matrix4			mCamTargetOff_;
	irr::core::matrix4			mCamLocOff_;

	float						fAdvance_;
	float						fRotate_;

	std::string					sURL_;
};

} // namespace meta

#endif // _VSPORE_PLAYER_LOCAL_CONTROLLED_H_