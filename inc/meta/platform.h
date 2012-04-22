/** 
 * @file	platform.h
 * @brief	Platform interfaces.
 * @author	Darren Liew<br>
 *			Copyright (C) 2009 Metaversum Asia Pte Ltd.
 *
 * Platform interface templates. Generalize thread, event, timer, dll handling and localization
 */
#pragma once
#ifndef _META_PLATFORM_INTERFACES_H_
#define _META_PLATFORM_INTERFACES_H_

#ifdef WIN32
#	include <meta\platform\win32.h>
#elif MAC
#	include <meta\platform\mac.h>
#elif LINUX
#	include <meta\platform\linux.h>
#endif

namespace meta {

struct ModuleCfg
{
	Platform::InstanceHnd	hInstance;
	Platform::WindowHnd		hWindow;
	Platform::WindowHnd		hParentWindow;
	Platform::ContextHnd	hDevice;
	Platform::RendererHnd	hRenderer;
	Platform::EventHnd		hHostQuitEvt;		///< for host thread to trigger plugin's thread stop
	Platform::EventHnd		hPluginQuitEvt;		///< for plugin thread to trigger host thread stop
	Platform::DeviceMode	mOldDevMod;
	Platform::MessageFunc	listener;
	U32						nWidth;
	U32						nHeight;
	U32						nColorDepth;
	U32						nZbufferDepth;
	U32						nFreq;
	U32						nId;
	bool					bFullscreen;
	char					sName[64];			///< Application's class name
	const char				*sTitle;			///< Application's title
	const char				*sIconPath;
	const char				*sCursorPath;
};

} // namespace meta

#endif _META_PLATFORM_INTERFACES_H_