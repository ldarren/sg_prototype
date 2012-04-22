#pragma once
#ifndef _VSPORE_PLAYER_AVATAR_H_
#define _VSPORE_PLAYER_AVATAR_H_

#include <meta/types.h>

#include <irrlicht.h>

namespace meta {

enum EActionId
{
	EACTID_Born,
	EACTID_Dead,
	EACTID_Move,
	EACTID_Chat,
	EACTID_Surf,

	EACTID_Count
};

enum EModelAnimId
{
	EMAID_Idle,
	EMAID_Walk,

	EMAID_NA,
	EMAID_Count=EMAID_NA
};

enum EModelCallback
{
	EMCB_Loaded,
	EMCB_Unloaded,
	EMCB_Start,
	EMCB_Stop,

	EMCB_Count
};

struct AnimUnit
{
	S32 nStart;
	S32 nEnd;
	F32	fSpeed;
};

struct AvatarProp
{
	char								sName[64];
	char								sPath[128];
	AnimUnit							mAnimList[EMAID_Count];
	F32									fGroundOffset;
	F32									fScale;
	float								mInitPos[3];
	float								mInitRot[3];
};

class Avatar
{
public:
	Avatar(const AvatarProp &prop);
	virtual ~Avatar();

	virtual bool create(irr::IrrlichtDevice *device);
	virtual void update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);
	virtual bool destroy();

	void forceAnim(U16 anim);

	void setTarget(irr::core::vector3df &pos);

	const AvatarProp& getProperties() const;

protected:
	void changeAnim(U16 anim);
	irr::core::matrix4& getDirection(const irr::core::vector3df &dist);

protected:
	AvatarProp							mProp_;
	U16									nCurrAnim_;
	irr::core::vector3df				mTarget_; ///< x,z for position, y for rotation

	irr::IrrlichtDevice					*pDevice_; ///< for posting user events
	irr::scene::ISceneManager			*pScene_;
	irr::scene::IAnimatedMeshSceneNode	*pNode_;

private:
	irr::core::vector3df				mDelta__;
};

} // namespace meta

#endif // _VSPORE_PLAYER_AVATAR_H_