#include "./avatar.h"

#include <assert.h>

using namespace meta;
using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;

Avatar::Avatar(const AvatarProp &prop)
: mProp_(prop), nCurrAnim_(EMAID_NA), pDevice_(0), pScene_(0), pNode_(0)
{
}

Avatar::~Avatar()
{
}

bool Avatar::create(irr::IrrlichtDevice *device)
{
	assert(device);

	pDevice_ = device;
	pScene_ = pDevice_->getSceneManager();
	IAnimatedMesh *mesh = pScene_->getMesh(mProp_.sPath);
	if (!mesh)
	{
		return false;
	}

	pNode_ = pScene_->addAnimatedMeshSceneNode(mesh);
	if (pNode_)
	{
		changeAnim(EMAID_Idle);
		pNode_->setMaterialFlag(EMF_LIGHTING, false);
		if (mProp_.fScale != 1)
			pNode_->setScale(vector3df(mProp_.fScale, mProp_.fScale, mProp_.fScale));
		//pNode_->setMaterialTexture(0, pVideo__->getTexture("../res/models/ninja/nskinwh.jpg"));
		pNode_->setPosition(vector3df(mProp_.mInitPos[0], mProp_.mInitPos[1], mProp_.mInitPos[2]));
		pNode_->setRotation(vector3df(mProp_.mInitRot[0], mProp_.mInitRot[1], mProp_.mInitRot[2]));
		//pNode_->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(scene::EDS_BBOX_BUFFERS));
	}

	return true;
}

bool Avatar::destroy()
{
	if (!pNode_) return false;
	pNode_->remove(); // pCurrScene clear to entire scene
	return true;
}

void Avatar::forceAnim(U16 anim)
{
	nCurrAnim_ = anim;
}

void Avatar::setTarget(irr::core::vector3df &pos)
{
}

void Avatar::update(F32 elapsed)
{
	if (mTarget_.equals(vector3df(0,0,0)))
	{
		changeAnim(EMAID_Idle);
	}
	else
	{
		changeAnim(EMAID_Walk);

		if (mTarget_.Y)
		{
			mDelta__.Y=mTarget_.Y>0 ? 1.0f : -1.0f;
		
			pNode_->setRotation(pNode_->getRotation()+mDelta__);
			mDelta__.Y = 0;
		}

		if (mTarget_.Z)
		{
			mDelta__.Z=mTarget_.Z>0 ? 0.3f : -0.2f;
		
			pNode_->setPosition(getDirection(mDelta__).getTranslation());
			mDelta__.Z = 0;
		}
	}
}

bool Avatar::onEvent(const irr::SEvent &evt)
{
	return false;
}

void Avatar::changeAnim(U16 anim)
{
	assert(pNode_);
	if (nCurrAnim_ == anim) return;

	nCurrAnim_ = anim;
	pNode_->setFrameLoop(mProp_.mAnimList[nCurrAnim_].nStart, mProp_.mAnimList[nCurrAnim_].nEnd);
	pNode_->setAnimationSpeed(mProp_.mAnimList[nCurrAnim_].fSpeed);
}

const AvatarProp& Avatar::getProperties() const
{
	return mProp_;
}

matrix4& Avatar::getDirection(const vector3df &dist)
{
	static matrix4 dir;
	static matrix4 delta;
	delta.setTranslation(dist);
	dir = pNode_->getRelativeTransformation()*delta;

	return dir;
}
