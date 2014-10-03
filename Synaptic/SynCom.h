/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Mar 15 17:14:17 2004
 */
/* Compiler settings for C:\src\Driver\SynCom\SynCom\SynCom.idl:
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

#ifndef __SynCom_h__
#define __SynCom_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef ___ISynAPIEvents_FWD_DEFINED__
#define ___ISynAPIEvents_FWD_DEFINED__
typedef interface _ISynAPIEvents _ISynAPIEvents;
#endif 	/* ___ISynAPIEvents_FWD_DEFINED__ */


#ifndef ___ISynDeviceEvents_FWD_DEFINED__
#define ___ISynDeviceEvents_FWD_DEFINED__
typedef interface _ISynDeviceEvents _ISynDeviceEvents;
#endif 	/* ___ISynDeviceEvents_FWD_DEFINED__ */


#ifndef ___ISynDisplayEvents_FWD_DEFINED__
#define ___ISynDisplayEvents_FWD_DEFINED__
typedef interface _ISynDisplayEvents _ISynDisplayEvents;
#endif 	/* ___ISynDisplayEvents_FWD_DEFINED__ */


#ifndef __ISynAPI_FWD_DEFINED__
#define __ISynAPI_FWD_DEFINED__
typedef interface ISynAPI ISynAPI;
#endif 	/* __ISynAPI_FWD_DEFINED__ */


#ifndef __ISynDevice_FWD_DEFINED__
#define __ISynDevice_FWD_DEFINED__
typedef interface ISynDevice ISynDevice;
#endif 	/* __ISynDevice_FWD_DEFINED__ */


#ifndef __ISynPacket_FWD_DEFINED__
#define __ISynPacket_FWD_DEFINED__
typedef interface ISynPacket ISynPacket;
#endif 	/* __ISynPacket_FWD_DEFINED__ */


#ifndef __ISynDisplay_FWD_DEFINED__
#define __ISynDisplay_FWD_DEFINED__
typedef interface ISynDisplay ISynDisplay;
#endif 	/* __ISynDisplay_FWD_DEFINED__ */


#ifndef __SynAPI_FWD_DEFINED__
#define __SynAPI_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynAPI SynAPI;
#else
typedef struct SynAPI SynAPI;
#endif /* __cplusplus */

#endif 	/* __SynAPI_FWD_DEFINED__ */


#ifndef __SynDevice_FWD_DEFINED__
#define __SynDevice_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynDevice SynDevice;
#else
typedef struct SynDevice SynDevice;
#endif /* __cplusplus */

#endif 	/* __SynDevice_FWD_DEFINED__ */


#ifndef __SynPacket_FWD_DEFINED__
#define __SynPacket_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynPacket SynPacket;
#else
typedef struct SynPacket SynPacket;
#endif /* __cplusplus */

#endif 	/* __SynPacket_FWD_DEFINED__ */


#ifndef __SynDisplay_FWD_DEFINED__
#define __SynDisplay_FWD_DEFINED__

#ifdef __cplusplus
typedef class SynDisplay SynDisplay;
#else
typedef struct SynDisplay SynDisplay;
#endif /* __cplusplus */

#endif 	/* __SynDisplay_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "SynComDefs.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_SynCom_0000 */
/* [local] */ 








extern RPC_IF_HANDLE __MIDL_itf_SynCom_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SynCom_0000_v0_0_s_ifspec;

#ifndef ___ISynAPIEvents_INTERFACE_DEFINED__
#define ___ISynAPIEvents_INTERFACE_DEFINED__

/* interface _ISynAPIEvents */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID__ISynAPIEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2566B5BA-ADDC-4cc6-BBFB-B777E5C860CC")
    _ISynAPIEvents
    {
    public:
        BEGIN_INTERFACE
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnSynAPINotify( 
            long lReason) = 0;
        
        END_INTERFACE
    };
    
#else 	/* C style interface */

    typedef struct _ISynAPIEventsVtbl
    {
        BEGIN_INTERFACE
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSynAPINotify )( 
            _ISynAPIEvents __RPC_FAR * This,
            long lReason);
        
        END_INTERFACE
    } _ISynAPIEventsVtbl;

    interface _ISynAPIEvents
    {
        CONST_VTBL struct _ISynAPIEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynAPIEvents_OnSynAPINotify(This,lReason)	\
    (This)->lpVtbl -> OnSynAPINotify(This,lReason)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _ISynAPIEvents_OnSynAPINotify_Proxy( 
    _ISynAPIEvents __RPC_FAR * This,
    long lReason);


void __RPC_STUB _ISynAPIEvents_OnSynAPINotify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___ISynAPIEvents_INTERFACE_DEFINED__ */


#ifndef ___ISynDeviceEvents_INTERFACE_DEFINED__
#define ___ISynDeviceEvents_INTERFACE_DEFINED__

/* interface _ISynDeviceEvents */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID__ISynDeviceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AE255EED-248F-4998-8376-F063ECB9E220")
    _ISynDeviceEvents
    {
    public:
        BEGIN_INTERFACE
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnSynDevicePacket( 
            long lSequenceNumber) = 0;
        
        END_INTERFACE
    };
    
#else 	/* C style interface */

    typedef struct _ISynDeviceEventsVtbl
    {
        BEGIN_INTERFACE
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSynDevicePacket )( 
            _ISynDeviceEvents __RPC_FAR * This,
            long lSequenceNumber);
        
        END_INTERFACE
    } _ISynDeviceEventsVtbl;

    interface _ISynDeviceEvents
    {
        CONST_VTBL struct _ISynDeviceEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynDeviceEvents_OnSynDevicePacket(This,lSequenceNumber)	\
    (This)->lpVtbl -> OnSynDevicePacket(This,lSequenceNumber)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _ISynDeviceEvents_OnSynDevicePacket_Proxy( 
    _ISynDeviceEvents __RPC_FAR * This,
    long lSequenceNumber);


void __RPC_STUB _ISynDeviceEvents_OnSynDevicePacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___ISynDeviceEvents_INTERFACE_DEFINED__ */


#ifndef ___ISynDisplayEvents_INTERFACE_DEFINED__
#define ___ISynDisplayEvents_INTERFACE_DEFINED__

/* interface _ISynDisplayEvents */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID__ISynDisplayEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4b55d73c-87d6-4c49-9cf2-ae7654226d68")
    _ISynDisplayEvents
    {
    public:
        BEGIN_INTERFACE
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnSynDisplayMessage( 
            long lMessage) = 0;
        
        END_INTERFACE
    };
    
#else 	/* C style interface */

    typedef struct _ISynDisplayEventsVtbl
    {
        BEGIN_INTERFACE
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSynDisplayMessage )( 
            _ISynDisplayEvents __RPC_FAR * This,
            long lMessage);
        
        END_INTERFACE
    } _ISynDisplayEventsVtbl;

    interface _ISynDisplayEvents
    {
        CONST_VTBL struct _ISynDisplayEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISynDisplayEvents_OnSynDisplayMessage(This,lMessage)	\
    (This)->lpVtbl -> OnSynDisplayMessage(This,lMessage)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _ISynDisplayEvents_OnSynDisplayMessage_Proxy( 
    _ISynDisplayEvents __RPC_FAR * This,
    long lMessage);


void __RPC_STUB _ISynDisplayEvents_OnSynDisplayMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___ISynDisplayEvents_INTERFACE_DEFINED__ */


#ifndef __ISynAPI_INTERFACE_DEFINED__
#define __ISynAPI_INTERFACE_DEFINED__

/* interface ISynAPI */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID_ISynAPI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("41320763-F0EC-4B7F-9A2E-B4DA92C80FE7")
    ISynAPI : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindDevice( 
            long lConnectionType,
            long lDeviceType,
            long __RPC_FAR *ulHandle) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateDevice( 
            long lHandle,
            ISynDevice __RPC_FAR *__RPC_FAR *ppDevice) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            long lSpecifier,
            long lValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventNotification( 
            HANDLE hEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventParameter( 
            long __RPC_FAR *lParameter) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PersistState( 
            long lStateFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RestoreState( 
            long lStateFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE HardwareBroadcast( 
            long lAction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetSynchronousNotification( 
            _ISynAPIEvents __RPC_FAR *pCallbackInstance) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ForwardSystemMessage( 
            UINT uMsg,
            WPARAM wParam,
            LPARAM lParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynAPIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynAPI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynAPI __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynAPI __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            ISynAPI __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindDevice )( 
            ISynAPI __RPC_FAR * This,
            long lConnectionType,
            long lDeviceType,
            long __RPC_FAR *ulHandle);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDevice )( 
            ISynAPI __RPC_FAR * This,
            long lHandle,
            ISynDevice __RPC_FAR *__RPC_FAR *ppDevice);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ISynAPI __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynAPI __RPC_FAR * This,
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ISynAPI __RPC_FAR * This,
            long lSpecifier,
            long lValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEventNotification )( 
            ISynAPI __RPC_FAR * This,
            HANDLE hEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventParameter )( 
            ISynAPI __RPC_FAR * This,
            long __RPC_FAR *lParameter);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistState )( 
            ISynAPI __RPC_FAR * This,
            long lStateFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RestoreState )( 
            ISynAPI __RPC_FAR * This,
            long lStateFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HardwareBroadcast )( 
            ISynAPI __RPC_FAR * This,
            long lAction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSynchronousNotification )( 
            ISynAPI __RPC_FAR * This,
            _ISynAPIEvents __RPC_FAR *pCallbackInstance);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForwardSystemMessage )( 
            ISynAPI __RPC_FAR * This,
            UINT uMsg,
            WPARAM wParam,
            LPARAM lParam);
        
        END_INTERFACE
    } ISynAPIVtbl;

    interface ISynAPI
    {
        CONST_VTBL struct ISynAPIVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynAPI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynAPI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynAPI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynAPI_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define ISynAPI_FindDevice(This,lConnectionType,lDeviceType,ulHandle)	\
    (This)->lpVtbl -> FindDevice(This,lConnectionType,lDeviceType,ulHandle)

#define ISynAPI_CreateDevice(This,lHandle,ppDevice)	\
    (This)->lpVtbl -> CreateDevice(This,lHandle,ppDevice)

#define ISynAPI_GetProperty(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetProperty(This,lSpecifier,pValue)

#define ISynAPI_GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)	\
    (This)->lpVtbl -> GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)

#define ISynAPI_SetProperty(This,lSpecifier,lValue)	\
    (This)->lpVtbl -> SetProperty(This,lSpecifier,lValue)

#define ISynAPI_SetEventNotification(This,hEvent)	\
    (This)->lpVtbl -> SetEventNotification(This,hEvent)

#define ISynAPI_GetEventParameter(This,lParameter)	\
    (This)->lpVtbl -> GetEventParameter(This,lParameter)

#define ISynAPI_PersistState(This,lStateFlags)	\
    (This)->lpVtbl -> PersistState(This,lStateFlags)

#define ISynAPI_RestoreState(This,lStateFlags)	\
    (This)->lpVtbl -> RestoreState(This,lStateFlags)

#define ISynAPI_HardwareBroadcast(This,lAction)	\
    (This)->lpVtbl -> HardwareBroadcast(This,lAction)

#define ISynAPI_SetSynchronousNotification(This,pCallbackInstance)	\
    (This)->lpVtbl -> SetSynchronousNotification(This,pCallbackInstance)

#define ISynAPI_ForwardSystemMessage(This,uMsg,wParam,lParam)	\
    (This)->lpVtbl -> ForwardSystemMessage(This,uMsg,wParam,lParam)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_Initialize_Proxy( 
    ISynAPI __RPC_FAR * This);


void __RPC_STUB ISynAPI_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_FindDevice_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lConnectionType,
    long lDeviceType,
    long __RPC_FAR *ulHandle);


void __RPC_STUB ISynAPI_FindDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_CreateDevice_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lHandle,
    ISynDevice __RPC_FAR *__RPC_FAR *ppDevice);


void __RPC_STUB ISynAPI_CreateDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_GetProperty_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynAPI_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_GetStringProperty_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lSpecifier,
    TCHAR __RPC_FAR *pBuffer,
    long __RPC_FAR *ulBufLen);


void __RPC_STUB ISynAPI_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_SetProperty_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lSpecifier,
    long lValue);


void __RPC_STUB ISynAPI_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_SetEventNotification_Proxy( 
    ISynAPI __RPC_FAR * This,
    HANDLE hEvent);


void __RPC_STUB ISynAPI_SetEventNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_GetEventParameter_Proxy( 
    ISynAPI __RPC_FAR * This,
    long __RPC_FAR *lParameter);


void __RPC_STUB ISynAPI_GetEventParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_PersistState_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lStateFlags);


void __RPC_STUB ISynAPI_PersistState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_RestoreState_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lStateFlags);


void __RPC_STUB ISynAPI_RestoreState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_HardwareBroadcast_Proxy( 
    ISynAPI __RPC_FAR * This,
    long lAction);


void __RPC_STUB ISynAPI_HardwareBroadcast_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_SetSynchronousNotification_Proxy( 
    ISynAPI __RPC_FAR * This,
    _ISynAPIEvents __RPC_FAR *pCallbackInstance);


void __RPC_STUB ISynAPI_SetSynchronousNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynAPI_ForwardSystemMessage_Proxy( 
    ISynAPI __RPC_FAR * This,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam);


void __RPC_STUB ISynAPI_ForwardSystemMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynAPI_INTERFACE_DEFINED__ */


#ifndef __ISynDevice_INTERFACE_DEFINED__
#define __ISynDevice_INTERFACE_DEFINED__

/* interface ISynDevice */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID_ISynDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E7D5F8AC-866C-4C8C-AF5F-F28DE4918647")
    ISynDevice : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetBooleanProperty( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            long lSpecifier,
            long lValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventNotification( 
            HANDLE hEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreatePacket( 
            ISynPacket __RPC_FAR *__RPC_FAR *ppPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadPacket( 
            ISynPacket __RPC_FAR *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ForceMotion( 
            long lDeltaX,
            long lDeltaY,
            long lButtonState) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ForcePacket( 
            ISynPacket __RPC_FAR *pPacket) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Acquire( 
            long lFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Unacquire( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateDisplay( 
            ISynDisplay __RPC_FAR *__RPC_FAR *ppDisplay) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Select( 
            long lHandle) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PeekPacket( 
            long __RPC_FAR *plSequence) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetSynchronousNotification( 
            _ISynDeviceEvents __RPC_FAR *pCallbackInstance) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPropertyDefault( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE BulkTransaction( 
            ULONG ulWriteLength,
            UCHAR __RPC_FAR *ucWriteBuffer,
            ULONG ulReadLength,
            UCHAR __RPC_FAR *ucReadBuffer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DiagnosticTransaction( 
            ULONG ulWriteLength,
            UCHAR __RPC_FAR *ucWriteBuffer,
            ULONG ulReadLength,
            UCHAR __RPC_FAR *ucReadBuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynDevice __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynDevice __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynDevice __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ISynDevice __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBooleanProperty )( 
            ISynDevice __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynDevice __RPC_FAR * This,
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ISynDevice __RPC_FAR * This,
            long lSpecifier,
            long lValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEventNotification )( 
            ISynDevice __RPC_FAR * This,
            HANDLE hEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreatePacket )( 
            ISynDevice __RPC_FAR * This,
            ISynPacket __RPC_FAR *__RPC_FAR *ppPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadPacket )( 
            ISynDevice __RPC_FAR * This,
            ISynPacket __RPC_FAR *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForceMotion )( 
            ISynDevice __RPC_FAR * This,
            long lDeltaX,
            long lDeltaY,
            long lButtonState);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForcePacket )( 
            ISynDevice __RPC_FAR * This,
            ISynPacket __RPC_FAR *pPacket);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Acquire )( 
            ISynDevice __RPC_FAR * This,
            long lFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unacquire )( 
            ISynDevice __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDisplay )( 
            ISynDevice __RPC_FAR * This,
            ISynDisplay __RPC_FAR *__RPC_FAR *ppDisplay);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            ISynDevice __RPC_FAR * This,
            long lHandle);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PeekPacket )( 
            ISynDevice __RPC_FAR * This,
            long __RPC_FAR *plSequence);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSynchronousNotification )( 
            ISynDevice __RPC_FAR * This,
            _ISynDeviceEvents __RPC_FAR *pCallbackInstance);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPropertyDefault )( 
            ISynDevice __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BulkTransaction )( 
            ISynDevice __RPC_FAR * This,
            ULONG ulWriteLength,
            UCHAR __RPC_FAR *ucWriteBuffer,
            ULONG ulReadLength,
            UCHAR __RPC_FAR *ucReadBuffer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DiagnosticTransaction )( 
            ISynDevice __RPC_FAR * This,
            ULONG ulWriteLength,
            UCHAR __RPC_FAR *ucWriteBuffer,
            ULONG ulReadLength,
            UCHAR __RPC_FAR *ucReadBuffer);
        
        END_INTERFACE
    } ISynDeviceVtbl;

    interface ISynDevice
    {
        CONST_VTBL struct ISynDeviceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynDevice_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynDevice_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynDevice_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynDevice_GetProperty(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetProperty(This,lSpecifier,pValue)

#define ISynDevice_GetBooleanProperty(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetBooleanProperty(This,lSpecifier,pValue)

#define ISynDevice_GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)	\
    (This)->lpVtbl -> GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)

#define ISynDevice_SetProperty(This,lSpecifier,lValue)	\
    (This)->lpVtbl -> SetProperty(This,lSpecifier,lValue)

#define ISynDevice_SetEventNotification(This,hEvent)	\
    (This)->lpVtbl -> SetEventNotification(This,hEvent)

#define ISynDevice_CreatePacket(This,ppPacket)	\
    (This)->lpVtbl -> CreatePacket(This,ppPacket)

#define ISynDevice_LoadPacket(This,pPacket)	\
    (This)->lpVtbl -> LoadPacket(This,pPacket)

#define ISynDevice_ForceMotion(This,lDeltaX,lDeltaY,lButtonState)	\
    (This)->lpVtbl -> ForceMotion(This,lDeltaX,lDeltaY,lButtonState)

#define ISynDevice_ForcePacket(This,pPacket)	\
    (This)->lpVtbl -> ForcePacket(This,pPacket)

#define ISynDevice_Acquire(This,lFlags)	\
    (This)->lpVtbl -> Acquire(This,lFlags)

#define ISynDevice_Unacquire(This)	\
    (This)->lpVtbl -> Unacquire(This)

#define ISynDevice_CreateDisplay(This,ppDisplay)	\
    (This)->lpVtbl -> CreateDisplay(This,ppDisplay)

#define ISynDevice_Select(This,lHandle)	\
    (This)->lpVtbl -> Select(This,lHandle)

#define ISynDevice_PeekPacket(This,plSequence)	\
    (This)->lpVtbl -> PeekPacket(This,plSequence)

#define ISynDevice_SetSynchronousNotification(This,pCallbackInstance)	\
    (This)->lpVtbl -> SetSynchronousNotification(This,pCallbackInstance)

#define ISynDevice_GetPropertyDefault(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetPropertyDefault(This,lSpecifier,pValue)

#define ISynDevice_BulkTransaction(This,ulWriteLength,ucWriteBuffer,ulReadLength,ucReadBuffer)	\
    (This)->lpVtbl -> BulkTransaction(This,ulWriteLength,ucWriteBuffer,ulReadLength,ucReadBuffer)

#define ISynDevice_DiagnosticTransaction(This,ulWriteLength,ucWriteBuffer,ulReadLength,ucReadBuffer)	\
    (This)->lpVtbl -> DiagnosticTransaction(This,ulWriteLength,ucWriteBuffer,ulReadLength,ucReadBuffer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_GetProperty_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynDevice_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_GetBooleanProperty_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynDevice_GetBooleanProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_GetStringProperty_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lSpecifier,
    TCHAR __RPC_FAR *pBuffer,
    long __RPC_FAR *ulBufLen);


void __RPC_STUB ISynDevice_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_SetProperty_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lSpecifier,
    long lValue);


void __RPC_STUB ISynDevice_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_SetEventNotification_Proxy( 
    ISynDevice __RPC_FAR * This,
    HANDLE hEvent);


void __RPC_STUB ISynDevice_SetEventNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_CreatePacket_Proxy( 
    ISynDevice __RPC_FAR * This,
    ISynPacket __RPC_FAR *__RPC_FAR *ppPacket);


void __RPC_STUB ISynDevice_CreatePacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_LoadPacket_Proxy( 
    ISynDevice __RPC_FAR * This,
    ISynPacket __RPC_FAR *pPacket);


void __RPC_STUB ISynDevice_LoadPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_ForceMotion_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lDeltaX,
    long lDeltaY,
    long lButtonState);


void __RPC_STUB ISynDevice_ForceMotion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_ForcePacket_Proxy( 
    ISynDevice __RPC_FAR * This,
    ISynPacket __RPC_FAR *pPacket);


void __RPC_STUB ISynDevice_ForcePacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_Acquire_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lFlags);


void __RPC_STUB ISynDevice_Acquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_Unacquire_Proxy( 
    ISynDevice __RPC_FAR * This);


void __RPC_STUB ISynDevice_Unacquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_CreateDisplay_Proxy( 
    ISynDevice __RPC_FAR * This,
    ISynDisplay __RPC_FAR *__RPC_FAR *ppDisplay);


void __RPC_STUB ISynDevice_CreateDisplay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_Select_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lHandle);


void __RPC_STUB ISynDevice_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_PeekPacket_Proxy( 
    ISynDevice __RPC_FAR * This,
    long __RPC_FAR *plSequence);


void __RPC_STUB ISynDevice_PeekPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_SetSynchronousNotification_Proxy( 
    ISynDevice __RPC_FAR * This,
    _ISynDeviceEvents __RPC_FAR *pCallbackInstance);


void __RPC_STUB ISynDevice_SetSynchronousNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_GetPropertyDefault_Proxy( 
    ISynDevice __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynDevice_GetPropertyDefault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_BulkTransaction_Proxy( 
    ISynDevice __RPC_FAR * This,
    ULONG ulWriteLength,
    UCHAR __RPC_FAR *ucWriteBuffer,
    ULONG ulReadLength,
    UCHAR __RPC_FAR *ucReadBuffer);


void __RPC_STUB ISynDevice_BulkTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDevice_DiagnosticTransaction_Proxy( 
    ISynDevice __RPC_FAR * This,
    ULONG ulWriteLength,
    UCHAR __RPC_FAR *ucWriteBuffer,
    ULONG ulReadLength,
    UCHAR __RPC_FAR *ucReadBuffer);


void __RPC_STUB ISynDevice_DiagnosticTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynDevice_INTERFACE_DEFINED__ */


#ifndef __ISynPacket_INTERFACE_DEFINED__
#define __ISynPacket_INTERFACE_DEFINED__

/* interface ISynPacket */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID_ISynPacket;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BF9D398B-F631-44B4-8EC0-D3FB3E388B62")
    ISynPacket : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            long lSpecifier,
            long lValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Copy( 
            ISynPacket __RPC_FAR *pFrom) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynPacketVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynPacket __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynPacket __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynPacket __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ISynPacket __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ISynPacket __RPC_FAR * This,
            long lSpecifier,
            long lValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynPacket __RPC_FAR * This,
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )( 
            ISynPacket __RPC_FAR * This,
            ISynPacket __RPC_FAR *pFrom);
        
        END_INTERFACE
    } ISynPacketVtbl;

    interface ISynPacket
    {
        CONST_VTBL struct ISynPacketVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynPacket_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynPacket_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynPacket_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynPacket_GetProperty(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetProperty(This,lSpecifier,pValue)

#define ISynPacket_SetProperty(This,lSpecifier,lValue)	\
    (This)->lpVtbl -> SetProperty(This,lSpecifier,lValue)

#define ISynPacket_GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)	\
    (This)->lpVtbl -> GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)

#define ISynPacket_Copy(This,pFrom)	\
    (This)->lpVtbl -> Copy(This,pFrom)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynPacket_GetProperty_Proxy( 
    ISynPacket __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynPacket_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynPacket_SetProperty_Proxy( 
    ISynPacket __RPC_FAR * This,
    long lSpecifier,
    long lValue);


void __RPC_STUB ISynPacket_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynPacket_GetStringProperty_Proxy( 
    ISynPacket __RPC_FAR * This,
    long lSpecifier,
    TCHAR __RPC_FAR *pBuffer,
    long __RPC_FAR *ulBufLen);


void __RPC_STUB ISynPacket_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynPacket_Copy_Proxy( 
    ISynPacket __RPC_FAR * This,
    ISynPacket __RPC_FAR *pFrom);


void __RPC_STUB ISynPacket_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynPacket_INTERFACE_DEFINED__ */


#ifndef __ISynDisplay_INTERFACE_DEFINED__
#define __ISynDisplay_INTERFACE_DEFINED__

/* interface ISynDisplay */
/* [helpstring][uuid][object][local] */ 


EXTERN_C const IID IID_ISynDisplay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A398ED6B-A2CC-471D-96F7-959610870AE0")
    ISynDisplay : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetProperty( 
            long lSpecifier,
            long __RPC_FAR *pValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            long lSpecifier,
            long lValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PixelToTouch( 
            long PixelX,
            long PixelY,
            long __RPC_FAR *pTouchX,
            long __RPC_FAR *pTouchY) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TouchToPixel( 
            long TouchX,
            long TouchY,
            long __RPC_FAR *pPixelX,
            long __RPC_FAR *pPixelY) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDC( 
            HDC __RPC_FAR *pHDC) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FlushDC( 
            long lFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Acquire( 
            long lDisplayAcquisitionMethod) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Unacquire( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Select( 
            long lDeviceHandle) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEventNotification( 
            HANDLE hEvent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEventParameter( 
            long __RPC_FAR *lParameter) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetSynchronousNotification( 
            _ISynDisplayEvents __RPC_FAR *pCallbackInstance) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStringProperty( 
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetBackgroundImage( 
            HBITMAP hBmp) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CloneBackgroundImage( 
            HBITMAP __RPC_FAR *pHBmp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISynDisplayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISynDisplay __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISynDisplay __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISynDisplay __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ISynDisplay __RPC_FAR * This,
            long lSpecifier,
            long __RPC_FAR *pValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ISynDisplay __RPC_FAR * This,
            long lSpecifier,
            long lValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PixelToTouch )( 
            ISynDisplay __RPC_FAR * This,
            long PixelX,
            long PixelY,
            long __RPC_FAR *pTouchX,
            long __RPC_FAR *pTouchY);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TouchToPixel )( 
            ISynDisplay __RPC_FAR * This,
            long TouchX,
            long TouchY,
            long __RPC_FAR *pPixelX,
            long __RPC_FAR *pPixelY);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDC )( 
            ISynDisplay __RPC_FAR * This,
            HDC __RPC_FAR *pHDC);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FlushDC )( 
            ISynDisplay __RPC_FAR * This,
            long lFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Acquire )( 
            ISynDisplay __RPC_FAR * This,
            long lDisplayAcquisitionMethod);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unacquire )( 
            ISynDisplay __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            ISynDisplay __RPC_FAR * This,
            long lDeviceHandle);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEventNotification )( 
            ISynDisplay __RPC_FAR * This,
            HANDLE hEvent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventParameter )( 
            ISynDisplay __RPC_FAR * This,
            long __RPC_FAR *lParameter);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSynchronousNotification )( 
            ISynDisplay __RPC_FAR * This,
            _ISynDisplayEvents __RPC_FAR *pCallbackInstance);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStringProperty )( 
            ISynDisplay __RPC_FAR * This,
            long lSpecifier,
            TCHAR __RPC_FAR *pBuffer,
            long __RPC_FAR *ulBufLen);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBackgroundImage )( 
            ISynDisplay __RPC_FAR * This,
            HBITMAP hBmp);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloneBackgroundImage )( 
            ISynDisplay __RPC_FAR * This,
            HBITMAP __RPC_FAR *pHBmp);
        
        END_INTERFACE
    } ISynDisplayVtbl;

    interface ISynDisplay
    {
        CONST_VTBL struct ISynDisplayVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISynDisplay_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISynDisplay_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISynDisplay_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISynDisplay_GetProperty(This,lSpecifier,pValue)	\
    (This)->lpVtbl -> GetProperty(This,lSpecifier,pValue)

#define ISynDisplay_SetProperty(This,lSpecifier,lValue)	\
    (This)->lpVtbl -> SetProperty(This,lSpecifier,lValue)

#define ISynDisplay_PixelToTouch(This,PixelX,PixelY,pTouchX,pTouchY)	\
    (This)->lpVtbl -> PixelToTouch(This,PixelX,PixelY,pTouchX,pTouchY)

#define ISynDisplay_TouchToPixel(This,TouchX,TouchY,pPixelX,pPixelY)	\
    (This)->lpVtbl -> TouchToPixel(This,TouchX,TouchY,pPixelX,pPixelY)

#define ISynDisplay_GetDC(This,pHDC)	\
    (This)->lpVtbl -> GetDC(This,pHDC)

#define ISynDisplay_FlushDC(This,lFlags)	\
    (This)->lpVtbl -> FlushDC(This,lFlags)

#define ISynDisplay_Acquire(This,lDisplayAcquisitionMethod)	\
    (This)->lpVtbl -> Acquire(This,lDisplayAcquisitionMethod)

#define ISynDisplay_Unacquire(This)	\
    (This)->lpVtbl -> Unacquire(This)

#define ISynDisplay_Select(This,lDeviceHandle)	\
    (This)->lpVtbl -> Select(This,lDeviceHandle)

#define ISynDisplay_SetEventNotification(This,hEvent)	\
    (This)->lpVtbl -> SetEventNotification(This,hEvent)

#define ISynDisplay_GetEventParameter(This,lParameter)	\
    (This)->lpVtbl -> GetEventParameter(This,lParameter)

#define ISynDisplay_SetSynchronousNotification(This,pCallbackInstance)	\
    (This)->lpVtbl -> SetSynchronousNotification(This,pCallbackInstance)

#define ISynDisplay_GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)	\
    (This)->lpVtbl -> GetStringProperty(This,lSpecifier,pBuffer,ulBufLen)

#define ISynDisplay_SetBackgroundImage(This,hBmp)	\
    (This)->lpVtbl -> SetBackgroundImage(This,hBmp)

#define ISynDisplay_CloneBackgroundImage(This,pHBmp)	\
    (This)->lpVtbl -> CloneBackgroundImage(This,pHBmp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_GetProperty_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lSpecifier,
    long __RPC_FAR *pValue);


void __RPC_STUB ISynDisplay_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_SetProperty_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lSpecifier,
    long lValue);


void __RPC_STUB ISynDisplay_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_PixelToTouch_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long PixelX,
    long PixelY,
    long __RPC_FAR *pTouchX,
    long __RPC_FAR *pTouchY);


void __RPC_STUB ISynDisplay_PixelToTouch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_TouchToPixel_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long TouchX,
    long TouchY,
    long __RPC_FAR *pPixelX,
    long __RPC_FAR *pPixelY);


void __RPC_STUB ISynDisplay_TouchToPixel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_GetDC_Proxy( 
    ISynDisplay __RPC_FAR * This,
    HDC __RPC_FAR *pHDC);


void __RPC_STUB ISynDisplay_GetDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_FlushDC_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lFlags);


void __RPC_STUB ISynDisplay_FlushDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_Acquire_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lDisplayAcquisitionMethod);


void __RPC_STUB ISynDisplay_Acquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_Unacquire_Proxy( 
    ISynDisplay __RPC_FAR * This);


void __RPC_STUB ISynDisplay_Unacquire_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_Select_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lDeviceHandle);


void __RPC_STUB ISynDisplay_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_SetEventNotification_Proxy( 
    ISynDisplay __RPC_FAR * This,
    HANDLE hEvent);


void __RPC_STUB ISynDisplay_SetEventNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_GetEventParameter_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long __RPC_FAR *lParameter);


void __RPC_STUB ISynDisplay_GetEventParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_SetSynchronousNotification_Proxy( 
    ISynDisplay __RPC_FAR * This,
    _ISynDisplayEvents __RPC_FAR *pCallbackInstance);


void __RPC_STUB ISynDisplay_SetSynchronousNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_GetStringProperty_Proxy( 
    ISynDisplay __RPC_FAR * This,
    long lSpecifier,
    TCHAR __RPC_FAR *pBuffer,
    long __RPC_FAR *ulBufLen);


void __RPC_STUB ISynDisplay_GetStringProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_SetBackgroundImage_Proxy( 
    ISynDisplay __RPC_FAR * This,
    HBITMAP hBmp);


void __RPC_STUB ISynDisplay_SetBackgroundImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISynDisplay_CloneBackgroundImage_Proxy( 
    ISynDisplay __RPC_FAR * This,
    HBITMAP __RPC_FAR *pHBmp);


void __RPC_STUB ISynDisplay_CloneBackgroundImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISynDisplay_INTERFACE_DEFINED__ */



#ifndef __SYNCOMLib_LIBRARY_DEFINED__
#define __SYNCOMLib_LIBRARY_DEFINED__

/* library SYNCOMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SYNCOMLib;

EXTERN_C const CLSID CLSID_SynAPI;

#ifdef __cplusplus

class DECLSPEC_UUID("9C042297-D1CD-4F0D-B1AB-9F48AD6A6DFF")
SynAPI;
#endif

EXTERN_C const CLSID CLSID_SynDevice;

#ifdef __cplusplus

class DECLSPEC_UUID("9345312C-D098-4BB1-B2B2-D529EB995173")
SynDevice;
#endif

EXTERN_C const CLSID CLSID_SynPacket;

#ifdef __cplusplus

class DECLSPEC_UUID("E0C6335D-27F8-424B-A5C2-561291A902A0")
SynPacket;
#endif

EXTERN_C const CLSID CLSID_SynDisplay;

#ifdef __cplusplus

class DECLSPEC_UUID("248AFB1A-27C4-4A30-BF45-6544146648BC")
SynDisplay;
#endif
#endif /* __SYNCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
