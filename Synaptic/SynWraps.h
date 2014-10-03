// Public header file for access to the Synaptics COM API/Toolkit.

#ifndef __SYNWRAPS_H
#define __SYNWRAPS_H

#include <comdef.h>

template <class Object, class Interface> class SynWrap
{
public:
  SynWrap()
  {
    m_pInternal = 0; // Fault reliably if a SynWrap can't be created.

    DoThreadCoInitialize();
    CoCreateInstance(_uuidof(Object), 0, CLSCTX_SERVER,
      _uuidof(Interface), (void **) &m_pInternal);
  }

  SynWrap(LPCOLESTR psProgID)
  {
    CLSID clsid;
    m_pInternal = 0; // Fault reliably if a SynWrap can't be created.

    DoThreadCoInitialize();
    CLSIDFromProgID( psProgID, &clsid );
    CoCreateInstance(clsid, 0, CLSCTX_SERVER,
      _uuidof(Interface), (void **) &m_pInternal);
  }

  SynWrap(Interface *pDevice)
  {
    m_pInternal = pDevice;
    if (m_pInternal)
      pDevice->AddRef();
  }

  ~SynWrap()
  {
    if (m_pInternal)
      m_pInternal->Release();
  }

  operator Interface *() { return m_pInternal; }

  Interface *operator->() { return m_pInternal; }

  long GetLongProperty(long lSpecifier) const
  {
    long lProperty = 0;

    if (m_pInternal)
      m_pInternal->GetProperty(lSpecifier, &lProperty);
    return lProperty;
  }

  _bstr_t GetStringProperty (long lSpecifier)
  {
    char buf[256];
    long ulSize = sizeof(buf);
    buf[0] = 0;

    if (m_pInternal)
      m_pInternal->GetStringProperty(lSpecifier, buf, &ulSize);
    return _bstr_t(buf);
  }

  Interface *m_pInternal;

private:
  void DoThreadCoInitialize()
  {
    static int s_dwTlsIndex = TlsAlloc();
    if (!TlsGetValue(s_dwTlsIndex)) { // These are initted to NULL for each thread.
      CoInitialize(0);
      TlsSetValue(s_dwTlsIndex, (PVOID)1);
    }
  }
};

template <class Object, class Interface, class Events> class SynWrapEvents :
  public SynWrap<Object, Interface>,
  public Events
{
public:
  SynWrapEvents() { }
  SynWrapEvents(LPCOLESTR psProgID) : SynWrap<Object, Interface>(psProgID){ }
  SynWrapEvents(Interface *pDevice) : SynWrap<Object, Interface>(pDevice) { }
  ~SynWrapEvents() { }

  HRESULT Activate()
  {
    HRESULT res = SYNE_FAIL;
    if (m_pInternal)
      res = m_pInternal->SetSynchronousNotification(this);

    return res;
  }

  HRESULT Deactivate()
  {
    HRESULT res = SYNE_FAIL;
    if (m_pInternal)
      res = m_pInternal->SetSynchronousNotification(0);

    return res;
  }

  HRESULT STDMETHODCALLTYPE OnSynAPINotify(long /* lReason */) { return 0; }
  HRESULT STDMETHODCALLTYPE OnSynDevicePacket(long /* lSequence */) { return 0; }
  HRESULT STDMETHODCALLTYPE OnSynDisplayMessage(long /* lMessage */) { return 0; }
};

class SynAPI :
  public SynWrapEvents<SynAPI, ISynAPI, _ISynAPIEvents>
{
public:
  SynAPI() :
    SynWrapEvents<SynAPI, ISynAPI, _ISynAPIEvents>(L"SynCom.SynAPI") { }

  SynAPI(ISynAPI *pInterface) : 
    SynWrapEvents<SynAPI, ISynAPI, _ISynAPIEvents>(pInterface) { }

  HRESULT STDMETHODCALLTYPE OnSynAPINotify(long /* lReason */) { return 0; }


  long FindDevice(enum SynConnectionType eConnectionType, 
    enum SynDeviceType eDeviceType, long lStart)
  {
    if (m_pInternal)
    {
      return m_pInternal->FindDevice(eConnectionType, eDeviceType, &lStart) 
        ? -1 : lStart;
    }
    else
      return -1;
  }
};

class SynDevice :
  public SynWrapEvents<SynDevice, ISynDevice, _ISynDeviceEvents>
{
public:
  SynDevice() :
    SynWrapEvents<SynDevice, ISynDevice, _ISynDeviceEvents>(L"SynCom.PointingDevice") { }

  SynDevice(ISynDevice *pInterface) : 
    SynWrapEvents<SynDevice, ISynDevice, _ISynDeviceEvents>(pInterface) { }

  long GetLongPropertyDefault(long lSpecifier)
  {
    long lProperty = 0;

    if (m_pInternal)
      m_pInternal->GetPropertyDefault(lSpecifier, &lProperty);
    return lProperty;
  }

  HRESULT STDMETHODCALLTYPE OnSynDevicePacket(long /* lSequence */) { return 0; }

  bool IsTouchPad() {
    return (SE_DeviceTouchPad == GetLongProperty(SP_DeviceType));
  }

  bool IsStyk() {
    return (SE_DeviceStyk == GetLongProperty(SP_DeviceType));
  }

  bool IsIBMCompatibleStick() {
    return (SE_DeviceIBMCompatibleStick == GetLongProperty(SP_DeviceType));
  }

  bool IsStykOrIBMCompatibleStick() {
    return (IsStyk() || IsIBMCompatibleStick());
  }

  bool IscPad() {
    return IsTouchPad() &&
      !!(GetLongProperty(SP_DisplayFlags) & SF_DisplayPresent);
  }

  bool IsStandardMouse() {
    return (SE_DeviceMouse == GetLongProperty(SP_DeviceType));
  }

  bool IsWheelMouse() {
    return (SE_DeviceWheelMouse == GetLongProperty(SP_DeviceType));
  }
  
  bool IsFiveButtonWheelMouse() {
    return (SE_DeviceFiveButtonWheelMouse == GetLongProperty(SP_DeviceType));
  }

  bool IsMouse() {
    return (IsStandardMouse() || IsWheelMouse() || IsFiveButtonWheelMouse());
  }

  bool IsUSBDevice() {
    return (SE_ConnectionUSB == GetLongProperty(SP_ConnectionType));
  }

  bool IsPS2Device() {
    return (SE_ConnectionPS2 == GetLongProperty(SP_ConnectionType));
  }
};

class SynDisplay :
  public SynWrapEvents<SynDisplay, ISynDisplay, _ISynDisplayEvents>
{
public:
  SynDisplay() :
    SynWrapEvents<SynDisplay, ISynDisplay, _ISynDisplayEvents>(L"SynCom.SynDisplay") { }

  SynDisplay(ISynDisplay *pInterface) : 
    SynWrapEvents<SynDisplay, ISynDisplay, _ISynDisplayEvents>(pInterface) { }

  HRESULT STDMETHODCALLTYPE OnSynDisplayMessage(long /* lMessage */) { return 0; }

  long PixelX(long lTouchX)
  {
    long lPixelX = 0, lPixelY = 0;
    if (m_pInternal)
    {
      m_pInternal->TouchToPixel(lTouchX, 0, &lPixelX, &lPixelY);
    }

    return lPixelX;
  }

  long PixelY(long lTouchY)
  {
    long lPixelX = 0, lPixelY = 0;
    if (m_pInternal)
    {
      m_pInternal->TouchToPixel(0, lTouchY, &lPixelX, &lPixelY);
    }

    return lPixelY;
  }

  long TouchX(long lPixelX)
  {
    long lTouchX = 0, lTouchY = 0;
    if (m_pInternal)
    {
      m_pInternal->PixelToTouch(lPixelX, 0, &lTouchX, &lTouchY);
    }

    return lTouchX;
  }

  long TouchY(long lPixelY)
  {
    long lTouchX = 0, lTouchY = 0;
    if (m_pInternal)
    {
      m_pInternal->PixelToTouch(0, lPixelY, &lTouchX, &lTouchY);
    }

    return lTouchY;
  }
};

class SynPacket :
  public SynWrap<SynPacket, ISynPacket>
{
public:
  SynPacket() : SynWrap<SynPacket, ISynPacket>(L"SynCom.SynPacket") { }

  SynPacket(ISynPacket *pPacket) : SynWrap<SynPacket, ISynPacket>(pPacket) { }

  long AssociatedDeviceHandle() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_AssociatedDeviceHandle, &lProperty);
    return lProperty;
  }

  long SequenceNumber() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_SequenceNumber, &lProperty);
    return lProperty;
  }

  unsigned long TimeStamp() const
  {
    unsigned long ulProperty;
    m_pInternal->GetProperty(SP_TimeStamp, (long *) &ulProperty);
    return ulProperty;
  }

  long W() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_W, &lProperty);
    return lProperty;
  }

  long X() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_X, &lProperty);
    return lProperty;
  }

  long Y() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_Y, &lProperty);
    return lProperty;
  }

  long Z() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_Z, &lProperty);
    return lProperty;
  }

  long XRaw() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_XRaw, &lProperty);
    return lProperty;
  }

  long ZXRaw() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ZXRaw, &lProperty);
    return lProperty;
  }

  long YRaw() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_YRaw, &lProperty);
    return lProperty;
  }

  long ZRaw() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ZRaw, &lProperty);
    return lProperty;
  }

  long ZYRaw() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ZYRaw, &lProperty);
    return lProperty;
  }

  long XDelta() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_XDelta, &lProperty);
    return lProperty;
  }

  long YDelta() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_YDelta, &lProperty);
    return lProperty;
  }

  long ZDelta() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ZDelta, &lProperty);
    return lProperty;
  }

  long XMickeys() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_XMickeys, &lProperty);
    return lProperty;
  }

  long YMickeys() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_YMickeys, &lProperty);
    return lProperty;
  }

  long ExtendedState() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ExtendedState, &lProperty);
    return lProperty;
  }

  long FingerState() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_FingerState, &lProperty);
    return lProperty;
  }

  long ButtonState() const
  {
    long lProperty;
    m_pInternal->GetProperty(SP_ButtonState, &lProperty);
    return lProperty;
  }

  bool IsLeftButton() const { 
    return (ButtonState() & SF_ButtonLeft) != 0; 
  }

  bool IsRightButton() const { 
    return (ButtonState() & SF_ButtonRight) != 0; 
  }

  bool IsMiddleButton() const { 
    return (ButtonState() & SF_ButtonMiddle) != 0; 
  }

  bool IsTap() const {
    return (FingerState() & SF_FingerTap) != 0;
  }

  bool IsMotion() const {
    return (FingerState() & SF_FingerMotion) != 0;
  }

  bool IsFinger() const {
    return (FingerState() & SF_FingerPresent) != 0;
  }

  bool IsScrollFinger() const {
    return (ExtendedState() & SF_StateScrollFinger) != 0;
  }

  bool IsAnyFinger() const {
    return (IsFinger() || IsScrollFinger());
  }

  bool IsAnyButton() const {
    return (!!ButtonState() || !!(FingerState() & SF_FingerAsButton));
  }
};

#endif