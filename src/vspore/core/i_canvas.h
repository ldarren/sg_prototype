#pragma once
#ifndef _VSPORE_CANVAS_INTERFACE_H_
#define _VSPORE_CANVAS_INTERFACE_H_

#include <meta/types.h>
#include <network/netclient.h>
#include <world/world_info.h>
#include <irrlicht.h>

namespace meta {

struct ICanvas
{
	virtual void create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath);
	virtual void destroy();

	virtual void onWake();
	virtual void onSleep();
	virtual bool update(F32 elapsed) = 0;
	virtual bool onEvent(const irr::SEvent &evt) = 0;

	virtual void setId(U8 id);
	virtual U8	 getId() const;
	virtual void changeScene(U8 id);

protected:
	U8							nId_;
	U8							nNewId_;
	NetClient					*pNet_;
	WorldInfo					*pInfo_;
	const char					*pPath_;
	irr::IrrlichtDevice			*pDevice_;
};

} // namespace meta

#endif // _VSPORE_CANVAS_INTERFACE_H_