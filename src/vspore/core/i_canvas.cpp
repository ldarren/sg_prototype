#include "./i_canvas.h"

using namespace meta;
using namespace irr;

void ICanvas::create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath)
{
	pDevice_ = device;
	pPath_ = rcsPath;
	pNet_ = net;
	pInfo_ = info;
}

void ICanvas::destroy()
{
	pDevice_ = 0;
	pPath_ = 0;
	pNet_ = 0;
	pInfo_ = 0;

	nNewId_ = nId_ = 0;
}

void ICanvas::onWake()
{
	nNewId_ = nId_;
}

void ICanvas::onSleep()
{
}

void ICanvas::setId(U8 id)
{
	nNewId_ = nId_ = id;
}

U8 ICanvas::getId() const
{
	return nId_;
}

void ICanvas::changeScene(U8 id)
{
	nNewId_ = id;
}
