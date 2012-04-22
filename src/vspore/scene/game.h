#pragma once
#ifndef _VSPORE_SCENE_GAME_H_
#define _VSPORE_SCENE_GAME_H_

#include "./web_texture.h"

#include <core/i_canvas.h>
#include <libgeodb/geodb.h>
#include <libgeodb/octree.h>

#include <string>
#include <list>

#define PT_COUNT 1000

namespace meta {

class GameScene : public ICanvas
{
	typedef ICanvas Parent;
	typedef std::list<irr::scene::IBillboardSceneNode*> SelBillList;

	enum
	{
		EPICKID_NotPickable	= 0,
		EPICKID_Pickable	= 1 << 0,
	};

public:
	GameScene();
	virtual ~GameScene();

	virtual void create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath);
	virtual void destroy();

	virtual void onWake();
	virtual void onSleep();
	virtual bool update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);

#ifdef WIN32
	void handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam);
	HHOOK							hKeyboardHook;
#endif

protected:
	// helper functions
	bool testWebHit(irr::core::vector2di &);
	void selectNextHouse();
	void searchNextPoint();
	void GeoBBox2aabbox(const GeoBBox &gbox, irr::core::aabbox3df &irrbox);

private:
	irr::scene::ICameraSceneNode	*pFPSCam_;				///< fps cam for scene inspection
	irr::scene::ISceneManager		*pScene_;

	irr::scene::IMeshSceneNode		*pWebBoxNode_;
	WebTexture						mWebTex_;
	Awesomium::WebCore				*pWebBrowser_;
	bool							bIsWebSelected_;

	OctreeNode						*pRootNode_;
	OctreeNode						*pLocalNode_;

	irr::scene::IBillboardSceneNode *pPointBill_[PT_COUNT];
	irr::scene::ISceneNode			*pHouses_[4];
	U32								nSelectedPt_;
	U32								nSelectedHouse_;
	irr::core::aabbox3df			mPtSelBox_;
	irr::core::aabbox3df			mQueryBox_;
	irr::core::aabbox3df			mResultBox_;
	GeoBBox							mQueryBBox_;
	SelBillList						mSelPtList_;

	irr::video::ITexture			*pPointTex_;
	irr::video::ITexture			*pSelPointTex_;
};

} // namespace meta

#endif // _VSPORE_SCENE_GAME_H_