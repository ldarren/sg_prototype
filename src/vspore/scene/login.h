#pragma once
#ifndef _VSPORE_SCENE_LOGIN_H_
#define _VSPORE_SCENE_LOGIN_H_

#include <core/i_canvas.h>

namespace meta {

class LoginScene : public ICanvas
{
	typedef ICanvas Parent;

public:
	LoginScene();
	virtual ~LoginScene();

	virtual void create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath);
	virtual void destroy();

	virtual void onWake();
	virtual void onSleep();
	virtual bool update(F32 elapsed);
	virtual bool onEvent(const irr::SEvent &evt);

protected:
	irr::scene::ISceneManager	*pScene_;
};

} // namespace meta

#endif // _VSPORE_SCENE_LOGIN_H_