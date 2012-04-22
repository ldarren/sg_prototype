#include "./login.h"

#include <player/local_player.h>

using namespace meta;
using namespace irr;
using namespace irr::gui;

LoginGui::LoginGui()
:	pGui_(0), bConsumeKBEvent__(false), pLoginEdit__(0), pErrorText__(0)
{
}

LoginGui::~LoginGui()
{
}

void LoginGui::create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath)
{
	Parent::create(device, net, info, rcsPath);

	pGui_ = device->getGUIEnvironment();
}

void LoginGui::destroy()
{
}

void LoginGui::onWake()
{
	Parent::onWake();

	// GUI
	pGui_->loadGUI(pPath_);

	IGUIElement *root = pGui_->getRootGUIElement();
	//IGUIElement *win = root->getElementFromId(0, true);
	//win->setRelativePosition(position2di((cfg->nWidth-310)/2, cfg->nHeight-160));

	pLoginEdit__ = reinterpret_cast<IGUIEditBox*>(root->getElementFromId(EGUIID_LoginEdit, true));
	//root->bringToFront(pLoginEdit__);
	pErrorText__ = reinterpret_cast<IGUIStaticText*>(root->getElementFromId(EGUIID_ErrorMsg, true));
}

void LoginGui::onSleep()
{
	pGui_->clear();

	Parent::onSleep();
}

bool LoginGui::update(F32 elapsed)
{
	pGui_->drawAll();
	return true;
}

bool LoginGui::onEvent(const irr::SEvent &evt)
{
	switch (evt.EventType)
	{
	case EET_GUI_EVENT:
		{
			s32 id = evt.GUIEvent.Caller->getID();
			switch(evt.GUIEvent.EventType)
			{
			case EGET_ELEMENT_FOCUSED:
				if (id == EGUIID_LoginEdit)
				{
					bConsumeKBEvent__=true;
				}
				break;
			case EGET_ELEMENT_FOCUS_LOST:
				if (id == EGUIID_LoginEdit)
				{
					bConsumeKBEvent__=false;
				}
				break;
			case EGET_ELEMENT_HOVERED:
			case EGET_ELEMENT_LEFT:
				break;
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case EGUIID_LoginBtn:
					serverLogin(pLoginEdit__->getText());
					break;
				}
				break;
			}
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		break;
	case EET_KEY_INPUT_EVENT:
		if (bConsumeKBEvent__) 
		{
			if (evt.KeyInput.PressedDown)
			{
			}
			else
			{
				switch(evt.KeyInput.Key)
				{
				case KEY_RETURN:
					serverLogin(pLoginEdit__->getText());
					break;
				}
			}
			return true;
		}
		break;
	case EET_JOYSTICK_INPUT_EVENT:
	case EET_LOG_TEXT_EVENT:
	case EET_USER_EVENT:
		break;
	}

	return false;
}

bool LoginGui::serverLogin(const wchar_t *name)
{
	AvatarProp prop={
		"",
		"../res/models/ninja/ninja.b3d",
		{{206, 250, 15.0f}, {1, 14, 15.0f}},
		0,
		1,
		{0, 0, 0},
		{0, 0, 0}
	};

	sprintf_s(prop.sName, sizeof(prop.sName), "%ls", name);
	IPlayerPrx proxy = pNet_->login(prop.sName);
	if (!proxy)
		return false;

	LocalPlayer *player = new LocalPlayer(prop, pNet_, proxy);
	
	irr::SEvent evt;
	evt.EventType = irr::EET_USER_EVENT;
	evt.UserEvent.UserData1 = EACTID_Born;
	evt.UserEvent.UserData2 = reinterpret_cast<irr::s32>(player);
	pDevice_->postEventFromUser(evt);
	
	return true;
}
