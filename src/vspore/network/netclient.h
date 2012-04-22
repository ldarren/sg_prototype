#pragma once
#ifndef _VSPORE_NETWORK_CLIENT_H_
#define _VSPORE_NETWORK_CLIENT_H_

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <libnetwork/i_chat.h>
#include <libnetwork/i_world.h>

namespace meta {

class NetClient
{
public:
	NetClient();

	bool init(IZoneCBPtr);
	bool deinit();

	IPlayerPrx	login(const char *name);
	void		logout();

	IWorldPrx getWorldClient() const;
	IZonePrx getZoneClient() const;
	Ice::CommunicatorPtr getCommunicator();
	Ice::ObjectAdapterPtr getCBAdapter();

private:
	Ice::CommunicatorPtr	pComm__;
	Ice::ObjectAdapterPtr	pCBAdapter_;
	Ice::Identity			mId__;
	IZonePrx				pZoneProxy__;
	IZoneCBPtr				pZone__;
	IWorldPrx				pWorldProxy__;
};

} // namespace meta

#endif // _VSPORE_NETWORK_CLIENT_H_