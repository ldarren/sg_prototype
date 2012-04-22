/** 
 * @file	gfx/core/thread.h
 * @brief	Graphics thread.
 * @author	Darren Liew<br>
 *			Copyright (C) 2009 Metaversum Asia Pte Ltd.
 *
 * Gfx thread definition
 */
#pragma once
#ifndef _GFX_CORE_THREAD_H_
#define _GFX_CORE_THREAD_H_

#include <meta/types.h>

namespace meta {

struct Thread
{
	static U32 __stdcall main(void *args);
};

} // namespace meta

#endif // _GFX_CORE_THREAD_H_