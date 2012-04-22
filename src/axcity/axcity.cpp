// axcity.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"


// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{C5FDF1B0-7FC9-4B01-BFA7-B19F7D89D353}", 
		 name = "axcity", 
		 helpstring = "axcity 1.0 Type Library",
		 resource_name = "IDR_AXCITY") ]
class CaxcityModule
{
public:
// Override CAtlDllModuleT members
};
		 
