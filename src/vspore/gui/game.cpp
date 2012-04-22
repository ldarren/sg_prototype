#include "./game.h"

using namespace meta;
using namespace irr;
using namespace irr::gui;

GameGui::GameGui()
:	pGui_(0), bConsumeKBEvent__(false), pChatEdit__(0), pChatList__(0)
{
}

GameGui::~GameGui()
{
}

void GameGui::create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath)
{
	Parent::create(device, net, info, rcsPath);

	pGui_ = device->getGUIEnvironment();
}

void GameGui::destroy()
{
}

void GameGui::onWake()
{
	Parent::onWake();

	// GUI
	pGui_->loadGUI(pPath_);

	IGUIElement *root = pGui_->getRootGUIElement();
	//IGUIElement *win = root->getElementFromId(0, true);
	//win->setRelativePosition(position2di((cfg->nWidth-310)/2, cfg->nHeight-160));

	pChatEdit__ = reinterpret_cast<IGUIEditBox*>(root->getElementFromId(EGUIID_ChatEdit, true));
	pChatList__ = reinterpret_cast<IGUIListBox*>(root->getElementFromId(EGUIID_ChatList, true));
}

void GameGui::onSleep()
{
	pGui_->clear();

	Parent::onSleep();
}

bool GameGui::update(F32 elapsed)
{
	pGui_->drawAll();
	return true;
}

bool GameGui::onEvent(const irr::SEvent &evt)
{
	switch (evt.EventType)
	{
	case EET_GUI_EVENT:
		{
			s32 id = evt.GUIEvent.Caller->getID();
			switch(evt.GUIEvent.EventType)
			{
			case EGET_ELEMENT_FOCUSED:
				if (id == EGUIID_ChatEdit)
				{
					bConsumeKBEvent__=true;
				}
				break;
			case EGET_ELEMENT_FOCUS_LOST:
				if (id == EGUIID_ChatEdit)
				{
					bConsumeKBEvent__=false;
				}
				break;
			case EGET_ELEMENT_HOVERED:
				break;
			case EGET_ELEMENT_LEFT:
				break;
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case EGUIID_HomeBtn:
					break;
				case EGUIID_BrowserBtn:
					sprintf_s(sNetMsg__, sizeof(sNetMsg__), "%ls", pChatEdit__->getText());
					pChatEdit__->setText(L"");
					pInfo_->pPlayer->surf(sNetMsg__);
					break;
				case EGUIID_OptionsBtn:
					break;
				case EGUIID_ExitBtn:
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
					sprintf_s(sNetMsg__, sizeof(sNetMsg__), "%ls", pChatEdit__->getText());
					pChatEdit__->setText(L"");
					pInfo_->pPlayer->chat(sNetMsg__);

					break;
				}
			}
			return true;
		}
		break;
	case EET_JOYSTICK_INPUT_EVENT:
	case EET_LOG_TEXT_EVENT:
		break;
	case EET_USER_EVENT:
		switch(evt.UserEvent.UserData1)
		{
		case EACTID_Born:
		case EACTID_Dead:
		case EACTID_Move:
			break;
		case EACTID_Chat:
			NetPlayer *p = reinterpret_cast<NetPlayer*>(evt.UserEvent.UserData2);
			swprintf(sLocalMsg__, L"%S: %S", p->getProperties().sName, p->getChatMsg().c_str());
			
			pChatList__->addItem(sLocalMsg__);
			pChatList__->setSelected(pChatList__->getItemCount()); 
			return true;
		}
		break;
	}

	return false;
}
