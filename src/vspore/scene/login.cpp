#include "./login.h"

using namespace meta;
using namespace irr;
using namespace irr::core;

LoginScene::LoginScene()
:	pScene_(0)
{
}

LoginScene::~LoginScene()
{
}

void LoginScene::create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath)
{
	Parent::create(device, net, info, rcsPath);

	pScene_ = pDevice_->getSceneManager();
}

void LoginScene::destroy()
{
}

void LoginScene::onWake()
{
	Parent::onWake();

	if (pPath_)
		pScene_->loadScene(pPath_);
}

void LoginScene::onSleep()
{
	pScene_->clear();

	Parent::onSleep();
}

bool LoginScene::update(F32 elapsed)
{
	pScene_->drawAll();
	return true;
}

bool LoginScene::onEvent(const irr::SEvent &evt)
{
	switch (evt.EventType)
	{
	case EET_GUI_EVENT:
		break;
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
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
