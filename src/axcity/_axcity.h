

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Apr 20 11:08:24 2009
 */
/* Compiler settings for _axcity.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___axcity_h__
#define ___axcity_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IcityCtrl_FWD_DEFINED__
#define __IcityCtrl_FWD_DEFINED__
typedef interface IcityCtrl IcityCtrl;
#endif 	/* __IcityCtrl_FWD_DEFINED__ */


#ifndef __CcityCtrl_FWD_DEFINED__
#define __CcityCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CcityCtrl CcityCtrl;
#else
typedef struct CcityCtrl CcityCtrl;
#endif /* __cplusplus */

#endif 	/* __CcityCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IcityCtrl_INTERFACE_DEFINED__
#define __IcityCtrl_INTERFACE_DEFINED__

/* interface IcityCtrl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IcityCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("64AC281C-32C9-4359-AC2B-673B202F1F6A")
    IcityCtrl : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IcityCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IcityCtrl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IcityCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IcityCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IcityCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IcityCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IcityCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IcityCtrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IcityCtrlVtbl;

    interface IcityCtrl
    {
        CONST_VTBL struct IcityCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IcityCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IcityCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IcityCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IcityCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IcityCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IcityCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IcityCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IcityCtrl_INTERFACE_DEFINED__ */



#ifndef __axcity_LIBRARY_DEFINED__
#define __axcity_LIBRARY_DEFINED__

/* library axcity */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_axcity;

EXTERN_C const CLSID CLSID_CcityCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("0ABDAB11-A970-4C01-A5A4-61EDA7B0AF30")
CcityCtrl;
#endif
#endif /* __axcity_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


