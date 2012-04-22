#pragma once
#ifndef _VSPORE_WORLD_INFORMATION_H_
#define _VSPORE_WORLD_INFORMATION_H_

#include <player/local_player.h>
#include <player/net_player.h>
#include <meta/platform.h>

#include <list>

namespace meta {

struct WorldInfo
{
	typedef std::list<NetPlayer*> NetPlayerListType;

	NetPlayerListType	mNetPlayerList;
	LocalPlayer			*pPlayer;
	Platform::MutexHnd	hMutex;
};

} // namespace meta

#endif // _VSPORE_WORLD_INFORMATION_H_