#ifdef WIN32

#include <meta/platform/win32.h>
#include <meta/platform.h>
#include <meta/i_plugin.h>

#include <process.h>
#include <math.h>

using namespace meta;

Platform::Platform()
:	fFreq__(0), bPerfCounterAvail__(false), nCountNew__(0), nCountOld__(0),	nRemainder__(0)
{
}

Platform::~Platform()
{
}

/*	Quoted "http://msdn.microsoft.com/en-us/library/ms644904(VS.85).aspx"
	On a multiprocessor computer, it should not matter which processor is called. 
	However, you can get different results on different processors due to bugs in 
	the basic input/output system (BIOS) or the hardware abstraction layer (HAL). 
	To specify processor affinity for a thread, use the SetThreadAffinityMask function. */
void Platform::init()
{
	// Timer
	SetProcessAffinityMask( GetCurrentProcess(), 1 ); // first processor

	nRemainder__ = 0;
	bPerfCounterAvail__ = QueryPerformanceFrequency((LARGE_INTEGER *) &fFreq__);
	if (bPerfCounterAvail__)
		bPerfCounterAvail__ = QueryPerformanceCounter((LARGE_INTEGER *) &nCountOld__);
	if (!bPerfCounterAvail__)
		nCountOld__ = GetTickCount();

	// event

	// thread
}

void Platform::deinit()
{
}

U32 Platform::timeElapse()
{
	U32 elapsed;

	if(bPerfCounterAvail__)
	{
		QueryPerformanceCounter( (LARGE_INTEGER *) &nCountNew__);
		F64 elapsed64 = (1000.0 * F64(nCountNew__ - nCountOld__) / F64(fFreq__));
		elapsed64 += nRemainder__;
		elapsed = (U32)floor(elapsed64);
		nRemainder__ = elapsed64 - F64(elapsed);
		return elapsed;
	}
	else
	{
		nCountNew__ = GetTickCount();
		elapsed = (U32)(nCountNew__ - nCountOld__);
	}

	nCountOld__ = nCountNew__;
	return elapsed;
}

void Platform::timeSleep(U32 millisec)
{
	::Sleep(millisec);
}

Platform::EventHnd Platform::eventCreate(bool manual, bool set)
{
	return CreateEventA(0, manual, set, 0); 
}

bool Platform::eventDestroy(const EventHnd &evt)
{
	return CloseHandle(evt) == TRUE;
}

bool Platform::eventSet(const EventHnd &evt)
{
	return SetEvent(evt) == TRUE;
}

bool Platform::eventReset(const EventHnd &evt)
{
	return ResetEvent(evt) == TRUE;
}

U32 Platform::eventSelect(U32 count, const EventHnd *list, U32 period, bool wait)
{
	return WaitForMultipleObjects(count, list, wait, period);
}

Platform::MutexHnd& Platform::mutexCreate(Platform::MutexHnd &hnd)
{
	InitializeCriticalSection(&hnd);
	return hnd;
}

void Platform::mutexDestroy(Platform::MutexHnd &hnd)
{
	DeleteCriticalSection(&hnd);
}

void Platform::mutexLock(Platform::MutexHnd &hnd)
{
	EnterCriticalSection(&hnd); 
}

void Platform::mutexUnlock(Platform::MutexHnd &hnd)
{
	LeaveCriticalSection(&hnd);
}

IPlugin* Platform::pluginLoad(char *pluginName, PluginHnd &handle)
{
	handle = LoadLibraryA(pluginName);
	if (!handle) return 0;
	
	GetModuleFunc	loaderFunc = (GetModuleFunc)GetProcAddress(handle, "GetInterface");

	if(!loaderFunc)	return 0;
	IPlugin *loader = loaderFunc();
	if (!loader) return 0;

	return loader;
}

bool Platform::pluginUnload(const PluginHnd &handle)
{
	return FreeLibrary(handle) == TRUE;
}

Platform::ThreadHnd Platform::threadCreate(ThreadFunc func, void *args)
{
	U32 ptr;
	return (HANDLE)_beginthreadex(0, 0, func, args, 0, &ptr);
}

void Platform::threadDestroy(U32 code)
{
	_endthreadex(code);
}

bool Platform::threadStatus(Platform::ThreadHnd hnd)
{
	DWORD code;
	GetExitCodeThread(hnd, &code);
	return code == STILL_ACTIVE;
}

Platform::InstanceHnd Platform::InstanceGet(const char *name)
{
	return GetModuleHandleA(name);
}

bool Platform::windowOpen(ModuleCfg &info)
{
	DWORD		exStyle;
	DWORD		style;
	RECT		rect = { 0, 0, info.nWidth, info.nHeight };

	if (!info.hInstance) 
		info.hInstance = InstanceGet();
	//info.hInstance = ::GetModuleHandle(NULL);
	if (info.hWindow) // close old window
	{
		windowClose(info);
	}

	WNDCLASSEXA wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)(info.listener ? info.listener : ::DefWindowProc);
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= info.hInstance;
	wc.hIcon			= info.sIconPath ? (HICON)LoadImageA(info.hInstance, info.sIconPath, IMAGE_ICON, 0,0, LR_LOADFROMFILE) : 0; //LoadIcon(hInstance_, "../data/icons/pica.ico"); //
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= 0;
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= info.sName;

	if (!RegisterClassExA(&wc))
	{
		goto ln_creation_err;
	}

	if (info.hParentWindow)
	{
		exStyle = WS_EX_TOPMOST;
		style = WS_CHILD | WS_VISIBLE;
	}
	else
	{
		if (info.bFullscreen)
		{
			exStyle = WS_EX_APPWINDOW;
			style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		else
		{
			exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
	}

	AdjustWindowRectEx(&rect, style, FALSE, exStyle);

	if (!(info.hWindow = CreateWindowExA(
		exStyle,
		info.sName,
		info.sTitle,
		style,						// window clipping
		0, 0,						// dun use wRect.top and left here
		rect.right - rect.left,
		rect.bottom - rect.top,
		info.hParentWindow,
		0,
		info.hInstance,
		0)))
	{
		windowClose(info); // unregister class
		goto ln_creation_err;
	}

	ShowWindow(info.hWindow, SW_SHOW);
	SetFocus(info.hWindow);

	if (!(info.hDevice = GetDC(info.hWindow)))
	{
		windowClose(info);
		goto ln_creation_err;
	}

	if (!setDeviceMode(info))
		return false;

	return true;

ln_creation_err:
	DWORD error = GetLastError();
	char *msg;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error, 0, (LPSTR)&msg, 0, NULL);
	MessageBoxA( info.hWindow, msg, "Window Creation Failed", MB_OK | MB_ICONERROR );
	return false;
}

bool Platform::windowClose(ModuleCfg &info)
{
	if (info.bFullscreen)
	{
		ChangeDisplaySettingsA(
			info.mOldDevMod.dmDisplayFrequency!=0 ? &info.mOldDevMod : 0, 0); // switch back to window
		ShowCursor(TRUE);
	}

	if (info.hDevice && !ReleaseDC(info.hWindow, info.hDevice))
	{
		//MessageBoxA(info.hWindow, "Release Device Context Failed", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
	}

	if (info.hWindow && !DestroyWindow(info.hWindow))
	{
		//MessageBoxA(0, "Destroy window Failed", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
	}

	if (!UnregisterClassA(info.sName, info.hInstance))
	{
		//MessageBoxA(0, "Unregister class Failed", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
	}

	info.hDevice=0;
	info.hWindow=0;
	info.hInstance=0;

	return true;
}

bool Platform::setDeviceMode(ModuleCfg &info)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	S32 pixelFormat=1;

	if (info.bFullscreen)
	{
		// cache the current display mode
		EnumDisplaySettingsA(0, ENUM_CURRENT_SETTINGS, &info.mOldDevMod);

		int mode = 0;
		DEVMODE devMode;
		bool found=false;

		while (EnumDisplaySettings(0, mode++, &devMode))
		{
			if (devMode.dmPelsWidth != info.nWidth || devMode.dmPelsHeight != info.nHeight)
				continue;

			if (devMode.dmBitsPerPel != info.nColorDepth)
				continue;

			if (devMode.dmDisplayFrequency != info.nFreq)
				continue;

			if (ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
			{
				S32 totalFormats = DescribePixelFormat(info.hDevice, 1, sizeof(PIXELFORMATDESCRIPTOR), 0);

				for (pixelFormat=1; pixelFormat<totalFormats; ++pixelFormat)
				{
					if (DescribePixelFormat(info.hDevice, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd)==0)
						goto ln_driver_init_failed;

					if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
						continue;

					if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
						continue;

					if ((pfd.iPixelType & PFD_TYPE_RGBA))
						continue;

					if (pfd.cColorBits != info.nColorDepth)
						continue;

					if (pfd.cDepthBits < info.nZbufferDepth)
						continue;

					if (SetPixelFormat(info.hDevice, pixelFormat, &pfd) == FALSE)
						goto ln_driver_init_failed;

					if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
						goto ln_driver_init_failed;

					found = true;
					break; // found
				}
				break;
			}
		}

		if (!found)
			return false;
	}
	else
	{
		// suggesting format, system will give us a best setting
		pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion	= 1;
		pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType	= PFD_TYPE_RGBA;
		pfd.cColorBits	= info.nColorDepth;
		pfd.cDepthBits	= info.nZbufferDepth;

		pixelFormat = ChoosePixelFormat(info.hDevice, &pfd);

		if (pixelFormat != 0)
		{
			// given pfd
			DescribePixelFormat(info.hDevice, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

			if (pfd.cDepthBits < info.nZbufferDepth || pfd.cColorBits < info.nColorDepth)
				// TODO: check the return pfd for resolution
				return false;

			if (SetPixelFormat(info.hDevice, pixelFormat, &pfd) == FALSE)
				goto ln_driver_init_failed;
		}
		else
		{
			goto ln_driver_init_failed;
		}
	}

	return true;

ln_driver_init_failed:
	DWORD error = GetLastError();
	char *msg;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error, 0, (LPSTR)&msg, 0, NULL);
	MessageBoxA( info.hWindow, msg, "Pixel Formatting Failed", MB_OK | MB_ICONERROR );
	return false;
}

bool Platform::windowMsgProc()
{
	if (PeekMessage(&mMsg__, 0, 0, 0, PM_REMOVE))
	{
		if (mMsg__.message == WM_QUIT)
		{
			return false;
		}
		else
		{
			TranslateMessage(&mMsg__);
			DispatchMessage(&mMsg__);
		}
	}

	return true;
}

#endif // WIN32