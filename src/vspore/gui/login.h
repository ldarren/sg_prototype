#pragma once
#ifndef _VSPORE_GUI_LOGIN_H_
#define _VSPORE_GUI_LOGIN_H_

#include <core/i_canvas.h>
#include <network/netclient.h>

namespace meta
{

class LoginGui : public ICanvas
{
	typedef ICanvas Parent;

	enum
	{
		EGUIID_Window=200,
		EGUIID_LoginEdit,
		EGUIID_ErrorMsg,
		EGUIID_LoginBtn,
	};

public:
	LoginGui();
	virtual ~LoginGui();

	virtual void create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath);
	virtual void destroy();

	virtual void onWake();
	virtual void onSleep();
	virtual bool update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);

protected:
	bool serverLogin(const wchar_t *name);

protected:
	irr::gui::IGUIEnvironment	*pGui_;

private:
	bool						bConsumeKBEvent__;
	irr::gui::IGUIEditBox		*pLoginEdit__;
	irr::gui::IGUIStaticText	*pErrorText__;
};

} // namespace meta

#endif // _VSPORE_GUI_LOGIN_H_