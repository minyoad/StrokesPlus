/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Mar 15 17:14:27 2004
 */
/* Compiler settings for C:\src\Driver\SynCom\SynCtrl\SynCtrl.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __SynCtrl_h__
#define __SynCtrl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ISynAPICtrl_FWD_DEFINED__
#define __ISynAPICtrl_FWD_DEFINED__
typedef interface ISynAPICtrl ISynAPICtrl;
#endif 	/* __ISynAPICtrl_FWD_DEFINED__ */


#ifndef __ISynDeviceCtrl_FWD_DEFINED__
#define __ISynDeviceCtrl_FWD_DEFINED__
typedef interface ISynDeviceCtrl ISynDeviceCtrl;
#endif 	/* __ISynDeviceCtrl_FWD_DEFINED__ */


#ifndef __ISynPacketCtrl_FWD_DEFINED__
#define __ISynPacketCtrl_FWD_DEFINED__
typedef interface ISynPacketCtrl ISynPacketCtrl;
#endif 	/* __ISynPacketCtrl_FWD_DEFINED__ */


#ifndef __ISynDisplayCtrl_FWD_DEFINED__
#define __ISynDisplayCtrl_FWD_DEFINED__
typedef interface ISynDisplayCtrl ISynDisplayCtrl;
#endif 	/* __ISynDisplayCtrl_FWD_DEFINED__ */


#ifndef ___ISynAPICtrlEvents_FWD_DEFINED__
#define ___ISynAPICtrlEvents_FWD_DEFINED__
typedef interface _ISynAPICtrlEvents _ISynAPICtrlEvents;
#endif 	/* ___ISynAPICtrlEvents_FWD_DEFINED__ */


#ifndef __SynAPICtrl_FWD_DEFINED__
#define __SynAPICtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynAPICtrl SynAPICtrl;
#else
typedef struct SynAPICtrl SynAPICtrl;
#endif /* __cplusplus */

#endif 	/* __SynAPICtrl_FWD_DEFINED__ */


#ifndef __SynPacketCtrl_FWD_DEFINED__
#define __SynPacketCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynPacketCtrl SynPacketCtrl;
#else
typedef struct SynPacketCtrl SynPacketCtrl;
#endif /* __cplusplus */

#endif 	/* __SynPacketCtrl_FWD_DEFINED__ */


#ifndef ___ISynDeviceCtrlEvents_FWD_DEFINED__
#define ___ISynDeviceCtrlEvents_FWD_DEFINED__
typedef interface _ISynDeviceCtrlEvents _ISynDeviceCtrlEvents;
#endif 	/* ___ISynDeviceCtrlEvents_FWD_DEFINED__ */


#ifndef __SynDeviceCtrl_FWD_DEFINED__
#define __SynDeviceCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynDeviceCtrl SynDeviceCtrl;
#else
typedef struct SynDeviceCtrl SynDeviceCtrl;
#endif /* __cplusplus */

#endif 	/* __SynDeviceCtrl_FWD_DEFINED__ */


#ifndef ___ISynDisplayCtrlEvents_FWD_DEFINED__
#define ___ISynDisplayCtrlEvents_FWD_DEFINED__
typedef interface _ISynDisplayCtrlEvents _ISynDisplayCtrlEvents;
#endif 	/* ___ISynDisplayCtrlEvents_FWD_DEFINED__ */


#ifndef __SynDisplayCtrl_FWD_DEFINED__
#define __SynDisplayCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynDisplayCtrl SynDisplayCtrl;
#else
typedef struct SynDisplayCtrl SynDisplayCtrl;
#endif /* __cplusplus */

#endif 	/* __SynDisplayCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_SynCtrl_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_SynCtrl_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SynCtrl_0000_v0_0_s_ifspec;

#ifndef __ISynAPICtrl_INTERFACE_DEFINED__
#define __ISynAPICtrl_INTERFACE_DEFINED__

/* interface ISynAPICtrl */
/* [helpstring][oleautomation][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_ISynAPICtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5C66C161-9F39-42E2-A0E8-B0787560F058")
    ISynAPICtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindDevice( 
            enum SynConnectionType eConnectionType,
            enum SynDeviceType eDeviceType,
            /* [in] */ long lLast,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongProperty( 
            enum SynAPIProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLongProperty( 
            enum SynAPIProperty eProperty,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            enum SynAPIStringProperty eProperty,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HardwareBroadcast( 
            long lAction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PersistState( 
            long lStateFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RestoreState( 
            long lStateFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Deactivate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynAPICtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynAPICtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynAPICtrl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynAPICtrl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISynAPICtrl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISynAPICtrl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISynAPICtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISynAPICtrl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            ISynAPICtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindDevice )( 
            ISynAPICtrl __RPC_FAR * This,
            enum SynConnectionType eConnectionType,
            enum SynDeviceType eDeviceType,
            /* [in] */ long lLast,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongProperty )( 
            ISynAPICtrl __RPC_FAR * This,
            enum SynAPIProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLongProperty )( 
            ISynAPICtrl __RPC_FAR * This,
            enum SynAPIProperty eProperty,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynAPICtrl __RPC_FAR * This,
            enum SynAPIStringProperty eProperty,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HardwareBroadcast )( 
            ISynAPICtrl __RPC_FAR * This,
            long lAction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            ISynAPICtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistState )( 
            ISynAPICtrl __RPC_FAR * This,
            long lStateFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RestoreState )( 
            ISynAPICtrl __RPC_FAR * This,
            long lStateFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deactivate )( 
            ISynAPICtrl __RPC_FAR * This);
        
        END_INTERFACE
    } ISynAPICtrlVtbl;

    interface ISynAPICtrl
    {
        CONST_VTBL struct ISynAPICtrlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynAPICtrl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynAPICtrl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynAPICtrl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynAPICtrl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISynAPICtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISynAPICtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISynAPICtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISynAPICtrl_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define ISynAPICtrl_FindDevice(This,eConnectionType,eDeviceType,lLast,pVal)	\
    (This)->lpVtbl -> FindDevice(This,eConnectionType,eDeviceType,lLast,pVal)

#define ISynAPICtrl_GetLongProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetLongProperty(This,eProperty,pVal)

#define ISynAPICtrl_SetLongProperty(This,eProperty,newVal)	\
    (This)->lpVtbl -> SetLongProperty(This,eProperty,newVal)

#define ISynAPICtrl_GetStringProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetStringProperty(This,eProperty,pVal)

#define ISynAPICtrl_HardwareBroadcast(This,lAction)	\
    (This)->lpVtbl -> HardwareBroadcast(This,lAction)

#define ISynAPICtrl_Activate(This)	\
    (This)->lpVtbl -> Activate(This)

#define ISynAPICtrl_PersistState(This,lStateFlags)	\
    (This)->lpVtbl -> PersistState(This,lStateFlags)

#define ISynAPICtrl_RestoreState(This,lStateFlags)	\
    (This)->lpVtbl -> RestoreState(This,lStateFlags)

#define ISynAPICtrl_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_Initialize_Proxy( 
    ISynAPICtrl __RPC_FAR * This);


void __RPC_STUB ISynAPICtrl_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_FindDevice_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    enum SynConnectionType eConnectionType,
    enum SynDeviceType eDeviceType,
    /* [in] */ long lLast,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynAPICtrl_FindDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_GetLongProperty_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    enum SynAPIProperty eProperty,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynAPICtrl_GetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_SetLongProperty_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    enum SynAPIProperty eProperty,
    /* [in] */ long newVal);


void __RPC_STUB ISynAPICtrl_SetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_GetStringProperty_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    enum SynAPIStringProperty eProperty,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISynAPICtrl_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_HardwareBroadcast_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    long lAction);


void __RPC_STUB ISynAPICtrl_HardwareBroadcast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_Activate_Proxy( 
    ISynAPICtrl __RPC_FAR * This);


void __RPC_STUB ISynAPICtrl_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_PersistState_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    long lStateFlags);


void __RPC_STUB ISynAPICtrl_PersistState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_RestoreState_Proxy( 
    ISynAPICtrl __RPC_FAR * This,
    long lStateFlags);


void __RPC_STUB ISynAPICtrl_RestoreState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynAPICtrl_Deactivate_Proxy( 
    ISynAPICtrl __RPC_FAR * This);


void __RPC_STUB ISynAPICtrl_Deactivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynAPICtrl_INTERFACE_DEFINED__ */


#ifndef __ISynDeviceCtrl_INTERFACE_DEFINED__
#define __ISynDeviceCtrl_INTERFACE_DEFINED__

/* interface ISynDeviceCtrl */
/* [helpstring][oleautomation][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_ISynDeviceCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8317C718-98AB-484A-A12F-EDEB48C8B39F")
    ISynDeviceCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Select( 
            long lDeviceHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongProperty( 
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLongProperty( 
            enum SynDeviceProperty eProperty,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadPacket( 
            /* [in] */ ISynPacketCtrl __RPC_FAR *pPacket,
            /* [retval][out] */ long __RPC_FAR *lAvailable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ForceMotion( 
            long lDeltaX,
            long lDeltaY,
            long lButtonState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ForcePacket( 
            ISynPacketCtrl __RPC_FAR *pPacket) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Acquire( 
            long lFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Unacquire( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Deactivate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongPropertyDefault( 
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynDeviceCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynDeviceCtrl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynDeviceCtrl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            ISynDeviceCtrl __RPC_FAR * This,
            long lDeviceHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongProperty )( 
            ISynDeviceCtrl __RPC_FAR * This,
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLongProperty )( 
            ISynDeviceCtrl __RPC_FAR * This,
            enum SynDeviceProperty eProperty,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynDeviceCtrl __RPC_FAR * This,
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadPacket )( 
            ISynDeviceCtrl __RPC_FAR * This,
            /* [in] */ ISynPacketCtrl __RPC_FAR *pPacket,
            /* [retval][out] */ long __RPC_FAR *lAvailable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForceMotion )( 
            ISynDeviceCtrl __RPC_FAR * This,
            long lDeltaX,
            long lDeltaY,
            long lButtonState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForcePacket )( 
            ISynDeviceCtrl __RPC_FAR * This,
            ISynPacketCtrl __RPC_FAR *pPacket);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Acquire )( 
            ISynDeviceCtrl __RPC_FAR * This,
            long lFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unacquire )( 
            ISynDeviceCtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            ISynDeviceCtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deactivate )( 
            ISynDeviceCtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongPropertyDefault )( 
            ISynDeviceCtrl __RPC_FAR * This,
            enum SynDeviceProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } ISynDeviceCtrlVtbl;

    interface ISynDeviceCtrl
    {
        CONST_VTBL struct ISynDeviceCtrlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynDeviceCtrl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynDeviceCtrl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynDeviceCtrl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynDeviceCtrl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISynDeviceCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISynDeviceCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISynDeviceCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISynDeviceCtrl_Select(This,lDeviceHandle)	\
    (This)->lpVtbl -> Select(This,lDeviceHandle)

#define ISynDeviceCtrl_GetLongProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetLongProperty(This,eProperty,pVal)

#define ISynDeviceCtrl_SetLongProperty(This,eProperty,newVal)	\
    (This)->lpVtbl -> SetLongProperty(This,eProperty,newVal)

#define ISynDeviceCtrl_GetStringProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetStringProperty(This,eProperty,pVal)

#define ISynDeviceCtrl_LoadPacket(This,pPacket,lAvailable)	\
    (This)->lpVtbl -> LoadPacket(This,pPacket,lAvailable)

#define ISynDeviceCtrl_ForceMotion(This,lDeltaX,lDeltaY,lButtonState)	\
    (This)->lpVtbl -> ForceMotion(This,lDeltaX,lDeltaY,lButtonState)

#define ISynDeviceCtrl_ForcePacket(This,pPacket)	\
    (This)->lpVtbl -> ForcePacket(This,pPacket)

#define ISynDeviceCtrl_Acquire(This,lFlags)	\
    (This)->lpVtbl -> Acquire(This,lFlags)

#define ISynDeviceCtrl_Unacquire(This)	\
    (This)->lpVtbl -> Unacquire(This)

#define ISynDeviceCtrl_Activate(This)	\
    (This)->lpVtbl -> Activate(This)

#define ISynDeviceCtrl_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#define ISynDeviceCtrl_GetLongPropertyDefault(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetLongPropertyDefault(This,eProperty,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_Select_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    long lDeviceHandle);


void __RPC_STUB ISynDeviceCtrl_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_GetLongProperty_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    enum SynDeviceProperty eProperty,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDeviceCtrl_GetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_SetLongProperty_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    enum SynDeviceProperty eProperty,
    /* [in] */ long newVal);


void __RPC_STUB ISynDeviceCtrl_SetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_GetStringProperty_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    enum SynDeviceProperty eProperty,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISynDeviceCtrl_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_LoadPacket_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    /* [in] */ ISynPacketCtrl __RPC_FAR *pPacket,
    /* [retval][out] */ long __RPC_FAR *lAvailable);


void __RPC_STUB ISynDeviceCtrl_LoadPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_ForceMotion_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    long lDeltaX,
    long lDeltaY,
    long lButtonState);


void __RPC_STUB ISynDeviceCtrl_ForceMotion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_ForcePacket_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    ISynPacketCtrl __RPC_FAR *pPacket);


void __RPC_STUB ISynDeviceCtrl_ForcePacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_Acquire_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    long lFlags);


void __RPC_STUB ISynDeviceCtrl_Acquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_Unacquire_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This);


void __RPC_STUB ISynDeviceCtrl_Unacquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_Activate_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This);


void __RPC_STUB ISynDeviceCtrl_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_Deactivate_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This);


void __RPC_STUB ISynDeviceCtrl_Deactivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDeviceCtrl_GetLongPropertyDefault_Proxy( 
    ISynDeviceCtrl __RPC_FAR * This,
    enum SynDeviceProperty eProperty,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDeviceCtrl_GetLongPropertyDefault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynDeviceCtrl_INTERFACE_DEFINED__ */


#ifndef __ISynPacketCtrl_INTERFACE_DEFINED__
#define __ISynPacketCtrl_INTERFACE_DEFINED__

/* interface ISynPacketCtrl */
/* [helpstring][oleautomation][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_ISynPacketCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f9765b3b-73d0-4a8f-b321-a66206a7780a")
    ISynPacketCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongProperty( 
            enum SynPacketProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLongProperty( 
            enum SynPacketProperty eProperty,
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AssociatedDeviceHandle( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AssociatedDeviceHandle( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SequenceNumber( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SequenceNumber( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimeStamp( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TimeStamp( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XRaw( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XRaw( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_YRaw( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_YRaw( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ZRaw( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ZRaw( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ZYRaw( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ZYRaw( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_W( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_W( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Z( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Z( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XDelta( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XDelta( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_YDelta( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_YDelta( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ZDelta( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ZDelta( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XMickeys( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XMickeys( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_YMickeys( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_YMickeys( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FingerState( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FingerState( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExtendedState( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExtendedState( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ButtonState( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ButtonState( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ZXRaw( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ZXRaw( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            ISynPacketCtrl __RPC_FAR *pFrom) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynPacketCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynPacketCtrl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynPacketCtrl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongProperty )( 
            ISynPacketCtrl __RPC_FAR * This,
            enum SynPacketProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLongProperty )( 
            ISynPacketCtrl __RPC_FAR * This,
            enum SynPacketProperty eProperty,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AssociatedDeviceHandle )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AssociatedDeviceHandle )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SequenceNumber )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SequenceNumber )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimeStamp )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TimeStamp )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_XRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_YRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_YRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZYRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZYRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_W )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_W )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_X )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_X )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Y )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Y )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Z )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Z )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_XDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_YDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_YDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZDelta )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XMickeys )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_XMickeys )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_YMickeys )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_YMickeys )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FingerState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FingerState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExtendedState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExtendedState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ButtonState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ButtonState )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZXRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZXRaw )( 
            ISynPacketCtrl __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )( 
            ISynPacketCtrl __RPC_FAR * This,
            ISynPacketCtrl __RPC_FAR *pFrom);
        
        END_INTERFACE
    } ISynPacketCtrlVtbl;

    interface ISynPacketCtrl
    {
        CONST_VTBL struct ISynPacketCtrlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynPacketCtrl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynPacketCtrl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynPacketCtrl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynPacketCtrl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISynPacketCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISynPacketCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISynPacketCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISynPacketCtrl_GetLongProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetLongProperty(This,eProperty,pVal)

#define ISynPacketCtrl_SetLongProperty(This,eProperty,newVal)	\
    (This)->lpVtbl -> SetLongProperty(This,eProperty,newVal)

#define ISynPacketCtrl_get_AssociatedDeviceHandle(This,pVal)	\
    (This)->lpVtbl -> get_AssociatedDeviceHandle(This,pVal)

#define ISynPacketCtrl_put_AssociatedDeviceHandle(This,newVal)	\
    (This)->lpVtbl -> put_AssociatedDeviceHandle(This,newVal)

#define ISynPacketCtrl_get_SequenceNumber(This,pVal)	\
    (This)->lpVtbl -> get_SequenceNumber(This,pVal)

#define ISynPacketCtrl_put_SequenceNumber(This,newVal)	\
    (This)->lpVtbl -> put_SequenceNumber(This,newVal)

#define ISynPacketCtrl_get_TimeStamp(This,pVal)	\
    (This)->lpVtbl -> get_TimeStamp(This,pVal)

#define ISynPacketCtrl_put_TimeStamp(This,newVal)	\
    (This)->lpVtbl -> put_TimeStamp(This,newVal)

#define ISynPacketCtrl_get_XRaw(This,pVal)	\
    (This)->lpVtbl -> get_XRaw(This,pVal)

#define ISynPacketCtrl_put_XRaw(This,newVal)	\
    (This)->lpVtbl -> put_XRaw(This,newVal)

#define ISynPacketCtrl_get_YRaw(This,pVal)	\
    (This)->lpVtbl -> get_YRaw(This,pVal)

#define ISynPacketCtrl_put_YRaw(This,newVal)	\
    (This)->lpVtbl -> put_YRaw(This,newVal)

#define ISynPacketCtrl_get_ZRaw(This,pVal)	\
    (This)->lpVtbl -> get_ZRaw(This,pVal)

#define ISynPacketCtrl_put_ZRaw(This,newVal)	\
    (This)->lpVtbl -> put_ZRaw(This,newVal)

#define ISynPacketCtrl_get_ZYRaw(This,pVal)	\
    (This)->lpVtbl -> get_ZYRaw(This,pVal)

#define ISynPacketCtrl_put_ZYRaw(This,newVal)	\
    (This)->lpVtbl -> put_ZYRaw(This,newVal)

#define ISynPacketCtrl_get_W(This,pVal)	\
    (This)->lpVtbl -> get_W(This,pVal)

#define ISynPacketCtrl_put_W(This,newVal)	\
    (This)->lpVtbl -> put_W(This,newVal)

#define ISynPacketCtrl_get_X(This,pVal)	\
    (This)->lpVtbl -> get_X(This,pVal)

#define ISynPacketCtrl_put_X(This,newVal)	\
    (This)->lpVtbl -> put_X(This,newVal)

#define ISynPacketCtrl_get_Y(This,pVal)	\
    (This)->lpVtbl -> get_Y(This,pVal)

#define ISynPacketCtrl_put_Y(This,newVal)	\
    (This)->lpVtbl -> put_Y(This,newVal)

#define ISynPacketCtrl_get_Z(This,pVal)	\
    (This)->lpVtbl -> get_Z(This,pVal)

#define ISynPacketCtrl_put_Z(This,newVal)	\
    (This)->lpVtbl -> put_Z(This,newVal)

#define ISynPacketCtrl_get_XDelta(This,pVal)	\
    (This)->lpVtbl -> get_XDelta(This,pVal)

#define ISynPacketCtrl_put_XDelta(This,newVal)	\
    (This)->lpVtbl -> put_XDelta(This,newVal)

#define ISynPacketCtrl_get_YDelta(This,pVal)	\
    (This)->lpVtbl -> get_YDelta(This,pVal)

#define ISynPacketCtrl_put_YDelta(This,newVal)	\
    (This)->lpVtbl -> put_YDelta(This,newVal)

#define ISynPacketCtrl_get_ZDelta(This,pVal)	\
    (This)->lpVtbl -> get_ZDelta(This,pVal)

#define ISynPacketCtrl_put_ZDelta(This,newVal)	\
    (This)->lpVtbl -> put_ZDelta(This,newVal)

#define ISynPacketCtrl_get_XMickeys(This,pVal)	\
    (This)->lpVtbl -> get_XMickeys(This,pVal)

#define ISynPacketCtrl_put_XMickeys(This,newVal)	\
    (This)->lpVtbl -> put_XMickeys(This,newVal)

#define ISynPacketCtrl_get_YMickeys(This,pVal)	\
    (This)->lpVtbl -> get_YMickeys(This,pVal)

#define ISynPacketCtrl_put_YMickeys(This,newVal)	\
    (This)->lpVtbl -> put_YMickeys(This,newVal)

#define ISynPacketCtrl_get_FingerState(This,pVal)	\
    (This)->lpVtbl -> get_FingerState(This,pVal)

#define ISynPacketCtrl_put_FingerState(This,newVal)	\
    (This)->lpVtbl -> put_FingerState(This,newVal)

#define ISynPacketCtrl_get_ExtendedState(This,pVal)	\
    (This)->lpVtbl -> get_ExtendedState(This,pVal)

#define ISynPacketCtrl_put_ExtendedState(This,newVal)	\
    (This)->lpVtbl -> put_ExtendedState(This,newVal)

#define ISynPacketCtrl_get_ButtonState(This,pVal)	\
    (This)->lpVtbl -> get_ButtonState(This,pVal)

#define ISynPacketCtrl_put_ButtonState(This,newVal)	\
    (This)->lpVtbl -> put_ButtonState(This,newVal)

#define ISynPacketCtrl_get_ZXRaw(This,pVal)	\
    (This)->lpVtbl -> get_ZXRaw(This,pVal)

#define ISynPacketCtrl_put_ZXRaw(This,newVal)	\
    (This)->lpVtbl -> put_ZXRaw(This,newVal)

#define ISynPacketCtrl_Copy(This,pFrom)	\
    (This)->lpVtbl -> Copy(This,pFrom)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_GetLongProperty_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    enum SynPacketProperty eProperty,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_GetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_SetLongProperty_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    enum SynPacketProperty eProperty,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_SetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_AssociatedDeviceHandle_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_AssociatedDeviceHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_AssociatedDeviceHandle_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_AssociatedDeviceHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_SequenceNumber_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_SequenceNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_SequenceNumber_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_SequenceNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_TimeStamp_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_TimeStamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_TimeStamp_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_TimeStamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_XRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_XRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_XRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_XRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_YRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_YRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_YRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_YRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ZRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ZRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ZRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ZRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ZYRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ZYRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ZYRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ZYRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_W_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_W_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_W_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_W_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_X_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_X_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_Y_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_Y_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_Z_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_Z_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_XDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_XDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_XDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_XDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_YDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_YDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_YDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_YDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ZDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ZDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ZDelta_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ZDelta_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_XMickeys_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_XMickeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_XMickeys_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_XMickeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_YMickeys_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_YMickeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_YMickeys_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_YMickeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_FingerState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_FingerState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_FingerState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_FingerState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ExtendedState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ExtendedState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ExtendedState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ExtendedState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ButtonState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ButtonState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ButtonState_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ButtonState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_get_ZXRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynPacketCtrl_get_ZXRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_put_ZXRaw_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISynPacketCtrl_put_ZXRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynPacketCtrl_Copy_Proxy( 
    ISynPacketCtrl __RPC_FAR * This,
    ISynPacketCtrl __RPC_FAR *pFrom);


void __RPC_STUB ISynPacketCtrl_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynPacketCtrl_INTERFACE_DEFINED__ */


#ifndef __ISynDisplayCtrl_INTERFACE_DEFINED__
#define __ISynDisplayCtrl_INTERFACE_DEFINED__

/* interface ISynDisplayCtrl */
/* [helpstring][oleautomation][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_ISynDisplayCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("338CF912-683E-438E-9641-76AAD2D62E22")
    ISynDisplayCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongProperty( 
            enum SynDisplayProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLongProperty( 
            enum SynDisplayProperty eProperty,
            long newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Select( 
            long lDeviceHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Flush( 
            enum SynFlushMethod eMethod) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Acquire( 
            enum SynDisplayAquisitionMethod eMethod) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Unacquire( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendPicture( 
            IDispatch __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PixelX( 
            long lTouchX,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PixelY( 
            long lTouchY,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TouchX( 
            long lPixelX,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TouchY( 
            long lPixelY,
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Deactivate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynDisplayCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynDisplayCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynDisplayCtrl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynDisplayCtrl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISynDisplayCtrl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISynDisplayCtrl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISynDisplayCtrl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISynDisplayCtrl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongProperty )( 
            ISynDisplayCtrl __RPC_FAR * This,
            enum SynDisplayProperty eProperty,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLongProperty )( 
            ISynDisplayCtrl __RPC_FAR * This,
            enum SynDisplayProperty eProperty,
            long newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            ISynDisplayCtrl __RPC_FAR * This,
            long lDeviceHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Flush )( 
            ISynDisplayCtrl __RPC_FAR * This,
            enum SynFlushMethod eMethod);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Acquire )( 
            ISynDisplayCtrl __RPC_FAR * This,
            enum SynDisplayAquisitionMethod eMethod);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unacquire )( 
            ISynDisplayCtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendPicture )( 
            ISynDisplayCtrl __RPC_FAR * This,
            IDispatch __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PixelX )( 
            ISynDisplayCtrl __RPC_FAR * This,
            long lTouchX,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PixelY )( 
            ISynDisplayCtrl __RPC_FAR * This,
            long lTouchY,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TouchX )( 
            ISynDisplayCtrl __RPC_FAR * This,
            long lPixelX,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TouchY )( 
            ISynDisplayCtrl __RPC_FAR * This,
            long lPixelY,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            ISynDisplayCtrl __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deactivate )( 
            ISynDisplayCtrl __RPC_FAR * This);
        
        END_INTERFACE
    } ISynDisplayCtrlVtbl;

    interface ISynDisplayCtrl
    {
        CONST_VTBL struct ISynDisplayCtrlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynDisplayCtrl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynDisplayCtrl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynDisplayCtrl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynDisplayCtrl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISynDisplayCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISynDisplayCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISynDisplayCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISynDisplayCtrl_GetLongProperty(This,eProperty,pVal)	\
    (This)->lpVtbl -> GetLongProperty(This,eProperty,pVal)

#define ISynDisplayCtrl_SetLongProperty(This,eProperty,newVal)	\
    (This)->lpVtbl -> SetLongProperty(This,eProperty,newVal)

#define ISynDisplayCtrl_Select(This,lDeviceHandle)	\
    (This)->lpVtbl -> Select(This,lDeviceHandle)

#define ISynDisplayCtrl_Flush(This,eMethod)	\
    (This)->lpVtbl -> Flush(This,eMethod)

#define ISynDisplayCtrl_Acquire(This,eMethod)	\
    (This)->lpVtbl -> Acquire(This,eMethod)

#define ISynDisplayCtrl_Unacquire(This)	\
    (This)->lpVtbl -> Unacquire(This)

#define ISynDisplayCtrl_SendPicture(This,pVal)	\
    (This)->lpVtbl -> SendPicture(This,pVal)

#define ISynDisplayCtrl_get_PixelX(This,lTouchX,pVal)	\
    (This)->lpVtbl -> get_PixelX(This,lTouchX,pVal)

#define ISynDisplayCtrl_get_PixelY(This,lTouchY,pVal)	\
    (This)->lpVtbl -> get_PixelY(This,lTouchY,pVal)

#define ISynDisplayCtrl_get_TouchX(This,lPixelX,pVal)	\
    (This)->lpVtbl -> get_TouchX(This,lPixelX,pVal)

#define ISynDisplayCtrl_get_TouchY(This,lPixelY,pVal)	\
    (This)->lpVtbl -> get_TouchY(This,lPixelY,pVal)

#define ISynDisplayCtrl_Activate(This)	\
    (This)->lpVtbl -> Activate(This)

#define ISynDisplayCtrl_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_GetLongProperty_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    enum SynDisplayProperty eProperty,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_GetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_SetLongProperty_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    enum SynDisplayProperty eProperty,
    long newVal);


void __RPC_STUB ISynDisplayCtrl_SetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Select_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    long lDeviceHandle);


void __RPC_STUB ISynDisplayCtrl_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Flush_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    enum SynFlushMethod eMethod);


void __RPC_STUB ISynDisplayCtrl_Flush_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Acquire_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    enum SynDisplayAquisitionMethod eMethod);


void __RPC_STUB ISynDisplayCtrl_Acquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Unacquire_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This);


void __RPC_STUB ISynDisplayCtrl_Unacquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_SendPicture_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    IDispatch __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_SendPicture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_get_PixelX_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    long lTouchX,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_get_PixelX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_get_PixelY_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    long lTouchY,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_get_PixelY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_get_TouchX_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    long lPixelX,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_get_TouchX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_get_TouchY_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This,
    long lPixelY,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISynDisplayCtrl_get_TouchY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Activate_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This);


void __RPC_STUB ISynDisplayCtrl_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISynDisplayCtrl_Deactivate_Proxy( 
    ISynDisplayCtrl __RPC_FAR * This);


void __RPC_STUB ISynDisplayCtrl_Deactivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynDisplayCtrl_INTERFACE_DEFINED__ */



#ifndef __SYNCTRLLib_LIBRARY_DEFINED__
#define __SYNCTRLLib_LIBRARY_DEFINED__

/* library SYNCTRLLib */
/* [helpstring][version][uuid] */ 


enum SynAPIProperty
    {	SP_First	= 0x10000000,
	SP_Version	= SP_First + 1,
	SP_SynComDllVersion	= SP_Version,
	SP_MaxDevices	= SP_SynComDllVersion + 1,
	SP_DevicesPresent	= SP_MaxDevices + 1,
	SP_DriverVersion	= SP_DevicesPresent + 1,
	SP_RequiredDriverVersion	= SP_DriverVersion + 1,
	SP_APILast	= SP_First + 0x100
    };

enum SynDeviceProperty
    {	SP_DeviceFirst	= SP_APILast,
	SP_Handle	= SP_DeviceFirst + 1,
	SP_DeviceType	= SP_Handle + 1,
	SP_ConnectionType	= SP_DeviceType + 1,
	SP_FWVersion	= SP_ConnectionType + 1,
	SP_Geometry	= SP_FWVersion + 1,
	SP_SensorType	= SP_Geometry + 1,
	SP_ProductType	= SP_SensorType + 1,
	SP_ASICType	= SP_ProductType + 2,
	SP_ReportRate	= SP_ASICType + 4,
	SP_Gestures	= SP_ReportRate + 1,
	SP_SecondaryGestures	= SP_Gestures + 1,
	SP_EdgeMotionOptions	= SP_SecondaryGestures + 1,
	SP_EdgeMotionSpeed	= SP_EdgeMotionOptions + 1,
	SP_MotionRotationAngle	= SP_EdgeMotionSpeed + 1,
	SP_XDPI	= SP_MotionRotationAngle + 1,
	SP_YDPI	= SP_XDPI + 1,
	SP_XLoSensor	= SP_YDPI + 1,
	SP_YLoSensor	= SP_XLoSensor + 1,
	SP_XHiSensor	= SP_YLoSensor + 1,
	SP_YHiSensor	= SP_XHiSensor + 1,
	SP_XLoRim	= SP_YHiSensor + 1,
	SP_YLoRim	= SP_XLoRim + 1,
	SP_XHiRim	= SP_YLoRim + 1,
	SP_YHiRim	= SP_XHiRim + 1,
	SP_XLoBorder	= SP_YHiRim + 1,
	SP_YLoBorder	= SP_XLoBorder + 1,
	SP_XHiBorder	= SP_YLoBorder + 1,
	SP_YHiBorder	= SP_XHiBorder + 1,
	SP_YLoBorderVScroll	= SP_YHiBorder + 1,
	SP_YHiBorderVScroll	= SP_YLoBorderVScroll + 1,
	SP_XLoWideBorder	= SP_YHiBorderVScroll + 1,
	SP_YLoWideBorder	= SP_XLoWideBorder + 1,
	SP_XHiWideBorder	= SP_YLoWideBorder + 1,
	SP_YHiWideBorder	= SP_XHiWideBorder + 1,
	SP_ZMaximum	= SP_YHiWideBorder + 1,
	SP_ZTouchThreshold	= SP_ZMaximum + 1,
	SP_TopLeftCornerWidth	= SP_ZTouchThreshold + 1,
	SP_TopLeftCornerHeight	= SP_TopLeftCornerWidth + 1,
	SP_TopRightCornerWidth	= SP_TopLeftCornerHeight + 1,
	SP_TopRightCornerHeight	= SP_TopRightCornerWidth + 1,
	SP_BottomRightCornerWidth	= SP_TopRightCornerHeight + 1,
	SP_BottomRightCornerHeight	= SP_BottomRightCornerWidth + 1,
	SP_BottomLeftCornerWidth	= SP_BottomRightCornerHeight + 1,
	SP_BottomLeftCornerHeight	= SP_BottomLeftCornerWidth + 1,
	SP_TopLeftCornerAction	= SP_BottomLeftCornerHeight + 1,
	SP_TopRightCornerAction	= SP_TopLeftCornerAction + 1,
	SP_BottomRightCornerAction	= SP_TopRightCornerAction + 1,
	SP_BottomLeftCornerAction	= SP_BottomRightCornerAction + 1,
	SP_LeftButtonAction	= SP_BottomLeftCornerAction + 1,
	SP_RightButtonAction	= SP_LeftButtonAction + 1,
	SP_BothButtonAction	= SP_RightButtonAction + 1,
	SP_MiddleButtonAction	= SP_BothButtonAction + 1,
	SP_UpButtonAction	= SP_MiddleButtonAction + 1,
	SP_DownButtonAction	= SP_UpButtonAction + 1,
	SP_Ex1ButtonAction	= SP_DownButtonAction + 1,
	SP_Ex2ButtonAction	= SP_Ex1ButtonAction + 1,
	SP_Ex3ButtonAction	= SP_Ex2ButtonAction + 1,
	SP_Ex4ButtonAction	= SP_Ex3ButtonAction + 1,
	SP_Ex5ButtonAction	= SP_Ex4ButtonAction + 1,
	SP_Ex6ButtonAction	= SP_Ex5ButtonAction + 1,
	SP_Ex7ButtonAction	= SP_Ex6ButtonAction + 1,
	SP_Ex8ButtonAction	= SP_Ex7ButtonAction + 1,
	SP_ExtendedButtons	= SP_Ex8ButtonAction + 1,
	SP_HasMiddleButton	= SP_ExtendedButtons + 1,
	SP_HasUpDownButtons	= SP_HasMiddleButton + 1,
	SP_IsMultiFingerCapable	= SP_HasUpDownButtons + 1,
	SP_IsPenCapable	= SP_IsMultiFingerCapable + 1,
	SP_IsVScroll	= SP_IsPenCapable + 1,
	SP_IsHScroll	= SP_IsVScroll + 1,
	SP_IsWEMode	= SP_IsHScroll + 1,
	SP_IsLowReportRate	= SP_IsWEMode + 1,
	SP_IsHighReportRate	= SP_IsLowReportRate + 1,
	SP_IsTapEnabled	= SP_IsHighReportRate + 1,
	SP_IsDragEnabled	= SP_IsTapEnabled + 1,
	SP_IsDragLockEnabled	= SP_IsDragEnabled + 1,
	SP_IsCornerTapEnabled	= SP_IsDragLockEnabled + 1,
	SP_IsEdgeMotionEnabled	= SP_IsCornerTapEnabled + 1,
	SP_IsEdgeMotionDragEnabled	= SP_IsEdgeMotionEnabled + 1,
	SP_IsEdgeMotionMoveEnabled	= SP_IsEdgeMotionDragEnabled + 1,
	SP_IsReleaseToSelectEnabled	= SP_IsEdgeMotionMoveEnabled + 1,
	SP_IsMiddleTapToHelpEnabled	= SP_IsReleaseToSelectEnabled + 1,
	SP_IsMiddleButtonBlockEnabled	= SP_IsMiddleTapToHelpEnabled + 1,
	SP_IsPressureDragEnabled	= SP_IsMiddleButtonBlockEnabled + 1,
	SP_Is3ButtonEnabled	= SP_IsPressureDragEnabled + 1,
	SP_IsPressureEdgeMotionEnabled	= SP_Is3ButtonEnabled + 1,
	SP_IsMiddleButtonLock	= SP_IsPressureEdgeMotionEnabled + 1,
	SP_Button4Action	= SP_IsMiddleButtonLock + 1,
	SP_Button5Action	= SP_Button4Action + 1,
	SP_VerticalScrollingFlags	= SP_Button5Action + 17,
	SP_HorizontalScrollingFlags	= SP_VerticalScrollingFlags + 1,
	SP_DisplayFlags	= SP_HorizontalScrollingFlags + 1,
	SP_ModelId	= SP_DisplayFlags + 1,
	SP_DisableState	= SP_ModelId + 1,
	SP_DeviceLast	= SP_DeviceFirst + 0x200
    };

enum SynPacketProperty
    {	SP_PacketFirst	= SP_DeviceLast,
	SP_AssociatedDeviceHandle	= SP_PacketFirst + 1,
	SP_SequenceNumber	= SP_AssociatedDeviceHandle + 1,
	SP_TimeStamp	= SP_SequenceNumber + 1,
	SP_XRaw	= SP_TimeStamp + 1,
	SP_ZXRaw	= SP_XRaw,
	SP_YRaw	= SP_ZXRaw + 1,
	SP_ZRaw	= SP_YRaw + 1,
	SP_ZYRaw	= SP_ZRaw + 1,
	SP_W	= SP_ZYRaw + 1,
	SP_X	= SP_W + 1,
	SP_Y	= SP_X + 1,
	SP_Z	= SP_Y + 1,
	SP_XDelta	= SP_Z + 1,
	SP_YDelta	= SP_XDelta + 1,
	SP_ZDelta	= SP_YDelta + 1,
	SP_XMickeys	= SP_ZDelta + 1,
	SP_YMickeys	= SP_XMickeys + 1,
	SP_AnachronisticState	= SP_YMickeys + 1,
	SP_FingerState	= SP_AnachronisticState + 1,
	SP_ExtendedState	= SP_FingerState + 1,
	SP_ButtonState	= SP_ExtendedState + 1,
	SP_ExtraFingerState	= SP_ButtonState + 1,
	SP_PacketLast	= SP_PacketFirst + 0x100
    };

enum SynDisplayProperty
    {	SP_DisplayFirst	= SP_PacketLast,
	SP_BackLightState	= SP_DisplayFirst + 1,
	SP_DisplayRows	= SP_BackLightState + 1,
	SP_DisplayColumns	= SP_DisplayRows + 1,
	SP_DisplayOwned	= SP_DisplayColumns + 1,
	SP_BackLightOnOffOnce	= SP_DisplayOwned + 1,
	SP_DisplayLast	= SP_DisplayFirst + 0x100
    };

enum SynAPIStringProperty
    {	SP_StringFirst	= SP_DisplayLast,
	SP_APIStringFirst	= SP_StringFirst,
	SP_VersionString	= SP_APIStringFirst + 1,
	SP_APIStringLast	= SP_APIStringFirst + 0x100
    };

enum SynDeviceStringProperty
    {	SP_DeviceStringFirst	= SP_APIStringLast,
	SP_ModelString	= SP_DeviceStringFirst + 1,
	SP_PnPID	= SP_ModelString + 1,
	SP_ShortName	= SP_PnPID + 1,
	SP_DeviceStringLast	= SP_DeviceStringFirst + 0x100,
	SP_StringLast	= SP_DeviceStringLast
    };

enum SynDeviceType
    {	SE_DeviceAny	= 0,
	SE_DeviceUnknown	= SE_DeviceAny,
	SE_DeviceMouse	= SE_DeviceUnknown + 1,
	SE_DeviceTouchPad	= SE_DeviceMouse + 1,
	SE_DeviceWheelMouse	= SE_DeviceTouchPad + 1,
	SE_DeviceIBMCompatibleStick	= SE_DeviceWheelMouse + 1,
	SE_DeviceStyk	= SE_DeviceIBMCompatibleStick + 1,
	SE_DeviceFiveButtonWheelMouse	= SE_DeviceStyk + 1,
	SE_DevicecPad	= SE_DeviceFiveButtonWheelMouse + 1,
	SE_DeviceTouchScreen	= SE_DeviceTouchPad
    };

enum SynConnectionType
    {	SE_ConnectionAny	= 0,
	SE_ConnectionCOM	= SE_ConnectionAny + 1,
	SE_ConnectionPS2	= SE_ConnectionCOM + 1,
	SE_ConnectionUSB	= SE_ConnectionPS2 + 1,
	SE_ConnectionUnknown	= SE_ConnectionAny
    };

enum SynNotificationReason
    {	SE_Configuration_Changed	= 0x1,
	SE_DeviceRemoved	= 0x2,
	SE_DeviceAdded	= 0x4,
	SE_InternalPS2DeviceDisabled	= 0x8,
	SE_InternalPS2DeviceInCompatibilityMode	= 0x10
    };

enum SynButtonFlags
    {	SF_ButtonLeft	= 0x1,
	SF_ButtonRight	= 0x2,
	SF_ButtonMiddle	= 0x4,
	SF_ButtonUp	= 0x10,
	SF_ButtonDown	= 0x20,
	SF_Button4	= 0x40,
	SF_Button5	= 0x80,
	SF_ButtonExtended1	= 0x100,
	SF_ButtonExtended2	= 0x200,
	SF_ButtonExtended3	= 0x400,
	SF_ButtonExtended4	= 0x800,
	SF_ButtonExtended5	= 0x1000,
	SF_ButtonExtended6	= 0x2000,
	SF_ButtonExtended7	= 0x4000,
	SF_ButtonExtended8	= 0x8000,
	SF_ButtonReportedLeft	= 0x10000,
	SF_ButtonReportedRight	= 0x20000,
	SF_ButtonReportedMiddle	= 0x40000,
	SF_ButtonReported4	= 0x80000,
	SF_ButtonReported5	= 0x1000000,
	SF_ButtonVirtualLeft	= 0x100000,
	SF_ButtonVirtualRight	= 0x200000,
	SF_ButtonVirtualMiddle	= 0x400000,
	SF_ButtonVirtual4	= 0x800000,
	SF_ButtonVirtual5	= 0x2000000,
	SF_ButtonAnyVirtual	= 0x2f00000,
	SF_ButtonAnyReported	= 0x10f0000,
	SF_ButtonAnyPhysical	= 0xffff,
	SF_ButtonAny	= 0x3ffffff
    };

enum SynScrollingFlags
    {	SF_ScrollingEnabled	= 0x1,
	SF_ScrollingEdgeMotion	= 0x2,
	SF_ScrollingMomentum	= 0x4,
	SF_ScrollingUnderPointer	= 0x8,
	SF_ScrollingReversed	= 0x40,
	SF_ScrollingAxisSwapped	= 0x80
    };

enum SynHardwareBroadcastFlags
    {	SF_Detect	= 0,
	SF_Configure	= SF_Detect + 1
    };

enum SynPersistFlags
    {	SF_PersistMachine	= 0
    };

enum SynFingerFlags
    {	SF_FingerProx	= 0x200,
	SF_FingerTouch	= 0x400,
	SF_FingerHeavy	= 0x800,
	SF_FingerPress	= 0x1000,
	SF_FingerPresent	= 0x2000,
	SF_FingerPossTap	= 0x4000,
	SF_FingerStylus	= 0x8000,
	SF_FingerTap	= 0x10000,
	SF_FingerDrag	= 0x20000,
	SF_FingerDragLock	= 0x40000,
	SF_FingerPrimGest	= 0x80000,
	SF_FingerSecGest	= 0x100000,
	SF_FingerAuxGest	= 0x200000,
	SF_FingerMotion	= 0x400000,
	SF_FingerMoving	= 0x800000,
	SF_FingerTopLeftTap	= 0x1000000,
	SF_FingerTopRightTap	= 0x2000000,
	SF_FingerBottomLeftTap	= 0x4000000,
	SF_FingerBottomRightTap	= 0x8000000,
	SF_FingerTap2	= 0x40000000,
	SF_FingerTap3	= 0x80000000,
	SF_FingerAllCorner	= 0xf000000,
	SF_FingerAllTap	= 0xcf000000,
	SF_FingerAsButton	= 0xcf380000,
	SF_FingerAllGest	= 0xcf3f1000
    };

enum SynStateFlags
    {	SF_StateFinger	= 0x2000
    };

enum SynExtendedStateFlags
    {	SF_StateScrollFinger	= 0x100
    };

enum SynDisplayFlags
    {	SF_DisplayNone	= 0,
	SF_DisplayPresent	= 0x1
    };

enum SynFlushMethod
    {	SE_FlushSynchronous	= 0,
	SE_FlushAsynchronous	= SE_FlushSynchronous + 1
    };

enum SynDisplayAquisitionMethod
    {	SE_AcquireCooperative	= 0,
	SE_AcquireExclusive	= SE_AcquireCooperative + 1,
	SE_AcquirePassive	= SE_AcquireExclusive + 1
    };

enum SynRequiredVersion
    {	SE_RequiredDriverVersion	= 0x7080300,
	SE_RequiredSynCOMDllVersion	= 0x7080300
    };

enum SynDisplayMessage
    {	SE_AcquisitionLost	= 0x1,
	SE_AcquisitionGained	= 0x2,
	SE_BackgroundImageChanged	= 0x3
    };

enum SynGestures
    {	SF_GestureTap	= 0x1,
	SF_GesturePressToSelect	= 0x1,
	SF_GestureDrag	= 0x2,
	SF_GestureDragLock	= 0x4,
	SF_GesturePressureDrag	= 0x8,
	SF_GestureReleaseToSelect	= 0x10,
	SF_GestureMiddleTapToHelp	= 0x20,
	SF_GestureMiddleButtonLock	= 0x40,
	SF_GestureMiddleButtonBlock	= 0x80,
	SF_GestureTapMask	= 0x7,
	SF_GestureAll	= 0xff
    };

enum SynActions
    {	SF_ActionNone	= 0,
	SF_ActionPrimary	= 0x1,
	SF_ActionSecondary	= 0x2,
	SF_ActionAuxilliary	= 0x4,
	SF_ActionAll	= 0x7
    };

enum SynAcquisitionFlags
    {	SF_AcquireAll	= 0,
	SF_AcquireMotionOnly	= 0x1
    };

EXTERN_C const IID LIBID_SYNCTRLLib;

#ifndef ___ISynAPICtrlEvents_DISPINTERFACE_DEFINED__
#define ___ISynAPICtrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISynAPICtrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ISynAPICtrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("093E1E71-478B-43A0-8A02-3E3074B081E0")
    _ISynAPICtrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISynAPICtrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ISynAPICtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ISynAPICtrlEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ISynAPICtrlEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ISynAPICtrlEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ISynAPICtrlEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ISynAPICtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ISynAPICtrlEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ISynAPICtrlEventsVtbl;

    interface _ISynAPICtrlEvents
    {
        CONST_VTBL struct _ISynAPICtrlEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynAPICtrlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISynAPICtrlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ISynAPICtrlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ISynAPICtrlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ISynAPICtrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ISynAPICtrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ISynAPICtrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISynAPICtrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SynAPICtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("F418EBA0-6A10-4482-AC2B-2D10C807073A")
SynAPICtrl;
#endif

EXTERN_C const CLSID CLSID_SynPacketCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("a220a2df-406f-4d68-9b62-995669ae0c92")
SynPacketCtrl;
#endif

#ifndef ___ISynDeviceCtrlEvents_DISPINTERFACE_DEFINED__
#define ___ISynDeviceCtrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISynDeviceCtrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ISynDeviceCtrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("67FE52E0-3DD2-4BBF-9DA2-D795317F6531")
    _ISynDeviceCtrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISynDeviceCtrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ISynDeviceCtrlEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ISynDeviceCtrlEventsVtbl;

    interface _ISynDeviceCtrlEvents
    {
        CONST_VTBL struct _ISynDeviceCtrlEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynDeviceCtrlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISynDeviceCtrlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ISynDeviceCtrlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ISynDeviceCtrlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ISynDeviceCtrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ISynDeviceCtrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ISynDeviceCtrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISynDeviceCtrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SynDeviceCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("2060435E-AB52-49E1-A2EA-5D31645887CF")
SynDeviceCtrl;
#endif

#ifndef ___ISynDisplayCtrlEvents_DISPINTERFACE_DEFINED__
#define ___ISynDisplayCtrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISynDisplayCtrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ISynDisplayCtrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("763FF858-421F-4E12-B5A5-C63B9285AF57")
    _ISynDisplayCtrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISynDisplayCtrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ISynDisplayCtrlEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ISynDisplayCtrlEventsVtbl;

    interface _ISynDisplayCtrlEvents
    {
        CONST_VTBL struct _ISynDisplayCtrlEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynDisplayCtrlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISynDisplayCtrlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ISynDisplayCtrlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ISynDisplayCtrlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ISynDisplayCtrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ISynDisplayCtrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ISynDisplayCtrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISynDisplayCtrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SynDisplayCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("206D8F65-689B-40D0-8F07-8D974CD8884B")
SynDisplayCtrl;
#endif
#endif /* __SYNCTRLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
