#pragma once
#ifndef _VSPORE_GUI_GAME_H_
#define _VSPORE_GUI_GAME_H_

#include <core/i_canvas.h>
#include <network/netclient.h>

namespace meta {

class GameGui : public ICanvas
{
	typedef ICanvas Parent;

	enum
	{
		EGUIID_Window=100,
		EGUIID_ChatEdit,
		EGUIID_HomeBtn,
		EGUIID_BrowserBtn,
		EGUIID_OptionsBtn,
		EGUIID_ExitBtn,
		EGUIID_ChatList,
	};

public:
	GameGui();
	virtual ~GameGui();

	virtual void create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath);
	virtual void destroy();

	virtual void onWake();
	virtual void onSleep();
	virtual bool update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);

protected:
	irr::gui::IGUIEnvironment	*pGui_;

private:
	bool						bConsumeKBEvent__;
	irr::gui::IGUIEditBox		*pChatEdit__;
	irr::gui::IGUIListBox		*pChatList__;
	char						sNetMsg__[128];
	wchar_t						sLocalMsg__[128];
};

} // namespace meta

#endif // _VSPORE_GUI_GAME_H_