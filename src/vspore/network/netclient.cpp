#include "./netclient.h"
#include <string.h>
#include <iostream>

using namespace meta;
using namespace std;

NetClient::NetClient()
: pComm__(0), pCBAdapter_(0), pZone__(0), pZoneProxy__(0), pWorldProxy__(0)
{
}

bool NetClient::init(IZoneCBPtr cb)
{
	bool ret = true;
	pZone__ = cb;

	try {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->load("config.client");
		int argc = 0;
		pComm__ = Ice::initialize(argc, 0, initData);

		pWorldProxy__ = IWorldPrx::checkedCast(pComm__->propertyToProxy("VSpore.Proxy")->ice_twoway());
		if (!pWorldProxy__) 
			ret = false;

		pZoneProxy__ = pWorldProxy__->authenticate();
		if (!pZoneProxy__)
			ret = false;
	} catch (const Ice::Exception &exp) {
		// TODO: a proper log here
		cerr<<exp<<endl;
		ret = false;
	}

	return ret;
}

bool NetClient::deinit()
{
	if (!pComm__) return true;

	bool ret = true;

	try {
		pComm__->shutdown();
		//pComm__->destroy();
	} catch (const Ice::Exception &) {
		ret = false;
	}

	pZone__ = 0;
	return ret;
}

IPlayerPrx NetClient::login(const char *name)
{
	pCBAdapter_ = pComm__->createObjectAdapter("");
	mId__.name = name;
	mId__.category = "zone";
	pCBAdapter_->add(pZone__, mId__);
	pCBAdapter_->activate();

    pZoneProxy__->ice_getConnection()->setAdapter(pCBAdapter_);

	return pZoneProxy__->login(mId__);
}

void NetClient::logout()
{
	if (pZoneProxy__ != 0)
		pZoneProxy__->logout(mId__);
}

IWorldPrx NetClient::getWorldClient() const
{
	return pWorldProxy__;
}

IZonePrx NetClient::getZoneClient() const
{
	return pZoneProxy__;
}

Ice::CommunicatorPtr NetClient::getCommunicator()
{
	return pComm__;
}

Ice::ObjectAdapterPtr NetClient::getCBAdapter()
{
	return pCBAdapter_;
}
