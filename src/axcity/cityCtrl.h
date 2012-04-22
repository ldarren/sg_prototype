// cityCtrl.h : Declaration of the CcityCtrl
#pragma once
#include <libclient/application.h>

#include "resource.h"       // main symbols
#include <atlctl.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

static LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
		printf("fksl");
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// IcityCtrl
[
	object,
	uuid(64AC281C-32C9-4359-AC2B-673B202F1F6A),
	dual,
	helpstring("IcityCtrl Interface"),
	pointer_default(unique)
]
__interface IcityCtrl : public IDispatch
{
};


// CcityCtrl
[
	coclass,
	control,
	default(IcityCtrl),
	threading(apartment),
	vi_progid("axcity.cityCtrl"),
	progid("axcity.cityCtrl.1"),
	version(1.0),
	uuid("0ABDAB11-A970-4C01-A5A4-61EDA7B0AF30"),
	helpstring("cityCtrl Class"),
	support_error_info(IcityCtrl),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CcityCtrl :
	public IcityCtrl,
	public IPersistStreamInitImpl<CcityCtrl>,
	public IOleControlImpl<CcityCtrl>,
	public IOleObjectImpl<CcityCtrl>,
	public IOleInPlaceActiveObjectImpl<CcityCtrl>,
	public IViewObjectExImpl<CcityCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CcityCtrl>,
	public IPersistStorageImpl<CcityCtrl>,
	public ISpecifyPropertyPagesImpl<CcityCtrl>,
	public IQuickActivateImpl<CcityCtrl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CcityCtrl>,
#endif
	public IProvideClassInfo2Impl<&__uuidof(CcityCtrl), NULL>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CcityCtrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComControl<CcityCtrl>
{
protected:
	meta::IModule				*pModule_;
	static meta::Application	mApp;
	static meta::U32			nRefCount;

public:


	CcityCtrl()
		: pModule_(0)
	{
		m_bWindowOnly=true; //DARREN: make sure window only
	}

	virtual ~CcityCtrl()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CcityCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CcityCtrl)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	CHAIN_MSG_MAP(CComControl<CcityCtrl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IcityCtrl
public:
	HRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		const CREATESTRUCT *win = reinterpret_cast<CREATESTRUCT*>(lParam);

		SetCurrentDirectoryA(getenv("VSPORE_DIR"));

		if (nRefCount==0)
			mApp.init();
		++nRefCount;
		if (pModule_ = mApp.createModule())
		{
			mApp.initModule(pModule_, m_hWnd, win->cx, win->cy);
		   return S_OK;
		}
		return S_FALSE;
	}

	HRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		pModule_->requestToQuit();

		return S_OK;
	}

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		const meta::Application::EventHnd *evts = mApp.getPluginQuitEvents();
		mApp.eventSelect(meta::Application::MODULE_COUNT, evts, 10000, false);
		mApp.deinitModule(pModule_);
		mApp.destroyModule(pModule_);

		--nRefCount;
		if (nRefCount == 0)
			mApp.deinit();
	}
};

meta::Application	CcityCtrl::mApp;
meta::U32			CcityCtrl::nRefCount=0;
