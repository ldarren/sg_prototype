#include "./vspore.h"

#include <liblogger/console.h>
#include <assert.h>

using namespace meta;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

VSpore::VSpore(ModuleCfg &cfg, Module *mod)
:	pDevice__(0), pVideo__(0), pBufPlayer_(0), pCurrScene_(0), pCurrGui_(0), 
	bQuit__(false), nTimeNow__(0), nTimeOld__(0), fElapsed__(0), 
	mCfg__(cfg), pModule__(mod)
{
	mWorldInfo_.hMutex = pModule__->mutexCreate(mWorldInfo_.hMutex);
	mWorldInfo_.pPlayer = 0;
}

VSpore::~VSpore()
{
	pModule__->mutexDestroy(mWorldInfo_.hMutex);
	mWorldInfo_.pPlayer = 0;
}

bool VSpore::init()
{
	if (!mNet_.init(this))
		return false;

	irr::SIrrlichtCreationParameters param;

	param.DriverType = video::EDT_OPENGL;
	if (!mCfg__.hParentWindow)
	{
		param.Bits = 16;
		param.Stencilbuffer = true;
		param.Vsync = false;
		param.Fullscreen = mCfg__.bFullscreen;
		param.WindowSize.Width = mCfg__.nWidth;
		param.WindowSize.Height = mCfg__.nHeight;
	}
	else
	{
		pModule__->windowOpen(mCfg__);
		param.WindowId = mCfg__.hWindow;
	}

	pDevice__ = irr::createDeviceEx(param);
	if (!pDevice__) return false;
	pDevice__->setEventReceiver(this);

	if (!mCfg__.hParentWindow)
	{
		pDevice__->setWindowCaption(L"Hello World! - Virtual Singapore");
	}

	// 3d and texture
	pVideo__ = pDevice__->getVideoDriver();

	mLoginScene_.create(pDevice__, &mNet_, &mWorldInfo_, 0);
	mLoginGui_.create(pDevice__, &mNet_, &mWorldInfo_, "../res/gui_login.xml");
	mGameScene_.create(pDevice__, &mNet_, &mWorldInfo_, "../res/scene_game.irr");
	mGameGui_.create(pDevice__, &mNet_, &mWorldInfo_, "../res/gui_game.xml");

	pCurrScene_ = &mLoginScene_;
	pCurrScene_->onWake();
	pCurrGui_ = &mLoginGui_;
	pCurrGui_->onWake();

	mNetPlayerInList_.clear();
	mNetPlayerOutList_.clear();

    nTimeOld__ = nTimeNow__ = pDevice__->getTimer()->getTime();

	logger::infof("VSpore::init\n");

	return true;
}

bool VSpore::deinit(bool clearWnd)
{
	bQuit__=true;
	mNet_.logout();

	if (pBufPlayer_)
	{
		pBufPlayer_->destroy();
		delete pBufPlayer_;
		pBufPlayer_=0;
	}

	PlayerListType::iterator iter;
	for (iter = mWorldInfo_.mNetPlayerList.begin(); iter != mWorldInfo_.mNetPlayerList.end(); ++iter)
	{
		(*iter)->destroy();
		delete *iter;
	}
	mWorldInfo_.mNetPlayerList.clear();
	for (iter = mNetPlayerInList_.begin(); iter != mNetPlayerInList_.end(); ++iter)
	{
		delete *iter;
	}
	mNetPlayerInList_.clear();

	if (pCurrGui_)
	{
		pCurrGui_->onSleep();
		pCurrGui_=0;
	}

	if (pCurrScene_)
	{
		pCurrScene_->onSleep();
		pCurrScene_=0;
	}

	mGameGui_.destroy();
	mGameScene_.destroy();

	if (pDevice__)
	{
		//pDevice__->closeDevice();
		pDevice__->drop();
		pDevice__=0;
	}

	if (clearWnd && mCfg__.hParentWindow)
		pModule__->windowClose(mCfg__);
	
	logger::infof("VSpore::deinit\n");

	return mNet_.deinit();
}

bool VSpore::update()
{
	assert(pDevice__);

	if (bQuit__) return false;

	// TODO: windows specific
	if (!pDevice__->isWindowFocused())
	{
		::SetFocus(mCfg__.hWindow);
	}

	pModule__->mutexLock(mWorldInfo_.hMutex);
	if (pBufPlayer_ != mWorldInfo_.pPlayer)
	{
		if (mWorldInfo_.pPlayer) 
		{
			mWorldInfo_.pPlayer->destroy();
			delete mWorldInfo_.pPlayer;
			mWorldInfo_.pPlayer=0;
		}

		if (pCurrScene_ == &mLoginScene_)
		{
			pCurrScene_->onSleep();
			pCurrGui_->onSleep();
			
			mWorldInfo_.pPlayer = pBufPlayer_;
			if (mWorldInfo_.pPlayer) 
				mWorldInfo_.pPlayer->create(pDevice__);

			pCurrScene_ = &mGameScene_;
			pCurrScene_->onWake();

			pCurrGui_ = &mGameGui_;
			pCurrGui_->onWake();
		}
	}

	if (pCurrScene_ == &mGameScene_)
	{
		if (!mNetPlayerInList_.empty())
		{
			for (PlayerListType::iterator iter = mNetPlayerInList_.begin(); iter != mNetPlayerInList_.end(); ++iter)
			{
				(*iter)->create(pDevice__);
				mWorldInfo_.mNetPlayerList.push_back(*iter);
			}
			mNetPlayerInList_.clear();
		}
		if (!mNetPlayerOutList_.empty())
		{
			for (PlayerListType::iterator iter = mNetPlayerOutList_.begin(); iter != mNetPlayerOutList_.end(); ++iter)
			{
				(*iter)->destroy();
				mWorldInfo_.mNetPlayerList.remove(*iter);
				delete *iter;
			}
			mNetPlayerOutList_.clear();
		}
	}
	pModule__->mutexUnlock(mWorldInfo_.hMutex);

	if (!pDevice__->run()) return false;

	if (pDevice__->isWindowMinimized())
	{
		pDevice__->yield();
	}
	else
	{
        // Work out a frame delta time. 
        nTimeNow__ = pDevice__->getTimer()->getTime(); 
        fElapsed__ = (nTimeNow__ - nTimeOld__) * 0.001f; // Time in seconds 
        nTimeOld__ = nTimeNow__; 

		pVideo__->beginScene(true, true, SColor(255, 100, 101, 140));

		bQuit__ |= !pCurrScene_->update(fElapsed__);
		bQuit__ |= !pCurrGui_->update(fElapsed__);

		pVideo__->endScene();
	}

	return true;
}

bool VSpore::OnEvent(const irr::SEvent &evt)
{
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
			case KEY_ESCAPE:
				bQuit__ = true;
				return true;
			}
		}
		break;
	case EET_JOYSTICK_INPUT_EVENT:
		break;
	case EET_LOG_TEXT_EVENT:
		break;
	case EET_USER_EVENT:
		switch(evt.UserEvent.UserData1)
		{
		case EACTID_Born:
			pBufPlayer_ = reinterpret_cast<LocalPlayer*>(evt.UserEvent.UserData2);
			return true;
		case EACTID_Dead:
		case EACTID_Move:
		case EACTID_Chat:
			break;
		}
		break;
	}

	if (pCurrGui_ && pCurrGui_->onEvent(evt))
		goto ln_consumed;
	if (pCurrScene_ && pCurrScene_->onEvent(evt))
		goto ln_consumed;

ln_consumed:
	return false;
}

void VSpore::onPlayerAdd(const std::string &name,const meta::Vector3F &pos,const meta::Vector3F &rot, const IPlayerPrx &proxy, const Ice::Current &c)
{
	std::cout<<"Created player: "<<name<<std::endl;
	pModule__->mutexLock(mWorldInfo_.hMutex);
	
	AvatarProp prop={
		"",
		"../res/models/ninja/ninja.b3d",
		{{206, 250, 15.0f}, {1, 14, 15.0f}},
		0,
		1,
		{pos.x, pos.y, pos.z},
		{rot.x, rot.y, rot.z}
	};
	strcpy_s(prop.sName, sizeof(prop.sName), name.c_str());

	NetPlayer *player = new NetPlayer(prop, &mNet_, proxy);
	mNetPlayerInList_.push_back(player);
	
	pModule__->mutexUnlock(mWorldInfo_.hMutex);
}

void VSpore::onPlayerRemove(const std::string &name, const Ice::Current &)
{
	std::cout<<"Deleted player: "<<name<<std::endl;
	pModule__->mutexLock(mWorldInfo_.hMutex);

	for (PlayerListType::iterator iter = mWorldInfo_.mNetPlayerList.begin(); iter != mWorldInfo_.mNetPlayerList.end(); ++iter)
	{
		if (name == (*iter)->getProperties().sName)
		mNetPlayerOutList_.push_back(*iter);
	}
	pModule__->mutexUnlock(mWorldInfo_.hMutex);
}
