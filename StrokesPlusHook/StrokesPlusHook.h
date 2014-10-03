
#include "options.h"


#ifdef SP_DEBUG_MODE
    #define LOGMESSAGE( str ) OutputDebugString( str );
#else
    #define LOGMESSAGE( str )
#endif

#ifndef _DEFINED_44E531B1_14D3_11d5_A025_006067718D04
#define _DEFINED_44E531B1_14D3_11d5_A025_006067718D04
#if _MSC_VER > 1000
#pragma once
#endif

#ifdef SP_SYNAPTIC
	#include "..\Synaptic\SynKit.h"
#endif
#include "Resource.h"
#include <boost/property_tree/ptree.hpp>
#ifdef __cplusplus
extern "C" {
#endif 

#define LIBSPEC __declspec(dllexport)

#ifdef SP_TOUCH
	LIBSPEC void processInputMessage(UINT message, WPARAM wParam, LPARAM lParam);
	LIBSPEC void InitTouchWindow(HWND hWnd);
	LIBSPEC void DestroyTouchWindow(HWND hWnd);
	LIBSPEC bool isTouchEnabled();
	LIBSPEC bool isPenEnabled();
	LIBSPEC bool isTouchAvailable();
#endif

#ifdef SP_NOGLOBALACTIONS
	LIBSPEC bool getOnlyDefinedApps(); 
#endif

LIBSPEC int getResumeDelay(); 

#ifdef SP_SYNAPTIC
	LIBSPEC void DestroySynaptics(); 
#endif

LIBSPEC void enableHotkey(int hkid);
LIBSPEC void disableHotkey(int hkid);
LIBSPEC void popupMenu();
LIBSPEC void initLua();
LIBSPEC void paintDisplayText(HWND hDlg, bool bReset);
LIBSPEC BOOL getCheckForOtherGesturePrograms();
LIBSPEC void setWindowTransparency();
LIBSPEC void setDPIModifier(float val);
LIBSPEC void setTrayID(int val);
LIBSPEC bool getHideAdditionalInstanceMessage();
LIBSPEC bool getTrayIconVisible();
LIBSPEC void setTrayIconVisible(bool val);
LIBSPEC bool getGesturesDisabled();
LIBSPEC void setGesturesDisabled(bool val);
LIBSPEC bool getReInitOnResume();
LIBSPEC bool getShowCopyData();
LIBSPEC void openPassword();
LIBSPEC void openSettings();
LIBSPEC void openPrefs();
LIBSPEC void openHotkeys();
LIBSPEC void openConfig();
LIBSPEC void openAbout();
LIBSPEC void openIgnored();
LIBSPEC void openHelp();
LIBSPEC void setPreviousTrainingModeState(bool val);
LIBSPEC void sethWndServer(HWND val);
LIBSPEC bool getCheckForegroundTimeout();
LIBSPEC void saveConfig();
LIBSPEC void setLastActive();
LIBSPEC void openGestureName();
LIBSPEC boost::property_tree::ptree ptConfig;	
LIBSPEC boost::property_tree::ptree ptLang;	
LIBSPEC void ClipOrCenterWindowToMonitor(HWND hwnd, BOOL bUseWorkArea, BOOL bClip);
LIBSPEC void FitWindowToMonitor(HWND hWndTarget, BOOL bUseWorkArea);
LIBSPEC void FitWindowToDesktop(HWND hWndTarget);
LIBSPEC void clearCaptureVars();
LIBSPEC BOOL saveGesture(TCHAR *string);
LIBSPEC BOOL drawLastGestureToHandle(HWND hWndForm, int left, int top, int iLeftOffset, int iTopOffset, BOOL bUIPoints);
LIBSPEC BOOL setMyHook(HWND hWnd);
LIBSPEC BOOL fireTrimTimer();
LIBSPEC BOOL clearMyHook(HWND hWnd);
LIBSPEC BOOL getLearningMode();
LIBSPEC BOOL setLearningMode(BOOL val);
LIBSPEC BOOL setMatchProbabilityThreshold(int val);
LIBSPEC int getMatchProbabilityThreshold();
LIBSPEC BOOL setMatchPrecision(int val);
LIBSPEC int getMatchPrecision();
LIBSPEC BOOL setHookBtn(int val);
LIBSPEC int getHookBtn();
LIBSPEC BOOL setAggressiveTrim(BOOL val);
LIBSPEC BOOL getAggressiveTrim();
LIBSPEC BOOL setMaxDrawPoints(int val);
LIBSPEC int getMaxDrawPoints();
LIBSPEC BOOL setStreamerDelay(int val);
LIBSPEC int getStreamerDelay();
LIBSPEC BOOL setIgnoreKey(int key);
LIBSPEC int getIgnoreKey();
LIBSPEC BOOL setDrawGesture(BOOL val);
LIBSPEC BOOL getDrawGesture();
LIBSPEC BOOL setCancel(BOOL val);
LIBSPEC BOOL getCancel();
LIBSPEC BOOL setDrawColor(COLORREF rgb);
LIBSPEC int getDrawColorR();
LIBSPEC int getDrawColorG();
LIBSPEC int getDrawColorB();
LIBSPEC BOOL setCancelDelay(int delay);
LIBSPEC int getCancelDelay();
LIBSPEC BOOL setTrimDelay(int delay);
LIBSPEC int getTrimDelay();
LIBSPEC BOOL setDrawWidth(int width);
LIBSPEC int getDrawWidth();
LIBSPEC BOOL setOSVersion();
LIBSPEC BOOL setWindowState();
LIBSPEC BOOL loadHookConfig();
LIBSPEC BOOL reloadLuaState();
LIBSPEC void FireHotkey(int iHotkeyID);
LIBSPEC void ClearHotkeys();
LIBSPEC void LoadHotkeys();

BOOL SetModifierStates(HWND hDlg);
void setPrefStrokeButton();
void setSelectedTab(BOOL bRefresh);
void clearCaptureVars();
int acCenterWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea);
int acFitWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea);
int acMaximizeToAllScreens(HWND hWndTarget, POINT start);
int acClipWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea);
int acSendControlDown();
int acSendControlUp();
int acSendAltDown();
int acSendAltUp();
int acSendShiftDown();
int acSendShiftUp();
int acSendWinDown();
int acSendWinUp();
int acGetMonitorFromPoint(POINT start);
int acPreviousApplication();
int acGetDesktopWindow();
int acGetForegroundWindow();
int acNextApplication();
int acDelay(DWORD delay);
int acDisable();
int acDisableNext();
int acDrawLastGesture(BOOL bSetTimer);
int acCloseApplication(HWND hWndTarget, POINT start);
int acMaximizeWindow(HWND hWndTarget, POINT start);
int acMaximizeOrRestoreWindow(HWND hWndTarget, POINT start);
int acMoveWindow(HWND hWndTarget, POINT start, int left, int top);
int acMinimizeWindow(HWND hWndTarget, POINT start);
int acSendKeys(LPCTSTR keystring, BOOL bWait); //REGRESS_SYN
int acRunProgram(LPCTSTR inFullPathToExe, LPCTSTR inParameters, int secs, LONG style);
int acSetVolume(float nVolumee, bool bIncrement);
int acGetVolume();
int acToggleMute();
int acGetMute();
int acSetMute(BOOL bMute);
int acFindWindow(LPCTSTR Class, LPCTSTR Title);
int acMessageBox(LPCTSTR sMsg, LPCTSTR sTitle, UINT uType);
int acFindWindowByTitleRegex(TCHAR const * windowTitle);
int acSetWindowSize(HWND hWndTarget, POINT start, int width, int height);
int acSendWindowToBottom(HWND hWndTarget, POINT start);
int acActivateWindow(HWND hWndTarget, POINT start, int iMethod);
int acRestoreWindow(HWND hWndTarget, POINT start);
int acGetWindowByPoint(POINT start);
int acGetParentWindowByPoint(POINT start);
int acGetOwnerWindowByPoint(POINT start);
int acSetProcessPriority(HWND hWndTarget, POINT start, DWORD PriorityClass);
int acTerminateProcess(HWND hWndTarget, POINT start);
int acToggleTopmost(HWND hWndTarget, POINT start);
int acSetTopmost(HWND hWndTarget, POINT start);
int acClearTopmost(HWND hWndTarget, POINT start);
int acPauseResumeThreadList(HWND hWndTarget, POINT start, bool bResumeThread) ;
int acGetSystemMetrics(int ival);
int acSetWindowTransparency(HWND hWndTarget, POINT start, int opacity);
int acGetWindowTransparency(HWND hWndTarget, POINT start);
int acSetWindowColorKey(HWND hWndTarget, POINT start, BOOL bRemove, BYTE red, BYTE green, BYTE blue);
int acGetWindowColorKeyR(HWND hWndTarget, POINT start);
int acGetWindowColorKeyG(HWND hWndTarget, POINT start);
int acGetWindowColorKeyB(HWND hWndTarget, POINT start);
int acGetPixelRByPoint(POINT start);
int acGetPixelGByPoint(POINT start);
int acGetPixelBByPoint(POINT start);
int acSetWindowTransparencyAndColorKey(HWND hWndTarget, POINT start, int opacity, BYTE red, BYTE green, BYTE blue);
int acShowActions();
int acShowHotkeys();
int acShowPrefs();
int acShowIgnoreList();
int acGetWindowBottom(HWND hWndTarget, POINT start);
int acGetWindowTop(HWND hWndTarget, POINT start);
int acGetWindowLeft(HWND hWndTarget, POINT start);
int acGetWindowRight(HWND hWndTarget, POINT start);
int acExit();
int acReloadConfig();
int acMouseClick(POINT end, int button, BOOL bDown, BOOL bUp);
int acMouseMove(POINT end);
int acGetMouseLocationX();
int acGetMouseLocationY();
int acSendMessage(HWND hWndTarget, UINT MessageID, WPARAM wParam, LPARAM lParam);
int acPostMessage(HWND hWndTarget, UINT MessageID, WPARAM wParam, LPARAM lParam);
int acSendWindowToPreviousMonitor(HWND hWndTarget, POINT start);
int acSendWindowToNextMonitor(HWND hWndTarget, POINT start);
int acSendWindowToMonitorByHandle(HWND hWndTarget, POINT start, HMONITOR hMonitor);
int acSendWindowToMonitorByName(HWND hWndTarget, POINT start);
int acSendWindowToOtherMonitor(HWND hWndTarget, LONG flag);
int acGetMonitorFromName();
int acGetMonitorRight(HMONITOR hMonitor, BOOL bUseWorkArea);
int acGetMonitorLeft(HMONITOR hMonitor, BOOL bUseWorkArea);
int acGetMonitorTop(HMONITOR hMonitor, BOOL bUseWorkArea);
int acGetMonitorBottom(HMONITOR hMonitor, BOOL bUseWorkArea);

static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
static BOOL CALLBACK FindWindowImpl( HWND hWnd, LPARAM lParam );
static void CALLBACK TrimTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
static void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
static void CALLBACK MouseHideTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
static void CALLBACK DisplayTextTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
static LRESULT CALLBACK MouseProc(UINT nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK KeyboardProc(UINT nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ClearGesture(LPVOID lpParam);
BOOL CALLBACK MonitorsCallBack( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData );

void saveConfig();
long StartSearchWindowDialog (HWND hwndMain);
BOOL CheckWindowValidity (HWND hwndDialog, HWND hwndToCheck);
long DoMouseMove(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam);
long DoMouseUp(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam);
BOOL InitialiseResources();
BOOL UninitialiseResources();
BOOL SetFinderToolImage (HWND hwndDialog, BOOL bSet);
BOOL MoveCursorPositionToBullsEye (HWND hwndDialog);
long SearchWindow (HWND hwndDialog);
long DisplayInfoOnFoundWindow (HWND hwndDialog, HWND hwndFoundWindow);
long RefreshWindow (HWND hwndWindowToBeRefreshed);
long HighlightFoundWindow (HWND hwndDialog, HWND hwndFoundWindow);
void resizeSettings();
BOOL LoadHotkeyVectors();
DWORD WINAPI gPopupMenuClick ( LPVOID lpParam ); 

INT_PTR CALLBACK	DisplayTextProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	SettingsProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	PreferencesTab(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ConfigTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	HotkeyTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	ReportTab(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	IgnoredTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	GestureNameProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	NameProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AboutTab(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	LuaTab(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	GestureTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	ChangePasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	InputBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#undef LIBSPEC

   

#ifdef __cplusplus
}
#endif // __cplusplus

// These declarations are because the latest SDK isn't installed...
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(p) ((int)(short)LOWORD(p))
#define GET_Y_LPARAM(p) ((int)(short)HIWORD(p))
#endif

//=============================================
// Please see SendKeys.cpp for copyright and usage issues. (actually this is in StrokesPlusHook.cpp)


  bool m_bWait, m_bUsingParens, m_bShiftDown, m_bAltDown, m_bControlDown, m_bWinDown;
  DWORD  m_nDelayAlways, m_nDelayNow;

  void   CarryDelay();

  typedef BYTE KEYBOARDSTATE_t[256];

  struct key_desc_t
  {
    LPCTSTR keyName;
    BYTE VKey;
    bool normalkey; // a normal character or a VKEY ?
  };

  enum
  {
    MaxSendKeysRecs  = 86,
    MaxExtendedVKeys = 10
  };

  /*
  Reference: VkKeyScan() / MSDN
  Bit Meaning 
  --- --------
  1   Either SHIFT key is pressed. 
  2   Either CTRL key is pressed. 
  4   Either ALT key is pressed. 
  8   The Hankaku key is pressed 
  16  Reserved (defined by the keyboard layout driver). 
  32  Reserved (defined by the keyboard layout driver). 
  */

  //static key_desc_t KeyNames[MaxSendKeysRecs]; 

  static bool BitSet(BYTE BitTable, UINT BitMask);

  void PopUpShiftKeys();

  static bool IsVkExtended(BYTE VKey);
  void SendKeyUp(BYTE VKey);
  void SendKeyDown(BYTE VKey, WORD NumTimes, bool GenUpMsg, bool bDelay = false);
  void SendKey(WORD MKey, WORD NumTimes, bool GenDownMsg);
  static WORD StringToVKey(LPCTSTR KeyString, int &idx);
  void KeyboardEvent(BYTE VKey, BYTE ScanCode, LONG Flags);


  bool SendKeys(LPCTSTR KeysString, bool Wait = false, bool SendDown = true, bool SendUp = true);

  void SetDelay(const DWORD delay) { m_nDelayAlways = delay; }

#ifdef SP_SYNAPTIC
  //SYNAPTIC TOUCHPAD RELATED

	class PacketData
	{
	public:
	  PacketData()
	  {
	    memset(this, 0, sizeof(*this));
	  }
	
	  PacketData(SynPacket *pPacket)
	  {
	    dwSequenceNumber = pPacket->SequenceNumber();
	    dwTimestamp = pPacket->TimeStamp();
	    lW = pPacket->W();
	    lX = pPacket->X();
	    lY = pPacket->Y();
	    lZ = pPacket->Z();
	    lXDelta = pPacket->XDelta();
	    lYDelta = pPacket->YDelta();
	    lZDelta = pPacket->ZDelta();
	    lXMickeys = pPacket->XMickeys();
	    lYMickeys = pPacket->YMickeys();
	    lXRaw = pPacket->XRaw();
	    lYRaw = pPacket->YRaw();
	    lZRaw = pPacket->ZRaw();
	    lZYRaw = pPacket->ZYRaw();
	    dwFingerState = pPacket->FingerState();
	    dwExtendedState = pPacket->ExtendedState();
	  }
	
	  DWORD dwSequenceNumber;
	  DWORD dwTimestamp;
	  LONG  lW;
	  LONG  lX;
	  LONG  lY;
	  LONG  lZ;
	  LONG  lXDelta;
	  LONG  lYDelta;
	  LONG  lZDelta;
	  LONG  lXMickeys;
	  LONG  lYMickeys;
	  LONG  lXRaw;
	  LONG  lYRaw;
	  LONG  lZRaw;
	  LONG  lZYRaw;
	  DWORD dwState;
	  DWORD dwFingerState;
	  DWORD dwExtendedState;
	};
	
	class PacketBuffer
	{
	public:
	  enum numbers { eSize = 128 };
	
	  PacketBuffer(void)
	  {
	    m_uiCurrent = 0;
	    m_iUnread = 0;
	  }
	
	  int NextLocation(void) 
	  { 
	    m_iUnread += m_iUnread == eSize ? 0 : 1;
	    return m_uiCurrent++ % eSize;
	  }
	
	  void Add(PacketData &Packet)
	  {
	    m_Packets[NextLocation()] = Packet;
	  }
	
	  PacketData *Read(void)
	  {
	    if (m_iUnread)
	      return m_Packets + ((m_uiCurrent - m_iUnread--) % eSize);
	    else
	      return 0;
	  }
	
	  int Unread(void) { return m_iUnread; }
	
	  PacketData m_Packets[eSize];
	  int m_iUnread;
	  unsigned int m_uiCurrent;
	};
	
	class Synaptic: public _ISynDeviceEvents
	{
		// Construction
		public:
			Synaptic();
			virtual ~Synaptic();
	
		// Attributes
		public:
	
			SynDevice m_Device;
			SynAPI m_API;
			SynPacket m_Packet;
			PacketBuffer m_Undrawn;
	
		// Operations
		public:
			void Init();
	
			void SelectDevice(DWORD hDevice);
	
			void NewDevice(long lHandle)
			{
			SelectDevice(lHandle);
			}
	
			int GetDeviceHandle(void)
			{
			return m_Device.GetLongProperty(SP_Handle);
			}
	
			// Overrides
			long STDMETHODCALLTYPE OnSynDevicePacket(long lSeq);
	
	};
	
	ISynAPI *m_pAPI;
#endif

// *** Touch API ***
#ifdef SP_TOUCH 

	//*********************************************
	#define WM_POINTERDEVICECHANGE          0x238
	#define WM_POINTERDEVICEINRANGE         0x239
	#define WM_POINTERDEVICEOUTOFRANGE      0x23A
	#define WM_TOUCH                        0x0240
	#define WM_NCPOINTERUPDATE              0x0241
	#define WM_NCPOINTERDOWN                0x0242
	#define WM_NCPOINTERUP                  0x0243
	#define WM_POINTERUPDATE                0x0245
	#define WM_POINTERDOWN                  0x0246
	#define WM_POINTERUP                    0x0247
	#define WM_POINTERENTER                 0x0249
	#define WM_POINTERLEAVE                 0x024A
	#define WM_POINTERACTIVATE              0x024B
	#define WM_POINTERCAPTURECHANGED        0x024C
	#define WM_TOUCHHITTESTING              0x024D
	#define WM_POINTERWHEEL                 0x024E
	#define WM_POINTERHWHEEL                0x024F

	enum tagPOINTER_INPUT_TYPE {
		PT_POINTER  = 0x00000001,   // Generic pointer
		PT_TOUCH    = 0x00000002,   // Touch
		PT_PEN      = 0x00000003,   // Pen
		PT_MOUSE    = 0x00000004,   // Mouse
	};
	typedef DWORD POINTER_INPUT_TYPE;

	typedef UINT32 POINTER_FLAGS;

	#define POINTER_FLAG_NONE               0x00000000 // Default
	#define POINTER_FLAG_NEW                0x00000001 // New pointer
	#define POINTER_FLAG_INRANGE            0x00000002 // Pointer has not departed
	#define POINTER_FLAG_INCONTACT          0x00000004 // Pointer is in contact
	#define POINTER_FLAG_FIRSTBUTTON        0x00000010 // Primary action
	#define POINTER_FLAG_SECONDBUTTON       0x00000020 // Secondary action
	#define POINTER_FLAG_THIRDBUTTON        0x00000040 // Third button
	#define POINTER_FLAG_FOURTHBUTTON       0x00000080 // Fourth button
	#define POINTER_FLAG_FIFTHBUTTON        0x00000100 // Fifth button
	#define POINTER_FLAG_PRIMARY            0x00002000 // Pointer is primary
	#define POINTER_FLAG_CONFIDENCE         0x00004000 // Pointer is considered unlikely to be accidental
	#define POINTER_FLAG_CANCELED           0x00008000 // Pointer is departing in an abnormal manner
	#define POINTER_FLAG_DOWN               0x00010000 // Pointer transitioned to down state (made contact)
	#define POINTER_FLAG_UPDATE             0x00020000 // Pointer update
	#define POINTER_FLAG_UP                 0x00040000 // Pointer transitioned from down state (broke contact)
	#define POINTER_FLAG_WHEEL              0x00080000 // Vertical wheel
	#define POINTER_FLAG_HWHEEL             0x00100000 // Horizontal wheel
	#define POINTER_FLAG_CAPTURECHANGED     0x00200000 // Lost capture


	/*
	 * Pointer info key states defintions.
	 */
	#define POINTER_MOD_SHIFT   (0x0004)    // Shift key is held down.
	#define POINTER_MOD_CTRL    (0x0008)    // Ctrl key is held down.

	typedef enum tagPOINTER_BUTTON_CHANGE_TYPE {
		POINTER_CHANGE_NONE,
		POINTER_CHANGE_FIRSTBUTTON_DOWN,
		POINTER_CHANGE_FIRSTBUTTON_UP,
		POINTER_CHANGE_SECONDBUTTON_DOWN,
		POINTER_CHANGE_SECONDBUTTON_UP,
		POINTER_CHANGE_THIRDBUTTON_DOWN,
		POINTER_CHANGE_THIRDBUTTON_UP,
		POINTER_CHANGE_FOURTHBUTTON_DOWN,
		POINTER_CHANGE_FOURTHBUTTON_UP,
		POINTER_CHANGE_FIFTHBUTTON_DOWN,
		POINTER_CHANGE_FIFTHBUTTON_UP,
	} POINTER_BUTTON_CHANGE_TYPE;

	typedef struct tagPOINTER_INFO {
		POINTER_INPUT_TYPE    pointerType;
		UINT32          pointerId;
		UINT32          frameId;
		POINTER_FLAGS   pointerFlags;
		HANDLE          sourceDevice;
		HWND            hwndTarget;
		POINT           ptPixelLocation;
		POINT           ptHimetricLocation;
		POINT           ptPixelLocationRaw;
		POINT           ptHimetricLocationRaw;
		DWORD           dwTime;
		UINT32          historyCount;
		INT32           InputData;
		DWORD           dwKeyStates;
		UINT64          PerformanceCount;
		POINTER_BUTTON_CHANGE_TYPE ButtonChangeType;
	} POINTER_INFO;


	typedef UINT32 TOUCH_FLAGS;
	#define TOUCH_FLAG_NONE                 0x00000000 // Default

	typedef UINT32 TOUCH_MASK;
	#define TOUCH_MASK_NONE                 0x00000000 // Default - none of the optional fields are valid
	#define TOUCH_MASK_CONTACTAREA          0x00000001 // The rcContact field is valid
	#define TOUCH_MASK_ORIENTATION          0x00000002 // The orientation field is valid
	#define TOUCH_MASK_PRESSURE             0x00000004 // The pressure field is valid

	typedef struct tagPOINTER_TOUCH_INFO {
		POINTER_INFO    pointerInfo;
		TOUCH_FLAGS     touchFlags;
		TOUCH_MASK      touchMask;
		RECT            rcContact;
		RECT            rcContactRaw;
		UINT32          orientation;
		UINT32          pressure;
	} POINTER_TOUCH_INFO;

	typedef UINT32 PEN_FLAGS;
	#define PEN_FLAG_NONE                   0x00000000 // Default
	#define PEN_FLAG_BARREL                 0x00000001 // The barrel button is pressed
	#define PEN_FLAG_INVERTED               0x00000002 // The pen is inverted
	#define PEN_FLAG_ERASER                 0x00000004 // The eraser button is pressed

	typedef UINT32 PEN_MASK;
	#define PEN_MASK_NONE                   0x00000000 // Default - none of the optional fields are valid
	#define PEN_MASK_PRESSURE               0x00000001 // The pressure field is valid
	#define PEN_MASK_ROTATION               0x00000002 // The rotation field is valid
	#define PEN_MASK_TILT_X                 0x00000004 // The tiltX field is valid
	#define PEN_MASK_TILT_Y                 0x00000008 // The tiltY field is valid

	typedef struct tagPOINTER_PEN_INFO {
		POINTER_INFO    pointerInfo;
		PEN_FLAGS       penFlags;
		PEN_MASK        penMask;
		UINT32          pressure;
		UINT32          rotation;
		INT32           tiltX;
		INT32           tiltY;
	} POINTER_PEN_INFO;

	/*
	 * Flags that appear in pointer input message parameters
	 */
	#define POINTER_MESSAGE_FLAG_NEW                0x00000001 // New pointer
	#define POINTER_MESSAGE_FLAG_INRANGE            0x00000002 // Pointer has not departed
	#define POINTER_MESSAGE_FLAG_INCONTACT          0x00000004 // Pointer is in contact
	#define POINTER_MESSAGE_FLAG_FIRSTBUTTON        0x00000010 // Primary action
	#define POINTER_MESSAGE_FLAG_SECONDBUTTON       0x00000020 // Secondary action
	#define POINTER_MESSAGE_FLAG_THIRDBUTTON        0x00000040 // Third button
	#define POINTER_MESSAGE_FLAG_FOURTHBUTTON       0x00000080 // Fourth button
	#define POINTER_MESSAGE_FLAG_FIFTHBUTTON        0x00000100 // Fifth button
	#define POINTER_MESSAGE_FLAG_PRIMARY            0x00002000 // Pointer is primary
	#define POINTER_MESSAGE_FLAG_CONFIDENCE         0x00004000 // Pointer is considered unlikely to be accidental
	#define POINTER_MESSAGE_FLAG_CANCELED           0x00008000 // Pointer is departing in an abnormal manner

	/*
	 * Macros to retrieve information from pointer input message parameters
	 */
	#define GET_POINTERID_WPARAM(wParam)                (LOWORD(wParam))
	#define IS_POINTER_FLAG_SET_WPARAM(wParam, flag)    (((DWORD)HIWORD(wParam) & (flag)) == (flag))
	#define IS_POINTER_NEW_WPARAM(wParam)               IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_NEW)
	#define IS_POINTER_INRANGE_WPARAM(wParam)           IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_INRANGE)
	#define IS_POINTER_INCONTACT_WPARAM(wParam)         IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_INCONTACT)
	#define IS_POINTER_FIRSTBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FIRSTBUTTON)
	#define IS_POINTER_SECONDBUTTON_WPARAM(wParam)      IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_SECONDBUTTON)
	#define IS_POINTER_THIRDBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_THIRDBUTTON)
	#define IS_POINTER_FOURTHBUTTON_WPARAM(wParam)      IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FOURTHBUTTON)
	#define IS_POINTER_FIFTHBUTTON_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FIFTHBUTTON)
	#define IS_POINTER_PRIMARY_WPARAM(wParam)           IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_PRIMARY)
	#define HAS_POINTER_CONFIDENCE_WPARAM(wParam)       IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_CONFIDENCE)
	#define IS_POINTER_CANCELED_WPARAM(wParam)          IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_CANCELED)

	/*
	 * WM_POINTERACTIVATE return codes
	 */
	#define PA_ACTIVATE                     MA_ACTIVATE
	#define PA_NOACTIVATE                   MA_NOACTIVATE

	typedef bool(*LPREGISTERTOUCHINPUT)(HWND hwnd, long pointerType);
	typedef bool(*LPUNREGISTERTOUCHINPUT)(HWND hwnd, long pointerType);
	typedef bool(*LPINITTOUCHINJECTION)(UINT32 maxCount, DWORD dwMode);
	typedef bool(*LPINJECTTOUCHINPUT)(UINT32 count, const POINTER_TOUCH_INFO *contacts);

#endif

#ifdef SP_WIN8_CHECKS
	typedef BOOL (WINAPI* IsImmersiveProcessFunc)(HANDLE process);
#endif

#ifdef SP_MONITOR_BRIGHTNESS
	#define PHYSICAL_MONITOR_DESCRIPTION_SIZE                   128
	typedef struct _PHYSICAL_MONITOR
	{
		HANDLE hPhysicalMonitor;
		WCHAR szPhysicalMonitorDescription[PHYSICAL_MONITOR_DESCRIPTION_SIZE];
	} PHYSICAL_MONITOR, *LPPHYSICAL_MONITOR;

	typedef BOOL (WINAPI *GetNumberOfPhysicalMonitorsFromHMONITOR)(HMONITOR hMonitor, __out LPDWORD pdwNumberOfPhysicalMonitors);
	typedef BOOL (WINAPI *GetPhysicalMonitorsFromHMONITOR)(HMONITOR hMonitor, DWORD dwPhysicalMonitorArraySize, __out_ecount(dwPhysicalMonitorArraySize) LPPHYSICAL_MONITOR pPhysicalMonitorArray);
	typedef BOOL (WINAPI *GetMonitorBrightness)(HANDLE hMonitor, __out LPDWORD pdwMinimumBrightness, __out LPDWORD pdwCurrentBrightness, __out LPDWORD pdwMaximumBrightness);
	typedef BOOL (WINAPI *SetMonitorBrightness)(HANDLE hMonitor, DWORD dwNewBrightness );
	typedef BOOL (WINAPI *DestroyPhysicalMonitors)(DWORD dwPhysicalMonitorArraySize, __in_ecount(dwPhysicalMonitorArraySize) LPPHYSICAL_MONITOR pPhysicalMonitorArray);

#endif

#endif // _DEFINED_44E531B1_14D3_11d5_A025_006067718D04

