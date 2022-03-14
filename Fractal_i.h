

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Mon Jan 18 21:14:07 2038
 */
/* Compiler settings for Fractal.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __Fractal_i_h__
#define __Fractal_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IFractalDoc_FWD_DEFINED__
#define __IFractalDoc_FWD_DEFINED__
typedef interface IFractalDoc IFractalDoc;

#endif 	/* __IFractalDoc_FWD_DEFINED__ */


#ifndef __CFractalDoc_FWD_DEFINED__
#define __CFractalDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CFractalDoc CFractalDoc;
#else
typedef struct CFractalDoc CFractalDoc;
#endif /* __cplusplus */

#endif 	/* __CFractalDoc_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __FractalLib_LIBRARY_DEFINED__
#define __FractalLib_LIBRARY_DEFINED__

/* library FractalLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_FractalLib;

#ifndef __IFractalDoc_DISPINTERFACE_DEFINED__
#define __IFractalDoc_DISPINTERFACE_DEFINED__

/* dispinterface IFractalDoc */
/* [uuid] */ 


EXTERN_C const IID DIID_IFractalDoc;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("76900ADC-BD0F-4e83-825D-96CF4DCC4D1C")
    IFractalDoc : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IFractalDocVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFractalDoc * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFractalDoc * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFractalDoc * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFractalDoc * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFractalDoc * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFractalDoc * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFractalDoc * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IFractalDocVtbl;

    interface IFractalDoc
    {
        CONST_VTBL struct IFractalDocVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFractalDoc_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFractalDoc_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFractalDoc_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFractalDoc_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFractalDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFractalDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFractalDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IFractalDoc_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CFractalDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("5446301F-C657-4852-98EB-18FFBB64E136")
CFractalDoc;
#endif
#endif /* __FractalLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


