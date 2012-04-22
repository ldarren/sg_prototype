#pragma once
#ifndef _VSPORE_WORLD_H_
#define _VSPORE_WORLD_H_

#include "./world_info.h"
#include <gui/login.h>
#include <gui/game.h>
#include <scene/login.h>
#include <scene/game.h>
#include <player/local_player.h>
#include <player/net_player.h>
#include <network/netClient.h>
#include <core/plugin.h>
#include <meta/platform.h>

#include <irrlicht.h>
#include <list>

namespace meta {

class VSpore : public irr::IEventReceiver, public IZoneCB
{
	typedef WorldInfo::NetPlayerListType	PlayerListType;

public:
	VSpore(ModuleCfg &cfg, Module *mod);
	virtual ~VSpore();

public:
	bool init();
	bool deinit(bool clearWnd);
	bool update();
	
	virtual bool OnEvent(const irr::SEvent &evt);
	virtual void onPlayerAdd(const std::string &,const meta::Vector3F &,const meta::Vector3F &, const IPlayerPrx &, const Ice::Current &);
	virtual void onPlayerRemove(const std::string &name, const Ice::Current &);

protected:
	PlayerListType					mNetPlayerInList_;
	PlayerListType					mNetPlayerOutList_;
	LocalPlayer						*pBufPlayer_;
	ICanvas							*pCurrScene_;
	ICanvas							*pCurrGui_;

	WorldInfo						mWorldInfo_;
	GameScene						mGameScene_;
	LoginScene						mLoginScene_;
	GameGui							mGameGui_;
	LoginGui						mLoginGui_;
	NetClient						mNet_;

private:
	irr::IrrlichtDevice			*pDevice__;
	irr::video::IVideoDriver	*pVideo__;
	ModuleCfg					&mCfg__;
	Module						*pModule__;

	bool						bQuit__;
	U32							nTimeNow__;
	U32							nTimeOld__;
	F32							fElapsed__;
};

} // namespace meta

#endif // _VSPORE_WORLD_H_