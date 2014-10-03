

#pragma region Definitions and Includes

#define BOOST_REGEX_NO_LIB
extern "C" {
#include <lua.h>
#include <lauxlib.h> 
#include <lualib.h>
}
#define _USE_MATH_DEFINES
#define SCI_LEXER
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <cmath>
#include <vector>
#include "stdafx.h"
#include "Windows.h"
#include "Winver.h"
#include "OCIdl.h"
#include "commctrl.h"
#include "Commdlg.h"
#include "Mmsystem.h"
#include "shellapi.h"
#include "psapi.h"
#include <string>
#include <sstream>
#include <strsafe.h>
#include <iostream>
#include "tlhelp32.h"
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "Registry.h"
/* dynamically loaded to allow running on XP
#ifdef SP_MONITOR_BRIGHTNESS
	#include "PhysicalMonitorEnumerationAPI.h"
	#include "HighLevelMonitorConfigurationAPI.h"
#endif
*/
#pragma comment( lib, "psapi.lib" )
#pragma comment(lib,"comctl32.lib")

#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#define _WIN32_IE 0x500
#define COMPILE_MULTIMON_STUBS
#include "multimon.h" 


//#define _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
#define ID_TVPOPUP_CUT (WM_USER + 42)
#define ID_TVPOPUP_DELETE (WM_USER + 41)
#define ID_TVPOPUP_RENAME (WM_USER + 40)
#define ID_TVPOPUP_ADDAPP (WM_USER + 39)
#define ID_TVPOPUP_ADDACTION (WM_USER + 38)
#define ID_TVPOPUP_PASTE (WM_USER + 37)
#define ID_TVPOPUP_COPY (WM_USER + 36)
#define ID_ICONPOPUP_SETTINGS 144
#define ID_ICONPOPUP_PREF 123
#define ID_ICONPOPUP_HOTKEYS 161
#define ID_ICONPOPUP_DISABLED 122
#define ID_ICONPOPUP_CONFIG 132
#define ID_ICONPOPUP_EXIT_NOSAVE 160
#define ID_ICONPOPUP_EXIT 120
#define ID_ICONPOPUP_IGNORED 134
#define ID_ICONPOPUP_ABOUT 143
#define ID_ICONPOPUP_RELOAD 135
#define ID_ICONPOPUP_LOADCONFIG 162
#define ID_ICONPOPUP_RELOAD_DELAY 163
//#define IDT_RELEASEMSG 1
#define WM_SHOWGESTURENAMEDLG (WM_USER + 3)
#define WM_IDT_RELEASEMSG (WM_USER + 4)
#define WM_IDT_TRIM (WM_USER + 20)
#define WM_IDT_MOUSEHIDE (WM_USER + 21)
#define WM_SHOWDISABLEICON (WM_USER + 5)
#define WM_RESETDISABLEICON (WM_USER + 6)
#define WM_TOGGLETRAYICON (WM_USER + 7)
#define WM_IDT_DISPLAYTEXT (WM_USER + 19)
#define WM_IDT_STROKESWATCH_DRAW (WM_USER + 8)
#define WM_LOADHOTKEYS (WM_USER + 22)
#define BULLSEYE_CENTER_X_OFFSET		15
#define BULLSEYE_CENTER_Y_OFFSET		18
#define WM_TRAYMESSAGE (WM_USER + 1)
#define WM_SETTINGSPREVTAB (WM_USER + 23)
#define WM_SETTINGSNEXTTAB (WM_USER + 24)
#define WM_MENUPOPUP (WM_USER + 1024)
#define WM_DISABLEHOTKEY (WM_USER + 8)
#define WM_ENABLEHOTKEY (WM_USER + 9)


#include <boost/property_tree/xml_parser.hpp>

#include <boost/lexical_cast.hpp>
#include "StrokesPlusHook.h"

// Include Scintilla parser
#include <SciLexer.h>
#include <Scintilla.h>

extern "C" int luaopen_alien_core(lua_State *L);
extern "C" int luaopen_alien_struct(lua_State *L);

#pragma endregion Definitions and Includes


#pragma region Namespaces, Templates, Structs, Global Variables

using namespace Gdiplus;
using namespace std;

HTREEITEM rightClickCopyNode = NULL;
BOOL rightClickCutMode = FALSE;
HTREEITEM itemUnderMouse = NULL;
bool bGlobalUserCancel = false;			
bool bOnlyAllowDefinedApps = false;		
int iResumeDelay = 5000;				
#ifdef SP_SYNAPTIC
	bool bUseTouchCursor = false;
	std::string sTouchCursor = _T("");
	int iTouchPadHoldThresholdDelta = 4;
	int iTouchPadHoldDelay = 500;
	int iTouchPadThresholdDelta = 1;
	long lTouchPadFirstContactStamp = 0;
	long lTouchPadStamp = 0;
	bool bIgnoreTouchpadSequence = false;
	long tickcount = 0;
	bool bTouchEnabled = false;
	bool bPenEnabled = false;
	bool bTouchAvailable = false;
	bool bTouchPadMultiFinger = false;
#endif
#ifdef SP_WIN8_CHECKS
	IsImmersiveProcessFunc lpfnIsImmersiveProcessFunc; 
#endif

#ifdef SP_MONITOR_BRIGHTNESS
	GetNumberOfPhysicalMonitorsFromHMONITOR lpfnGetNumberOfPhysicalMonitorsFromHMONITOR; 
	GetPhysicalMonitorsFromHMONITOR lpfnGetPhysicalMonitorsFromHMONITOR;
	GetMonitorBrightness lpfnGetMonitorBrightness;
	SetMonitorBrightness lpfnSetMonitorBrightness;
	DestroyPhysicalMonitors lpfnDestroyPhysicalMonitors;
#endif

#ifdef SP_TOUCH
	LPREGISTERTOUCHINPUT lpfnRegisterTouchInput;    
	LPUNREGISTERTOUCHINPUT lpfnUnregisterTouchInput;   
	LPINITTOUCHINJECTION lpfnInitializeTouchInjection;
	LPINJECTTOUCHINPUT lpfnInjectTouchInput;
#endif
#ifdef SP_SYNAPTIC
	Synaptic *mSynaptic = NULL;
	int iTouchPadFingers = 1;
	int iTouchPadSensitivity = 25;
	BOOL bDisableSynaptics = true;
	BOOL bCancelTouchPad = false;
	BOOL bTouchPadDown = false;
	BOOL bSynapticGesture = false;
#endif
#ifdef SP_AFTERRELEASESCRIPT
	BOOL bFireOnRelease = false;  
#endif
#ifdef SP_MOUSE_CLICK_ACTION
	BOOL bFireOnLeftClick = false;
	BOOL bFireOnRightClick = false;
	BOOL bFireOnMiddleClick = false;
	BOOL bFireOnX1Click = false;
	BOOL bFireOnX2Click = false;
	struct XClick
	{
		int button;
		DWORD fwKeys;
		int x;
		int y;
	};
#endif
#ifdef SP_WHEEL_HOOK
	bool bMouseWheelTick = FALSE; 
	bool bHorzMouseWheelTick = FALSE; 
	struct WheelTick
	{
		bool Horizontal;
		HWND hwndControl;
		DWORD wParam;
		DWORD lParam;
		int x;
		int y;
		int delta;
	};
#endif
int iSetNumber = 0;
bool bRelayGesture = FALSE;
bool bShowCopyData = FALSE;
int popx = 0;
int popy = 0;
std::string menuItems;
std::string menuDelim;
std::string popupMenuFunctionName = "";
int popupMenuItemID = 0;
vector<HWND> GetAllWindowsList;
BOOL bActivateMouseWheelWindow = false;
string sInputBoxTitle = "";
string sInputBoxLabel = "";
string sInputBoxResult = "";
string sInputBoxDelim = "";
BOOL bInputBoxOpen = false;
int iActionLuaZoomFactor = 0;
int iHotkeyLuaZoomFactor = 0;
int iGlobalLuaZoomFactor = 0;
HBRUSH g_hbrBackground;
int trayID = 100;
BOOL bConsumeStrokeKey = false;
HWND hWndHotkey;
int iHotkey = 1;
vector<int> HotkeyIDs;
vector<string> HotkeyNames;
vector<BYTE> HotkeyValues;
int iMouseHideCounter = 0;
int iMouseReHideCounter = 0;
int iHideMouseAfter = 0;
int iReHideMouseAfter = 0;
int iHideLastX = 0;
int iHideLastY = 0;
bool bCursorHidden = false;
bool bAutoHideMouse = false;
HCURSOR curBlank[13];
bool bSetPassword = false;
bool bPasswordOpen = false;
bool bChangePasswordOpen = false;
bool bPasswordMatch = false;
bool bCheckPassword = false;
bool bTriedDecrypt = false;
string sPassword = "";
RECT lpPreview;
POINT ptPreviewTL;
POINT ptPreviewTR;
boost::property_tree::ptree ptActions;
std::basic_string<TCHAR> sNoMatchSoundWAV = _T("");
int iXVIRTUALSCREEN = GetSystemMetrics(SM_XVIRTUALSCREEN);
int iYVIRTUALSCREEN = GetSystemMetrics(SM_YVIRTUALSCREEN);
int iCYVIRTUALSCREEN = GetSystemMetrics(SM_CYVIRTUALSCREEN);
int iCXVIRTUALSCREEN = GetSystemMetrics(SM_CXVIRTUALSCREEN);
float fDPIModifier = 1;
int opacity = 150;
int iSelectedTab = 0;
bool bSettingsMaximized = false;
bool bSendKeysExecuting = false;
bool bNoMatchSound = false;
int iBoundingLeft = 0;
int iBoundingTop = 0;
int iBoundingRight = 0;
int iBoundingBottom = 0;
bool bConsumePhysical = false;
bool bLuaState1Firing = false;
bool bLuaState2Firing = false;
HWND hWndT;
HWND hWndName;
HWND hAboutTab;
HWND hLuaTab;
HWND hWndSettings;
HWND hTabControlMain; 
HWND hActionsTab; 
HWND hActionsReportTab; 
HWND hIgnoredTab; 
HWND hHotkeyTab; 
HWND hPreferencesTab; 
HWND hGesturesTab; 
HIMAGELIST hList = NULL;
HIMAGELIST hListTree = NULL;
HWINEVENTHOOK g_hook;
CRITICAL_SECTION CriticalSection; 
CRITICAL_SECTION CriticalSectionHotkey; 
CRITICAL_SECTION WheelTickCriticalSection;
BOOL bCheckCursorFlags = true;
BOOL bIgnoreDisable = false;
BOOL bHooked = false;
BOOL bAutosave = FALSE;
BOOL bGesturesDisabled = FALSE;
BOOL bAggressiveTrim = TRUE;
BOOL bCheckForOtherGesturePrograms = TRUE;
BOOL bCheckForegroundTimeout = FALSE;
BOOL bDrawWasOn = TRUE;
BOOL bScreenChanged = FALSE;
vector<POINT> g_Points;
TCHAR strExePath [_MAX_PATH];
TCHAR strStrokesConfigFile [_MAX_PATH];
BOOL bTrayIconVisible = TRUE;
BOOL bAllowAfterScript = FALSE;
BOOL bResetCancelDelayOnMovement = TRUE;
BOOL bDrawWindowAlwaysOnTop = FALSE;
HWND hWndCB;
BOOL g_bStartSearchWindow = FALSE;
HCURSOR	g_hCursorSearchWindow = NULL;
HCURSOR	g_hCursorPrevious = NULL;
HBITMAP	g_hBitmapFinderToolFilled;
HBITMAP	g_hBitmapFinderToolEmpty;
HWND g_hwndFoundWindow = NULL;
HPEN g_hRectanglePen = NULL;
int iConfigWidth = 1014;
int iConfigHeight = 703;
int iMinSegmentLength = 6;
BOOL bDrawWindowToBottom = FALSE;
BOOL bSaveActionSize = FALSE;
BOOL bFireOnMouseWheel = FALSE;
BOOL bCaptureModifiersOnMouseDown = FALSE;
vector<POINTF> TempDrawPoints;
vector<POINTF> ScaledDrawPoints;
vector<std::basic_string<TCHAR>> GestureNames;
int iScaledWidth = 0;
int iScaledHeight = 0;
std::basic_string<TCHAR> sLastParent = _T("");
std::basic_string<TCHAR> sLastChild = _T("");
std::basic_string<TCHAR> sLastIgnore = _T("");
std::basic_string<TCHAR> sLastHotkey = _T("");
bool bSettingsOpen = false;
BOOL bReInitOnResume = FALSE;
BOOL bIsSaving = FALSE;
BOOL bIgnoredIsSaving = FALSE;
HWND hWndIgnore;
HWND hWndConfig;
HWND hWndReport;
BOOL bAboutOpen = FALSE;
BOOL bGestureOpen = FALSE;
BOOL bNameOpen = FALSE;
BOOL bIgnoreName = FALSE;
BOOL bIgnoreRename = FALSE;
BOOL bConfigNameApp = FALSE;
BOOL bConfigRenameApp = FALSE;
BOOL bConfigNameAction = FALSE;
BOOL bConfigRenameAction = FALSE;
BOOL bConfigRenameGesture = FALSE;
BOOL bConfigNameHotkey = FALSE;
BOOL bConfigRenameHotkey = FALSE;
BOOL bCriteraFieldsActive = FALSE;
BOOL bNewGestureMode = FALSE;
BOOL bPreviousTrainingModeState = FALSE;
BOOL bHideNewGestureMessage = FALSE;
BOOL bHideAdditionalInstanceMessage = FALSE;
BOOL g_ConfigShown = FALSE;
BOOL g_GestureNameShown = FALSE;
DWORD g_processID = 0;
HWND hWndProcessID = NULL;
BOOL bMouseWheelFiring = FALSE;
TCHAR* ActionBuffer1;
BOOL bBreak = FALSE;
BOOL bScrolling = FALSE;
TCHAR gszFileName[_MAX_PATH];
TCHAR gszClassName[256];
HWND hWndServer = NULL;
HWND hWndDisplayText = NULL;
BOOL bOSDDisplayed = FALSE;
BOOL bMouseWheelFocus = FALSE;
HINSTANCE hInst;
HHOOK mousehook;
HHOOK keyboardhook;
RECT rcWindow;
UINT_PTR strokeswatchtimerid;
UINT_PTR canceltimerid;  
UINT_PTR trimtimerid;
UINT_PTR displaytexttimerid;
UINT_PTR mousehidetimerid;
int iDisplayTextSize = 96;
std::basic_string<TCHAR> sDisplayTextFont = _T("Arial");
std::basic_string<TCHAR> sDisplayTextMessage = _T("");
int iDisplayTextX = 0;
int iDisplayTextY = 0;
int iDisplayTextDuration = 1000;
int iDisplayTextR = 255;
int iDisplayTextG = 0;
int iDisplayTextB = 0;
int iDisplayTextBGR = 255;
int iDisplayTextBGG = 0;
int iDisplayTextBGB = 0;
std::basic_string<TCHAR> DrawnGestureName = _T("");
std::basic_string<TCHAR> LastDrawnGestureName = _T("");
std::basic_string<TCHAR> MatchedAppName = _T("");
std::basic_string<TCHAR> MatchedActionName = _T("");
BOOL bIgnoreGesture = FALSE;
BOOL bCancel = FALSE;  
BOOL bGlobalCancel = FALSE;  
BOOL bDrawGesture = TRUE;
BOOL bMouseDown = FALSE;
BOOL bLeftMouseDown = FALSE;
BOOL bMiddleMouseDown = FALSE;
BOOL bRightMouseDown = FALSE;
BOOL bX1MouseDown = FALSE;
BOOL bX2MouseDown = FALSE;
BOOL bMouseWheelUp = FALSE;
BOOL bMouseWheelDown = FALSE;
BOOL bControlDown = FALSE;
BOOL bAltDown = FALSE;
BOOL bShiftDown = FALSE;
BOOL bLeftMouseDownBefore = FALSE;
BOOL bMiddleMouseDownBefore = FALSE;
BOOL bRightMouseDownBefore = FALSE;
BOOL bX1MouseDownBefore = FALSE;
BOOL bX2MouseDownBefore = FALSE;
BOOL bControlDownBefore = FALSE;
BOOL bAltDownBefore = FALSE;
BOOL bShiftDownBefore = FALSE;
BOOL bLearningMode = FALSE;
HMONITOR hTempScreen = NULL;
HWND hWndLastWindow = NULL;
DWORD dWheelDelta = 0;
vector<POINTF> DrawPoints;
vector<POINTF> LastDrawPoints;
int MatchProbabilityThreshold = 75;
int iPrecision = 100;
int EnumWindowCount = 0;
int EnumWindowCountTemp = 0;
int IgnoreKey = 0;
int CancelDelay = 500;
int iMaxDrawPoints = 0;
int TrimDelay = 2000;
int PenWidth = 5;
int hookbtn = 0;
int OSVersion = 5; 
int lastx,lasty,x,y,startx,starty;
COLORREF PenColor = RGB(0,0,255);
HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));   //Gesture draw window background color (white)
HPEN hDrawPen = CreatePen(PS_SOLID, PenWidth, RGB(0,0,255));	//Create a blue pen (for gesture thumbnails)
HPEN hBackgroundPen = CreatePen(PS_SOLID, PenWidth, RGB(255,255,255));	//Background (white) pen, for retracing the gesture to erase it
HDC hdc;
lua_State *L;
lua_State *L2;
HMONITOR hFirstEnumMonitor = NULL;
HMONITOR hLastEnumMonitor = NULL;
HMONITOR hCurrentEnumMonitor = NULL;
HMONITOR hActiveMonitor = NULL;
HMONITOR hSendToMonitor = NULL;
HMONITOR hIncomingMonitor = NULL;
std::basic_string<TCHAR> sMonitorName = _T("");

static const string g_CursorTypeNames[] = {	_T("ARROW"), 
											_T("IBEAM"), 
											_T("WAIT"), 
											_T("CROSS"), 
											_T("UP"), 
											_T("SIZENWSE"), 
											_T("SIZENESW"), 
											_T("SIZEWE"), 
											_T("SIZENS"), 
											_T("SIZEALL"), 
											_T("NO"), 
											_T("HAND"),
											_T("APPSTARTING"),
											_T("HELP")
										   };
static const int g_CursorTypes[] = {32512 /*OCR_NORMAL*/, 
									32513 /*OCR_IBEAM*/, 
									32514 /*OCR_WAIT*/, 
									32515 /*OCR_CROSS*/, 
									32516 /*OCR_UP*/, 
									32642 /*OCR_SIZENWSE*/, 
									32643 /*OCR_SIZENESW*/, 
									32644 /*OCR_SIZEWE*/, 
									32645 /*OCR_SIZENS*/, 
									32646 /*OCR_SIZEALL*/, 
									32648 /*OCR_NO*/, 
									32649 /*OCR_HAND*/, 
									32650 /*OCR_APPSTARTING*/, 
									32651 /*IDC_HELP*/};

typedef struct {
    int iImage;
    int iSelectedImage;
    int iIndent;
    LPTSTR pszText;
} ITEMINFO, *PITEMINFO;

struct LessNoCase {
	bool operator()(const std::basic_string<TCHAR> &x, const std::basic_string<TCHAR> &y) const {
		return (_tcsicmp (x.c_str(), y.c_str()) < 0);
	}
};

void initLua();

static int acShowMouseCursor(lua_State *L);

typedef struct luaMemFile
{
  const char *text;
  size_t size;
} luaMemFile;

struct FindWindowData {
    FindWindowData( TCHAR const * windowTitle )
        : WindowTitle( windowTitle )
        , ResultHandle( 0 )
    {}

    std::basic_string<TCHAR> WindowTitle;
    HWND ResultHandle;
};

#pragma endregion Namespaces, Templates, Structs, Global Variables


#pragma region Scintilla

//I don't recall why I triplicated these Scintilla functions..but I never got back to see if there was a valid reason or if I was just prototyping and never refactored

HWND m_hwndEditor;
HWND m_hwndHotkeyEditor;
HWND m_hwndLuaEditor;

static const char g_LuaKeyWords[] = 

    // define the Lua keywords for Scintilla to apply color coding
	"and break do else elseif end false for function if in local nil not or repeat return then true until while _VERSION assert "
	"collectgarbage dofile error gcinfo loadfile load print rawget rawset require tonumber tostring type unpack "
	"assert collectgarbage dofile error gcinfo loadfile loadstring print rawget rawset require tonumber tostring type unpack "
	"_ALERT _ERRORMESSAGE _INPUT _PROMPT _OUTPUT _STDERR _STDIN _STDOUT call dostring foreach foreachi getn globals newtype "
	"abs acos asin atan atan2 ceil cos deg exp floor format frexp gsub ldexp log log10 max min mod rad random randomseed "
	"sin sqrt strbyte strchar strfind _tcslen strlower strrep strupper tan string.byte string.char string.dump string.find string.len "
	"string.lower string.rep string.sub string.upper string.format string.gfind string.gsub table.concat table.foreach table.foreachi "
	"table.getn table.sort table.insert table.remove table.setn math.abs math.acos math.asin math.atan math.atan2 math.ceil "
	"math.cos math.deg math.exp math.floor math.frexp math.ldexp math.log math.log10 math.max math.min math.mod math.pi math.pow math.rad "
	"math.random math.randomseed math.sin math.sqrt math.tan string.gmatch string.match string.reverse table.maxn "
	"math.cosh math.fmod math.modf math.sinh math.tanh math.huge sort tinsert tremove bit32.arshift bit32.band bit32.bnot bit32.bor "
	"bit32.btest bit32.bxor bit32.extract bit32.lrotate bit32.lshift bit32.replace bit32.rrotate bit32.rshift";
    
static const char g_actionKeyWords[] = 

	// define the S+ keywords for Scintilla to apply color coding
    "gact gapp gsy gsx gey gex gbt gbl gbb gbr sp_action_name sp_app_name sp_gesture_name gnm gwd sp_wheel_delta sp_gesture_end_y sp_gesture_start_y sp_gesture_start_x sp_gesture_end_x "
    "acPreviousApplication acNextApplication acDisable acDisableNext acCloseApplication acMaximizeWindow acMaximizeOrRestoreWindow acDelay "
	"acMoveWindow acMinimizeWindow acSendKeys acRunProgram acSetVolume acGetVolume acToggleMute acGetMute acSetMute acGetControlID acFindWindow "
	"acGetWindowTitle acMessageBox acFindWindowByTitleRegex acSetWindowSize acSendWindowToBottom acActivateWindow acRestoreWindow acGetOwnerWindowByPoint "
	"acGetParentWindowByPoint acGetWindowByPoint acSetProcessPriority acTerminateProcess acToggleTopmost acSetTopmost acClearTopmost acPauseResumeThreadList "
	"acGetSystemMetrics acSetWindowTransparency acGetWindowTransparency acSetWindowColorKey acSetWindowTransparencyAndColorKey acShowSettings acShowPrefs acShowHotkeys "
	"acShowActions acShowIgnoreList acGetWindowLeft acGetWindowTop acGetWindowRight acGetWindowBottom acGetWindowColorKeyR acGetWindowColorKeyG acGetWindowColorKeyB "
	"acGetPixelRByPoint acGetPixelGByPoint acGetPixelBByPoint acExit acReloadConfig acGetMouseLocationX acGetMouseLocationY acMouseMove acMouseClick acSendMessage "
	"acPostMessage acGetForegroundWindow acGetDesktopWindow acGetClipboardText acSetClipboardText acSendWinDown acSendWinUp acSendControlDown acSendControlUp "
	"acSendAltDown acSendAltUp acSendShiftDown acSendShiftUp acCenterWindowToScreen acClipWindowToScreen acFitWindowToScreen acMaximizeToAllScreens acGetMonitorFromPoint "
	#ifdef SP_MONITOR_BRIGHTNESS
		"acGetMonitorBrightness acSetMonitorBrightness acSetDisplayGamma " //acGetDisplayGamma sp_gamma_ramp
	#endif
	#ifdef SP_WHEEL_HOOK
		"sp_wheel_tick sp_h_wheel_tick "
	#endif
	#ifdef SP_MOUSE_CLICK_ACTION
		"sp_right_mouse_up sp_middle_mouse_up sp_left_mouse_up sp_x1_mouse_up sp_x2_mouse_up "
	#endif
	"acGetMonitorName acSendWindowToPreviousMonitor acSendWindowToMonitorByName acSendWindowToMonitorByHandle acSendWindowToNextMonitor acGetMonitorFromName acGetMonitorBottom "
	"acGetMonitorTop acGetMonitorLeft acGetMonitorRight acPlaySound acDisplayText acKillDisplayText acGetExecutableName acGetClassName acLockWindows acToggleTrayIcon acShellExecute acTileWindows "
	"acShowAbout acGetProcessIDFromPattern acGetWindowFromProcessID sp_init sp_after_action sp_before_action acConsumePhysicalInput acSetKeyboardHook acGetMouseCursorType acHideMouseCursor acDisplayBalloonTip "
	"acShowMouseCursor acAutoHideMouseCursor acSendKeyDown acSendKeyUp acRegistryGetString acRegistrySetString acRegistryGetNumber acRegistrySetNumber acRegistryCreateKey acRegistryDeleteKey acRegistryDeleteValue "
	"acGetFileVersion acGetFileProductVersion acSendString acInputBox acActivateWindowUnderMouseWheel acGetAllWindows sp_all_windows sp_after_release acIsImmersiveProcess acDisableCapture acEnableCapture "
	"acCreatePopupMenu acDisableHotkey acEnableHotkey acRelayGesture acSetNumber acGetNumber"; 


/// Scintilla Colors structure
struct SScintillaColors
{   int         iItem;
    COLORREF    rgb;
};

// A few basic colors
const COLORREF black = RGB( 0, 0, 0 );
const COLORREF white = RGB( 255, 255, 255 );
const COLORREF green = RGB( 24, 156, 55 );
const COLORREF red = RGB( 255, 0, 0 );
const COLORREF blue = RGB( 0, 0, 255 );
const COLORREF yellow = RGB( 255, 255, 0 );
const COLORREF magenta = RGB( 255, 0, 255 );
const COLORREF cyan = RGB( 0, 255, 255 );
const COLORREF darkgray = RGB( 85, 85, 85 );

/// Default color scheme
static SScintillaColors g_rgbSyntaxLua[] = 
{
	{   SCE_LUA_DEFAULT,             black },
    {   SCE_LUA_COMMENT,          green },
    {   SCE_LUA_COMMENTLINE,      green },
    {   SCE_LUA_COMMENTDOC,       green },
    {   SCE_LUA_NUMBER,           magenta },
    {   SCE_LUA_STRING,           darkgray },
    {   SCE_LUA_CHARACTER,        darkgray },
    {   SCE_LUA_OPERATOR,         red },
    {   SCE_LUA_PREPROCESSOR,     cyan },
    {   SCE_LUA_IDENTIFIER,        black },
	{   SCE_LUA_WORD,             blue },
	{   SCE_LUA_WORD2,             RGB(0,128,128) },
	{   SCE_LUA_WORD3,             RGB(0,128,128) },
	{   SCE_LUA_LABEL,             cyan },
    {   -1,                     0 }
};

/// Sends a message to the Scintilla editor (Global Lua tab)
LRESULT SendLuaEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
{	return ::SendMessage(m_hwndLuaEditor, Msg, wParam, lParam); }

/// Sends a message to the Scintilla editor (hotkey tab)
LRESULT SendHotkeyEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
{	return ::SendMessage(m_hwndHotkeyEditor, Msg, wParam, lParam); }

/// Sets a Scintilla style (Global Lua tab)
void SetLuaAStyle(int style, COLORREF fore, COLORREF back=RGB( 0, 0, 0 ), int size=-1, const TCHAR *face=0)
{	SendLuaEditor(SCI_STYLESETFORE, style, fore);
	SendLuaEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendLuaEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendLuaEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

/// Sets a Scintilla style (Hotkey tab)
void SetHotkeyAStyle(int style, COLORREF fore, COLORREF back=RGB( 0, 0, 0 ), int size=-1, const TCHAR *face=0)
{	SendHotkeyEditor(SCI_STYLESETFORE, style, fore);
	SendHotkeyEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendHotkeyEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendHotkeyEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

/// Sends a message to the Scintilla editor (Actions tab)
LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) 
{	return ::SendMessage(m_hwndEditor, Msg, wParam, lParam); }

/// Sets a Scintilla style (Actions tab)
void SetAStyle(int style, COLORREF fore, COLORREF back=RGB( 0, 0, 0 ), int size=-1, const TCHAR *face=0)
{	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

//Init Scintilla editor (Actions tab)
void InitialiseEditor() 
{
    // Punt if we already have a window
    if ( ::IsWindow( m_hwndEditor ) ) return;

    // Create editor window
    m_hwndEditor = CreateWindowEx(  0, _T("Scintilla"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 10, 10, 500, 400, hTabControlMain, NULL, hInst, NULL );   

    // Did we get the editor window?
    if ( !::IsWindow( m_hwndEditor ) )
    {   MessageBox(hWndSettings, _T("Unable to create editor window"),_T("Uh oh"),NULL );
        return;
    } // end if

    // CPP lexer
    SendEditor( SCI_SETLEXER, SCLEX_LUA );

    // Set number of style bits to use
    SendEditor( SCI_SETSTYLEBITS, 5 );
    
    // Set tab width
    SendEditor( SCI_SETTABWIDTH, 4 );

	SendEditor( SCI_SETKEYWORDS, 0, (LPARAM)g_LuaKeyWords );
    SendEditor( SCI_SETKEYWORDS, 1, (LPARAM)g_actionKeyWords );

    // Set up the global default style. These attributes
    // are used wherever no explicit choices are made.
    SetAStyle( STYLE_DEFAULT, black, white, 12, _T("Courier New") );

    // Set caret foreground color
    SendEditor( SCI_SETCARETFORE, RGB( 0, 0, 0 ) );

    // Set all styles
    SendEditor( SCI_STYLECLEARALL );

    // Set selection color
    SendEditor( SCI_SETSELBACK, TRUE, RGB( 0, 0, 255 ) );
	SendEditor( SCI_SETSELFORE, TRUE, RGB( 255, 255, 255 ) );

	SendEditor( SCI_SETFOLDMARGINCOLOUR, TRUE, GetSysColor(COLOR_3DDKSHADOW) );
	//SendEditor( SCI_SETMARGINWIDTHN, 0, 48);
	//SendEditor( SCI_SETMARGINTYPEN, 0, 1);

	//set the zoom factor
	LRESULT lTemp = SendEditor( SCI_SETZOOM, iActionLuaZoomFactor );
    
    // Set syntax colors
    for ( long i = 0; g_rgbSyntaxLua[ i ].iItem != -1; i++ ) 
        SetAStyle( g_rgbSyntaxLua[ i ].iItem, g_rgbSyntaxLua[ i ].rgb, white );
}

//Init Scintilla editor (Global Lua tab)
void InitialiseLuaEditor() 
{
    // Punt if we already have a window
    if ( ::IsWindow( m_hwndLuaEditor ) ) return;

    // Create editor window
    m_hwndLuaEditor = CreateWindowEx(  0, _T("Scintilla"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 10, 10, 500, 400, hTabControlMain, NULL, hInst, NULL );   

    // Did we get the editor window?
    if ( !::IsWindow( m_hwndLuaEditor ) )
    {   MessageBox(hWndSettings, _T("Unable to create editor window"),_T("Uh oh"),NULL );
        return;
    } // end if

    // CPP lexer
    SendLuaEditor( SCI_SETLEXER, SCLEX_LUA );

    // Set number of style bits to use
    SendLuaEditor( SCI_SETSTYLEBITS, 5 );
    
    // Set tab width
    SendLuaEditor( SCI_SETTABWIDTH, 4 );

	SendLuaEditor( SCI_SETKEYWORDS, 0, (LPARAM)g_LuaKeyWords );
    SendLuaEditor( SCI_SETKEYWORDS, 1, (LPARAM)g_actionKeyWords );

    // Set up the global default style. These attributes
    // are used wherever no explicit choices are made.
    SetLuaAStyle( STYLE_DEFAULT, black, white, 12, _T("Courier New") );

    // Set caret foreground color
    SendLuaEditor( SCI_SETCARETFORE, RGB( 0, 0, 0 ) );

    // Set all styles
    SendLuaEditor( SCI_STYLECLEARALL );

    // Set selection color
    SendLuaEditor( SCI_SETSELBACK, TRUE, RGB( 0, 0, 255 ) );
	SendLuaEditor( SCI_SETSELFORE, TRUE, RGB( 255, 255, 255 ) );

	SendLuaEditor( SCI_SETFOLDMARGINCOLOUR, TRUE, GetSysColor(COLOR_3DDKSHADOW) );
	//SendLuaEditor( SCI_SETMARGINWIDTHN, 0, 48);
	//SendLuaEditor( SCI_SETMARGINTYPEN, 0, 1);
    
	//set the zoom factor
	SendLuaEditor( SCI_SETZOOM, iGlobalLuaZoomFactor );

    // Set syntax colors
    for ( long i = 0; g_rgbSyntaxLua[ i ].iItem != -1; i++ ) 
        SetLuaAStyle( g_rgbSyntaxLua[ i ].iItem, g_rgbSyntaxLua[ i ].rgb, white );
}

//Init Scintilla editor (Hotkey tab)
void InitialiseHotkeyEditor() 
{
    // Punt if we already have a window
    if ( ::IsWindow( m_hwndHotkeyEditor ) ) return;

    // Create editor window
    m_hwndHotkeyEditor = CreateWindowEx(  0, _T("Scintilla"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 10, 10, 500, 400, hTabControlMain, NULL, hInst, NULL );   

    // Did we get the editor window?
    if ( !::IsWindow( m_hwndHotkeyEditor ) )
    {   MessageBox(hWndSettings, _T("Unable to create editor window"),_T("Uh oh"),NULL );
        return;
    } // end if

    // CPP lexer
    SendHotkeyEditor( SCI_SETLEXER, SCLEX_LUA );

    // Set number of style bits to use
    SendHotkeyEditor( SCI_SETSTYLEBITS, 5 );
    
    // Set tab width
    SendHotkeyEditor( SCI_SETTABWIDTH, 4 );

	SendHotkeyEditor( SCI_SETKEYWORDS, 0, (LPARAM)g_LuaKeyWords );
    SendHotkeyEditor( SCI_SETKEYWORDS, 1, (LPARAM)g_actionKeyWords );

    // Set up the global default style. These attributes
    // are used wherever no explicit choices are made.
    SetHotkeyAStyle( STYLE_DEFAULT, black, white, 12, _T("Courier New") );

    // Set caret foreground color
    SendHotkeyEditor( SCI_SETCARETFORE, RGB( 0, 0, 0 ) );

    // Set all styles
    SendHotkeyEditor( SCI_STYLECLEARALL );

    // Set selection color
    SendHotkeyEditor( SCI_SETSELBACK, TRUE, RGB( 0, 0, 255 ) );
	SendHotkeyEditor( SCI_SETSELFORE, TRUE, RGB( 255, 255, 255 ) );

	SendHotkeyEditor( SCI_SETFOLDMARGINCOLOUR, TRUE, GetSysColor(COLOR_3DDKSHADOW) );
	//SendHotkeyEditor( SCI_SETMARGINWIDTHN, 0, 48);
	//SendHotkeyEditor( SCI_SETMARGINTYPEN, 0, 1);
    
	//set the zoom factor
	SendHotkeyEditor( SCI_SETZOOM, iHotkeyLuaZoomFactor );

    // Set syntax colors
    for ( long i = 0; g_rgbSyntaxLua[ i ].iItem != -1; i++ ) 
        SetHotkeyAStyle( g_rgbSyntaxLua[ i ].iItem, g_rgbSyntaxLua[ i ].rgb, white );
}

#pragma endregion Scintilla


#pragma region Mixer

//Used for XP mute/volume..I think, either that or this was used for > XP
class Mixer
{
public:
    /*************************************************************************/
    bool init()
    {
        _nNumMixers = mixerGetNumDevs();
        _hMixer = NULL;
        ZeroMemory(&_mxcaps, sizeof(MIXERCAPS));

        if(_nNumMixers != 0) 
        {
            if (mixerOpen(&_hMixer, 0, 0, NULL, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR) 
            {
                return FALSE;
            }

            if (mixerGetDevCaps((UINT)_hMixer, &_mxcaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR) 
            {
                return FALSE;
            }
        }

        return _hMixer != NULL;
    }

    /*************************************************************************/
    bool close()
    {
        return mixerClose(_hMixer) == MMSYSERR_NOERROR;
    }

    /*************************************************************************/
    bool GetMuteControl() 
    {
        MIXERLINE mixerLine;
        mixerLine.cbStruct = sizeof(MIXERLINE);
        mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

        if (mixerGetLineInfo((HMIXEROBJ)_hMixer,
                             &mixerLine,
                             MIXER_OBJECTF_HMIXER |
                             MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR) 
        {
            return FALSE;
        }

        MIXERCONTROL mixerControl;
        MIXERLINECONTROLS mixerLineControl;

        mixerLineControl.cbStruct         = sizeof(MIXERLINECONTROLS);
        mixerLineControl.dwLineID         = mixerLine.dwLineID;
        mixerLineControl.dwControlType    = MIXERCONTROL_CONTROLTYPE_MUTE;
        mixerLineControl.cControls        = 1;
        mixerLineControl.cbmxctrl         = sizeof(MIXERCONTROL);
        mixerLineControl.pamxctrl         = &mixerControl;

        if (mixerGetLineControls((HMIXEROBJ)_hMixer,
                                 &mixerLineControl,
                                 MIXER_OBJECTF_HMIXER |
                                 MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR) 
        {
            return FALSE;
        }

        _controlID = mixerControl.dwControlID;

        return TRUE;
    }

    /*************************************************************************/
    bool GetVolumeControl() 
    {
        MIXERLINE mixerLine;
        mixerLine.cbStruct = sizeof(MIXERLINE);
        mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

        if (mixerGetLineInfo((HMIXEROBJ)_hMixer,
                             &mixerLine,
                             MIXER_OBJECTF_HMIXER |
                             MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR) 
        {
            return FALSE;
        }

        MIXERCONTROL mixerControl;
        MIXERLINECONTROLS mixerLineControl;

        mixerLineControl.cbStruct         = sizeof(MIXERLINECONTROLS);
        mixerLineControl.dwLineID         = mixerLine.dwLineID;
        mixerLineControl.dwControlType    = MIXERCONTROL_CONTROLTYPE_VOLUME;
        mixerLineControl.cControls        = 1;
        mixerLineControl.cbmxctrl         = sizeof(MIXERCONTROL);
        mixerLineControl.pamxctrl         = &mixerControl;

        if (mixerGetLineControls((HMIXEROBJ)_hMixer,
                                 &mixerLineControl,
                                 MIXER_OBJECTF_HMIXER |
                                 MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR) 
        {
            return FALSE;
        }

        _controlID = mixerControl.dwControlID;

        return TRUE;
    }

    /*************************************************************************/
    bool GetMuteValue(LONG* value) 
    {

        MIXERCONTROLDETAILS_BOOLEAN mxcdMute;
        MIXERCONTROLDETAILS mixerControlDetails;

        mixerControlDetails.cbStruct          = sizeof(MIXERCONTROLDETAILS);
        mixerControlDetails.dwControlID       = _controlID;
        mixerControlDetails.cChannels         = 1;
        mixerControlDetails.cMultipleItems    = 0;
        mixerControlDetails.cbDetails         = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mixerControlDetails.paDetails         = &mxcdMute;

        if (mixerGetControlDetails((HMIXEROBJ)_hMixer,
                                     &mixerControlDetails,
                                     MIXER_OBJECTF_HMIXER
                                     | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
        {
            return FALSE;
        }

        *value = mxcdMute.fValue;
        return TRUE;
    }

    /*************************************************************************/
    int GetVolumeValue() 
    {

        tMIXERCONTROLDETAILS_SIGNED mxcdVolume;
        MIXERCONTROLDETAILS mixerControlDetails;

        mixerControlDetails.cbStruct          = sizeof(MIXERCONTROLDETAILS);
        mixerControlDetails.dwControlID       = _controlID;
        mixerControlDetails.cChannels         = 1;
        mixerControlDetails.cMultipleItems    = 0;
        mixerControlDetails.cbDetails         = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mixerControlDetails.paDetails         = &mxcdVolume;

        if (mixerGetControlDetails((HMIXEROBJ)_hMixer,
                                     &mixerControlDetails,
                                     MIXER_OBJECTF_HMIXER
                                     | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
        {
            return -1;
        }

		return mxcdVolume.lValue;
    }

    /*************************************************************************/
    bool SetMute( bool state) 
    {

        MIXERCONTROLDETAILS_BOOLEAN mxcdMute = { (LONG) state };
        MIXERCONTROLDETAILS mixerControlDetails;

        mixerControlDetails.cbStruct        = sizeof(MIXERCONTROLDETAILS);
        mixerControlDetails.dwControlID     = _controlID;
        mixerControlDetails.cChannels       = 1;
        mixerControlDetails.cMultipleItems  = 0;
        mixerControlDetails.cbDetails       = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
        mixerControlDetails.paDetails       = &mxcdMute;

        if (mixerSetControlDetails((HMIXEROBJ)_hMixer,
                                     &mixerControlDetails,
                                     MIXER_OBJECTF_HMIXER
                                     | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR) 
        {
            return FALSE;
        }
        return TRUE;
    }

    /*************************************************************************/
    bool SetVolume( int newVolume) 
    {
		MIXERCONTROLDETAILS mcd = {0};
		MIXERCONTROLDETAILS_UNSIGNED mcdu = {0};

		mcdu.dwValue = newVolume; // the volume is a number between 0 and 65535

		mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mcd.hwndOwner = 0;
		mcd.dwControlID = _controlID;
		mcd.paDetails = &mcdu;
		mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		mcd.cChannels = 1;
		if (mixerSetControlDetails((HMIXEROBJ) _hMixer, 
					   &mcd, MIXER_SETCONTROLDETAILSF_VALUE))
		{
			return FALSE;
		}
		return TRUE;
	}	


private:
    /*************************************************************************/
    UINT _nNumMixers;
    HMIXER _hMixer;
    MIXERCAPS _mxcaps;
    DWORD _controlID;
};

#pragma endregion Mixer


#pragma region DllMain

BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD  Reason, LPVOID Reserved)
{
 switch(Reason)
    { /* reason */
     case DLL_PROCESS_ATTACH:
	{
		LOGMESSAGE("In DLL_PROCESS_ATTACH");
		hInst = hInstance;
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000040);
		InitializeCriticalSectionAndSpinCount(&CriticalSectionHotkey, 0x00000040);
		InitializeCriticalSectionAndSpinCount(&WheelTickCriticalSection, 0x00000040);
		TCHAR drive[MAX_PATH] = {0};
		TCHAR dir[MAX_PATH] = {0};
		TCHAR fname[MAX_PATH] = {0};
		TCHAR ext[MAX_PATH] = {0};
		GetModuleFileName(NULL, strExePath, _MAX_PATH);
		std::basic_string<TCHAR> sCmdLine = GetCommandLine();
		boost::replace_all(sCmdLine, strExePath, "");
		boost::replace_all(sCmdLine, "\"", "");
		boost::trim(sCmdLine);
		_tsplitpath(strExePath, drive, dir, fname, ext);
		memset(strExePath,0,sizeof(strExePath));
		_tcscat(strExePath,drive);
		_tcscat(strExePath,dir);
		SetCurrentDirectory(strExePath);
		if(sCmdLine.length() > 0){
			_tcscpy(strStrokesConfigFile, sCmdLine.c_str());
		} else {
			_tcscpy(strStrokesConfigFile, _T("StrokesPlus.xml"));
			fstream tmpConfig(strStrokesConfigFile, ifstream::in | ifstream::out);
			if(!tmpConfig.good()){
				TCHAR szPath[MAX_PATH];
				ExpandEnvironmentStringsA("%appdata%",szPath,MAX_PATH);
				string sPath = szPath;
				sPath.append("\\StrokesPlus");
				CreateDirectory(sPath.c_str(), NULL);
				sPath.append("\\StrokesPlus.xml");
				_tcscpy(strStrokesConfigFile,sPath.c_str());
			}
			tmpConfig.close();
		}
		LOGMESSAGE("About to init DLL resources");
		InitialiseResources();
		
		LOGMESSAGE("About to register Scintilla");
		// Initialize the Scintilla
		if ( !Scintilla_RegisterClasses( hInstance ) )
		{   
			MessageBox(GetForegroundWindow(),_T("Scintilla failed to initiailze"),_T("Error"), NULL);
			return FALSE;
		} 
		
		m_nDelayNow = m_nDelayAlways = 0;
		CoInitialize(NULL);

		#ifdef SP_WIN8_CHECKS
			LOGMESSAGE("About to attempt to resolve IsImmersiveProcess");
			HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
			if(hUser32 != NULL){
				lpfnIsImmersiveProcessFunc = (IsImmersiveProcessFunc)GetProcAddress(hUser32,"IsImmersiveProcess");;
			}
		#endif

		#ifdef SP_MONITOR_BRIGHTNESS
			LOGMESSAGE("About to attempt to resolve monitor brightness functions");
			HINSTANCE hDxva2 = LoadLibrary(_T("Dxva2.dll"));
			if(hDxva2 != NULL){
				LOGMESSAGE(NULL, _T("hDxva2 != NULL"), _T(""), NULL);
				lpfnGetNumberOfPhysicalMonitorsFromHMONITOR = (GetNumberOfPhysicalMonitorsFromHMONITOR)GetProcAddress(hDxva2,"GetNumberOfPhysicalMonitorsFromHMONITOR");
				lpfnGetPhysicalMonitorsFromHMONITOR = (GetPhysicalMonitorsFromHMONITOR)GetProcAddress(hDxva2,"GetPhysicalMonitorsFromHMONITOR");
				lpfnGetMonitorBrightness = (GetMonitorBrightness)GetProcAddress(hDxva2,"GetMonitorBrightness");
				lpfnSetMonitorBrightness = (SetMonitorBrightness)GetProcAddress(hDxva2,"SetMonitorBrightness");
				lpfnDestroyPhysicalMonitors = (DestroyPhysicalMonitors)GetProcAddress(hDxva2,"DestroyPhysicalMonitors");
			} else {
				LOGMESSAGE(NULL, _T("hDxva2 == NULL"), _T(""), NULL);
			}
		#endif

		#ifdef SP_TOUCH

			//Dynamically bind to these touch-related functions so < Win8 still run
			if(hUser32 != NULL){
				lpfnRegisterTouchInput = (LPREGISTERTOUCHINPUT)GetProcAddress(hUser32,"RegisterPointerInputTarget");
				lpfnUnregisterTouchInput = (LPUNREGISTERTOUCHINPUT)GetProcAddress(hUser32,"UnregisterPointerInputTarget");
				lpfnInitializeTouchInjection = (LPINITTOUCHINJECTION)GetProcAddress(hUser32,"InitializeTouchInjection");
				lpfnInjectTouchInput = (LPINJECTTOUCHINPUT)GetProcAddress(hUser32,"InjectTouchInput");
				if(lpfnRegisterTouchInput==NULL || lpfnUnregisterTouchInput==NULL || lpfnInitializeTouchInjection==NULL || lpfnInjectTouchInput==NULL){
					//Couldn't get proc address of one or more touch-related functions
					//disable touch-related functionality
					bTouchAvailable = false;
				} else {
					bTouchAvailable = true;
				}
			} else {
				//Couldn't get the reference to User32.dll .. should never happen
				bTouchEnabled = false;
				bPenEnabled = false;
				bTouchAvailable = false;
			}

		#endif

		ptPreviewTL.x = 65536;
		return TRUE;
	}
     case DLL_PROCESS_DETACH:
		 LOGMESSAGE("In DLL_PROCESS_DETACH");
		// Close the LUA states
		lua_close(L);
		lua_close(L2);
		CoUninitialize();
		 if(hWndServer != NULL)
		{
			clearMyHook(hWndServer);
			return TRUE;
		}
		 UninitialiseResources();
		// Release Scintilla
		Scintilla_ReleaseResources();
		for(int i = 0; i < 14; i++) { 
			DestroyCursor(curBlank[i]);
		}
		DeleteCriticalSection(&CriticalSection);
		DeleteCriticalSection(&CriticalSectionHotkey);
		DeleteCriticalSection(&WheelTickCriticalSection);
    } /* reason */
    return TRUE;
}

#pragma endregion DllMain


#pragma region Graphic Functions

void ScaleGesture(BOOL bUIPoints)
{
	int Width = 125;
	int Height = 125;

	ScaledDrawPoints.clear();

	// Get total width and height of gesture
	float fGestureOffsetLeft = 0; 
	float fGestureOffsetTop = 0; 
	float fGestureWidth = 0;
	float fGestureHeight = 0; 

	if(bUIPoints){
		fGestureOffsetLeft = g_Points[0].x;
		fGestureOffsetTop = g_Points[0].y;
		fGestureWidth = g_Points[0].x;
		fGestureHeight = g_Points[0].y;
		for(int i = 0; i < g_Points.size(); i++) { 
			if(g_Points[i].x <= fGestureOffsetLeft)
				fGestureOffsetLeft = g_Points[i].x;
			if(g_Points[i].y <= fGestureOffsetTop)
				fGestureOffsetTop = g_Points[i].y;
			if(g_Points[i].x >= fGestureWidth)
				fGestureWidth = g_Points[i].x;
			if(g_Points[i].y >= fGestureHeight)
				fGestureHeight = g_Points[i].y;
		}
	} else {
		fGestureOffsetLeft = LastDrawPoints[0].x;
		fGestureOffsetTop = LastDrawPoints[0].y;
		fGestureWidth = LastDrawPoints[0].x;
		fGestureHeight = LastDrawPoints[0].y;
		for(int i = 0; i < LastDrawPoints.size(); i++) { 
			if(LastDrawPoints[i].x <= fGestureOffsetLeft)
				fGestureOffsetLeft = LastDrawPoints[i].x;
			if(LastDrawPoints[i].y <= fGestureOffsetTop)
				fGestureOffsetTop = LastDrawPoints[i].y;
			if(LastDrawPoints[i].x >= fGestureWidth)
				fGestureWidth = LastDrawPoints[i].x;
			if(LastDrawPoints[i].y >= fGestureHeight)
				fGestureHeight = LastDrawPoints[i].y;
		}
	}

	fGestureWidth -= fGestureOffsetLeft;
	fGestureHeight -= fGestureOffsetTop;

	// Get each scale ratio
	double dScaleX = (double)Width / fGestureWidth;
	double dScaleY = (double)Height / fGestureHeight;
	double dScaleVar = 0;
	POINTF tmpPoint;

	// Scale on the longest axis
	if (fGestureWidth >= fGestureHeight)
	{
		// Scale on X axis
		dScaleVar = dScaleX;
	}
	else
	{
		dScaleVar = dScaleY;
	}

	if(bUIPoints){
		for(int i = 0; i < g_Points.size(); i++) { 
			tmpPoint.x = (float)((g_Points[i].x - fGestureOffsetLeft) * dScaleVar);
			tmpPoint.y = (float)((g_Points[i].y - fGestureOffsetTop) * dScaleVar);
			ScaledDrawPoints.push_back(tmpPoint);
		}
	} else {
		for(int i = 0; i < LastDrawPoints.size(); i++) { 
			tmpPoint.x = (float)((LastDrawPoints[i].x - fGestureOffsetLeft) * dScaleVar);
			tmpPoint.y = (float)((LastDrawPoints[i].y - fGestureOffsetTop) * dScaleVar);
			ScaledDrawPoints.push_back(tmpPoint);
		}
	}

	// Calculate new gesture width and height
	iScaledWidth = (int)floor(fGestureWidth * dScaleVar);
	iScaledHeight = (int)floor(fGestureHeight * dScaleVar);
}

#pragma endregion Graphic Functions


#pragma region App Exports

//These are functions which are exposed to StrokesPlus.cpp (EXE) 
//Some functions were created when the EXE held all of the dialogs and are no longer really necessary, but I haven't cleaned that code up yet

void DisplayLastError(LPCSTR name) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)"") + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s error %d: %s"), 
		name, dw, lpMsgBuf); 


	string sTemp = _T("");
	MessageBox(GetForegroundWindow(), (LPCTSTR)lpDisplayBuf
		,_T("Error")
		,  MB_ICONWARNING + MB_APPLMODAL);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

#ifdef SP_NOGLOBALACTIONS
	__declspec(dllexport) bool getOnlyDefinedApps(){
		return bOnlyAllowDefinedApps;
	}
#endif

__declspec(dllexport) int getResumeDelay(){
	return iResumeDelay;
}
 
__declspec(dllexport) int getOSVersion(){
	return OSVersion;
}

__declspec(dllexport) void ClearHotkeys(){
	iHotkey = 1;

	for(int i = 0; i < HotkeyIDs.size(); i++) { 
		if(HotkeyIDs[i]>0){
			UnregisterHotKey(hWndServer, HotkeyIDs[i]);
		}
	}

	HotkeyIDs.clear();
}

__declspec(dllexport) void LoadHotkeys(){

	ClearHotkeys();
	LoadHotkeyVectors();

	//loop through all actions and register hotkeys
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v5, ptConfig.get_child("config.Global.Hotkeys")) {
			if(v5.second.get("<xmlattr>.enabled",0) == 1){

				int iMod1 = v5.second.get("Modifier1",0);
				int iMod2 = v5.second.get("Modifier2",0);
				int iMod3 = v5.second.get("Modifier3",0);
				int iMod4 = v5.second.get("Modifier4",0);
				int iKey = v5.second.get("Key",0);

				int iModifiers = 0;

				iModifiers = iModifiers | (iMod1 == 1 ? 0x0002 : (iMod1 == 2 ? 0x0001 : (iMod1 == 3 ? 0x0004 : (iMod1 == 4 ? 0x0008 : 0)))) | (iMod2 == 1 ? 0x0002 : (iMod2 == 2 ? 0x0001 : (iMod2 == 3 ? 0x0004 : (iMod2 == 4 ? 0x0008 : 0)))) | (iMod3 == 1 ? 0x0002 : (iMod3 == 2 ? 0x0001 : (iMod3 == 3 ? 0x0004 : (iMod3 == 4 ? 0x0008 : 0)))) | (iMod4 == 1 ? 0x0002 : (iMod4 == 2 ? 0x0001 : (iMod4 == 3 ? 0x0004 : (iMod4 == 4 ? 0x0008 : 0))));

				v5.second.put("HotkeyID", 0);

				if(iKey>0){
				
					if(OSVersion>=7) iModifiers = iModifiers | MOD_NOREPEAT;
					int iHotkeyID = RegisterHotKey(hWndServer, iHotkey, iModifiers, HotkeyValues[iKey]);
					if(iHotkeyID==0){
						DisplayLastError(_T("RegisterHotKey"));
					} else {
						if(iHotkey == INT_MAX) {
							MessageBox(GetForegroundWindow(), "Maximum hotkey identifier limit reached. Please restart StrokesPlus.", "Lame", MB_ICONWARNING + MB_APPLMODAL);
						} else {

							v5.second.put("HotkeyID", iHotkey);
							HotkeyIDs.push_back(iHotkey);
							iHotkey++;
						}
					}
				}
			}
		//}
	}
	HotkeyNames.clear();
	HotkeyValues.clear();
}

__declspec(dllexport) BOOL fireTrimTimer()
{
		trimtimerid = SetTimer(hWndServer, WM_IDT_TRIM, TrimDelay, (TIMERPROC) TrimTimerProc);
		return TRUE;
}

__declspec(dllexport) BOOL getCheckForOtherGesturePrograms(){
	return bCheckForOtherGesturePrograms;
}

__declspec(dllexport) void setWindowTransparency()
{
	SetLayeredWindowAttributes(hWndServer, RGB(255,255,255), (BYTE)opacity, LWA_COLORKEY | LWA_ALPHA);
	setWindowState();
}

__declspec(dllexport) void setDPIModifier(float val)
{
	fDPIModifier = val;
}

__declspec(dllexport) void setTrayID(int val)
{
	trayID = val;
}

__declspec(dllexport) bool getHideAdditionalInstanceMessage()
{
	return bHideAdditionalInstanceMessage;
}

__declspec(dllexport) bool getTrayIconVisible()
{
	return bTrayIconVisible;
}

__declspec(dllexport) void setTrayIconVisible(bool val)
{
	bTrayIconVisible = val;
}

__declspec(dllexport) bool getGesturesDisabled()
{
	return bGesturesDisabled;
}

__declspec(dllexport) void setGesturesDisabled(bool val)
{
	bGesturesDisabled = val;
	#ifdef SP_TOUCH
		if((isTouchEnabled() || isPenEnabled()) && isTouchAvailable()){
			if(bGesturesDisabled){
				DestroyTouchWindow(hWndServer);
			} else {
				InitTouchWindow(hWndServer);
			}
		}		
	#endif
}

__declspec(dllexport) void sethWndServer(HWND val)
{
	//Sets the handle to the gesture draw window, called from StrokesPlus.cpp
	hWndServer = val;
}

__declspec(dllexport) void setLastActive()
{
	HWND hTemp = hWndServer;
	while(hTemp != NULL)
	{
		hTemp = GetNextWindow(hTemp, GW_HWNDNEXT);

		#ifdef SP_WIN8_CHECKS
			bool bIsImmersive = false;
			if(lpfnIsImmersiveProcessFunc){
				DWORD processID = 0;
				GetWindowThreadProcessId((HWND)hTemp, &processID);
				HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
				bIsImmersive = lpfnIsImmersiveProcessFunc(hProcess);
				CloseHandle(hProcess);	
			}		
		#endif
		
		if ( ( GetWindowLong(hTemp,GWL_STYLE) & WS_VISIBLE ) 
				&& ( GetWindow(hTemp,GW_OWNER) == NULL) 
				&& ( IsWindowEnabled(hTemp)==TRUE ) 
				&& ( hTemp != hWndServer ) 
				&& ( hTemp != hWndDisplayText ) 
				&& ( hTemp != hWndSettings ) 
				&& ( (GetWindowLong(hTemp,GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0 ) 
				#ifdef SP_WIN8_CHECKS
					&& ( !bIsImmersive )  //this is Windows 8 specific, seems to be the case for all Metro apps..this condition is saying to NOT count them) 
				#endif
					)
		{ 
			#ifdef SP_WIN8_CHECKS
				GetClassName(hTemp,gszClassName,sizeof(gszClassName));
				if(_tcsicmp(gszClassName, _T("Shell_CharmWindow")) != 0){
					SetForegroundWindow(hTemp);
				}
			#else
				SetForegroundWindow(hTemp);
			#endif
			break;
		}
	}
}

__declspec(dllexport) void setPreviousTrainingModeState(bool val)
{
	bPreviousTrainingModeState= val;
}

__declspec(dllexport) bool getReInitOnResume()
{
	return bReInitOnResume;
}

__declspec(dllexport) bool getCheckForegroundTimeout()
{
	return bCheckForegroundTimeout;
}

__declspec(dllexport) bool getShowCopyData()
{
	return bShowCopyData;
}

DWORD WINAPI showSettings( LPVOID lpParam ){
	DialogBox(hInst,MAKEINTRESOURCE(IDD_DLGSETTINGS), NULL,SettingsProc);
	return 1;
}

__declspec(dllexport) void openPassword()
{
	if(bPasswordOpen || bSettingsOpen){
		return;
	}
	if(sPassword.length() > 0)
	{
		SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGCHGPASSWORD), hWndServer, ChangePasswordProc);
		setWindowState();
	} else {
		bCheckPassword = false;
		bSetPassword = true;
		SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGPASSWORD), hWndServer, PasswordProc);
		setWindowState();
	}
}

__declspec(dllexport) void openSettings()
{
	if(bPasswordOpen){
		return;
	}
	if(!bSettingsOpen){
		bool bContinue = false;
		bPasswordMatch = false;
		if(sPassword.length() > 0)
		{
			bCheckPassword = true;
			bSetPassword = false;
			SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGPASSWORD), hWndServer, PasswordProc);
			setWindowState();
			bContinue = bPasswordMatch;
		} else {
			bContinue = true;
		}
		if(bContinue){
			HANDLE hThread = CreateThread(NULL, 0, &showSettings, NULL, 0, NULL); 
			CloseHandle(hThread);
		}
	} else {
		SetForegroundWindow(hWndSettings);
		TabCtrl_SetCurSel(hTabControlMain,iSelectedTab);
		setSelectedTab(true);
		InvalidateRect(hWndSettings,NULL,true);
		UpdateWindow(hWndSettings);
	}
}

__declspec(dllexport) void openAbout()
{
	iSelectedTab = 6;
	openSettings();
}

__declspec(dllexport) void openPrefs()
{
	iSelectedTab = 3;
	openSettings();
}

__declspec(dllexport) void openConfig()
{
	iSelectedTab = 0;
	openSettings();
}

__declspec(dllexport) void openHotkeys()
{
	iSelectedTab = 1;
	openSettings();
}

__declspec(dllexport) void openIgnored()
{
	iSelectedTab = 2;
	openSettings();
}

__declspec(dllexport) void openGestureName()
{
	if(!bGestureOpen) {
		g_GestureNameShown = FALSE;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGGESTURENAME), hWndServer, GestureNameProc);
	}
}

__declspec(dllexport) void openHelp()
{
	SetCurrentDirectory(strExePath);
	ShellExecute(hWndServer, _T("open"),
			_T("StrokesPlus.html"),
			NULL, NULL, SW_SHOWNORMAL);
}

__declspec(dllexport) BOOL drawLastGestureToHandle(HWND hWndForm, int left, int top, int iLeftOffset, int iTopOffset, BOOL bUIPoints)
	{

		ScaleGesture(bUIPoints);

		if(iLeftOffset == -1){
			iLeftOffset = (int)floor((double)(74-(iScaledWidth / 2)));
			iTopOffset = (int)floor((double)(74-(iScaledHeight / 2)));
		} else {
			iLeftOffset = (int)floor((double)((left+64)-(iScaledWidth / 2)));
			iTopOffset = (int)floor((double)((top+64)-(iScaledHeight / 2)));
		}

		Graphics graphics(hWndForm);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.SetCompositingQuality ( CompositingQualityHighQuality ) ;
		graphics.SetInterpolationMode( InterpolationModeHighQuality ) ; 
		PointF *arrPoints = new PointF[ScaledDrawPoints.size()];
		
		for(int i = 0; i < ScaledDrawPoints.size(); i++) { 
			arrPoints[i].X = ScaledDrawPoints[i].x+15+iLeftOffset;
			arrPoints[i].Y = ScaledDrawPoints[i].y+15+iTopOffset;
		}

		Pen bluePen(Color(255, 0,0,255), 5);
		bluePen.SetLineCap(LineCapRoundAnchor, LineCapArrowAnchor, DashCapRound);
		graphics.DrawLines(&bluePen, arrPoints, ScaledDrawPoints.size());

		delete[] arrPoints;

		return TRUE;
	}

__declspec(dllexport) BOOL saveGesture(TCHAR *string)
	{
		size_t len;
		size_t maxsize = 100;
		len = _tcsnlen(string, maxsize);

		if(len > 0) {	

			std::basic_string<TCHAR> sName = string; 
			boost::replace_all(sName, _T("\""), _T(" "));

			//======== save to config
			if(LastDrawPoints.size() > 1)
			{
				using boost::property_tree::ptree;
				ptree child;
				ptree points;
				BOOL bExistingMatchFound = FALSE;

				BOOST_FOREACH(ptree::value_type &v2, ptConfig.get_child("config.Gestures")) {
					//each point pattern
					//Found a match, add new point pattern
					if( _tcscmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 ) {
						BOOST_FOREACH(ptree::value_type &v3, v2.second) {
						//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
							if(v3.first == _T("PointPatterns")){
								for(int i = 0;i<LastDrawPoints.size();i++)
								{
									points.put("<xmlattr>.x",LastDrawPoints[i].x);
									points.put("<xmlattr>.y",LastDrawPoints[i].y);
									//Add <Point x="X" y="Y" /> node to new <Gesture name="NAME"> node
									child.add_child("Point", points);
								}
								v3.second.add_child("PointPattern",child);
								bExistingMatchFound = TRUE;
							}
						}
					}
				}

				//New gesture
				if(!bExistingMatchFound) {
					//set main gesture node: <Gesture name="NAME">
					child.put("<xmlattr>.name",sName.c_str());
					for(int i = 0;i<LastDrawPoints.size();i++)
					{
						points.put("<xmlattr>.x",LastDrawPoints[i].x);
						points.put("<xmlattr>.y",LastDrawPoints[i].y);
						//Add <Point x="X" y="Y" /> node to new <Gesture name="NAME"> node
						child.add_child("PointPatterns.PointPattern.Point", points);
					}
			
					//Save new <Gesture name="NAME"><Points...> node to config
					ptConfig.add_child("config.Gestures.Gesture", child); 
				}
				if(!bNewGestureMode){
					saveConfig();
				}
			}
		}
		clearCaptureVars();
		
		return TRUE;
	}

__declspec(dllexport) BOOL setHookBtn(int btn)
    {
       hookbtn = btn;
	   if(IsWindowVisible(hWndConfig)){
		   SetModifierStates(hWndConfig);
	   }
	   if(IsWindowVisible(hWndSettings)){
		   setPrefStrokeButton();
	   }
	   return TRUE;
    }

__declspec(dllexport) int getHookBtn()
    {
       return hookbtn;
    }

__declspec(dllexport) BOOL setCancel(BOOL val)
    {
       bGlobalCancel = val;
	   return TRUE;
    }

__declspec(dllexport) BOOL getCancel()
    {
       return bGlobalCancel;
    }

__declspec(dllexport) BOOL setMatchProbabilityThreshold(int val)
    {
       MatchProbabilityThreshold= val;
	   return TRUE;
    }

__declspec(dllexport) int getMatchProbabilityThreshold()
    {
       return MatchProbabilityThreshold;
    }

__declspec(dllexport) BOOL setMatchPrecision(int val)
    {
       iPrecision= val;
	   return TRUE;
    }

__declspec(dllexport) int getMatchPrecision()
    {
       return iPrecision;
    }

__declspec(dllexport) BOOL setLearningMode(BOOL val)
    {
       bLearningMode = val;
	   return TRUE;
    }

__declspec(dllexport) BOOL getLearningMode()
    {
	   return bLearningMode;
    }

__declspec(dllexport) BOOL setMaxDrawPoints(int val)
    {
       iMaxDrawPoints = val;
	   if(iMaxDrawPoints < 0) iMaxDrawPoints = 0;
	   if(iMaxDrawPoints > 0 && iMaxDrawPoints < 3) iMaxDrawPoints = 3;
	   return TRUE;
    }

__declspec(dllexport) int getMaxDrawPoints()
    {
       return iMaxDrawPoints;
    }

__declspec(dllexport) BOOL setDrawGesture(BOOL val)
    {
       bDrawGesture = val;
	   setWindowState();
	   return TRUE;
    }

__declspec(dllexport) BOOL getDrawGesture()
    {
       return bDrawGesture;
    }

__declspec(dllexport) BOOL setDrawColor(COLORREF rgb)
    {
	   PenColor = rgb;
	   DeleteObject(hDrawPen);
	   if(GetRValue(rgb) == 255 && GetGValue(rgb) == 255 && GetBValue(rgb) == 255)
		   PenColor = RGB(255,255,254);
	   hDrawPen = CreatePen(PS_SOLID, PenWidth, PenColor);
	   return TRUE;
    }

__declspec(dllexport) int getDrawColorR()
    {
		return (int)GetRValue(PenColor); 
    }

__declspec(dllexport) int getDrawColorG()
    {
		return (int)GetGValue(PenColor); 
    }

__declspec(dllexport) int getDrawColorB()
    {
		return (int)GetBValue(PenColor); 
    }

__declspec(dllexport) BOOL setDrawWidth(int width)
    {
	   PenWidth = width;
	   DeleteObject(hDrawPen);
	   DeleteObject(hBackgroundPen);
	   hDrawPen = CreatePen(PS_SOLID, PenWidth, PenColor);
	   hBackgroundPen = CreatePen(PS_SOLID, PenWidth, RGB(255,255,255));
	   return TRUE;
    }

__declspec(dllexport) int getDrawWidth()
    {
       return PenWidth;
    }

__declspec(dllexport) BOOL setIgnoreKey(int key)
	{
		IgnoreKey = key;
		return TRUE;
	}

__declspec(dllexport) int getIgnoreKey()
	{
		return IgnoreKey;
	}

__declspec(dllexport) BOOL setAggressiveTrim(BOOL val)
	{
		bAggressiveTrim = val;
		return TRUE;
	}

__declspec(dllexport) BOOL getAggressiveTrim()
	{
		return bAggressiveTrim;
	}

__declspec(dllexport) BOOL setCancelDelay(int delay)
	{
		CancelDelay = delay;
		return TRUE;
	}

__declspec(dllexport) int getCancelDelay()
	{
		return CancelDelay;
	}

__declspec(dllexport) BOOL setTrimDelay(int delay)
	{
		TrimDelay = delay;
		return TRUE;
	}

__declspec(dllexport) int getTrimDelay()
	{
		return TrimDelay;
	}

__declspec(dllexport) BOOL setMyHook(HWND hWnd)
    {
     if(bHooked)
		return FALSE; // already hooked!
     mousehook = SetWindowsHookEx(WH_MOUSE_LL,  // WH_GETMESSAGE
			    (HOOKPROC)MouseProc,
			    hInst,
			    0);
     if(mousehook != NULL)
	{ /* success */
	 hWndServer = hWnd;
	 bCancel = FALSE;
	 bMouseDown = FALSE;
	 bHooked = true;

	iXVIRTUALSCREEN = GetSystemMetrics(SM_XVIRTUALSCREEN);
	iYVIRTUALSCREEN = GetSystemMetrics(SM_YVIRTUALSCREEN);
	iCYVIRTUALSCREEN = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	iCXVIRTUALSCREEN = GetSystemMetrics(SM_CXVIRTUALSCREEN);

	 GetWindowRect(hWndServer,&rcWindow);

	 keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL,  // WH_GETMESSAGE
			    (HOOKPROC)KeyboardProc,
			    hInst,
			    0);
	 TCHAR buffer2[256];
wsprintf(buffer2, _T("keyboardhook: %d"),keyboardhook);
LOGMESSAGE(buffer2);
     if(keyboardhook != NULL)
	 {
		setWindowState();
		if(bAggressiveTrim) {
			SetProcessWorkingSetSize(GetCurrentProcess(), 0xffffffff, 0xffffffff); 
		}
		mousehidetimerid = SetTimer(hWndServer, WM_IDT_MOUSEHIDE, 100, (TIMERPROC) MouseHideTimerProc);
		if(IsWindow(hWndServer)){
			LoadHotkeys();
		}
		g_hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,  NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.

		return TRUE;
	 }
	 else {
		return FALSE;
	 }
	} /* success */
     return FALSE; // failed to set hook
    } // setMyHook

__declspec(dllexport) BOOL clearMyHook(HWND hWnd)
    {
	 LOGMESSAGE("Enter clearMyHook");
     if(hWnd != hWndServer || hWnd == NULL)
	return FALSE;
     BOOL unhooked = UnhookWindowsHookEx(mousehook);
	 unhooked = UnhookWindowsHookEx(keyboardhook);
	 unhooked = UnhookWinEvent(g_hook);
     /*if(unhooked)
		if(bAggressiveTrim) {
			KillTimer(NULL, trimtimerid);
		}*/
	bHooked = false;
	KillTimer(NULL,mousehidetimerid);
	acShowMouseCursor(L);
	acShowMouseCursor(L2);  
	ClearHotkeys();

     return unhooked;
    } // clearMyHook

__declspec(dllexport) BOOL setOSVersion()
{
	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
	typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
    PGNSI pGNSI;
    //PGPI pGPI;
    BOOL bOsVersionInfoEx;
    //DWORD dwType;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);

    // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

    pGNSI = (PGNSI) GetProcAddress(
	  GetModuleHandle(TEXT("kernel32.dll")), 
	  "GetNativeSystemInfo");
    if(NULL != pGNSI)
	   pGNSI(&si);
    else GetSystemInfo(&si);

    if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
    {
	  // Test for the specific product.

	   if ( osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 2 )
	   {
		 //win8
		  //MessageBox(NULL,_T("Win8",_T("8"),MB_OK);
		  OSVersion = 8;
		  return TRUE;
	   }

	   if ( osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 1 )
	   {
		 //win7
		  //MessageBox(NULL,_T("Win7",_T("7"),MB_OK);
		  OSVersion = 7;
		  return TRUE;
	   }
      
	   if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
	   {
		//vista
		  //MessageBox(NULL,_T("Vista",_T("7"),MB_OK);
			OSVersion = 6;
			return TRUE;
	   }

	   if ( osvi.dwMajorVersion == 5 )
	   {
		//xp
		  //MessageBox(NULL,_T("XP",_T("7"),MB_OK);
		   OSVersion = 5; //not sure why, but I changed XP to 6..
		   return TRUE;
	   }

	   if ( osvi.dwMajorVersion < 5 )
	   {
		//xp
		   MessageBox(GetForegroundWindow(),_T("Yeah, I don't think so.  XP or better, please  :-)"),_T("Very Cute."),MB_OK | MB_SETFOREGROUND );
		   //OSVersion = 5;
		   return FALSE;
	   }
    }
	else
	{
		   MessageBox(GetForegroundWindow(),_T("Yeah, I don't think so.  XP or better, please  :-)"),_T("Very Cute."),MB_OK | MB_SETFOREGROUND );
		   //OSVersion = 5;
		   return FALSE;		
	}
	return TRUE;
}

DWORD WINAPI CreateOSDDialog( LPVOID lpParam ){

	DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGDISPLAYTEXT), hWndServer, DisplayTextProc);
	return 0;
}

__declspec(dllexport) BOOL setWindowState()
{
	LOGMESSAGE("Enter setWindowState");
	if(bDrawGesture)
	{
		if((bMouseDown && DrawPoints.size() > 0) || (bDrawWindowAlwaysOnTop))
		{
			SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}
		else
		{
			if(!bOSDDisplayed && !bPasswordOpen){
				if(bDrawWindowToBottom){
					SetWindowPos(hWndServer, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
				} else {
					//Hide window
					ShowWindow(hWndServer, SW_HIDE);
				}
			}
		}
	} else {
		if(!bOSDDisplayed && !bPasswordOpen){
			ShowWindow(hWndServer, SW_HIDE);
		}
	}		

	return TRUE;
}

__declspec(dllexport) BOOL reloadLuaState(){
	LOGMESSAGE("Enter reloadLuaState");
	while(bLuaState1Firing || bLuaState2Firing){ 
		Sleep(25);
	}
	EnterCriticalSection(&CriticalSection);
	lua_close(L);
	lua_close(L2);
	initLua();
	LeaveCriticalSection(&CriticalSection);
	return TRUE;
}

__declspec(dllexport) void popupMenu(){
	
	int menuID = 1;
	HMENU hPopupMenuAction;
	hPopupMenuAction = CreatePopupMenu();

	if(menuDelim.length()==0){
		//No delim
		InsertMenu(hPopupMenuAction, 0, MF_BYPOSITION | MF_STRING, menuID, menuItems.c_str());
	} else {
		std::vector<std::string> strs;
		boost::split(strs, menuItems, boost::is_any_of(menuDelim),boost::algorithm::token_compress_on);
		for(int i=0; i < strs.size(); i++){
			if(strs[i] == "-") {
				InsertMenu(hPopupMenuAction, -1, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
			} else {
				InsertMenu(hPopupMenuAction, -1, MF_BYPOSITION | MF_STRING, menuID, strs[i].c_str());
				menuID++;				
			}
		}
	}

	popupMenuItemID = TrackPopupMenu(hPopupMenuAction, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD | TPM_NONOTIFY , popx, popy, 0, hWndServer, NULL);

	if(popupMenuItemID > 0)
	{
		HANDLE hThread = CreateThread(NULL, 0, &gPopupMenuClick, NULL, 0, NULL); 
		CloseHandle(hThread);
	} else {
		/*
		popupMenuItemID = 0;
		popx = 0;
		popy = 0;
		menuItems = "";
		menuDelim = "";
		popupMenuFunctionName = "";
		*/
	}
}

__declspec(dllexport) void disableHotkey(int hkid){
	EnterCriticalSection(&CriticalSectionHotkey);
	for(int i = 0; i < HotkeyIDs.size(); i++) { 
		if(HotkeyIDs[i]==hkid){
			UnregisterHotKey(hWndServer, HotkeyIDs[i]);
		}
	}
	LeaveCriticalSection(&CriticalSectionHotkey);
}

__declspec(dllexport) void enableHotkey(int hkid){

	EnterCriticalSection(&CriticalSectionHotkey);

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v5, ptConfig.get_child("config.Global.Hotkeys")) {
		if(v5.second.get("HotkeyID",0) == hkid){

			int iMod1 = v5.second.get("Modifier1",0);
			int iMod2 = v5.second.get("Modifier2",0);
			int iMod3 = v5.second.get("Modifier3",0);
			int iMod4 = v5.second.get("Modifier4",0);
			int iKey = v5.second.get("Key",0);

			int iModifiers = 0;

			iModifiers = iModifiers | (iMod1 == 1 ? 0x0002 : (iMod1 == 2 ? 0x0001 : (iMod1 == 3 ? 0x0004 : (iMod1 == 4 ? 0x0008 : 0)))) | (iMod2 == 1 ? 0x0002 : (iMod2 == 2 ? 0x0001 : (iMod2 == 3 ? 0x0004 : (iMod2 == 4 ? 0x0008 : 0)))) | (iMod3 == 1 ? 0x0002 : (iMod3 == 2 ? 0x0001 : (iMod3 == 3 ? 0x0004 : (iMod3 == 4 ? 0x0008 : 0)))) | (iMod4 == 1 ? 0x0002 : (iMod4 == 2 ? 0x0001 : (iMod4 == 3 ? 0x0004 : (iMod4 == 4 ? 0x0008 : 0))));

			if(iKey>0){
				
				if(OSVersion>=7) iModifiers = iModifiers | MOD_NOREPEAT;
				int iHotkeyID = RegisterHotKey(hWndServer, hkid, iModifiers, HotkeyValues[iKey]);
				if(iHotkeyID==0){
					DisplayLastError(_T("RegisterHotKey"));
				}
			}
		}
	}

	LeaveCriticalSection(&CriticalSectionHotkey);
	
}

#pragma endregion App Exports


#pragma region Action Functions

static int acSetNumber(lua_State *L) {
	iSetNumber = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	return 1;
}

static int acGetNumber(lua_State *L) {
	lua_pushnumber(L, iSetNumber);
	return 1;
}

static int acDisableHotkey(lua_State *L){
	int hkid = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_DISABLEHOTKEY, 1), MAKELPARAM(hkid,1));
	return 1;
}

static int acEnableHotkey(lua_State *L){
	int hkid = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_ENABLEHOTKEY, 1), MAKELPARAM(hkid,1));
	return 1;
}

static int acCreatePopupMenu(lua_State *L){
	popx = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	popy = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	menuItems = luaL_checkstring(L,3);
	menuDelim = luaL_checkstring(L,4);
	popupMenuFunctionName = luaL_checkstring(L,5);
	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_MENUPOPUP, 1), 0);
	return 1;
}


static int acEnableCapture(lua_State *L){
	bGlobalUserCancel = false;
	lua_pushnumber(L,1);
	return 1;
}

static int acDisableCapture(lua_State *L){
	bGlobalUserCancel = true;
	lua_pushnumber(L,1);
	return 1;
}


static int acIsImmersiveProcess(lua_State *L){

	#ifdef SP_WIN8_CHECKS
		int hWndTarget = NULL;
		POINT start;
		hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
		start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
		start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
		bool bIsImmersive = false;
	
		if(lpfnIsImmersiveProcessFunc){
			if(hWndTarget == NULL){
				hWndTarget = (int)WindowFromPoint(start);
			}
			DWORD processID = 0;
			GetWindowThreadProcessId((HWND)hWndTarget, &processID);
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
			bIsImmersive = lpfnIsImmersiveProcessFunc(hProcess);
			CloseHandle(hProcess);	
		}	
	
		lua_pushnumber(L, (int)bIsImmersive);
	#endif	

	return 1;
}

static int acGetAllWindows(lua_State *L){
	GetAllWindowsList.clear();
	EnumWindows(EnumWindowsProc, (lua_isnumber(L,1) ? (lua_tonumber(L, 1) == 1 ? 11 : 10) : 10));	
	lua_createtable(L, GetAllWindowsList.size(), 0);
	for (int i=0; i<GetAllWindowsList.size(); i++) {
	  lua_pushinteger(L, (int)GetAllWindowsList[i]);
	  lua_rawseti (L, -2, i);
	}
	lua_setglobal(L,"sp_all_windows");
	GetAllWindowsList.clear();
	return 1;
}


static int acKillDisplayText(lua_State *L){
	sDisplayTextMessage = _T("");
	KillTimer(NULL,displaytexttimerid);
	DisplayTextTimerProc(hWndDisplayText, NULL, NULL, NULL);
	return 1;
}

static int acActivateWindowUnderMouseWheel(lua_State *L) {
	bActivateMouseWheelWindow = (lua_isnumber(L,1) ? (BOOL)lua_tonumber(L, 1) : false);
	return 1;
}

static int acInputBox(lua_State *L) 
{
	if(!bInputBoxOpen) {
		sInputBoxResult = "";
		sInputBoxDelim = "";
		sInputBoxTitle = luaL_checkstring(L,1); 
		sInputBoxLabel = luaL_checkstring(L,2); 
		sInputBoxResult = luaL_checkstring(L,3); 
		sInputBoxDelim = (lua_isstring(L,4) ? lua_tostring(L,4) : _T(""));
		//hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGINPUTBOX), NULL, InputBoxProc);
		lua_pushstring(L, sInputBoxResult.c_str());
	}
	return 1;
}

static int acSendString(lua_State *L)
{
	int hWndTarget = NULL;
	int dwValue = NULL;
	POINT start;
	COPYDATASTRUCT cds; 
	std::string sData = luaL_checkstring(L,1); 
	std::replace( sData.begin(), sData.end(), '\004', '\000');
	dwValue = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : NULL);
	hWndTarget = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : NULL);
	start.x = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	start.y = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);

	if(hWndTarget == NULL) 
		hWndTarget = (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	cds.dwData = (DWORD)dwValue;
	cds.lpData = (PVOID)sData.data();
	cds.cbData = sData.size();


	if (!hWndTarget) { 
		lua_pushnumber(L,0);
		return 1;
	}

	SendMessage((HWND)hWndTarget, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);

	lua_pushnumber(L,1);

	return 0;
} 

static int acGetFileVersion(lua_State *L) {

    DWORD               dwSize              = 0;
    BYTE                *pbVersionInfo      = NULL;
    VS_FIXEDFILEINFO    *pFileInfo          = NULL;
    UINT                puLenFileInfo       = 0;

	int hWndTarget = NULL;
	POINT start;
	std::string sFile = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	hWndTarget = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : NULL);
	start.x = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	start.y = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);

	if(sFile.length() == 0){
		if(hWndTarget == NULL) 
			hWndTarget = (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

		memset(gszFileName,0,sizeof(gszFileName));
		DWORD processID = 0;
		GetWindowThreadProcessId((HWND)hWndTarget, &processID);
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
		GetModuleFileNameEx( hProcess, NULL, gszFileName, sizeof(gszFileName) / sizeof(TCHAR));
		CloseHandle(hProcess);	
		sFile = gszFileName;
	}


    // get the version info for the file requested
    dwSize = GetFileVersionInfoSize( sFile.c_str(), NULL );
    if ( dwSize == 0 )
    {
        return 0;
    }

    pbVersionInfo = new BYTE[ dwSize ];

    if ( !GetFileVersionInfo( sFile.c_str(), 0, dwSize, pbVersionInfo ) )
    {
        delete[] pbVersionInfo;
        return 0;
    }

    if ( !VerQueryValue( pbVersionInfo, TEXT("\\"), (LPVOID*) &pFileInfo, &puLenFileInfo ) )
    {
        delete[] pbVersionInfo;
        return 0;
    }

    // pFileInfo->dwFileVersionMS is usually zero. However, you should check
    // this if your version numbers seem to be wrong
	TCHAR buffer[256];
	wsprintf( buffer,"%d.%d.%d.%d",
		HIWORD(pFileInfo->dwFileVersionMS),
		LOWORD(pFileInfo->dwFileVersionMS),
		HIWORD(pFileInfo->dwFileVersionLS),
		LOWORD(pFileInfo->dwFileVersionLS)
		);

	lua_pushstring(L, buffer);

	delete[] pbVersionInfo;
	return 1;

}

static int acGetFileProductVersion(lua_State *L) {

    DWORD               dwSize              = 0;
    BYTE                *pbVersionInfo      = NULL;
    VS_FIXEDFILEINFO    *pFileInfo          = NULL;
    UINT                puLenFileInfo       = 0;

	int hWndTarget = NULL;
	POINT start;
	std::string sFile = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	hWndTarget = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : NULL);
	start.x = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	start.y = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);

	if(sFile.length() == 0){
		if(hWndTarget == NULL) 
			hWndTarget = (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

		memset(gszFileName,0,sizeof(gszFileName));
		DWORD processID = 0;
		GetWindowThreadProcessId((HWND)hWndTarget, &processID);
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
		GetModuleFileNameEx( hProcess, NULL, gszFileName, sizeof(gszFileName) / sizeof(TCHAR));
		CloseHandle(hProcess);	
		sFile = gszFileName;
	}

    // get the version info for the file requested
    dwSize = GetFileVersionInfoSize( sFile.c_str(), NULL );
    if ( dwSize == 0 )
    {
        return 0;
    }

    pbVersionInfo = new BYTE[ dwSize ];

    if ( !GetFileVersionInfo( sFile.c_str(), 0, dwSize, pbVersionInfo ) )
    {
        delete[] pbVersionInfo;
        return 0;
    }

    if ( !VerQueryValue( pbVersionInfo, TEXT("\\"), (LPVOID*) &pFileInfo, &puLenFileInfo ) )
    {
        delete[] pbVersionInfo;
        return 0;
    }

    // pFileInfo->dwProductVersionMS is usually zero. However, you should check
    // this if your version numbers seem to be wrong

	TCHAR buffer[256];
    wsprintf(buffer,  "%d.%d.%d.%d",
		HIWORD(pFileInfo->dwProductVersionMS),
		LOWORD(pFileInfo->dwProductVersionMS),
		HIWORD(pFileInfo->dwProductVersionLS),
		LOWORD(pFileInfo->dwProductVersionLS)
        );

	lua_pushstring(L, buffer);

	delete[] pbVersionInfo;
	return 1;

}

static int acDisplayBalloonTip(lua_State *L){
	std::string sText = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	std::string sTitle = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	int iIcon = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : NIIF_INFO );

	NOTIFYICONDATA nida;
	nida.cbSize = NOTIFYICONDATA_V3_SIZE;//sizeof(NOTIFYICONDATA);
	nida.hWnd = hWndServer;
	nida.uID = trayID;
	nida.uVersion = 3; //NOTIFYICON_VERSION;
	_tcscpy_s(nida.szInfoTitle,sTitle.c_str());
	_tcscpy_s(nida.szInfo,sText.c_str());
	nida.uFlags = NIF_INFO; 
	nida.dwInfoFlags = iIcon | NIIF_NOSOUND;
	Shell_NotifyIcon(NIM_MODIFY, &nida);

	return 1;
}

static int acRegistryCreateKey(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	//std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
	}
	
	if(Reg.CreateKey(sPath)){
		lua_pushnumber(L, 1);
	} else {
		lua_pushnumber(L, 0);
	}
	
	return 1;
}

static int acRegistryDeleteKey(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, FALSE))
	{
		if(Reg.DeleteKey(sPath)){
			lua_pushnumber(L, 1);
		} else {
			lua_pushnumber(L, 0);
		}
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	
	return 1;
}

static int acRegistryDeleteValue(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, FALSE))
	{
		if(Reg.DeleteValue(sName)){
			lua_pushnumber(L, 1);
		} else {
			lua_pushnumber(L, 0);
		}
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	
	return 1;
}

static int acRegistryGetString(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	std::string sDefault = (lua_isstring(L,4) ? lua_tostring(L, 4) : _T(""));
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushstring(L, sDefault.c_str());
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, FALSE))
	{
		std::string m_strValue = Reg.ReadString(sName, sDefault);
		lua_pushstring(L, m_strValue.c_str());
	}
	else
	{
		lua_pushstring(L, sDefault.c_str());
		return 1;
	}
	
	return 1;
}

static int acRegistrySetString(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	std::string sValue = (lua_isstring(L,4) ? lua_tostring(L, 4) : _T(""));
	int iType = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 1);
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, TRUE))
	{
		if(Reg.WriteString(sName, sValue, iType))
		{
			lua_pushnumber(L, 1);
		} else {
			lua_pushnumber(L, 0);
			return 1;
		}
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	
	return 1;
}

static int acRegistryGetNumber(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	double dDefault = (double)(lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	int iType = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 3);
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, (DWORD)dDefault);
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, FALSE))
	{
		switch(iType) {
			case 1: 
				{
					lua_pushnumber(L, Reg.ReadFloat(sName, dDefault));
					break;
				}
			case 2: 
				{
					lua_pushnumber(L, Reg.ReadInt(sName, (int)dDefault));
					break;
				}
			case 3: 
				{
					lua_pushnumber(L, Reg.ReadDword(sName, (DWORD)dDefault));
					break;
				}
			default: 
				{
					lua_pushnumber(L, (DWORD)dDefault);
					return 1;
				}
		}
	}
	else
	{
		lua_pushnumber(L, (DWORD)dDefault);
		return 1;
	}
	
	return 1;
}

static int acRegistrySetNumber(lua_State *L) {
	CRegistry Reg;
	int iRoot = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	std::string sPath = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	std::string sName = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	double dValue = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	int iType = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 3);
	switch(iRoot) {
		case 1: 
			{
				Reg.SetRootKey(HKEY_CLASSES_ROOT);
				break;
			}
		case 2: 
			{
				Reg.SetRootKey(HKEY_LOCAL_MACHINE);
				break;
			}
		case 3: 
			{
				Reg.SetRootKey(HKEY_CURRENT_USER);
				break;
			}
		case 4: 
			{
				Reg.SetRootKey(HKEY_USERS);
				break;
			}
		default: 
			{
				lua_pushnumber(L, 0);
				return 1;
			}
	}
	
	if (Reg.SetKey(sPath, TRUE))
	{
		BOOL bRet;
		switch(iType) {
			case 1: 
				{
					bRet = Reg.WriteFloat(sName, dValue);
					break;
				}
			case 2: 
				{
					bRet = Reg.WriteInt(sName, (int)dValue);
					break;
				}
			case 3: 
				{
					bRet = Reg.WriteDword(sName, (DWORD)dValue);
					break;
				}
			default: 
				{
					lua_pushnumber(L, 0);
					return 1;
				}
		}
		
		if(bRet)
		{
			lua_pushnumber(L, 1);
		} else {
			lua_pushnumber(L, 0);
			return 1;
		}
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	
	return 1;
}

static int acSendKeyDown(lua_State *L) {
	while(bSendKeysExecuting) {
		Sleep(25);
	}
	bSendKeysExecuting = true;
	BYTE VKey = (lua_isnumber(L,1) ? (BYTE)lua_tonumber(L, 1) : 0);
	BYTE ScanCode = LOBYTE(MapVirtualKey(VKey, 0));
	KeyboardEvent(VKey, ScanCode, IsVkExtended(VKey) ? KEYEVENTF_EXTENDEDKEY : 0);
	bSendKeysExecuting = false;
	return 1;
}

static int acSendKeyUp(lua_State *L) {
	while(bSendKeysExecuting) {
		Sleep(25);
	}
	bSendKeysExecuting = true;
	BYTE VKey = (lua_isnumber(L,1) ? (BYTE)lua_tonumber(L, 1) : 0);
	BYTE ScanCode = LOBYTE(MapVirtualKey(VKey, 0));
	KeyboardEvent(VKey, ScanCode, KEYEVENTF_KEYUP | (IsVkExtended(VKey) ? KEYEVENTF_EXTENDEDKEY : 0));  
	bSendKeysExecuting = false;
	return 1;
}

static int acShowMouseCursor(lua_State *L){
	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, WM_SETTINGCHANGE | SPIF_UPDATEINIFILE);
	return 1;
}

static int acAutoHideMouseCursor(lua_State *L){
	iHideMouseAfter = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 5000);
	iReHideMouseAfter = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 5000);
	if(iHideMouseAfter == 0){
		bAutoHideMouse = false;
		bCursorHidden = false;
		iMouseHideCounter = 0;
		iMouseReHideCounter = 0;
		acShowMouseCursor(L);
	} else {
		bAutoHideMouse = true;
		bCursorHidden = false;
		iMouseHideCounter = 0;
		iMouseReHideCounter = 0;
	}
	return 1;
}

static int acHideMouseCursor(lua_State *L){

	BYTE ANDmaskCursor[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
							0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
 
	BYTE XORmaskCursor[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	for(int i = 0; i < 14; i++) { 
		curBlank[i] = CreateCursor( hInst, 0, 0, 32, 32, ANDmaskCursor, XORmaskCursor);  
		SetSystemCursor(curBlank[i], g_CursorTypes[i]);
	}

	return 1;
	
}

static int acGetMouseCursorType(lua_State *L){

	vector<HCURSOR> aCursorHandles;
	for(int i = 0; i < 14; i++) { 
		aCursorHandles.push_back((HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(g_CursorTypes[i]), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
	}

	CURSORINFO CursorInfo;
	(VOID)memset(&CursorInfo, 0, sizeof(CursorInfo));
	CursorInfo.cbSize = sizeof(CursorInfo);
	GetCursorInfo(&CursorInfo);

	for(int i = 0; i < 14; i++) { 
		if(aCursorHandles[i] == CursorInfo.hCursor){
			lua_pushstring(L, g_CursorTypeNames[i].c_str());
			return 1;
		}
	}

	lua_pushstring(L, _T("Unknown"));
	return 1;

}

void SetForegroundWindowInternalB(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return;
 
	BYTE keyState[256] = {0};
	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if(::GetKeyboardState((LPBYTE)&keyState))
	{
		if(!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}
 
	::SetForegroundWindow(hWnd);
 
	if(::GetKeyboardState((LPBYTE)&keyState))
	{
		if(!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
}

void SetForegroundWindowInternal(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return;
 
	//relation time of SetForegroundWindow lock
	DWORD lockTimeOut = 0;
	HWND  hCurrWnd = ::GetForegroundWindow();
	DWORD dwThisTID = ::GetCurrentThreadId(),
	      dwCurrTID = ::GetWindowThreadProcessId(hCurrWnd,0);
 
	//we need to bypass some limitations from Microsoft :)
	if(dwThisTID != dwCurrTID)
	{
		::AttachThreadInput(dwThisTID, dwCurrTID, TRUE);
 
		::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,0,&lockTimeOut,0);
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,0,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
 
		::AllowSetForegroundWindow(ASFW_ANY);
	}
 
	::SetForegroundWindow(hWnd);
 
	if(dwThisTID != dwCurrTID)
	{
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,(PVOID)lockTimeOut,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
		::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);
	}
}

static int acSetKeyboardHook(BOOL value){
	if(keyboardhook != NULL && !value){
		UnhookWindowsHookEx(keyboardhook);
		keyboardhook = NULL;
	} else if(keyboardhook && value) {
		keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc,hInst,0);
	}
	return 1;
}
static int acSetKeyboardHook_glue(lua_State *L){
	int bValue = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 1);
	lua_pushnumber(L, acSetKeyboardHook(bValue));
	return 1;
}

static int acShellExecute(LPCTSTR lpVerb, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, int nShowCmd){
	try{
		ShellExecute(NULL, lpVerb, lpFile, lpParameters, lpDirectory, nShowCmd);
	}
	catch (std::basic_string<TCHAR> err) {
		MessageBox(GetForegroundWindow(),err.c_str(), _T("ShellExecute Error"), NULL);
	}
	return 1;
}
static int acShellExecute_glue(lua_State *L){
	std::basic_string<TCHAR> sVerb, sFile, sParameters, sDirectory;
	int nShowCmd;
	sVerb = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T("open"));
	sFile = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	sParameters = (lua_isstring(L,3) ? lua_tostring(L, 3) : _T(""));
	sDirectory = (lua_isstring(L,4) ? lua_tostring(L, 4) : _T(""));
	nShowCmd = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 1);
	lua_pushnumber(L, acShellExecute((LPCTSTR)sVerb.c_str(),(LPCTSTR)sFile.c_str(),(LPCTSTR)sParameters.c_str(),(LPCTSTR)sDirectory.c_str(),nShowCmd));
	return 1;
}

static int acGetControlID(HWND hWndTarget, POINT start) {

	if(hWndTarget == NULL)
		hWndTarget = WindowFromPoint(start);

	return (int) GetDlgCtrlID(hWndTarget);
}
static int acGetControlID_glue(lua_State *L){
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetControlID((HWND)hWndTarget, start));
	return 1;
}

static int acGetWindowFromProcessID(int pID){
	hWndProcessID = NULL;
	if(pID > 0){
		g_processID = (DWORD)pID;
		EnumWindows(EnumWindowsProc, 9);
	}
	return (int)hWndProcessID;
}
static int acGetWindowFromProcessID_glue(lua_State *L){
	int pID = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	lua_pushnumber(L, acGetWindowFromProcessID(pID));
	return 1;
}

static DWORD acGetProcessIDFromPattern(LPCTSTR lpFile){
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return 0;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
	{
        if( aProcesses[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = _T("<unknown>");

			// Get a handle to the process.

			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
										   PROCESS_VM_READ,
										   FALSE, aProcesses[i] );

			// Get the process name.

			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
					 &cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName) );
					CloseHandle(hProcess);
					std::basic_string<TCHAR> sFile = lpFile;
					boost::regex e(sFile.c_str());
					if(boost::regex_match(szProcessName, e ) == TRUE)
					{
						return aProcesses[i];
						//break;
					}
				}
			}
		}
	}
	return 0;
}
static int acGetProcessIDFromPattern_glue(lua_State *L) {
	std::basic_string<TCHAR> sName;
	sName = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	lua_pushnumber(L, acGetProcessIDFromPattern((LPCTSTR)sName.c_str()));
	return 1;
}

static int acConsumePhysicalInput(bool bVal){
	bConsumePhysical = bVal;
	return 1;
}
static int acConsumePhysicalInput_glue(lua_State *L)
{
	BOOL bVal = false;
	bVal = lua_tonumber(L, 1);
	lua_pushnumber(L, acConsumePhysicalInput(bVal));
	return 1;
}

static int acDisplayText() {

	if(bOSDDisplayed) return 0;
	bOSDDisplayed = TRUE;
	SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	HANDLE hThread = CreateThread(NULL, 0, &CreateOSDDialog, NULL, 0, NULL); 
	CloseHandle(hThread);

	return 1;
}
static int acDisplayText_glue(lua_State *L) {
	sDisplayTextMessage = (lua_isstring(L, 1) ? lua_tostring(L, 1) : _T(""));
	sDisplayTextFont = (lua_isstring(L, 2) ? lua_tostring(L, 2) : _T("Arial"));
	iDisplayTextSize = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 10);
	iDisplayTextR = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	iDisplayTextG = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);
	iDisplayTextB = (lua_isnumber(L,6) ? lua_tonumber(L, 6) : 0);
	iDisplayTextDuration = (lua_isnumber(L,7) ? lua_tonumber(L, 7) : 0);
	iDisplayTextX = (lua_isnumber(L,8) ? lua_tonumber(L, 8) : 0);
	iDisplayTextY = (lua_isnumber(L,9) ? lua_tonumber(L, 9) : 0);
	iDisplayTextBGR = (lua_isnumber(L,10) ? lua_tonumber(L, 10) : iDisplayTextR);
	iDisplayTextBGG = (lua_isnumber(L,11) ? lua_tonumber(L, 11) : iDisplayTextG);
	iDisplayTextBGB = (lua_isnumber(L,12) ? lua_tonumber(L, 12) : iDisplayTextB);
	if(iDisplayTextR == iDisplayTextBGR && iDisplayTextG == iDisplayTextBGG && iDisplayTextB == iDisplayTextBGB)
	{
		iDisplayTextBGR = (iDisplayTextBGR == 0 ? iDisplayTextBGR + 1 : iDisplayTextBGR - 1);
		iDisplayTextBGG = (iDisplayTextBGG == 0 ? iDisplayTextBGG + 1 : iDisplayTextBGG - 1);
		iDisplayTextBGB = (iDisplayTextBGB == 0 ? iDisplayTextBGB + 1 : iDisplayTextBGB - 1);
	}
	lua_pushnumber(L, acDisplayText());
	return 1;
}

static int acLockWindows() {
	LockWorkStation();
	return 1;
}
static int acLockWindows_glue(lua_State *L){
	lua_pushnumber(L, acLockWindows());
	return 1;
}
/*
static TCHAR* acGetClassName(HWND hWndTarget, POINT start) {

	if(hWndTarget == NULL)
		hWndTarget = WindowFromPoint(start);

	GetClassName(hWndTarget,gszClassName,sizeof(gszClassName));

	return (TCHAR*)gszClassName;

}*/
static int acGetClassName(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);

	if(hWndTarget == NULL)
		hWndTarget = (int)WindowFromPoint(start);

	GetClassName((HWND)hWndTarget,gszClassName,sizeof(gszClassName));

	if(_tcslen(gszClassName) > 0){
		lua_pushstring(L, gszClassName);
		ZeroMemory(gszClassName,sizeof(gszClassName));
	} else {
		lua_pushstring(L, _T(""));
	}
	return 1;
}
/*
static TCHAR* acGetExecutableName(HWND hWndTarget, POINT start) {

	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	memset(gszFileName,0,sizeof(gszFileName));
	DWORD processID = 0;
	
	TCHAR drive[MAX_PATH] = {0};
	TCHAR dir[MAX_PATH] = {0};
	TCHAR fname[MAX_PATH] = {0};
	TCHAR ext[MAX_PATH] = {0};

	GetWindowThreadProcessId(hWndTarget, &processID);
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	GetProcessImageFileName( hProcess, gszFileName, sizeof(gszFileName) / sizeof(TCHAR));
	_tsplitpath(gszFileName, drive, dir, fname, ext);
	memset(gszFileName,0,sizeof(gszFileName));
	_tcscat(gszFileName,fname);
	_tcscat(gszFileName,ext);

	return (TCHAR*)gszFileName;

}*/
static int acGetExecutableName(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);

	if(hWndTarget == NULL)
		hWndTarget = (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
	memset(gszFileName,0,sizeof(gszFileName));
	DWORD processID = 0;

	TCHAR drive[MAX_PATH] = {0};
	TCHAR dir[MAX_PATH] = {0};
	TCHAR fname[MAX_PATH] = {0};
	TCHAR ext[MAX_PATH] = {0};

	GetWindowThreadProcessId((HWND)hWndTarget, &processID);
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	GetProcessImageFileName( hProcess, gszFileName, sizeof(gszFileName) / sizeof(TCHAR));
	CloseHandle(hProcess);
	_tsplitpath(gszFileName, drive, dir, fname, ext);
	memset(gszFileName,0,sizeof(gszFileName));
	_tcscat(gszFileName,fname);
	_tcscat(gszFileName,ext);
	lua_pushstring(L, gszFileName);

	if(_tcslen(gszFileName) > 0){
		ZeroMemory(gszFileName,sizeof(gszFileName));
	} else {
		lua_pushstring(L, _T(""));
	}
	return 1;
}


static int acPlaySound(LPCTSTR pszFile) {
	PlaySound(pszFile, NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
	return 1;
}
static int acPlaySound_glue(lua_State *L) {
	std::basic_string<TCHAR> sndFile;
	sndFile = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	lua_pushnumber(L, acPlaySound((LPCTSTR)sndFile.c_str()));
	return 1;
}

static int acSendControlDown_glue(lua_State *L){
	lua_pushnumber(L, acSendControlDown());
	return 1;
}
static int acSendControlDown(){

	SendKeyDown(VK_CONTROL, 1, false);
	return 1;
}

static int acSendControlUp_glue(lua_State *L){
	lua_pushnumber(L, acSendControlUp());
	return 1;
}
static int acSendControlUp(){

	SendKeyUp(VK_CONTROL);
	return 1;
}

static int acSendAltDown_glue(lua_State *L){
	lua_pushnumber(L, acSendAltDown());
	return 1;
}
static int acSendAltDown(){

	SendKeyDown(VK_MENU, 1, false);
	return 1;
}

static int acSendAltUp_glue(lua_State *L){
	lua_pushnumber(L, acSendAltUp());
	return 1;
}
static int acSendAltUp(){

	SendKeyUp(VK_MENU);
	return 1;
}

static int acSendShiftDown_glue(lua_State *L){
	lua_pushnumber(L, acSendShiftDown());
	return 1;
}
static int acSendShiftDown(){

	SendKeyDown(VK_SHIFT, 1, false);
	return 1;
}

static int acSendShiftUp_glue(lua_State *L){
	lua_pushnumber(L, acSendShiftUp());
	return 1;
}
static int acSendShiftUp(){

	SendKeyUp(VK_SHIFT);
	return 1;
}

static int acSendWinDown_glue(lua_State *L){
	lua_pushnumber(L, acSendWinDown());
	return 1;
}
static int acSendWinDown(){

	SendKeyDown(VK_LWIN, 1, false);
	return 1;
}

static int acSendWinUp_glue(lua_State *L){
	lua_pushnumber(L, acSendWinUp());
	return 1;
}
static int acSendWinUp(){

	SendKeyUp(VK_LWIN);
	return 1;
}

static int acGetClipboardText(lua_State *L){
	try {

		HANDLE clip;
 
		if (OpenClipboard(NULL)) {
			try {
			  clip = GetClipboardData(CF_TEXT);
			  if(clip != NULL)
				  ActionBuffer1 = (TCHAR*)clip;
			  else
				  ActionBuffer1 = _T("");
			}
			catch (...) {
			  ActionBuffer1 = _T("");
			}
			CloseClipboard();
		}

		if(_tcslen(ActionBuffer1) > 0){
			lua_pushstring(L, ActionBuffer1);
			ZeroMemory(ActionBuffer1,sizeof(ActionBuffer1));
		} else {
			lua_pushstring(L, _T(""));
		}
	}
	catch (...) {
		lua_pushstring(L, _T(""));
	}
	
	return 1;
}/*
static TCHAR* acGetClipboardText(){

	HANDLE clip;
 
    if (OpenClipboard(NULL)) {
		try {
		  clip = GetClipboardData(CF_TEXT);
		  if(clip != NULL)
			  ActionBuffer1 = (TCHAR*)clip;
		  else
			  ActionBuffer1 = _T("");
		}
		catch (...) {
		  ActionBuffer1 = _T("");
		}
		CloseClipboard();
    }

  return ActionBuffer1;
}*/

/*
static int acSetClipboardText(string szText){

	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		TCHAR * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, szText.length()+1);
		buffer = (TCHAR*)GlobalLock(clipbuffer);
		_tcscpy(buffer, (LPCTSTR)szText.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}

  return 1;
}*/
static int acSetClipboardText(lua_State *L){
	std::basic_string<TCHAR> cliptext;
	cliptext = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));

	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		TCHAR * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, cliptext.length()+1);
		buffer = (TCHAR*)GlobalLock(clipbuffer);
		_tcscpy(buffer, (LPCTSTR)cliptext.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}

	lua_pushnumber(L,1);
	return 1;
}

static int acSendMessage_glue(lua_State *L) {
	long iWndTarget = (lua_isnumber(L,1) ? lua_tounsigned(L, 1) : NULL);
	long MessageID = (lua_isnumber(L,2) ? lua_tounsigned(L, 2) : 0);
	long wParam = (lua_isnumber(L,3) ? lua_tounsigned(L, 3) : 0);
	long lParam = (lua_isnumber(L,4) ? lua_tounsigned(L, 4) : 0);
	lua_pushnumber(L, acSendMessage((HWND)iWndTarget, (UINT)MessageID, (WPARAM)wParam, (LPARAM)lParam));
	return 1;
}
static int acSendMessage(HWND hWndTarget, UINT MessageID, WPARAM wParam, LPARAM lParam) {
	SendMessage(hWndTarget, MessageID, wParam, lParam);
	return 1;
}

static int acPostMessage_glue(lua_State *L) {
	long iWndTarget = (lua_isnumber(L,1) ? lua_tounsigned(L, 1) : NULL);
	long MessageID = (lua_isnumber(L,2) ? lua_tounsigned(L, 2) : 0);
	long wParam = (lua_isnumber(L,3) ? lua_tounsigned(L, 3) : 0);
	long lParam = (lua_isnumber(L,4) ? lua_tounsigned(L, 4) : 0);
	lua_pushnumber(L, acPostMessage((HWND)iWndTarget, (UINT)MessageID, (WPARAM)wParam, (LPARAM)lParam));
	return 1;
}
static int acPostMessage(HWND hWndTarget, UINT MessageID, WPARAM wParam, LPARAM lParam) {
	PostMessage(hWndTarget, MessageID, wParam, lParam);
	return 1;
}

static int acGetMouseLocationX_glue(lua_State *L) {
	lua_pushnumber(L, acGetMouseLocationX());
	return 1;
}
static int acGetMouseLocationX() {
	POINT screenpoint;
	GetCursorPos (&screenpoint); //Get the current mouse pos
	DWORD dx = screenpoint.x;
	return dx;
}

static int acGetMouseLocationY_glue(lua_State *L) {
	lua_pushnumber(L, acGetMouseLocationY());
	return 1;
}
static int acGetMouseLocationY() {
	POINT screenpoint;
	GetCursorPos (&screenpoint); //Get the current mouse pos
	DWORD dy = screenpoint.y;
	return dy;
}

static int acMouseMove_glue(lua_State *L) {
	POINT end;
	end.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	end.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acMouseMove(end));
	return 1;
}
static int acMouseMove(POINT end) {
	DWORD dxe = (65535 * (end.x-rcWindow.left)) / GetSystemMetrics(SM_CXVIRTUALSCREEN) + 1;//Set the x,y of the location where the drawing began
	DWORD dye = (65535 * (end.y-rcWindow.top)) / GetSystemMetrics(SM_CYVIRTUALSCREEN) + 1;//
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE, dxe, dye, 0, 123456); //Move the mouse to that start location
	return 1;
}

static int acMouseClick_glue(lua_State *L) {
	POINT end;
	int button; 
	BOOL bDown;
	BOOL bUp;
	end.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	end.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	button = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 2);
	bDown = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	bUp = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);
	lua_pushnumber(L, acMouseClick(end, button, bDown, bUp));
	return 1;
}
static int acMouseClick(POINT end, int button, BOOL bDown, BOOL bUp) {
	DWORD dx = (65535 * (end.x-rcWindow.left)) / GetSystemMetrics(SM_CXVIRTUALSCREEN) + 1;//Set the x,y of the location where the drawing began
	DWORD dy = (65535 * (end.y-rcWindow.top)) / GetSystemMetrics(SM_CYVIRTUALSCREEN) + 1;//
	if(bDown) {
		if(button == 3 || button == 4) {
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE | MOUSEEVENTF_XDOWN, dx, dy, (button == 3 ? XBUTTON1 : XBUTTON2), 123456); //Send the xbutton down event to the foreground window
		} else {
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE | (button == 0 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN) : (button == 1 ? MOUSEEVENTF_MIDDLEDOWN : (button == 2 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) : NULL))), dx, dy, 0, 123456); //Send the hook button down event to the foreground window
		}
	}
	if(bUp) {
		if(button == 3 || button == 4) {
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE | MOUSEEVENTF_XUP, dx, dy, (button == 3 ? XBUTTON1 : XBUTTON2), 123456); //Send the xbutton down event to the foreground window
		} else {
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE | (button == 0 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP) : (button == 1 ? MOUSEEVENTF_MIDDLEUP : (button == 2 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP) : NULL))), dx, dy, 0, 123456); //Send the right button down event to the foreground window
		}
	}
	return 1;
}

static int acReloadConfig_glue(lua_State *L){
	lua_pushnumber(L, acReloadConfig());
	return 1;
}
static int acReloadConfig() {
	if(bSettingsOpen || bGestureOpen){
		MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.ReloadSettingsOpenMessage",_T("Please close all StrokesPlus windows before calling acReload().")).c_str(),ptLang.get("language.Base.Messages.ReloadSettingsOpenTitle",_T("I'm sorry Dave, I'm afraid I can't do that.")).c_str(), MB_ICONWARNING);
	} else {
		PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_RELOAD_DELAY, 1), 0);
	}
	return 1;
}

static int acExit_glue(lua_State *L){
	lua_pushnumber(L, acExit());
	return 1;
}
static int acExit() {
	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_EXIT, 1), 0);
	return 1;
}

static int acGetWindowLeft_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowLeft((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowLeft(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	RECT rectWin;
	GetWindowRect(hWndTarget, &rectWin);
	return rectWin.left;
}

static int acGetWindowTop_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowTop((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowTop(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	RECT rectWin;
	GetWindowRect(hWndTarget, &rectWin);
	return rectWin.top;
}

static int acGetWindowRight_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowRight((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowRight(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	RECT rectWin;
	GetWindowRect(hWndTarget, &rectWin);
	return rectWin.right;
}

static int acGetWindowBottom_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowBottom((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowBottom(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	RECT rectWin;
	GetWindowRect(hWndTarget, &rectWin);
	return rectWin.bottom;
}
/*
static int acClearCaptureVarsBeforeLua_glue(lua_State *L)
{
	BOOL bVal = TRUE;
	bVal = lua_tonumber(L, 1);
	lua_pushnumber(L, acClearCaptureVarsBeforeLua(bVal));
	return 1;
}
static int acClearCaptureVarsBeforeLua(BOOL bVal) {
	bClearCaptureVarsBeforeLua = bVal;
	return 1;
}
*/
static int acGetForegroundWindow_glue(lua_State *L)
{
	lua_pushnumber(L, acGetForegroundWindow());
	return 1;
}
static int acGetForegroundWindow() {
	return (int) GetForegroundWindow();
}

static int acGetDesktopWindow_glue(lua_State *L)
{
	lua_pushnumber(L, acGetDesktopWindow());
	return 1;
}
static int acGetDesktopWindow() {
	return (int) GetDesktopWindow();
}

static int acTileWindows(int iMode, BOOL bAllScreens){
	EnumWindowCount = 0;
	EnumWindowCountTemp = 0;
	EnumWindows(EnumWindowsProc, 4);
	if(iMode==1){
		//Vertical
		EnumWindows(EnumWindowsProc, (bAllScreens ? 7 : 5));
	} else if(iMode ==2){
		//Horizontal
		EnumWindows(EnumWindowsProc, (bAllScreens ? 8 : 6));
	} 
	return 1;
}
static int acTileWindowsShell(int iMode, BOOL bAllScreens){
	RECT rcTemp;
	if(!bAllScreens){
		POINT screenpoint;
		GetCursorPos (&screenpoint); //Get the current mouse pos
		HMONITOR hTempScreen = (HMONITOR)acGetMonitorFromPoint(screenpoint);

		rcTemp.right = acGetMonitorRight(hTempScreen, TRUE);
		rcTemp.left = acGetMonitorLeft(hTempScreen, TRUE);
		rcTemp.bottom = acGetMonitorBottom(hTempScreen, TRUE) ;
		rcTemp.top = acGetMonitorTop(hTempScreen, TRUE);
	}
	if(iMode==1){
		//Vertical
		TileWindows(NULL, MDITILE_VERTICAL, (bAllScreens ? NULL : &rcTemp), 0, NULL);
	} else if(iMode ==2){
		//Horizontal
		TileWindows(NULL, MDITILE_HORIZONTAL, (bAllScreens ? NULL : &rcTemp), 0, NULL);
	} 
	return 1;
}
static int acTileWindows_glue(lua_State *L){
	int iMode = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	BOOL bAllScreens = (BOOL)(lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	BOOL bShell = (BOOL)(lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	if(bShell){
		lua_pushnumber(L, acTileWindowsShell(iMode,bAllScreens));
	} else {
		lua_pushnumber(L, acTileWindows(iMode,bAllScreens));
	}
	return 1;
}


static int acPreviousApplication_glue(lua_State *L)
{
	lua_pushnumber(L, acPreviousApplication());
	return 1;
}
static int acPreviousApplication()
{
	EnumWindowCount = 0;
	EnumWindows(EnumWindowsProc, 1);
	DWORD processID;
	GetWindowThreadProcessId(hWndServer, &processID);
	AllowSetForegroundWindow(processID);
	Sleep(25);
	if(IsIconic(hWndLastWindow)) ::ShowWindow(hWndLastWindow, SW_RESTORE);  
	SetForegroundWindow(hWndLastWindow);
	hWndLastWindow = NULL;
	return 1;
}

static int acNextApplication_glue(lua_State *L)
{
	lua_pushnumber(L, acNextApplication());
	return 1;
}
static int acNextApplication()
{
	EnumWindowCount = 0;
	EnumWindows(EnumWindowsProc, 2);
	DWORD processID;
	GetWindowThreadProcessId(hWndServer, &processID);
	AllowSetForegroundWindow(processID);
	Sleep(25);
	if(IsIconic(hWndLastWindow)) ::ShowWindow(hWndLastWindow, SW_RESTORE);  
	SetForegroundWindow(hWndLastWindow);
	return 1;
}

static int acToggleTrayIcon(int iShow)
{
	if((iShow == NULL) || (bTrayIconVisible && iShow == -1) || (!bTrayIconVisible && iShow == 1)){
		SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_TOGGLETRAYICON, 1), 0);
	}
	return 1;
}
static int acToggleTrayIcon_glue(lua_State *L)
{
	int iShow = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	lua_pushnumber(L, acToggleTrayIcon(iShow));
	return 1;
}

static int acDisable_glue(lua_State *L)
{
	lua_pushnumber(L, acDisable());
	return 1;
}
static int acDisable()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_DISABLED, 1), 0);
	return 1;
}

static int acDisableNext_glue(lua_State *L)
{
	lua_pushnumber(L, acDisableNext());
	return 1;
}
static int acDisableNext()
{
	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
	bCancel = TRUE;
	return TRUE;
}

static int acCloseApplication_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acCloseApplication((HWND)hWndTarget, start));
	return 1;
}
static int acCloseApplication(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL){
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
	}
		//else
			//hWndTarget = GetForegroundWindow();

	SendMessage(hWndTarget,WM_CLOSE,0,0);
	return 1;

}

static int acMaximizeWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acMaximizeWindow((HWND)hWndTarget, start));
	return 1;
}
static int acMaximizeWindow(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		//else
			//hWndTarget = GetForegroundWindow();

	WINDOWPLACEMENT windPlace;
    windPlace.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWndTarget, &windPlace);
	if ( windPlace.showCmd != SW_SHOWMAXIMIZED ) {
		ShowWindow(hWndTarget, SW_SHOWMAXIMIZED);
	}

	return 1;
}

static int acMaximizeOrRestoreWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L, 1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L, 2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L, 3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acMaximizeOrRestoreWindow((HWND)hWndTarget, start));
	return 1;
}
static int acMaximizeOrRestoreWindow(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL){
		/*if(IsWindowVisible(GetForegroundWindow()) == 0) {*/
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		/*} else {
			hWndTarget = GetForegroundWindow();
		}*/
	}
	WINDOWPLACEMENT windPlace;
    windPlace.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWndTarget, &windPlace);
	if ( windPlace.showCmd == SW_SHOWMAXIMIZED ) {
		// restore window if maximized
		ShowWindow(hWndTarget, SW_RESTORE);
	} else {
		// maximize if not maximized
		ShowWindow(hWndTarget, SW_SHOWMAXIMIZED);
	}
	return 1;
}

static int acDelay_glue(lua_State *L)
{
	int delay = NULL;
	delay = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	lua_pushnumber(L, acDelay(delay));
	return 1;
}
static int acDelay(DWORD delay)
{
	Sleep(delay);
	return TRUE;
}

static int acMoveWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	int left = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	int top = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);
	lua_pushnumber(L, acMoveWindow((HWND)hWndTarget, start, left, top));
	return 1;
}
static int acMoveWindow(HWND hWndTarget, POINT start, int left, int top)
{
	if(hWndTarget == NULL)
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		//else
			//hWndTarget = GetForegroundWindow();

	SetWindowPos(hWndTarget, NULL, left, top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	return TRUE;
}

static int acMinimizeWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acMinimizeWindow((HWND)hWndTarget, start));
	return 1;
}
static int acMinimizeWindow(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		//else
			//hWndTarget = GetForegroundWindow();

	SendMessage(hWndTarget, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return TRUE;
}

static int acSendKeys_glue(lua_State *L)
{
	while(bSendKeysExecuting) {
		Sleep(25);
	}
	std::basic_string<TCHAR> keystring;
	keystring = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	bool bWait = (bool)(lua_isnumber(L,2) ? lua_tonumber(L, 2) : false); 
	lua_pushnumber(L, acSendKeys((LPCTSTR)keystring.c_str(), bWait));	 
	return 1;
}
static int acSendKeys(LPCTSTR keystring, BOOL bWait){ 
	if(GetAncestor(GetForegroundWindow(),GA_ROOTOWNER) == hWndServer && GetForegroundWindow() != hWndServer && GetForegroundWindow() != hWndDisplayText){
		setLastActive();
	}
	bSendKeysExecuting = true;
	SendKeys(keystring, bWait); 
	Sleep(15); //Just to make sure the keyboard hook forwards the injected messages
	bSendKeysExecuting = false;
	return TRUE;
}

//Add more features to the acRunProgram function, like hidden.  
//Though may not need to have a ton, just call other actions
//maybe have run program return hwnd?  Or just make subsequent call to findwindow
static int acRunProgram_glue(lua_State *L)
{
	std::basic_string<TCHAR> path;
	std::basic_string<TCHAR> params;
	int secs = 0;
	int style = 10;
	path = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	params = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	secs = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	style = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 1);
	lua_pushnumber(L, acRunProgram((LPCTSTR)path.c_str(), (LPCTSTR)params.c_str(), secs, (LONG)style));
	return 1;
}
static int acRunProgram(LPCTSTR inFullPathToExe, LPCTSTR inParameters, int secs, LONG style) {
    size_t iMyCounter = 0, iReturnVal = 0, iPos = 0; 
    DWORD dwExitCode = 0; 
    std::basic_string<TCHAR> sTempStr = _T("");
	std::basic_string<TCHAR> FullPathToExe = inFullPathToExe;
	std::basic_string<TCHAR> Parameters = inParameters;

    /* - NOTE - You should check here to see if the exe even exists */ 

    /* Add a space to the beginning of the Parameters */ 
    if (Parameters.size() != 0) 
    { 
        if (Parameters[0] != _T(' ')) 
        { 
            Parameters.insert(0,_T(" ")); 
        } 
    } 

    /* The first parameter needs to be the exe itself */ 
    sTempStr = FullPathToExe; 
    iPos = sTempStr.find_last_of(_T("\\")); 
    sTempStr.erase(0, iPos +1); 
    Parameters = sTempStr.append(Parameters); 

     /* CreateProcessW can modify Parameters thus we allocate needed memory */ 
    TCHAR * pwszParam = new TCHAR[Parameters.size() + 1]; 
    if (pwszParam == 0) 
    { 
        return 1; 
    } 
    const TCHAR* pchrTemp = Parameters.c_str(); 
    _tcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp); 

    /* CreateProcess API initialization */ 
    STARTUPINFO siStartupInfo; 
    PROCESS_INFORMATION piProcessInfo; 
    memset(&siStartupInfo, 0, sizeof(siStartupInfo)); 
    memset(&piProcessInfo, 0, sizeof(piProcessInfo)); 
	siStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	siStartupInfo.wShowWindow = style;
	/*if(left > -1 && top > -1){
		siStartupInfo.dwFlags = siStartupInfo.dwFlags | STARTF_USEPOSITION;
		siStartupInfo.dwX = left;
		siStartupInfo.dwY = top;
	}
	if(width > -1 && height > -1) {
		siStartupInfo.dwFlags = siStartupInfo.dwFlags | STARTF_USESIZE;
		siStartupInfo.dwXSize = width;
		siStartupInfo.dwYSize = height;
	}*/
    siStartupInfo.cb = sizeof(siStartupInfo); 

    if (CreateProcess(const_cast<LPCTSTR>(FullPathToExe.c_str()), 
                            pwszParam, 0, 0, false, 
                            CREATE_DEFAULT_ERROR_MODE, 0, 0, 
                            &siStartupInfo, &piProcessInfo) != false) 
    { 
         /* Watch the process. */ 
        dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (secs * 1000)); 
    } 

    /* Free memory */ 
    delete[]pwszParam; 
    pwszParam = 0; 

    /* Release handles */ 
    CloseHandle(piProcessInfo.hProcess); 
    CloseHandle(piProcessInfo.hThread); 

    return (int)dwExitCode; 
}

static int acSetVolume_glue(lua_State *L) {
	double nVolume = NULL;
	bool bIncrement;
	nVolume = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	bIncrement = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acSetVolume(nVolume, bIncrement));
	return 1;
}
static int acSetVolume(float newVolume, bool bIncrement)
{
	// value between 0.0 and 1.0

	if(OSVersion == 5) 
	{

		newVolume = (newVolume/100)*65536;

		Mixer mixer;

		if( mixer.init() && mixer.GetVolumeControl() )
		{
			if(bIncrement) {
				int currValue = 0;
				currValue = mixer.GetVolumeValue();
				newVolume = currValue + newVolume;
			}
			if(newVolume > 65536)
				newVolume = 65536;
			if(newVolume < 0)
				newVolume = 0;
			mixer.SetVolume(newVolume);
			mixer.close();
		}

	} else 	{

		newVolume = newVolume/100;

		CoInitialize(NULL);

		IMMDeviceEnumerator* deviceEnumerator = NULL;
		if(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator) == S_OK) {
			IMMDevice* defaultDevice = NULL;
			if(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK) {
				IAudioEndpointVolume* endpointVolume = NULL;
				if(defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume) == S_OK) {
					if(bIncrement) {
						float currentVolume = 0;
						endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
						newVolume = newVolume + currentVolume;
					}
					if(newVolume > 1)
						newVolume = 1.0;
					if(newVolume < 0)
						newVolume = 0;
					endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
					endpointVolume->Release();
				}
				defaultDevice->Release();
			}
			deviceEnumerator->Release();
		}

		CoUninitialize();
	}
	
    return TRUE;
}

static int acGetVolume_glue(lua_State *L) {
	lua_pushnumber(L, acGetVolume());
	return 1;
}
static int acGetVolume()
{
	// value between 0.0 and 1.0
	float currentVolume = 0;

	if(OSVersion == 5) 
	{
		Mixer mixer;

		if( mixer.init() && mixer.GetVolumeControl() )
		{
			currentVolume = (float)mixer.GetVolumeValue();
			currentVolume = currentVolume / 65536;
			mixer.close();
		}

	} else 	{

		CoInitialize(NULL);

		IMMDeviceEnumerator* deviceEnumerator = NULL;
		if(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator) == S_OK) {
			IMMDevice* defaultDevice = NULL;
			if(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK) {
				IAudioEndpointVolume* endpointVolume = NULL;
				if(defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume) == S_OK) {
					endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
				}
				defaultDevice->Release();
			}
			deviceEnumerator->Release();
		}

		CoUninitialize();
	}
	
    return (int)(currentVolume*100.0f+0.5f);
}

static int acToggleMute_glue(lua_State *L) {
	lua_pushnumber(L, acToggleMute());
	return 1;
}
static int acToggleMute()
{
	BOOL bMute;

	if(OSVersion == 5) 
	{
		Mixer mixer;

		if( mixer.init() && mixer.GetMuteControl() )
		{
			LONG currValue;
			mixer.GetMuteValue(&currValue);
			if(currValue==1){
				mixer.SetMute(false);
			} else {
				mixer.SetMute(true);
			}
			mixer.close();
		}

	} else 	{
		CoInitialize(NULL);

		IMMDeviceEnumerator* deviceEnumerator = NULL;
		if(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator) == S_OK) {
			IMMDevice* defaultDevice = NULL;
			if(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK) {
				IAudioEndpointVolume* endpointVolume = NULL;
				if(defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume) == S_OK) {
					endpointVolume->GetMute(&bMute);
					endpointVolume->SetMute(!bMute, NULL);
					endpointVolume->Release();
				}
				defaultDevice->Release();
			}
			deviceEnumerator->Release();
		}

		CoUninitialize();
	}
	
    return TRUE;
}

static int acGetMute_glue(lua_State *L) {
	lua_pushnumber(L, acGetMute());
	return 1;
}
static int acGetMute()
{
	BOOL bMute;

	if(OSVersion == 5) 
	{
		Mixer mixer;

		if( mixer.init() && mixer.GetMuteControl() )
		{
			LONG currValue;
			mixer.GetMuteValue(&currValue);
			bMute = currValue;
			mixer.close();
		}

	} else 	{
		CoInitialize(NULL);

		IMMDeviceEnumerator* deviceEnumerator = NULL;
		if(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator) == S_OK) {
			IMMDevice* defaultDevice = NULL;
			if(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK) {
				IAudioEndpointVolume* endpointVolume = NULL;
				if(defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume) == S_OK) {
					endpointVolume->GetMute(&bMute);
					endpointVolume->Release();
				}
				defaultDevice->Release();
			}
			deviceEnumerator->Release();
		}

		CoUninitialize();
	}
	
    return (int)bMute;
}

static int acSetMute_glue(lua_State *L) {
	BOOL bMute;
	bMute = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	lua_pushnumber(L, acSetMute(bMute));
	return 1;
}
static int acSetMute(BOOL bMute)
{
	if(OSVersion == 5) 
	{
		Mixer mixer;

		if( mixer.init() && mixer.GetMuteControl() )
		{
			mixer.SetMute(bMute);
			mixer.close();
		}

	} else 	{
		CoInitialize(NULL);

		IMMDeviceEnumerator* deviceEnumerator = NULL;
		if(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator) == S_OK) {
			IMMDevice* defaultDevice = NULL;
			if(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK) {
				IAudioEndpointVolume* endpointVolume = NULL;
				if(defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume) == S_OK) {
					endpointVolume->SetMute(bMute, NULL);
					endpointVolume->Release();
				}
				defaultDevice->Release();
			}
			deviceEnumerator->Release();
		}

		CoUninitialize();
	}
	
    return TRUE;
}

static int acFindWindow_glue(lua_State *L) {
	std::basic_string<TCHAR> sClass = _T("");
	std::basic_string<TCHAR> sTitle = _T("");
	sClass = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	sTitle = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
	lua_pushnumber(L, acFindWindow((LPCTSTR)sClass.c_str(),(LPCTSTR)sTitle.c_str()));
	return 1;
}
static int acFindWindow(LPCTSTR Class, LPCTSTR Title)
{
	return (int)FindWindow((_tcslen(Class) != 0 ? Class : NULL), (_tcslen(Title) != 0 ? Title : NULL));
}

static int acFindWindowByTitleRegex_glue(lua_State *L) {
	std::basic_string<TCHAR> sTitle = _T("");
	sTitle = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	lua_pushnumber(L, acFindWindowByTitleRegex((LPCTSTR)sTitle.c_str()));
	return 1;
}
static int acFindWindowByTitleRegex(TCHAR const * windowTitle)
{
    if( !windowTitle ) {
        //SetLastError( ERROR_INVALID_PARAMETER );
        return 0;
    }

    FindWindowData data( windowTitle );
    if( !EnumWindows( FindWindowImpl, PtrToLong(&data) ) && data.ResultHandle != 0 ) {
        //SetLastError( ERROR_SUCCESS );
        return (int)data.ResultHandle;
    }

    // Return ERROR_FILE_NOT_FOUND in GetLastError
    //SetLastError( ERROR_FILE_NOT_FOUND );
    return 0;
}

static int acGetWindowTitle(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);

	if(hWndTarget == NULL)
			hWndTarget = (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	TCHAR szOwnerTitle[1024];
	memset(szOwnerTitle,0,sizeof(szOwnerTitle));

	if(GetWindowText((HWND)hWndTarget,szOwnerTitle,sizeof(szOwnerTitle)) > 0) {
		ActionBuffer1 = szOwnerTitle;
	} else {
		ActionBuffer1 = _T("");
	}

	if(_tcslen(ActionBuffer1) != 0){
		lua_pushstring(L, ActionBuffer1);
		ZeroMemory(ActionBuffer1,sizeof(ActionBuffer1));
	} else {
		lua_pushstring(L, _T(""));
	}
	return 1;
}/*
static TCHAR* acGetWindowTitle(HWND hWndTarget, POINT start) {

	if(hWndTarget == NULL)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	TCHAR szOwnerTitle[1024];
	memset(szOwnerTitle,0,sizeof(szOwnerTitle));

	if(GetWindowText(hWndTarget,szOwnerTitle,sizeof(szOwnerTitle)) > 0) {
		ActionBuffer1 = szOwnerTitle;
	} else {
		ActionBuffer1 = _T("");
	}

	return ActionBuffer1;
}*/

static int acMessageBox_glue(lua_State *L) {
	try {
		std::basic_string<TCHAR> sMsg = _T("");
		std::basic_string<TCHAR> sTitle = _T("");
		UINT uType = 0;
		sMsg = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
		sTitle = (lua_isstring(L,2) ? lua_tostring(L, 2) : _T(""));
		uType = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : NULL);
		lua_pushnumber(L, acMessageBox((LPCTSTR)sMsg.c_str(),(LPCTSTR)sTitle.c_str(),(UINT)uType));
		return 1;
	}
	catch (...) {
	  return -1;
	}
}
static int acMessageBox(LPCTSTR sMsg, LPCTSTR sTitle, UINT uType) {
	try {
		return MessageBox(GetForegroundWindow(), sMsg, sTitle, uType);
	}
	catch (...) {
	  return -1;
	}
}

static int acSetWindowSize_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	int width = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 10);
	int height = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 10);
	lua_pushnumber(L, acSetWindowSize((HWND)hWndTarget, start, width, height));
	return 1;
}
static int acSetWindowSize(HWND hWndTarget, POINT start, int width, int height)
{
	RECT rect;
	if(hWndTarget == NULL)
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		//else
			//hWndTarget = GetForegroundWindow();

	GetWindowRect(hWndTarget, &rect);

	SetWindowPos(hWndTarget, NULL, rect.left, rect.top, width, height, SWP_NOZORDER);
	return TRUE;
}

static int acSendWindowToBottom_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acSendWindowToBottom((HWND)hWndTarget, start));
	return 1;
}
static int acSendWindowToBottom(HWND hWndTarget, POINT start) {

	if(hWndTarget == NULL)
		//if(IsWindowVisible(GetForegroundWindow()) == 0)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
		//else
		//	hWndTarget = GetForegroundWindow();

	SetWindowPos(hWndTarget, HWND_BOTTOM, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	return TRUE;
}

static int acActivateWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	int iMethod = 0;
	POINT start;
	hWndTarget = (lua_isnumber(L, 1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L, 2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L, 3) ? lua_tonumber(L, 3) : 0);
	iMethod = (lua_isnumber(L, 4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acActivateWindow((HWND)hWndTarget, start, iMethod));
	return 1;
}
static int acActivateWindow(HWND hWndTarget, POINT start, int iMethod)
{
	if(hWndTarget == NULL)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	if(hWndTarget != GetForegroundWindow())
	{
		switch(iMethod){
			case 0:
				SetForegroundWindow(hWndTarget);
				break;
			case 1: 
				SetForegroundWindowInternal(hWndTarget);
				break;
			case 2: 
				SetForegroundWindowInternalB(hWndTarget);
				break;
		}
	}
	return 1;
}

static int acRestoreWindow_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acRestoreWindow((HWND)hWndTarget, start));
	return 1;
}
static int acRestoreWindow(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	WINDOWPLACEMENT windPlace;
    windPlace.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWndTarget, &windPlace);
	//if ( windPlace.showCmd == SW_SHOWMAXIMIZED ) 
		ShowWindow(hWndTarget, SW_RESTORE);
	return 1;
}

static int acGetOwnerWindowByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetOwnerWindowByPoint(start));
	return 1;
}
static int acGetOwnerWindowByPoint(POINT start)
{
	return (int)GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
}

static int acGetParentWindowByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetParentWindowByPoint(start));
	return 1;
}
static int acGetParentWindowByPoint(POINT start)
{
	return (int)GetAncestor(WindowFromPoint(start),GA_ROOT);
}

static int acGetWindowByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetWindowByPoint(start));
	return 1;
}
static int acGetWindowByPoint(POINT start)
{
	return (int)WindowFromPoint(start);
}

static int acSetProcessPriority_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	DWORD dwPriority = 0;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	dwPriority = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : NORMAL_PRIORITY_CLASS);
	lua_pushnumber(L, acSetProcessPriority((HWND)hWndTarget, start, dwPriority));
	return 1;
}
static int acSetProcessPriority(HWND hWndTarget, POINT start, DWORD PriorityClass)
{
	if(hWndTarget == NULL)
			hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	DWORD processID;
	if(hWndTarget == NULL)
	{
		if(IsWindowVisible(GetForegroundWindow()) == 0)
			GetWindowThreadProcessId(GetAncestor(WindowFromPoint(start),GA_ROOTOWNER), &processID);
		else
			GetWindowThreadProcessId(GetForegroundWindow(), &processID);
	}
	else
	{
		GetWindowThreadProcessId(hWndTarget, &processID);
	}
	HANDLE hProcess = OpenProcess( PROCESS_SET_INFORMATION , FALSE, processID );
	SetPriorityClass(hProcess,PriorityClass);
	CloseHandle(hProcess);
	return 1;
}

static int acTerminateProcess_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acTerminateProcess((HWND)hWndTarget, start));
	return 1;
}
static int acTerminateProcess(HWND hWndTarget, POINT start)
{
	DWORD processID;
	if(hWndTarget == NULL)
	{
		if(IsWindowVisible(GetForegroundWindow()) == 0)
			GetWindowThreadProcessId(GetAncestor(WindowFromPoint(start),GA_ROOTOWNER), &processID);
		else
			GetWindowThreadProcessId(GetForegroundWindow(), &processID);
	}
	else
	{
		GetWindowThreadProcessId(hWndTarget, &processID);
	}
	HANDLE hProcess = OpenProcess( PROCESS_TERMINATE , FALSE, processID );
	TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	return 1;
}

static int acToggleTopmost_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acToggleTopmost((HWND)hWndTarget, start));
	return 1;
}
static int acToggleTopmost(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	if((GetWindowLong(hWndTarget, GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST)
		SetWindowPos(hWndTarget, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	else
		SetWindowPos(hWndTarget, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	return 1;
}

static int acSetTopmost_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acSetTopmost((HWND)hWndTarget, start));
	return 1;
}
static int acSetTopmost(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	SetWindowPos(hWndTarget, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	return 1;
}								

static int acClearTopmost_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acClearTopmost((HWND)hWndTarget, start));
	return 1;
}
static int acClearTopmost(HWND hWndTarget, POINT start)
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	SetWindowPos(hWndTarget, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	return 1;
}	

static int acGetSystemMetrics_glue(lua_State *L)
{
	int iVal = 0;
	iVal = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	lua_pushnumber(L, acGetSystemMetrics(iVal));
	return 1;
}
static int acGetSystemMetrics(int iVal)
{
	return GetSystemMetrics(iVal);
}	

static int acPauseResumeThreadList_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	bool bResume = TRUE;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	bResume = (bool)(lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acPauseResumeThreadList((HWND)hWndTarget, start, bResume));
	return 1;
}
static int acPauseResumeThreadList(HWND hWndTarget, POINT start, bool bResumeThread) 
{ 
    HANDLE        hThreadSnap = NULL; 
    BOOL          bRet        = FALSE; 
    THREADENTRY32 te32        = {0}; 
 	DWORD processID;
	if(hWndTarget == NULL)
	{
		if(IsWindowVisible(GetForegroundWindow()) == 0)
			GetWindowThreadProcessId(GetAncestor(WindowFromPoint(start),GA_ROOTOWNER), &processID);
		else
			GetWindowThreadProcessId(GetForegroundWindow(), &processID);
	}
	else
	{
		GetWindowThreadProcessId(hWndTarget, &processID);
	}

    // Take a snapshot of all threads currently in the system. 

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    // Fill in the size of the structure before using it. 

    te32.dwSize = sizeof(THREADENTRY32); 
 
    // Walk the thread snapshot to find all threads of the process. 
    // If the thread belongs to the process, add its information 
    // to the display list.
 
    if (Thread32First(hThreadSnap, &te32)) 
    { 
        do 
        { 
            if (te32.th32OwnerProcessID == processID) 
            {
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
				if (bResumeThread)
				{
					//cout << _T("Resuming Thread 0x") << cout.setf( ios_base::hex ) << te32.th32ThreadID << '\n';
					ResumeThread(hThread);
				}
				else
				{
					//cout << _T("Suspending Thread 0x") << cout.setf( ios_base::hex ) << te32.th32ThreadID << '\n';
					SuspendThread(hThread);
				}
				CloseHandle(hThread);
            } 
        }
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;          // could not walk the list of threads 
 
    // Do not forget to clean up the snapshot object. 
    CloseHandle (hThreadSnap); 
 
    return (int)bRet; 
} 

static int acSetWindowColorKey_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	bool bRemove = TRUE;
	BYTE opacity = 255;
	BYTE red = 255;
	BYTE blue = 255;
	BYTE green = 255;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	bRemove = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	red = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);
	green = (lua_isnumber(L,6) ? lua_tonumber(L, 6) : 0);
	blue = (lua_isnumber(L,7) ? lua_tonumber(L, 7) : 0);

	lua_pushnumber(L, acSetWindowColorKey((HWND)hWndTarget, start, bRemove, red, green, blue));
	return 1;
}
static int acSetWindowColorKey(HWND hWndTarget, POINT start, BOOL bRemove, BYTE red, BYTE green, BYTE blue) {

	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
	
	if(red > 255)
		red= 255;
	if(red < 0 )
		red = 0;

	if(blue > 255)
		blue= 255;
	if(blue < 0 )
		blue = 0;

	if(green > 255)
		green= 255;
	if(green < 0 )
		green = 0;

	if(bRemove) {
		SetWindowLong(hWndTarget, GWL_EXSTYLE, GetWindowLong(hWndTarget, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		// Ask the window and its children to repaint
		RedrawWindow(hWndTarget, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);			
	} else {
		SetWindowLong(hWndTarget, GWL_EXSTYLE, GetWindowLong(hWndTarget, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWndTarget, RGB(red,green,blue), 1.0, LWA_COLORKEY);
	}
	return 0;
}

static int acSetWindowTransparency_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	int opacity = 255;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	opacity = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 255);
	lua_pushnumber(L, acSetWindowTransparency((HWND)hWndTarget, start, opacity));
	return 1;
}
static int acSetWindowTransparency(HWND hWndTarget, POINT start, int opacity) {
	
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	if(opacity > 255)
		opacity = 255;
	if(opacity < 0 )
		opacity = 0;

	if(opacity == 255) {
		SetWindowLong(hWndTarget, GWL_EXSTYLE, GetWindowLong(hWndTarget, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		// Ask the window and its children to repaint
		RedrawWindow(hWndTarget, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);			
	} else {
		SetWindowLong(hWndTarget, GWL_EXSTYLE, GetWindowLong(hWndTarget, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWndTarget , 0, (BYTE)opacity, LWA_ALPHA);
	}
	return 0;
}

static int acSetWindowTransparencyAndColorKey_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	int opacity = 255;
	BYTE red = 255;
	BYTE blue = 255;
	BYTE green = 255;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	opacity = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 255);
	red = (lua_isnumber(L,5) ? lua_tonumber(L, 5) : 0);
	green = (lua_isnumber(L,6) ? lua_tonumber(L, 6) : 0);
	blue = (lua_isnumber(L,7) ? lua_tonumber(L, 7) : 0);
	lua_pushnumber(L, acSetWindowTransparencyAndColorKey((HWND)hWndTarget, start, opacity, red, green, blue));
	return 1;
}
static int acSetWindowTransparencyAndColorKey(HWND hWndTarget, POINT start, int opacity, BYTE red, BYTE green, BYTE blue) {
	
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	if(opacity > 255)
		opacity = 255;
	if(opacity < 0 )
		opacity = 0;

	if(red > 255)
		red= 255;
	if(red < 0 )
		red = 0;

	if(blue > 255)
		blue= 255;
	if(blue < 0 )
		blue = 0;

	if(green > 255)
		green= 255;
	if(green < 0 )
		green = 0;

	SetWindowLong(hWndTarget, GWL_EXSTYLE, GetWindowLong(hWndTarget, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWndTarget , RGB(red,green,blue), (BYTE)opacity, LWA_COLORKEY | LWA_ALPHA);

	return 0;
}

static int acGetWindowTransparency_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowTransparency((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowTransparency(HWND hWndTarget, POINT start) 
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	BYTE alpha = 255;
	GetLayeredWindowAttributes(hWndTarget, NULL, &alpha, NULL);

	return (int)alpha;
}

static int acGetWindowColorKeyR_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowColorKeyR((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowColorKeyR(HWND hWndTarget, POINT start) 
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	COLORREF cRef = NULL;
	GetLayeredWindowAttributes(hWndTarget, &cRef, NULL, NULL);

	return (int)GetRValue(cRef);
}

static int acGetWindowColorKeyG_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowColorKeyG((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowColorKeyG(HWND hWndTarget, POINT start) 
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	COLORREF cRef = NULL;
	GetLayeredWindowAttributes(hWndTarget, &cRef, NULL, NULL);

	return (int)GetGValue(cRef);
}

static int acGetWindowColorKeyB_glue(lua_State *L)
{
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acGetWindowColorKeyB((HWND)hWndTarget, start));
	return 1;
}
static int acGetWindowColorKeyB(HWND hWndTarget, POINT start) 
{
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	COLORREF cRef = NULL;
	GetLayeredWindowAttributes(hWndTarget, &cRef, NULL, NULL);

	return (int)GetBValue(cRef);
}

static int acGetPixelRByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetPixelRByPoint(start));
	return 1;
}
static int acGetPixelRByPoint(POINT start)
{
    COLORREF color;
    HDC hDC;

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return -2;

    // Retrieve the color at that position
    color = GetPixel(hDC, start.x, start.y);
    if (color == CLR_INVALID)
        return -1;

    // Release the device context again
    ReleaseDC(GetDesktopWindow(), hDC);

	return (int)GetRValue(color);

}

static int acGetPixelGByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetPixelGByPoint(start));
	return 1;
}
static int acGetPixelGByPoint(POINT start)
{
    COLORREF color;
    HDC hDC;

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return -2;

    // Retrieve the color at that position
    color = GetPixel(hDC, start.x, start.y);
    if (color == CLR_INVALID)
        return -1;

    // Release the device context again
    ReleaseDC(GetDesktopWindow(), hDC);

	return (int)GetGValue(color);

}

static int acGetPixelBByPoint_glue(lua_State *L)
{
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetPixelBByPoint(start));
	return 1;
}
static int acGetPixelBByPoint(POINT start)
{
    COLORREF color;
    HDC hDC;

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return -2;

    // Retrieve the color at that position
    color = GetPixel(hDC, start.x, start.y);
    if (color == CLR_INVALID)
        return -1;

    // Release the device context again
    ReleaseDC(GetDesktopWindow(), hDC);

	return (int)GetBValue(color);

}

static int acShowSettings()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_SETTINGS, 1), 0);
	return 1;
}
static int acShowSettings_glue(lua_State *L)
{
	lua_pushnumber(L, acShowSettings());
	return 1;
}

static int acShowAbout()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_ABOUT, 1), 0);
	return 1;
}
static int acShowAbout_glue(lua_State *L)
{
	lua_pushnumber(L, acShowAbout());
	return 1;
}


static int acShowIgnoreList_glue(lua_State *L)
{
	lua_pushnumber(L, acShowIgnoreList());
	return 1;
}
static int acShowIgnoreList()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_IGNORED, 1), 0);
	return 1;
}

static int acShowPrefs_glue(lua_State *L)
{
	lua_pushnumber(L, acShowPrefs());
	return 1;
}
static int acShowPrefs()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_PREF, 1), 0);
	return 1;
}

static int acShowHotkeys_glue(lua_State *L)
{
	lua_pushnumber(L, acShowHotkeys());
	return 1;
}
static int acShowHotkeys()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_HOTKEYS, 1), 0);
	return 1;
}

static int acShowActions_glue(lua_State *L)
{
	lua_pushnumber(L, acShowActions());
	return 1;
}
static int acShowActions()
{
	SendMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_CONFIG, 1), 0);
	
	return 1;
}

static int acCenterWindowToScreen_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	BOOL bUseWorkArea;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	bUseWorkArea = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acCenterWindowToScreen((HWND)hWndTarget, start, bUseWorkArea));
	return 1;
}
static int acCenterWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea) {
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	ClipOrCenterWindowToMonitor(hWndTarget, bUseWorkArea, FALSE);
	return 1;
}

static int acClipWindowToScreen_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	BOOL bUseWorkArea;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	bUseWorkArea = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acClipWindowToScreen((HWND)hWndTarget, start, bUseWorkArea));
	return 1;
}
static int acClipWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea) {
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	ClipOrCenterWindowToMonitor(hWndTarget, bUseWorkArea, TRUE);
	return 1;
}

static int acFitWindowToScreen_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	BOOL bUseWorkArea;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	bUseWorkArea = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acFitWindowToScreen((HWND)hWndTarget, start, bUseWorkArea));
	return 1;
}
static int acFitWindowToScreen(HWND hWndTarget, POINT start, BOOL bUseWorkArea) {
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	FitWindowToMonitor(hWndTarget, bUseWorkArea);
	return 1;
}

static int acMaximizeToAllScreens_glue(lua_State *L) {
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acMaximizeToAllScreens((HWND)hWndTarget, start));
	return 1;
}
static int acMaximizeToAllScreens(HWND hWndTarget, POINT start) {
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	FitWindowToDesktop(hWndTarget);
	return 1;
}

static int acGetMonitorFromPoint_glue(lua_State *L){
	POINT start;
	start.x = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	start.y = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetMonitorFromPoint(start));
	return 1;
}
static int acGetMonitorFromPoint(POINT start) {
	HMONITOR hMonitor = MonitorFromPoint(start, MONITOR_DEFAULTTONEAREST);
	return (LONG) hMonitor;
}

#ifdef SP_MONITOR_BRIGHTNESS

	/*
	static int acGetDisplayGamma(lua_State *L) {

		//Generate the 256-colors array for the specified wBrightness value.
		WORD GammaArray[3][256];

		lua_createtable(L, 3, 0);
		lua_pushinteger(L, RVAL);
		lua_rawseti (L, -2, 0);		

		lua_pushinteger(L, GVAL);
		lua_rawseti (L, -2, 1);		

		lua_pushinteger(L, BVAL);
		lua_rawseti (L, -2, 2);		
		lua_setglobal(L,"sp_gamma_ramp");

		return 1;
	}
	*/

	static int acSetDisplayGamma(lua_State *L) {

			int iRGamma = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
			int iGGamma = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
			int iBGamma = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);

			HDC hGammaDC = GetDC(NULL);

			//Generate the 256-colors array for the specified wBrightness value.
			WORD GammaArray[3][256];

			for (int iIndex = 0; iIndex < 256; iIndex++)
			{
				int iRArrayValue = iIndex * (iRGamma + 128);
				int iGArrayValue = iIndex * (iGGamma + 128);
				int iBArrayValue = iIndex * (iBGamma + 128);

				if (iRArrayValue > 65535) iRArrayValue = 65535;
				if (iGArrayValue > 65535) iGArrayValue = 65535;
				if (iBArrayValue > 65535) iBArrayValue = 65535;

				GammaArray[0][iIndex] = (WORD)iRArrayValue;
				GammaArray[1][iIndex] = (WORD)iGArrayValue;
				GammaArray[2][iIndex] = (WORD)iBArrayValue;
			
			}

			//Set the GammaArray values into the display device context.
			if(SetDeviceGammaRamp(hGammaDC, GammaArray)) {
				lua_pushnumber(L, 1);
			} else {
				lua_pushnumber(L, -1);
			}

			ReleaseDC(NULL, hGammaDC);

			return 1;
	}

	static int acGetMonitorBrightness(lua_State *L) {
		if(OSVersion<6 || lpfnGetNumberOfPhysicalMonitorsFromHMONITOR == NULL){//Minimum OS Vista
			lua_pushnumber(L, -1);
			return 0;
		}

		LONG hMonitor;
		POINT start;
		int iMode;
		hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
		start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
		start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
		iMode = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);

		DWORD cPhysicalMonitors;
		DWORD dwMinimumBrightness;
		DWORD dwCurrentBrightness;
		DWORD dwMaximumBrightness;
		LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;

		if(hMonitor == 0)
			hMonitor = (LONG)MonitorFromPoint(start, MONITOR_DEFAULTTONEAREST);

		// Get the number of physical monitors.
		BOOL bSuccess = lpfnGetNumberOfPhysicalMonitorsFromHMONITOR(
		  (HMONITOR)hMonitor, 
		  &cPhysicalMonitors
		   );

		if (bSuccess)
		{
			// Allocate the array of PHYSICAL_MONITOR structures.
			pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(
				cPhysicalMonitors* sizeof(PHYSICAL_MONITOR));

			if (pPhysicalMonitors != NULL)
			{
				// Get the array.
				bSuccess = lpfnGetPhysicalMonitorsFromHMONITOR(
					(HMONITOR)hMonitor, cPhysicalMonitors, pPhysicalMonitors);

				/*
				int i = sizeof(pPhysicalMonitors) / sizeof(pPhysicalMonitors[0]);
				for(int j = i;j<i;j++) {
					//not using looping through monitor for now..just getting the first
				}
				*/

				if(!lpfnGetMonitorBrightness(pPhysicalMonitors[0].hPhysicalMonitor, &dwMinimumBrightness, &dwCurrentBrightness, &dwMaximumBrightness)) {
					DisplayLastError(_T("acGetMonitorBrightness"));
					lua_pushnumber(L, -1);
				} else {
					//iMode 1 = Current Brightness, 2 = Min Brightness, 3 = Max Brightness
					lua_pushnumber(L, (iMode == 1 ? dwCurrentBrightness : (iMode == 2 ? dwMinimumBrightness : (iMode == 3 ? dwMaximumBrightness : 0))));
				}

				// Close the monitor handles.
				bSuccess = lpfnDestroyPhysicalMonitors(
					cPhysicalMonitors, 
					pPhysicalMonitors);

				// Free the array.
				free(pPhysicalMonitors);
			} else {
				lua_pushnumber(L, -1);
			}
		} else {
			lua_pushnumber(L, -1);
		}


		return 1;
	}

	static int acSetMonitorBrightness(lua_State *L) {
		if(OSVersion<6 || lpfnGetNumberOfPhysicalMonitorsFromHMONITOR == NULL){ //Minimum OS Vista
			lua_pushnumber(L, -1);
			return 0;
		}
		LONG hMonitor;
		POINT start;
		LONG dwBrightness;
		hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
		start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
		start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
		dwBrightness = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	
		if(hMonitor == 0)
			hMonitor = acGetMonitorFromPoint(start);	

		DWORD cPhysicalMonitors;
		LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;

		// Get the number of physical monitors.
		BOOL bSuccess = lpfnGetNumberOfPhysicalMonitorsFromHMONITOR(
		  (HMONITOR)hMonitor, 
		  &cPhysicalMonitors
		   );

		if (bSuccess)
		{
			// Allocate the array of PHYSICAL_MONITOR structures.
			pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(
				cPhysicalMonitors* sizeof(PHYSICAL_MONITOR));

			if (pPhysicalMonitors != NULL)
			{
				// Get the array.
				bSuccess = lpfnGetPhysicalMonitorsFromHMONITOR(
					(HMONITOR)hMonitor, cPhysicalMonitors, pPhysicalMonitors);

				/*
				int i = sizeof(pPhysicalMonitors) / sizeof(pPhysicalMonitors[0]);
				for(int j = i;j<i;j++) {
					//not using looping through monitor for now..just getting the first
				}
				*/

				if(!lpfnSetMonitorBrightness(pPhysicalMonitors[0].hPhysicalMonitor, (DWORD)dwBrightness)) {
					DisplayLastError(_T("acSetMonitorBrightness"));
					lua_pushnumber(L, -1);
				} else {
					lua_pushnumber(L, 1);
				}

				// Close the monitor handles.
				bSuccess = lpfnDestroyPhysicalMonitors(
					cPhysicalMonitors, 
					pPhysicalMonitors);

				// Free the array.
				free(pPhysicalMonitors);
			} else {
				lua_pushnumber(L, -1);
			}
		} else {
			lua_pushnumber(L, -1);
		}

		return 1;
	}
#endif

static int acGetMonitorName(lua_State *L) {
	LONG hMonitor;
	hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);

	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( (HMONITOR)hMonitor, &mi );
	ActionBuffer1 = mi.szDevice;

	lua_pushstring(L, ActionBuffer1);
	ZeroMemory(ActionBuffer1,sizeof(ActionBuffer1));
	return 1;
}/*
static TCHAR* acGetMonitorName(HMONITOR hMonitor) {
	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );
	ActionBuffer1 = mi.szDevice;
	return ActionBuffer1;
}*/

static int acGetMonitorFromName_glue(lua_State *L) {
	sMonitorName = (lua_isstring(L,1) ? lua_tostring(L, 1) : _T(""));
	lua_pushnumber(L, acGetMonitorFromName());
	return 1;
}
static int acGetMonitorFromName() {
	hLastEnumMonitor = NULL;
	hCurrentEnumMonitor = NULL;
	hFirstEnumMonitor = NULL;
	hSendToMonitor = NULL;
	EnumDisplayMonitors( NULL, NULL, MonitorsCallBack, (LPARAM)4);
	return (LONG) hSendToMonitor;
}

static int acGetMonitorLeft_glue(lua_State *L) {
	LONG hMonitor;
	BOOL bUseWorkArea;
	hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	bUseWorkArea = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetMonitorLeft((HMONITOR)hMonitor, bUseWorkArea));
	return 1;
}
static int acGetMonitorLeft(HMONITOR hMonitor, BOOL bUseWorkArea){
	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	if(bUseWorkArea)
		return mi.rcWork.left;	
	else
		return mi.rcMonitor.left;
}

static int acGetMonitorTop_glue(lua_State *L) {
	LONG hMonitor;
	BOOL bUseWorkArea;
	hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	bUseWorkArea = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetMonitorTop((HMONITOR)hMonitor, bUseWorkArea));
	return 1;
}
static int acGetMonitorTop(HMONITOR hMonitor, BOOL bUseWorkArea){
	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	if(bUseWorkArea)
		return mi.rcWork.top;	
	else
		return mi.rcMonitor.top;
}

static int acGetMonitorRight_glue(lua_State *L) {
	LONG hMonitor;
	BOOL bUseWorkArea;
	hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	bUseWorkArea = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetMonitorRight((HMONITOR)hMonitor, bUseWorkArea));
	return 1;
}
static int acGetMonitorRight(HMONITOR hMonitor, BOOL bUseWorkArea){
	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	if(bUseWorkArea)
		return mi.rcWork.right;	
	else
		return mi.rcMonitor.right;
}

static int acGetMonitorBottom_glue(lua_State *L) {
	LONG hMonitor;
	BOOL bUseWorkArea;
	hMonitor = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : 0);
	bUseWorkArea = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	lua_pushnumber(L, acGetMonitorBottom((HMONITOR)hMonitor, bUseWorkArea));
	return 1;
}
static int acGetMonitorBottom(HMONITOR hMonitor, BOOL bUseWorkArea){
	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );

	if(bUseWorkArea)
		return mi.rcWork.bottom;	
	else
		return mi.rcMonitor.bottom;
}

static int acSendWindowToPreviousMonitor_glue(lua_State *L){
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acSendWindowToPreviousMonitor((HWND)hWndTarget, start));
	return 1;
}
static int acSendWindowToPreviousMonitor(HWND hWndTarget, POINT start){
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	return acSendWindowToOtherMonitor(hWndTarget, 1);
}

static int acSendWindowToNextMonitor_glue(lua_State *L){
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	lua_pushnumber(L, acSendWindowToNextMonitor((HWND)hWndTarget, start));
	return 1;
}
static int acSendWindowToNextMonitor(HWND hWndTarget, POINT start){
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	return acSendWindowToOtherMonitor(hWndTarget, 2);
}

static int acSendWindowToMonitorByHandle_glue(lua_State *L){
	int hWndTarget = NULL;
	POINT start;
	LONG hMonitor;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	hMonitor = (lua_isnumber(L,4) ? lua_tonumber(L, 4) : 0);
	lua_pushnumber(L, acSendWindowToMonitorByHandle((HWND)hWndTarget, start, (HMONITOR)hMonitor));
	return 1;
}
static int acSendWindowToMonitorByHandle(HWND hWndTarget, POINT start, HMONITOR hMonitor){
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);
	hIncomingMonitor = hMonitor;
	return acSendWindowToOtherMonitor(hWndTarget, 3);
}

static int acSendWindowToMonitorByName_glue(lua_State *L){
	int hWndTarget = NULL;
	POINT start;
	hWndTarget = (lua_isnumber(L,1) ? lua_tonumber(L, 1) : NULL);
	start.x = (lua_isnumber(L,2) ? lua_tonumber(L, 2) : 0);
	start.y = (lua_isnumber(L,3) ? lua_tonumber(L, 3) : 0);
	sMonitorName = (lua_isstring(L,4) ? lua_tostring(L, 4) : _T(""));
	lua_pushnumber(L, acSendWindowToMonitorByName((HWND)hWndTarget, start));
	return 1;
}
static int acSendWindowToMonitorByName(HWND hWndTarget, POINT start){
	if(hWndTarget == NULL)
		hWndTarget = GetAncestor(WindowFromPoint(start),GA_ROOTOWNER);

	return acSendWindowToOtherMonitor(hWndTarget, 4);
}

static int acSendWindowToOtherMonitor(HWND hWndTarget, LONG flag) {
	
	hLastEnumMonitor = NULL;
	hCurrentEnumMonitor = NULL;
	hFirstEnumMonitor = NULL;
	hSendToMonitor = NULL;

	hActiveMonitor = MonitorFromWindow(hWndTarget,MONITOR_DEFAULTTONEAREST);

	EnumDisplayMonitors( NULL, NULL, MonitorsCallBack, (LPARAM)flag);

	if(hSendToMonitor == NULL && flag == 1){
		//if previous, send to very last monitor in enum
		hSendToMonitor = hCurrentEnumMonitor;
	}

	if(hSendToMonitor == NULL && flag == 2){
		//if next, send to very first monitor in enum
		hSendToMonitor = hFirstEnumMonitor;
	}

	if(hSendToMonitor != NULL){
		MONITORINFOEX miA;
		miA.cbSize = sizeof( miA );
		GetMonitorInfo( hActiveMonitor, &miA );

		MONITORINFOEX mi;
		mi.cbSize = sizeof( mi );
		GetMonitorInfo( hSendToMonitor, &mi );
	
		RECT rectWin;
		GetWindowRect(hWndTarget, &rectWin);

		SetWindowPos(hWndTarget, NULL, 
			mi.rcWork.left + (rectWin.left - miA.rcWork.left),
			mi.rcWork.top + (rectWin.top - miA.rcWork.top),
			0,0,SWP_NOSIZE | SWP_NOACTIVATE);
	}
	return 1;
}

static int acRelayGesture() {
	if(LastDrawPoints.size() > 1 && hookbtn >= 0 && hookbtn <= 4)
	{
		POINT currentpos;
		POINT temppoint;
		GetCursorPos(&currentpos);

		acConsumePhysicalInput(true);
		acHideMouseCursor(L);
		temppoint.x = (int)LastDrawPoints[0].x;
		temppoint.y = (int)LastDrawPoints[0].y;
		acMouseMove(temppoint);
		Sleep(3);
		acMouseClick(temppoint,hookbtn,true,false);
		Sleep(3);
		for(int i = 0; i < LastDrawPoints.size(); i++) { 
			temppoint.x = (int)LastDrawPoints[i].x;
			temppoint.y = (int)LastDrawPoints[i].y;
			acMouseMove(temppoint);
			Sleep(1);
		}
		temppoint.x = (int)LastDrawPoints.back().x;
		temppoint.y = (int)LastDrawPoints.back().y;
		acMouseClick(temppoint,hookbtn,false,true);
		Sleep(3);
		acMouseMove(currentpos);
		Sleep(3);
		acShowMouseCursor(L);
		acConsumePhysicalInput(false);
	}
	return 1;
}

static int acRelayGesture_glue(lua_State *L) {
	acRelayGesture();
	return 1;
}

#pragma endregion Action Functions


#pragma region Lua

const char *readMemFile(lua_State *, void *ud, size_t *size)
{
  // Convert the ud pointer (UserData) to a pointer of our structure
  luaMemFile *luaMF = (luaMemFile *) ud;

  // Are we done?
  if(luaMF->size == 0)
    return NULL;

  // Read everything at once
  // And set size to zero to tell the next call we're done
  *size = luaMF->size;
  luaMF->size = 0;

  // Return a pointer to the readed text
  return luaMF->text;
}

void initLuaState(lua_State *LS) {
	luaL_openlibs(LS);
	luaopen_alien_core(LS);
	luaopen_alien_struct(LS);

	lua_register(LS, "acPreviousApplication", acPreviousApplication_glue);
	lua_register(LS, "acNextApplication", acNextApplication_glue);
	lua_register(LS, "acDisable", acDisable_glue);
	lua_register(LS, "acDisableNext", acDisableNext_glue);
	lua_register(LS, "acCloseApplication", acCloseApplication_glue);	
	lua_register(LS, "acMaximizeWindow", acMaximizeWindow_glue);	
	lua_register(LS, "acMaximizeOrRestoreWindow", acMaximizeOrRestoreWindow_glue);	
	lua_register(LS, "acDelay", acDelay_glue);	
	lua_register(LS, "acMoveWindow", acMoveWindow_glue);	
	lua_register(LS, "acMinimizeWindow", acMinimizeWindow_glue);	
	lua_register(LS, "acSendKeys", acSendKeys_glue);	
	lua_register(LS, "acRunProgram", acRunProgram_glue);	
	lua_register(LS, "acSetVolume", acSetVolume_glue);	
	lua_register(LS, "acGetVolume", acGetVolume_glue);	
	lua_register(LS, "acToggleMute", acToggleMute_glue);	
	lua_register(LS, "acGetMute", acGetMute_glue);	
	lua_register(LS, "acSetMute", acSetMute_glue);	
	lua_register(LS, "acGetControlID", acGetControlID_glue);	
	lua_register(LS, "acFindWindow", acFindWindow_glue);	
	lua_register(LS, "acGetWindowTitle", acGetWindowTitle);	
	lua_register(LS, "acMessageBox", acMessageBox_glue);	
	lua_register(LS, "acFindWindowByTitleRegex", acFindWindowByTitleRegex_glue);	
	lua_register(LS, "acSetWindowSize", acSetWindowSize_glue);	
	lua_register(LS, "acSendWindowToBottom", acSendWindowToBottom_glue);	
	lua_register(LS, "acActivateWindow", acActivateWindow_glue);	
	lua_register(LS, "acRestoreWindow", acRestoreWindow_glue);	
	lua_register(LS, "acGetOwnerWindowByPoint", acGetOwnerWindowByPoint_glue);	
	lua_register(LS, "acGetParentWindowByPoint", acGetParentWindowByPoint_glue);	
	lua_register(LS, "acGetWindowByPoint", acGetWindowByPoint_glue);	
	lua_register(LS, "acSetProcessPriority", acSetProcessPriority_glue);	
	lua_register(LS, "acTerminateProcess", acTerminateProcess_glue);	
	lua_register(LS, "acToggleTopmost", acToggleTopmost_glue);	
	lua_register(LS, "acSetTopmost", acSetTopmost_glue);	
	lua_register(LS, "acClearTopmost", acClearTopmost_glue);	
	lua_register(LS, "acPauseResumeThreadList", acPauseResumeThreadList_glue);	
	lua_register(LS, "acGetSystemMetrics", acGetSystemMetrics_glue);	
	lua_register(LS, "acSetWindowTransparency", acSetWindowTransparency_glue);	
	lua_register(LS, "acGetWindowTransparency", acGetWindowTransparency_glue);	
	lua_register(LS, "acSetWindowColorKey", acSetWindowColorKey_glue);	
	lua_register(LS, "acSetWindowTransparencyAndColorKey", acSetWindowTransparencyAndColorKey_glue);	
	lua_register(LS, "acShowSettings", acShowSettings_glue);	
	lua_register(LS, "acShowPrefs", acShowPrefs_glue);	
	lua_register(LS, "acShowHotkeys", acShowHotkeys_glue);	
	lua_register(LS, "acShowActions", acShowActions_glue);	
	lua_register(LS, "acShowIgnoreList", acShowIgnoreList_glue);	
	lua_register(LS, "acGetWindowLeft", acGetWindowLeft_glue);	
	lua_register(LS, "acGetWindowTop", acGetWindowTop_glue);	
	lua_register(LS, "acGetWindowRight", acGetWindowRight_glue);	
	lua_register(LS, "acGetWindowBottom", acGetWindowBottom_glue);	
	lua_register(LS, "acGetWindowColorKeyR", acGetWindowColorKeyR_glue);	
	lua_register(LS, "acGetWindowColorKeyG", acGetWindowColorKeyG_glue);	
	lua_register(LS, "acGetWindowColorKeyB", acGetWindowColorKeyB_glue);	
	lua_register(LS, "acGetPixelRByPoint", acGetPixelRByPoint_glue);	
	lua_register(LS, "acGetPixelGByPoint", acGetPixelGByPoint_glue);	
	lua_register(LS, "acGetPixelBByPoint", acGetPixelBByPoint_glue);	
	lua_register(LS, "acExit", acExit_glue);	
	lua_register(LS, "acReloadConfig", acReloadConfig_glue);	
	lua_register(LS, "acGetMouseLocationX", acGetMouseLocationX_glue);	
	lua_register(LS, "acGetMouseLocationY", acGetMouseLocationY_glue);	
	lua_register(LS, "acMouseMove", acMouseMove_glue);	
	lua_register(LS, "acMouseClick", acMouseClick_glue);	
	lua_register(LS, "acSendMessage", acSendMessage_glue);	
	lua_register(LS, "acPostMessage", acPostMessage_glue);	
	lua_register(LS, "acGetForegroundWindow", acGetForegroundWindow_glue);	
	lua_register(LS, "acGetDesktopWindow", acGetDesktopWindow_glue);	
	lua_register(LS, "acGetClipboardText", acGetClipboardText);	
	lua_register(LS, "acSetClipboardText", acSetClipboardText);	
	lua_register(LS, "acSendWinDown", acSendWinDown_glue);	
	lua_register(LS, "acSendWinUp", acSendWinUp_glue);	
	lua_register(LS, "acSendControlDown", acSendControlDown_glue);	
	lua_register(LS, "acSendControlUp", acSendControlUp_glue);	
	lua_register(LS, "acSendAltDown", acSendAltDown_glue);	
	lua_register(LS, "acSendAltUp", acSendAltUp_glue);	
	lua_register(LS, "acSendShiftDown", acSendShiftDown_glue);	
	lua_register(LS, "acSendShiftUp", acSendShiftUp_glue);	
	lua_register(LS, "acCenterWindowToScreen", acCenterWindowToScreen_glue);	
	lua_register(LS, "acClipWindowToScreen", acClipWindowToScreen_glue);	
	lua_register(LS, "acFitWindowToScreen", acFitWindowToScreen_glue);	
	lua_register(LS, "acMaximizeToAllScreens", acMaximizeToAllScreens_glue);	
	lua_register(LS, "acGetMonitorFromPoint", acGetMonitorFromPoint_glue);	
	lua_register(LS, "acGetMonitorName", acGetMonitorName);	
	lua_register(LS, "acSendWindowToPreviousMonitor", acSendWindowToPreviousMonitor_glue);	
	lua_register(LS, "acSendWindowToMonitorByName", acSendWindowToMonitorByName_glue);	
	lua_register(LS, "acSendWindowToMonitorByHandle", acSendWindowToMonitorByHandle_glue);	
	lua_register(LS, "acSendWindowToNextMonitor", acSendWindowToNextMonitor_glue);	
	lua_register(LS, "acGetMonitorFromName", acGetMonitorFromName_glue);	
	lua_register(LS, "acGetMonitorBottom", acGetMonitorBottom_glue);	
	lua_register(LS, "acGetMonitorTop", acGetMonitorTop_glue);	
	lua_register(LS, "acGetMonitorLeft", acGetMonitorLeft_glue);	
	lua_register(LS, "acGetMonitorRight", acGetMonitorRight_glue);	
	lua_register(LS, "acPlaySound", acPlaySound_glue);	
	lua_register(LS, "acDisplayText", acDisplayText_glue);	
	lua_register(LS, "acGetExecutableName", acGetExecutableName);	
	lua_register(LS, "acGetClassName", acGetClassName);	
	lua_register(LS, "acLockWindows", acLockWindows_glue);	
	lua_register(LS, "acToggleTrayIcon", acToggleTrayIcon_glue);	
	lua_register(LS, "acShellExecute", acShellExecute_glue);	
	lua_register(LS, "acTileWindows", acTileWindows_glue);	
	lua_register(LS, "acShowAbout", acShowAbout_glue);	
	lua_register(LS, "acGetProcessIDFromPattern", acGetProcessIDFromPattern_glue);	
	lua_register(LS, "acGetWindowFromProcessID", acGetWindowFromProcessID_glue);	
	lua_register(LS, "acConsumePhysicalInput", acConsumePhysicalInput_glue);	
	lua_register(LS, "acSetKeyboardHook", acSetKeyboardHook_glue);	
	lua_register(LS, "acGetMouseCursorType", acGetMouseCursorType);	
	lua_register(LS, "acHideMouseCursor", acHideMouseCursor);	
	lua_register(LS, "acShowMouseCursor", acShowMouseCursor);
	lua_register(LS, "acAutoHideMouseCursor", acAutoHideMouseCursor);
	lua_register(LS, "acSendKeyDown", acSendKeyDown);
	lua_register(LS, "acSendKeyUp", acSendKeyUp);
	lua_register(LS, "acRegistryGetString", acRegistryGetString);
	lua_register(LS, "acRegistrySetString", acRegistrySetString);
	lua_register(LS, "acRegistryGetNumber", acRegistryGetNumber);
	lua_register(LS, "acRegistrySetNumber", acRegistrySetNumber);
	lua_register(LS, "acRegistryDeleteValue", acRegistryDeleteValue);
	lua_register(LS, "acRegistryDeleteKey", acRegistryDeleteKey);
	lua_register(LS, "acRegistryCreateKey", acRegistryCreateKey);
	lua_register(LS, "acDisplayBalloonTip", acDisplayBalloonTip);
	lua_register(LS, "acGetFileVersion", acGetFileVersion);
	lua_register(LS, "acGetFileProductVersion", acGetFileProductVersion);
	lua_register(LS, "acSendString", acSendString);
	lua_register(LS, "acInputBox", acInputBox);
	lua_register(LS, "acCreatePopupMenu", acCreatePopupMenu);
	lua_register(LS, "acActivateWindowUnderMouseWheel", acActivateWindowUnderMouseWheel);
	lua_register(LS, "acKillDisplayText", acKillDisplayText);
	lua_register(LS, "acGetAllWindows", acGetAllWindows);
	lua_register(LS, "acIsImmersiveProcess", acIsImmersiveProcess);	
	lua_register(LS, "acEnableCapture", acEnableCapture);	
	lua_register(LS, "acDisableCapture", acDisableCapture);	
	lua_register(LS, "acEnableHotkey", acEnableHotkey);	
	lua_register(LS, "acDisableHotkey", acDisableHotkey);	
	lua_register(LS, "acGetMonitorBrightness", acGetMonitorBrightness);	
	lua_register(LS, "acSetMonitorBrightness", acSetMonitorBrightness);	
	lua_register(LS, "acSetDisplayGamma", acSetDisplayGamma);	
	lua_register(LS, "acRelayGesture", acRelayGesture_glue);	
	lua_register(LS, "acSetNumber", acSetNumber);	
	lua_register(LS, "acGetNumber", acGetNumber);	
	
	//Load global Lua script and execute
	fstream filestr;
	SetCurrentDirectory(strExePath);
	string sLuaPath = strStrokesConfigFile;
	//MessageBox(GetForegroundWindow(), strStrokesConfigFile, "initLua", NULL);
	boost::replace_all(sLuaPath, _T(".xml"), _T(".lua"));
	filestr.open(sLuaPath, fstream::in);
	std::stringstream sInitLua;
	if(filestr){
		sInitLua << filestr.rdbuf();
	}
	filestr.close();
	
	//sInitLua.
	if(!sInitLua.str().empty()) {
		std::basic_string<TCHAR> sLua = sInitLua.str();

		// Use LUA...
		luaMemFile luaMFB;
		// Load the command and try to execute it...
		luaMFB.text = sLua.c_str();
		luaMFB.size = _tcslen(luaMFB.text);
		if(lua_load(LS, readMemFile, &luaMFB, _T("InitLuaScript"),NULL) == 0)
		{
			// Execute the loaded command...
			// The function takes 0 parameters and will return 1 result
			if(lua_pcall(LS, 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber(LS, lua_gettop(L));

				//Setup calling sp_init()
				luaMemFile luaMFI;
				// Load the command and try to execute it...
				std::basic_string<TCHAR> sLuaspinit = "if (sp_init ~= nil) then\n  sp_init()\n  end";
				luaMFI.text = sLuaspinit.c_str();
				luaMFI.size = _tcslen(luaMFI.text);
				if(lua_load(LS, readMemFile, &luaMFI, _T("InitLuaScript_sp_init"),NULL) == 0)
				{
					if(lua_pcall(LS, 0, 1, 0) == 0)
					{
						// There was no error
						// Let's get the result from the stack
						int result = lua_tonumber(LS, lua_gettop(L));
					}
				}
			} else {
				MessageBox(GetForegroundWindow(), lua_tostring(LS, -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
			}

			// Clean-up the stack
			lua_pop(LS, 1);
		} else {
			// There was a lua_load error...
			// Pop the error value from the stack
			MessageBox(GetForegroundWindow(), lua_tostring(LS, -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
			//MessageBox(GetForegroundWindow(), luaMFB.text, _T("Script"), NULL);
			lua_pop(LS, 1);
		}

		// Verify the stack
		// (don't forget to include crtdbg.h)
		//_ASSERT(lua_gettop(L) == 0);
	}
}

void initLua()
{
	// Open the first (default/primary) LUA state

	L = luaL_newstate();
	L2 = luaL_newstate();
	initLuaState(L);
	initLuaState(L2);
}

#pragma endregion Lua


#pragma region Gesture Match Math

double GetAngularGradient(POINTF LineStartPoint, POINTF LineEndPoint)
{
	return atan2((double)(LineEndPoint.y - LineStartPoint.y), (double)(LineEndPoint.x - LineStartPoint.x));
}

double GetAngularDelta(double Angle1, double Angle2)
{
	double retValue = abs(Angle1 - Angle2);

	if (retValue > M_PI )
		retValue = M_PI - (retValue - M_PI);

	return retValue;
}

double GetProbabilityFromAngularDelta(double AngularDelta)
{
	const double dScale = 31.830988618379067;
	return abs(AngularDelta * dScale - 100);
}

double GetDegreeFromRadian(double Angle)
{
	return Angle * (180.0 / M_PI);
}

double GetDistance(POINTF LineStartPoint, POINTF LineEndPoint)
{
	return sqrt(pow(LineEndPoint.x - LineStartPoint.x,2.0F) + pow(LineEndPoint.y - LineStartPoint.y, 2.0F)); 
}

double GetPointArrayLength(vector<POINTF> Points)
{
	// Create return variable to hold final calculated length
	double returnLength = 0;

	// Enumerate points in point pattern and get a sum of each line segments distances
	for (int currentIndex = 1; currentIndex < Points.size(); currentIndex++)
		returnLength += GetDistance(Points[currentIndex - 1], Points[currentIndex]);

	// Return calculated length
	return returnLength;
}

POINTF GetInterpolatedPoint(POINTF LineStartPoint, POINTF LineEndPoint, double InterpolatePosition)
{
	// Create return point
	POINTF pReturn;

	// Calculate x and y of increment point
	pReturn.x = (float)((1 - InterpolatePosition) * (float)LineStartPoint.x + InterpolatePosition * (float)LineEndPoint.x);
	pReturn.y = (float)((1 - InterpolatePosition) * (float)LineStartPoint.y + InterpolatePosition * (float)LineEndPoint.y);

	// Return new point
	return pReturn;
}

vector<double> GetPointArrayAngularMargins(vector<POINTF> PointArray)
{
	// Create an empty collection of angles
	vector<double> angularMargins;

	// Enumerate input point array starting with second point and calculate angular margin
	for (int currentIndex = 1; currentIndex < PointArray.size(); currentIndex++)
		angularMargins.push_back(GetAngularGradient(PointArray[currentIndex - 1], PointArray[currentIndex]));

	// Return angular margins array
	return angularMargins;
}

vector<POINTF> GetInterpolatedPointArray(vector<POINTF> Points, int Segments, bool bActualGesture)
{
	// Create an empty return collection to store interpolated points
	vector<POINTF> interpolatedPoints;

	// Precalculate desired segment length and define helper variables
	double desiredSegmentLength = GetPointArrayLength(Points) / Segments;
	double dCurrSegmentLength, dTestSegmentLength, dIncToCurrentlength, dInterpolationPosition;
	POINTF currentPoint;
	dCurrSegmentLength = 0;	// Initialize to zero
	
	// Add first point in point pattern to return array and save it for use in the interpolation process
	POINTF lastTestPoint = Points[0]; // Initialize to first point in point pattern

	if(bActualGesture){
		iBoundingLeft = lastTestPoint.x;
		iBoundingTop = lastTestPoint.y;
		iBoundingRight = lastTestPoint.x;
		iBoundingBottom = lastTestPoint.y;
	}

	interpolatedPoints.push_back(lastTestPoint);

	// Enumerate points starting with second point (if any)
	for (int currentIndex = 1; currentIndex < Points.size(); currentIndex++)
	{
		// Store current index point in helper variable
		currentPoint = Points[currentIndex];

		if(bActualGesture){
			if(currentPoint.x <= iBoundingLeft)
				iBoundingLeft = currentPoint.x;
			if(currentPoint.y <= iBoundingTop)
				iBoundingTop = currentPoint.y;
			if(currentPoint.x >= iBoundingRight)
				iBoundingRight = currentPoint.x;
			if(currentPoint.y >= iBoundingBottom)
				iBoundingBottom = currentPoint.y;
		}

		// Calculate distance between last added point and current point in point pattern
		// and use calculated length to calculate test segment length for next point to add
		dIncToCurrentlength = GetDistance(lastTestPoint, currentPoint);
		dTestSegmentLength = dCurrSegmentLength + dIncToCurrentlength;

		// Does the test segment length meet our desired length requirement
		
		if (dTestSegmentLength < desiredSegmentLength)
		{
			// Desired segment length has not been satisfied so we don't need to add an interpolated point
			// save this test point and move on to next test point
			dCurrSegmentLength = dTestSegmentLength;
			lastTestPoint = currentPoint;
			continue;
		}
		

		// Test segment length has met or exceeded our desired segment length
		// so lets calculate how far we overshot our desired segment length and calculate
		// an interpolation position to use to derive our new interpolation point
		dInterpolationPosition = (desiredSegmentLength - dCurrSegmentLength) * (1 / dIncToCurrentlength);

		// Use interpolation position to derive our new interpolation point
		POINTF interpolatedPoint = GetInterpolatedPoint(lastTestPoint, currentPoint, dInterpolationPosition);
		interpolatedPoints.push_back(interpolatedPoint);

		// Sometimes rounding errors cause us to attempt to add more points than the user has requested.
		// If we've reached our segment count limit, exit loop
		if (interpolatedPoints.size() == Segments)
			break;

		// Store new interpolated point as last test point for use in next segment calculations
		// reset current segment length and jump back to the last index because we aren't done with original line segment
		lastTestPoint = interpolatedPoint;
		dCurrSegmentLength = 0;
		currentIndex--;
	}
	
	// Return interpolated point array
	return interpolatedPoints;
}

#pragma endregion Gesture Match Math


#pragma region Gesture Match and Execute

	//loop through all actions until hotkeyID match, then exec script
DWORD WINAPI gFireHotkey( LPVOID lpParam )
{
	EnterCriticalSection(&CriticalSection);

	int iHotkeyID = (int) lpParam;

	if(bIgnoreGesture){
		bIgnoreGesture = FALSE;
		PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
	}

	std::basic_string<TCHAR> ActionScript = "local hkid = "+boost::lexical_cast<std::basic_string<TCHAR>>(iHotkeyID)+"\n";

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
		if(v2.second.get("<xmlattr>.enabled",0) == 1) {
			if(iHotkeyID == v2.second.get("HotkeyID",0)){
				ActionScript = ActionScript.append(v2.second.get("Lua",_T("")));
				break;
			}
		}
	}

	bool bUseState2 = bLuaState1Firing;
	if(bUseState2){
		LOGMESSAGE("FireHotkey: Main script - Using state 2");
		bLuaState2Firing = true;
	} else {
		LOGMESSAGE("FireHotkey: Main script - Using state 1");
		bLuaState1Firing = true;
	}

	if(!ActionScript.empty())
	{

		// Use LUA...
		luaMemFile luaMF;

		// Load the command and try to execute it...
		luaMF.text = ActionScript.c_str();
		luaMF.size = _tcslen(luaMF.text);

		if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMF, _T("ActionScript"), NULL) == 0)
		{

			// Execute the loaded command...
			// The function takes 0 parameters and will return 1 result
			if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));

			} else {
				MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
			}

			// Clean-up the stack
			lua_pop((bUseState2 ? L2 : L), 1);
		} else {
			// There was a lua_load error...
			// Pop the error value from the stack
			MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
			lua_pop((bUseState2 ? L2 : L), 1);
		}
	}

	if(bUseState2){
		LOGMESSAGE("FireHotkey: (After script) Release Lua state 2");
		bLuaState2Firing = false;
	} else {
		LOGMESSAGE("FireHotkey: (After script) Release Lua state 1");
		bLuaState1Firing = false;
	}

	acDelay(50);

	LeaveCriticalSection(&CriticalSection);

	return 1;

}

__declspec(dllexport) void FireHotkey(int iHotkeyID){
	HANDLE hThread = CreateThread(NULL, NULL, &gFireHotkey, (LPVOID)iHotkeyID, 0, NULL); 
	CloseHandle(hThread);
}

void clearCaptureVars()
{
	bIgnoreGesture = FALSE;
	dWheelDelta = 0;
	bMouseWheelUp = FALSE;
	bMouseWheelDown = FALSE;
	bLeftMouseDown = FALSE;
	bMiddleMouseDown = FALSE;
	bRightMouseDown = FALSE;
	bX1MouseDown = FALSE;
	bX2MouseDown = FALSE;
	bControlDown = FALSE;
	bAltDown = FALSE;
	bShiftDown = FALSE;
	bLeftMouseDownBefore = FALSE;
	bMiddleMouseDownBefore = FALSE;
	bRightMouseDownBefore = FALSE;
	bX1MouseDownBefore = FALSE;
	bX2MouseDownBefore = FALSE;
	bControlDownBefore = FALSE;
	bAltDownBefore = FALSE;
	bShiftDownBefore = FALSE;
	if(DrawPoints.size() > 0) {
		LastDrawnGestureName = DrawnGestureName;
		LastDrawPoints.clear();
		for(int i =0; i < DrawPoints.size(); i++) { 
			LastDrawPoints.push_back(DrawPoints[i]);
		}
	}
	DrawPoints.clear();
	DrawnGestureName = _T("");
	LOGMESSAGE("DrawnGestureName Cleared - clearCaptureVars()");
	MatchedAppName = _T("");
	MatchedActionName = _T("");
	iBoundingLeft = 0;
	iBoundingTop = 0;
	iBoundingRight = 0;
	iBoundingBottom = 0;
	//SetPriorityClass(GetCurrentProcess(),ABOVE_NORMAL_PRIORITY_CLASS);
}

std::basic_string<TCHAR> GetActionScript(HWND hWndTarget, BOOL bCheckForIgnored
#ifdef SP_NOGLOBALACTIONS
	, BOOL bCheckForDefinedApp
#endif
)
{
	using boost::property_tree::ptree;

	MatchedAppName = _T("");
	MatchedActionName = _T("");
	TCHAR szOwnerClassName[256];
	TCHAR szOwnerTitle[256];
	TCHAR szParentClassName[256];
	TCHAR szParentTitle[256];
	TCHAR szControlClassName[256];
	TCHAR szControlTitle[256];
	TCHAR szFileName[_MAX_PATH];
	TCHAR szFilePath[_MAX_PATH];
	int iControlID = 0;
	memset(szOwnerClassName,0,sizeof(szOwnerClassName));
	memset(szOwnerTitle,0,sizeof(szOwnerTitle));
	memset(szParentClassName,0,sizeof(szParentClassName));
	memset(szParentTitle,0,sizeof(szParentTitle));
	memset(szControlClassName,0,sizeof(szControlClassName));
	memset(szControlTitle,0,sizeof(szControlTitle));
	memset(szFileName,0,sizeof(szFileName));
	memset(szFilePath,0,sizeof(szFilePath));
	
	DWORD processID = 0;
	
	TCHAR drive[MAX_PATH] = {0};
	TCHAR dir[MAX_PATH] = {0};
	TCHAR fname[MAX_PATH] = {0};
	TCHAR ext[MAX_PATH] = {0};

	BOOL bOwnerClassStringMatch = FALSE;
	BOOL bOwnerClassPatternMatch = FALSE;
	BOOL bOwnerTitleStringMatch = FALSE;
	BOOL bOwnerTitlePatternMatch = FALSE;
	BOOL bParentClassStringMatch = FALSE;
	BOOL bParentClassPatternMatch = FALSE;
	BOOL bParentTitleStringMatch = FALSE;
	BOOL bParentTitlePatternMatch = FALSE;
	BOOL bControlClassStringMatch = FALSE;
	BOOL bControlClassPatternMatch = FALSE;
	BOOL bControlTitleStringMatch = FALSE;
	BOOL bControlTitlePatternMatch = FALSE;
	BOOL bControlIDMatch = FALSE;
	BOOL bFileStringMatch = FALSE;
	BOOL bFilePatternMatch = FALSE;
	BOOL bPathPatternMatch = FALSE;
	BOOL bOneValidSettingFound = FALSE;
	BOOL bAppSettingValid = FALSE;
	BOOL bDoesIgnoreDisable = false;

	GetClassName(GetAncestor(hWndTarget, GA_ROOTOWNER),szOwnerClassName,sizeof(szOwnerClassName));
	GetWindowText(GetAncestor(hWndTarget, GA_ROOTOWNER),szOwnerTitle,sizeof(szOwnerTitle));
	GetClassName(GetAncestor(hWndTarget, GA_ROOT),szParentClassName,sizeof(szParentClassName));
	GetWindowText(GetAncestor(hWndTarget, GA_ROOT),szParentTitle,sizeof(szParentTitle));
	GetClassName(hWndTarget,szControlClassName,sizeof(szControlClassName));
	GetWindowText(hWndTarget,szControlTitle,sizeof(szControlTitle));
	GetClassName(hWndTarget,szControlClassName,sizeof(szControlClassName));
	GetWindowText(hWndTarget,szControlTitle,sizeof(szControlTitle));
	GetWindowThreadProcessId(hWndTarget, &processID);
	iControlID = GetDlgCtrlID(hWndTarget);
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	GetProcessImageFileName( hProcess, szFileName, sizeof(szFileName) / sizeof(TCHAR));
	CloseHandle(hProcess);
	//GetModuleFileNameExA(hProcess, NULL, szFileName, sizeof(szFileName) / sizeof(TCHAR));
	_tsplitpath(szFileName, drive, dir, fname, ext);
	_tcscat(szFilePath,drive);
	_tcscat(szFilePath,dir);
	memset(szFileName,0,sizeof(szFileName));
	_tcscat(szFileName,fname);
	_tcscat(szFileName,ext);

	/*if(_tcsicmp( szFileName, "notepad.exe" ) == 0 ){
		MessageBox(NULL,szFilePath,"",NULL);
	}*/

	/*
	MessageBoxA(NULL,szOwnerClassName,NULL,MB_OK);
	MessageBoxA(NULL,szOwnerTitle,NULL,MB_OK);
	MessageBoxA(NULL,szParentClassName,NULL,MB_OK);
	MessageBoxA(NULL,szParentTitle,NULL,MB_OK);
	MessageBoxA(NULL,szControlClassName,NULL,MB_OK);
	MessageBoxA(NULL,szControlTitle,NULL,MB_OK);
	*/
			
	BOOST_FOREACH(ptree::value_type &v2, ptConfig.get_child((bCheckForIgnored == FALSE ? "config.Applications" : _T("config.Ignored")))) {
		//each application

		//if(!bCheckForIgnored)
			//MessageBoxA(NULL,szFileName,NULL,MB_OK);

		if(v2.second.get("<xmlattr>.enabled",1) == 1){

		MatchedAppName = v2.second.get("<xmlattr>.name",_T(""));
		bAppSettingValid = FALSE;
		bOwnerClassStringMatch = FALSE;
		bOwnerClassPatternMatch = FALSE;
		bOwnerTitleStringMatch = FALSE;
		bOwnerTitlePatternMatch = FALSE;
		bParentClassStringMatch = FALSE;
		bParentClassPatternMatch = FALSE;
		bParentTitleStringMatch = FALSE;
		bParentTitlePatternMatch = FALSE;
		bControlClassStringMatch = FALSE;
		bControlClassPatternMatch = FALSE;
		bControlTitleStringMatch = FALSE;
		bControlTitlePatternMatch = FALSE;
		bControlIDMatch = FALSE;
		bFileStringMatch = FALSE;
		bFilePatternMatch = FALSE;
		bPathPatternMatch = FALSE;

		BOOST_FOREACH(ptree::value_type &v3, v2.second) {
			//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
			if(v3.first == _T("MatchCriteria")){

				if(v3.second.get("ownerclass",_T("")) != _T("")
					|| v3.second.get("ownerclassregex",_T("")) != _T("")
					|| v3.second.get("ownertitle",_T("")) != _T("")
					|| v3.second.get("ownertitleregex",_T("")) != _T("")
					|| v3.second.get("parentclass",_T("")) != _T("")
					|| v3.second.get("parentclassregex",_T("")) != _T("")
					|| v3.second.get("parenttitle",_T("")) != _T("")
					|| v3.second.get("parenttitleregex",_T("")) != _T("")
					|| v3.second.get("controlclass",_T("")) != _T("")
					|| v3.second.get("controlclassregex",_T("")) != _T("")
					|| v3.second.get("controltitle",_T("")) != _T("")
					|| v3.second.get("controltitleregex",_T("")) != _T("")
					|| v3.second.get("controlid",0) != 0
					|| v3.second.get("file",_T("")) != _T("")
					|| v3.second.get("fileregex",_T("")) != _T("")
					|| v3.second.get("pathregex",_T("")) != _T(""))
					{
						if(v3.second.get("ownerclass",_T("")) == _T("")) {
							bOwnerClassStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szOwnerClassName, v3.second.get("ownerclass",_T("")).c_str() ) == 0 ){
								bOwnerClassStringMatch = TRUE;
							}
						}
							
						if(v3.second.get("ownerclassregex",_T("")) == _T("")) {
							bOwnerClassPatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							//boost::wregex e(v3.second.get("ownerclassregex",_T("")).c_str());
							boost::regex e(v3.second.get("ownerclassregex",_T("")).c_str());
							if(boost::regex_match(szOwnerClassName, e ) == TRUE){
								bOwnerClassPatternMatch = TRUE;
							}
						}
							
						if(v3.second.get("ownertitle",_T("")) == _T("")) {
							bOwnerTitleStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szOwnerTitle, v3.second.get("ownertitle",_T("")).c_str() ) == 0 ){
								bOwnerTitleStringMatch = TRUE;
							}
						}

						if(v3.second.get("ownertitleregex",_T("")) == _T("")) {
							bOwnerTitlePatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("ownertitleregex",_T("")).c_str());
							if(boost::regex_match(szOwnerTitle, e) == TRUE){
								bOwnerTitlePatternMatch = TRUE;
							}
						}

						if(v3.second.get("parentclass",_T("")) == _T("")) {
							bParentClassStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szParentClassName, v3.second.get("parentclass",_T("")).c_str() ) == 0 ){
								bParentClassStringMatch = TRUE;
							}
						}

						if(v3.second.get("parentclassregex",_T("")) == _T("")) {
							bParentClassPatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							static boost::regex e(v3.second.get("parentclassregex",_T("")).c_str());
							if(boost::regex_match(szParentClassName, e ) == TRUE){
								bParentClassPatternMatch = TRUE;
							}
						}

						if(v3.second.get("parenttitle",_T("")) == _T("")) {
							bParentTitleStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szParentTitle, v3.second.get("parenttitle",_T("")).c_str() ) == 0 ){
								bParentTitleStringMatch = TRUE;
							}
						}

						if(v3.second.get("parenttitleregex",_T("")) == _T("")) {
							bParentTitlePatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("parenttitleregex",_T("")).c_str());
							if(boost::regex_match(szParentTitle, e ) == TRUE){
								bParentTitlePatternMatch = TRUE;
							}
						}

						if(v3.second.get("controlclass",_T("")) == _T("")) {
							bControlClassStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szControlClassName, v3.second.get("controlclass",_T("")).c_str() ) == 0 ){
								bControlClassStringMatch = TRUE;
							}
						}

						if(v3.second.get("controlclassregex",_T("")) == _T("")) {
							bControlClassPatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("controlclassregex",_T("")).c_str());
							if(boost::regex_match(szControlClassName, e ) == TRUE){
								bControlClassPatternMatch = TRUE;
							}
						}

						if(v3.second.get("controltitle",_T("")) == _T("")) {
							bControlTitleStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szControlTitle, v3.second.get("controltitle",_T("")).c_str() ) == 0 ){
								bControlTitleStringMatch = TRUE;
							}
						}

						if(v3.second.get("controltitleregex",_T("")) == _T("")) {
							bControlTitlePatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("controltitleregex",_T("")).c_str());
							if(boost::regex_match(szControlTitle, e ) == TRUE){
								bControlTitlePatternMatch = TRUE;
							}
						}

						if(v3.second.get("controlid",0) == 0) {
							bControlIDMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(iControlID == v3.second.get("controlid",0)){
								bControlIDMatch = TRUE;
							}
						}

						if(v3.second.get("file",_T("")) == _T("")) {
							bFileStringMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							if(_tcsicmp( szFileName, v3.second.get("file",_T("")).c_str() ) == 0 ){
								bFileStringMatch = TRUE;
							}
						}

						if(v3.second.get("fileregex",_T("")) == _T("")) {
							bFilePatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("fileregex",_T("")).c_str());
							if(boost::regex_match(szFileName, e ) == TRUE){
								bFilePatternMatch = TRUE;
							}
						}

						if(v3.second.get("pathregex",_T("")) == _T("")) {
							bPathPatternMatch = TRUE;
						} else {
							bAppSettingValid = TRUE;
							boost::regex e(v3.second.get("pathregex",_T("")).c_str());
							if(boost::regex_match(szFilePath, e ) == TRUE){
								bPathPatternMatch = TRUE;
							}
						}

						if(v3.second.get("disableonactivate",0) == 1){
							bDoesIgnoreDisable = true;
						}
					}
				}
			}

			if(bAppSettingValid 
				&& bOwnerClassStringMatch 
				&& bOwnerClassPatternMatch 
				&& bOwnerTitleStringMatch 
				&& bOwnerTitlePatternMatch 
				&& bParentClassStringMatch 
				&& bParentClassPatternMatch 
				&& bParentTitleStringMatch 
				&& bParentTitlePatternMatch 
				&& bControlClassStringMatch 
				&& bControlClassPatternMatch 
				&& bControlIDMatch
				&& bControlTitleStringMatch 
				&& bControlTitlePatternMatch 
				&& bFileStringMatch 
				&& bFilePatternMatch
				&& bPathPatternMatch)
			{
			#ifdef SP_NOGLOBALACTIONS
				if(bCheckForDefinedApp) 
				{
					return _T("APPDEFINED");
				}
				else 
			#endif
				if(bCheckForIgnored){
					if(bDoesIgnoreDisable){
						bIgnoreDisable = true;
					}
					return _T("IGNORED");
				}
				else
				{
					BOOST_FOREACH(ptree::value_type &v3, v2.second) {
						//each match (technically, everything at the Actions level, TODO: optimize, no reason to loop
						if(v3.first == "Actions"){
							BOOST_FOREACH(ptree::value_type &v4, v3.second) {
								//each action
								if(v4.second.get("<xmlattr>.enabled",0) == 1) {
									MatchedActionName = v4.second.get("<xmlattr>.description",_T(""));
									//if(v4.second.get("GestureName",_T("")) == DrawnGestureName) {
									if(_tcscmp( v4.second.get("GestureName",_T("")).c_str(), DrawnGestureName.c_str() ) == 0) {
										if(v4.second.get("Control",FALSE) == (!bCaptureModifiersOnMouseDown ? bControlDown : (v4.second.get("StrokeState",0) == 0 ? (bControlDownBefore || bControlDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bControlDownBefore : (v4.second.get("StrokeState",0) == 2 ? bControlDown : FALSE))))
											&& v4.second.get("Alt",FALSE) == (!bCaptureModifiersOnMouseDown ? bAltDown : (v4.second.get("StrokeState",0) == 0 ? (bAltDownBefore || bAltDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bAltDownBefore : (v4.second.get("StrokeState",0) == 2 ? bAltDown : FALSE))))
											&& v4.second.get("Shift",FALSE) == (!bCaptureModifiersOnMouseDown ? bShiftDown : (v4.second.get("StrokeState",0) == 0 ? (bShiftDownBefore || bShiftDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bShiftDownBefore : (v4.second.get("StrokeState",0) == 2 ? bShiftDown : FALSE))))
											&& v4.second.get("Left",FALSE) == (!bCaptureModifiersOnMouseDown ? bLeftMouseDown : (v4.second.get("StrokeState",0) == 0 ? (bLeftMouseDownBefore || bLeftMouseDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bLeftMouseDownBefore : (v4.second.get("StrokeState",0) == 2 ? bLeftMouseDown : FALSE))))
											&& v4.second.get("Middle",FALSE) == (!bCaptureModifiersOnMouseDown ? bMiddleMouseDown : (v4.second.get("StrokeState",0) == 0 ? (bMiddleMouseDownBefore || bMiddleMouseDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bMiddleMouseDownBefore : (v4.second.get("StrokeState",0) == 2 ? bMiddleMouseDown : FALSE))))
											&& v4.second.get("Right",FALSE) == (!bCaptureModifiersOnMouseDown ? bRightMouseDown : (v4.second.get("StrokeState",0) == 0 ? (bRightMouseDownBefore || bRightMouseDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bRightMouseDownBefore : (v4.second.get("StrokeState",0) == 2 ? bRightMouseDown : FALSE))))
											&& v4.second.get("X1",FALSE) == (!bCaptureModifiersOnMouseDown ? bX1MouseDown : (v4.second.get("StrokeState",0) == 0 ? (bX1MouseDownBefore || bX1MouseDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bX1MouseDownBefore : (v4.second.get("StrokeState",0) == 2 ? bX1MouseDown : FALSE))))
											&& v4.second.get("X2",FALSE) == (!bCaptureModifiersOnMouseDown ? bX2MouseDown : (v4.second.get("StrokeState",0) == 0 ? (bX2MouseDownBefore || bX2MouseDown ? TRUE : FALSE) : (v4.second.get("StrokeState",0) == 1 ? bX2MouseDownBefore : (v4.second.get("StrokeState",0) == 2 ? bX2MouseDown : FALSE))))
											&& v4.second.get("WheelDown",FALSE) == bMouseWheelDown
											&& v4.second.get("WheelUp",FALSE) == bMouseWheelUp) 
										{
											return v4.second.get("Lua",_T(""));
										}										
									}
								}
							}
						}
					}
				}
				if(v2.second.get("<xmlattr>.noglobal",0) == 1){
					return _T("");
				}
			} else {
				bDoesIgnoreDisable = false;
			}
		}
	}


	if(bCheckForIgnored
		#ifdef SP_NOGLOBALACTIONS
			 || bCheckForDefinedApp || bOnlyAllowDefinedApps
		#endif
			 )
		return _T("");
	else
	{
		MatchedAppName = ptLang.get("language.Actions.GlobalActions",_T("Global Actions"));
		//No app match found, test for global action match
		BOOST_FOREACH(ptree::value_type &vg2, ptConfig.get_child(_T("config.Global.Actions"))) {
			//each global action
			if(vg2.second.get("<xmlattr>.enabled",0) == 1) {
				//if(vg2.second.get("GestureName",_T("")) == DrawnGestureName) {
				if(_tcscmp( vg2.second.get("GestureName",_T("")).c_str(), DrawnGestureName.c_str() ) == 0){
					if(vg2.second.get("Control",FALSE) == (!bCaptureModifiersOnMouseDown ? bControlDown : (vg2.second.get("StrokeState",0) == 0 ? (bControlDownBefore || bControlDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bControlDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bControlDown : FALSE))))
						&& vg2.second.get("Alt",FALSE) == (!bCaptureModifiersOnMouseDown ? bAltDown : (vg2.second.get("StrokeState",0) == 0 ? (bAltDownBefore || bAltDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bAltDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bAltDown : FALSE))))
						&& vg2.second.get("Shift",FALSE) == (!bCaptureModifiersOnMouseDown ? bShiftDown : (vg2.second.get("StrokeState",0) == 0 ? (bShiftDownBefore || bShiftDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bShiftDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bShiftDown : FALSE))))
						&& vg2.second.get("Left",FALSE) == (!bCaptureModifiersOnMouseDown ? bLeftMouseDown : (vg2.second.get("StrokeState",0) == 0 ? (bLeftMouseDownBefore || bLeftMouseDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bLeftMouseDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bLeftMouseDown : FALSE))))
						&& vg2.second.get("Middle",FALSE) == (!bCaptureModifiersOnMouseDown ? bMiddleMouseDown : (vg2.second.get("StrokeState",0) == 0 ? (bMiddleMouseDownBefore || bMiddleMouseDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bMiddleMouseDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bMiddleMouseDown : FALSE))))
						&& vg2.second.get("Right",FALSE) == (!bCaptureModifiersOnMouseDown ? bRightMouseDown : (vg2.second.get("StrokeState",0) == 0 ? (bRightMouseDownBefore || bRightMouseDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bRightMouseDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bRightMouseDown : FALSE))))
						&& vg2.second.get("X1",FALSE) == (!bCaptureModifiersOnMouseDown ? bX1MouseDown : (vg2.second.get("StrokeState",0) == 0 ? (bX1MouseDownBefore || bX1MouseDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bX1MouseDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bX1MouseDown : FALSE))))
						&& vg2.second.get("X2",FALSE) == (!bCaptureModifiersOnMouseDown ? bX2MouseDown : (vg2.second.get("StrokeState",0) == 0 ? (bX2MouseDownBefore || bX2MouseDown ? TRUE : FALSE) : (vg2.second.get("StrokeState",0) == 1 ? bX2MouseDownBefore : (vg2.second.get("StrokeState",0) == 2 ? bX2MouseDown : FALSE))))
						&& vg2.second.get("WheelDown",FALSE) == bMouseWheelDown
						&& vg2.second.get("WheelUp",FALSE) == bMouseWheelUp) 
					{
						MatchedActionName = vg2.second.get("<xmlattr>.description",_T(""));
						return vg2.second.get("Lua",_T(""));
				
					}
				}										
			}
		}
	}

	return _T("");
	
}

std::basic_string<TCHAR> GetGestureName( vector<POINTF> Points )
{
	std::basic_string<TCHAR> HighestMatchName = _T("");
	double HighestMatchProbability = (double)MatchProbabilityThreshold;
	int Precision = iPrecision;
	vector<double> Probabilities;

	using boost::property_tree::ptree;

	BOOST_FOREACH(ptree::value_type &v2, ptConfig.get_child("config.Gestures")) {

		//each gesture 
		double ProbAvg = 0;

		BOOST_FOREACH(ptree::value_type &v3, v2.second) {
			//get point Patterns
			if(v3.first == _T("PointPatterns")) {
				BOOST_FOREACH(ptree::value_type &v4, v3.second) {
					//each point pattern
					double DeltaAvg = 0;
					vector<double> aDeltas(Precision,0);
					vector<POINTF> CompareToPoints;
					if(v4.first == _T("PointPattern")) {
						BOOST_FOREACH(ptree::value_type &v5, v4.second) {
							//each point 
							if(v5.first == "Point") {
								POINTF Point;
								int x = (int)v5.second.get("<xmlattr>.x",0);
								int y = (int)v5.second.get("<xmlattr>.y",0);
								Point.x = (float)x;
								Point.y = (float)y;
								CompareToPoints.push_back(Point);						
							}
						}
						vector<double> aCompareToAngles = GetPointArrayAngularMargins(GetInterpolatedPointArray(CompareToPoints, Precision, false));
						vector<double> aCompareAngles = GetPointArrayAngularMargins(GetInterpolatedPointArray(Points, Precision, true));

						for(int k = 0; k < aCompareToAngles.size(); k++) { 
							aDeltas[k] = GetAngularDelta(aCompareToAngles[k], aCompareAngles[k]);
						}

						for(int k = 0;k<aDeltas.size();k++)
							DeltaAvg += aDeltas[k];
					}
					Probabilities.push_back(GetProbabilityFromAngularDelta(DeltaAvg / aDeltas.size()));
				}
			}
		}

		for(int j = 0;j<Probabilities.size();j++) {
			ProbAvg += Probabilities[j];
		}

		ProbAvg = ProbAvg / Probabilities.size();

		if(ProbAvg > HighestMatchProbability && ProbAvg > 0)
		{
			HighestMatchName = v2.second.get("<xmlattr>.name",_T(""));
			HighestMatchProbability = ProbAvg;
		}
		Probabilities.clear();
	}
		
	return HighestMatchName;
}

DWORD WINAPI gPopupMenuClick ( LPVOID lpParam ) 
{
	
	while(bLuaState1Firing && bLuaState2Firing){
		Sleep(5);
	}
	
	bool bUseState2 = (bLuaState1Firing ? true : false);
	if(bUseState2){
		LOGMESSAGE("gPopupMenuClick: Using state 2");
		bLuaState2Firing = true;
	} else {
		LOGMESSAGE("gPopupMenuClick: Using state 1");
		bLuaState1Firing = true;
	}
	
	//Setup calling sp_after_release()
	luaMemFile luaMFI;
	// Load the command and try to execute it...
	std::basic_string<TCHAR> sMenuID = boost::lexical_cast<string>(popupMenuItemID);
	std::basic_string<TCHAR> sLuaspinit = "if (";
	sLuaspinit.append(popupMenuFunctionName).append(" ~= nil) then\n  ").append(popupMenuFunctionName).append("(").append(sMenuID).append(")\n  end");
	luaMFI.text = sLuaspinit.c_str();
	luaMFI.size = _tcslen(luaMFI.text);
	
	if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMFI, _T(popupMenuFunctionName.c_str()),NULL) == 0)
	{
		if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
		{
			// There was no error
			// Let's get the result from the stack
			int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));
		}
	}
	
	if(bUseState2){
		LOGMESSAGE("gPopupMenuClick: Release Lua state 2");
		bLuaState2Firing = false;
	} else {
		LOGMESSAGE("gPopupMenuClick: Release Lua state 1");
		bLuaState1Firing = false;
	}

	/*
	popupMenuItemID = 0;
	popx = 0;
	popy = 0;
	menuItems = "";
	menuDelim = "";
	popupMenuFunctionName = "";
	*/

	return 0;
}

#ifdef SP_WHEEL_HOOK
	DWORD WINAPI gWheelTick ( LPVOID lpParam ) 
	{

		WheelTick *wheelTick = static_cast<WheelTick*>(lpParam);

		while(bLuaState1Firing && bLuaState2Firing){
			Sleep(5);
		}

		EnterCriticalSection(&WheelTickCriticalSection);

		EnterCriticalSection(&CriticalSection);

		bool bUseState2 = (bLuaState1Firing ? true : false);
		if(bUseState2){
			LOGMESSAGE("gWheelTick: Using state 2");
			bLuaState2Firing = true;
		} else {
			LOGMESSAGE("gWheelTick: Using state 1");
			bLuaState1Firing = true;
		}

		LeaveCriticalSection(&CriticalSection);
	
		//Setup calling sp_after_release()
		luaMemFile luaMFI;
		// Load the command and try to execute it...
		std::basic_string<TCHAR> sLuaspinit = "if (sp_"+boost::lexical_cast<string>((wheelTick->Horizontal ? "h_" : ""))+"wheel_tick ~= nil) then\n  sp_"+boost::lexical_cast<string>((wheelTick->Horizontal ? "h_" : ""))+"wheel_tick(0x"+ boost::lexical_cast<string>(wheelTick->hwndControl) +","+ boost::lexical_cast<string>(wheelTick->wParam) +","+ boost::lexical_cast<string>(wheelTick->lParam) +","+ boost::lexical_cast<string>(wheelTick->x) +","+ boost::lexical_cast<string>(wheelTick->y) +","+ boost::lexical_cast<string>(wheelTick->delta) +")\n  end";
		luaMFI.text = sLuaspinit.c_str();
		luaMFI.size = _tcslen(luaMFI.text);
	
		//MessageBox(hWndServer, sLuaspinit.c_str(), _T(""), NULL);

		if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMFI, _T("WheelTickLuaScript_sp_?_wheel_tick"),NULL) == 0)
		{
			if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));
			}
		}
	
		if(bUseState2){
			LOGMESSAGE("gWheelTick: (mouse wheel tick) Release Lua state 2");
			bLuaState2Firing = false;
		} else {
			LOGMESSAGE("gWheelTick: (mouse wheel tick) Release Lua state 1");
			bLuaState1Firing = false;
		}
	
		delete wheelTick;

		LeaveCriticalSection(&WheelTickCriticalSection);

		return 0;
	}
#endif

#ifdef SP_MOUSE_CLICK_ACTION
	DWORD WINAPI gMouseClick ( LPVOID lpParam ) 
	{
	
		XClick *xClick = static_cast<XClick*>(lpParam);

		while(bLuaState1Firing && bLuaState2Firing){
			//LOGMESSAGE("gMouseClick: Waiting for Lua states...");
			Sleep(5);
		}
	
		EnterCriticalSection(&CriticalSection);

		bool bUseState2 = (bLuaState1Firing ? true : false);
		if(bUseState2){
			LOGMESSAGE("gMouseClick: Using state 2");
			bLuaState2Firing = true;
		} else {
			LOGMESSAGE("gMouseClick: Using state 1");
			bLuaState1Firing = true;
		}
	
		LeaveCriticalSection(&CriticalSection);

		//Setup calling sp_after_release()
		luaMemFile luaMFI;
		// Load the command and try to execute it...
		std::basic_string<TCHAR> sLuaspinit = "";
		switch(xClick->button)
		{
			case 0:
				sLuaspinit = "if (sp_right_mouse_up ~= nil) then\n  sp_right_mouse_up("+boost::lexical_cast<string>(xClick->x)+","+boost::lexical_cast<string>(xClick->y)+","+boost::lexical_cast<string>(xClick->fwKeys)+")\n  end";
				break;
			case 1:
				sLuaspinit = "if (sp_middle_mouse_up ~= nil) then\n  sp_middle_mouse_up("+boost::lexical_cast<string>(xClick->x)+","+boost::lexical_cast<string>(xClick->y)+","+boost::lexical_cast<string>(xClick->fwKeys)+")\n  end";
				break;
			case 2:
				sLuaspinit = "if (sp_left_mouse_up ~= nil) then\n  sp_left_mouse_up("+boost::lexical_cast<string>(xClick->x)+","+boost::lexical_cast<string>(xClick->y)+","+boost::lexical_cast<string>(xClick->fwKeys)+")\n  end";
				break;
			case 3:
				sLuaspinit = "if (sp_x1_mouse_up ~= nil) then\n  sp_x1_mouse_up("+boost::lexical_cast<string>(xClick->x)+","+boost::lexical_cast<string>(xClick->y)+","+boost::lexical_cast<string>(xClick->fwKeys)+")\n  end";
				break;
			case 4:
				sLuaspinit = "if (sp_x2_mouse_up ~= nil) then\n  sp_x2_mouse_up("+boost::lexical_cast<string>(xClick->x)+","+boost::lexical_cast<string>(xClick->y)+","+boost::lexical_cast<string>(xClick->fwKeys)+")\n  end";
				break;
		}
		luaMFI.text = sLuaspinit.c_str();
		luaMFI.size = _tcslen(luaMFI.text);
	
		if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMFI, _T("MouseActionLuaScript_sp_?_mouse_up"),NULL) == 0)
		{
			if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));
			}
		}
	
		if(bUseState2){
			//LOGMESSAGE("gMouseClick: (On Mouse button "+boost::lexical_cast<string>(xClick->button)+" click up) Release Lua state 2");
			bLuaState2Firing = false;
		} else {
			//LOGMESSAGE("gMouseClick: (On Mouse button "+boost::lexical_cast<string>(xClick->button)+" click up) Release Lua state 1");
			bLuaState1Firing = false;
		}
	
		delete xClick;

		return 0;
	}
#endif

#ifdef SP_AFTERRELEASESCRIPT
	DWORD WINAPI gGestureRelease ( LPVOID lpParam ) 
	{
	
		EnterCriticalSection(&CriticalSection);

		while(bLuaState1Firing && bLuaState2Firing){
			Sleep(5);
		}
	
		bool bUseState2 = (bLuaState1Firing ? true : false);
		if(bUseState2){
			LOGMESSAGE("gGestureRelease: Using state 2");
			bLuaState2Firing = true;
		} else {
			LOGMESSAGE("gGestureRelease: Using state 1");
			bLuaState1Firing = true;
		}
	
		LeaveCriticalSection(&CriticalSection);

		//Setup calling sp_after_release()
		luaMemFile luaMFI;
		// Load the command and try to execute it...
		std::basic_string<TCHAR> sLuaspinit = "if (sp_after_release ~= nil) then\n  sp_after_release()\n  end";
		luaMFI.text = sLuaspinit.c_str();
		luaMFI.size = _tcslen(luaMFI.text);
	
		if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMFI, _T("AfterReleaseLuaScript_sp_after_release"),NULL) == 0)
		{
			if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));
			}
		}
	
		if(bUseState2){
			LOGMESSAGE("gGestureRelease: (After script) Release Lua state 2");
			bLuaState2Firing = false;
		} else {
			LOGMESSAGE("gGestureRelease: (After script) Release Lua state 1");
			bLuaState1Firing = false;
		}
	
		return 0;
	}
#endif

DWORD WINAPI gGestureComplete( LPVOID lpParam )
{
	LOGMESSAGE("gGestureComplete: Before EnterCriticalSection");
	EnterCriticalSection(&CriticalSection);

	ClearGesture((LPVOID)0);

	//Is DrawPoints long enough to be a gesture?
	if(DrawPoints.size() > 1)
	{
		DrawnGestureName = GetGestureName( DrawPoints );
		//Is Gesture recognized
		if(DrawnGestureName.empty() && !bLearningMode)
		{	//Not recognized, do nothing.. could prompt to train/assign here, but I'm thinking not...
			if(bNoMatchSound){
				if(sNoMatchSoundWAV.length() == 0){
					PlaySound((LPCTSTR)SND_ALIAS_SYSTEMQUESTION, NULL, SND_ALIAS_ID | SND_ASYNC);
				} else {
					PlaySound((LPCTSTR)sNoMatchSoundWAV.c_str(), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
				}
			}
			clearCaptureVars();
			if(bRelayGesture) {
				acRelayGesture();
			}
			LeaveCriticalSection(&CriticalSection);
			return 1;
		}
	} else {
		//no drawing, so test for actions defined without a drawing
		DrawnGestureName = _T(""); 
		LOGMESSAGE("DrawnGestureName Cleared - gGestureComplete()");
	}

	if(bLearningMode){
		LOGMESSAGE("gGestureComplete: Learning Mode Enabled");
		clearCaptureVars();
		if(LastDrawPoints.size() > 1) {
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWGESTURENAMEDLG, 1), 0);
		} else {
			MessageBoxA(hWndServer, ptLang.get("language.Base.Messages.GestureTooShortMessage",_T("Please draw a longer gesture.")).c_str() , ptLang.get("language.Base.Messages.GestureTooShortTitle",_T("Gesture Too Short")).c_str(), NULL);
		}
		LeaveCriticalSection(&CriticalSection);
		return 1;
	}
	
	if(bLuaState1Firing && bLuaState2Firing /* && (bFireOnMouseWheel && bMouseWheelFiring ? false : true)*/){
		LOGMESSAGE("gGestureComplete: Both Lua states currently in use - discarding gesture");
		if(bNoMatchSound){
			if(sNoMatchSoundWAV.length() == 0){
				PlaySound((LPCTSTR)SND_ALIAS_SYSTEMQUESTION, NULL, SND_ALIAS_ID | SND_ASYNC);
			} else {
				PlaySound((LPCTSTR)sNoMatchSoundWAV.c_str(), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
			}
		}
		clearCaptureVars();
		LeaveCriticalSection(&CriticalSection);
		return 1;
	}

	using boost::lexical_cast;
	std::basic_string<TCHAR> ActionScript = _T("");
	std::basic_string<TCHAR> ActionScriptTemp = _T("");
	//Define mouse start and end coords at top of Lua script, so they're available to any action script
	std::basic_string<TCHAR> sTempGestureName = DrawnGestureName;
	boost::replace_all(sTempGestureName, _T("\\"), _T("\\\\"));
	ActionScript.append(_T("local sp_gesture_name = \"")+sTempGestureName+"\"\n");
	ActionScript.append(_T("local sp_gesture_start_x = ")+boost::lexical_cast<std::basic_string<TCHAR>>(DrawPoints[0].x)+"\n");
	ActionScript.append(_T("local sp_gesture_start_y = ")+boost::lexical_cast<std::basic_string<TCHAR>>(DrawPoints[0].y)+"\n");
	ActionScript.append(_T("local sp_gesture_end_x = ")+boost::lexical_cast<std::basic_string<TCHAR>>(lastx)+"\n");
	ActionScript.append(_T("local sp_gesture_end_y = ")+boost::lexical_cast<std::basic_string<TCHAR>>(lasty)+"\n");
	ActionScript.append(_T("local sp_wheel_delta = ")+boost::lexical_cast<std::basic_string<TCHAR>>((int)dWheelDelta)+"\n");
	ActionScript.append(_T("local gwd = sp_wheel_delta\n"));
	ActionScript.append(_T("local gnm = sp_gesture_name\n"));
	ActionScript.append(_T("local gsx = sp_gesture_start_x\n"));
	ActionScript.append(_T("local gsy = sp_gesture_start_y\n"));
	ActionScript.append(_T("local gex = sp_gesture_end_x\n"));
	ActionScript.append(_T("local gey = sp_gesture_end_y\n"));
	ActionScript.append(_T("local gbl = ")+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingLeft)+_T("\n"));
	ActionScript.append(_T("local gbt = ")+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingTop)+_T("\n"));
	ActionScript.append(_T("local gbr = ")+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingRight)+_T("\n"));
	ActionScript.append(_T("local gbb = ")+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingBottom)+_T("\n"));
	

	//Calls the match function, which takes the DrawnGestureName and all of the key/mouse modifiers to see
	//if an action exists for the combo and returns the Lua
	POINT tmpPoint;
	tmpPoint.x = (float)DrawPoints[0].x;
	tmpPoint.y = (float)DrawPoints[0].y;
	ActionScriptTemp = GetActionScript(WindowFromPoint(tmpPoint), false
			#ifdef SP_NOGLOBALACTIONS
				, false
			#endif
		);

	std::basic_string<TCHAR> sTempAppName = MatchedAppName;
	std::basic_string<TCHAR> sTempActionName = MatchedActionName;
	ActionScript.append(_T("local sp_app_name = \"")+sTempAppName+_T("\"\n"));
	ActionScript.append(_T("local sp_action_name = \"")+sTempActionName+_T("\"\n"));
	ActionScript.append(_T("local gapp = sp_app_name\n\n"));
	ActionScript.append(_T("local gact = sp_action_name\n\n"));
	ActionScript.append(_T("if (sp_before_action ~= nil) then\n"));
	ActionScript.append(_T("    sp_before_action(gnm,gsx,gsy,gex,gey,gwd,gapp,gact,gbl,gbt,gbr,gbb)\n"));
	ActionScript.append(_T("end\n"));

	std::basic_string<TCHAR> AfterActionScript = _T("");
	if(bAllowAfterScript){
		AfterActionScript.append(_T("if (sp_after_action ~= nil) then\n"));
		AfterActionScript.append(_T("    sp_after_action(\"")
														+sTempGestureName+_T("\",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(DrawPoints[0].x)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(DrawPoints[0].y)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(lastx)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(lasty)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>((int)dWheelDelta)
														+_T(",\"")
															+sTempAppName
														+_T("\",\"")
															+sTempActionName
														+_T("\",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingLeft)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingTop)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingRight)
														+_T(",")
															+boost::lexical_cast<std::basic_string<TCHAR>>(iBoundingBottom)
														+_T(")\n"));
		AfterActionScript.append(_T("end\n"));
	}

	bool bUseState2 = (bLuaState1Firing ? true : false);
	if(bUseState2){
		LOGMESSAGE("gGestureComplete: Main script - Using state 2");
		bLuaState2Firing = true;
	} else {
		LOGMESSAGE("gGestureComplete: Main script - Using state 1");
		bLuaState1Firing = true;
	}

	if(!bMouseWheelFiring){
		LOGMESSAGE("gGestureComplete: (Before script) Mouse Wheel Not Firing, calling clearCaptureVars");
		clearCaptureVars();
	} 
	
	LeaveCriticalSection(&CriticalSection);

	if(!ActionScriptTemp.empty())
	{

		//Append actual script to the header (mouse coords) string
		ActionScript.append(ActionScriptTemp);

		if(bSettingsOpen){
			EnableWindow(GetDlgItem(hWndSettings,ID_S_OK), false);
			EnableWindow(GetDlgItem(hWndSettings,ID_S_APPLY), false);
		}

		// Use LUA...
		luaMemFile luaMF;

		// Load the command and try to execute it...
		luaMF.text = ActionScript.c_str();
		luaMF.size = _tcslen(luaMF.text);

		if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMF, _T("ActionScript"), NULL) == 0)
		{

			// Execute the loaded command...
			// The function takes 0 parameters and will return 1 result
			if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
			{
				// There was no error
				// Let's get the result from the stack
				int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));

				/*CHAR res[10];
				sprintf(res,"%d",result);
				MessageBoxA(GetForegroundWindow(),res,NULL,MB_OK | MB_SETFOREGROUND);*/
			} else {
				MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
				//MessageBox(GetForegroundWindow(), luaMF.text, _T("Script"), NULL);
			}

			// Clean-up the stack
			lua_pop((bUseState2 ? L2 : L), 1);
		} else {
			// There was a lua_load error...
			// Pop the error value from the stack
			MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
			//MessageBox(GetForegroundWindow(), luaMF.text, _T("Script"), NULL);
			lua_pop((bUseState2 ? L2 : L), 1);
		}

		if(!AfterActionScript.empty())
		{
			if(bUseState2){
				LOGMESSAGE("gGestureComplete: sp_after_action script - Using state 2");
			} else {
				LOGMESSAGE("gGestureComplete: sp_after_action script - Using state 1");
			}
			// Use LUA...
			luaMemFile luaMF;

			// Load the command and try to execute it...
			luaMF.text = AfterActionScript.c_str();
			luaMF.size = _tcslen(luaMF.text);
			if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMF, _T("AfterActionScript"), NULL) == 0)
			{
				// Execute the loaded command...
				// The function takes 0 parameters and will return 1 result
				if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
				{
					// There was no error
					// Let's get the result from the stack
					int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));

					/*CHAR res[10];
					sprintf(res,"%d",result);
					MessageBoxA(GetForegroundWindow(),res,NULL,MB_OK | MB_SETFOREGROUND);*/
				} else {
					MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
					//MessageBox(GetForegroundWindow(), luaMF.text, _T("Script"), NULL);
				}

				// Clean-up the stack
				lua_pop((bUseState2 ? L2 : L), 1);
			} else {
				// There was a lua_load error...
				// Pop the error value from the stack
				MessageBox(GetForegroundWindow(), lua_tostring((bUseState2 ? L2 : L), -1), ptLang.get("language.Base.Messages.LuaScriptError",_T("Lua Script Error!")).c_str(), NULL);
				//MessageBox(GetForegroundWindow(), luaMF.text, _T("Script"), NULL);
				lua_pop((bUseState2 ? L2 : L), 1);
			}
		}

		// Verify the stack
		// (don't forget to include crtdbg.h)
		//_ASSERT(lua_gettop(L) == 0);
	} else {
		LOGMESSAGE("gGestureComplete: (After script) No match found, ignoring gesture");
		if(bNoMatchSound){
			if(sNoMatchSoundWAV.length() == 0){
				PlaySound((LPCTSTR)SND_ALIAS_SYSTEMQUESTION, NULL, SND_ALIAS_ID | SND_ASYNC);
			} else {
				PlaySound((LPCTSTR)sNoMatchSoundWAV.c_str(), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
			}
		}
		if(bRelayGesture && !bMouseWheelFiring) {
			acRelayGesture();
		}
	}

	#ifdef SP_AFTERRELEASESCRIPT
		if(bFireOnRelease && !bMouseWheelFiring){
			//Setup calling sp_after_release()
			luaMemFile luaMFI;
			// Load the command and try to execute it...
			std::basic_string<TCHAR> sLuaspinit = "if (sp_after_release ~= nil) then\n  sp_after_release()\n  end";
			luaMFI.text = sLuaspinit.c_str();
			luaMFI.size = _tcslen(luaMFI.text);
	
			if(lua_load((bUseState2 ? L2 : L), readMemFile, &luaMFI, _T("AfterReleaseLuaScript_sp_after_release"),NULL) == 0)
			{
				if(lua_pcall((bUseState2 ? L2 : L), 0, 1, 0) == 0)
				{
					// There was no error
					// Let's get the result from the stack
					int result = lua_tonumber((bUseState2 ? L2 : L), lua_gettop(L));
				}
			}
		}
	#endif

	if(bUseState2){
		LOGMESSAGE("gGestureComplete: (After script) Release Lua state 2");
		bLuaState2Firing = false;
	} else {
		LOGMESSAGE("gGestureComplete: (After script) Release Lua state 1");
		bLuaState1Firing = false;
	}

	if(bSettingsOpen){
		EnableWindow(GetDlgItem(hWndSettings,ID_S_OK), true);
		EnableWindow(GetDlgItem(hWndSettings,ID_S_APPLY), true);
	}

	if(!bMouseWheelFiring){
		LOGMESSAGE("gGestureComplete: (After script) Mouse Wheel Not Firing, calling setWindowState");
		setWindowState();
	}
		
	GetWindowRect(hWndServer,&rcWindow);

	return 0;
}

DWORD WINAPI ClearGesture(LPVOID lpParam)
{
	if(bDrawGesture == TRUE)
	{
		HDC hdc = GetDC(hWndServer);			//If XP, draw directly to screen DC //(OSVersion == 4 ? NULL : hWndServer)
		SelectObject(hdc ,hBackgroundPen);								//Set hdc pen
		int drawSize = DrawPoints.size();
		for(int i = ((iMaxDrawPoints > 0 && drawSize > iMaxDrawPoints) ? drawSize-iMaxDrawPoints : 0);i<drawSize-1;i++)
		{
			MoveToEx(hdc,DrawPoints[i].x-rcWindow.left,DrawPoints[i].y-rcWindow.top,NULL);			//Retrace the line using the window's color
			LineTo(hdc,DrawPoints[i+1].x-rcWindow.left,DrawPoints[i+1].y-rcWindow.top);			
		}
	
		ReleaseDC(hWndServer,hdc);

		setWindowState();
	}
	return 0;
}

#pragma endregion Gesture Match and Execute


#pragma region Timer Procs

static void CALLBACK MouseHideTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	LOGMESSAGE("Enter MouseHideTimerProc");
	//this timer is used for the acAutoHideMouseCursor action
	if(!bAutoHideMouse){
		return;
	}

	int iCurrentX = acGetMouseLocationX();
	int iCurrentY = acGetMouseLocationY();

	iMouseReHideCounter++;
	if (iMouseReHideCounter*100 >= iReHideMouseAfter && bCursorHidden)
	{
		iMouseReHideCounter = 0;
		acHideMouseCursor(L);
	}

	iMouseHideCounter++;
	if(iCurrentX != iHideLastX && iCurrentY != iHideLastY) {
		iMouseHideCounter = 0;
		if(bCursorHidden){
			acShowMouseCursor(L);
			bCursorHidden = false;
		}
	} else {
		if (iMouseHideCounter*100 >= iHideMouseAfter)
		{
			iMouseHideCounter = 0;
			iMouseReHideCounter = 0;
			acHideMouseCursor(L);
			bCursorHidden = true;
		}
	}

	iHideLastX = iCurrentX;
	iHideLastY = iCurrentY;
}

static void CALLBACK DisplayTextTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	LOGMESSAGE("Enter DisplayTextTimerProc");
	//this timer removes the window used to display the text in the acDisplayText action
	KillTimer(NULL,displaytexttimerid);
	EndDialog(hWndDisplayText, NULL);
	hWndDisplayText = NULL;
	bOSDDisplayed = false;
	setWindowState();
}

static void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	//The purpose of this function is to automatically timeout the gesture and replay the mouse events
	//that occurred during the gesture; so right-click dragging from a file in Explorer, then letting the gesture
	//timeout threshold pass will cause the mouse to now have the file attached to it for copy/move operations
	//The timer is created in MouseProc
	if(!bLearningMode && !bMouseWheelFiring){
		LOGMESSAGE("TimerProc");
		#ifdef SP_SYNAPTIC
			bCancelTouchPad = true;
		#endif
		bMouseDown = FALSE; //Tell the hook that a mouse button is no longer down
		bCancel = TRUE; //Tell the hook that a gesture cancel has been issued
		KillTimer(NULL,canceltimerid); //Cancel timer event so it doesn't fire again
		ClearGesture((LPVOID)0);
		bMouseWheelFiring = FALSE;
		setWindowState();
		clearCaptureVars();
	#ifdef SP_SYNAPTIC
		lTouchPadFirstContactStamp = -1;
		if(bSynapticGesture){
			bSynapticGesture = false;
		} else {
			//TODO: SP_TOUCH - Need to cancel and replay all touch contact history..maybe this is handled in the processInputMessage proc
	#endif
			if(hookbtn < 0){ //hookbtn < 0 are keyboard based stroke buttons
				if(hookbtn == -1){
					acSendControlDown();
				} else if (hookbtn == -2) {
					acSendAltDown();
				} else if (hookbtn == -3) {
					acSendShiftDown();
				} else if (hookbtn == -4) {
					acSendKeys(" ", false);
				}
			} else {
				POINT screenpoint;
				GetCursorPos (&screenpoint); //Get the current mouse pos
				DWORD dx = (65535 * (startx-rcWindow.left)) / GetSystemMetrics(SM_CXVIRTUALSCREEN) + 1;//Set the x,y of the location where the drawing began
				DWORD dy = (65535 * (starty-rcWindow.top)) / GetSystemMetrics(SM_CYVIRTUALSCREEN) + 1;//
				mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE, dx, dy, 0, 0); //Move the mouse to that start location
				if(hookbtn == 3 || hookbtn == 4) {
					mouse_event(MOUSEEVENTF_XDOWN, dx, dy, (hookbtn == 3 ? XBUTTON1 : XBUTTON2), 0); //Send the xbutton down event to the foreground window
				} else {
					mouse_event((hookbtn == 0 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN) : (hookbtn == 1 ? MOUSEEVENTF_MIDDLEDOWN : (hookbtn == 2 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) : NULL))), dx, dy, 0, 0); //Send the hook button down event to the foreground window
				}
				dx = (65535 * (screenpoint.x-rcWindow.left)) / GetSystemMetrics(SM_CXVIRTUALSCREEN) + 2;//Set the x,y of the location where the drawing was canceled
				dy = (65535 * (screenpoint.y-rcWindow.top)) / GetSystemMetrics(SM_CYVIRTUALSCREEN) + 2;//
				mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE, dx, dy, 0, 0); //Move the mouse back to where it was when canceled
			}
	#ifdef SP_SYNAPTIC
		}
	#endif
	}
}

static void CALLBACK TrimTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	LOGMESSAGE("Enter TrimTimerProc ");
	//Yep! This is the timer that simply trims the memory usage so S+ shows as little RAM being used as possible in Task Manager
	//If the Aggressively Manage Memory preference is unchecked, this timer simply doesn't fire. Hey, perception is reality!
	if(!bMouseDown && !bLuaState1Firing && !bLuaState2Firing && bGestureOpen == FALSE && bNameOpen == FALSE && bSettingsOpen == FALSE && bAggressiveTrim){  
		SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);   
	}
}

static void CALLBACK TimerStrokeSwatch(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime) {
	LOGMESSAGE("Enter TimerStrokeSwatch");
	//this timer is used for drawing the stroke color swatch in the Prefernces tab
	KillTimer(NULL, strokeswatchtimerid);
	PostMessage(hWndSettings, WM_EXITSIZEMOVE, 0, 0);
}

#pragma endregion Timer Procs


#pragma region Win32 Callbacks

static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	LOGMESSAGE("Enter WinEventProc");
	//This function is called by windows every time a window gains focus. It's used to control the Disable S+ if this application becomes active setting for ignored windows
	if(bOnlyAllowDefinedApps == false){
		if(!getGesturesDisabled()){
			bIgnoreDisable = false;
			if(GetActionScript(hwnd, true
				#ifdef SP_NOGLOBALACTIONS
					, false
				#endif
				).compare(_T("IGNORED")) == 0){
				if(bIgnoreDisable){
					PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_DISABLED, 1), 0);
				}
			}
		}
	}
}

static BOOL CALLBACK EnumWindowsProc(HWND hWndEnum, LPARAM lParam)
{
	LOGMESSAGE("Enter EnumWindowProc");
	//This function is used by the following actions:
	/*
		acNextApplication
		acPreviousApplication
		acTileWindows
		acGetWindowFromProcessID

	  It simply loops through all windows, qualifying only top-level windows in the multiple if() qualifiers below
	*/

	RECT hwndRt;
	GetClientRect(hWndEnum, &hwndRt);
	
	#ifdef SP_WIN8_CHECKS
		bool bIsImmersive = false;
		if(lpfnIsImmersiveProcessFunc){
			DWORD processID = 0;
			GetWindowThreadProcessId((HWND)hWndEnum, &processID);
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
			bIsImmersive = lpfnIsImmersiveProcessFunc(hProcess);
			CloseHandle(hProcess);	
		}	
	#endif

	//only pay attention to top-level windows
	if ( (( GetWindowLong(hWndEnum,GWL_STYLE) & WS_VISIBLE ) //the window is visible
			&& ( GetWindow(hWndEnum,GW_OWNER) == NULL ) //There is no owner for this window
			&& ( !(IsRectEmpty(&hwndRt) && !IsIconic(hWndEnum))) //The rect for the window (its size) isn't empty (it actually occupies a visible area)
			&& ( IsWindowEnabled(hWndEnum)==TRUE 
			#ifdef SP_WIN8_CHECKS
				|| bIsImmersive
			#endif
				) //The window is enabled OR it is a Metro window //(GetWindowLong(hWndEnum,GWL_EXSTYLE) & 0x00200000L) != 0
			&& ( hWndEnum != (lParam == 3 ? hWndLastWindow : hWndServer) ) //The window is NOT the S+ gesture draw window
			&& ( (GetWindowLong(hWndEnum,GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0 )  //The window is NOT a tool window
		 ) 
		  || 
		 lParam == 10 // when called from acGetAllWindows, not requesting only top-level windows (literally get ALL window handles)
		)
	{ 

		switch(lParam)
		{
			//acPreviousApplication
			case 1:
			case 3:
				#ifdef SP_WIN8_CHECKS
					if((GetWindowLong(hWndEnum,GWL_EXSTYLE) & 0x00200000L) == 0 && (GetWindowLong(hWndEnum, GWL_EXSTYLE) & WS_EX_TOPMOST) != WS_EX_TOPMOST){
				#endif
					if(EnumWindowCount == 1)
					{
						hWndLastWindow = hWndEnum;
						return FALSE;
					}
					EnumWindowCount++;
				#ifdef SP_WIN8_CHECKS
					}
				#endif
				break;
			//acNextApplication
			case 2:
				#ifdef SP_WIN8_CHECKS
					if((GetWindowLong(hWndEnum,GWL_EXSTYLE) & 0x00200000L) == 0 && (GetWindowLong(hWndEnum, GWL_EXSTYLE) & WS_EX_TOPMOST) != WS_EX_TOPMOST){
						hWndLastWindow = hWndEnum;
					}
				#else
					hWndLastWindow = hWndEnum;
				#endif
				break;
			case 4:
				//Count windows
				EnumWindowCount++;
				break;
			case 5:
			case 7:
			{
				//Vertical tile
				int iScreenRight = 0;
				int iScreenLeft = 0;
				int iScreenWidth = 0;
				int iScreenBottom = 0;
				int iScreenTop = 0;
				int iScreenHeight = 0;
				
				POINT tmpPt;

				if(lParam==5){
					if(EnumWindowCountTemp==0){
						RECT rcTemp;
						GetWindowRect(hWndEnum, &rcTemp);
						tmpPt.x = (rcTemp.right+rcTemp.left) / 2;
						tmpPt.y = (rcTemp.bottom+rcTemp.top) / 2;
						hTempScreen = (HMONITOR)acGetMonitorFromPoint(tmpPt);
					}
					iScreenRight = acGetMonitorRight(hTempScreen, TRUE);
					iScreenLeft = acGetMonitorLeft(hTempScreen, TRUE);
					iScreenWidth = iScreenRight - iScreenLeft;
					iScreenBottom = acGetMonitorBottom(hTempScreen, TRUE) ;
					iScreenTop = acGetMonitorTop(hTempScreen, TRUE);
					iScreenHeight = iScreenBottom - iScreenTop;
				} else {
					iScreenRight = rcWindow.right;
					iScreenLeft = rcWindow.left;
					iScreenWidth = iScreenRight - iScreenLeft;
					iScreenBottom = rcWindow.bottom;
					iScreenTop = rcWindow.top;
					iScreenHeight = iScreenBottom - iScreenTop;
				}

				acRestoreWindow(hWndEnum, tmpPt);
				acMoveWindow(hWndEnum, tmpPt, iScreenLeft+(EnumWindowCountTemp==0 ? 0 : ((iScreenWidth/(EnumWindowCount))*(EnumWindowCountTemp))), iScreenTop);
				acSetWindowSize(hWndEnum, tmpPt, iScreenWidth/(EnumWindowCount), iScreenHeight);
				EnumWindowCountTemp++;
				break;
			}
			case 6:
			case 8:
			{
				//Horizontal tile
				int iScreenRight = 0;
				int iScreenLeft = 0;
				int iScreenWidth = 0;
				int iScreenBottom = 0;
				int iScreenTop = 0;
				int iScreenHeight = 0;

				POINT tmpPt;

				if(lParam==6){
					if(EnumWindowCountTemp==0){
						RECT rcTemp;
						GetWindowRect(hWndEnum, &rcTemp);
						tmpPt.x = (rcTemp.right+rcTemp.left) / 2;
						tmpPt.y = (rcTemp.bottom+rcTemp.top) / 2;
						hTempScreen = (HMONITOR)acGetMonitorFromPoint(tmpPt);
					}
					iScreenRight = acGetMonitorRight(hTempScreen, TRUE);
					iScreenLeft = acGetMonitorLeft(hTempScreen, TRUE);
					iScreenWidth = iScreenRight - iScreenLeft;
					iScreenBottom = acGetMonitorBottom(hTempScreen, TRUE) ;
					iScreenTop = acGetMonitorTop(hTempScreen, TRUE);
					iScreenHeight = iScreenBottom - iScreenTop;
				} else {
					iScreenRight = rcWindow.right;
					iScreenLeft = rcWindow.left;
					iScreenWidth = iScreenRight - iScreenLeft;
					iScreenBottom = rcWindow.bottom;
					iScreenTop = rcWindow.top;
					iScreenHeight = iScreenBottom - iScreenTop;
				}

				acRestoreWindow(hWndEnum, tmpPt);
				acMoveWindow(hWndEnum, tmpPt, iScreenLeft, iScreenTop+(EnumWindowCountTemp==0 ? 0 : ((iScreenHeight/(EnumWindowCount))*(EnumWindowCountTemp))));
				acSetWindowSize(hWndEnum, tmpPt, iScreenWidth, iScreenHeight/(EnumWindowCount));
				EnumWindowCountTemp++;
				break;
			}
			case 9:
			{
				//Used to get hwnd from processID
				DWORD processID = 0;
				GetWindowThreadProcessId(hWndEnum, &processID);
				if(processID == g_processID)
				{
					hWndProcessID = hWndEnum;
					return FALSE;
				}
				break;
			}
			case 10:
			case 11:
			{
				GetAllWindowsList.push_back(hWndEnum);
				break;
			}
		}

	}

	return TRUE;
}

static LRESULT CALLBACK MouseProc(UINT nCode, WPARAM wParam, LPARAM lParam)
    {

	//The lifeblood mouse hook which starts everything when drawing a mouse gesture

    if(nCode < HC_ACTION){ /* pass it on */
		 return CallNextHookEx(mousehook, nCode, wParam, lParam);
	} 

	if(bGlobalCancel == TRUE || bGlobalUserCancel) {  
		 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
	}

	MSLLHOOKSTRUCT *hookStruct = (MSLLHOOKSTRUCT*)lParam;
	CURSORINFO CursorInfo;
	(VOID)memset(&CursorInfo, 0, sizeof(CursorInfo));
	CursorInfo.cbSize = sizeof(CursorInfo);

	if (bConsumePhysical){ 
		if((hookStruct->flags & LLMHF_INJECTED)){
			if(hookStruct->dwExtraInfo == 123456){
				//This injected mouse event originated from S+, so pass it along (123456 is passed to mouse_event calls in acMouseMove and acMouseClick)
				return CallNextHookEx(mousehook, nCode, wParam, lParam);
			} else {
				//This injected event did NOT originate from S+, so consume it
				return TRUE;
			}
		} else {
			if(bMouseWheelFiring){
				//Handles de-capturing the mouse when a call to acConsumePhysicalInput was called via an action which was triggered by a mouse wheel scroll and Fire on Mouse Wheel is enabled
				if((wParam == (hookbtn == 0 ? WM_RBUTTONUP : (hookbtn == 1 ? WM_MBUTTONUP : (hookbtn == 2 ? WM_LBUTTONUP : NULL))) || wParam == (hookbtn == 0 ? WM_NCRBUTTONUP : (hookbtn == 1 ? WM_NCMBUTTONUP : (hookbtn == 2 ? WM_NCLBUTTONUP : NULL)))) || ((GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && wParam == WM_XBUTTONUP && hookbtn == 3) || (GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && wParam == WM_XBUTTONUP && hookbtn == 4)))
				{
					bMouseDown = FALSE;
					KillTimer(NULL,canceltimerid); 
					bMouseWheelFiring = FALSE;
					clearCaptureVars();
					setWindowState();
				}
			}
		}
		return TRUE;
	}
	
	if(bIgnoreGesture == TRUE){
		 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
	}

	// Mouse cursor is currently hidden, take no action (specifically corrects Input Director issue)
	GetCursorInfo(&CursorInfo);
	if (bCheckCursorFlags && CursorInfo.flags == 0) {
		 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
	}

	//Mouse is off screen, ignore
	if((CursorInfo.ptScreenPos.x > iCXVIRTUALSCREEN) || 
		(CursorInfo.ptScreenPos.x < iXVIRTUALSCREEN) || 
		(CursorInfo.ptScreenPos.y > iCYVIRTUALSCREEN) || 
		(CursorInfo.ptScreenPos.y < iYVIRTUALSCREEN)){
		 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
	}

#ifdef SP_DEBUG_MODE
	if(bLuaState1Firing && bLuaState2Firing)
	{
		LOGMESSAGE("Both Lua States in Use");
	}
	//if(bMouseDown) LOGMESSAGE("bMouseDown == TRUE");
#endif

	//Capture mouse scroll wheel movement; gestures can include mouse scroll wheel up/down as part of definition
	if(bMouseDown == TRUE && wParam == WM_MOUSEWHEEL && bLearningMode != TRUE)  
	{
		if(GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData) > 0)
		{
			if(bFireOnMouseWheel){
				bMouseWheelDown = FALSE;
				dWheelDelta = 0;
			}
			bMouseWheelUp = TRUE;
			dWheelDelta += GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData);
		}
		if(GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData) < 0)
		{
			if(bFireOnMouseWheel){
				bMouseWheelUp = FALSE;
				dWheelDelta = 0;
			}
			bMouseWheelDown = TRUE;
			dWheelDelta += GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData);
		}
		if(bResetCancelDelayOnMovement){
			KillTimer(NULL,canceltimerid);  //Kill timer so it doesn't fire
			canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when the mouse moves while drawing 
		}

		//Ok, here we're telling S+ to stop monitoring/drawing/capturing modifiers and gestures if Fire Mouse Wheel is enable in preferences
		//only recognize the mouse wheel event and fire the action
		if(bFireOnMouseWheel){ 
			KillTimer(NULL,canceltimerid);  
			bMouseWheelFiring = TRUE;
			HANDLE hThread = CreateThread(NULL, 0, &gGestureComplete, NULL, 0, NULL); 
			CloseHandle(hThread);
		}
		return TRUE;
	}

	//If we're not in a gesture and Enable Mouse Wheel Relay is enabled, forward the mouse wheel event, and any keystates, to the window below the mouse cursor
	if((wParam == WM_MOUSEWHEEL || wParam == WM_MOUSEHWHEEL)&& (bMouseWheelFocus 
#ifdef SP_WHEEL_HOOK 
		|| bMouseWheelTick || bHorzMouseWheelTick
#endif
		) && bMouseDown == false && bLearningMode != TRUE) {  
		HWND hwndControlBelowMouse = WindowFromPoint(CursorInfo.ptScreenPos);

		if((GetWindowLong(hwndControlBelowMouse,GWL_EXSTYLE) & 0x00200000L) == 0)
		{
			DWORD fwKeys = 0;
			//The checks below are to ensure any keystates are sent along with the mouse message
			if((GetKeyState(VK_CONTROL) & 0x8000) != 0){
				fwKeys = fwKeys | MK_CONTROL;
			}
			if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
				fwKeys = fwKeys | MK_SHIFT;
			}
			if((GetKeyState(VK_LBUTTON) & 0x8000) != 0){
				fwKeys = fwKeys | MK_LBUTTON;
			}
			if((GetKeyState(VK_MBUTTON) & 0x8000) != 0){
				fwKeys = fwKeys | MK_MBUTTON;
			}
			if((GetKeyState(VK_RBUTTON) & 0x8000) != 0){
				fwKeys = fwKeys | MK_RBUTTON;
			}		
			if((GetKeyState(VK_XBUTTON1) & 0x8000) != 0){
				fwKeys = fwKeys | MK_XBUTTON1;
			}
			if((GetKeyState(VK_XBUTTON2) & 0x8000) != 0){
				fwKeys = fwKeys | MK_XBUTTON2;
			}
#ifdef SP_WHEEL_HOOK 
			if(bMouseWheelTick || bHorzMouseWheelTick) {
				//Fire the sp_wheel_tick Global Lua action
			#ifdef SP_NOGLOBALACTIONS
				if(bOnlyAllowDefinedApps) {
					LOGMESSAGE("bOnlyAllowDefinedApps == true");
					string strReturn = GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), false, true);
					LOGMESSAGE(strReturn.c_str());
					if(strReturn.compare(_T("APPDEFINED")) == 0){
						WheelTick *wheelTick = new WheelTick();
						wheelTick->Horizontal = (wParam == WM_MOUSEHWHEEL ? true : false);
						wheelTick->hwndControl = hwndControlBelowMouse;
						wheelTick->wParam = MAKEWPARAM(fwKeys,GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData));
						wheelTick->lParam = MAKELPARAM(CursorInfo.ptScreenPos.x,CursorInfo.ptScreenPos.y);
						wheelTick->delta = GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData);
						wheelTick->x = CursorInfo.ptScreenPos.x;
						wheelTick->y = CursorInfo.ptScreenPos.y;
						HANDLE hThread = CreateThread(NULL, 0, &gWheelTick, (LPVOID *)wheelTick, 0, NULL); 
						CloseHandle(hThread);
					} else {
						LOGMESSAGE("Not APPDEFINED");
						 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
					}
				} else {
					LOGMESSAGE("bOnlyAllowDefinedApps == false");
			#endif
					if(GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), TRUE
								#ifdef SP_NOGLOBALACTIONS
									, false
								#endif
								).compare(_T("IGNORED")) != 0){
						WheelTick *wheelTick = new WheelTick();
						wheelTick->Horizontal = (wParam == WM_MOUSEHWHEEL ? true : false);
						wheelTick->hwndControl = hwndControlBelowMouse;
						wheelTick->wParam = MAKEWPARAM(fwKeys,GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData));
						wheelTick->lParam = MAKELPARAM(CursorInfo.ptScreenPos.x,CursorInfo.ptScreenPos.y);
						wheelTick->delta = GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData);
						wheelTick->x = CursorInfo.ptScreenPos.x;
						wheelTick->y = CursorInfo.ptScreenPos.y;
						HANDLE hThread = CreateThread(NULL, 0, &gWheelTick, (LPVOID *)wheelTick, 0, NULL); 
						CloseHandle(hThread);
					} else {
						return CallNextHookEx(mousehook, nCode, wParam, lParam);
					}
			#ifdef SP_NOGLOBALACTIONS
				}
			#endif
			} else {
#endif
				if(bActivateMouseWheelWindow){
					// acActivateWindowUnderMouseWheel(1) was called to set bActivateMouseWheelWindow=true
					// This code will activate the window below the mouse when the mouse wheel is scrolled and Enable Mouse Wheel Relay is active
					// Get the owner window handle of the window below the mouse cursor
					HWND hOwner = (HWND)acGetOwnerWindowByPoint(CursorInfo.ptScreenPos); 
					if(GetAncestor(GetForegroundWindow(),GA_ROOTOWNER) != hOwner && hOwner != GetDesktopWindow()){
						// if the foreground window isn't the owner window below the mouse, bring it to the foreground
						SetForegroundWindow(hOwner);
					}
				}
				// Post the mouse wheel message to the window below the mouse
				PostMessage(hwndControlBelowMouse, WM_MOUSEWHEEL, MAKEWPARAM(fwKeys,GET_WHEEL_DELTA_WPARAM(hookStruct->mouseData)), MAKELPARAM(CursorInfo.ptScreenPos.x,CursorInfo.ptScreenPos.y));
#ifdef SP_WHEEL_HOOK 
			}
#endif
			return TRUE;
		}
	}

    #ifdef SP_MOUSE_CLICK_ACTION
		//If Allow Middle/Left/Right/X1/X2 Click Script is on, then fire the function to execute sp_middle_mouse_down
		if(bMouseDown == false){
			if(	   (bFireOnRightClick && wParam == WM_RBUTTONUP && hookbtn != 0) 
				|| (bFireOnMiddleClick && wParam == WM_MBUTTONUP && hookbtn != 1) 
				|| (bFireOnLeftClick && wParam == WM_LBUTTONUP && hookbtn != 2) 
				|| (bFireOnX1Click && wParam == WM_XBUTTONUP && GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && hookbtn != 3) 
				|| (bFireOnX2Click && wParam == WM_XBUTTONUP && GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && hookbtn != 4))
			{
				#ifdef SP_NOGLOBALACTIONS
					if(bOnlyAllowDefinedApps) {
						LOGMESSAGE("bOnlyAllowDefinedApps == true");
						string strReturn = GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), false, true);
						LOGMESSAGE(strReturn.c_str());
						if(strReturn.compare(_T("APPDEFINED")) != 0 && bLearningMode != TRUE){
							LOGMESSAGE("Not APPDEFINED");
							 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
						}
					} else {
						LOGMESSAGE("bOnlyAllowDefinedApps == false");
				#endif
						if(GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), TRUE
							#ifdef SP_NOGLOBALACTIONS
								, false
							#endif
							).compare(_T("IGNORED")) == 0 && bLearningMode != TRUE){
							LOGMESSAGE("Ignored window");
							 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
						}
				#ifdef SP_NOGLOBALACTIONS
					}
				#endif
				DWORD fwKeys = 0;
				//The checks below are to ensure any keystates are sent along with the mouse message
				if((GetKeyState(VK_CONTROL) & 0x8000) != 0){
					fwKeys = fwKeys | MK_CONTROL;
				}
				if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
					fwKeys = fwKeys | MK_SHIFT;
				}
				if((GetKeyState(VK_LBUTTON) & 0x8000) != 0){
					fwKeys = fwKeys | MK_LBUTTON;
				}
				if((GetKeyState(VK_MBUTTON) & 0x8000) != 0){
					fwKeys = fwKeys | MK_MBUTTON;
				}
				if((GetKeyState(VK_RBUTTON) & 0x8000) != 0){
					fwKeys = fwKeys | MK_RBUTTON;
				}		
				if((GetKeyState(VK_XBUTTON1) & 0x8000) != 0){
					fwKeys = fwKeys | MK_XBUTTON1;
				}
				if((GetKeyState(VK_XBUTTON2) & 0x8000) != 0){
					fwKeys = fwKeys | MK_XBUTTON2;
				}
				XClick *xClick = new XClick();
				if(wParam == WM_RBUTTONUP && hookbtn != 0)
					xClick->button = 0;
				else if(wParam == WM_MBUTTONUP && hookbtn != 1)
					xClick->button = 1;
				else if(wParam == WM_LBUTTONUP && hookbtn != 2)
					xClick->button = 2;
				else if(wParam == WM_XBUTTONUP && GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && hookbtn != 3)
					xClick->button = 3;
				else if(wParam == WM_XBUTTONUP && GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && hookbtn != 4)
					xClick->button = 4;
				xClick->fwKeys = fwKeys;
				xClick->x = CursorInfo.ptScreenPos.x;
				xClick->y = CursorInfo.ptScreenPos.y;
				HANDLE hThread = CreateThread(NULL, 0, &gMouseClick, (LPVOID *)xClick, 0, NULL); 
				CloseHandle(hThread);
			}
		}
	#endif

	//If currently in a gesture, store non-hook button press; gestures can include other mouse buttons as part of definition
	//Basically, here is where we're capturing the modifiers after the stroke button has been pressed
	if(bMouseDown == TRUE && bCancel == FALSE && !bMouseWheelFiring && bLearningMode != TRUE) 
	{
		#ifdef SP_DEBUG_MODE
  			//TCHAR buffer[256];
			//wsprintf(buffer, _T("MouseHook: wParam: %d\n"), wParam);
			//LOGMESSAGE(buffer);
		#endif

		switch(wParam)
		{
			case WM_RBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				if(hookbtn != 0)
				{
					bRightMouseDown = TRUE;
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid);  //Kill timer so it doesn't fire (Cancel Delay)
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc); //Set timer, basically we're just restarting the timer when modifier is pressed
					}
					return TRUE; //consume the DOWN event
				}
				break;
			case WM_NCRBUTTONUP:
			case WM_RBUTTONUP:
				if(hookbtn != 0 && bRightMouseDown)
				{
					return TRUE; //consume the UP event
				}
				break;
			case WM_MBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
				if(hookbtn != 1)
				{
					bMiddleMouseDown = TRUE;
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid);  //Kill timer so it doesn't fire (Cancel Delay)
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc); //Set timer, basically we're just restarting the timer when a modifier is pressed
					}
					return TRUE; //consume the DOWN event
				}
				break;
			case WM_MBUTTONUP:
			case WM_NCMBUTTONUP:
				if(hookbtn != 1 && bMiddleMouseDown)
				{
					return TRUE; //consume the UP event
				}
				break;
			case WM_LBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
				if(hookbtn != 2)
				{
					LOGMESSAGE("Left mouse down");
					bLeftMouseDown = TRUE;
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc); //Set timer, basically we're just restarting the timer when modifier is pressed
					}
					return TRUE; //consume the DOWN event
				}
				break;
			case WM_LBUTTONUP:
			case WM_NCLBUTTONUP:
				if(hookbtn != 2 && bLeftMouseDown)
				{
					return TRUE; //consume the UP event
				}
				break;
			case WM_XBUTTONDOWN:
				if(GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && hookbtn != 3)
				{
					bX1MouseDown = TRUE;
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc); //Set timer, basically we're just restarting the timer when modifier is pressed
					}
					return TRUE; //consume the DOWN event						
				}
				if(GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && hookbtn != 4)
				{
					bX2MouseDown = TRUE;
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc); //Set timer, basically we're just restarting the timer when modifier is pressed
					}
					return TRUE; //consume the DOWN event					
				}
				break;
			case WM_XBUTTONUP:
				if(GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && hookbtn != 3 && bX1MouseDown)
				{
					return TRUE; //consume the UP event
				}
				if(GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && hookbtn != 4 && bX2MouseDown)
				{
					return TRUE; //consume the UP event
				}
				break;
		}
	}

	// MOUSE BUTTON DOWN *****************************************************************************	 
	if((wParam == (hookbtn == 0 ? WM_RBUTTONDOWN : (hookbtn == 1 ? WM_MBUTTONDOWN : (hookbtn == 2 ? WM_LBUTTONDOWN : NULL))) || wParam == (hookbtn == 0 ? WM_NCRBUTTONDOWN : (hookbtn == 1 ? WM_NCMBUTTONDOWN : (hookbtn == 2 ? WM_NCLBUTTONDOWN : NULL)))) || ((GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && wParam == WM_XBUTTONDOWN && hookbtn == 3) || (GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && wParam == WM_XBUTTONDOWN && hookbtn == 4)))
	{
		
		//If the hook button was pressed over a S+ dialog other than the gesture draw window, call next hook
		if(GetAncestor(WindowFromPoint(CursorInfo.ptScreenPos),GA_ROOTOWNER) == hWndServer && WindowFromPoint(CursorInfo.ptScreenPos) != hWndServer){
			return CallNextHookEx(mousehook, nCode, wParam, lParam);		
		}

		//If the hook button has been pressed on an ignored window, call the next hook
	#ifdef SP_NOGLOBALACTIONS
		if(bOnlyAllowDefinedApps) {
			LOGMESSAGE("bOnlyAllowDefinedApps == true");
			string strReturn = GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), false, true);
			LOGMESSAGE(strReturn.c_str());
			if(strReturn.compare(_T("APPDEFINED")) != 0 && bLearningMode != TRUE){
				LOGMESSAGE("Not APPDEFINED");
				 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
			}
		} else {
			LOGMESSAGE("bOnlyAllowDefinedApps == false");
	#endif
			if(GetActionScript(WindowFromPoint(CursorInfo.ptScreenPos), TRUE
				#ifdef SP_NOGLOBALACTIONS
					, false
				#endif
				).compare(_T("IGNORED")) == 0 && bLearningMode != TRUE){
				LOGMESSAGE("Ignored window");
				 return CallNextHookEx(mousehook, nCode, wParam, lParam);		
			}
	#ifdef SP_NOGLOBALACTIONS
		}
	#endif

		//If the a cancel hasn't been issue (disable next gesture) and the mouse 
		//isn't already down (specifically, the hook button), init the beginning of the gesture
		if(bCancel == FALSE && bMouseDown == FALSE)
		{
			bMouseDown = TRUE;
			//The code block below stores the modifiers that were active on stroke button down, if that option is enabled
			if(bCaptureModifiersOnMouseDown) {
				if((GetKeyState(VK_CONTROL) & 0x8000) != 0){
					bControlDownBefore = TRUE;
				}
				if((GetKeyState(VK_MENU) & 0x8000) != 0){
					bAltDownBefore = TRUE;
				}
				if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
					bShiftDownBefore = TRUE;
				}
				if((GetKeyState(VK_LBUTTON) & 0x8000) != 0){
					bLeftMouseDownBefore = TRUE;
				}
				if((GetKeyState(VK_MBUTTON) & 0x8000) != 0){
					bMiddleMouseDownBefore = TRUE;
				}
				if((GetKeyState(VK_RBUTTON) & 0x8000) != 0){
					bRightMouseDownBefore = TRUE;
				}		
				if((GetKeyState(VK_XBUTTON1) & 0x8000) != 0){
					bX1MouseDownBefore = TRUE;
				}
				if((GetKeyState(VK_XBUTTON2) & 0x8000) != 0){
					bX2MouseDownBefore = TRUE;
				}
			}
			//Store the starting point of the gesture and fire the cancel delay timer
			POINTF tmpPointF;
			tmpPointF.x = (float)CursorInfo.ptScreenPos.x; 
			tmpPointF.y = (float)CursorInfo.ptScreenPos.y; 
			DrawPoints.push_back(tmpPointF);
			lastx=CursorInfo.ptScreenPos.x; 
			lasty=CursorInfo.ptScreenPos.y; 
			startx = CursorInfo.ptScreenPos.x; 
			starty = CursorInfo.ptScreenPos.y; 
			canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  
			return TRUE;
		} else {
			return CallNextHookEx(mousehook, nCode, wParam, lParam);
		}

	}

	// MOUSE MOVEMENT *****************************************************************************	 
    if(wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE)
	{
		if(bMouseDown == TRUE && bCancel == FALSE)
		{
			//Draw gesture on screen
			if(!bMouseWheelFiring){
				if(sqrt(pow((CursorInfo.ptScreenPos.x - lastx),2.0) + pow((CursorInfo.ptScreenPos.y - lasty),2.0)) > iMinSegmentLength)  //Has the mouse moved at least the distance of iMinSegmentLength?
				{
					if(bResetCancelDelayOnMovement){
						KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire
						canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when the mouse moves while drawing
					}
					//store the new point for later comparison to identify the gesture
					POINTF tmpPointF;
					tmpPointF.x = (float)CursorInfo.ptScreenPos.x; 
					tmpPointF.y = (float)CursorInfo.ptScreenPos.y; 
					DrawPoints.push_back(tmpPointF);
					setWindowState();
					if(bDrawGesture == TRUE)
					{
						hdc = GetDC(hWndServer);			
						SelectObject(hdc ,hDrawPen);								//Set hdc pen
						MoveToEx(hdc,lastx-rcWindow.left,lasty-rcWindow.top,NULL);	//Draw the line from the last pair of coordiates to current
						LineTo(hdc,CursorInfo.ptScreenPos.x-rcWindow.left,CursorInfo.ptScreenPos.y-rcWindow.top);	

						if(DrawPoints.size() >= iMaxDrawPoints && iMaxDrawPoints > 0)  //Creates a streamer behind the drawing only showing the last NN points, if specifid in preferences window
						{
							SelectObject(hdc ,hBackgroundPen);								//Set hdc pen
							MoveToEx(hdc,DrawPoints[DrawPoints.size()-iMaxDrawPoints].x-rcWindow.left,DrawPoints[DrawPoints.size()-iMaxDrawPoints].y-rcWindow.top,NULL);	//Draw the line frome the last pair of coordiates to current
							LineTo(hdc,DrawPoints[DrawPoints.size()-(iMaxDrawPoints-1)].x-rcWindow.left,DrawPoints[DrawPoints.size()-(iMaxDrawPoints-1)].y-rcWindow.top);			
						}
						ReleaseDC(hWndServer,hdc);
					}
					lastx=CursorInfo.ptScreenPos.x;  //The current x becomes the lastx for next line to be drawn
					lasty=CursorInfo.ptScreenPos.y;  //The current y becomes the lasty for next line to be drawn
				}
			}
		}
		//Pass mouse move onto next hook in chain
		return CallNextHookEx(mousehook, nCode, wParam, lParam); 
	}

	// MOUSE BUTTON UP *****************************************************************************	 
	if((wParam == (hookbtn == 0 ? WM_RBUTTONUP : (hookbtn == 1 ? WM_MBUTTONUP : (hookbtn == 2 ? WM_LBUTTONUP : NULL))) || wParam == (hookbtn == 0 ? WM_NCRBUTTONUP : (hookbtn == 1 ? WM_NCMBUTTONUP : (hookbtn == 2 ? WM_NCLBUTTONUP : NULL)))) || ((GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON1 && wParam == WM_XBUTTONUP && hookbtn == 3) || (GET_XBUTTON_WPARAM(hookStruct->mouseData) == XBUTTON2 && wParam == WM_XBUTTONUP && hookbtn == 4)))
	{
		//bCancel is set by acDisableNext (gesture), on mouse up of the stroke button, this code tells S+ to start watching for stroke button down again
		if(bCancel){
			bCancel = FALSE;
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
		}

		//The stroke button was down (S+ was capturing a gesture), so begin the match and execute process
		if(bMouseDown == TRUE)
		{
			bMouseDown = FALSE;
			KillTimer(NULL,canceltimerid); //stop the cancel delay timer

			//ClearGesture((LPVOID)0); //retraces the captured points with the color key for the gesture draw window, effectively erasing the line
			POINTF start = {(float)startx, (float)starty};
			#ifdef SP_SYNAPTIC
				bSynapticGesture = false;
			#endif

			//if the mouse hasn't moved far enough and no other button/keypress/wheel direction events have occurred, 
			//send the mouse down & up events to the original location (basically emulating this program didn't capture the mouse events)
			if(DrawPoints.size() == 1
				&& bMouseWheelUp == FALSE && bMouseWheelDown == FALSE && bLeftMouseDown == FALSE && bMiddleMouseDown == FALSE && bRightMouseDown == FALSE && bX1MouseDown == FALSE && bX2MouseDown == FALSE
				&& bControlDown == FALSE && bAltDown == FALSE && bShiftDown == FALSE && bLeftMouseDownBefore == FALSE && bMiddleMouseDownBefore == FALSE && bRightMouseDownBefore == FALSE && bX1MouseDownBefore == FALSE && bX2MouseDownBefore == FALSE
				&& bControlDownBefore == FALSE && bAltDownBefore == FALSE && bShiftDownBefore == FALSE)
			{
				ClearGesture((LPVOID)0); //retraces the captured points with the color key for the gesture draw window, effectively erasing the line
				bCancel = TRUE; //Tell hook to ignore the rightdown event
				clearCaptureVars();
				setWindowState();
				if(hookbtn == 3 || hookbtn == 4) {
					mouse_event(MOUSEEVENTF_XDOWN, CursorInfo.ptScreenPos.x, CursorInfo.ptScreenPos.y, (hookbtn == 3 ? XBUTTON1 : XBUTTON2), 0); //Send the xbutton down event to the foreground window
				} else {
					mouse_event((hookbtn == 0 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN) : (hookbtn == 1 ? MOUSEEVENTF_MIDDLEDOWN : (hookbtn == 2 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) : NULL))), CursorInfo.ptScreenPos.x, CursorInfo.ptScreenPos.y, 0, 0); //Send the hook button down event to the foreground window
				}
				if(hookbtn == 3 || hookbtn == 4) {
					mouse_event(MOUSEEVENTF_XUP, CursorInfo.ptScreenPos.x, CursorInfo.ptScreenPos.y, (hookbtn == 3 ? XBUTTON1 : XBUTTON2), 0); //Send the xbutton down event to the foreground window
				} else {
					mouse_event((hookbtn == 0 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP) : (hookbtn == 1 ? MOUSEEVENTF_MIDDLEUP : (hookbtn == 2 ? (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP) : NULL))), CursorInfo.ptScreenPos.x, CursorInfo.ptScreenPos.y, 0, 0); //Send the right button down event to the foreground window
				}
				bCancel = FALSE; //Reset cancel flag
				return TRUE;
			}
			else
			{
				//If S+ isn't in a state where only mouse wheel events are firing (code towards top of this function), go ahead and fire
				if(!bMouseWheelFiring){
					//Fire action here
					HANDLE hThread = CreateThread(NULL, 0, &gGestureComplete, NULL, 0, NULL); 
					CloseHandle(hThread);
				} else {
					#ifdef SP_AFTERRELEASESCRIPT
						//If Allow Release Script is on, then fire the function to execute sp_after_release
						if(bFireOnRelease){
							HANDLE hThread = CreateThread(NULL, 0, &gGestureRelease, NULL, 0, NULL); 
							CloseHandle(hThread);
						}
					#endif
					bMouseWheelFiring = FALSE;
					clearCaptureVars();
					setWindowState();
				}

				return TRUE;
			}
		} else {
			return CallNextHookEx(mousehook, nCode, wParam, lParam);
		}
	} else {
		return CallNextHookEx(mousehook, nCode, wParam, lParam);
	}

	return CallNextHookEx(mousehook, nCode, wParam, lParam);
}

static LRESULT CALLBACK KeyboardProc(UINT nCode, WPARAM wParam, LPARAM lParam)
{

	//This function is used for keyboard-based stroke buttons (Control, Alt, etc.) as well as watching for the ignore key press
	//It is also used to capture Ctrl+Tab / Ctrl+Shift+Tab events when the S+ window is open, for changing tabs

	LOGMESSAGE("in KeyboardProc");

    if(nCode < HC_ACTION)
	{ /* pass it on */
		LOGMESSAGE("nCode < HC_ACTION");
		 return CallNextHookEx(keyboardhook, nCode, wParam, lParam);
	}
	else
	{

		PKBDLLHOOKSTRUCT p = ( PKBDLLHOOKSTRUCT ) lParam;
		HWND hwndForeground = GetForegroundWindow();

		//This is used to capture Ctrl+Tab / Ctrl+Shift+Tab events when the S+ window has focus, for changing tabs
		if((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && hwndForeground == hWndSettings)
		{
			if((GetKeyState(VK_CONTROL) & 0x8000) != 0 && p->vkCode == VK_TAB){
				if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
					PostMessage(hWndSettings, WM_COMMAND, MAKEWPARAM(WM_SETTINGSPREVTAB, 1), 0);
				} else {
					PostMessage(hWndSettings, WM_COMMAND, MAKEWPARAM(WM_SETTINGSNEXTTAB, 1), 0);
				}
				return TRUE;
			}
		}

		//Don't pay attention to keystrokes that S+ is sending during acSendKeys events
		if(bSendKeysExecuting == TRUE)
			return CallNextHookEx(keyboardhook, nCode, wParam, lParam);

		if (bConsumePhysical && !(p->flags & LLKHF_INJECTED)){ 
			if(bMouseWheelFiring){
				//Handles de-capturing the stroke key when a call to acConsumePhysicalInput was called via an action which was trigger by a mouse wheel scroll and Fire on Mouse Wheel is enabled
				if((((GetKeyState(VK_CONTROL) & 0x8000) == 0 && hookbtn == -1) || ((GetKeyState(VK_MENU) & 0x8000) == 0 && hookbtn == -2) || ((GetKeyState(VK_SHIFT) & 0x8000) == 0 && hookbtn == -3) || (p->vkCode == VK_SPACE && hookbtn == -4)))
				{
					bMouseDown = FALSE;
					KillTimer(NULL,canceltimerid);  
					bMouseWheelFiring = FALSE;
					clearCaptureVars();
					setWindowState();
				}
			}
			return TRUE;
		}

		 switch (wParam)  
		 {  
			case WM_KEYDOWN:  
			case WM_SYSKEYDOWN:  
		
				if(bMouseDown==FALSE)
				{
					if(GetAncestor(hwndForeground,GA_ROOTOWNER) != hWndServer && hwndForeground != hWndSettings)
					{
						//Check to see if the ignore key has been pressed
						switch(IgnoreKey)
						{
							case 0: 
								if((GetKeyState(VK_CONTROL) & 0x8000) != 0 && !bIgnoreGesture) {
									bIgnoreGesture = TRUE;
									PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
								}
								break;
							case 1:
								if((GetKeyState(VK_MENU) & 0x8000) != 0 && !bIgnoreGesture) {
									bIgnoreGesture = TRUE;
									PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
								}
								break;
							case 2:
								if((GetKeyState(VK_SHIFT) & 0x8000) != 0 && !bIgnoreGesture) {
									bIgnoreGesture = TRUE;
									PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
								}
								break;
						}
					}

					// CAPTURE KEY DOWN *****************************************************************************	 
					if(!bMouseDown && !bIgnoreGesture && (((GetKeyState(VK_CONTROL) & 0x8000) != 0 && hookbtn == -1) || ((GetKeyState(VK_MENU) & 0x8000) != 0 && hookbtn == -2) || ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && hookbtn == -3) || (p->vkCode == VK_SPACE && hookbtn == -4)))
					{
						POINT screenpoint;
						GetCursorPos (&screenpoint); 
						//If the hook button has been pressed on an ignored window, call the next hook
					#ifdef SP_AFTERRELEASESCRIPT
						if(bOnlyAllowDefinedApps){
							if(GetActionScript(WindowFromPoint(screenpoint), false
								#ifdef SP_NOGLOBALACTIONS
									, true
								#endif
								).compare(_T("APPDEFINED")) != 0 && bLearningMode != TRUE){
								 return CallNextHookEx(keyboardhook, nCode, wParam, lParam);	
							}
						} else {
					#endif
							if(GetActionScript(WindowFromPoint(screenpoint), TRUE
								#ifdef SP_NOGLOBALACTIONS
									, false
								#endif
								).compare(_T("IGNORED")) == 0 && bLearningMode != TRUE){
								 return CallNextHookEx(keyboardhook, nCode, wParam, lParam);	
							}
					#ifdef SP_AFTERRELEASESCRIPT
						}
					#endif

						//If the a cancel hasn't been issue (disable next gesture) and the mouse 
						//isn't already down (specifically, the hook button), init the beginning of the gesture
						if(bCancel == FALSE && bMouseDown == FALSE)
						{
							//Capture the modifiers pressed at the time of the stroke key press, if enabled
							if(bCaptureModifiersOnMouseDown) {
								if((GetKeyState(VK_CONTROL) & 0x8000) != 0 && hookbtn != -1){
									bControlDownBefore = TRUE;
								}
								if((GetKeyState(VK_MENU) & 0x8000) != 0  && hookbtn != -2){
									bAltDownBefore = TRUE;
								}
								if((GetKeyState(VK_SHIFT) & 0x8000) != 0 && hookbtn != -3){
									bShiftDownBefore = TRUE;
								}
								if((GetKeyState(VK_LBUTTON) & 0x8000) != 0){
									bLeftMouseDownBefore = TRUE;
								}
								if((GetKeyState(VK_MBUTTON) & 0x8000) != 0){
									bMiddleMouseDownBefore = TRUE;
								}
								if((GetKeyState(VK_RBUTTON) & 0x8000) != 0){
									bRightMouseDownBefore = TRUE;
								}		
								if((GetKeyState(VK_XBUTTON1) & 0x8000) != 0){
									bX1MouseDownBefore = TRUE;
								}
								if((GetKeyState(VK_XBUTTON2) & 0x8000) != 0){
									bX2MouseDownBefore = TRUE;
								}
							}
							bConsumeStrokeKey = true;
							bMouseDown = TRUE;
							//capture the starting point of the gesture
							//MouseProc > WM_MOUSEMOVE handles the drawing and remaining gesture capture
							POINTF tmpPointF;
							tmpPointF.x = (float)screenpoint.x;
							tmpPointF.y = (float)screenpoint.y;
							DrawPoints.push_back(tmpPointF);
							lastx=screenpoint.x;
							lasty=screenpoint.y;
							startx = screenpoint.x;
							starty = screenpoint.y;
							canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  
							return TRUE;
						} else {
							if(bConsumeStrokeKey){
								return TRUE;
							} else {
								return CallNextHookEx(keyboardhook, nCode, wParam, lParam);
							}
						}
					}

				} else {
					//Capture the After modifier key events
					if(!bMouseWheelFiring){
						bool bHandled = false;
						if((GetKeyState(VK_CONTROL) & 0x8000) != 0 && hookbtn != -1){
							if(bResetCancelDelayOnMovement){
								KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire
								canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when the mouse moves while drawing
							}
							bControlDown = TRUE;
							bHandled = true;
						}
						if((GetKeyState(VK_MENU) & 0x8000) != 0 && hookbtn != -2){
							if(bResetCancelDelayOnMovement){
								KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire
								canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when the mouse moves while drawing
							}
							bAltDown = TRUE;
							bHandled = true;
						}
						if((GetKeyState(VK_SHIFT) & 0x8000) != 0 && hookbtn != -3){
							if(bResetCancelDelayOnMovement){
								KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire
								canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when the mouse moves while drawing
							}
							bShiftDown = TRUE;
							bHandled = true;
						}
						if(p->vkCode == VK_SPACE && hookbtn == -4){
							bHandled = true;
						}
						if(bHandled){
							return TRUE;
						}
					}
					if(!bFireOnMouseWheel){
						return TRUE;	
					}
				}
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				if(bIgnoreGesture == TRUE && GetAncestor(GetForegroundWindow(),GA_ROOTOWNER) != hWndServer)
				{
					//If the ignore key has been released, allow S+ to capture stroke button down events
					switch(IgnoreKey)
					{
						case 0:
							if((GetKeyState(VK_CONTROL) & 0x8000) == 0) {
								bIgnoreGesture = FALSE;
								PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
							}
							break;
						case 1:
							if((GetKeyState(VK_MENU) & 0x8000) == 0 ) {
								bIgnoreGesture = FALSE;
								PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
							}
							break;
						case 2:
							if((GetKeyState(VK_SHIFT) & 0x8000) == 0) {
								bIgnoreGesture = FALSE;
								PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
							}
							break;
						default:
							break;
					}
				}

				// CAPTURE KEY UP *****************************************************************************	 
				if(!bIgnoreGesture && (((GetKeyState(VK_CONTROL) & 0x8000) == 0 && hookbtn == -1) || ((GetKeyState(VK_MENU) & 0x8000) == 0 && hookbtn == -2) || ((GetKeyState(VK_SHIFT) & 0x8000) == 0 && hookbtn == -3) || (p->vkCode == VK_SPACE && hookbtn == -4)))
				{
					if(bCancel){
						bCancel = FALSE;
						PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
					}
					if(bMouseDown == TRUE)
					{
						#ifdef SP_SYNAPTIC
							bSynapticGesture = false;
						#endif
						bConsumeStrokeKey = false;
						bMouseDown = FALSE;
						KillTimer(NULL,canceltimerid); //kill the cancel delay timer

						ClearGesture((LPVOID)0); //Erase the gesture line
						POINTF start = {(float)startx, (float)starty};

						POINT screenpoint;
						GetCursorPos (&screenpoint); 

						//if the mouse hasn't moved far enough and no other button/keypress/wheel direction events have occurred, 
						//send the mouse down & up events to the original location (basically emulating this program didn't capture the mouse events)
						if(DrawPoints.size() == 1
							&& bMouseWheelUp == FALSE && bMouseWheelDown == FALSE && bLeftMouseDown == FALSE && bMiddleMouseDown == FALSE && bRightMouseDown == FALSE && bX1MouseDown == FALSE && bX2MouseDown == FALSE
							&& bControlDown == FALSE && bAltDown == FALSE && bShiftDown == FALSE && bLeftMouseDownBefore == FALSE && bMiddleMouseDownBefore == FALSE && bRightMouseDownBefore == FALSE && bX1MouseDownBefore == FALSE && bX2MouseDownBefore == FALSE
							&& bControlDownBefore == FALSE && bAltDownBefore == FALSE && bShiftDownBefore == FALSE)
						{
							bCancel = TRUE; //Tell hook to ignore the rightdown event
							clearCaptureVars();
							setWindowState();
							
							//***** SEND ORIGINAL KEY DOWN/KEY_UP EVENTS
							if(hookbtn == -1){
								acSendControlDown();
								acSendControlUp();
							} else if (hookbtn == -2) {
								acSendAltDown();
								acSendAltUp();
							} else if (hookbtn == -3) {
								acSendShiftDown();
								acSendShiftUp();
							} else if (hookbtn == -4) {
								acSendKeys(" ", false); 
							}

							bCancel = FALSE; //Reset cancel flag
							return TRUE;
						}
						else
						{
							if(!bMouseWheelFiring){
								//Fire action here
								HANDLE hThread = CreateThread(NULL, 0, &gGestureComplete, NULL, 0, NULL); 
								CloseHandle(hThread);
							} else {
								//If Allow Release Script is on, then fire the function to execute sp_after_release
								#ifdef SP_AFTERRELEASESCRIPT
									if(bFireOnRelease){
										HANDLE hThread = CreateThread(NULL, 0, &gGestureRelease, NULL, 0, NULL); 
										CloseHandle(hThread);
									}
								#endif
								bMouseWheelFiring = FALSE;
								clearCaptureVars();
								setWindowState();
								if(hookbtn == -1){
									acSendControlUp();
								} else if (hookbtn == -2) {
									acSendAltUp();
								} else if (hookbtn == -3) {
									acSendShiftUp();
								}
							}

							return TRUE;
						}
					} else {
						if(bMouseDown){
							return TRUE;
						} else {
							if(bConsumeStrokeKey){
								bConsumeStrokeKey = false;
								return TRUE;
							} else {
								return CallNextHookEx(keyboardhook, nCode, wParam, lParam);
							}
						}
					}
				} else {
					return CallNextHookEx(keyboardhook, nCode, wParam, lParam);
				}

				//we're currently capturing a gesture, consume the KEY_UP events
				if(bMouseDown && !bMouseWheelFiring)
				{
					if((GetKeyState(VK_CONTROL) & 0x8000) == 0) {
						return TRUE;
					}
					if((GetKeyState(VK_MENU) & 0x8000) == 0 ) {
						return TRUE;
					}
					if((GetKeyState(VK_SHIFT) & 0x8000) == 0) {
						return TRUE;
					}
				}
				break;
		 }

		return CallNextHookEx(keyboardhook, nCode, wParam, lParam);
	}
}

static BOOL CALLBACK FindWindowImpl( HWND hWnd, LPARAM lParam ) 
{
	//Used by acFindWindow

    FindWindowData * p = reinterpret_cast<FindWindowData*>( LongToPtr( lParam ) );
    if( !p ) {
        // Finish enumerating we received an invalid parameter
        return FALSE;
    }

	TCHAR szOwnerTitle[1024];
	memset(szOwnerTitle,0,sizeof(szOwnerTitle));

    int length = GetWindowTextLength( hWnd ) + 1;
    if( length > 0 ) {
        std::vector<TCHAR> buffer( std::size_t( length ), 0 );      
        if( GetWindowText(hWnd,szOwnerTitle,sizeof(szOwnerTitle)) ) {
			boost::regex e(p->WindowTitle.c_str());
			if(boost::regex_match(szOwnerTitle, e ) == TRUE)
			{
				p->ResultHandle = hWnd;
				return FALSE;
			}
        }
    }
    // Continue enumerating
    return TRUE;
}

BOOL CALLBACK MonitorsCallBack( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	LOGMESSAGE("Enter MonitorsCallBack");
	//USed by the various monitor actions

	if(hFirstEnumMonitor == NULL)
		hFirstEnumMonitor = hMonitor;

	if(hCurrentEnumMonitor != NULL)
		hLastEnumMonitor = hCurrentEnumMonitor;

	hCurrentEnumMonitor = hMonitor;

	MONITORINFOEX mi;
	mi.cbSize = sizeof( mi );
	GetMonitorInfo( hMonitor, &mi );
			
	switch(dwData){
		case 1:
			//send to last monitor
			if(hLastEnumMonitor != NULL){
				if(hActiveMonitor == hCurrentEnumMonitor){
					hSendToMonitor = hLastEnumMonitor;
					return FALSE;
				}
			}
			break;

		case 2:
			//send to next monitor
			if(hLastEnumMonitor != NULL){
				if(hActiveMonitor == hLastEnumMonitor){
					hSendToMonitor = hCurrentEnumMonitor;
					return FALSE;
				}
			}
			break;

		case 3:
			//send to monitor by handle
			if(hIncomingMonitor == hCurrentEnumMonitor){
				hSendToMonitor = hCurrentEnumMonitor;
				return FALSE;
			}
			break;

		case 4:
			//send to monitor by name
			std::basic_string<TCHAR> sDevice = mi.szDevice;
			if (boost::iequals(sDevice, sMonitorName)){
				hSendToMonitor = hCurrentEnumMonitor;
				return FALSE;
			}
			break;
	}


	return TRUE;
}

#pragma endregion Win32 Callbacks


#pragma region SendKeys


/* 
* ----------------------------------------------------------------------------- 
* Copyright (c) 2004 lallous <lallousx86@yahoo.com>
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ----------------------------------------------------------------------------- 


The Original SendKeys copyright info
---------------------------------------
SendKeys (sndkeys32.pas) routine for 32-bit Delphi.
Written by Ken Henderson
Copyright (c) 1995 Ken Henderson <khen@compuserve.com>

History
----------
04/19/2004
  * Initial version development
04/21/2004
  * Added number of times specifier to special keys
  * Added {BEEP X Y}
  * Added {APPACTIVATE WindowTitle}
  * Added CarryDelay() and now delay works properly with all keys
  * Added SetDelay() method
  * Fixed code in AppActivate that allowed to pass both NULL windowTitle/windowClass

05/21/2004
  * Fixed a bug in StringToVKey() that caused the search for RIGHTPAREN to be matched as RIGHT
  * Adjusted code so it compiles w/ VC6
05/24/2004
  * Added unicode support

Todo
-------
* perhaps add mousecontrol: mousemove+mouse clicks
* allow sending of normal keys multiple times as: {a 10}

*/

const WORD VKKEYSCANSHIFTON = 0x01;
const WORD VKKEYSCANCTRLON  = 0x02;
const WORD VKKEYSCANALTON   = 0x04;
const WORD INVALIDKEY       = 0xFFFF;

const BYTE ExtendedVKeys[MaxExtendedVKeys] =
{
    VK_UP, 
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,
    VK_HOME,
    VK_END,
    VK_PRIOR, // PgUp
    VK_NEXT,  //  PgDn
    VK_INSERT,
    VK_DELETE
};


// Delphi port regexps:
// ---------------------
// search: .+Name:'([^']+)'.+vkey:([^\)]+)\)
// replace: {"\1", \2}
//
// **If you add to this list, you must be sure to keep it sorted alphabetically
// by Name because a binary search routine is used to scan it.**
// Rob: Also note that this code (as it was written) matches in sequence, so having {F1} would conflict with {F11}, firing F1 before comparing the rest of the string.
//      This is why I changed to use {F_1}..basically, make sure that no two strings below start with the same sequence of characters. Well, make sure that the entirety 
//      of a string isn't a match in another, like the F11 and F1 example I mentioned.
//
key_desc_t KeyNames[MaxSendKeysRecs] = 
{
  {_T("ADD"), VK_ADD}, 
  {_T("APPS"), VK_APPS},
  {_T("AT"), '@', true},
  {_T("BACKSPACE"), VK_BACK},
  {_T("BKSP"), VK_BACK},
  {_T("BREAK"), VK_CANCEL},
  {_T("BROWSERBACK"), VK_BROWSER_BACK},
  {_T("BROWSERFAVORITES"), VK_BROWSER_FAVORITES},
  {_T("BROWSERFORWARD"), VK_BROWSER_FORWARD},
  {_T("BROWSERHOME"), VK_BROWSER_HOME},
  {_T("BROWSERREFRESH"), VK_BROWSER_REFRESH},
  {_T("BROWSERSEARCH"), VK_BROWSER_SEARCH},
  {_T("BROWSERSTOP"), VK_BROWSER_STOP},
  {_T("BS"), VK_BACK},
  {_T("CAPSLOCK"), VK_CAPITAL},
  {_T("CARET"), '^', true},
  {_T("CLEAR"), VK_CLEAR},
  {_T("DECIMAL"), VK_DECIMAL}, 
  {_T("DEL"), VK_DELETE},
  {_T("DELETE"), VK_DELETE},
  {_T("DIVIDE"), VK_DIVIDE}, 
  {_T("DOWN"), VK_DOWN},
  {_T("END"), VK_END},
  {_T("ENTER"), VK_RETURN},
  {_T("ESC"), VK_ESCAPE},
  {_T("ESCAPE"), VK_ESCAPE},
  {_T("F10"), VK_F10},
  {_T("F11"), VK_F11},
  {_T("F12"), VK_F12},
  {_T("F13"), VK_F13},
  {_T("F14"), VK_F14},
  {_T("F15"), VK_F15},
  {_T("F16"), VK_F16},
  {_T("F_1"), VK_F1},
  {_T("F_2"), VK_F2},
  {_T("F_3"), VK_F3},
  {_T("F_4"), VK_F4},
  {_T("F_5"), VK_F5},
  {_T("F_6"), VK_F6},
  {_T("F_7"), VK_F7},
  {_T("F_8"), VK_F8},
  {_T("F_9"), VK_F9},
  {_T("HELP"), VK_HELP},
  {_T("HOME"), VK_HOME},
  {_T("INS"), VK_INSERT},
  {_T("LBRACE"), '{', true},
  {_T("LEFT"), VK_LEFT},
  {_T("LPAREN"), '(', true},
  {_T("LWIN"), VK_LWIN},
  {_T("MEDIANEXTTRACK"), VK_MEDIA_NEXT_TRACK},
  {_T("MEDIAPLAYPAUSE"), VK_MEDIA_PLAY_PAUSE},
  {_T("MEDIAPREVTRACK"), VK_MEDIA_PREV_TRACK},
  {_T("MEDIASTOP"), VK_MEDIA_STOP},
  {_T("MULTIPLY"), VK_MULTIPLY}, 
  {_T("NUMLOCK"), VK_NUMLOCK},
  {_T("NUMPAD0"), VK_NUMPAD0}, 
  {_T("NUMPAD1"), VK_NUMPAD1}, 
  {_T("NUMPAD2"), VK_NUMPAD2}, 
  {_T("NUMPAD3"), VK_NUMPAD3}, 
  {_T("NUMPAD4"), VK_NUMPAD4}, 
  {_T("NUMPAD5"), VK_NUMPAD5}, 
  {_T("NUMPAD6"), VK_NUMPAD6}, 
  {_T("NUMPAD7"), VK_NUMPAD7}, 
  {_T("NUMPAD8"), VK_NUMPAD8}, 
  {_T("NUMPAD9"), VK_NUMPAD9}, 
  {_T("PERCENT"), '%', true},
  {_T("PGDN"), VK_NEXT},
  {_T("PGUP"), VK_PRIOR},
  {_T("PLUS"), '+', true},
  {_T("PRTSC"), VK_PRINT},
  {_T("RBRACE"), '}', true},
  {_T("RIGHT"), VK_RIGHT},
  {_T("RPAREN"), ')', true},
  {_T("RWIN"), VK_RWIN},
  {_T("SCROLL"), VK_SCROLL},
  {_T("SEPARATOR"), VK_SEPARATOR}, 
  {_T("SLEEP"), VK_SLEEP}, 
  {_T("SNAPSHOT"), VK_SNAPSHOT},
  {_T("SUBTRACT"), VK_SUBTRACT}, 
  {_T("TAB"), VK_TAB},
  {_T("TILDE"), '~', true}, 
  {_T("UP"), VK_UP},
  {_T("VOLDOWN"), VK_VOLUME_DOWN},
  {_T("VOLMUTE"), VK_VOLUME_MUTE},
  {_T("VOLUP"), VK_VOLUME_UP},
  {_T("WIN"), VK_LWIN}
};


// calls keybd_event() and waits, if needed, till the sent input is processed
void KeyboardEvent(BYTE VKey, BYTE ScanCode, LONG Flags)
{
  MSG KeyboardMsg;

  keybd_event(VKey, ScanCode, Flags, 0);

  if (m_bWait)
  {
    while (PeekMessage(&KeyboardMsg, 0, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
    {
      TranslateMessage(&KeyboardMsg);
      DispatchMessage(&KeyboardMsg);
    }
  }
}

// Checks whether the specified VKey is an extended key or not
bool IsVkExtended(BYTE VKey)
{
  for (int i=0;i<MaxExtendedVKeys;i++)
  {
    if (ExtendedVKeys[i] == VKey)
      return true;
  }
  return false;
}

// Generates KEYUP
void SendKeyUp(BYTE VKey)
{
  BYTE ScanCode = LOBYTE(MapVirtualKey(VKey, 0));

  KeyboardEvent(VKey, 
                ScanCode, 
                KEYEVENTF_KEYUP | (IsVkExtended(VKey) ? KEYEVENTF_EXTENDEDKEY : 0));
}

void SendKeyDown(BYTE VKey, WORD NumTimes, bool GenUpMsg, bool bDelay)
{
  WORD Cnt = 0;
  BYTE ScanCode = 0;
  bool NumState = false;

  if (VKey == VK_NUMLOCK)
  {
    DWORD dwVersion = GetVersion();

    for (Cnt=1; Cnt<=NumTimes; Cnt++)
    {
      if (bDelay)
        CarryDelay();
      // snippet based on:
      // http://www.codeproject.com/cpp/togglekeys.asp
      if (dwVersion < 0x80000000)
      {
        keybd_event(VKey, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(VKey, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      }
      else
      {
        // Win98 and later
        if ( ((DWORD)(HIBYTE(LOWORD(dwVersion))) >= 10) )
        {
          // Define _WIN32_WINNT > 0x0400
          // to compile
          INPUT input[2] = {0};
          input[0].type = input[1].type = INPUT_KEYBOARD;
          input[0].ki.wVk = input[1].ki.wVk = VKey;
          input[1].ki.dwFlags = KEYEVENTF_KEYUP;
          SendInput(sizeof(input) / sizeof(INPUT), input, sizeof(INPUT));
        }
        // Win95
        else
        {
          KEYBOARDSTATE_t KeyboardState;
          NumState = GetKeyState(VK_NUMLOCK) & 1 ? true : false;
          GetKeyboardState(&KeyboardState[0]);
          if (NumState)
            KeyboardState[VK_NUMLOCK] &= ~1;
          else
            KeyboardState[VK_NUMLOCK] |= 1;

          SetKeyboardState(&KeyboardState[0]);
        }
      }
    }
    return;
  }

  // Get scancode
  ScanCode = LOBYTE(MapVirtualKey(VKey, 0));

  // Send keys
  for (Cnt=1; Cnt<=NumTimes; Cnt++)
  {
    // Carry needed delay ?
    if (bDelay)
      CarryDelay();

    KeyboardEvent(VKey, ScanCode, IsVkExtended(VKey) ? KEYEVENTF_EXTENDEDKEY : 0);
    if (GenUpMsg)
      KeyboardEvent(VKey, ScanCode, (IsVkExtended(VKey) ? KEYEVENTF_EXTENDEDKEY : 0) | KEYEVENTF_KEYUP);  
  }
}

// Checks whether a bit is set
bool BitSet(BYTE BitTable, UINT BitMask)
{
  return BitTable & BitMask ? true : false;
}

// Sends a single key
void SendKey(WORD MKey, WORD NumTimes, bool GenDownMsg)
{
  // Send appropriate shift keys associated with the given VKey
  if (BitSet(HIBYTE(MKey), VKKEYSCANSHIFTON))
    SendKeyDown(VK_SHIFT, 1, false);

  if (BitSet(HIBYTE(MKey), VKKEYSCANCTRLON))
    SendKeyDown(VK_CONTROL, 1, false);

  if (BitSet(HIBYTE(MKey), VKKEYSCANALTON))
    SendKeyDown(VK_MENU, 1, false);

  // Send the actual VKey
  SendKeyDown(LOBYTE(MKey), NumTimes, GenDownMsg, true);

  // toggle up shift keys
  if (BitSet(HIBYTE(MKey), VKKEYSCANSHIFTON))
    SendKeyUp(VK_SHIFT);

  if (BitSet(HIBYTE(MKey), VKKEYSCANCTRLON))
    SendKeyUp(VK_CONTROL);

  if (BitSet(HIBYTE(MKey), VKKEYSCANALTON))
    SendKeyUp(VK_MENU);
}

// Implements a simple binary search to locate special key name strings
WORD StringToVKey(LPCTSTR KeyString, int &idx)
{
  bool Found = false, Collided;
  int  Bottom = 0, 
       Top = MaxSendKeysRecs,
       Middle = (Bottom + Top) / 2;
  WORD retval = INVALIDKEY;

  idx    = -1;

  do
  {
    Collided = (Bottom == Middle) || (Top == Middle);
    int cmp = _tcsnicmp(KeyNames[Middle].keyName, KeyString, _tcslen(KeyString));
    if (cmp == 0)
    {
      Found = true;
      retval = KeyNames[Middle].VKey;
      idx    = Middle;
      break;
    }
    else
    {
      if (cmp < 0)
        Bottom = Middle;
      else
        Top = Middle;
      Middle = (Bottom + Top) / 2;
    }
  } while (!(Found || Collided));

  return retval;
}

// Releases all shift keys (keys that can be depressed while other keys are being pressed
// If we are in a modifier group this function does nothing
void PopUpShiftKeys()
{
  if (!m_bUsingParens)
  {
    if (m_bShiftDown)
      SendKeyUp(VK_SHIFT);
    if (m_bControlDown)
      SendKeyUp(VK_CONTROL);
    if (m_bAltDown)
      SendKeyUp(VK_MENU);
    if (m_bWinDown)
      SendKeyUp(VK_LWIN);
    m_bWinDown = m_bShiftDown = m_bControlDown = m_bAltDown = false;
  }
}

// Sends a key string
bool SendKeys(LPCTSTR KeysString, bool Wait, bool SendDown, bool SendUp)
{
  WORD MKey, NumTimes;
  TCHAR KeyString[1024] = {0};
  bool retval  = false;
  int  keyIdx;

  LPTSTR pKey = (LPTSTR) KeysString;
  TCHAR  ch;

  m_bWait = Wait;

  m_bWinDown = m_bShiftDown = m_bControlDown = m_bAltDown = m_bUsingParens = false;

  while (ch = *pKey)
  {
    switch (ch)
    {
    // begin modifier group
    case _TXCHAR('('):
      m_bUsingParens = true;
      break;

    // end modifier group
    case _TXCHAR(')'):
      m_bUsingParens = false;
      PopUpShiftKeys(); // pop all shift keys when we finish a modifier group close
      break;

    // ALT key
    case _TXCHAR('%'):
      m_bAltDown = true;
      SendKeyDown(VK_MENU, 1, false);
      break;

    // SHIFT key
    case _TXCHAR('+'):
      m_bShiftDown = true;
      SendKeyDown(VK_SHIFT, 1, false);
      break;

    // CTRL key
    case _TXCHAR('^'):
      m_bControlDown = true;
      SendKeyDown(VK_CONTROL, 1, false);
      break;

    // WINKEY (Left-WinKey)
    case '@':
      m_bWinDown = true;
      SendKeyDown(VK_LWIN, 1, false);
      break;

    // enter
    case _TXCHAR('~'):
      SendKeyDown(VK_RETURN, 1, true);
      PopUpShiftKeys();
      break;

    // begin special keys
    case _TXCHAR('{'):
      {

        LPTSTR p = pKey+1; // skip past the beginning '{'
        size_t t;

        // find end of close
        while (*p && *p != _TXCHAR('}'))
          p++;

        t = p - pKey;
        // special key definition too big?
        if (t > sizeof(KeyString))
          return false;

        // Take this KeyString into local buffer
        _tcsncpy(KeyString, pKey+1, t);

        KeyString[t-1] = _TXCHAR('\0');
        keyIdx = -1;

        pKey += t; // skip to next keystring

        // Invalidate key
        MKey = INVALIDKEY;

        // sending arbitrary vkeys?
        if (_tcsnicmp(KeyString, _T("VKEY"), 4) == 0)
        {
		  NumTimes = 1;
          p = KeyString + 4;
          MKey = _ttoi(p);
        }
        else if (_tcsnicmp(KeyString, _T("BEEP"), 4) == 0)
        {
          p = KeyString + 4 + 1;
          LPTSTR p1 = p;
          DWORD frequency, delay;

          if ((p1 = _tcsstr(p, _T(" "))) != NULL)
          {
            *p1++ = _TXCHAR('\0');
            frequency = _ttoi(p);
            delay = _ttoi(p1);
            ::Beep(frequency, delay);
          }
        }
        // Should activate a window?
        /*else if (_tcsnicmp(KeyString, _T("APPACTIVATE"), 11) == 0)
        {
          p = KeyString + 11 + 1;
          AppActivate(p);
        }*/
        // want to send/set delay?
        else if (_tcsnicmp(KeyString, _T("DELAY"), 5) == 0)
        {
          // Advance to parameters
          p = KeyString + 5;
          // set "sleep factor"
          if (*p == _TXCHAR('='))
            m_nDelayAlways = _ttoi(p + 1); // Take number after the '=' character
          else
            // set "sleep now"
            m_nDelayNow = _ttoi(p);
        }
        // not command special keys, then process as keystring to VKey
        else
        {
          MKey = StringToVKey(KeyString, keyIdx);
          // Key found in table
          if (keyIdx != -1)
          {
            NumTimes = 1;

            // Does the key string have also count specifier?
            t = _tcslen(KeyNames[keyIdx].keyName);
            if (_tcslen(KeyString) > t)
            {
              p = KeyString + t;
              // Take the specified number of times
              NumTimes = _ttoi(p);
            }

            if (KeyNames[keyIdx].normalkey)
              MKey = VkKeyScan(KeyNames[keyIdx].VKey);
          }
        }

        // A valid key to send?
        if (MKey != INVALIDKEY)
        {
          SendKey(MKey, NumTimes, true);
          PopUpShiftKeys();
        }
      }
      break;

      // a normal key was pressed
    default:
      // Get the VKey from the key
      MKey = VkKeyScan(ch);
      SendKey(MKey, 1, true);
      PopUpShiftKeys();
    }
    pKey++;
  }

  m_bUsingParens = false;
  PopUpShiftKeys();
  return true;
}


// Carries the required delay and clears the m_nDelaynow value
void CarryDelay()
{
  // Should we delay once?
  if (!m_nDelayNow)
    // should we delay always?
    m_nDelayNow = m_nDelayAlways;

  // No delay specified?
  if (m_nDelayNow)
    Sleep(m_nDelayNow); //::Beep(100, m_nDelayNow);

  // clear SleepNow
  m_nDelayNow = 0;
}


#pragma endregion SendKeys


#pragma region Touch & Pen Functions

#ifdef SP_TOUCH

__declspec(dllexport) void processInputMessage(UINT message, WPARAM wParam, LPARAM lParam){

	switch(message){

		case WM_POINTERDEVICECHANGE:
			{
				LOGMESSAGE("WM_POINTERDEVICECHANGE");
				break;
			}
		case WM_POINTERDEVICEINRANGE:
			{
				LOGMESSAGE("WM_POINTERDEVICEINRANGE");
				break;
			}
		case WM_POINTERDEVICEOUTOFRANGE:
			{
				LOGMESSAGE("WM_POINTERDEVICEOUTOFRANGE");
				break;
			}
		case WM_NCPOINTERUPDATE:
			{
				LOGMESSAGE("WM_NCPOINTERUPDATE");
				break;
			}
		case WM_NCPOINTERDOWN:
			{
				LOGMESSAGE("WM_NCPOINTERDOWN");
				break;
			}
		case WM_NCPOINTERUP:
			{
				LOGMESSAGE("WM_NCPOINTERUP");
				break;
			}
		case WM_POINTERUPDATE:
			{
				tickcount++;
				if(tickcount%40==0 || tickcount%41==0){
  					TCHAR buffer[512];
					wsprintf(buffer, 
					   _T("WM_POINTERUPDATE: ptr_id: %d - isnew: %d - in_contact: %d - ptr_is_primary: %d - is_first: %d - is_sec: %d - is_3rd: %d - x: %d - y: %d"),
					   GET_POINTERID_WPARAM(wParam), IS_POINTER_NEW_WPARAM(wParam), IS_POINTER_INCONTACT_WPARAM(wParam),IS_POINTER_PRIMARY_WPARAM(wParam), 
					   IS_POINTER_FIRSTBUTTON_WPARAM(wParam), IS_POINTER_SECONDBUTTON_WPARAM(wParam), IS_POINTER_THIRDBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					LOGMESSAGE(buffer);
				}
				break;
			}
		case WM_POINTERDOWN:
			{
  				TCHAR buffer[512];
				wsprintf(buffer, 
				   _T("WM_POINTERDOWN: ptr_id: %d - isnew: %d - in_contact: %d - ptr_is_primary: %d - is_first: %d - is_sec: %d - is_3rd: %d - x: %d - y: %d"),
				   GET_POINTERID_WPARAM(wParam), IS_POINTER_NEW_WPARAM(wParam), IS_POINTER_INCONTACT_WPARAM(wParam),IS_POINTER_PRIMARY_WPARAM(wParam), 
				   IS_POINTER_FIRSTBUTTON_WPARAM(wParam), IS_POINTER_SECONDBUTTON_WPARAM(wParam), IS_POINTER_THIRDBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			
				LOGMESSAGE(buffer);
				break;
			}
		case WM_POINTERUP:
			{
  				TCHAR buffer[512];
				wsprintf(buffer, 
				   _T("WM_POINTERUP: ptr_id: %d - isnew: %d - in_contact: %d - ptr_is_primary: %d - is_first: %d - is_sec: %d - is_3rd: %d - x: %d - y: %d"),
				   GET_POINTERID_WPARAM(wParam), IS_POINTER_NEW_WPARAM(wParam), IS_POINTER_INCONTACT_WPARAM(wParam),IS_POINTER_PRIMARY_WPARAM(wParam), 
				   IS_POINTER_FIRSTBUTTON_WPARAM(wParam), IS_POINTER_SECONDBUTTON_WPARAM(wParam), IS_POINTER_THIRDBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			
				LOGMESSAGE(buffer);
				break;
			}
		case WM_POINTERENTER:
			{
  				TCHAR buffer[512];
				wsprintf(buffer, 
				   _T("WM_POINTERENTER: ptr_id: %d - isnew: %d - in_contact: %d - ptr_is_primary: %d - is_first: %d - is_sec: %d - is_3rd: %d - x: %d - y: %d"),
				   GET_POINTERID_WPARAM(wParam), IS_POINTER_NEW_WPARAM(wParam), IS_POINTER_INCONTACT_WPARAM(wParam),IS_POINTER_PRIMARY_WPARAM(wParam), 
				   IS_POINTER_FIRSTBUTTON_WPARAM(wParam), IS_POINTER_SECONDBUTTON_WPARAM(wParam), IS_POINTER_THIRDBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			
				LOGMESSAGE(buffer);
				break;
			}
		case WM_POINTERLEAVE:
			{
  				TCHAR buffer[512];
				wsprintf(buffer, 
				   _T("WM_POINTERLEAVE: ptr_id: %d - isnew: %d - in_contact: %d - ptr_is_primary: %d - is_first: %d - is_sec: %d - is_3rd: %d - x: %d - y: %d"),
				   GET_POINTERID_WPARAM(wParam), IS_POINTER_NEW_WPARAM(wParam), IS_POINTER_INCONTACT_WPARAM(wParam),IS_POINTER_PRIMARY_WPARAM(wParam), 
				   IS_POINTER_FIRSTBUTTON_WPARAM(wParam), IS_POINTER_SECONDBUTTON_WPARAM(wParam), IS_POINTER_THIRDBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			
				LOGMESSAGE(buffer);
				break;
			}
		case WM_POINTERACTIVATE:
			{
				LOGMESSAGE("WM_POINTERACTIVATE");
				break;
			}
		case WM_POINTERCAPTURECHANGED:
			{
				LOGMESSAGE("WM_POINTERCAPTURECHANGED");
				break;
			}
		case WM_TOUCHHITTESTING:
			{
				LOGMESSAGE("WM_TOUCHHITTESTING");
				break;
			}
		case WM_POINTERWHEEL:
			{
				LOGMESSAGE("WM_POINTERWHEEL");
				break;
			}
		case WM_POINTERHWHEEL:
			{
				LOGMESSAGE("WM_POINTERHWHEEL");
				break;
			}
	}


	//On Global Touch Input 
		//if(touch input matches the S+ capture sequence){
		//		if(gesture capture sequence is no longer valid [extra fingers detected, etc.])
		//			set cancel flag ? are we sure?
		//			//create function to handle this, for which TIMERPROC can also call to replay on Cancel Delay Timeout
		//			for(each touch event in touchhistory [see MSDN]){
		//				//replay touch history
		//				Call lpfnInitializeTouchInjection
		//				Call lpfnInjectTouchInput
		//			}
		//		} else {
		//			if(gesture release [fingers removed from surface]){
		//				match gesture
		//				fire action
		//			} else {
		//				push points to vector for gesture recognition
		//				initially, only capture the points for the first touch (finger)
		//				capture modifers
		//			}
		//		}
		//} else {
		//		//inject touch input since it doesn't match S+ capture sequence
		//		Call lpfnInitializeTouchInjection
		//		Call lpfnInjectTouchInput
		//}

	//If timeout has been reached..handle in here or in TimerProc like all other code? Well, except Synaptics which doesn't replay anything

	if(bCancel){
		//if all pointer contacts have left the surface
		bCancel = FALSE;
		PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
	}
}

__declspec(dllexport) bool isTouchAvailable(){
	return bTouchAvailable;
}

__declspec(dllexport) bool isTouchEnabled(){
	return bTouchEnabled;
}

__declspec(dllexport) bool isPenEnabled(){
	return bPenEnabled;
}

__declspec(dllexport) void InitTouchWindow(HWND hWnd){
	if(bTouchEnabled){
		if(lpfnRegisterTouchInput(hWnd, PT_TOUCH)==0){
			bTouchEnabled = false;
			MessageBox(GetForegroundWindow(),ptLang.get("language.Base.Messages.RegisterTouchWindowFailMessage",_T("Failed to register touch input window. This means that:\n\n1. StrokesPlus doesn't have UI Access (not the signed version, running from under Program Files)\n2. There's another program running with has already registered as the touch input window\n\nEither way, touch input will be not be available.")).c_str(), ptLang.get("language.Base.Messages.RegisterTouchWindowFailTitle",_T("Can't Touch This")).c_str(), MB_ICONEXCLAMATION);
		}
	}
	if(bPenEnabled){
		if(lpfnRegisterTouchInput(hWnd, PT_PEN)==0){
			MessageBox(GetForegroundWindow(),ptLang.get("language.Base.Messages.RegisterPenWindowFailMessage",_T("Failed to register pen input window. This means that:\n\n1. StrokesPlus doesn't have UI Access (not the signed version, running from under Program Files)\n2. There's another program running with has already registered as the pen input window\n\nEither way, pen input will be not be available.")).c_str(), ptLang.get("language.Base.Messages.RegisterPenWindowFailTitle",_T("Does anyone know where my pen is?")).c_str(), MB_ICONEXCLAMATION);
			bPenEnabled = false;
		}
	}
}

__declspec(dllexport) void DestroyTouchWindow(HWND hWnd){
	if(bTouchEnabled){
		lpfnUnregisterTouchInput(hWnd, PT_TOUCH);
	}
	if(bPenEnabled){
		lpfnUnregisterTouchInput(hWnd, PT_PEN);
	}
}
#endif

#pragma endregion Touch & Pen Functions


#pragma region Synaptic

#ifdef SP_SYNAPTIC
__declspec(dllexport) void DestroySynaptics(){
		LOGMESSAGE("Synatpics Destroyed");
		if(mSynaptic != NULL) {
			LOGMESSAGE("mSynaptic != NULL");
			mSynaptic->~Synaptic();
			mSynaptic = NULL;
		}
}

Synaptic::Synaptic(){}

Synaptic::~Synaptic(){}

void Synaptic::Init(){
	LOGMESSAGE("TouchPad: Init()");
	long devHandle = -1;
    m_API->Initialize();
    m_API->FindDevice(SE_ConnectionAny, SE_DeviceTouchPad, &devHandle);

    if (devHandle >= 0) {
		SelectDevice(devHandle); 
		#ifdef SP_DEBUG_MODE
  			TCHAR buffer[256];
			wsprintf(buffer, _T("TouchPad: Init() - devHandle = %d"), devHandle);
			LOGMESSAGE(buffer);
		#endif
    } else {
		LOGMESSAGE("TouchPad: Init() - devHandle < 0");
	}
}

void Synaptic::SelectDevice(DWORD hDevice)
{
	m_Device->Select(hDevice);
	HRESULT hTmp = m_Device->SetSynchronousNotification(this);
	//check for failure here SYN-OK, SYN_EFAIL, SYNE_ACCESSDENIED
}

long Synaptic::OnSynDevicePacket(long lSeq) 
{
	LOGMESSAGE("TouchPad: LoadPacket");
	m_Device->LoadPacket(m_Packet);

	if(bGesturesDisabled || bGlobalUserCancel){
		LOGMESSAGE("TouchPad: Gestures Disabled");
		return 0;
	}
	
    long nof, fstate, xd, yd;
	
    // extract relevant data
    m_Packet->GetProperty(SP_ExtraFingerState, &nof);
	#ifdef SP_DEBUG_MODE
  		TCHAR buffer[256];
		wsprintf(buffer, _T("TouchPad: SP_ExtraFingerState: %d"), nof);
		LOGMESSAGE(buffer);
	#endif
    nof &= 3; //AND 3. Technically, newer TouchPads support more than 3 buttons, but it seems this should be fine for now
    m_Packet->GetProperty(SP_FingerState, &fstate);
    m_Packet->GetProperty(SP_XDelta, &xd);
    m_Packet->GetProperty(SP_YDelta, &yd);	

	if(bCancelTouchPad){
		if(bDrawGesture){
			hdc = GetDC(hWndServer);			
			SelectObject(hdc ,hBackgroundPen);								//Set hdc pen
			MoveToEx(hdc,lastx-rcWindow.left,lasty-rcWindow.top,NULL);	//Draw the line from the last pair of coordiates to current
			LineTo(hdc,lastx-rcWindow.left,lasty-rcWindow.top);	
			ReleaseDC(hWndServer,hdc);
		}
		bCancelTouchPad = false;
		bCancel = false;
		LOGMESSAGE("TouchPad: Unacquire (cancel)");
		//RESET CURSOR
		m_Device.m_pInternal->Unacquire();
	}

	#ifdef SP_DEBUG_MODE
  		TCHAR buffer2[256];
		wsprintf(buffer2, _T("TouchPad: bMouseDown: %d, bTouchPadDown: %d, fstate: %d, nof: %d"), bMouseDown, bTouchPadDown, fstate, nof);
		LOGMESSAGE(buffer2);
	#endif

	LOGMESSAGE("TouchPad: Checking FingerState & FingersPresent");

	if(nof > 0 && nof != iTouchPadFingers && bTouchPadDown && bMouseDown){
		long lTempTouchStamp = 0;
		m_Packet->GetProperty(SP_TimeStamp, &lTempTouchStamp);
		if((lTempTouchStamp - lTouchPadStamp) > 150 /*&& !(fstate & SF_FingerMotion)*/){
			if(bDrawGesture){
				hdc = GetDC(hWndServer);			
				SelectObject(hdc ,hBackgroundPen);								//Set hdc pen
				MoveToEx(hdc,lastx-rcWindow.left,lasty-rcWindow.top,NULL);	//Draw the line from the last pair of coordiates to current
				LineTo(hdc,lastx-rcWindow.left,lasty-rcWindow.top);	
				ReleaseDC(hWndServer,hdc);
			}
			//Proper number of fingers no longer down, abort - but only if 
			KillTimer(NULL,canceltimerid); //stop the cancel delay timer
			ClearGesture((LPVOID)0); //retraces the captured points with the color key for the gesture draw window, effectively erasing the line
			LOGMESSAGE("TouchPad: Fingers present no longer meet S+ setting, abort capture.");
			bCancel = TRUE; //Tell hook to ignore the rightdown event
			bMouseDown = false;
			clearCaptureVars();
			setWindowState();
			bCancel = FALSE; //Reset cancel flag
			bSynapticGesture = false;
			bTouchPadDown = false;
			lTouchPadFirstContactStamp = -1;
			//RESET CURSOR
			LOGMESSAGE("TouchPad: Unacquire (finger count changed)");
			m_Device.m_pInternal->Unacquire();
		} else {
			LOGMESSAGE("TouchPad: Fingers present no longer meet S+ setting, but 150ms hasn't passed.");
		}
	}

	if((fstate & SF_FingerPresent))
	{
		if(bIgnoreGesture){
			bIgnoreTouchpadSequence = true;
		}

		if(bIgnoreTouchpadSequence){
			LOGMESSAGE("TouchPad: Ignore");
			return 0;
		}

		long currentTimestamp = 0;
		if(iTouchPadFingers==1 && nof==1 && !bTouchPadDown && !bMouseDown)
		{
			if(lTouchPadFirstContactStamp==0){
				m_Packet->GetProperty(SP_TimeStamp,&lTouchPadFirstContactStamp);
			}
			
			if(xd > iTouchPadHoldThresholdDelta || xd < (0-iTouchPadHoldThresholdDelta) || yd > iTouchPadHoldThresholdDelta || yd < (0-iTouchPadHoldThresholdDelta)){
				lTouchPadFirstContactStamp = -1;
			} else {
				m_Packet->GetProperty(SP_TimeStamp,&currentTimestamp);
			}
		}

		if(((iTouchPadFingers > 1 && nof == iTouchPadFingers) || (iTouchPadFingers==1 && currentTimestamp > (lTouchPadFirstContactStamp + iTouchPadHoldDelay) && lTouchPadFirstContactStamp > 0)) && !bMouseDown && !bTouchPadDown)
		{
			LOGMESSAGE("TouchPad: Capture");
			
			#ifdef SP_DEBUG_MODE
				wsprintf(buffer, "TouchPad: x: %.4d, y: %.4d, z: %.4d, dx: %.4d, dy: %.4d SP_FingerState: %d SP_ExtendedState: %d SP_ExtraFingerState: %d\n",  
				m_Packet.GetLongProperty(SP_XRaw), m_Packet.GetLongProperty(SP_YRaw), m_Packet.GetLongProperty(SP_ZRaw),m_Packet.GetLongProperty(SP_XMickeys), m_Packet.GetLongProperty(SP_YMickeys),
				m_Packet.GetLongProperty(SP_FingerState),m_Packet.GetLongProperty(SP_ExtendedState),m_Packet.GetLongProperty(SP_ExtraFingerState) << m_Packet.GetLongProperty(SP_ExtraFingerState));
				LOGMESSAGE(buffer);
			#endif
		
			POINT screenpoint;
			GetCursorPos (&screenpoint); 

			//If the hook button was pressed over a S+ dialog other than the gesture draw window, call next hook
			if(GetAncestor(WindowFromPoint(screenpoint),GA_ROOTOWNER) == hWndServer && WindowFromPoint(screenpoint) != hWndServer){
				LOGMESSAGE("TouchPad: S+ Window");
				return 0;	
			}

			//If the hook button has been pressed on an ignored window, call the next hook
			if(bOnlyAllowDefinedApps){
				if(GetActionScript(WindowFromPoint(screenpoint), false
					#ifdef SP_NOGLOBALACTIONS
						, true
					#endif
						).compare(_T("APPDEFINED")) != 0 && bLearningMode != TRUE){
					LOGMESSAGE("TouchPad: Not defined app");
					 return 0;	
				}
			} else {
				if(GetActionScript(WindowFromPoint(screenpoint), TRUE
					#ifdef SP_NOGLOBALACTIONS
						, false
					#endif
						).compare(_T("IGNORED")) == 0 && bLearningMode != TRUE){
					LOGMESSAGE("TouchPad: Ignored window");
					 return 0;
				}
			}

			//If the a cancel hasn't been issue (disable next gesture) and the mouse 
			//isn't already down (specifically, the hook button), init the beginning of the gesture
			if(bCancel == FALSE && bMouseDown == FALSE)
			{
				LOGMESSAGE("TouchPad: Begin gesture capture");
				bTouchPadDown = true;
				bMouseDown = TRUE;
				//The code block below stores the modifiers that were active on stroke button down, if that option is enabled
				if(bCaptureModifiersOnMouseDown) {
					if((GetKeyState(VK_CONTROL) & 0x8000) != 0){
						bControlDownBefore = TRUE;
					}
					if((GetKeyState(VK_MENU) & 0x8000) != 0){
						bAltDownBefore = TRUE;
					}
					if((GetKeyState(VK_SHIFT) & 0x8000) != 0){
						bShiftDownBefore = TRUE;
					}
					if((GetKeyState(VK_LBUTTON) & 0x8000) != 0){
						bLeftMouseDownBefore = TRUE;
					}
					if((GetKeyState(VK_MBUTTON) & 0x8000) != 0){
						bMiddleMouseDownBefore = TRUE;
					}
					if((GetKeyState(VK_RBUTTON) & 0x8000) != 0){
						bRightMouseDownBefore = TRUE;
					}		
					if((GetKeyState(VK_XBUTTON1) & 0x8000) != 0){
						bX1MouseDownBefore = TRUE;
					}
					if((GetKeyState(VK_XBUTTON2) & 0x8000) != 0){
						bX2MouseDownBefore = TRUE;
					}
				}

				LOGMESSAGE("TouchPad: Acquire");
				m_Device.m_pInternal->Acquire(0);

				//Store the starting point of the gesture and fire the cancel delay timer
				POINTF tmpPointF;
				tmpPointF.x = (float)screenpoint.x; 
				tmpPointF.y = (float)screenpoint.y; 
				DrawPoints.push_back(tmpPointF);
				lastx=screenpoint.x; 
				lasty=screenpoint.y; 
				startx = screenpoint.x; 
				starty = screenpoint.y; 
				canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);
				bSynapticGesture = true;
				setWindowState();
				if(iTouchPadFingers==1 && bDrawGesture){
					hdc = GetDC(hWndServer);			
					SelectObject(hdc ,hDrawPen);								//Set hdc pen
					MoveToEx(hdc,lastx-rcWindow.left,lasty-rcWindow.top,NULL);	//Draw the line from the last pair of coordiates to current
					LineTo(hdc,lastx-rcWindow.left,lasty-rcWindow.top);	
					ReleaseDC(hWndServer,hdc);
				}
				return 0;
			}
		}

		if(nof == iTouchPadFingers && bMouseDown == TRUE && bCancel == FALSE && bTouchPadDown)
		{
			m_Packet->GetProperty(SP_TimeStamp, &lTouchPadStamp); //set last packet timestamp, for comparison above in # of fingers changed block
			//Capture touchpad mouse button modifiers. The ForceMotion does pass along ButtonState, but if the touchpad is stationary or doesn't have a high enough delta
			//the ForceMotion doesn't get called at the right time while the Touchpad button is down.
			if(m_Packet.IsLeftButton()){
				bLeftMouseDown = TRUE;
				LOGMESSAGE("TouchPad: Left Button Down");
				if(bResetCancelDelayOnMovement){
					KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
					canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when modifier is pressed
				}
			} else if(m_Packet.IsRightButton()){
				bRightMouseDown = TRUE;
				LOGMESSAGE("TouchPad: Right Button Down");
				if(bResetCancelDelayOnMovement){
					KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
					canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when modifier is pressed
				}
			} else if(m_Packet.IsMiddleButton()){
				bMiddleMouseDown = TRUE;
				LOGMESSAGE("TouchPad: Middle Button Down");
				if(bResetCancelDelayOnMovement){
					KillTimer(NULL,canceltimerid); //Kill timer so it doesn't fire (Cancel Delay)
					canceltimerid = SetTimer(NULL, WM_IDT_RELEASEMSG, CancelDelay, (TIMERPROC) TimerProc);  //Set timer, basically we're just restarting the timer when a modifier is pressed
				}
			}

			if((fstate & SF_FingerMotion) && (xd > iTouchPadThresholdDelta || xd < (0-iTouchPadThresholdDelta) || yd > iTouchPadThresholdDelta || yd < (0-iTouchPadThresholdDelta)))
			{
				#ifdef SP_DEBUG_MODE
					wsprintf(buffer, "TouchPad: Move Mouse x: %.4d, y: %.4d, z: %.4d, dx: %.4d, dy: %.4d SP_FingerState: %d SP_ExtendedState: %d SP_ExtraFingerState: %d\n",  
					m_Packet.GetLongProperty(SP_XRaw), m_Packet.GetLongProperty(SP_YRaw), m_Packet.GetLongProperty(SP_ZRaw),m_Packet.GetLongProperty(SP_XMickeys), m_Packet.GetLongProperty(SP_YMickeys),
					m_Packet.GetLongProperty(SP_FingerState),m_Packet.GetLongProperty(SP_ExtendedState),m_Packet.GetLongProperty(SP_ExtraFingerState) << m_Packet.GetLongProperty(SP_ExtraFingerState));
					LOGMESSAGE(buffer);
				#endif
				long tx = 1;
				long ty = 1;
				tx = (xd*((float)iTouchPadSensitivity/100));
				ty = (yd*((float)iTouchPadSensitivity/100));
				m_Device.m_pInternal->ForceMotion(((tx < 1 && tx > 0) ? 1 : ((tx > -1 && tx < 0) ? -1 : tx)),((ty < 1 && ty > 0) ? 1 : ((ty > -1 && ty < 0) ? -1 : ty)),m_Packet.ButtonState());
			}

		}

	} else {

		if(bTouchPadDown && nof==0)
		{
			bTouchPadDown = false;
			LOGMESSAGE("TouchPad: Release");
			//The stroke button was down (S+ was capturing a gesture), so begin the match and execute process
			if(bMouseDown == TRUE)
			{
				bMouseDown = FALSE;
				KillTimer(NULL,canceltimerid); //stop the cancel delay timer

				//bCancel is set by acDisableNext (gesture), on mouse up of the stroke button, this code tells S+ to start watching for stroke button down again
				if(bCancel){
					bCancel = FALSE;
					PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
				}

				ClearGesture((LPVOID)0); //retraces the captured points with the color key for the gesture draw window, effectively erasing the line
				POINTF start = {(float)startx, (float)starty};
			
				bSynapticGesture = false;

				if(bDrawGesture){
					hdc = GetDC(hWndServer);			
					SelectObject(hdc ,hBackgroundPen);								//Set hdc pen
					MoveToEx(hdc,lastx-rcWindow.left,lasty-rcWindow.top,NULL);	//Draw the line from the last pair of coordiates to current
					LineTo(hdc,lastx-rcWindow.left,lasty-rcWindow.top);	
					ReleaseDC(hWndServer,hdc);
				}

				if(DrawPoints.size() == 1
					&& bMouseWheelUp == FALSE && bMouseWheelDown == FALSE && bLeftMouseDown == FALSE && bMiddleMouseDown == FALSE && bRightMouseDown == FALSE && bX1MouseDown == FALSE && bX2MouseDown == FALSE
					&& bControlDown == FALSE && bAltDown == FALSE && bShiftDown == FALSE && bLeftMouseDownBefore == FALSE && bMiddleMouseDownBefore == FALSE && bRightMouseDownBefore == FALSE && bX1MouseDownBefore == FALSE && bX2MouseDownBefore == FALSE
					&& bControlDownBefore == FALSE && bAltDownBefore == FALSE && bShiftDownBefore == FALSE)
				{
					LOGMESSAGE("TouchPad: No draw points or modifiers");
					bCancel = TRUE; //Tell hook to ignore the rightdown event
					clearCaptureVars();
					setWindowState();
					bCancel = FALSE; //Reset cancel flag
				}
				else
				{
					//If S+ isn't in a state where only mouse wheel events are firing (code towards top of this function), go ahead and fire
					if(!bMouseWheelFiring){
						LOGMESSAGE("TouchPad: Mouse Wheel Not Firing");
						LOGMESSAGE("TouchPad: Fire Gesture Complete");
						//Fire action here
						HANDLE hThread = CreateThread(NULL, 0, &gGestureComplete, NULL, 0, NULL); 
						CloseHandle(hThread);
					} else {
						LOGMESSAGE("TouchPad: Mouse Wheel Is Firing");
						LOGMESSAGE("TouchPad: Fire Gesture Release");
						//If Allow Release Script is on, then fire the function to execute sp_after_release
						#ifdef SP_AFTERRELEASESCRIPT
							if(bFireOnRelease){
								HANDLE hThread = CreateThread(NULL, 0, &gGestureRelease, NULL, 0, NULL); 
								CloseHandle(hThread);
							}
						#endif
						bMouseWheelFiring = FALSE;
						clearCaptureVars();
						setWindowState();
					}
				}
				LOGMESSAGE("TouchPad: Unacquire");
				m_Device.m_pInternal->Unacquire();
			}
		}
		if(nof==0){
			lTouchPadFirstContactStamp = 0;
		}
		if(bIgnoreTouchpadSequence){
			LOGMESSAGE("TouchPad: Clear Ignore Gesture Flag");
			bIgnoreGesture = FALSE;
			bIgnoreTouchpadSequence = false;
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
		}
	}
		
	return 0;
}
#endif

#pragma endregion Synaptic


#pragma region Multimon

void ClipOrCenterRectToMonitor(LPRECT prc, BOOL bUseWorkArea, BOOL bClip)
{
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
    int         w = prc->right  - prc->left;
    int         h = prc->bottom - prc->top;

    // 
    // get the nearest monitor to the passed rect. 
    // 
    hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

    // 
    // get the work area or entire monitor rect. 
    // 
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (bUseWorkArea)
        rc = mi.rcWork;
    else
        rc = mi.rcMonitor;

    // 
    // center or clip the passed rect to the monitor rect 
    // 
    if (!bClip) //then center on screen
    {
        prc->left   = rc.left + (rc.right  - rc.left - w) / 2;
        prc->top    = rc.top  + (rc.bottom - rc.top  - h) / 2;
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
    else //otherwise clip to fit
    {
        prc->left   = max(rc.left, min(rc.right-w,  prc->left));
        prc->top    = max(rc.top,  min(rc.bottom-h, prc->top));
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
}

void ClipOrCenterWindowToMonitor(HWND hwnd, BOOL bUseWorkArea, BOOL bClip)
{
    RECT rc;

	GetWindowRect(hwnd, &rc);
    ClipOrCenterRectToMonitor(&rc, bUseWorkArea, bClip);
	SetWindowPos(hwnd, NULL, rc.left, rc.top, rc.right, rc.left, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE); 
}

void FitWindowToMonitor(HWND hWndTarget, BOOL bUseWorkArea){
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
	RECT		rcw;

	GetWindowRect(hWndTarget, &rc);	

    // 
    // get the nearest monitor to the passed rect. 
    // 
    hMonitor = MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);

    // 
    // get the work area or entire monitor rect. 
    // 
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (bUseWorkArea)
        rcw = mi.rcWork;
    else
        rcw = mi.rcMonitor;

	SetWindowPos(hWndTarget, NULL, rcw.left, rcw.top, (rcw.left < 0 ? rcw.right-rcWindow.left : rcw.right-rcw.left), (rcw.top < 0 ? rcw.bottom-rcWindow.top : rcw.bottom-rcw.top), SWP_NOZORDER);
}

void FitWindowToDesktop(HWND hWndTarget){
   
	SetWindowPos(hWndTarget, NULL,rcWindow.left, rcWindow.top, rcWindow.right-rcWindow.left, rcWindow.bottom-rcWindow.top, SWP_NOZORDER);
}

#pragma endregion Multimon


#pragma region Config Save / Load

void changePassword(string sNewPassword){
	string filenamein = strStrokesConfigFile;
	string filenameout = strStrokesConfigFile;
	filenameout.append(".dec");
	ifstream in(filenamein.c_str(), ios::binary);
	ofstream out(filenameout.c_str(), ios::binary);
	char ch, y;
	if(in.is_open())
	{
		for(int z = 0; in.read( (char*)&ch, sizeof(ch) ) ; z++)
		{
			if(z == sPassword.length())
			{
				z = 0;
			}
			y = ch^sPassword[z];
			out.write (  (char*)&y, sizeof(y) );
		}
	}
	in.close();
	out.close();

	remove(filenamein.c_str());
	rename(filenameout.c_str(),filenamein.c_str());

	if(sNewPassword.length() > 0) {
		filenamein = strStrokesConfigFile;
		filenameout = strStrokesConfigFile;
		filenameout.append(".enc");
		ifstream in(filenamein.c_str(), ios::binary);
		ofstream out(filenameout.c_str(), ios::binary);
		char ch, y;
		if(in.is_open())
		{
			for(int z = 0; in.read( (char*)&ch, sizeof(ch) ) ; z++)
			{
				if(z == sNewPassword.length())
				{
					z = 0;
				}
				y = ch^sNewPassword[z];
				out.write (  (char*)&y, sizeof(y) );
			}
		}
		in.close();
		out.close();

		remove(filenamein.c_str());
		rename(filenameout.c_str(),filenamein.c_str());
	}

	sPassword = sNewPassword;
}

__declspec(dllexport) void saveConfig()
{
	LOGMESSAGE("Enter saveConfig ");
	ptConfig.put("config.settings.DrawOpacity", opacity);
	ptConfig.put("config.settings.ActionsWidth", iConfigWidth);
	ptConfig.put("config.settings.ActionsHeight", iConfigHeight);
	ptConfig.put("config.settings.GesturesDisabled",(int)bGesturesDisabled);
	ptConfig.put("config.settings.MouseButton",(int)getHookBtn());
	ptConfig.put("config.settings.IgnoreKey",(int)getIgnoreKey());
	ptConfig.put("config.settings.CancelDelay",(int)getCancelDelay());
	ptConfig.put("config.settings.DrawGestures",(int)getDrawGesture());
	ptConfig.put("config.settings.DrawColorR",(int)getDrawColorR());
	ptConfig.put("config.settings.DrawColorG",(int)getDrawColorG());
	ptConfig.put("config.settings.DrawColorB",(int)getDrawColorB());
	ptConfig.put("config.settings.DrawWidth", getDrawWidth());
	ptConfig.put("config.settings.AggressivelyTrimMemory", (int)bAggressiveTrim);
	ptConfig.put("config.settings.CheckForegroundTimeout", (int)bCheckForegroundTimeout);
	ptConfig.put("config.settings.CheckForOtherGesturePrograms", (int)bCheckForOtherGesturePrograms);
	ptConfig.put("config.settings.MaxDrawPoints", (int)getMaxDrawPoints());
	ptConfig.put("config.settings.MatchProbabilityThreshold", (int)getMatchProbabilityThreshold());
	ptConfig.put("config.settings.MatchPrecision", (int)getMatchPrecision());
	ptConfig.put("config.settings.Autosave", (int)bAutosave);
	ptConfig.put("config.settings.ReInitOnResume",(int)bReInitOnResume);
	ptConfig.put("config.settings.DrawWindowAlwaysOnTop",(int)bDrawWindowAlwaysOnTop);
	ptConfig.put("config.settings.ResetCancelDelayOnMovement",(int)bResetCancelDelayOnMovement);
	ptConfig.put("config.settings.TrayIconVisible",(int)bTrayIconVisible);
	ptConfig.put("config.settings.FireOnMouseWheel",(int)bFireOnMouseWheel);
	ptConfig.put("config.settings.CaptureModifiersOnMouseDown",(int)bCaptureModifiersOnMouseDown);
	ptConfig.put("config.settings.NoMatchSound", (int)bNoMatchSound);
	ptConfig.put("config.settings.NoMatchSoundFile", sNoMatchSoundWAV.c_str());
	ptConfig.put("config.settings.MinSegmentLength", iMinSegmentLength);
	ptConfig.put("config.settings.GestureWindowToBottom", (int)bDrawWindowToBottom);
	ptConfig.put("config.settings.AllowAfterActionScript", (int)bAllowAfterScript);
	ptConfig.put("config.settings.CheckCursorFlags", (int)bCheckCursorFlags);
	ptConfig.put("config.settings.MouseWheelFocus",(int)bMouseWheelFocus);
	ptConfig.put("config.settings.SettingsMaximized",(int)bSettingsMaximized);
	ptConfig.put("config.settings.ActionLuaZoomFactor",(int)iActionLuaZoomFactor);
	ptConfig.put("config.settings.HotkeyLuaZoomFactor",(int)iHotkeyLuaZoomFactor);
	ptConfig.put("config.settings.GlobalLuaZoomFactor",(int)iGlobalLuaZoomFactor);
	#ifdef SP_AFTERRELEASESCRIPT
		ptConfig.put("config.settings.AllowReleaseScript",(int)bFireOnRelease);
	#endif
	#ifdef SP_SYNAPTIC
		ptConfig.put("config.settings.SynapticFingers",(int)iTouchPadFingers);
		ptConfig.put("config.settings.DisableSynaptics",(int)bDisableSynaptics);
		ptConfig.put("config.settings.SynapticsMovementSensitivity",(int)iTouchPadSensitivity);
		ptConfig.put("config.settings.TouchPadThresholdDelta",(int)iTouchPadThresholdDelta);
		ptConfig.put("config.settings.TouchPadHoldThresholdDelta",(int)iTouchPadHoldThresholdDelta);
		ptConfig.put("config.settings.TouchPadHoldDelay",(int)iTouchPadHoldDelay);
		ptConfig.put("config.settings.UseTouchCustomCursor",(int)bUseTouchCursor);
		ptConfig.put("config.settings.TouchCustomCursor",sTouchCursor.c_str());
	#endif
	ptConfig.put("config.settings.ResumeDelay",(int)iResumeDelay);
	ptConfig.put("config.settings.HideNewGestureMessage",(int)bHideNewGestureMessage);
	ptConfig.put("config.settings.HideAdditionalInstanceMessage",(int)bHideAdditionalInstanceMessage);
	#ifdef SP_NOGLOBALACTIONS
		ptConfig.put("config.settings.OnlyAllowDefinedApps",(int)bOnlyAllowDefinedApps);
	#endif
	#ifdef SP_MOUSE_CLICK_ACTION
		ptConfig.put("config.settings.AllowRightClickScript",(int)bFireOnRightClick);
		ptConfig.put("config.settings.AllowMiddleClickScript",(int)bFireOnMiddleClick);
		ptConfig.put("config.settings.AllowLeftClickScript",(int)bFireOnLeftClick);
		ptConfig.put("config.settings.AllowX1ClickScript",(int)bFireOnX1Click);
		ptConfig.put("config.settings.AllowX2ClickScript",(int)bFireOnX2Click);
	#endif
	#ifdef SP_WHEEL_HOOK
		ptConfig.put("config.settings.AllowWheelTickScript",(int)bMouseWheelTick);
		ptConfig.put("config.settings.AllowHorzWheelTickScript",(int)bHorzMouseWheelTick);
	#endif
	ptConfig.put("config.settings.ShowCopyData",(int)bShowCopyData);
	ptConfig.put("config.settings.RelayGestureOnNoMatch",(int)bRelayGesture);
			
#ifdef SP_TOUCH
	ptConfig.put("config.settings.EnableTouch",(int)bTouchEnabled);
	ptConfig.put("config.settings.EnablePen",(int)bPenEnabled);
#endif

	SetCurrentDirectory(strExePath);
	fstream filestr;
	filestr.open(strStrokesConfigFile, fstream::out);
	try{
		write_xml(filestr, ptConfig);
	}
	catch (std::exception& e) {
		filestr.close();
		MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.SaveConfigFail",_T("Failed to write StrokesPlus.xml. If this perists, your config file may be corrupt or you may not have permissions to update the file; deleting StrokesPlus.xml will create a new config file, but will have no gestures or actions defined.\n\nSee this post to specify a custom location for StrokesPlus.xml:\n\nhttp://www.strokesplus.com/customconfig")).c_str(), ptLang.get("language.Base.Messages.SaveConfigFailTitle",_T("No Save For You!")).c_str(), MB_ICONERROR);
		return;
	}
	filestr.close();

	if(sPassword.length()>0){
		string filenamein = strStrokesConfigFile;
		string filenameout = strStrokesConfigFile;
		filenameout.append(".enc");
		ifstream in(filenamein.c_str(), ios::binary);
		ofstream out(filenameout.c_str(), ios::binary);
		char ch, y;
		if(in.is_open())
		{
			for(int z = 0; in.read( (char*)&ch, sizeof(ch) ) ; z++)
			{
				if(z == sPassword.length())
				{
					z = 0;
				}
				y = ch^sPassword[z];
				out.write (  (char*)&y, sizeof(y) );
			}
		}
		in.close();
		out.close();

		remove(filenamein.c_str());
		rename(filenameout.c_str(),filenamein.c_str());
	}
}

__declspec(dllexport) BOOL loadHookConfig()
{
	LOGMESSAGE("In loadHookConfig");
	// Init config file
	bGlobalCancel = TRUE;
	SetCurrentDirectoryA(strExePath);
	
	fstream filestr;
	string sLangPath = strStrokesConfigFile;
	string sActionHelpPath = strStrokesConfigFile;
	boost::replace_all(sLangPath, _T("StrokesPlus.xml"), _T("Language.xml"));
	boost::replace_all(sActionHelpPath, _T("StrokesPlus.xml"), _T("ActionHelp.xml"));
	LOGMESSAGE("About to open lang file");
	filestr.open(sLangPath, fstream::in | fstream::out);
	if(filestr){
		try{
			read_xml(filestr, ptLang);
		}
		catch (std::exception& e) {
			MessageBox(GetForegroundWindow(), _T("Failed to load/parse Language.xml. \n\nThis isn't a fatal error, but the program text may be missing or incorrect."), _T("¿Que?"), MB_ICONERROR);
		}
	}
	filestr.close();
	
	LOGMESSAGE("About to open config file");
	filestr.open(strStrokesConfigFile, fstream::in | fstream::out);
	if(!filestr){
		filestr.close();
		ptConfig.put("config.settings",_T(""));
		ptConfig.put("config.Gestures",_T(""));
		ptConfig.put("config.Ignored",_T(""));
		ptConfig.put("config.Global.Actions",_T(""));
		ptConfig.put("config.Global.Hotkeys",_T(""));
		ptConfig.put("config.Applications",_T(""));
		//MessageBox(GetForegroundWindow(),"in the create config check, about to call saveConfig","",NULL);
		saveConfig();
		filestr.open(strStrokesConfigFile, fstream::in | fstream::out);
		if(!filestr){
			return false;
		}
	}

	LOGMESSAGE("About to see if we need to prompt for password");
	string filenameout;
	if(sPassword.length()>0){
		filestr.close();
		string filenamein = strStrokesConfigFile;
		filenameout = strStrokesConfigFile;
		filenameout.append(".dec");
		ifstream in(filenamein.c_str(), ios::binary);
		ofstream out(filenameout.c_str(), ios::binary);
		char ch, y;
		if(in.is_open())
		{
			for(int z = 0; in.read( (char*)&ch, sizeof(ch) ) ; z++)
			{
				if(z == sPassword.length())
				{
					z = 0;
				}
				y = ch^sPassword[z];
				out.write (  (char*)&y, sizeof(y) );
			}
		}
		in.close();
		out.close();

		filestr.open(filenameout, fstream::in | fstream::out);
	}

	LOGMESSAGE("About to call read_xml for main config");
	if(filestr) {
		try{
			read_xml(filestr, ptConfig);
		}
		catch (std::exception& e) {
			filestr.close();
			if(!bTriedDecrypt){
				bTriedDecrypt = true;
				bCheckPassword = false;
				bSetPassword = false;
				SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGPASSWORD), hWndServer, PasswordProc);
				setWindowState();
				return loadHookConfig();
			}
			remove(filenameout.c_str());
			MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.LoadConfigFail",_T("Failed to load/parse StrokesPlus.xml. If this perists, your config file may be corrupt; deleting StrokesPlus.xml will create a new config file, but will have no gestures or actions defined.\n\nExiting StrokesPlus.")).c_str(), ptLang.get("language.Base.Messages.LoadConfigFailTitle",_T("Shit, this isn't good...")).c_str(), MB_ICONERROR);
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_EXIT_NOSAVE, 1), 0);
			return FALSE;
		}
	}
	filestr.close();

	LOGMESSAGE("About to delete copied file (when password exists)");
	if(sPassword.length()>0){
		remove(filenameout.c_str());
	}
	
	LOGMESSAGE("About to load all settings");
	//MessageBox(GetForegroundWindow(),"Before popuplate vars",strStrokesConfigFile,NULL);
	bGlobalCancel = FALSE;
	//ptConfig.get_child("config");
	bCheckForOtherGesturePrograms = (bool)ptConfig.get("config.settings.CheckForOtherGesturePrograms", 1);
	opacity = ptConfig.get("config.settings.DrawOpacity", 150);
	bGesturesDisabled = (bool)ptConfig.get("config.settings.GesturesDisabled",0);
	setHookBtn((int)ptConfig.get("config.settings.MouseButton",0));
	setIgnoreKey((int)ptConfig.get("config.settings.IgnoreKey",1));
	setCancelDelay((int)ptConfig.get("config.settings.CancelDelay",250));
	setDrawGesture((bool)ptConfig.get("config.settings.DrawGestures",1));
	setDrawColor(RGB((BYTE)(int)ptConfig.get("config.settings.DrawColorR",56),(BYTE)(int)ptConfig.get("config.settings.DrawColorG",169),(BYTE)(int)ptConfig.get("config.settings.DrawColorB",255)));
	setDrawWidth(ptConfig.get("config.settings.DrawWidth",5));
	bAggressiveTrim = (bool)ptConfig.get("config.settings.AggressivelyTrimMemory",1);
	bReInitOnResume = (bool)ptConfig.get("config.settings.ReInitOnResume",0);
	bCheckForegroundTimeout = (bool)ptConfig.get("config.settings.CheckForegroundTimeout",0);
	setMaxDrawPoints((int)ptConfig.get("config.settings.MaxDrawPoints",0));
	setMatchProbabilityThreshold((int)ptConfig.get("config.settings.MatchProbabilityThreshold",75));
	setMatchPrecision((int)ptConfig.get("config.settings.MatchPrecision",100));
	setTrimDelay((int)ptConfig.get("config.settings.AggressivelyTrimMemoryInterval",1000));
	bHideNewGestureMessage = (bool)ptConfig.get("config.settings.HideNewGestureMessage",0);
	bHideAdditionalInstanceMessage = (bool)ptConfig.get("config.settings.HideAdditionalInstanceMessage",0);
	bAutosave = (bool)ptConfig.get("config.settings.Autosave",0);
	bTrayIconVisible = (bool)ptConfig.get("config.settings.TrayIconVisible",1);
	bDrawWindowAlwaysOnTop = (bool)ptConfig.get("config.settings.DrawWindowAlwaysOnTop",0);
	bResetCancelDelayOnMovement = (bool)ptConfig.get("config.settings.ResetCancelDelayOnMovement",1);
	bFireOnMouseWheel = (bool)ptConfig.get("config.settings.FireOnMouseWheel",0);
	bCaptureModifiersOnMouseDown = (bool)ptConfig.get("config.settings.CaptureModifiersOnMouseDown",0);
	iConfigHeight = ptConfig.get("config.settings.ActionsHeight",703);
	iConfigWidth = ptConfig.get("config.settings.ActionsWidth",1014);
	iMinSegmentLength = ptConfig.get("config.settings.MinSegmentLength",6);
	bDrawWindowToBottom = (bool)ptConfig.get("config.settings.GestureWindowToBottom",1);
	bAllowAfterScript = (bool)ptConfig.get("config.settings.AllowAfterActionScript",0);
	bMouseWheelFocus = (bool)ptConfig.get("config.settings.MouseWheelFocus",0);
	sNoMatchSoundWAV = ptConfig.get("config.settings.NoMatchSoundFile",_T(""));
	bNoMatchSound = (bool)ptConfig.get("config.settings.NoMatchSound",0);
	bSettingsMaximized = (bool)ptConfig.get("config.settings.SettingsMaximized",0);
	bCheckCursorFlags = (bool)ptConfig.get("config.settings.CheckCursorFlags",1);
	#ifdef SP_AFTERRELEASESCRIPT
 		bFireOnRelease = (bool)ptConfig.get("config.settings.AllowReleaseScript",0);
	#endif
	#ifdef SP_MOUSE_CLICK_ACTION
		bFireOnRightClick = (bool)ptConfig.get("config.settings.AllowRightClickScript",0);
 		bFireOnMiddleClick = (bool)ptConfig.get("config.settings.AllowMiddleClickScript",0);
		bFireOnLeftClick = (bool)ptConfig.get("config.settings.AllowLeftClickScript",0);
		bFireOnX1Click = (bool)ptConfig.get("config.settings.AllowX1ClickScript",0);
		bFireOnX2Click = (bool)ptConfig.get("config.settings.AllowX2ClickScript",0);
	#endif
	iActionLuaZoomFactor = (int)ptConfig.get("config.settings.ActionLuaZoomFactor",0);
	iHotkeyLuaZoomFactor = (int)ptConfig.get("config.settings.HotkeyLuaZoomFactor",0);
	iGlobalLuaZoomFactor = (int)ptConfig.get("config.settings.GlobalLuaZoomFactor",0);
	if(iActionLuaZoomFactor < -10) iActionLuaZoomFactor = -10;
	if(iActionLuaZoomFactor > 20) iActionLuaZoomFactor = 20;
	if(iHotkeyLuaZoomFactor < -10) iHotkeyLuaZoomFactor = -10;
	if(iHotkeyLuaZoomFactor > 20) iHotkeyLuaZoomFactor = 20;
	if(iGlobalLuaZoomFactor < -10) iGlobalLuaZoomFactor = -10;
	if(iGlobalLuaZoomFactor > 20) iGlobalLuaZoomFactor = 20;
	#ifdef SP_SYNAPTIC
		iTouchPadFingers = (int)ptConfig.get("config.settings.SynapticFingers",1);
		bDisableSynaptics = (bool)ptConfig.get("config.settings.DisableSynaptics",false);
		iTouchPadSensitivity = (int)ptConfig.get("config.settings.SynapticsMovementSensitivity",25);
		if(iTouchPadSensitivity < 0) iTouchPadSensitivity = 0;
		iTouchPadThresholdDelta = (int)ptConfig.get("config.settings.TouchPadThresholdDelta",1);
		if(iTouchPadThresholdDelta < 0) iTouchPadThresholdDelta = 0;
		iTouchPadHoldThresholdDelta = (int)ptConfig.get("config.settings.TouchPadHoldThresholdDelta",4);
		if(iTouchPadHoldThresholdDelta < 0) iTouchPadHoldThresholdDelta = 1;
		iTouchPadHoldDelay = (int)ptConfig.get("config.settings.TouchPadHoldDelay",500);
		if(iTouchPadHoldDelay < 50) iTouchPadHoldDelay = 50;
		bUseTouchCursor = (bool)ptConfig.get("config.settings.UseTouchCustomCursor",false);
		sTouchCursor = ptConfig.get("config.settings.TouchCustomCursor",_T(""));
	#endif
	iResumeDelay = (int)ptConfig.get("config.settings.ResumeDelay",5000);
	#ifdef SP_NOGLOBALACTIONS
		bOnlyAllowDefinedApps = (bool)ptConfig.get("config.settings.OnlyAllowDefinedApps",false);
	#endif
	#ifdef SP_WHEEL_HOOK
		bMouseWheelTick = (bool)ptConfig.get("config.settings.AllowWheelTickScript",false);
		bHorzMouseWheelTick = (bool)ptConfig.get("config.settings.AllowHorzWheelTickScript",false);
	#endif
	bShowCopyData = (bool)ptConfig.get("config.settings.ShowCopyData",0);
	bRelayGesture = (bool)ptConfig.get("config.settings.RelayGestureOnNoMatch",0);
	
#ifdef SP_TOUCH
	bTouchEnabled = (bool)ptConfig.get("config.settings.EnableTouch",false);
	bPenEnabled = (bool)ptConfig.get("config.settings.EnablePen",false);
#endif
	
	LOGMESSAGE("About to set draw window as topmost (if setting active)");
	if(bDrawWindowAlwaysOnTop){
		SetWindowPos(hWndServer, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}
	if(bAggressiveTrim) {
		SetProcessWorkingSetSize(GetCurrentProcess(), 0xffffffff, 0xffffffff);
		fireTrimTimer();
	}
	LOGMESSAGE("About call read_xml for action help");
	try{
		read_xml(sActionHelpPath, ptActions);
	}
	catch (std::exception& e) {
		MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.LoadActionsFail",_T("Failed to load/parse ActionHelp.xml. Try replacing the file with a copy from the portable StrokesPlus ZIP download.\n\nThis isn't a fatal error, but there will be no actions in the Available Actions drop down.")).c_str(), ptLang.get("language.Base.Messages.LoadActionsFailTitle",_T("Bah!")).c_str(), MB_ICONERROR);
	}

	if(ptConfig.get("config.settings.SawAdminModeMessage",0) == 0){
		MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.AdminModeMessage",_T("Beginning with version 2.2.1, StrokesPlus no longer requires Administrator privileges to run.\n\nFor most uses, this is acceptable. However, there are some unexpected behaviors that you may run into.\n\nFor example, S+ will not be able to control other programs running with elevated privileges, and there may be a couple actions which will not work due the underlying calls to Windows requiring Administrator privileges.\n\nIf you run the signed version of S+ from a protected folder (Program Files) in Windows 8, these issues can be avoided.")).c_str(),ptLang.get("language.Base.Messages.AdminModeTitle",_T("Admin")).c_str(),NULL);
		ptConfig.put("config.settings.SawAdminModeMessage",1);
		//saveConfig();
	}

	if(OSVersion==8){
		if(ptConfig.get("config.settings.SawWin8LauncherMessage",0) == 0){
			MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.Win8LauncherMessage",_T("Due to changes in Windows 8, S+ may not interact with certain Windows 8 screens unless you have UAC disabled or use the signed version of S+.\n\nSee this post for more details:\nhttp://www.strokesplus.com/windows8")).c_str(),ptLang.get("language.Base.Messages.Win8LauncherTitle",_T("Windows 8 Interoperability")).c_str(),NULL);
			ptConfig.put("config.settings.SawWin8LauncherMessage",1);
			//saveConfig();
		}
	}

	LOGMESSAGE("About to load hotkeys");
	BOOL bHotkeysFound = false;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &vt3, ptConfig.get_child("config.Global")){
		if(vt3.first == "Hotkeys"){
			bHotkeysFound = true;
		}
	}
	if(!bHotkeysFound){
		ptConfig.put("config.Global.Hotkeys",_T(""));
		//saveConfig();
	}
	
	if(getGesturesDisabled()==false){
		PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
	}

#ifdef SP_SYNAPTIC
	LOGMESSAGE("About to test and load for Synaptics");
	if(bDisableSynaptics)
	{
		DestroySynaptics();
	} 
	else 
	{
		LOGMESSAGE("Synatpics Enabled");
		if(mSynaptic == NULL) 
		{
			LOGMESSAGE("mSynaptic == NULL");
			CLSID ClassID;
			if(SUCCEEDED(CLSIDFromProgID(OLESTR("SynCtrl.SynAPICtrl"), &ClassID)))
			{
				LOGMESSAGE("SynCtrl.SynAPICtrl Found");
				//success, Synaptic software found
				mSynaptic = new Synaptic;
				mSynaptic->Init();
				bool bIsTouchPad = false;
				bool bIsMultiFinger = false;
				try {
					bIsTouchPad = mSynaptic->m_Device.IsTouchPad();
					bIsMultiFinger = (bool)mSynaptic->m_Device.GetLongProperty(SP_IsMultiFingerCapable);
				} catch (...) { 
					bIsTouchPad = false;
					bIsMultiFinger = false;	
				}
				LOGMESSAGE("After new Synaptic instantiation");
				if(!bIsTouchPad){
					MessageBox(GetForegroundWindow(), ptLang.get("language.Base.Messages.TouchPadNotSupportedMessage",_T("TouchPad not supported.\n\nEither it's not being reported as a TouchPad or an error was encountered while initializing the device.\n\nSynaptics TouchPad support will be disabled; you may enable support again in the Preferences tab.")).c_str(), ptLang.get("language.Base.Messages.TouchPadNotSupportedTitle",_T("TouchPad Not Supported")).c_str(), NULL);
					mSynaptic->~Synaptic();
					mSynaptic = NULL;
					bDisableSynaptics = true;
					saveConfig();
				}
				bTouchPadMultiFinger = bIsMultiFinger;
			} else {
				LOGMESSAGE("SynCtrl.SynAPICtrl Not Found - Disabling TouchPad");
				bDisableSynaptics = true;
				saveConfig();
			}
		}
	}
#endif

	return TRUE;
}

#pragma endregion Config Save / Load


#pragma region Find Window Code

BOOL CheckWindowValidity (HWND hwndDialog, HWND hwndToCheck)
{
// Synopsis :
// 1. This function checks a hwnd to see if it is actually the "Search Window" Dialog's or Main Window's
// own window or one of their children. If so a FALSE will be returned so that these windows will not
// be selected. 
//
// 2. Also, this routine checks to see if the hwnd to be checked is already a currently found window.
// If so, a FALSE will also be returned to avoid repetitions.
  HWND hwndTemp = NULL;
  BOOL bRet = TRUE;

  // The window must not be NULL.
  if (hwndToCheck == NULL)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }

  // It must also be a valid window as far as the OS is concerned.
  if (IsWindow(hwndToCheck) == FALSE)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }

  // Ensure that the window is not the current one which has already been found.
  if (hwndToCheck == g_hwndFoundWindow)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }

  // It must also not be the main window itself.
  if (hwndToCheck == hWndServer || hwndToCheck == hTabControlMain || hwndToCheck == hWndSettings)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }
  
  // It also must not be the "Search Window" dialog box itself.
  if (hwndToCheck == hwndDialog)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }

  // It also must not be one of the dialog box's children...
  hwndTemp = GetParent (hwndToCheck);
  if ((hwndTemp == hwndDialog) || (hwndTemp == hWndServer) || hwndTemp == hTabControlMain || hwndTemp == hWndSettings)
  {
    bRet = FALSE;
	goto CheckWindowValidity_0;
  }

CheckWindowValidity_0:

  return bRet;
}

long DoMouseMove(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
// Synopsis :
// 1. This is the handler for WM_MOUSEMOVE messages sent to the "Search Window" dialog proc.
//
// 2. Note that we do not handle every WM_MOUSEMOVE message sent. Instead, we check to see 
// if "g_bStartSearchWindow" is TRUE. This BOOL will be set to TRUE when the Window
// Searching Operation is actually started. See the WM_COMMAND message handler in 
// SearchWindowDialogProc() for more details.
//
// 3. Because the "Search Window" dialog immediately captures the mouse when the Search Operation 
// is started, all mouse movement is monitored by the "Search Window" dialog box. This is 
// regardless of whether the mouse is within or without the "Search Window" dialog. 
//
// 4. One important note is that the horizontal and vertical positions of the mouse cannot be 
// calculated from "lParam". These values can be inaccurate when the mouse is outside the
// dialog box. Instead, use the GetCursorPos() API to capture the position of the mouse.
  POINT		screenpoint;
  HWND		hwndFoundWindow = NULL;
  //char		szText[256];
  long		lRet = 0;

  // Must use GetCursorPos() instead of calculating from "lParam".
  GetCursorPos (&screenpoint);  

  // Display global positioning in the dialog box.
  //wsprintf (szText, "%d", screenpoint.x);
  //SetDlgItemText (hwndDialog, IDC_STATIC_X_POS, szText);
  
  //wsprintf (szText, "%d", screenpoint.y);
  //SetDlgItemText (hwndDialog, IDC_STATIC_Y_POS, szText);

  // Determine the window that lies underneath the mouse cursor.
  hwndFoundWindow = WindowFromPoint (screenpoint);

  // Check first for validity.
  if (CheckWindowValidity (hwndDialog, hwndFoundWindow))
  {
    // We have just found a new window.

    // Display some information on this found window.
	DisplayInfoOnFoundWindow (hwndDialog, hwndFoundWindow);

    // If there was a previously found window, we must instruct it to refresh itself. 
	// This is done to remove any highlighting effects drawn by us.
    if (g_hwndFoundWindow)
    {
      RefreshWindow (g_hwndFoundWindow);
    }

    // Indicate that this found window is now the current global found window.
    g_hwndFoundWindow = hwndFoundWindow;

    // We now highlight the found window.
    HighlightFoundWindow (hwndDialog, g_hwndFoundWindow);
  }

  return lRet;
}

long DoMouseUp(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
// Synopsis :
// 1. Handler for WM_LBUTTONUP message sent to the "Search Window" dialog box.
// 
// 2. We restore the screen cursor to the previous one.
//
// 3. We stop the window search operation and release the mouse capture.
  long lRet = 0;

  // If we had a previous cursor, set the screen cursor to the previous one.
  // The cursor is to stay exactly where it is currently located when the 
  // left mouse button is lifted.
  if (g_hCursorPrevious)
  {
    SetCursor (g_hCursorPrevious);
  }

  // If there was a found window, refresh it so that its highlighting is erased. 
  if (g_hwndFoundWindow)
  {
    RefreshWindow (g_hwndFoundWindow);
  }

  // Set the bitmap on the Finder Tool icon to be the bitmap with the bullseye bitmap.
  SetFinderToolImage (hwndDialog, TRUE);

  // Very important : must release the mouse capture.
  ReleaseCapture ();

  // Make the main window appear normally.
  //ShowWindow (hWnd, SW_SHOWNORMAL);

  // Set the global search window flag to FALSE.
  g_bStartSearchWindow = FALSE;

  return lRet;
}

BOOL SetFinderToolImage (HWND hwndDialog, BOOL bSet)
{
// Synopsis :
// 1. This routine sets the Finder Tool icon to contain an appropriate bitmap.
//
// 2. If bSet is TRUE, we display the BullsEye bitmap. Otherwise the empty window
// bitmap is displayed.
  HBITMAP hBmpToSet = NULL;
  BOOL bRet = TRUE;

  if (bSet)
  {
    // Set a FILLED image.
	hBmpToSet = g_hBitmapFinderToolFilled;
  }
  else
  {
    // Set an EMPTY image.
	hBmpToSet = g_hBitmapFinderToolEmpty;
  }

  SendDlgItemMessage
  (
    (HWND)hwndDialog, // handle of dialog box 
    (int)IDC_STATIC_ICON_FINDER_TOOL, // identifier of control 
    (UINT)STM_SETIMAGE, // message to send 
    (WPARAM)IMAGE_BITMAP, // first message parameter 
    (LPARAM)hBmpToSet // second message parameter 
  );

  return bRet;
}

BOOL MoveCursorPositionToBullsEye (HWND hwndDialog)
{
// Synopsis :
// 1. This routine moves the mouse cursor hotspot to the exact 
// centre position of the bullseye in the finder tool static control.
//
// 2. This function, when used together with DoSetFinderToolImage(),
// gives the illusion that the bullseye image has indeed been transformed
// into a cursor and can be moved away from the Finder Tool Static
// control.
  BOOL bRet = FALSE;
  HWND hwndToolFinder = NULL;
  RECT rect;
  POINT screenpoint;

  // Get the window handle of the Finder Tool static control.
  hwndToolFinder = GetDlgItem (hwndDialog, IDC_STATIC_ICON_FINDER_TOOL);

  if (hwndToolFinder)
  {
    // Get the screen coordinates of the static control,
	// add the appropriate pixel offsets to the center of 
	// the bullseye and move the mouse cursor to this exact
	// position.
    GetWindowRect (hwndToolFinder, &rect);
	screenpoint.x = rect.left + BULLSEYE_CENTER_X_OFFSET;
    screenpoint.y = rect.top + BULLSEYE_CENTER_Y_OFFSET;
	SetCursorPos (screenpoint.x, screenpoint.y);
  }

  return bRet;
}

long SearchWindow (HWND hwndDialog)
{
// Synopsis :
// 1. This function starts the window searching operation.
//
// 2. A very important part of this function is to capture 
// all mouse activities from now onwards and direct all mouse 
// messages to the "Search Window" dialog box procedure.
  long lRet = 0;

  // Set the global "g_bStartSearchWindow" flag to TRUE.
  g_bStartSearchWindow = TRUE;

  // Display the empty window bitmap image in the Finder Tool static control.
  SetFinderToolImage (hwndDialog, FALSE);

  MoveCursorPositionToBullsEye (hwndDialog);

  // Set the screen cursor to the BullsEye cursor.
  if (g_hCursorSearchWindow)
  {
    g_hCursorPrevious = SetCursor (g_hCursorSearchWindow);
  }
  else
  {
    g_hCursorPrevious = NULL;
  }

  // Very important : capture all mouse activities from now onwards and
  // direct all mouse messages to the "Search Window" dialog box procedure.
  SetCapture (hwndDialog);
    
  return lRet;
}

long DisplayInfoOnFoundWindow (HWND hwndDialog, HWND hwndFoundWindow)
{

  long		lRet = 0;

	CHAR szOwnerClassName[256];
	CHAR szOwnerTitle[256];
	CHAR szParentClassName[256];
	CHAR szParentTitle[256];
	CHAR szControlClassName[256];
	CHAR szControlTitle[256];
	CHAR szFileName[_MAX_PATH];
	memset(szOwnerClassName,0,sizeof(szOwnerClassName));
	memset(szOwnerTitle,0,sizeof(szOwnerTitle));
	memset(szParentClassName,0,sizeof(szParentClassName));
	memset(szParentTitle,0,sizeof(szParentTitle));
	memset(szControlClassName,0,sizeof(szControlClassName));
	memset(szControlTitle,0,sizeof(szControlTitle));
	memset(szFileName,0,sizeof(szFileName));
	int iControlID = 0;
	DWORD processID = 0;
	
	CHAR drive[MAX_PATH] = {0};
	CHAR dir[MAX_PATH] = {0};
	CHAR fname[MAX_PATH] = {0};
	CHAR ext[MAX_PATH] = {0};

	GetClassNameA(GetAncestor(hwndFoundWindow, GA_ROOTOWNER),szOwnerClassName,sizeof(szOwnerClassName));
	GetWindowTextA(GetAncestor(hwndFoundWindow, GA_ROOTOWNER),szOwnerTitle,sizeof(szOwnerTitle));
	GetClassNameA(GetAncestor(hwndFoundWindow, GA_ROOT),szParentClassName,sizeof(szParentClassName));
	GetWindowTextA(GetAncestor(hwndFoundWindow, GA_ROOT),szParentTitle,sizeof(szParentTitle));
	GetClassNameA(hwndFoundWindow,szControlClassName,sizeof(szControlClassName));
	GetWindowTextA(hwndFoundWindow,szControlTitle,sizeof(szControlTitle));
	GetClassNameA(hwndFoundWindow,szControlClassName,sizeof(szControlClassName));
	GetWindowTextA(hwndFoundWindow,szControlTitle,sizeof(szControlTitle));
	iControlID = GetDlgCtrlID(hwndFoundWindow);
	GetWindowThreadProcessId(hwndFoundWindow, &processID);
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	GetProcessImageFileNameA( hProcess, szFileName, sizeof(szFileName) / sizeof(TCHAR));
	_tsplitpath(szFileName, drive, dir, fname, ext );
	memset(szFileName,0,sizeof(szFileName));
	_tcscat(szFileName,fname);
	_tcscat(szFileName,ext);

	if(hwndDialog == hWndIgnore)
	{
		SetDlgItemText (hwndDialog, IDC_EDIT_OWNERTITLE, szOwnerTitle);
		SetDlgItemText (hwndDialog, IDC_EDIT_OWNERCLASS, szOwnerClassName);
		SetDlgItemText (hwndDialog, IDC_EDIT_PARENTTITLE, szParentTitle);
		SetDlgItemText (hwndDialog, IDC_EDIT_PARENTCLASS, szParentClassName);
		SetDlgItemText (hwndDialog, IDC_EDIT_CONTROLTITLE, szControlTitle);
		SetDlgItemText (hwndDialog, IDC_EDIT_CONTROLCLASS, szControlClassName);
		SetDlgItemText (hwndDialog, IDC_EDIT_FILENAME, szFileName);
		if(iControlID==0){
			SetDlgItemText (hwndDialog, IDC_EDIT_CONTROLID, "");
		} else {
			SetDlgItemInt(hwndDialog, IDC_EDIT_CONTROLID, iControlID, false);
		}
	} else {
		SetDlgItemText (hwndDialog, IDC_EDITCFG_OWNERTITLE, szOwnerTitle);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_OWNERCLASS, szOwnerClassName);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_PARENTTITLE, szParentTitle);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_PARENTCLASS, szParentClassName);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_CONTROLTITLE, szControlTitle);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_CONTROLCLASS, szControlClassName);
		SetDlgItemText (hwndDialog, IDC_EDITCFG_FILENAME, szFileName);
		if(iControlID==0){
			SetDlgItemText (hwndDialog, IDC_EDITCFG_CONTROLID, "");
		} else {
			SetDlgItemInt(hwndDialog, IDC_EDITCFG_CONTROLID, iControlID, false);
		}
	}	

  return lRet;
}

long RefreshWindow (HWND hwndWindowToBeRefreshed)
{
  long lRet = 0;

  InvalidateRect (hwndWindowToBeRefreshed, NULL, TRUE);
  UpdateWindow (hwndWindowToBeRefreshed);
  RedrawWindow (hwndWindowToBeRefreshed, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

  return lRet;
}

long HighlightFoundWindow (HWND hwndDialog, HWND hwndFoundWindow)
{
// Performs a highlighting of a found window.
// Comments below will demonstrate how this is done.
  HDC		hWindowDC = NULL;  // The DC of the found window.
  HGDIOBJ	hPrevPen = NULL;   // Handle of the existing pen in the DC of the found window.
  HGDIOBJ	hPrevBrush = NULL; // Handle of the existing brush in the DC of the found window.
  RECT		rect;              // Rectangle area of the found window.
  long		lRet = 0;

  // Get the screen coordinates of the rectangle of the found window.
  GetWindowRect (hwndFoundWindow, &rect);

  // Get the window DC of the found window.
  hWindowDC = GetWindowDC (hwndFoundWindow);

  if (hWindowDC)
  {
    // Select our created pen into the DC and backup the previous pen.
    hPrevPen = SelectObject (hWindowDC, g_hRectanglePen);

    // Select a transparent brush into the DC and backup the previous brush.
    hPrevBrush = SelectObject (hWindowDC, GetStockObject(HOLLOW_BRUSH));

    // Draw a rectangle in the DC covering the entire window area of the found window.
    Rectangle (hWindowDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top);

    // Reinsert the previous pen and brush into the found window's DC.
    SelectObject (hWindowDC, hPrevPen);

    SelectObject (hWindowDC, hPrevBrush);

    // Finally release the DC.
    ReleaseDC (hwndFoundWindow, hWindowDC);
  }

  return lRet;
}

BOOL InitialiseResources()
{
  BOOL bRet = FALSE;

  g_hCursorSearchWindow = LoadCursor (hInst, MAKEINTRESOURCE(IDC_CURSOR_SEARCH_WINDOW));
  if (g_hCursorSearchWindow == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hRectanglePen = CreatePen (PS_SOLID, 3, RGB(256, 0, 0));
  if (g_hRectanglePen == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hBitmapFinderToolFilled = LoadBitmap (hInst, MAKEINTRESOURCE(IDB_BITMAP_FINDER_FILLED));
  if (g_hBitmapFinderToolFilled == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  g_hBitmapFinderToolEmpty = LoadBitmap (hInst, MAKEINTRESOURCE(IDB_BITMAP_FINDER_EMPTY));
  if (g_hBitmapFinderToolEmpty == NULL)
  {
    bRet = FALSE;
	goto InitialiseResources_0;
  }

  // All went well. Return TRUE.
  bRet = TRUE;

InitialiseResources_0:

  return bRet;
}

BOOL UninitialiseResources()
{
  BOOL bRet = TRUE;

  if (g_hCursorSearchWindow)
  {
    // No need to destroy g_hCursorSearchWindow. It was not created using 
	// CreateCursor().
  }

  if (g_hRectanglePen)
  {
    bRet = DeleteObject (g_hRectanglePen);
	g_hRectanglePen = NULL;
  }

  if (g_hBitmapFinderToolFilled)
  {
	DeleteObject (g_hBitmapFinderToolFilled);
	g_hBitmapFinderToolFilled = NULL;
  }

  if (g_hBitmapFinderToolEmpty)
  {
	DeleteObject (g_hBitmapFinderToolEmpty);
	g_hBitmapFinderToolEmpty = NULL;
  }
    
  return bRet;
}

#pragma endregion Find Window Code


#pragma region Gesture Functions

void ScaleGesture(int Width, int Height)
{

	ScaledDrawPoints.clear();

	// Get total width and height of gesture
	float fGestureOffsetLeft = 0; 
	float fGestureOffsetTop = 0; 
	float fGestureWidth = 0; 
	float fGestureHeight = 0; 

	fGestureOffsetLeft = TempDrawPoints[0].x;
	fGestureOffsetTop = TempDrawPoints[0].y;
	fGestureWidth = TempDrawPoints[0].x;
	fGestureHeight = TempDrawPoints[0].y;
	for(int i = 0; i < TempDrawPoints.size(); i++) { 
		if(TempDrawPoints[i].x <= fGestureOffsetLeft)
			fGestureOffsetLeft = TempDrawPoints[i].x;
		if(TempDrawPoints[i].y <= fGestureOffsetTop)
			fGestureOffsetTop = TempDrawPoints[i].y;
		if(TempDrawPoints[i].x >= fGestureWidth)
			fGestureWidth = TempDrawPoints[i].x;
		if(TempDrawPoints[i].y >= fGestureHeight)
			fGestureHeight = TempDrawPoints[i].y;
	}

	fGestureWidth -= fGestureOffsetLeft;
	fGestureHeight -= fGestureOffsetTop;

	// Get each scale ratio
	double dScaleX = (double)Width / fGestureWidth;
	double dScaleY = (double)Height / fGestureHeight;
	double dScaleVar = 0;
	POINTF tmpPoint;

	// Scale on the longest axis
	if (fGestureWidth >= fGestureHeight)
	{
		// Scale on X axis
		dScaleVar = dScaleX;
	}
	else
	{
		dScaleVar = dScaleY;
	}

	for(int i = 0; i < TempDrawPoints.size(); i++) { 
		tmpPoint.x = (float)((TempDrawPoints[i].x - fGestureOffsetLeft) * dScaleVar);
		tmpPoint.y = (float)((TempDrawPoints[i].y - fGestureOffsetTop) * dScaleVar);
		ScaledDrawPoints.push_back(tmpPoint);
	}

	// Calculate new gesture width and height
	iScaledWidth = (int)floor(fGestureWidth * dScaleVar);
	iScaledHeight = (int)floor(fGestureHeight * dScaleVar);
}

void BuildGestureDropDown(){
	SendMessage(hWndCB, CB_RESETCONTENT,0, 0);
	COMBOBOXEXITEM cbei;
	cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
	vector<string> TempGestureNames;
	TempGestureNames.push_back(_T(""));
	BOOST_FOREACH(boost::property_tree::ptree::value_type &vGest, ptConfig.get_child("config.Gestures")) {
		if(vGest.second.get("<xmlattr>.name",_T("")).length() > 0){
			TempGestureNames.push_back(vGest.second.get("<xmlattr>.name",_T("")));
		}
	}
	sort(TempGestureNames.begin(), TempGestureNames.end(), LessNoCase());

	for(int i = 0; i < TempGestureNames.size(); i++) { 
		TCHAR szGesture[100];
		_tcscpy(szGesture,TempGestureNames[i].c_str());
		cbei.iItem          = i;
		cbei.pszText        = szGesture;
		cbei.cchTextMax     = 100;
		for(int ii = 0; ii < GestureNames.size(); ii++) { 
			if(_tcscmp( GestureNames[ii].c_str(), TempGestureNames[i].c_str() ) == 0 ){
				cbei.iImage         = ii;
				cbei.iSelectedImage = ii;
				break;
			}
		}
		cbei.iIndent        = 0;	
		SendMessage(hWndCB,CBEM_INSERTITEM,0,(LPARAM)&cbei);
	}
}

void BuildGestureImageList(BOOL bTree, int iSize) {
	
	EnterCriticalSection(&CriticalSection); 
	int iGestCount = 0;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &vGest, ptConfig.get_child("config.Gestures")) {
		iGestCount++;
	}

	if(bTree){
		ImageList_Destroy(hListTree);
		hListTree = ImageList_Create(iSize, iSize, ILC_COLOR16 | ILC_MASK, iGestCount, 1);
	} else {
		ImageList_Destroy(hList);
		hList = ImageList_Create(iSize, iSize, ILC_COLOR16 | ILC_MASK, iGestCount, 1);
	}
	
	#ifdef SP_DEBUG_MODE
  		TCHAR buffer[256];
		wsprintf(buffer, 
			_T("hListTree: %d\n")
			_T("hList: %d"),
			hListTree,hList);
		LOGMESSAGE(buffer);
	#endif

	GestureNames.clear();

	//This section creates a bitmap for the X icon when no gesture is selected for an action
	Bitmap* blank;
	blank = new Bitmap(iSize, iSize,PixelFormat32bppRGB);
	Graphics bgr(blank);
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	bgr.FillRectangle(&whiteBrush, 0, 0, iSize, iSize);
	Pen grayPen(Color(255, 200,200,200), iSize/16);
	grayPen.SetLineCap(LineCapNoAnchor, LineCapNoAnchor, DashCapRound); 
	bgr.SetSmoothingMode(SmoothingModeAntiAlias);
	bgr.SetCompositingQuality ( CompositingQualityHighQuality ) ;
	bgr.SetInterpolationMode( InterpolationModeHighQuality ) ; 
	Point startPoint;
	Point endPoint;

	startPoint.X = iSize/7;
	startPoint.Y = iSize/7;
	endPoint.X = iSize - iSize/7;
	endPoint.Y = iSize - iSize/7;

	bgr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.Y = iSize - iSize/7;
	endPoint.Y = iSize/7;
	bgr.DrawLine(&grayPen, startPoint, endPoint);

	HBITMAP bhandle = NULL;
	blank->GetHBITMAP(Color::White, &bhandle);

	if(bTree){
		ImageList_Add(hListTree, bhandle, NULL);
	} else {
		ImageList_Add(hList, bhandle, NULL);
	}

	delete blank;
	DeleteObject(bhandle);

	GestureNames.push_back("");
	
	//This section creates a bitmap for the app icon
	Bitmap* appimage;
	appimage = new Bitmap(iSize, iSize,PixelFormat32bppRGB);
	Graphics agr(appimage);
	agr.FillRectangle(&whiteBrush, 0, 0, iSize, iSize);
	grayPen.SetLineCap(LineCapNoAnchor, LineCapNoAnchor, DashCapRound); 
	agr.SetSmoothingMode(SmoothingModeAntiAlias);
	agr.SetCompositingQuality ( CompositingQualityHighQuality ) ;
	agr.SetInterpolationMode( InterpolationModeHighQuality ) ; 

	startPoint.X = iSize/8;
	startPoint.Y = iSize/7;
	endPoint.X = iSize - iSize/8;
	endPoint.Y = iSize/7;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.X = iSize - iSize/8;
	endPoint.Y = iSize - iSize/7;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.X = iSize - iSize/8;
	endPoint.Y = iSize - iSize/7;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.X = iSize - iSize/8;
	startPoint.Y = iSize - iSize/7;
	endPoint.X = iSize/8;
	endPoint.Y = iSize - iSize/7;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.X = iSize/8;
	startPoint.Y = iSize/7;
	endPoint.X = iSize/8;
	endPoint.Y = iSize - iSize/7;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	startPoint.X = iSize/8;
	startPoint.Y = (iSize/8)*3;
	endPoint.X = iSize - iSize/8;
	endPoint.Y = (iSize/8)*3;
	agr.DrawLine(&grayPen, startPoint, endPoint);

	HBITMAP ahandle = NULL;
	appimage->GetHBITMAP(Color::White, &ahandle);
	if(bTree){
		ImageList_Add(hListTree, ahandle, NULL);
	} else {
		ImageList_Add(hList, ahandle, NULL);
	}
	delete appimage;
	DeleteObject(ahandle);

	GestureNames.push_back(_T("%%STROKESPLUS_APP_ICON%%"));

	//Now, build bitmaps for each actual gesture
	BOOST_FOREACH(boost::property_tree::ptree::value_type &vGest, ptConfig.get_child("config.Gestures")) {
		TempDrawPoints.clear();
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v8, vGest.second) {
			if(v8.first == _T("PointPatterns")) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v9, v8.second) {
					//each point pattern
					if(v9.first == _T("PointPattern")) {
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v10, v9.second) {
							//each point 
							if(v10.first == _T("Point")) {
								POINTF Point;
								int x = (int)v10.second.get("<xmlattr>.x",0);
								int y = (int)v10.second.get("<xmlattr>.y",0);
								Point.x = (float)x;
								Point.y = (float)y;
								TempDrawPoints.push_back(Point);						
							}
						}
						break;
					}
				}
				Bitmap* result;
				result = new Bitmap(iSize, iSize,PixelFormat32bppRGB);
				Graphics gr(result);
				gr.FillRectangle(&whiteBrush, 0, 0, iSize, iSize);
				Pen bluePen(Color(255, 0,0,255), iSize/16);
				bluePen.SetLineCap(LineCapRoundAnchor, LineCapArrowAnchor, DashCapRound); 
				gr.SetSmoothingMode(SmoothingModeAntiAlias);
				gr.SetCompositingQuality ( CompositingQualityHighQuality ) ;
				gr.SetInterpolationMode( InterpolationModeHighQuality ) ; 
				ScaleGesture(iSize*.78, iSize*.78);
				int iLeftOffset = (int)floor((double)(((iSize*.78)/2)-(iScaledWidth / 2)));
				int iTopOffset = (int)floor((double)(((iSize*.78)/2)-(iScaledHeight / 2)));
				PointF *arrPoints = new PointF[ScaledDrawPoints.size()];
				for(int i = 0; i < ScaledDrawPoints.size(); i++) { 
					arrPoints[i].X = ScaledDrawPoints[i].x+(iSize*.079)+iLeftOffset;
					arrPoints[i].Y = ScaledDrawPoints[i].y+(iSize*.079)+iTopOffset;
				}
				gr.DrawLines(&bluePen, arrPoints, TempDrawPoints.size());

				HBITMAP handle = NULL;
				result->GetHBITMAP(Color::White, &handle);
				if(bTree){
					ImageList_Add(hListTree, handle, NULL);
				} else {
					ImageList_Add(hList, handle, NULL);
				}

				GestureNames.push_back(vGest.second.get("<xmlattr>.name",_T("")));

				delete[] arrPoints;
				delete result;
				DeleteObject(handle);
				break;
			}
		}
	}
	LeaveCriticalSection(&CriticalSection);
}

#pragma endregion Gesture Functions


#pragma region Preferences Window Functions

void setPrefStrokeButton(){
	SendDlgItemMessage(hPreferencesTab, IDC_USERIGHT, BM_SETCHECK, hookbtn == 0 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USEMIDDLE, BM_SETCHECK, hookbtn == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USELEFT, BM_SETCHECK, hookbtn == 2 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USEXBTN1, BM_SETCHECK, hookbtn == 3 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USEXBTN2, BM_SETCHECK, hookbtn == 4 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USECTRL, BM_SETCHECK, hookbtn == -1 ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hPreferencesTab, IDC_USEALT, BM_SETCHECK, hookbtn == -2 ? BST_CHECKED : BST_UNCHECKED, 0);
}

void savePreferences(HWND hDlg)
{
	setDrawColor(RGB(GetDlgItemInt(hDlg, IDC_PREFSRED, NULL, false),GetDlgItemInt(hDlg, IDC_PREFSGREEN, NULL, false),GetDlgItemInt(hDlg, IDC_PREFSBLUE, NULL, false)));
	setDrawWidth((GetDlgItemInt(hDlg, IDC_PREFSWIDTH, NULL, false) == 0 ? 1 : GetDlgItemInt(hDlg, IDC_PREFSWIDTH, NULL, false)));
	opacity = GetDlgItemInt(hDlg, IDC_PREFSALPHA, NULL, false);
	if(opacity==0) opacity=1;
	setWindowTransparency();

	iMinSegmentLength = (GetDlgItemInt(hDlg, IDC_PREFSSEGMENT, NULL, false) < 7 ? 6 : GetDlgItemInt(hDlg, IDC_PREFSSEGMENT, NULL, false));

	if(SendDlgItemMessage(hDlg, IDC_USERIGHT, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(0);
	if(SendDlgItemMessage(hDlg, IDC_USEMIDDLE, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(1);
	if(SendDlgItemMessage(hDlg, IDC_USELEFT, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(2);
	if(SendDlgItemMessage(hDlg, IDC_USEXBTN1, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(3);
	if(SendDlgItemMessage(hDlg, IDC_USEXBTN2, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(4);
	if(SendDlgItemMessage(hDlg, IDC_USECTRL, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(-1);
	if(SendDlgItemMessage(hDlg, IDC_USEALT, BM_GETCHECK, 0, 0) == 1)
		setHookBtn(-2);
	#ifdef SP_SYNAPTIC
		if(SendDlgItemMessage(hDlg, IDC_SYNAPTICSDISABLE, BM_GETCHECK, 0, 0) == 1) {
			bDisableSynaptics = TRUE;
		} else {
			bDisableSynaptics = FALSE;
		}
	#endif

	#ifdef SP_WHEEL_HOOK
		if(SendDlgItemMessage(hDlg, IDC_CHK_FIREWHEELTICK, BM_GETCHECK, 0, 0) == 1) {
			bMouseWheelTick = TRUE;
		} else {
			bMouseWheelTick = FALSE;
		}
		if(SendDlgItemMessage(hDlg, IDC_CHK_FIREHORZWHEELTICK, BM_GETCHECK, 0, 0) == 1) {
			bHorzMouseWheelTick = TRUE;
		} else {
			bHorzMouseWheelTick = FALSE;
		}
	#endif

	CHAR szWAVFile[_MAX_PATH];
	GetDlgItemText(hDlg, IDC_EDIT_NOMATCHWAV, szWAVFile, _MAX_PATH);
	sNoMatchSoundWAV = szWAVFile;

	if(SendDlgItemMessage(hDlg, IDC_CHK_NOSOUNDMATCH, BM_GETCHECK, 0, 0) == 1) {
		bNoMatchSound = true;
	} else {
		bNoMatchSound = false;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_CHECKRELAYGESTURE, BM_GETCHECK, 0, 0) == 1) {
		bRelayGesture = true;
	} else {
		bRelayGesture = false;
	}
	
	if(SendDlgItemMessage(hDlg, IDC_CHK_HIDENEWGMSG, BM_GETCHECK, 0, 0) == 1) {
		bHideNewGestureMessage = TRUE;
	} else {
		bHideNewGestureMessage = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_HIDEINSTANCE, BM_GETCHECK, 0, 0) == 1) {
		bHideAdditionalInstanceMessage = TRUE;
	} else {
		bHideAdditionalInstanceMessage = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_INITONRESUME, BM_GETCHECK, 0, 0) == 1) {
		bReInitOnResume = TRUE;
	} else {
		bReInitOnResume = FALSE;
	}

	#ifdef SP_NOGLOBALACTIONS
		if(SendDlgItemMessage(hDlg, IDC_CHK_DISABLEGLOBALAPP, BM_GETCHECK, 0, 0) == 1) {
			bOnlyAllowDefinedApps = TRUE;
		} else {
			bOnlyAllowDefinedApps = FALSE;
		}
	#endif

	if(getGesturesDisabled()==false){
		PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_DRAWWINDOWONTOP, BM_GETCHECK, 0, 0) == 1) {
		bDrawWindowAlwaysOnTop = TRUE;
	} else {
		bDrawWindowAlwaysOnTop = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_DRAWWINDOWTOBTM, BM_GETCHECK, 0, 0) == 1) {
		bDrawWindowToBottom = TRUE;
	} else {
		bDrawWindowToBottom = FALSE;
	}

	setWindowState();

	if(SendDlgItemMessage(hDlg, IDC_CHK_RESETCANCELDELAY, BM_GETCHECK, 0, 0) == 1) {
		bResetCancelDelayOnMovement = TRUE;
	} else {
		bResetCancelDelayOnMovement = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_FIREONWHEEL, BM_GETCHECK, 0, 0) == 1) {
		bFireOnMouseWheel = TRUE;
	} else {
		bFireOnMouseWheel = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_GETMODSONMOUSEDOWN, BM_GETCHECK, 0, 0) == 1) {
		bCaptureModifiersOnMouseDown = TRUE;
	} else {
		bCaptureModifiersOnMouseDown = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWAFTERSCRIPT, BM_GETCHECK, 0, 0) == 1) {
		bAllowAfterScript = TRUE;
	} else {
		bAllowAfterScript = FALSE;
	}

	#ifdef SP_AFTERRELEASESCRIPT
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWAFTERRELEASESCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnRelease = TRUE;
		} else {
			bFireOnRelease = FALSE;
		}
	#endif

	#ifdef SP_MOUSE_CLICK_ACTION
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWRIGHTCLICKSCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnRightClick = TRUE;
		} else {
			bFireOnRightClick = FALSE;
		}
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWMIDDLECLICKSCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnMiddleClick = TRUE;
		} else {
			bFireOnMiddleClick = FALSE;
		}
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWLEFTCLICKSCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnLeftClick = TRUE;
		} else {
			bFireOnLeftClick = FALSE;
		}
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWX1CLICKSCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnX1Click = TRUE;
		} else {
			bFireOnX1Click = FALSE;
		}
		if(SendDlgItemMessage(hDlg, IDC_CHK_ALLOWX2CLICKSCRIPT, BM_GETCHECK, 0, 0) == 1) {
			bFireOnX2Click = TRUE;
		} else {
			bFireOnX2Click = FALSE;
		}
	#endif

	if(SendDlgItemMessage(hDlg, IDC_CHK_CHECKCURSORFLAGS, BM_GETCHECK, 0, 0) == 1) {
		bCheckCursorFlags = TRUE;
	} else {
		bCheckCursorFlags = FALSE;
	}

	if(SendDlgItemMessage(hDlg, IDC_CHK_MOUSEWHEEL, BM_GETCHECK, 0, 0) == 1) {
		bMouseWheelFocus = true;
	} else {
		bMouseWheelFocus = false;
	}
	setMatchPrecision(GetDlgItemInt(hDlg, IDC_EDIT_PRECISION, NULL, false));
	setMatchProbabilityThreshold(GetDlgItemInt(hDlg, IDC_EDIT_PROBABILITY, NULL, false));
	setMaxDrawPoints(GetDlgItemInt(hDlg,IDC_EDIT_MAXDRAWPTS, NULL, false));
	setDrawGesture((BOOL)SendDlgItemMessage(hDlg,IDC_PREFSDRAWS, CB_GETCURSEL, 0, 0));
	if(SendDlgItemMessage(hDlg, IDC_CHK_TRIM, BM_GETCHECK, 0, 0) == 1) {
		bAggressiveTrim = TRUE;
		SetProcessWorkingSetSize(GetCurrentProcess(), 0xffffffff, 0xffffffff); 
		fireTrimTimer();
	} else {
		//KillTimer(NULL, trimtimerid);
		bAggressiveTrim = FALSE;
	}
	setAggressiveTrim(bAggressiveTrim);

	setIgnoreKey(SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY, CB_GETCURSEL, 0, 0));
	setCancelDelay((GetDlgItemInt(hDlg,IDC_PREFSCANCELDELAY, NULL, false) < 50 ? 50 : GetDlgItemInt(hDlg,IDC_PREFSCANCELDELAY, NULL, false) ));
	#ifdef SP_SYNAPTIC
		iTouchPadFingers = SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS, CB_GETCURSEL, 0, 0);
		iTouchPadFingers++;
		iTouchPadSensitivity = (GetDlgItemInt(hDlg,IDC_EDIT_SYNAPTICSSENS, NULL, false) < 0 ? 0 : GetDlgItemInt(hDlg,IDC_EDIT_SYNAPTICSSENS, NULL, false) );
		iTouchPadHoldThresholdDelta = (GetDlgItemInt(hDlg,IDC_PREFSTPHOLDDELTA, NULL, false) < 0 ? 1 : GetDlgItemInt(hDlg,IDC_PREFSTPHOLDDELTA, NULL, false) );
		iTouchPadHoldDelay = (GetDlgItemInt(hDlg,IDC_PREFSTPHOLDDELAY, NULL, false) < 0 ? 1 : GetDlgItemInt(hDlg,IDC_PREFSTPHOLDDELAY, NULL, false) );
	#endif
}

void drawStrokeColor(HWND hDlg)
{

	RECT lpTop;
	POINT ptTopTL;

	GetWindowRect(GetDlgItem(hDlg, IDC_USERIGHT),&lpTop);
	ptTopTL.x = lpTop.left;
	ptTopTL.y = lpTop.top;
	ScreenToClient(hDlg, &ptTopTL);

	RECT lpLeft;
	POINT ptLeftTL;

	GetWindowRect(GetDlgItem(hDlg, IDC_BTN_COLOR),&lpLeft);
	ptLeftTL.x = lpLeft.left;
	ptLeftTL.y = lpLeft.top;
	ScreenToClient(hDlg, &ptLeftTL);

	RECT lpRight;
	POINT ptRightTL;

	GetWindowRect(GetDlgItem(hDlg, IDC_BTN_COLOR),&lpRight);
	ptRightTL.x = lpRight.right;
	ptRightTL.y = lpRight.bottom;
	ScreenToClient(hDlg, &ptRightTL);

	RECT lpBottom;
	POINT ptBottomTL;

	GetWindowRect(GetDlgItem(hDlg, IDC_USEMIDDLE),&lpBottom);
	ptBottomTL.x = lpBottom.left;
	ptBottomTL.y = lpBottom.bottom;
	ScreenToClient(hDlg, &ptBottomTL);

	Graphics graphics(hDlg);
	SolidBrush fBrush(Color(255, GetDlgItemInt(hDlg, IDC_PREFSRED, NULL, false), GetDlgItemInt(hDlg, IDC_PREFSGREEN, NULL, false),GetDlgItemInt(hDlg, IDC_PREFSBLUE, NULL, false)));
	//SolidBrush fBrush(Color(255, getDrawColorR(), getDrawColorG(), getDrawColorB()));
	graphics.FillRectangle(&fBrush, (float)ptLeftTL.x,(float)ptTopTL.y,(float)ptRightTL.x-ptLeftTL.x,(float)ptBottomTL.y-ptTopTL.y);
}

#pragma endregion Preferences Window Functions


#pragma region Actions Window Functions

void drawConfigGesture(HWND hDlg){

	RECT lpRect;
	lpRect.left = ptPreviewTL.x; 
	lpRect.top = ptPreviewTL.y; 
	lpRect.right = ptPreviewTR.x; 
	lpRect.bottom = ptPreviewTR.y; 
	InvalidateRect(hDlg, &lpRect, TRUE);
	UpdateWindow(hDlg);

	if(g_Points.size() > 0){
		drawLastGestureToHandle(hDlg, lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, true);
	}
}

BOOL BuildConfigTree(HWND hDlg) {

	//------------------------------------------------------------------------------------------------------------------
	//TREE VIEW CODE


	TreeView_SetImageList(GetDlgItem(hDlg,IDC_TREEAPPS), hListTree, TVSIL_NORMAL);

	TV_INSERTSTRUCT tvis ;
	HTREEITEM hLastSelected = NULL;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
			//each app
			if(v2.second.get("<xmlattr>.name",_T("")).length() > 0){
				TCHAR sDesc[255];
				ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
				tvis.hParent = NULL ;
				tvis.hInsertAfter = TVI_SORT ;
				tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				tvis.item.cchTextMax = 256 ;
				_tcscpy (sDesc,v2.second.get("<xmlattr>.name",_T("Unknown")).c_str());
				char *cDesc = const_cast<char*> ( sDesc );
				tvis.item.pszText = cDesc;
				tvis.item.lParam =(LPARAM) sDesc;
				tvis.item.cChildren = 1 ;
				tvis.item.iImage = 1;
				tvis.item.iSelectedImage = 1;
				HTREEITEM hParent = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEAPPS), &tvis) ;
				if(_tcsicmp( sLastParent.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0) {
					hLastSelected = hParent;
				}
				
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("Actions"))) {
					if(v3.second.get("<xmlattr>.description",_T("")).length() > 0){
						TCHAR sDesc[255];
						HTREEITEM hChild;
						tvis.hParent = hParent;
						tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
						tvis.hInsertAfter = TVI_SORT;
						_tcscpy (sDesc,v3.second.get("<xmlattr>.description",_T("Unknown")).c_str());
						char *cDesc = const_cast<char*> ( sDesc );
						tvis.item.pszText = cDesc;
						tvis.item.lParam =(LPARAM) sDesc;
						tvis.item.cChildren = 0 ;
						for(int i = 0; i < GestureNames.size(); i++) { 
							if(_tcscmp( GestureNames[i].c_str(), v3.second.get("GestureName",_T("")).c_str() ) == 0 ){
								tvis.item.iImage = i;
								tvis.item.iSelectedImage = i;
								break;
							}
						}
						hChild = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEAPPS), &tvis);
						if(_tcsicmp( sLastChild.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str()) == 0 && _tcsicmp( sLastParent.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str()) == 0) {
							hLastSelected = hChild;
						}
					}
				}
			}
	}

	// Initialize list view item structure
	ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
	tvis.hParent = NULL ;
	tvis.hInsertAfter = TVI_FIRST ;
	tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvis.item.cchTextMax = 256 ;
	CHAR sGlobal[255];
	_tcscpy(sGlobal,	ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str());
	tvis.item.pszText = sGlobal ;
	tvis.item.lParam = (LPARAM) _T("SP_Global_Root_App_Node") ;
	tvis.item.cChildren = 1 ;
	tvis.item.iImage = 1;
	tvis.item.iSelectedImage = 1;
	HTREEITEM hParent = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEAPPS), &tvis) ;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global.Actions"))) {
		//each global action
		if(v2.second.get("<xmlattr>.description",_T("")).length() > 0){
			TCHAR sDesc[255];
			HTREEITEM hChild;
			tvis.hParent = hParent;
			tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvis.hInsertAfter = TVI_SORT;
			_tcscpy (sDesc,v2.second.get("<xmlattr>.description",_T("Unknown")).c_str());
			char *cDesc = const_cast<char*> ( sDesc );
			tvis.item.pszText = cDesc;
			tvis.item.lParam =(LPARAM) sDesc;
			tvis.item.cChildren = 0 ;
			for(int i = 0; i < GestureNames.size(); i++) { 
				if(_tcscmp( GestureNames[i].c_str(), v2.second.get("GestureName",_T("")).c_str() ) == 0 ){
					tvis.item.iImage = i;
					tvis.item.iSelectedImage = i;
					break;
				}
			}
			hChild = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEAPPS), &tvis);
			if(_tcsicmp( sLastChild.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str()) == 0 && _tcsicmp( sLastParent.c_str(), sGlobal ) == 0) {
				hLastSelected = hChild;
			}
		}
	}
	
	BOOL bGetFirstVisible = FALSE;
	if(sLastParent.length() > 0) {
		bGetFirstVisible = TreeView_Select( GetDlgItem(hDlg , IDC_TREEAPPS), hLastSelected, TVGN_CARET);
	}
	if(!bGetFirstVisible) {
		TreeView_Select( GetDlgItem(hDlg , IDC_TREEAPPS), TreeView_GetNextItem(GetDlgItem(hDlg , IDC_TREEAPPS), TreeView_GetFirstVisible(GetDlgItem(hDlg , IDC_TREEAPPS)),TVGN_CHILD), TVGN_CARET);
	}

	return TRUE;

}

BOOL SetModifierStates(HWND hDlg){
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_RIGHTBTN), (hookbtn == 0 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_MIDDLEBTN), (hookbtn == 1 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_LEFTBTN), (hookbtn == 2 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_X1BTN), (hookbtn == 3 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_X2BTN), (hookbtn == 4 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_CTRL), (hookbtn == -1 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_ALT), (hookbtn == -2 ? false : true));
	EnableWindow(GetDlgItem(hDlg, IDC_CHK_CFG_SHIFT), (hookbtn == -3 ? false : true));

	EnableWindow (GetDlgItem(hDlg, IDC_GRP_WHENFIRE), bCaptureModifiersOnMouseDown);
	EnableWindow (GetDlgItem(hDlg, IDC_R_EITHER), bCaptureModifiersOnMouseDown);
	EnableWindow (GetDlgItem(hDlg, IDC_R_AFTER), bCaptureModifiersOnMouseDown);
	EnableWindow (GetDlgItem(hDlg, IDC_R_BEFORE), bCaptureModifiersOnMouseDown);

	return true;
}

BOOL ResetModifierStates(HWND hDlg){
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_SETCHECK, BST_UNCHECKED, 0);

	SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_SETCHECK, BST_CHECKED, 0);

	return true;
}

BOOL ShowConfigFields(HWND hDlg, BOOL bAppNode, BOOL bHideAll) {

	BOOL bShow;
	bCriteraFieldsActive = bAppNode;
	bShow = !bAppNode;
	if(bHideAll) bShow = false;
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_MOUSEMOD_GROUP), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_RIGHTBTN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_MIDDLEBTN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_LEFTBTN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_X1BTN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_X2BTN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_SCROLLUP), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_SCROLLDOWN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_CTRL), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_ALT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_CFG_SHIFT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (hWndCB, bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_NEWG), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_DELG), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_RENG), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_GESTURE_GROUP), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CLUA), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITLUA), SW_HIDE);
	ShowWindow (m_hwndEditor, bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_PREVIEW), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_MODTEXT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_GRP_ACTIONHELP), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CBO_ACTIONS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_INSACTION), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_INFOACTION), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHKACTACTIVE), bShow ? SW_SHOW : SW_HIDE);

	ShowWindow (GetDlgItem(hDlg, IDC_GRP_WHENFIRE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_R_EITHER), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_R_AFTER), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_R_BEFORE), bShow ? SW_SHOW : SW_HIDE);

	SetModifierStates(hDlg);
	
	bShow = bAppNode;
	if(bHideAll) bShow = false;
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_COT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CFG_APP_TEXT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_COTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCNR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CCCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CID), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_CONTROLID), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CFN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_FILENAME), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CFNR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_FILENAME_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_CPR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_EDITCFG_PATH_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_CFG_FW), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_STATIC_ICON_FINDER_TOOL), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHK_NOGLOBAL), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow (GetDlgItem(hDlg, IDC_CHKAPPACTIVE), bShow ? SW_SHOW : SW_HIDE);

	return true;
}

BOOL LoadConfigNode(HWND hDlg, HTREEITEM hSelected){

	BOOL bAppMatchFound = FALSE;
	BOOL bActionMatchFound = FALSE;
	
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	tvi.hItem=hSelected;
	char cText[255] = {0};
    tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tvi);

	//AN APPLICATION or GLOBAL is selected
	if(tvi.cChildren == 1) 
	{
		sLastParent = cText;
		sLastChild = _T("");
		//check applications
		//HIDE ACTION CONTROLS< SHOW APP QUALIFIER CONTROLS
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
		//each ignored window
			if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/)
			{
				bAppMatchFound = TRUE;
				ShowConfigFields(hDlg, true, false);
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
					//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
					if(v3.first == _T("MatchCriteria")){
						SetDlgItemText (hDlg, IDC_EDITCFG_OWNERTITLE, v3.second.get("ownertitle",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_OWNERTITLE_REGEX, v3.second.get("ownertitleregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_OWNERCLASS, v3.second.get("ownerclass",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_OWNERCLASS_REGEX, v3.second.get("ownerclassregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_PARENTTITLE, v3.second.get("parenttitle",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_PARENTTITLE_REGEX, v3.second.get("parenttitleregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_PARENTCLASS, v3.second.get("parentclass",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_PARENTCLASS_REGEX, v3.second.get("parentclassregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_CONTROLTITLE, v3.second.get("controltitle",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX, v3.second.get("controltitleregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_CONTROLCLASS, v3.second.get("controlclass",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX, v3.second.get("controlclassregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_CONTROLID, v3.second.get("controlid",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_FILENAME, v3.second.get("file",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_FILENAME_REGEX, v3.second.get("fileregex",_T("")).c_str());
						SetDlgItemText (hDlg, IDC_EDITCFG_PATH_REGEX, v3.second.get("pathregex",_T("")).c_str());
						SendDlgItemMessage(hDlg, IDC_CHKAPPACTIVE, BM_SETCHECK, (v2.second.get("<xmlattr>.enabled",1) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
						SendDlgItemMessage(hDlg, IDC_CHK_NOGLOBAL, BM_SETCHECK, (v2.second.get("<xmlattr>.noglobal",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
						EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),TRUE);
						EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),TRUE);
					}
				}
			}
		} 

		//no app match, this is global
		if(!bAppMatchFound) {
			g_Points.clear();
			ShowConfigFields(hDlg, false, true);
			EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),FALSE);
			EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),FALSE);
			if(TreeView_GetChild(GetDlgItem(hDlg , IDC_TREEAPPS), TreeView_GetFirstVisible(GetDlgItem(hDlg , IDC_TREEAPPS))) != NULL){
				TreeView_Select( GetDlgItem(hDlg , IDC_TREEAPPS), TreeView_GetNextVisible(GetDlgItem(hDlg , IDC_TREEAPPS), TreeView_GetFirstVisible(GetDlgItem(hDlg , IDC_TREEAPPS)),TVGN_CHILD), TVGN_CARET);
			}
		} else {
			g_Points.clear();
		}

		
	} 
	//AN ACTION WAS SELECTED
	else 
	{
		sLastParent = _T("");
		sLastChild = _T("");
		if(tvi.hItem == 0){
			ShowConfigFields(hDlg, false, true);
			EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),FALSE);
			EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),FALSE);
		} else {
			g_Points.clear();

			ShowConfigFields(hDlg, false, false);
			
			//SHOW ACTION CONTROLS < HIDE APP QUALIFIER CONTROLS
			//GET PARENT OF SELECTED
			HTREEITEM hSelectedParent = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
			TVITEM parenttvi;
			ZeroMemory(&parenttvi,sizeof(parenttvi));
			parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN;
			parenttvi.hItem=hSelectedParent;
			char cParentText[255] = {0};
			parenttvi.pszText = cParentText;
			parenttvi.cchTextMax=256;
			SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&parenttvi);
			sLastParent = cParentText;
			sLastChild = cText;
			EnableWindow(GetDlgItem(hDlg , IDC_CFG_DELG),FALSE);	
			EnableWindow(GetDlgItem(hDlg , IDC_CFG_RENG),FALSE);
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
				//each app
				if(_tcsicmp( cParentText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
				{
					bAppMatchFound = TRUE;
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("Actions"))) {
						//each action
						if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1*/)
						{
							bActionMatchFound = true;
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_SETCHECK, (v3.second.get("Right",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_SETCHECK, (v3.second.get("Middle",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_SETCHECK, (v3.second.get("Left",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_SETCHECK, (v3.second.get("X1",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_SETCHECK, (v3.second.get("X2",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_SETCHECK, (v3.second.get("WheelUp",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_SETCHECK, (v3.second.get("WheelDown",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);

							SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_SETCHECK, (v3.second.get("Control",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_SETCHECK, (v3.second.get("Alt",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_SETCHECK, (v3.second.get("Shift",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_CHKACTACTIVE, BM_SETCHECK, (v3.second.get("<xmlattr>.enabled",1) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);

							SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_SETCHECK, (v3.second.get("StrokeState",0) == 0) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_SETCHECK, (v3.second.get("StrokeState",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
							SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_SETCHECK, (v3.second.get("StrokeState",0) == 2) ? BST_CHECKED : BST_UNCHECKED, 0);

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v7, ptConfig.get_child("config.Gestures")) {
								//each ignored window
								if(v7.second.get("<xmlattr>.name",_T("")) != _T("")){
									//if(v3.second.get("GestureName",_T("")) == v7.second.get("<xmlattr>.name",_T("")))
									if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(), v7.second.get("<xmlattr>.name",_T("")).c_str() ) == 0)
									{
										BOOST_FOREACH(boost::property_tree::ptree::value_type &v8, v7.second) {
											//get point Patterns
											if(v8.first == _T("PointPatterns")) {
												BOOST_FOREACH(boost::property_tree::ptree::value_type &v9, v8.second) {
													//each point pattern
													if(v9.first == _T("PointPattern")) {
														BOOST_FOREACH(boost::property_tree::ptree::value_type &v10, v9.second) {
															//each point 
															if(v10.first == "Point") {
																POINT Point;
																int x = (int)v10.second.get("<xmlattr>.x",0);
																int y = (int)v10.second.get("<xmlattr>.y",0);
																Point.x = (float)x;
																Point.y = (float)y;
																g_Points.push_back(Point);						
															}
														}
														break;
													}
													break;
												}
												break;
											}
										}
										break;
									}
								}
							}
							int totalitems = SendMessage(hWndCB,CB_GETCOUNT, 0, 0);
							int loopct = 0;
							int startmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)v3.second.get("GestureName",_T("")).c_str());
							int lastmatchindex = startmatchindex;
							TCHAR szGestureName[255];
							SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
							if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(),szGestureName ) != 0)
							{
								if(startmatchindex != CB_ERR)
								{
									lastmatchindex++;
									while(lastmatchindex != startmatchindex && loopct < totalitems)
									{
										loopct++;
										lastmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,lastmatchindex-1,(LPARAM)v3.second.get("GestureName",_T("")).c_str());
										SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
										if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(),szGestureName ) == 0)
											break;
									}
								}
							}
							SendMessage(hWndCB, CB_SETCURSEL, lastmatchindex, 0);
							EnableWindow(GetDlgItem(hDlg , IDC_CFG_DELG),(g_Points.size() > 0));	
							EnableWindow(GetDlgItem(hDlg , IDC_CFG_RENG),(g_Points.size() > 0));
							EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),bActionMatchFound);
							EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),bActionMatchFound);
							std::basic_string<TCHAR> sLua = v3.second.get("Lua",_T(""));
							boost::replace_all(sLua, _T("\n"), _T(""));
							boost::replace_all(sLua, _T("\r"), _T("\r\n"));
							SendEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)sLua.c_str());
							break;
						}
					}
				}
			}
			if(!bAppMatchFound)  
			{ //find GLOBAL action

				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global"))) {
					if(v2.first == "Actions"){
						bAppMatchFound = TRUE;
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
							//each action
							if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1 */)
							{
								bActionMatchFound = true;
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_SETCHECK, (v3.second.get("Right",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_SETCHECK, (v3.second.get("Middle",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_SETCHECK, (v3.second.get("Left",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_SETCHECK, (v3.second.get("X1",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_SETCHECK, (v3.second.get("X2",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_SETCHECK, (v3.second.get("WheelUp",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_SETCHECK, (v3.second.get("WheelDown",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);

								SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_SETCHECK, (v3.second.get("Control",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_SETCHECK, (v3.second.get("Alt",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_SETCHECK, (v3.second.get("Shift",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHKACTACTIVE, BM_SETCHECK, (v3.second.get("<xmlattr>.enabled",1) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);

								SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_SETCHECK, (v3.second.get("StrokeState",0) == 0) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_SETCHECK, (v3.second.get("StrokeState",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_SETCHECK, (v3.second.get("StrokeState",0) == 2) ? BST_CHECKED : BST_UNCHECKED, 0);

								//BOOL bFoundMatch = FALSE;
								BOOST_FOREACH(boost::property_tree::ptree::value_type &v7, ptConfig.get_child("config.Gestures")) {
									//each ignored window
									if(v7.second.get("<xmlattr>.name",_T("")) != _T("")){
										//if(v3.second.get("GestureName",_T("")) == v7.second.get("<xmlattr>.name",_T("")))
										
										if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(), v7.second.get("<xmlattr>.name",_T("")).c_str() ) == 0)
										{
											BOOST_FOREACH(boost::property_tree::ptree::value_type &v8, v7.second) {
												//get point Patterns
												if(v8.first == _T("PointPatterns")) {
													BOOST_FOREACH(boost::property_tree::ptree::value_type &v9, v8.second) {
														//each point pattern
														if(v9.first == _T("PointPattern")) {
															BOOST_FOREACH(boost::property_tree::ptree::value_type &v10, v9.second) {
																//each point 
																if(v10.first == _T("Point")) {
																	POINT Point;
																	int x = (int)v10.second.get("<xmlattr>.x",0);
																	int y = (int)v10.second.get("<xmlattr>.y",0);
																	Point.x = (float)x;
																	Point.y = (float)y;
																	g_Points.push_back(Point);						
																}
															}
															break;
														}
														break;
													}
													break;
												}
											}
											break;
										}
									}
								}

								int totalitems = SendMessage(hWndCB,CB_GETCOUNT, 0, 0);
								int loopct = 0;
								int startmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)v3.second.get("GestureName",_T("")).c_str());
								int lastmatchindex = startmatchindex;
								TCHAR szGestureName[255];
								SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
								if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(),szGestureName ) != 0)
								{
									if(startmatchindex != CB_ERR)
									{
										lastmatchindex++;
										while(lastmatchindex != startmatchindex && loopct < totalitems)
										{
											loopct++;
											lastmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,lastmatchindex-1,(LPARAM)v3.second.get("GestureName",_T("")).c_str());
											SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
											if(_tcscmp( v3.second.get("GestureName",_T("")).c_str(),szGestureName ) == 0)
												break;
										}
									}
								}
								SendMessage(hWndCB, CB_SETCURSEL, lastmatchindex, 0);
								EnableWindow(GetDlgItem(hDlg , IDC_CFG_DELG),(g_Points.size() > 0));	
								EnableWindow(GetDlgItem(hDlg , IDC_CFG_RENG),(g_Points.size() > 0));
								EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),bActionMatchFound);
								EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),bActionMatchFound);
								std::basic_string<TCHAR> sLua = v3.second.get("Lua",_T(""));
								boost::replace_all(sLua, _T("\n"), _T(""));
								boost::replace_all(sLua, _T("\r"), _T("\r\n"));
								SendEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)sLua.c_str());
							}
						}
					}
				}
			}
		}
	}
	bAppMatchFound = FALSE; //probably not needed

	if(ptPreviewTL.x = 65536){
		GetWindowRect(GetDlgItem(hDlg, IDC_PREVIEW),&lpPreview);
		ptPreviewTL.x = lpPreview.left;
		ptPreviewTL.y = lpPreview.top;
		ptPreviewTR.x = lpPreview.right;
		ptPreviewTR.y = lpPreview.bottom;
		ScreenToClient(hDlg, &ptPreviewTL);
		ScreenToClient(hDlg, &ptPreviewTR);
	}
	drawConfigGesture(hDlg);

	return TRUE;
}

BOOL SaveConfigChange(HWND hDlg, HTREEITEM hSelected){
	
	if(hSelected == NULL)
		return TRUE;

	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	tvi.hItem=hSelected;
	char cText[255] = {0};
    tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tvi);
	if(tvi.cChildren == 1) {
		//Application
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
			//each app
			if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0)
			{
				CHAR szOwnerClassName[256];
				CHAR szOwnerClassNameRegex[256];
				CHAR szOwnerTitle[256];
				CHAR szOwnerTitleRegex[256];
				CHAR szParentClassName[256];
				CHAR szParentClassNameRegex[256];
				CHAR szParentTitle[256];
				CHAR szParentTitleRegex[256];
				CHAR szControlClassName[256];
				CHAR szControlClassNameRegex[256];
				CHAR szControlTitle[256];
				CHAR szControlTitleRegex[256];
				CHAR szFileName[_MAX_PATH];
				CHAR szFileNameRegex[_MAX_PATH];
				CHAR szPathRegex[_MAX_PATH];
				memset(szOwnerClassName,0,sizeof(szOwnerClassName));
				memset(szOwnerClassNameRegex,0,sizeof(szOwnerClassNameRegex));
				memset(szOwnerTitle,0,sizeof(szOwnerTitle));
				memset(szOwnerTitleRegex,0,sizeof(szOwnerTitleRegex));
				memset(szParentClassName,0,sizeof(szParentClassName));
				memset(szParentClassNameRegex,0,sizeof(szParentClassNameRegex));
				memset(szParentTitle,0,sizeof(szParentTitle));
				memset(szParentTitleRegex,0,sizeof(szParentTitleRegex));
				memset(szControlClassName,0,sizeof(szControlClassName));
				memset(szControlClassNameRegex,0,sizeof(szControlClassNameRegex));
				memset(szControlTitle,0,sizeof(szControlTitle));
				memset(szControlTitleRegex,0,sizeof(szControlTitleRegex));
				memset(szFileName,0,sizeof(szFileName));
				memset(szFileNameRegex,0,sizeof(szFileNameRegex));
				memset(szPathRegex,0,sizeof(szPathRegex));
				GetDlgItemText(hDlg, IDC_EDITCFG_OWNERTITLE, szOwnerTitle, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_OWNERTITLE_REGEX, szOwnerTitleRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_OWNERCLASS, szOwnerClassName, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_OWNERCLASS_REGEX, szOwnerClassNameRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_PARENTTITLE, szParentTitle, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_PARENTTITLE_REGEX, szParentTitleRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_PARENTCLASS, szParentClassName, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX, szParentClassNameRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_CONTROLTITLE, szControlTitle, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX, szControlTitleRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_CONTROLCLASS, szControlClassName, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX, szControlClassNameRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_FILENAME, szFileName, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_FILENAME_REGEX, szFileNameRegex, 255);
				GetDlgItemText(hDlg, IDC_EDITCFG_PATH_REGEX, szPathRegex, 255);
				v2.second.put("MatchCriteria.ownerclass", szOwnerClassName);
				if(szOwnerClassNameRegex[0]==_T('*')) szOwnerClassNameRegex[0]=_T('.');
				v2.second.put("MatchCriteria.ownerclassregex", szOwnerClassNameRegex);
				v2.second.put("MatchCriteria.ownertitle", szOwnerTitle);
				if(szOwnerTitleRegex[0]==_T('*')) szOwnerTitleRegex[0]=_T('.');
				v2.second.put("MatchCriteria.ownertitleregex", szOwnerTitleRegex);
				v2.second.put("MatchCriteria.parentclass", szParentClassName);
				if(szParentClassNameRegex[0]==_T('*')) szParentClassNameRegex[0]=_T('.');
				v2.second.put("MatchCriteria.parentclassregex", szParentClassNameRegex);
				v2.second.put("MatchCriteria.parenttitle", szParentTitle);
				if(szParentTitleRegex[0]==_T('*')) szParentTitleRegex[0]=_T('.');
				v2.second.put("MatchCriteria.parenttitleregex", szParentTitleRegex);
				v2.second.put("MatchCriteria.controlclass", szControlClassName);
				if(szControlClassNameRegex[0]==_T('*')) szControlClassNameRegex[0]=_T('.');
				v2.second.put("MatchCriteria.controlclassregex", szControlClassNameRegex);
				v2.second.put("MatchCriteria.controltitle", szControlTitle);
				if(szControlTitleRegex[0]==_T('*')) szControlTitleRegex[0]=_T('.');
				v2.second.put("MatchCriteria.controltitleregex", szControlTitleRegex);
				int iControlID = GetDlgItemInt(hDlg, IDC_EDITCFG_CONTROLID, NULL, FALSE);
				if(iControlID==0){
					v2.second.put("MatchCriteria.controlid", _T(""));
				} else {
					v2.second.put("MatchCriteria.controlid", iControlID);
				}
				v2.second.put("MatchCriteria.file", szFileName);
				if(szFileNameRegex[0]==_T('*')) szFileNameRegex[0]=_T('.');
				v2.second.put("MatchCriteria.fileregex", szFileNameRegex);
				if(szPathRegex[0]==_T('*')) szPathRegex[0]=_T('.');
				v2.second.put("MatchCriteria.pathregex", szPathRegex);
				v2.second.put("<xmlattr>.noglobal", SendDlgItemMessage(hDlg, IDC_CHK_NOGLOBAL, BM_GETCHECK, 0, 0));
				v2.second.put("<xmlattr>.enabled", SendDlgItemMessage(hDlg, IDC_CHKAPPACTIVE, BM_GETCHECK, 0, 0));
			}
		}

	} else {
		//Action

		TVITEM tvi;
		ZeroMemory(&tvi,sizeof(tvi));
		tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
		tvi.hItem=hSelected;
		TCHAR cText[255] = {0};
		tvi.pszText = cText;
		tvi.cchTextMax=256;
		SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tvi);

		HTREEITEM hSelectedParent = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
		TVITEM parenttvi;
		ZeroMemory(&parenttvi,sizeof(parenttvi));
		parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		parenttvi.hItem=hSelectedParent;
		TCHAR cParentText[255] = {0};
		parenttvi.pszText = cParentText;
		parenttvi.cchTextMax=256;
		SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&parenttvi);
		BOOL bAppMatch = FALSE;

		CHAR szGestureName[255];
		memset(szGestureName,0,sizeof(szGestureName));
		int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
		SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);

		//If global, check global actions for conflicts
		if(_tcsicmp( cParentText, ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str() ) == 0){
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, ptConfig.get_child(_T("config.Global"))) {
				//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
				if(v3.first == "Actions"){
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v4, v3.second) {
						//each action
						if(_tcsicmp( cText, v4.second.get("<xmlattr>.description",_T("")).c_str() ) != 0 && v4.second.get("<xmlattr>.enabled",1) == 1) {
							if(_tcscmp( szGestureName, v4.second.get("GestureName",_T("")).c_str() ) == 0) {
								int iStrokeState = (SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_GETCHECK, 0, 0) ? 0 : (SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_GETCHECK, 0, 0) ? 1 : (SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_GETCHECK, 0, 0) ? 2 : 0)));
								bool bStrokeStateMatch = (v4.second.get("StrokeState",0) == 0 ? true : (iStrokeState == 0 ? true : (v4.second.get("StrokeState",0) == iStrokeState)));
								if(!bCaptureModifiersOnMouseDown){
									bStrokeStateMatch = true;
								}
								if((v4.second.get("Control",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_GETCHECK, 0, 0)
									&& v4.second.get("Alt",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_GETCHECK, 0, 0)
									&& v4.second.get("Shift",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_GETCHECK, 0, 0)
									&& v4.second.get("Left",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_GETCHECK, 0, 0)
									&& v4.second.get("Middle",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_GETCHECK, 0, 0)
									&& v4.second.get("Right",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_GETCHECK, 0, 0)
									&& v4.second.get("X1",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_GETCHECK, 0, 0)
									&& v4.second.get("X2",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_GETCHECK, 0, 0)
									&& v4.second.get("WheelDown",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_GETCHECK, 0, 0)
									&& v4.second.get("WheelUp",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_GETCHECK, 0, 0)
									&& bStrokeStateMatch
									) && 
									(v4.second.get("Control",FALSE) == TRUE || v4.second.get("Alt",FALSE) == TRUE || v4.second.get("Shift",FALSE) == TRUE
									|| v4.second.get("Left",FALSE) == TRUE || v4.second.get("Middle",FALSE) == TRUE || v4.second.get("Right",FALSE) == TRUE
									|| v4.second.get("X1",FALSE) == TRUE || v4.second.get("X2",FALSE) == TRUE || v4.second.get("WheelDown",FALSE) == TRUE
									|| v4.second.get("WheelUp",FALSE) == TRUE || v4.second.get("GestureName",_T("")).length() > 0)) 
								{
									string sTemp = _T("");
									int iResp = 0;
									iResp = MessageBox(hDlg, sTemp.append(
										ptLang.get("language.Actions.Messages.ActionConflict",_T("The following action already has the same gesture/modifiers defined:")).append(_T("\n\n")).append(cParentText).append(_T(": ")).append(v4.second.get("<xmlattr>.description",_T(""))).append(_T("\n\n")).append(ptLang.get("language.Actions.Messages.ActionConflictSave",_T("Save anyway?")))).c_str()
										, ptLang.get("language.Actions.Messages.ActionConflictTitle",_T("Action Conflict")).c_str()
										, MB_YESNOCANCEL + MB_ICONWARNING + MB_APPLMODAL);
									if(iResp == IDNO){
										return TRUE;
									} else if(iResp == IDCANCEL){
										return FALSE;
									}
								}										
							}
						}
					}
				}
			}
		} else {
			//Check for conflicts only with the selected application
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, ptConfig.get_child("config.Applications")) {
				//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
				if(_tcsicmp( cParentText, v3.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 && v3.second.get("<xmlattr>.enabled",1) == 1) {
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v5, v3.second) {
						if(v5.first == "Actions"){
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v4, v5.second) {
								//each action
								if(_tcsicmp( cText, v4.second.get("<xmlattr>.description",_T("")).c_str() ) != 0) {
									if(_tcscmp( szGestureName, v4.second.get("GestureName",_T("")).c_str() ) == 0) {
										int iStrokeState = (SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_GETCHECK, 0, 0) ? 0 : (SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_GETCHECK, 0, 0) ? 1 : (SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_GETCHECK, 0, 0) ? 2 : 0)));
										bool bStrokeStateMatch = (v4.second.get("StrokeState",0) == 0 ? true : (iStrokeState == 0 ? true : (v4.second.get("StrokeState",0) == iStrokeState)));
										if(!bCaptureModifiersOnMouseDown){
											bStrokeStateMatch = true;
										}
										if((v4.second.get("Control",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_GETCHECK, 0, 0)
											&& v4.second.get("Alt",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_GETCHECK, 0, 0)
											&& v4.second.get("Shift",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_GETCHECK, 0, 0)
											&& v4.second.get("Left",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_GETCHECK, 0, 0)
											&& v4.second.get("Middle",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_GETCHECK, 0, 0)
											&& v4.second.get("Right",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_GETCHECK, 0, 0)
											&& v4.second.get("X1",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_GETCHECK, 0, 0)
											&& v4.second.get("X2",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_GETCHECK, 0, 0)
											&& v4.second.get("WheelDown",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_GETCHECK, 0, 0)
											&& v4.second.get("WheelUp",FALSE) == SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_GETCHECK, 0, 0)
											&& bStrokeStateMatch
											) && 
											(v4.second.get("Control",FALSE) == TRUE || v4.second.get("Alt",FALSE) == TRUE || v4.second.get("Shift",FALSE) == TRUE
											|| v4.second.get("Left",FALSE) == TRUE || v4.second.get("Middle",FALSE) == TRUE || v4.second.get("Right",FALSE) == TRUE
											|| v4.second.get("X1",FALSE) == TRUE || v4.second.get("X2",FALSE) == TRUE || v4.second.get("WheelDown",FALSE) == TRUE
											|| v4.second.get("WheelUp",FALSE) == TRUE  || v4.second.get("GestureName",_T("")).length() > 0))
										{
											string sTemp = _T("");
											int iResp = 0;
											iResp = MessageBox(hDlg, sTemp.append(
												ptLang.get("language.Actions.Messages.ActionConflict",_T("The following action already has the same gesture/modifiers defined:")).append(_T("\n\n")).append(cParentText).append(_T(": ")).append(v4.second.get("<xmlattr>.description",_T(""))).append(_T("\n\n")).append(ptLang.get("language.Actions.Messages.ActionConflictSave",_T("Save anyway?")))).c_str()
												, ptLang.get("language.Actions.Messages.ActionConflictTitle",_T("Action Conflict")).c_str()
												, MB_YESNOCANCEL + MB_ICONWARNING + MB_APPLMODAL);
											if(iResp == IDNO){
												return TRUE;
											} else if(iResp == IDCANCEL){
												return FALSE;
											}
										}										
									}
								}
							}
						}
					}
				}
			}
		}


		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
			//each app
			if(_tcsicmp( cParentText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
					//each action
					if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
						v3.second.put("Right",SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_GETCHECK, 0, 0));
						v3.second.put("Middle",SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_GETCHECK, 0, 0));
						v3.second.put("Left",SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_GETCHECK, 0, 0));
						v3.second.put("X1",SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_GETCHECK, 0, 0));
						v3.second.put("X2",SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_GETCHECK, 0, 0));
						v3.second.put("WheelUp",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_GETCHECK, 0, 0));
						v3.second.put("WheelDown",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_GETCHECK, 0, 0));
						v3.second.put("Control",SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_GETCHECK, 0, 0));
						v3.second.put("Alt",SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_GETCHECK, 0, 0));
						v3.second.put("Shift",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_GETCHECK, 0, 0));
						int iStrokeState = (SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_GETCHECK, 0, 0) ? 0 : (SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_GETCHECK, 0, 0) ? 1 : (SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_GETCHECK, 0, 0) ? 2 : 0)));
						v3.second.put("StrokeState",iStrokeState);
						v3.second.put("<xmlattr>.enabled", SendDlgItemMessage(hDlg, IDC_CHKACTACTIVE, BM_GETCHECK, 0, 0));
						TCHAR szLua[8192];
						memset(szLua,0,sizeof(szLua));
						UINT uSize = SendEditor( SCI_GETLENGTH, 0, 0L );
						SendEditor( SCI_GETTEXT, uSize + 1, (LPARAM)szLua );
						v3.second.put("GestureName",szGestureName);
						v3.second.put("Lua",szLua);
						
						TVITEM tvi;
						ZeroMemory(&tvi,sizeof(tvi));
						tvi.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
						tvi.hItem=hSelected;
						TCHAR cText[255] = {0};
						tvi.pszText = cText;
						tvi.cchTextMax=256;
						SendMessage(GetDlgItem(hDlg, IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

						for(int i = 0; i < GestureNames.size(); i++) { 
							if(_tcscmp( GestureNames[i].c_str(), szGestureName ) == 0 ){
								tvi.iImage = i;
								tvi.iSelectedImage = i;
								break;
							}
						}
						TreeView_SetItem(GetDlgItem(hDlg, IDC_TREEAPPS), &tvi);

						bAppMatch = TRUE;
					}
				}
			}
		}
		if(!bAppMatch){
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Actions")) {
				//each global action
				if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
					v2.second.put("Right",SendDlgItemMessage(hDlg, IDC_CHK_CFG_RIGHTBTN, BM_GETCHECK, 0, 0));
					v2.second.put("Middle",SendDlgItemMessage(hDlg, IDC_CHK_CFG_MIDDLEBTN, BM_GETCHECK, 0, 0));
					v2.second.put("Left",SendDlgItemMessage(hDlg, IDC_CHK_CFG_LEFTBTN, BM_GETCHECK, 0, 0));
					v2.second.put("X1",SendDlgItemMessage(hDlg, IDC_CHK_CFG_X1BTN, BM_GETCHECK, 0, 0));
					v2.second.put("X2",SendDlgItemMessage(hDlg, IDC_CHK_CFG_X2BTN, BM_GETCHECK, 0, 0));
					v2.second.put("WheelUp",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLUP, BM_GETCHECK, 0, 0));
					v2.second.put("WheelDown",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SCROLLDOWN, BM_GETCHECK, 0, 0));
					v2.second.put("Control",SendDlgItemMessage(hDlg, IDC_CHK_CFG_CTRL, BM_GETCHECK, 0, 0));
					v2.second.put("Alt",SendDlgItemMessage(hDlg, IDC_CHK_CFG_ALT, BM_GETCHECK, 0, 0));
					v2.second.put("Shift",SendDlgItemMessage(hDlg, IDC_CHK_CFG_SHIFT, BM_GETCHECK, 0, 0));
					int iStrokeState = (SendDlgItemMessage(hDlg, IDC_R_EITHER, BM_GETCHECK, 0, 0) ? 0 : (SendDlgItemMessage(hDlg, IDC_R_BEFORE, BM_GETCHECK, 0, 0) ? 1 : (SendDlgItemMessage(hDlg, IDC_R_AFTER, BM_GETCHECK, 0, 0) ? 2 : 0)));
					v2.second.put("StrokeState",iStrokeState);
					v2.second.put("<xmlattr>.enabled", SendDlgItemMessage(hDlg, IDC_CHKACTACTIVE, BM_GETCHECK, 0, 0));
					TCHAR szGestureName[255];
					TCHAR szLua[8192];
					memset(szGestureName,0,sizeof(szGestureName));
					memset(szLua,0,sizeof(szLua));
					int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
					SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);
					UINT uSize = SendEditor( SCI_GETLENGTH, 0, 0L );
					SendEditor( SCI_GETTEXT, uSize + 1, (LPARAM)szLua );
					v2.second.put("GestureName",szGestureName);
					v2.second.put("Lua",szLua);

					TVITEM tvi;
					ZeroMemory(&tvi,sizeof(tvi));
					tvi.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tvi.hItem=hSelected;
					char cText[255] = {0};
					tvi.pszText = cText;
					tvi.cchTextMax=256;
					SendMessage(GetDlgItem(hDlg, IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

					for(int i = 0; i < GestureNames.size(); i++) { 
						if(_tcscmp( GestureNames[i].c_str(), szGestureName ) == 0 ){
							tvi.iImage = i;
							tvi.iSelectedImage = i;
							break;
						}
					}
					TreeView_SetItem(GetDlgItem(hDlg, IDC_TREEAPPS), &tvi);
				}
			}
		}
	}

	return TRUE;
}

void ResizeConfigWindow(HWND hDlg){

	RECT lpRect;
	RECT lpWindowRect;
	POINT lpPoint;
	GetWindowRect(hDlg, &lpWindowRect);
			
	GetWindowRect(GetDlgItem(hDlg , IDC_TREEAPPS), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_TREEAPPS), NULL, 0, 0, lpRect.right - lpRect.left, lpWindowRect.bottom-lpWindowRect.top-42, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CHKAPPACTIVE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CHKAPPACTIVE), NULL, (lpWindowRect.right - lpWindowRect.left) - (lpRect.right - lpRect.left) - 10, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 6, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CHKACTACTIVE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CHKACTACTIVE), NULL, (lpWindowRect.right - lpWindowRect.left) - (lpRect.right - lpRect.left) - 10, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 6, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_MODTEXT), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_MODTEXT), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpRect.bottom-lpRect.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	if(IsWindowVisible(GetDlgItem(hDlg , IDC_MODTEXT))){
		ShowWindow(GetDlgItem(hDlg , IDC_MODTEXT),SW_HIDE);
		ShowWindow(GetDlgItem(hDlg , IDC_MODTEXT),SW_SHOW);
	}

	GetWindowRect(GetDlgItem(hDlg , IDC_CFG_APP_TEXT), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CFG_APP_TEXT), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 30, lpRect.bottom-lpRect.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	if(IsWindowVisible(GetDlgItem(hDlg , IDC_CFG_APP_TEXT))){
		ShowWindow(GetDlgItem(hDlg , IDC_CFG_APP_TEXT),SW_HIDE);
		ShowWindow(GetDlgItem(hDlg , IDC_CFG_APP_TEXT),SW_SHOW);
	}

	GetWindowRect(GetDlgItem(hDlg , IDC_CFGBTNADDAPP), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CFGBTNADDAPP), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CFGBTNADDACTION), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CFGBTNADDACTION), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CFGBTNRENAME), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CFGBTNRENAME), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CFGBTNDELETE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CFGBTNDELETE), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_EDITLUA), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_EDITLUA), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-lpPoint.y-30, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	SetWindowPos(m_hwndEditor, NULL, lpPoint.x, lpPoint.y, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-lpPoint.y-30, SWP_NOZORDER | SWP_NOOWNERZORDER);

}

void ShowActionTreePopupMenu(POINT pt) {

	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	tvi.hItem=itemUnderMouse;
	char cText[255] = {0};
    tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tvi);
	HMENU hPopupMenu = CreatePopupMenu();
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (_tcscmp( tvi.pszText, ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str()) == 0 ? MF_DISABLED : 0), ID_TVPOPUP_DELETE, ptLang.get("language.Actions.FormButtons.Delete",_T("Delete")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (_tcscmp( tvi.pszText, ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str()) == 0 ? MF_DISABLED : 0), ID_TVPOPUP_RENAME, ptLang.get("language.Actions.FormButtons.Rename",_T("Rename")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (rightClickCopyNode == NULL || tvi.cChildren == 0 ? MF_DISABLED : 0), ID_TVPOPUP_PASTE, ptLang.get("language.Menu.Paste",_T("Paste")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (tvi.cChildren == 1 ? MF_DISABLED : 0), ID_TVPOPUP_COPY, ptLang.get("language.Menu.Copy",_T("Copy")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (tvi.cChildren == 1 ? MF_DISABLED : 0), ID_TVPOPUP_CUT, ptLang.get("language.Menu.Cut",_T("Cut")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_TVPOPUP_ADDACTION, ptLang.get("language.Actions.AddActionPopupTitle",_T("Add Action")).c_str());
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_TVPOPUP_ADDAPP, ptLang.get("language.Actions.AddAppPopupTitle",_T("Add App")).c_str());
	
	TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWndConfig, NULL);
	PostMessage(hWndConfig, WM_NULL, 0, 0);
}

void PasteAction() {

	if(rightClickCopyNode == NULL)
		return;

	//Copy/paste node in rightClickCopyNode to itemUnderMouse
	//get parent to find app (or global) from rightClickCopyNode, then find action based on node's caption
	//Check for name collisions, append incremented text description
	//create new node under itemUnderMouse (app/global) in tree and in ptConfig
	//select pasted node

	//TODO: Not currently getting the rightClickCopyNode, needs more work, and some LOGMESSAGE calls

	boost::property_tree::ptree child;
	boost::property_tree::ptree match;

	BOOL bOriginalAppFound = FALSE;
	BOOL bOriginalActionFound = FALSE;
	BOOL bDestinationAppFound = FALSE;

	std::string orgDescription = "";
	int orgEnabled = 0;
	std::string orgGestureName = "";
	int orgControl = 0;
	int orgAlt = 0;
	int orgShift = 0;
	int orgRight = 0;
	int orgMiddle = 0;
	int orgLeft = 0;
	int orgX1 = 0;
	int orgX2 = 0;
	int orgWheelUp = 0;
	int orgWheelDown = 0;
	int orgStrokeState = 0;
	std::string orgLua = "";

	TVITEM tviOriginal;
	ZeroMemory(&tviOriginal,sizeof(tviOriginal));
	tviOriginal.mask=TVIF_TEXT | TVIF_CHILDREN;
	tviOriginal.hItem=rightClickCopyNode;
	char cOriginalText[255] = {0};
	tviOriginal.pszText = cOriginalText;
	tviOriginal.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tviOriginal);
	std::string sOriginalName = tviOriginal.pszText;

	TVITEM tviOriginalParent;
	HTREEITEM hOriginalParent = TreeView_GetParent(GetDlgItem(hWndConfig , IDC_TREEAPPS),rightClickCopyNode);
	ZeroMemory(&tviOriginalParent,sizeof(tviOriginalParent));
	tviOriginalParent.mask=TVIF_TEXT | TVIF_CHILDREN;
	tviOriginalParent.hItem=hOriginalParent;
	char cOriginalParentText[255] = {0};
	tviOriginalParent.pszText = cOriginalParentText;
	tviOriginalParent.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tviOriginalParent);	
	std::string sOriginalParentName = tviOriginalParent.pszText;

	TVITEM tviDestination;
	ZeroMemory(&tviDestination,sizeof(tviDestination));
	tviDestination.mask=TVIF_TEXT | TVIF_CHILDREN;
	tviDestination.hItem=itemUnderMouse;
	char cDestinationText[255] = {0};
	tviDestination.pszText = cDestinationText;
	tviDestination.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,0,(LPARAM)&tviDestination);
	std::string sDestinationName = tviDestination.pszText;

	if(tviDestination.cChildren == 1) 
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
			//Loop through each defined Application to find the parent of the copied action
			if(_tcsicmp( sOriginalParentName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
			{
				bOriginalAppFound = TRUE; //Found copied node's parent
	
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("Actions"))) {
					//Loop through each Action within the Application to load the copied node's values
					if(_tcsicmp( sOriginalName.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0)
					{
						bOriginalActionFound = true;

						orgDescription = v3.second.get("<xmlattr>.description",_T(""));
						orgEnabled = v3.second.get("<xmlattr>.enabled",0);
						orgGestureName = v3.second.get("GestureName",_T(""));
						orgControl = v3.second.get("Control",0);
						orgAlt = v3.second.get("Alt",0);
						orgShift = v3.second.get("Shift",0);
						orgRight = v3.second.get("Right",0);
						orgLeft = v3.second.get("Left",0);
						orgMiddle = v3.second.get("Middle",0);
						orgX1 = v3.second.get("X1",0);
						orgX2 = v3.second.get("X2",0);
						orgWheelUp = v3.second.get("WheelUp",0);
						orgWheelDown = v3.second.get("WheelDown",0);
						orgStrokeState = v3.second.get("StrokeState",0);
						orgLua = v3.second.get("Lua",_T(""));
					}
				}
			}
		}
		if(!bOriginalAppFound)
		{
			//Parent (Application) of copied node wasn't found, so loop through Global Actions
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global"))) {
				if(v2.first == "Actions"){
					bOriginalAppFound = TRUE;

					BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
						//Loop through each action to find copied node
						if(_tcsicmp( sOriginalName.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0)
						{
							bOriginalActionFound = true;
				
							orgDescription = v3.second.get("<xmlattr>.description",_T(""));
							orgEnabled = v3.second.get("<xmlattr>.enabled",0);
							orgGestureName = v3.second.get("GestureName",_T(""));
							orgControl = v3.second.get("Control",0);
							orgAlt = v3.second.get("Alt",0);
							orgShift = v3.second.get("Shift",0);
							orgRight = v3.second.get("Right",0);
							orgLeft = v3.second.get("Left",0);
							orgMiddle = v3.second.get("Middle",0);
							orgX1 = v3.second.get("X1",0);
							orgX2 = v3.second.get("X2",0);
							orgWheelUp = v3.second.get("WheelUp",0);
							orgWheelDown = v3.second.get("WheelDown",0);
							orgStrokeState = v3.second.get("StrokeState",0);
							orgLua = v3.second.get("Lua",_T(""));	
						}
					}
				}
			}
		}

		if(bOriginalActionFound) 
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
				//each app
				if(_tcsicmp( sDestinationName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
				{
					bDestinationAppFound = TRUE;
					BOOL bCollision = FALSE;
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
						//Loop through each action to check for naming conflicts, if matching action name found, append to name
						if(_tcsicmp( sOriginalName.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
							bCollision = TRUE;
							orgDescription.append(" (Copy)");
							break;
						}
					}
					if(bCollision)
					{
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
							//Loop through each action to check for naming conflicts, if matching action name found, , display error and exit
							if(_tcsicmp( orgDescription.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
								MessageBox(hWndSettings, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
								return;
							}
						}
					}

					child.put("<xmlattr>.description",orgDescription);
					child.put("<xmlattr>.enabled",orgEnabled);
					child.put("GestureName", orgGestureName);
					child.put("Control", orgControl);
					child.put("Alt", orgAlt);
					child.put("Shift", orgShift);
					child.put("Right", orgRight);
					child.put("Middle", orgMiddle);
					child.put("Left", orgLeft);
					child.put("X1", orgX1);
					child.put("X2", orgX2);
					child.put("WheelUp", orgWheelUp);
					child.put("WheelDown", orgWheelDown);
					child.put("StrokeState", orgStrokeState);
					child.put("Lua", orgLua);
					v2.second.get_child("Actions").add_child("Action", child);
				}
			}
			if(!bDestinationAppFound)
			{   //Didn't find App, copy to Global Actions
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global"))) {
					if(v2.first == "Actions"){
						bDestinationAppFound = TRUE;

						BOOL bCollision = FALSE;
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
							//Loop through each action to check for naming conflicts, if matching action name found, append to name
							if(_tcsicmp( sOriginalName.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
								bCollision = TRUE;
								orgDescription.append(" (Copy)");
								break;
							}
						}
						if(bCollision)
						{
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
								//Loop through each action to check for naming conflicts, if matching action name found, display error and exit
								if(_tcsicmp( orgDescription.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
									MessageBox(hWndSettings, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return;
								}
							}
						}
					}
				}
				if(bDestinationAppFound) 
				{
					child.put("<xmlattr>.description",orgDescription);
					child.put("<xmlattr>.enabled",orgEnabled);
					child.put("GestureName", orgGestureName);
					child.put("Control", orgControl);
					child.put("Alt", orgAlt);
					child.put("Shift", orgShift);
					child.put("Right", orgRight);
					child.put("Middle", orgMiddle);
					child.put("Left", orgLeft);
					child.put("X1", orgX1);
					child.put("X2", orgX2);
					child.put("WheelUp", orgWheelUp);
					child.put("WheelDown", orgWheelDown);
					child.put("StrokeState", orgStrokeState);
					child.put("Lua", orgLua);
					//v2.second.add_child("Action", child);
					ptConfig.add_child("config.Global.Actions.Action", child);
				}
			}

			TV_INSERTSTRUCT tvis ;
			ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
			tvis.hParent = itemUnderMouse;
			tvis.hInsertAfter = TVI_LAST;
			tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvis.item.cchTextMax = 256 ;
			TCHAR sDesc[255];
			_tcscpy (sDesc,orgDescription.c_str());
			TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
			tvis.item.pszText = cDesc;
			tvis.item.lParam =(LPARAM) sDesc;
			tvis.item.cChildren = 0 ;
			for(int i = 0; i < GestureNames.size(); i++) { 
				if(_tcscmp( GestureNames[i].c_str(), orgGestureName.c_str() ) == 0 ){
					tvis.item.iImage = i;
					tvis.item.iSelectedImage = i;
					break;
				}
			}
			HTREEITEM hNewItem = TreeView_InsertItem (GetDlgItem(hWndConfig , IDC_TREEAPPS), &tvis) ;
			TreeView_SelectItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),hNewItem);
		}
	}
	return;
}

void RemoveAction(HTREEITEM copyNode) {
	if(copyNode == NULL)
		return;

	HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT;
	tvi.hItem=copyNode;
	char cText[255] = {0};
	tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

	BOOL bAppMatch = FALSE;

	HTREEITEM hSelectedParent = TreeView_GetParent(GetDlgItem(hWndConfig , IDC_TREEAPPS),copyNode);
	TVITEM parenttvi;
	ZeroMemory(&parenttvi,sizeof(parenttvi));
	parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	parenttvi.hItem=hSelectedParent;
	char cParentText[255] = {0};
	parenttvi.pszText = cParentText;
	parenttvi.cchTextMax=256;
	SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&parenttvi);

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
		//each ignored window
		if(_tcsicmp( cParentText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
				//each ignored window
				if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1*/) {
					v3.second.clear();
					TreeView_DeleteItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),copyNode);
					bAppMatch = TRUE;
				}
			}
		}
	}

	if(!bAppMatch) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Actions")) {
			//each app
			if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
				v2.second.clear();
				TreeView_DeleteItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),copyNode);
			}
		}						
	}
}

#pragma endregion Actions Window Functions


#pragma region Hotkey Tab Functions

void ResizeHotkeyWindow(HWND hDlg){

	//MessageBox(GetForegroundWindow(),"ResizeHotkeyWindow() : Begin", "Hotkey", NULL);

	RECT lpRect;
	RECT lpWindowRect;
	POINT lpPoint;
	GetWindowRect(hDlg, &lpWindowRect);
			
	GetWindowRect(GetDlgItem(hDlg , IDC_TREEHOTKEYS), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_TREEHOTKEYS), NULL, 0, 0, lpRect.right - lpRect.left, lpWindowRect.bottom-lpWindowRect.top-42, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);

	GetWindowRect(GetDlgItem(hDlg , IDC_CHKHKACTIVE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_CHKHKACTIVE), NULL, (lpWindowRect.right - lpWindowRect.left) - (lpRect.right - lpRect.left) - 10, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 6, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
	/*
	GetWindowRect(GetDlgItem(hDlg , IDC_MODTEXT), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_MODTEXT), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpRect.bottom-lpRect.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	if(IsWindowVisible(GetDlgItem(hDlg , IDC_MODTEXT))){
		ShowWindow(GetDlgItem(hDlg , IDC_MODTEXT),SW_HIDE);
		ShowWindow(GetDlgItem(hDlg , IDC_MODTEXT),SW_SHOW);
	}
	*/

	GetWindowRect(GetDlgItem(hDlg , IDC_HKBTNADDHOTKEY), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_HKBTNADDHOTKEY), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_HKBTNRENAME), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_HKBTNRENAME), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_HKBTNDELETE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_HKBTNDELETE), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_EDITHKLUA), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_EDITHKLUA), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-lpPoint.y-30, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	SetWindowPos(m_hwndHotkeyEditor, NULL, lpPoint.x, lpPoint.y, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-lpPoint.y-30, SWP_NOZORDER | SWP_NOOWNERZORDER);

}

BOOL BuildHotkeyTree(HWND hDlg)
{
		TreeView_DeleteAllItems(GetDlgItem(hDlg,IDC_TREEHOTKEYS));

		TV_INSERTSTRUCT tvis ;
		HTREEITEM hLastSelected = NULL;
		
		// Initialize list view item structure
		ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
		tvis.hParent = NULL ;
		tvis.hInsertAfter = TVI_SORT;
		tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
		tvis.item.cchTextMax = 256 ;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global.Hotkeys"))) {
			//if(v2.first == "Hotkeys"){
				//each hotkey
				//MessageBox(GetForegroundWindow(), v2.second.get("<xmlattr>.description",_T("")).c_str(), "Hotkey", NULL);
				if(v2.second.get("<xmlattr>.description",_T("")).length() > 0 /*&& v2.second.get("<xmlattr>.enabled",0) == 1*/){
					TCHAR sDesc[255];
					HTREEITEM hParent;
					tvis.hParent = NULL;
					_tcscpy (sDesc,v2.second.get("<xmlattr>.description",_T("Unknown")).c_str());
					char *cDesc = const_cast<char*> ( sDesc );
					tvis.item.pszText = cDesc;
					tvis.item.lParam =(LPARAM) sDesc;
					tvis.item.cChildren = 0 ;
					hParent = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEHOTKEYS), &tvis) ;
					if(_tcsicmp( sLastHotkey.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str()) == 0) {
						hLastSelected = hParent;
					}
				}
			//}
		}
		if(hLastSelected == NULL){
			TreeView_Select( GetDlgItem(hDlg , IDC_TREEHOTKEYS), TreeView_GetFirstVisible(GetDlgItem(hDlg , IDC_TREEHOTKEYS)), TVGN_CARET);
		} else {
			TreeView_Select( GetDlgItem(hDlg , IDC_TREEHOTKEYS), hLastSelected, TVGN_CARET);
		}

		return TRUE;
}

BOOL LoadHotkeyVectors(){

	HotkeyNames.clear();
	HotkeyValues.clear();

	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.None",_T("None")));
	HotkeyValues.push_back(0x0);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.0",_T("0")));
	HotkeyValues.push_back(0x30);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.1",_T("1")));
	HotkeyValues.push_back(0x31);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.2",_T("2")));
	HotkeyValues.push_back(0x32);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.3",_T("3")));
	HotkeyValues.push_back(0x33);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.4",_T("4")));
	HotkeyValues.push_back(0x34);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.5",_T("5")));
	HotkeyValues.push_back(0x35);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.6",_T("6")));
	HotkeyValues.push_back(0x36);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.7",_T("7")));
	HotkeyValues.push_back(0x37);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.8",_T("8")));
	HotkeyValues.push_back(0x38);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.9",_T("9")));
	HotkeyValues.push_back(0x39);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD0",_T("NUMPAD 0")));
	HotkeyValues.push_back(VK_NUMPAD0);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD1",_T("NUMPAD 1")));
	HotkeyValues.push_back(VK_NUMPAD1);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD2",_T("NUMPAD 2")));
	HotkeyValues.push_back(VK_NUMPAD2);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD3",_T("NUMPAD 3")));
	HotkeyValues.push_back(VK_NUMPAD3);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD4",_T("NUMPAD 4")));
	HotkeyValues.push_back(VK_NUMPAD4);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD5",_T("NUMPAD 5")));
	HotkeyValues.push_back(VK_NUMPAD5);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD6",_T("NUMPAD 6")));
	HotkeyValues.push_back(VK_NUMPAD6);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD7",_T("NUMPAD 7")));
	HotkeyValues.push_back(VK_NUMPAD7);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD8",_T("NUMPAD 8")));
	HotkeyValues.push_back(VK_NUMPAD8);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMPAD9",_T("NUMPAD 9")));
	HotkeyValues.push_back(VK_NUMPAD9);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Multiply",_T("Multiply")));
	HotkeyValues.push_back(VK_MULTIPLY);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Add",_T("Add")));
	HotkeyValues.push_back(VK_ADD);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Subtract",_T("Subtract")));
	HotkeyValues.push_back(VK_SUBTRACT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Decimal",_T("Decimal")));
	HotkeyValues.push_back(VK_DECIMAL);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Divide",_T("Divide")));
	HotkeyValues.push_back(VK_DIVIDE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.A",_T("A")));
	HotkeyValues.push_back(0x41);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.B",_T("B")));
	HotkeyValues.push_back(0x42);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.C",_T("C")));
	HotkeyValues.push_back(0x43);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.D",_T("D")));
	HotkeyValues.push_back(0x44);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.E",_T("E")));
	HotkeyValues.push_back(0x45);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F",_T("F")));
	HotkeyValues.push_back(0x46);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.G",_T("G")));
	HotkeyValues.push_back(0x47);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.H",_T("H")));
	HotkeyValues.push_back(0x48);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.I",_T("I")));
	HotkeyValues.push_back(0x49);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.J",_T("J")));
	HotkeyValues.push_back(0x4A);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.K",_T("K")));
	HotkeyValues.push_back(0x4B);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.L",_T("L")));
	HotkeyValues.push_back(0x4C);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.M",_T("M")));
	HotkeyValues.push_back(0x4D);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.N",_T("N")));
	HotkeyValues.push_back(0x4E);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.O",_T("O")));
	HotkeyValues.push_back(0x4F);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.P",_T("P")));
	HotkeyValues.push_back(0x50);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Q",_T("Q")));
	HotkeyValues.push_back(0x51);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.R",_T("R")));
	HotkeyValues.push_back(0x52);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.S",_T("S")));
	HotkeyValues.push_back(0x53);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.T",_T("T")));
	HotkeyValues.push_back(0x54);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.U",_T("U")));
	HotkeyValues.push_back(0x55);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.V",_T("V")));
	HotkeyValues.push_back(0x56);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.W",_T("W")));
	HotkeyValues.push_back(0x57);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.X",_T("X")));
	HotkeyValues.push_back(0x58);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Y",_T("Y")));
	HotkeyValues.push_back(0x59);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.Z",_T("Z")));
	HotkeyValues.push_back(0x5A);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F1",_T("F1")));
	HotkeyValues.push_back(VK_F1);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F2",_T("F2")));
	HotkeyValues.push_back(VK_F2);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F3",_T("F3")));
	HotkeyValues.push_back(VK_F3);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F4",_T("F4")));
	HotkeyValues.push_back(VK_F4);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F5",_T("F5")));
	HotkeyValues.push_back(VK_F5);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F6",_T("F6")));
	HotkeyValues.push_back(VK_F6);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F7",_T("F7")));
	HotkeyValues.push_back(VK_F7);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F8",_T("F8")));
	HotkeyValues.push_back(VK_F8);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F9",_T("F9")));
	HotkeyValues.push_back(VK_F9);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F10",_T("F10")));
	HotkeyValues.push_back(VK_F10);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F11",_T("F11")));
	HotkeyValues.push_back(VK_F11);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.F12",_T("F12")));
	HotkeyValues.push_back(VK_F12);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BACKSLASH",_T("BACKSLASH")));
	HotkeyValues.push_back(0xDC);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BACKSPACE",_T("BACKSPACE")));
	HotkeyValues.push_back(VK_BACK);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.CAPSLOCK", _T("CAPS LOCK")));
	HotkeyValues.push_back(VK_CAPITAL);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.COMMA",_T("COMMA")));
	HotkeyValues.push_back(0xBC);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.DASH",_T("DASH")));
	HotkeyValues.push_back(0xBD);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.DELETE",_T("DELETE")));
	HotkeyValues.push_back(VK_DELETE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.DOWNARROW",_T("DOWN ARROW")));
	HotkeyValues.push_back(VK_DOWN);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.END",_T("END")));
	HotkeyValues.push_back(VK_END);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.ENTER",_T("ENTER")));
	HotkeyValues.push_back(VK_RETURN);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.ESCAPE",_T("ESCAPE")));
	HotkeyValues.push_back(VK_ESCAPE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.GRAVE",_T("GRAVE/TILDE")));
	HotkeyValues.push_back(0xC0);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.HOME",_T("HOME")));
	HotkeyValues.push_back(VK_HOME);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.INSERT",_T("INSERT")));
	HotkeyValues.push_back(VK_INSERT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LEFTARROW",_T("LEFT ARROW")));
	HotkeyValues.push_back(VK_LEFT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LBRACE",_T("LEFT BRACE")));
	HotkeyValues.push_back(0xDB);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.NUMLOCK",_T("NUM LOCK")));
	HotkeyValues.push_back(VK_NUMLOCK);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.PAGEDOWN",_T("PAGE DOWN")));
	HotkeyValues.push_back(VK_NEXT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.PAGEUP",_T("PAGE UP")));
	HotkeyValues.push_back(VK_PRIOR);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.PAUSE",_T("PAUSE")));
	HotkeyValues.push_back(VK_PAUSE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.PERIOD",_T("PERIOD")));
	HotkeyValues.push_back(0xBE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.PRINTSCREEN",_T("PRINT SCREEN")));
	HotkeyValues.push_back(VK_SNAPSHOT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.QUOTE",_T("QUOTE")));
	HotkeyValues.push_back(0xDE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.RIGHTARROW",_T("RIGHT ARROW")));
	HotkeyValues.push_back(VK_RIGHT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.RBRACE",_T("RIGHT BRACE")));
	HotkeyValues.push_back(0xDD);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.SCROLLLOCK",_T("SCROLL LOCK")));
	HotkeyValues.push_back(VK_SCROLL);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.SLASH",_T("SLASH")));
	HotkeyValues.push_back(0xBF);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.SPACEBAR",_T("SPACEBAR")));
	HotkeyValues.push_back(VK_SPACE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.TAB",_T("TAB")));
	HotkeyValues.push_back(VK_TAB);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.UPARROW",_T("UP ARROW")));
	HotkeyValues.push_back(VK_UP);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_BACK",_T("BROWSER BACK")));
	HotkeyValues.push_back(VK_BROWSER_BACK);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_FORWARD", _T("BROWSER FORWARD")));
	HotkeyValues.push_back(VK_BROWSER_FORWARD);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_REFRESH", _T("BROWSER REFRESH")));
	HotkeyValues.push_back(VK_BROWSER_REFRESH);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_STOP", _T("BROWSER STOP")));
	HotkeyValues.push_back(VK_BROWSER_STOP);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_SEARCH", _T("BROWSER SEARCH")));
	HotkeyValues.push_back(VK_BROWSER_SEARCH);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_FAVORITES", _T("BROWSER FAVORITES")));
	HotkeyValues.push_back(VK_BROWSER_FAVORITES);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.BROWSER_HOME", _T("BROWSER HOME")));
	HotkeyValues.push_back(VK_BROWSER_HOME);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.VOLUME_MUTE", _T("VOLUME MUTE")));
	HotkeyValues.push_back(VK_VOLUME_MUTE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.VOLUME_DOWN", _T("VOLUME DOWN")));
	HotkeyValues.push_back(VK_VOLUME_DOWN);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.VOLUME_UP", _T("VOLUME UP")));
	HotkeyValues.push_back(VK_VOLUME_UP);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.MEDIA_NEXT_TRACK", _T("MEDIA NEXT TRACK")));
	HotkeyValues.push_back(VK_MEDIA_NEXT_TRACK);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.MEDIA_PREV_TRACK", _T("MEDIA PREV TRACK")));
	HotkeyValues.push_back(VK_MEDIA_PREV_TRACK);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.MEDIA_STOP", _T("MEDIA STOP")));
	HotkeyValues.push_back(VK_MEDIA_STOP);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.MEDIA_PLAY_PAUSE", _T("MEDIA PLAY/PAUSE")));
	HotkeyValues.push_back(VK_MEDIA_PLAY_PAUSE);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LAUNCH_MAIL", _T("LAUNCH MAIL")));
	HotkeyValues.push_back(VK_LAUNCH_MAIL);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LAUNCH_MEDIA_SELECT", _T("LAUNCH MEDIA SELECT")));
	HotkeyValues.push_back(VK_LAUNCH_MEDIA_SELECT);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LAUNCH_APP1", _T("LAUNCH APP1")));
	HotkeyValues.push_back(VK_LAUNCH_APP1);
	HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.LAUNCH_APP2", _T("LAUNCH APP2")));
	HotkeyValues.push_back(VK_LAUNCH_APP2);

	//HotkeyNames.push_back(ptLang.get("language.Hotkeys.HotkeysFrame.Keys.ANGLE", _T("Angle bracket or \\")));
	//HotkeyValues.push_back(VK_OEM_102);

	return true;
}

BOOL EnableHotkeyFields(HWND hDlg, BOOL bState){
	EnableWindow(GetDlgItem(hDlg , IDC_HOTKEY_MOD1),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HOTKEY_MOD2),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HOTKEY_MOD3),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HOTKEY_MOD4),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HOTKEY_KEY),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_CHKHKACTIVE),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HK_INSACTION),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HKBTNDELETE),bState);
	EnableWindow(GetDlgItem(hDlg , IDC_HKBTNRENAME),bState);
	EnableWindow(m_hwndHotkeyEditor,bState);
	return true;
}

BOOL LoadHotkeyNode(HWND hDlg, HTREEITEM hSelected){

	sLastHotkey = _T("");
	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	tvi.hItem=hSelected;
	char cText[255] = {0};
    tvi.pszText = cText;
	tvi.cchTextMax=256;
	BOOL bGetItem = SendMessage(GetDlgItem(hDlg , IDC_TREEHOTKEYS),TVM_GETITEM,0,(LPARAM)&tvi);
	sLastHotkey = cText;

	if(!bGetItem){
		EnableHotkeyFields(hDlg, false);
	} else {

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global.Hotkeys"))) {

				if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0)
				{
					EnableHotkeyFields(hDlg, true);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD1, CB_SETCURSEL, (v2.second.get("Modifier1",0) == 0 ? -1 : v2.second.get("Modifier1",0)), 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD2, CB_SETCURSEL, (v2.second.get("Modifier2",0) == 0 ? -1 : v2.second.get("Modifier2",0)), 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD3, CB_SETCURSEL, (v2.second.get("Modifier3",0) == 0 ? -1 : v2.second.get("Modifier3",0)), 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD4, CB_SETCURSEL, (v2.second.get("Modifier4",0) == 0 ? -1 : v2.second.get("Modifier4",0)), 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_SETCURSEL, (v2.second.get("Key",0) == 0 ? -1 : v2.second.get("Key",0)), 0);
					SendDlgItemMessage(hDlg, IDC_CHKHKACTIVE, BM_SETCHECK, (v2.second.get("<xmlattr>.enabled",1) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);

					std::basic_string<TCHAR> sLua = v2.second.get("Lua",_T(""));
					boost::replace_all(sLua, _T("\n"), _T(""));
					boost::replace_all(sLua, _T("\r"), _T("\r\n"));
					SendHotkeyEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)sLua.c_str());
				}

		}
	}

	return TRUE;
}

BOOL SaveHotkeyChange(HWND hDlg, HTREEITEM hSelected){
	
	if(hSelected == NULL)
		return TRUE;

	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
	tvi.hItem=hSelected;
	TCHAR cText[255] = {0};
	tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hDlg , IDC_TREEHOTKEYS),TVM_GETITEM,0,(LPARAM)&tvi);

	int iModSum = SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD1, CB_GETCURSEL, 0, 0) + 
				  SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD2, CB_GETCURSEL, 0, 0) + 
				  SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD3, CB_GETCURSEL, 0, 0) +
				  SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD4, CB_GETCURSEL, 0, 0);

	//If global, check global actions for conflicts
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, ptConfig.get_child(_T("config.Global.Hotkeys"))) {
		//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
		//if(v3.first == "Hotkeys"){
			//BOOST_FOREACH(boost::property_tree::ptree::value_type &v4, v3.second) {
				//each action
				if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) != 0 && v3.second.get("<xmlattr>.enabled",1) == 1) {
					if((v3.second.get("Modifier1",0) + v3.second.get("Modifier2",0) + v3.second.get("Modifier3",0) + v3.second.get("Modifier4",0) == iModSum
						&& v3.second.get("Key",0) == SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_GETCURSEL, 0, 0)) && 
						(v3.second.get("Modifier1",-1) > -1 || v3.second.get("Modifier2",-1) > -1 || v3.second.get("Modifier3",-1) > -1
						|| v3.second.get("Modifier4",-1) > -1 || v3.second.get("Key",-1) > -1)) 
					{
						string sTemp = _T("");
						int iResp = 0;
						iResp = MessageBox(hDlg, sTemp.append(
							ptLang.get("language.Hotkeys.Messages.HotkeyConflict",_T("The following hotkey already has the same key/modifiers defined:")).append(_T("\n\n")).append(v3.second.get("<xmlattr>.description",_T(""))).append(_T("\n\n")).append(ptLang.get("language.Hotkeys.Messages.HotkeyConflictSave",_T("Save anyway?")))).c_str()
							, ptLang.get("language.Hotkeys.Messages.HotkeyConflictTitle",_T("Hotkey Conflict")).c_str()
							, MB_YESNOCANCEL + MB_ICONWARNING + MB_APPLMODAL);
						if(iResp == IDNO){
							return TRUE;
						} else if(iResp == IDCANCEL){
							return FALSE;
						}
					}										
				}
			//}
		//}
	}

	//save hotkey
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
		//each global hotkey
		if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0) {
			

			/*int iPreviousHotkeyID = v2.second.get("HotkeyID",0);
			if(iPreviousHotkeyID>0){
				UnregisterHotKey(hWndServer, iPreviousHotkeyID);
				for(int i = 0; i < HotkeyIDs.size(); i++) { 
					if(HotkeyIDs[i] == iPreviousHotkeyID){
						HotkeyIDs[i] = 0;
						break;
					}
				}
			}*/

			int iMod1 = SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD1, CB_GETCURSEL, 0, 0);
			int iMod2 = SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD2, CB_GETCURSEL, 0, 0);
			int iMod3 = SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD3, CB_GETCURSEL, 0, 0);
			int iMod4 = SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD4, CB_GETCURSEL, 0, 0);
			int iKey = SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_GETCURSEL, 0, 0);



			v2.second.put("<xmlattr>.enabled", SendDlgItemMessage(hDlg, IDC_CHKHKACTIVE, BM_GETCHECK, 0, 0));

			//int iModifiers = 0;
			//iModifiers = iModifiers | (iMod1 == 1 ? 0x0002 : (iMod1 == 2 ? 0x0001 : (iMod1 == 3 ? 0x0004 : (iMod1 == 4 ? 0x0008 : 0)))) | (iMod2 == 1 ? 0x0002 : (iMod2 == 2 ? 0x0001 : (iMod2 == 3 ? 0x0004 : (iMod2 == 4 ? 0x0008 : 0)))) | (iMod3 == 1 ? 0x0002 : (iMod3 == 2 ? 0x0001 : (iMod3 == 3 ? 0x0004 : (iMod3 == 4 ? 0x0008 : 0)))) | (iMod4 == 1 ? 0x0002 : (iMod4 == 2 ? 0x0001 : (iMod4 == 3 ? 0x0004 : (iMod4 == 4 ? 0x0008 : 0))));

			v2.second.put("HotkeyID", 0);

			if(iKey>0){
				/*
				int iHotkeyID = RegisterHotKey(hWndServer, iHotkey, iModifiers, HotkeyValues[iKey]);
				if(iHotkeyID==0){
					string sTemp = _T("");
					MessageBox(hDlg, sTemp.append(
						ptLang.get("language.Hotkeys.Messages.HotkeyFailure",_T("Failed to register hotkey for:")).append(_T("\n\n")).append(cText).append(_T(": ")).append(cText)).append(_T("\n\n")).c_str()
						, ptLang.get("language.Hotkeys.Messages.HotkeyFailureTitle",_T("Action Conflict")).c_str()
						,  MB_ICONWARNING + MB_APPLMODAL);
				} else {
					if(iHotkey == INT_MAX) {
						//TODO: Find a better solution to this
						MessageBox(hDlg, "Maximum hotkey identifier limit reached. Please restart StrokesPlus.", "Lame", MB_ICONWARNING + MB_APPLMODAL);
					} else {
						iHotkey++;
					}
				}
				v2.second.put("HotkeyID", iHotkeyID);
				HotkeyIDs.push_back(iHotkeyID);
				*/

				v2.second.put("Modifier1", iMod1);
				v2.second.put("Modifier2", iMod2);
				v2.second.put("Modifier3", iMod3);
				v2.second.put("Modifier4", iMod4);
				v2.second.put("Key", iKey);
			}

			TCHAR szLua[8192];
			memset(szLua,0,sizeof(szLua));
			UINT uSize = SendHotkeyEditor( SCI_GETLENGTH, 0, 0L );
			SendHotkeyEditor( SCI_GETTEXT, uSize + 1, (LPARAM)szLua );
			v2.second.put("Lua",szLua);

			TVITEM tvi;
			ZeroMemory(&tvi,sizeof(tvi));
			tvi.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.hItem=hSelected;
			char cText[255] = {0};
			tvi.pszText = cText;
			tvi.cchTextMax=256;
			SendMessage(GetDlgItem(hDlg, IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

			TreeView_SetItem(GetDlgItem(hDlg, IDC_TREEHOTKEYS), &tvi);
		}
	}

	PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_LOADHOTKEYS, 1), 0);

	return TRUE;
}

#pragma endregion Hotkey Tab Functions


#pragma region Ignored Window Functions

void ShowIgnoreFields(HWND hDlg, BOOL bShow){

	ShowWindow(GetDlgItem(hDlg,IDC_OWNT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_OWNERTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_OWNTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_OWNERTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_OWNC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_OWNERCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_OWNCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_OWNERCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_PART), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_PARENTTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_PARTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_PARENTTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_PARC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_PARENTCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_PARCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_PARENTCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CTT), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_CONTROLTITLE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CTTR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_CONTROLTITLE_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CID), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_CONTROLID), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CTC), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_CONTROLCLASS), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CTCR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_CONTROLCLASS_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FN), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_FILENAME), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FNR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_FILENAME_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FPR), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EDIT_PATH_REGEX), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FINDWINDOW), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_ICON_FINDER_TOOL), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CHK_DISABLEACTIVE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CHK_IGNAPPACTIVE), bShow ? SW_SHOW : SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_QUALIFIER), bShow ? SW_SHOW : SW_HIDE);	

}

BOOL BuildIgnoreTree(HWND hDlg)
{
		TreeView_DeleteAllItems(GetDlgItem(hDlg,IDC_TREEIGNOREDAPPS));

		TV_INSERTSTRUCT tvis ;
		HTREEITEM hLastSelected = NULL;
		
		// Initialize list view item structure
		ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
		tvis.hParent = NULL ;
		tvis.hInsertAfter = TVI_SORT;
		tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
		tvis.item.cchTextMax = 256 ;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Ignored"))) {
			//each ignored window
			if(v2.second.get("<xmlattr>.name",_T("")).length() > 0 /*&& v2.second.get("<xmlattr>.enabled",0) == 1*/){
				TCHAR sDesc[255];
				HTREEITEM hParent;
				tvis.hParent = NULL;
				_tcscpy (sDesc,v2.second.get("<xmlattr>.name",_T("Unknown")).c_str());
				char *cDesc = const_cast<char*> ( sDesc );
				tvis.item.pszText = cDesc;
				tvis.item.lParam =(LPARAM) sDesc;
				tvis.item.cChildren = 0 ;
				hParent = TreeView_InsertItem (GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS), &tvis) ;
				if(_tcsicmp( sLastIgnore.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str()) == 0) {
					hLastSelected = hParent;
				}
			}
		}
		
		if(hLastSelected == NULL){
			TreeView_Select( GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS), TreeView_GetFirstVisible(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS)), TVGN_CARET);
		} else {
			TreeView_Select( GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS), hLastSelected, TVGN_CARET);
		}
		

		return TRUE;
}

BOOL SaveIgnoredChange(HWND hDlg, HTREEITEM hSelected){

	if(hSelected == NULL)
		return TRUE;

	TVITEM tvi;
	ZeroMemory(&tvi,sizeof(tvi));
	tvi.mask=TVIF_TEXT;
	tvi.hItem=hSelected;
	char cText[255] = {0};
    tvi.pszText = cText;
	tvi.cchTextMax=256;
	SendMessage(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Ignored"))) {
		//each ignored window
		if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/)
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
				//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
				if(v3.first == _T("MatchCriteria")){
					TCHAR szOwnerClassName[256];
					TCHAR szOwnerClassNameRegex[256];
					TCHAR szOwnerTitle[256];
					TCHAR szOwnerTitleRegex[256];
					TCHAR szParentClassName[256];
					TCHAR szParentClassNameRegex[256];
					TCHAR szParentTitle[256];
					TCHAR szParentTitleRegex[256];
					TCHAR szControlClassName[256];
					TCHAR szControlClassNameRegex[256];
					TCHAR szControlTitle[256];
					TCHAR szControlTitleRegex[256];
					TCHAR szFileName[_MAX_PATH];
					TCHAR szFileNameRegex[_MAX_PATH];
					TCHAR szPathRegex[_MAX_PATH];
					memset(szOwnerClassName,0,sizeof(szOwnerClassName));
					memset(szOwnerClassNameRegex,0,sizeof(szOwnerClassNameRegex));
					memset(szOwnerTitle,0,sizeof(szOwnerTitle));
					memset(szOwnerTitleRegex,0,sizeof(szOwnerTitleRegex));
					memset(szParentClassName,0,sizeof(szParentClassName));
					memset(szParentClassNameRegex,0,sizeof(szParentClassNameRegex));
					memset(szParentTitle,0,sizeof(szParentTitle));
					memset(szParentTitleRegex,0,sizeof(szParentTitleRegex));
					memset(szControlClassName,0,sizeof(szControlClassName));
					memset(szControlClassNameRegex,0,sizeof(szControlClassNameRegex));
					memset(szControlTitle,0,sizeof(szControlTitle));
					memset(szControlTitleRegex,0,sizeof(szControlTitleRegex));
					memset(szFileName,0,sizeof(szFileName));
					memset(szFileNameRegex,0,sizeof(szFileNameRegex));
					memset(szPathRegex,0,sizeof(szPathRegex));
					GetDlgItemText(hDlg, IDC_EDIT_OWNERTITLE, szOwnerTitle, 255);
					GetDlgItemText(hDlg, IDC_EDIT_OWNERTITLE_REGEX, szOwnerTitleRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_OWNERCLASS, szOwnerClassName, 255);
					GetDlgItemText(hDlg, IDC_EDIT_OWNERCLASS_REGEX, szOwnerClassNameRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_PARENTTITLE, szParentTitle, 255);
					GetDlgItemText(hDlg, IDC_EDIT_PARENTTITLE_REGEX, szParentTitleRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_PARENTCLASS, szParentClassName, 255);
					GetDlgItemText(hDlg, IDC_EDIT_PARENTCLASS_REGEX, szParentClassNameRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_CONTROLTITLE, szControlTitle, 255);
					GetDlgItemText(hDlg, IDC_EDIT_CONTROLTITLE_REGEX, szControlTitleRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_CONTROLCLASS, szControlClassName, 255);
					GetDlgItemText(hDlg, IDC_EDIT_CONTROLCLASS_REGEX, szControlClassNameRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_FILENAME, szFileName, 255);
					GetDlgItemText(hDlg, IDC_EDIT_FILENAME_REGEX, szFileNameRegex, 255);
					GetDlgItemText(hDlg, IDC_EDIT_PATH_REGEX, szPathRegex, 255);
					v3.second.put("ownerclass", szOwnerClassName);
					v3.second.put("ownerclassregex", szOwnerClassNameRegex);
					v3.second.put("ownertitle", szOwnerTitle);
					v3.second.put("ownertitleregex", szOwnerTitleRegex);
					v3.second.put("parentclass", szParentClassName);
					v3.second.put("parentclassregex", szParentClassNameRegex);
					v3.second.put("parenttitle", szParentTitle);
					v3.second.put("parenttitleregex", szParentTitleRegex);
					v3.second.put("controlclass", szControlClassName);
					v3.second.put("controlclassregex", szControlClassNameRegex);
					v3.second.put("controltitle", szControlTitle);
					v3.second.put("controltitleregex", szControlTitleRegex);
					int iControlID = GetDlgItemInt(hDlg, IDC_EDIT_CONTROLID, NULL, FALSE);
					if(iControlID==0){
						v3.second.put("controlid", "");
					} else {
						v3.second.put("controlid", iControlID);
					}
					v3.second.put("file", szFileName);
					v3.second.put("fileregex", szFileNameRegex);
					v3.second.put("pathregex", szPathRegex);
					v3.second.put("disableonactivate", SendDlgItemMessage(hDlg, IDC_CHK_DISABLEACTIVE, BM_GETCHECK, 0, 0));
					v2.second.put("<xmlattr>.enabled", SendDlgItemMessage(hDlg, IDC_CHK_IGNAPPACTIVE, BM_GETCHECK, 0, 0));
				}
			}
		}
	}
	return TRUE;
}

void ResizeIgnoreWindow(HWND hDlg){

	RECT lpRect;
	RECT lpWindowRect;
	POINT lpPoint;
	GetWindowRect(hDlg, &lpWindowRect);
			
	GetWindowRect(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS), NULL, 0, 0, lpRect.right - lpRect.left, lpWindowRect.bottom-lpWindowRect.top-42, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);

	GetWindowRect(GetDlgItem(hDlg , IDC_IGNBTNADD), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_IGNBTNADD), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_IGNBTNRENAME), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_IGNBTNRENAME), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hDlg , IDC_IGNBTNDELETE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_IGNBTNDELETE), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
}

#pragma endregion Ignored Window Functions


#pragma region Report Window Functions

BOOL BuildConfigList(HWND hDlg) {

	ListView_SetExtendedListViewStyle(GetDlgItem(hDlg,IDC_LISTAPPS), LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	ListView_SetImageList(GetDlgItem(hDlg,IDC_LISTAPPS),NULL,LVSIL_SMALL); 
	BuildGestureImageList(FALSE, 64);
	ListView_SetImageList(GetDlgItem(hDlg,IDC_LISTAPPS),hList,LVSIL_SMALL); 
	
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
		//each app
		if(v2.second.get("<xmlattr>.enabled",0) == 1){
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("Actions"))) {
				if(v3.second.get("<xmlattr>.enabled",0) == 1){
					TCHAR sDesc[255];
					_tcscpy (sDesc,v3.second.get("<xmlattr>.description",_T("Unknown")).c_str());
					TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );

					LVITEM lvitm;
					ZeroMemory (&lvitm, sizeof(LVITEM));
					lvitm.mask = LVIF_IMAGE; // | LVIF_TEXT;
					for(int i = 0; i < GestureNames.size(); i++) { 
						if(_tcscmp( GestureNames[i].c_str(), v3.second.get("GestureName",_T("")).c_str() ) == 0 ){
							//TCHAR sGesture[255] = _T("");
							//_tcscpy(sGesture,v3.second.get("GestureName",_T("")).c_str());
							//lvitm.pszText = sGesture;
							lvitm.iImage = i;
							break;
						}
					}							

					//Gesture 
					lvitm.iItem = 0;
					lvitm.iSubItem = 0;
					int inewitm = ListView_InsertItem(GetDlgItem(hDlg,IDC_LISTAPPS), &lvitm);
						
					TCHAR sGesture[100];
					_tcscpy(sGesture,v3.second.get("GestureName",_T("")).c_str());
					ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 1,sGesture);

					//Modifiers
					TCHAR sModifiers[100];
					string strModifiers = _T("");
					strModifiers.append((v3.second.get("Control",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Control",_T("Control")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("Alt",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Alt",_T("Alt")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("Shift",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Shift",_T("Shift")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("Right",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Right",_T("Right")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("Middle",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Middle",_T("Middle")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("Left",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Left",_T("Left")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("X1",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X1",_T("X1")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("X2",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X2",_T("X2")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("WheelUp",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.ScrollUp",_T("Scroll Up")).append(" + ") : _T("")));
					strModifiers.append((v3.second.get("WheelDown",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.ScrollDown",_T("Scroll Down")).append(" + ") : _T("")));
					if(strModifiers.length() > 0){
						strModifiers = strModifiers.substr(0, strModifiers.length() - 3);
						if(bCaptureModifiersOnMouseDown){
							switch(v3.second.get("StrokeState",0)){
							case 1:
								strModifiers.append(_T("\n(")).append(ptLang.get("language.Actions.ModifierPress.Before",_T("On Stroke Button Down")).append(_T(")")));
								break;
							case 2:
								strModifiers.append(_T("\n(")).append(ptLang.get("language.Actions.ModifierPress.After",_T("After Stroke Button Down")).append(_T(")")));
								break;
							}
						}
					}

					_tcscpy(sModifiers,strModifiers.c_str());
					ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 2,sModifiers);

					//App name
					TCHAR sAppName[255];
					_tcscpy(sAppName,v2.second.get("<xmlattr>.name",_T("")).c_str());
					ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 3,sAppName);

					//Action name
					ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 4,cDesc);

					//Lua
					TCHAR sLua[8192];
					_tcscpy(sLua,v3.second.get("Lua",_T("")).c_str());
					ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 5,sLua);
				}
			}
		}
	}

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, ptConfig.get_child(_T("config.Global.Actions"))) {
		//each global action
		if(v3.second.get("<xmlattr>.enabled",0) == 1){
			TCHAR sDesc[255];
			_tcscpy (sDesc,v3.second.get("<xmlattr>.description",_T("Unknown")).c_str());
			TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );

			LVITEM lvitm;
			ZeroMemory (&lvitm, sizeof(LVITEM));
			lvitm.mask = LVIF_IMAGE;// | LVIF_TEXT;
			for(int i = 0; i < GestureNames.size(); i++) { 
				if(_tcscmp( GestureNames[i].c_str(), v3.second.get("GestureName",_T("")).c_str() ) == 0 ){
					lvitm.iImage = i;
					break;
				}
			}							

			//Gesture 
			lvitm.iItem = 0;
			lvitm.iSubItem = 0;
			int inewitm = ListView_InsertItem(GetDlgItem(hDlg,IDC_LISTAPPS), &lvitm);
			TCHAR sGesture[100];
			_tcscpy(sGesture,v3.second.get("GestureName",_T("")).c_str());
			ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 1,sGesture);

			//Modifers
			TCHAR sModifiers[100];
			string strModifiers = _T("");
			strModifiers.append((v3.second.get("Control",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Control",_T("Control")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("Alt",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Alt",_T("Alt")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("Shift",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Shift",_T("Shift")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("Right",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Right",_T("Right")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("Middle",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Middle",_T("Middle")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("Left",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Left",_T("Left")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("X1",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X1",_T("X1")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("X2",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X2",_T("X2")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("WheelUp",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.ScrollUp",_T("Scroll Up")).append(" + ") : _T("")));
			strModifiers.append((v3.second.get("WheelDown",0) == 1 ? ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.ScrollDown",_T("Scroll Down")).append(" + ") : _T("")));
			if(strModifiers.length() > 0){
				strModifiers = strModifiers.substr(0, strModifiers.length() - 3);
				if(bCaptureModifiersOnMouseDown){
					switch(v3.second.get("StrokeState",0)){
					case 1:
						strModifiers.append(_T("\n(")).append(ptLang.get("language.Actions.ModifierPress.Before",_T("On Stroke Button Down"))).append(_T(")"));
						break;
					case 2:
						strModifiers.append(_T("\n(")).append(ptLang.get("language.Actions.ModifierPress.After",_T("After Stroke Button Down"))).append(_T(")"));
						break;
					}
				}
			}
			_tcscpy(sModifiers,strModifiers.c_str());
			ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 2,sModifiers);

			//App Name
			CHAR sGlobal[255];
			_tcscpy(sGlobal,	ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str());
			ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 3, sGlobal);

			//Action Name
			ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 4,cDesc);

			//Lua
			TCHAR sLua[8192];
			_tcscpy(sLua,v3.second.get("Lua",_T("")).c_str());
			ListView_SetItemText(GetDlgItem(hDlg,IDC_LISTAPPS),0, 5,sLua);
		
		}
	}

	return TRUE;

}

int CALLBACK ListViewCompareProc (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	static char szBuf1[100], szBuf2[100];

	BOOL bSortAscending = (lParamSort > 0);
	int nColumn = abs(lParamSort) - 1;
 
	ListView_GetItemText(GetDlgItem(hWndReport,IDC_LISTAPPS), lParam1, nColumn, szBuf1, sizeof(szBuf1));
 
	ListView_GetItemText(GetDlgItem(hWndReport,IDC_LISTAPPS), lParam2, nColumn, szBuf2, sizeof(szBuf2));
 
	if (bSortAscending) 
 		return(_tcsicmp(szBuf1, szBuf2) * -1);
	else
		return(_tcsicmp(szBuf1, szBuf2));
}

void ResizeReportWindow(HWND hDlg){

	RECT lpRect;
	RECT lpWindowRect;
	POINT lpPoint;
	GetWindowRect(hDlg, &lpWindowRect);
			
	GetWindowRect(GetDlgItem(hDlg , IDC_LISTAPPS), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_LISTAPPS), NULL, 0, 0, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-60, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);

	GetWindowRect(GetDlgItem(hDlg , IDC_BTN_REFRESH), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(GetDlgItem(hDlg , IDC_BTN_REFRESH), NULL, lpPoint.x, (lpWindowRect.bottom - lpWindowRect.top) - (lpRect.bottom - lpRect.top) - 4, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
}

#pragma endregion Report Window Functions


#pragma region Lua Tab Functions

BOOL SaveLuaFile(){
	ofstream file;
	SetCurrentDirectory(strExePath);
	std::basic_string<TCHAR> sLuaPath = strStrokesConfigFile;
	//MessageBox(GetForegroundWindow(), strStrokesConfigFile, "initLua", NULL);
	boost::replace_all(sLuaPath, _T(".xml"), _T(".lua"));
	file.open(sLuaPath, ios::out);
	if (file.fail())
	{
		return FALSE; // Fail
	}
	int buflen = SendLuaEditor(SCI_GETLENGTH, 0, 0)+1; //last NULL
	TCHAR *pBuffer = new TCHAR[buflen];
	if (pBuffer != NULL)
	{
		SendLuaEditor(SCI_GETTEXT, buflen,(long)pBuffer);
		file.write(pBuffer, buflen-1);
		delete [] pBuffer;
	}
	file << ends;
	file.close();
	return TRUE; // Success
}

BOOL LoadLuaFile(){
	BOOL bReturn = TRUE;
	ifstream file;
	long len = 0L;
	long nTotal;
	TCHAR *szBuffer = NULL;
	SetCurrentDirectory(strExePath);
	std::basic_string<TCHAR> sLuaPath = strStrokesConfigFile;
	//MessageBox(GetForegroundWindow(), strStrokesConfigFile, "initLua", NULL);
	boost::replace_all(sLuaPath, _T(".xml"), _T(".lua"));
	file.open(sLuaPath, ios::in);
	// ok success - try to get length of file
	if (file.is_open())
	{
	//len = _filelength(file.fd());
	file.seekg (0, ios::end);
	len = file.tellg();
	file.seekg (0, ios::beg);
	if (len > 0)
	{
		// alloc new buffer of sie = filesize+1 for termination NULL
		szBuffer = new TCHAR[len+1];
		if (szBuffer != NULL)
		{
			file.read(szBuffer, len);
			nTotal = file.gcount();
			if (nTotal > 0 && nTotal <= len)
				szBuffer[nTotal] = _T('\0');
			// read error
			if (file.bad())
			{
				file.close();
				bReturn = FALSE;
			}
		}
		else
		{
			file.close();
			bReturn = FALSE;
		}
	}
	file.close();
	// set text to control
	SendLuaEditor(SCI_SETTEXT,0,(LPARAM)szBuffer);
	// tell scintilla that we have an unmodified document
	SendEditor(SCI_SETSAVEPOINT,0 , 0);
	} else {
          bReturn = FALSE;
	}

    // clean up
    if (szBuffer != NULL)
        delete [] szBuffer;

    return bReturn;
}

void ResizeLuaWindow(HWND hDlg){

	RECT lpRect;
	RECT lpWindowRect;
	POINT lpPoint;
	GetWindowRect(hDlg, &lpWindowRect);
			
	GetWindowRect(GetDlgItem(hDlg , IDC_LUATEXT), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hDlg, &lpPoint);
	SetWindowPos(m_hwndLuaEditor, NULL, lpPoint.x, lpPoint.y + (lpRect.bottom-lpRect.top) + 4, lpWindowRect.right - lpWindowRect.left - lpPoint.x - 4, lpWindowRect.bottom-lpWindowRect.top-(lpRect.bottom-lpRect.top)-18, SWP_NOZORDER | SWP_NOOWNERZORDER);

}

#pragma endregion Lua Tab Functions


#pragma region Settings Window Functions

void setSelectedTab(BOOL bRefresh) {
	EnterCriticalSection(&CriticalSection); 
	ShowWindow(hActionsTab, (iSelectedTab == 0 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hHotkeyTab, (iSelectedTab == 1 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hIgnoredTab, (iSelectedTab == 2 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hPreferencesTab, (iSelectedTab == 3 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hActionsReportTab, (iSelectedTab == 4 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hLuaTab, (iSelectedTab == 5 ? SW_SHOW : SW_HIDE));  
	ShowWindow(hAboutTab, (iSelectedTab == 6 ? SW_SHOW : SW_HIDE));  
	ShowWindow(GetDlgItem(hWndSettings,IDC_S_AUTOSAVE), (iSelectedTab == 0 || iSelectedTab == 1 ? SW_SHOW : SW_HIDE));  

	if(IsWindowVisible(hWndConfig)){
		if(hListTree == hList){
			//MessageBox(hWndSettings,"Image Lists Match!","Bah!",NULL);
			BuildGestureImageList(TRUE,28);
			TreeView_SetImageList(GetDlgItem(hWndConfig,IDC_TREEAPPS), hListTree, TVSIL_NORMAL);
			SendMessage(hWndCB,CBEM_SETIMAGELIST,0,(LPARAM)hListTree);
		}
		BringWindowToTop(hWndConfig);
		ResizeConfigWindow(hWndConfig);
		SetModifierStates(hWndConfig);
		SendMessage(hWndConfig, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWndConfig,IDC_TREEAPPS), TRUE);
	}
	if(IsWindowVisible(hWndIgnore)){
		BringWindowToTop(hWndIgnore);
		ResizeIgnoreWindow(hWndIgnore);
		SendMessage(hWndIgnore, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWndIgnore,IDC_TREEIGNOREDAPPS), TRUE);
	}
	#ifdef SP_NOGLOBALACTIONS
		EnableWindow(hIgnoredTab, !bOnlyAllowDefinedApps);
	#endif
	if(IsWindowVisible(hLuaTab)){
		ResizeLuaWindow(hLuaTab);
	}
	if(IsWindowVisible(hHotkeyTab)){
		BringWindowToTop(hWndHotkey);
		ResizeHotkeyWindow(hHotkeyTab);
		SendMessage(hWndHotkey, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWndHotkey,IDC_TREEHOTKEYS), TRUE);
	}
	if(IsWindowVisible(hWndReport)){
		BringWindowToTop(hWndReport);
		ListView_DeleteAllItems(GetDlgItem(hWndReport,IDC_LISTAPPS));
		BuildConfigList(hWndReport);
		LPARAM lParamSort = -5;
		ListView_SortItemsEx(GetDlgItem(hWndReport,IDC_LISTAPPS), ListViewCompareProc, lParamSort);
		lParamSort = -2;
		ListView_SortItemsEx(GetDlgItem(hWndReport,IDC_LISTAPPS), ListViewCompareProc, lParamSort);
		ResizeReportWindow(hWndReport);
	}
	if(IsWindowVisible(hPreferencesTab) || IsWindowVisible(hAboutTab) || IsWindowVisible(hWndReport) || IsWindowVisible(hLuaTab)){
		SendMessage(hWndSettings, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWndSettings,ID_S_OK), TRUE);
	}

	if(bRefresh){
		PostMessage(hWndSettings, WM_EXITSIZEMOVE, 0, 0);
	}

	LeaveCriticalSection(&CriticalSection);
}

void resizeSettings() {

	RECT lpWindowRect;
	GetWindowRect(hWndSettings, &lpWindowRect);
	BOOL bResize = FALSE;
	//if(bSaveActionSize){
	if(IsZoomed(hWndSettings)) {
		bSettingsMaximized = true;
	}
	if(bSettingsOpen && !bSettingsMaximized){
		iConfigWidth = (lpWindowRect.right - lpWindowRect.left);
		if(iConfigWidth < 1014){
			iConfigWidth = 1014;
			bResize = TRUE;
		}
		iConfigHeight = (lpWindowRect.bottom - lpWindowRect.top);
		if(iConfigHeight < 703){
			iConfigHeight = 703;
			bResize = TRUE;
		}
	}

	RECT rcClient;
	RECT rcTab; 
	RECT rcDisplayArea;
	POINT tpa;
	POINT tpb;

	/*
	DWORD dwDlgBase = GetDialogBaseUnits();
	int cxMargin = LOWORD(dwDlgBase) / 4; 
	int cyMargin = HIWORD(dwDlgBase) / 8; 

  	TCHAR buffer[256];
	wsprintf(buffer, _T("values:\n")
		_T("cxMargin: %d\n")
		_T("cyMargin: %d\n\n"),
		cxMargin,cyMargin);
	MessageBox(hWndServer,buffer,_T("Values"),MB_OK);
	*/

	GetClientRect(hWndSettings,&rcClient);

	SetWindowPos(hTabControlMain, NULL, rcClient.left+10, rcClient.top+10, (rcClient.right - rcClient.left)-20, (rcClient.bottom - rcClient.top)-47, SWP_NOZORDER); 

	GetWindowRect(hTabControlMain,&rcTab);

	tpa.x = rcTab.left;
	tpa.y = rcTab.top;
	tpb.x = rcTab.right;
	tpb.y = rcTab.bottom;

	ScreenToClient(hTabControlMain,&tpa);
	ScreenToClient(hTabControlMain,&tpb);

	rcDisplayArea.left  = tpa.x;
	rcDisplayArea.top = tpa.y;
	rcDisplayArea.right = tpb.x;
	rcDisplayArea.bottom = tpb.y;

	//MapDialogRect(hDlg, &rcTab);
	//TabCtrl_AdjustRect(hActionsTab, TRUE, &rcTab); 
	//OffsetRect(&rcTab, cxMargin - rcTab.left, cyMargin - rcTab.top); 
	//CopyRect(&rcDisplayArea, &rcTab); 
	//TabCtrl_AdjustRect(hActionsTab, FALSE, &rcDisplayArea); 

	//TabCtrl_GetItemRect(hTabControlMain,0,&rcDisplayArea);

	SetWindowPos(hActionsTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hIgnoredTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hHotkeyTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hPreferencesTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	//SetWindowPos(hGesturesTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hActionsReportTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hAboutTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 
	SetWindowPos(hLuaTab, NULL, rcDisplayArea.left+1, rcDisplayArea.top+22, (rcDisplayArea.right - rcDisplayArea.left) - 5, (rcDisplayArea.bottom - rcDisplayArea.top) - 25, SWP_NOZORDER); 

	RECT lpRect;
	POINT lpPoint;

	GetWindowRect(GetDlgItem(hWndSettings , IDC_S_AUTOSAVE), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hWndSettings, &lpPoint);
	SetWindowPos(GetDlgItem(hWndSettings , IDC_S_AUTOSAVE), NULL, rcClient.left + 10, (rcClient.bottom - rcClient.top) - (lpRect.bottom - lpRect.top) - 6, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	GetWindowRect(GetDlgItem(hWndSettings , ID_S_CANCEL), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hWndSettings, &lpPoint);
	SetWindowPos(GetDlgItem(hWndSettings , ID_S_CANCEL), NULL, (rcClient.right - rcClient.left) - (lpRect.right - lpRect.left) - 10, (rcClient.bottom - rcClient.top) - (lpRect.bottom - lpRect.top) - 6, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);

	int iTempX = (rcClient.right - rcClient.left) - (lpRect.right - lpRect.left) - 10;
	int iTempY = (rcClient.bottom - rcClient.top) - (lpRect.bottom - lpRect.top) - 6;

	GetWindowRect(GetDlgItem(hWndSettings , ID_S_OK), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hWndSettings, &lpPoint);
	SetWindowPos(GetDlgItem(hWndSettings , ID_S_OK), NULL, (iTempX) - (lpRect.right - lpRect.left) - 5, iTempY, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_OK),SW_HIDE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_OK),SW_SHOW);

	iTempX = (iTempX) - (lpRect.right - lpRect.left) - 5;

	GetWindowRect(GetDlgItem(hWndSettings , ID_S_APPLY), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hWndSettings, &lpPoint);
	SetWindowPos(GetDlgItem(hWndSettings , ID_S_APPLY), NULL, (iTempX) - (lpRect.right - lpRect.left) - 5, iTempY, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_APPLY),SW_HIDE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_APPLY),SW_SHOW);

	iTempX = (iTempX) - (lpRect.right - lpRect.left) - 5;

	GetWindowRect(GetDlgItem(hWndSettings , ID_S_HELP), &lpRect);
	lpPoint.x = lpRect.left;
	lpPoint.y = lpRect.top;
	ScreenToClient(hWndSettings, &lpPoint);
	SetWindowPos(GetDlgItem(hWndSettings , ID_S_HELP), NULL, (iTempX) - (lpRect.right - lpRect.left) - 5, iTempY, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_HELP),SW_HIDE);
	ShowWindow(GetDlgItem(hWndSettings , ID_S_HELP),SW_SHOW);

	//iTempX = (iTempX) - (lpRect.right - lpRect.left) - 5;

	if(bResize){
		SetWindowPos(hWndSettings, NULL, 0, 0, iConfigWidth, iConfigHeight, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
	}

}

#pragma endregion Settings Window Function


//##########################################################################################
//###################################### Dialog Procs ######################################
//##########################################################################################

#pragma region DisplayTextProc

INT_PTR CALLBACK DisplayTextProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		{
			SetWindowPos(hDlg, HWND_TOPMOST, iXVIRTUALSCREEN, iYVIRTUALSCREEN, iCXVIRTUALSCREEN, iCYVIRTUALSCREEN, SWP_NOACTIVATE);
			SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_NOACTIVATE);
			SetLayeredWindowAttributes(hDlg, RGB(iDisplayTextBGR,iDisplayTextBGG,iDisplayTextBGB), NULL, LWA_COLORKEY);
			displaytexttimerid = SetTimer(NULL, WM_IDT_DISPLAYTEXT, iDisplayTextDuration, DisplayTextTimerProc);
			hWndDisplayText = hDlg;

			return (INT_PTR)TRUE;
		}
	case WM_CTLCOLORDLG:
		{
			g_hbrBackground = CreateSolidBrush(RGB(iDisplayTextBGR,iDisplayTextBGG,iDisplayTextBGB));
			SetWindowLongPtr(hDlg, DWLP_MSGRESULT, 1);
			return (INT_PTR)g_hbrBackground;
		}
	case WM_PAINT:
		{
			/*if(hDlg == GetForegroundWindow()){
				setLastActive();
			}*/

			LOGMESSAGE("DisplayTextProc: WM_PAINT - About to check sDisplayTextMessage Length");

			if(sDisplayTextMessage.length() > 0){
				HDC         hDCText;
				HFONT	    font;

				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to CreateFont");
				font = CreateFont(iDisplayTextSize, 0, 0, 0,
								FW_NORMAL, FALSE, FALSE, FALSE,
								ANSI_CHARSET, OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
								DEFAULT_PITCH,
								sDisplayTextFont.c_str());

				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to GetDC");
				hDCText = GetDC(hDlg);
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to SetBkColor");
				SetBkColor(hDCText,RGB(iDisplayTextBGR,iDisplayTextBGG,iDisplayTextBGB));
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to SetTextColor");
				SetTextColor(hDCText, RGB(iDisplayTextR,iDisplayTextG,iDisplayTextB));
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to SelectObject");
				SelectObject(hDCText, font);
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to setup lpTextRect");
				RECT lpTextRect;
				lpTextRect.left = (iDisplayTextX-rcWindow.left);
				lpTextRect.top = (iDisplayTextY-rcWindow.top);
				lpTextRect.right = iCXVIRTUALSCREEN;
				lpTextRect.bottom = iCYVIRTUALSCREEN;
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About to DrawText");
				DrawText(hDCText, sDisplayTextMessage.c_str(), sDisplayTextMessage.length(), &lpTextRect, DT_LEFT);
				LOGMESSAGE("DisplayTextProc: WM_PAINT - About DeleteObject");
				DeleteObject(font);
				LOGMESSAGE("DisplayTextProc: WM_PAINT - ReleaseDC");
				ReleaseDC(hDlg, hDCText);
			}

			break;
		}
	case WM_CLOSE:
		{
			if(sDisplayTextMessage.length() == 0){
				RECT lpTextRect;
				lpTextRect.left = (iDisplayTextX-rcWindow.left);
				lpTextRect.top = (iDisplayTextY-rcWindow.top);
				lpTextRect.right = iCXVIRTUALSCREEN;
				lpTextRect.bottom = iCYVIRTUALSCREEN;
				InvalidateRect(hDlg, &lpTextRect, true);
			}
			KillTimer(NULL,displaytexttimerid);
			DeleteObject(g_hbrBackground);
			EndDialog(hDlg, LOWORD(wParam));
			bOSDDisplayed = FALSE;
			hWndDisplayText = NULL;
		}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion DisplayTextProc


#pragma region InputBoxProc

INT_PTR CALLBACK InputBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_INITDIALOG:
		{
				SetWindowText(hDlg,sInputBoxTitle.c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_IBSTATIC), sInputBoxLabel.c_str());
				SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.Password.OK",_T("OK")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDCANCEL), ptLang.get("language.Password.Cancel",_T("Cancel")).c_str());
				ShowWindow(GetDlgItem(hDlg, IDC_IBEDIT),(sInputBoxDelim.length()==0 ? SW_SHOW : SW_HIDE));
				ShowWindow(GetDlgItem(hDlg, IDC_IBCOMBO),(sInputBoxDelim.length()==0 ? SW_HIDE : SW_SHOW));
				if(sInputBoxDelim.length()==0){
					SetDlgItemText(hDlg, IDC_IBEDIT, sInputBoxResult.c_str());
				} else {
					std::vector<std::string> strs;
					boost::split(strs, sInputBoxResult, boost::is_any_of(sInputBoxDelim),boost::algorithm::token_compress_on);
					for(int i=0; i < strs.size(); i++){
						SendDlgItemMessage(hDlg,IDC_IBCOMBO,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)strs[i].c_str()));
					}
					SendDlgItemMessage(hDlg,IDC_IBCOMBO, CB_SETCURSEL, 0, 0);
				}
				SetForegroundWindow(hDlg);
				SendDlgItemMessage(hDlg, IDC_IBEDIT, EM_SETSEL, 0, -1);
				sInputBoxResult = "";
				bInputBoxOpen = true;
			return (INT_PTR)TRUE;
		}
		case WM_COMMAND:
		{
				if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{

					if(LOWORD(wParam) == IDOK){
						static TCHAR szString[255];
						GetDlgItemText(hDlg, (sInputBoxDelim.length()==0 ? IDC_IBEDIT : IDC_IBCOMBO), szString, 254);
						size_t len;
						size_t maxsize = 255;
						len = _tcsnlen(szString, maxsize);
						if(len > 0) {							
							sInputBoxResult = szString;
						}
					}
					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}
				break;
		}
		case WM_PAINT:
		{
			SetFocus(GetDlgItem(hDlg, IDC_IBEDIT));
			break;
		}
		case WM_CLOSE:
		{

			EndDialog(hDlg, LOWORD(wParam));
			bInputBoxOpen = false;
		}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion InputBoxProc


#pragma region PasswordProc

INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_INITDIALOG:
		{
				SetWindowText(hDlg,ptLang.get("language.Password.Title",_T("Enter Password")).c_str());
				if(bSetPassword){
					SetWindowText(GetDlgItem(hDlg, IDC_PWD), ptLang.get("language.Password.SetText",_T("Enter the password which will be used to encrypt your config file and restrict access to the configuration window.")).c_str());
				} else {
					SetWindowText(GetDlgItem(hDlg, IDC_PWD), ptLang.get("language.Password.EnterText",_T("You must enter your password to start StrokesPlus or access the configuration window.")).c_str());
				}
				SetWindowText(GetDlgItem(hDlg, IDC_PWDLABEL), ptLang.get("language.Password.Password",_T("Password:")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.Password.OK",_T("OK")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDCANCEL), ptLang.get("language.Password.Cancel",_T("Cancel")).c_str());
				SetForegroundWindow(hDlg);
				bPasswordOpen = true;
			return (INT_PTR)TRUE;
		}
		case WM_COMMAND:
		{
				if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{

					if(LOWORD(wParam) == IDOK){
						static TCHAR szString[100];
						GetDlgItemText(hDlg, IDC_PWDEDIT, szString, 90);
						size_t len;
						size_t maxsize = 100;
						len = _tcsnlen(szString, maxsize);
						if(bSetPassword){
							if(len > 0) {							
								sPassword = szString;
								saveConfig();
							}
						} else if(bCheckPassword) {
							if(_tcscmp(sPassword.c_str(), szString) == 0){
								bPasswordMatch = true;
							} else {
								MessageBox(hDlg, ptLang.get("language.Password.MatchFailMessage",_T("Incorrect password.")).c_str(),ptLang.get("language.Password.MatchFailTitle",_T("Oops!.")).c_str(), MB_ICONWARNING + MB_APPLMODAL);
								bPasswordMatch = false;
							}
						} else {
							sPassword = szString;
						}
					}

					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}
				break;
		}
		case WM_CLOSE:
		{

			EndDialog(hDlg, LOWORD(wParam));
			bPasswordOpen = false;
		}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion PasswordProc


#pragma region ChangePasswordProc

INT_PTR CALLBACK ChangePasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_INITDIALOG:
		{
				SetWindowText(hDlg,ptLang.get("language.Password.ChangeTitle",_T("Change Password")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_PWD), ptLang.get("language.Password.ChangeText",_T("Enter the current password and a new one. If the New Password field is left blank, password protection will be removed.")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_PWDLABEL), ptLang.get("language.Password.NewPassword",_T("New Password:")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_OLDPWDLABEL), ptLang.get("language.Password.CurrentPassword",_T("Current Password:")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.Password.OK",_T("OK")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDCANCEL), ptLang.get("language.Password.Cancel",_T("Cancel")).c_str());
				SetForegroundWindow(hDlg);
				bPasswordOpen = true;
			return (INT_PTR)TRUE;
		}
		case WM_COMMAND:
		{
				if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{

					if(LOWORD(wParam) == IDOK){
						static TCHAR szString[100];
						static TCHAR szOldString[100];
						GetDlgItemText(hDlg, IDC_OLDPWDEDIT, szOldString, 90);
						GetDlgItemText(hDlg, IDC_PWDEDIT, szString, 90);
						size_t len;
						size_t maxsize = 100;
						len = _tcsnlen(szString, maxsize);
						size_t lenOld;
						size_t maxsizeOld = 100;
						lenOld= _tcsnlen(szOldString, maxsizeOld);
						if(lenOld > 0) {	
							if(_tcscmp (sPassword.c_str(), szOldString) == 0){
								string sNewPass = szString;
								changePassword(sNewPass);
								saveConfig();
							} else {
								MessageBox(hDlg, ptLang.get("language.Password.MatchFailMessage",_T("Incorrect password.")).c_str(),ptLang.get("language.Password.MatchFailTitle",_T("Oops!")).c_str(), MB_ICONWARNING + MB_APPLMODAL);
							}
						} else {
							MessageBox(hDlg, ptLang.get("language.Password.MatchFailMessage",_T("Incorrect password.")).c_str(),ptLang.get("language.Password.MatchFailTitle",_T("Oops!")).c_str(), MB_ICONWARNING + MB_APPLMODAL);
						}
					}

					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}
				break;
		}
		case WM_CLOSE:
		{

			EndDialog(hDlg, LOWORD(wParam));
			bPasswordOpen = false;
		}

	}
	return (INT_PTR)FALSE;
}

#pragma endregion ChangePasswordProc


#pragma region SettingsProc

INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_INITDIALOG:
			{

				hWndSettings = hDlg;

				ShowCursor(FALSE); 
				SetCursor(LoadCursor(NULL,IDC_WAIT)); 
				ShowCursor(TRUE); 

				INITCOMMONCONTROLSEX InitCtrlEx;
				InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
				InitCtrlEx.dwICC = ICC_STANDARD_CLASSES | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES | ICC_USEREX_CLASSES | ICC_TAB_CLASSES;
				InitCommonControlsEx(&InitCtrlEx);
				
				HICON hIconMain = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
				SetClassLongPtr(hDlg, -34, (LONG)hIconMain); //GCL_HICONSM = -34, for some reason it was undeclared, not screwing with it at the moment
				SetClassLongPtr(hDlg, -14, (LONG)hIconMain); //GCL_HICON = -14, for some reason it was undeclared, not screwing with it at the moment

				SetWindowText(hDlg,ptLang.get("language.Settings.Title",_T("StrokesPlus")).c_str());
				SetWindowText(GetDlgItem(hDlg, ID_S_OK), ptLang.get("language.Preferences.OK",_T("OK")).c_str());
				SetWindowText(GetDlgItem(hDlg, ID_S_CANCEL), ptLang.get("language.Preferences.Cancel",_T("Cancel")).c_str());
				SetWindowText(GetDlgItem(hDlg, ID_S_APPLY), ptLang.get("language.Preferences.Save",_T("Apply")).c_str());
				SetWindowText(GetDlgItem(hDlg, ID_S_HELP), ptLang.get("language.Preferences.Help",_T("Help")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_S_AUTOSAVE),ptLang.get("language.Actions.FormButtons.Autosave",_T("Auto Save")).c_str());

				hTabControlMain = GetDlgItem(hDlg,IDC_TABMAIN);
				TCITEM ti;
				ti.mask = TCIF_TEXT; 
				TCHAR sDesc[255];

				SetWindowLong(hTabControlMain, GWL_EXSTYLE, GetWindowLong(hTabControlMain, GWL_EXSTYLE) | WS_EX_CONTROLPARENT);

				_tcscpy (sDesc,ptLang.get("language.Actions.Title",_T("Configure Actions")).c_str());
				TCHAR *cActionDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cActionDesc;
				TabCtrl_InsertItem(hTabControlMain,0,&ti);
				
				_tcscpy (sDesc,ptLang.get("language.Hotkeys.Title",_T("Configure Hotkeys")).c_str());
				TCHAR *cHotkeyDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cHotkeyDesc;
				TabCtrl_InsertItem(hTabControlMain,1,&ti);

				_tcscpy (sDesc,ptLang.get("language.Ignored.Title",_T("Ignored List")).c_str());
				TCHAR *cIgnoredDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cIgnoredDesc;
				TabCtrl_InsertItem(hTabControlMain,2,&ti);

				_tcscpy (sDesc,ptLang.get("language.Preferences.Title",_T("Preferences")).c_str());
				TCHAR *cPrefsDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cPrefsDesc;
				TabCtrl_InsertItem(hTabControlMain,3,&ti);

				_tcscpy (sDesc,ptLang.get("language.Report.Title",_T("Action Report")).c_str());
				TCHAR *cReportDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cReportDesc;
				TabCtrl_InsertItem(hTabControlMain,4,&ti); 

				_tcscpy (sDesc,ptLang.get("language.LuaTab.Title",_T("Global Lua")).c_str());
				TCHAR *cLuaDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cLuaDesc;
				TabCtrl_InsertItem(hTabControlMain,5,&ti); 

				_tcscpy (sDesc,ptLang.get("language.About.Title",_T("About")).c_str());
				TCHAR *cAboutDesc = const_cast<TCHAR*> ( sDesc );
				ti.pszText = cAboutDesc;
				TabCtrl_InsertItem(hTabControlMain,6,&ti); 

				InitialiseEditor();
				InitialiseLuaEditor();
				InitialiseHotkeyEditor();

				hActionsTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABACTIONS),hTabControlMain,ConfigTab); 
				SetWindowText(hActionsTab,ptLang.get("language.Actions.Title",_T("Configure Actions")).c_str());
				SetParent(m_hwndEditor, hActionsTab);
				hIgnoredTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABIGNORED),hTabControlMain,IgnoredTab);
				SetWindowText(hIgnoredTab,ptLang.get("language.Ignored.Title",_T("Ignored List")).c_str());
				hHotkeyTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABHOTKEYS),hTabControlMain,HotkeyTab);
				SetWindowText(hHotkeyTab,ptLang.get("language.Hotkeys.Title",_T("Configure Hotkeys")).c_str());
				SetParent(m_hwndHotkeyEditor, hHotkeyTab);
				hPreferencesTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABPREFS),hTabControlMain,PreferencesTab); 
				SetWindowText(hPreferencesTab,ptLang.get("language.Preferences.Title",_T("Preferences")).c_str());
				hActionsReportTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABREPORT),hTabControlMain,ReportTab);
				SetWindowText(hActionsReportTab,ptLang.get("language.Report.Title",_T("Action Report")).c_str());
				hLuaTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABLUA),hTabControlMain,LuaTab);
				SetWindowText(hLuaTab,ptLang.get("language.LuaTab.Title",_T("Global Lua")).c_str());
				SetParent(m_hwndLuaEditor, hLuaTab);
				hAboutTab = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TABABOUT),hTabControlMain,AboutTab);
				SetWindowText(hAboutTab,ptLang.get("language.About.Title",_T("About")).c_str());
				TabCtrl_SetCurSel(hTabControlMain,iSelectedTab);

				LoadLuaFile();

				if(bAutosave){
					SendDlgItemMessage(hDlg, IDC_S_AUTOSAVE, BM_SETCHECK, BST_CHECKED, 0);
				}

				//resize the window, since the criteria fields have been moved to the left
				RECT lpRect;
				GetWindowRect(hDlg,&lpRect);

				SetWindowPos(hDlg, HWND_NOTOPMOST, lpRect.left, lpRect.top, iConfigWidth, iConfigHeight, SWP_NOACTIVATE);
				POINT tmp;
				tmp.x = 0;
				tmp.y = 0;
				acCenterWindowToScreen(hDlg, tmp, TRUE);

				if(bSettingsMaximized){
					ShowWindow(hDlg, SW_MAXIMIZE);
				}

				setSelectedTab(true);

				SetForegroundWindow(hDlg);

				bSettingsOpen = true;

				return (INT_PTR)TRUE;
			}

		case WM_NOTIFY:
		{
			switch(((LPNMHDR)lParam)->code)
			{
				/*case TCN_SELCHANGING: 
				{
					return TRUE;
				}*/
				
				case TCN_SELCHANGE:
				{
					iSelectedTab = TabCtrl_GetCurSel(hTabControlMain);
					setSelectedTab(true);
					return TRUE;
				}
			}
			return TRUE;
		}
		
		case WM_SIZING:
		{
			ShowWindow(hActionsTab, SW_HIDE);
			ShowWindow(hAboutTab, SW_HIDE);
			ShowWindow(hIgnoredTab, SW_HIDE);
			ShowWindow(hHotkeyTab, SW_HIDE);
			ShowWindow(hPreferencesTab, SW_HIDE);
			ShowWindow(hActionsReportTab, SW_HIDE);
			ShowWindow(hLuaTab, SW_HIDE);
			break;
		}
		
		case WM_STYLECHANGED:
			{
				PostMessage(hDlg, WM_EXITSIZEMOVE, 0, 0);
				return (INT_PTR)TRUE;
				break;
			}
		case WM_EXITSIZEMOVE:
		{	
			setSelectedTab(false);
			if(IsWindowVisible(hPreferencesTab)){
				drawStrokeColor(hPreferencesTab);
			}
			if(IsWindowVisible(hActionsTab)){
				drawConfigGesture(hActionsTab);
			}
			return (INT_PTR)FALSE;
		}
		case WM_COMMAND:
		{
			
			if((LOWORD(wParam) == WM_SETTINGSPREVTAB) || (LOWORD(wParam) == WM_SETTINGSNEXTTAB)) {
				if (LOWORD(wParam) == WM_SETTINGSPREVTAB){
					if(iSelectedTab==0){
						iSelectedTab = TabCtrl_GetItemCount(hTabControlMain)-1;
					} else {
						iSelectedTab--;
					}
				}
				if (LOWORD(wParam) == WM_SETTINGSNEXTTAB){
					if(iSelectedTab==TabCtrl_GetItemCount(hTabControlMain)-1){
						iSelectedTab = 0;
					} else {
						iSelectedTab++;
					}
				} 
				TabCtrl_SetCurSel(hTabControlMain,iSelectedTab);
				setSelectedTab(true);
			}

			if (LOWORD(wParam) == IDC_S_AUTOSAVE){
				bAutosave = SendDlgItemMessage(hDlg, IDC_S_AUTOSAVE, BM_GETCHECK, 0, 0);
				return TRUE;
			}

			if (LOWORD(wParam) == ID_S_HELP)
			{
				EnableWindow(GetDlgItem(hDlg, ID_S_HELP),false);
				openHelp();
				EnableWindow(GetDlgItem(hDlg, ID_S_HELP),true);
			}
			if (LOWORD(wParam) == ID_S_OK || LOWORD(wParam) == ID_S_APPLY)
			{
				iActionLuaZoomFactor = (int)SendEditor( SCI_GETZOOM );
				iGlobalLuaZoomFactor = (int)SendLuaEditor( SCI_GETZOOM );
				iHotkeyLuaZoomFactor = (int)SendHotkeyEditor( SCI_GETZOOM );
				if(iActionLuaZoomFactor < -10) iActionLuaZoomFactor = -10;
				if(iActionLuaZoomFactor > 20) iActionLuaZoomFactor = 20;
				if(iHotkeyLuaZoomFactor < -10) iHotkeyLuaZoomFactor = -10;
				if(iHotkeyLuaZoomFactor > 20) iHotkeyLuaZoomFactor = 20;
				if(iGlobalLuaZoomFactor < -10) iGlobalLuaZoomFactor = -10;
				if(iGlobalLuaZoomFactor > 20) iGlobalLuaZoomFactor = 20;

				savePreferences(hPreferencesTab);
				SaveConfigChange(hActionsTab, TreeView_GetSelection(GetDlgItem(hActionsTab , IDC_TREEAPPS)));
				SaveHotkeyChange(hHotkeyTab,TreeView_GetSelection(GetDlgItem(hHotkeyTab , IDC_TREEHOTKEYS)));
				SaveIgnoredChange(hIgnoredTab,TreeView_GetSelection(GetDlgItem(hIgnoredTab , IDC_TREEIGNOREDAPPS)));
				SetModifierStates(hWndConfig);
				SaveLuaFile();
				reloadLuaState();
			}
			if (LOWORD(wParam) == ID_S_OK)
			{
				//save config file here
				bSettingsMaximized = IsZoomed(hDlg); 
				saveConfig();
			}
			if (LOWORD(wParam) == ID_S_OK || LOWORD(wParam) == ID_S_CANCEL)
			{
				//post to main thread to ensure allocation of variables aren't lost within the Settings window thread scope
				PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_LOADCONFIG, 1), 0); 
				//loadHookConfig();
				PostMessage(hDlg, WM_CLOSE, 0, 0);
			}
			break;
		}
		case WM_WINDOWPOSCHANGED:
		{
			if ((((WINDOWPOS*)lParam)->flags & SWP_SHOWWINDOW) && !g_ConfigShown && bSettingsOpen) {
				g_ConfigShown = TRUE;
				bSaveActionSize = TRUE;
				PostMessage(hDlg, WM_EXITSIZEMOVE, 0, 0);
			}
			resizeSettings();

			if(IsWindowVisible(hWndConfig)){
				ResizeConfigWindow(hWndConfig);
			}
			if(IsWindowVisible(hLuaTab)){
				ResizeLuaWindow(hLuaTab);
			}
			if(IsWindowVisible(hHotkeyTab)){
				ResizeHotkeyWindow(hHotkeyTab);
			}
			if(IsWindowVisible(hWndIgnore)){
				ResizeIgnoreWindow(hWndIgnore);
			}
			if(IsWindowVisible(hWndReport)){
				ResizeReportWindow(hWndReport);
			}
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO * mmiStruct = (MINMAXINFO*)lParam;
 
			POINT ptPoint;
 
			ptPoint.x = 1014;    //Minimum width of the window.
			ptPoint.y = 703;    //Minimum height of the window.
			mmiStruct->ptMinTrackSize = ptPoint;
 
			ptPoint.x = GetSystemMetrics(SM_CXMAXIMIZED);   //Maximum width of the window.
			ptPoint.y = GetSystemMetrics(SM_CYMAXIMIZED);   //Maximum height of the window.
			mmiStruct->ptMaxTrackSize = ptPoint;
 
			return 0;
		}
		case WM_CLOSE:
			{
				rightClickCopyNode = NULL;
				rightClickCutMode = FALSE;
				ImageList_Destroy(hList);
				hList = NULL;
				ImageList_Destroy(hListTree);
				hListTree = NULL;
				DestroyWindow(hActionsTab);
				DestroyWindow(hPreferencesTab);
				DestroyWindow(hIgnoredTab);
				DestroyWindow(hHotkeyTab);
				DestroyWindow(hActionsReportTab);
				DestroyWindow(hAboutTab);
				DestroyWindow(m_hwndEditor);
				DestroyWindow(m_hwndLuaEditor);
				DestroyWindow(m_hwndHotkeyEditor);
				bSettingsOpen = false;
				bNameOpen = FALSE;
				bIgnoreName = FALSE;
				bIgnoreRename = FALSE;
				bConfigNameApp = FALSE;
				bConfigRenameApp = FALSE;
				bConfigNameAction = FALSE;
				bConfigRenameAction = FALSE;
				bConfigRenameGesture = FALSE;
				hWndSettings = NULL;
				HotkeyNames.clear();
				HotkeyValues.clear();
				EndDialog(hDlg,0);
				setLastActive();
			}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion SettingsProc


#pragma region NameProc

INT_PTR CALLBACK NameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_INITDIALOG:
			{
				SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
				SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
				//SetWindowText(hDlg, ptLang.get("language.Name.Title",_T("Name").c_str());
				SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.Name.OK",_T("OK")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDCANCEL), ptLang.get("language.Name.Cancel",_T("Cancel")).c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_NAMELABEL), ptLang.get("language.Name.Name",_T("Name:")).c_str());
				
				hWndName = hDlg;
				bNameOpen = TRUE;

				HTREEITEM hSelectedParent;
				TVITEM tvi;
				ZeroMemory(&tvi,sizeof(tvi));
				tvi.mask=TVIF_TEXT;
				TCHAR cText[255] = {0};
				tvi.pszText = cText;
				tvi.cchTextMax=256;

				if(bIgnoreName) {
					//nothing to init
					SetWindowText(hDlg, ptLang.get("language.Ignored.AddPopupTitle",_T("Add Ignored Window")).c_str());
					return (INT_PTR)TRUE;
				} else if(bIgnoreRename) {
					SetWindowText(hDlg, ptLang.get("language.Ignored.RenamePopupTitle",_T("Rename Ignored Window")).c_str());
					HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS));
					tvi.hItem=hSelected;
					SendMessage(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
					SetDlgItemText (hDlg, IDC_NAMEEDIT, cText);
					SendDlgItemMessage(hDlg, IDC_NAMEEDIT, EM_SETSEL, 0, -1);
					return (INT_PTR)TRUE;
				} else if(bConfigNameApp){
					SetWindowText(hDlg, ptLang.get("language.Actions.AddAppPopupTitle",_T("Add Application")).c_str());
					//nothing to init
					return (INT_PTR)TRUE;
				} else if(bConfigRenameApp){
					SetWindowText(hDlg, ptLang.get("language.Actions.RenameAppPopupTitle",_T("Rename Application")).c_str());
					HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
					tvi.hItem=hSelected;
					SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
					SetDlgItemText (hDlg, IDC_NAMEEDIT, cText);
					SendDlgItemMessage(hDlg, IDC_NAMEEDIT, EM_SETSEL, 0, -1);
					return (INT_PTR)TRUE;
				} else if(bConfigNameAction){
					SetWindowText(hDlg, ptLang.get("language.Actions.AddActionPopupTitle",_T("Add Action")).c_str());
					//nothing to init
					return (INT_PTR)TRUE;
				} else if(bConfigRenameAction){
					SetWindowText(hDlg, ptLang.get("language.Actions.RenameActionPopupTitle",_T("Rename Action")).c_str());
					HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
					tvi.hItem=hSelected;
					SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
					SetDlgItemText (hDlg, IDC_NAMEEDIT, cText);
					SendDlgItemMessage(hDlg, IDC_NAMEEDIT, EM_SETSEL, 0, -1);
					return (INT_PTR)TRUE;
				} else if (bConfigRenameGesture){
					SetWindowText(hDlg, ptLang.get("language.Actions.RenameGesturePopupTitle",_T("Rename Gesture")).c_str());
					CHAR szGestureName[255];
					memset(szGestureName,0,sizeof(szGestureName));
					int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
					SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);
					SetDlgItemText (hDlg, IDC_NAMEEDIT, szGestureName);
					SendDlgItemMessage(hDlg, IDC_NAMEEDIT, EM_SETSEL, 0, -1);
				} else if(bConfigNameHotkey){
					SetWindowText(hDlg, ptLang.get("language.Hotkeys.AddHotkeyPopupTitle",_T("Add Hotkey")).c_str());
					//nothing to init
					return (INT_PTR)TRUE;
				} else if(bConfigRenameHotkey){
					SetWindowText(hDlg, ptLang.get("language.Hotkeys.RenameHotkeyPopupTitle",_T("Rename Hotkey")).c_str());
					HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS));
					tvi.hItem=hSelected;
					SendMessage(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
					SetDlgItemText (hDlg, IDC_NAMEEDIT, cText);
					SendDlgItemMessage(hDlg, IDC_NAMEEDIT, EM_SETSEL, 0, -1);
					return (INT_PTR)TRUE;
				}
			}

		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE){
				wParam = NULL;
				break;
			}
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				if(LOWORD(wParam) == IDOK)
				{
					HTREEITEM hNewItem;
					static TCHAR szString[100];
					GetDlgItemText(hDlg, IDC_NAMEEDIT, szString, 90);
					size_t len;
					size_t maxsize = 100;
					len = _tcsnlen(szString, maxsize);
					if(len > 0) {	
					
						//If it's the same text as when the Name window was opened, just exit
						if(bConfigRenameApp || bConfigRenameAction || bIgnoreRename || bConfigRenameGesture || bConfigRenameHotkey){
							HTREEITEM hSelectedParent;
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							if(bIgnoreRename){
								HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS));
								tvi.hItem=hSelected;
								SendMessage(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
							} else if(bConfigRenameApp || bConfigRenameAction) {
								HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
								tvi.hItem=hSelected;
								SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
							} else if (bConfigRenameGesture){
								int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
								SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&cText);
							} else if (bConfigRenameHotkey){
								HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS));
								tvi.hItem=hSelected;
								SendMessage(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
							}
							if(bConfigRenameGesture) {
								if(_tcscmp(szString,cText) == 0) {
									PostMessage(hDlg, WM_CLOSE, 0, 0);
									return FALSE;
									break;
								}
							} else {
								if(_tcsicmp(szString,cText) == 0) {
									PostMessage(hDlg, WM_CLOSE, 0, 0);
									return FALSE;
									break;
								}
							}
						}

						boost::property_tree::ptree child;
						boost::property_tree::ptree match;
						boost::property_tree::ptree actions;

						std::basic_string<TCHAR> sName = szString; 
						boost::replace_all(sName, _T("\""), _T(" "));

						if(bIgnoreName) {

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Ignored"))) {
								//each ignored window
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.IgnoredExists",_T("An ignored entry with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.IgnoredExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							match.put("<xmlattr>.name",sName.c_str());
							match.put("<xmlattr>.enabled",1);
							child.put("ownerclass", _T(""));
							child.put("ownerclassregex", _T(""));
							child.put("ownertitle", _T(""));
							child.put("ownertitleregex", _T(""));
							child.put("parentclass", _T(""));
							child.put("parentclassregex", _T(""));
							child.put("parenttitle", _T(""));
							child.put("parenttitleregex", _T(""));
							child.put("controlclass", _T(""));
							child.put("controlclassregex", _T(""));
							child.put("controltitle", _T(""));
							child.put("controltitleregex", _T(""));
							child.put("controlid", _T(""));
							child.put("file", _T(""));
							child.put("fileregex", _T(""));
							child.put("pathregex", _T(""));
							match.add_child(_T("MatchCriteria"), child);
							ptConfig.add_child(_T("config.Ignored.Ignore"), match); 

							TV_INSERTSTRUCT tvis ;
							ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
							tvis.hParent = NULL ;
							tvis.hInsertAfter = TVI_LAST ;
							tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
							tvis.item.cchTextMax = 256 ;
							sLastIgnore= sName;
							TCHAR sDesc[255];
							tvis.hParent = NULL;//hIgnored;
							_tcscpy (sDesc,sName.c_str());
							TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
							tvis.item.pszText = cDesc;
							tvis.item.lParam =(LPARAM) sDesc;
							tvis.item.cChildren = 0 ;
							hNewItem = TreeView_InsertItem (GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS), &tvis) ;
							EnableWindow(GetDlgItem(hWndIgnore , IDC_IGNBTNDELETE),true);
							EnableWindow(GetDlgItem(hWndIgnore , IDC_IGNBTNRENAME),true);

						} else if(bIgnoreRename) {
							HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS));
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT;
							tvi.hItem=hSelected;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Ignored"))) {
								//each ignored window
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.IgnoredExists",_T("An ignored entry with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.IgnoredExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Ignored")) {
								//each ignored window
								if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									v2.second.put("<xmlattr>.name",sName.c_str());
									_tcscpy(cText,sName.c_str());
									tvi.pszText = cText;
									sLastIgnore= sName;
									TreeView_SetItem(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS),&tvi);
								}
							}

						} else if(bConfigNameHotkey){

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
								//each ignored window
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.HotkeyExists",_T("A hotkey with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.HotkeyExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							match.put("<xmlattr>.description",sName.c_str());
							match.put("<xmlattr>.enabled",1);
							match.put("Modifier1", -1);
							match.put("Modifier2", -1);
							match.put("Modifier3", -1);
							match.put("Modifier4", -1);
							match.put("Key", -1);
							ptConfig.add_child(_T("config.Global.Hotkeys.Hotkey"), match); 

							TV_INSERTSTRUCT tvis ;
							ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
							tvis.hParent = NULL ;
							tvis.hInsertAfter = TVI_LAST ;
							tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE;// | TVIF_SELECTEDIMAGE;
							tvis.item.cchTextMax = 256 ;
							sLastHotkey= sName;
							TCHAR sDesc[255];
							_tcscpy (sDesc,sName.c_str());
							TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
							tvis.item.pszText = cDesc;
							tvis.item.lParam =(LPARAM) sDesc;
							tvis.item.cChildren = 1 ;

							hNewItem = TreeView_InsertItem (GetDlgItem(hWndHotkey, IDC_TREEHOTKEYS), &tvis) ;
							EnableWindow(GetDlgItem(hWndHotkey , IDC_HKBTNDELETE),true);
							EnableWindow(GetDlgItem(hWndHotkey , IDC_HKBTNRENAME),true);
							SendDlgItemMessage(hWndConfig, IDC_CHKHKACTIVE, BM_SETCHECK, BST_CHECKED, 0);
							
							//break;

						} else if(bConfigRenameHotkey){

							HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS));
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT | TVIF_IMAGE;// | TVIF_SELECTEDIMAGE;
							tvi.hItem=hSelected;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0 /*&& v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.HotkeyExists",_T("A hotkey with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.HotkeyExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
								//each ignored window
								if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									v2.second.put("<xmlattr>.description",sName.c_str());
									_tcscpy(cText,sName.c_str());
									tvi.pszText = cText;
									sLastHotkey = sName;
									TreeView_SetItem(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS),&tvi);
								}
							}

						} else if(bConfigNameApp){

							if(_tcsicmp( sName.c_str(), ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str()) == 0) {
								sName.append(" 2");
							}

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
								//each ignored window
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.AppExists",_T("An application with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.AppExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							match.put("<xmlattr>.name",sName.c_str());
							match.put("<xmlattr>.enabled",1);
							child.put("ownerclass", _T(""));
							child.put("ownerclassregex", _T(""));
							child.put("ownertitle", _T(""));
							child.put("ownertitleregex", _T(""));
							child.put("parentclass", _T(""));
							child.put("parentclassregex", _T(""));
							child.put("parenttitle", _T(""));
							child.put("parenttitleregex", _T(""));
							child.put("controlclass", _T(""));
							child.put("controlclassregex", _T(""));
							child.put("controltitle", _T(""));
							child.put("controltitleregex", _T(""));
							child.put("controlid", _T(""));
							child.put("file", _T(""));
							child.put("fileregex", _T(""));
							child.put("pathregex", _T(""));
							match.add_child(_T("MatchCriteria"), child);
							match.put("Actions",_T(""));
							ptConfig.add_child(_T("config.Applications.Application"), match); 

							TV_INSERTSTRUCT tvis ;
							ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
							tvis.hParent = NULL ;
							tvis.hInsertAfter = TVI_LAST ;
							tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
							tvis.item.cchTextMax = 256 ;
							sLastParent = sName;
							sLastChild = _T("");
							TCHAR sDesc[255];
							_tcscpy (sDesc,sName.c_str());
							TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
							tvis.item.pszText = cDesc;
							tvis.item.lParam =(LPARAM) sDesc;
							tvis.item.cChildren = 1 ;
							tvis.item.iImage = 1;
							tvis.item.iSelectedImage = 1;

							hNewItem = TreeView_InsertItem (GetDlgItem(hWndConfig , IDC_TREEAPPS), &tvis) ;
							//EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_RENG),FALSE);	
							//EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_DELG),FALSE);	
							EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNDELETE),true);
							EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNRENAME),true);
							SendDlgItemMessage(hWndConfig, IDC_CHKAPPACTIVE, BM_SETCHECK, BST_CHECKED, 0);
							
							//break;
					
						} else if(bConfigRenameApp){

							if(_tcsicmp( sName.c_str(), ptLang.get("language.Actions.GlobalActions",_T("Global Actions")).c_str()) == 0) {
								sName.append(" 2");
							}

							HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
							tvi.hItem=hSelected;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
								if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 /*&& v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.AppExists",_T("An application with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.AppExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
								//each ignored window
								if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
									v2.second.put("<xmlattr>.name",sName.c_str());
									_tcscpy(cText,sName.c_str());
									tvi.pszText = cText;
									for(int i = 0; i < GestureNames.size(); i++) { 
										if(_tcscmp( GestureNames[i].c_str(), v2.second.get("GestureName",_T("")).c_str() ) == 0 ){
											tvi.iImage = i;
											tvi.iSelectedImage = i;
											break;
										}
									}
									sLastParent = sName;
									sLastChild = _T("");
									TreeView_SetItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),&tvi);
								}
							}

						} else if(bConfigNameAction){

							HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
							HTREEITEM hSelectedParent;
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
							tvi.hItem=hSelected;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
							if(tvi.cChildren == 0){
								//GET PARENT OF SELECTED
								hSelectedParent = TreeView_GetParent(GetDlgItem(hWndConfig , IDC_TREEAPPS),hSelected);
								TVITEM parenttvi;
								ZeroMemory(&parenttvi,sizeof(parenttvi));
								parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN;
								parenttvi.hItem=hSelectedParent;
								parenttvi.pszText = cText;
								parenttvi.cchTextMax=256;
								SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&parenttvi);
							}
							BOOL bAppMatch = FALSE;
							//add app action
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
								if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {

									BOOST_FOREACH(boost::property_tree::ptree::value_type &v4, v2.second.get_child("Actions")) {
										//each ignored window
										if(_tcsicmp( sName.c_str(), v4.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v4.second.get("<xmlattr>.enabled",0) == 1*/) {
											MessageBox(hDlg, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
											return FALSE;
											break;
										}
									}

									TV_INSERTSTRUCT tvis ;
									ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
									tvis.hParent = (tvi.cChildren == 0 ? hSelectedParent : hSelected);
									tvis.hInsertAfter = TVI_LAST;
									tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
									tvis.item.cchTextMax = 256 ;
									TCHAR sDesc[255];
									_tcscpy (sDesc,sName.c_str());
									TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
									tvis.item.pszText = cDesc;
									tvis.item.lParam =(LPARAM) sDesc;
									tvis.item.cChildren = 0 ;
									tvis.item.iImage = 0;
									tvis.item.iSelectedImage = 0;
									HTREEITEM hNewItem = TreeView_InsertItem (GetDlgItem(hWndConfig , IDC_TREEAPPS), &tvis) ;
									TreeView_SelectItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),hNewItem);
									//sLastChild = sName;

									BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
										if(v3.first == _T("Actions")) {
											child.put("<xmlattr>.description",sName.c_str());
											child.put("<xmlattr>.enabled",1);
											child.put("GestureName", _T(""));
											child.put("Control", _T("0"));
											child.put("Alt", _T("0"));
											child.put("Shift", _T("0"));
											child.put("Right", _T("0"));
											child.put("Middle", _T("0"));
											child.put("Left", _T("0"));
											child.put("X1", _T("0"));
											child.put("X2", _T("0"));
											child.put("WheelUp", _T("0"));
											child.put("WheelDown", _T("0"));
											child.put("StrokeState", _T("0"));
											child.put("Lua", _T(""));
											v3.second.add_child("Action", child);
											bAppMatch = TRUE;
										}
									}
									SendDlgItemMessage(hWndConfig, IDC_CHKACTACTIVE, BM_SETCHECK, BST_CHECKED, 0);
									SetWindowText(GetDlgItem(hWndConfig, IDC_EDITLUA), _T(""));
									ResetModifierStates(hActionsTab);
									SendEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)_T(""));
									EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_RENG),FALSE);	
									EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_DELG),FALSE);	
									EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNDELETE),true);
									EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNRENAME),true);
									SendMessage(hWndCB, CB_SETCURSEL, 0, 0);
								}
							}
							if(!bAppMatch) {

								BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child(_T("config.Global.Actions"))) {
									//each ignored window
									if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
										MessageBox(hDlg, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
										return FALSE;
										break;
									}
								}

								TV_INSERTSTRUCT tvis ;
								ZeroMemory (&tvis, sizeof(TV_INSERTSTRUCT)) ;
								HTREEITEM hGlobalNode;
								if(hSelectedParent == NULL || hSelected == NULL)
								{
									hGlobalNode = TreeView_GetFirstVisible(GetDlgItem(hWndConfig , IDC_TREEAPPS));
									tvis.hParent = hGlobalNode;
								} else {
									tvis.hParent = (tvi.cChildren == 0 ? hSelectedParent : hSelected);
								}
								tvis.hInsertAfter = TVI_LAST;
								tvis.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
								tvis.item.cchTextMax = 256 ;
								TCHAR sDesc[255];
								_tcscpy (sDesc,sName.c_str());
								TCHAR *cDesc = const_cast<TCHAR*> ( sDesc );
								tvis.item.pszText = cDesc;
								tvis.item.lParam =(LPARAM) sDesc;
								tvis.item.cChildren = 0 ;
								tvis.item.iImage = 0;
								tvis.item.iSelectedImage = 0;
								HTREEITEM hNewItem = TreeView_InsertItem (GetDlgItem(hWndConfig , IDC_TREEAPPS), &tvis) ;
								TreeView_SelectItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),hNewItem);
								//sLastChild = sName;
								child.put("<xmlattr>.description",sName.c_str());
								child.put("<xmlattr>.enabled",1);
								child.put("GestureName", _T(""));
								child.put("Control", "0");
								child.put("Alt", "0");
								child.put("Shift", "0");
								child.put("Right", "0");
								child.put("Middle", "0");
								child.put("Left", "0");
								child.put("X1", "0");
								child.put("X2", "0");
								child.put("WheelUp", "0");
								child.put("WheelDown", "0");
								child.put("StrokeState", "0");
								child.put("Lua", _T(""));
								ptConfig.add_child("config.Global.Actions.Action", child);

								SendDlgItemMessage(hWndConfig, IDC_CHKACTACTIVE, BM_SETCHECK, BST_CHECKED, 0);
								SetWindowText(GetDlgItem(hWndConfig, IDC_EDITLUA), _T(""));
								ResetModifierStates(hActionsTab);
								SendEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)_T(""));
								EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_RENG),FALSE);	
								EnableWindow(GetDlgItem(hWndConfig , IDC_CFG_DELG),FALSE);	
								EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNDELETE),true);
								EnableWindow(GetDlgItem(hWndConfig , IDC_CFGBTNRENAME),true);
								SendMessage(hWndCB, CB_SETCURSEL, 0, 0);
							}

						} else if(bConfigRenameAction){

							HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hWndConfig , IDC_TREEAPPS));
							TVITEM tvi;
							ZeroMemory(&tvi,sizeof(tvi));
							tvi.mask=TVIF_TEXT;
							tvi.hItem=hSelected;
							TCHAR cText[255] = {0};
							tvi.pszText = cText;
							tvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

							HTREEITEM hSelectedParent = TreeView_GetParent(GetDlgItem(hWndConfig , IDC_TREEAPPS),hSelected);
							TVITEM parenttvi;
							ZeroMemory(&parenttvi,sizeof(parenttvi));
							parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
							parenttvi.hItem=hSelectedParent;
							TCHAR cParentText[255] = {0};
							parenttvi.pszText = cParentText;
							parenttvi.cchTextMax=256;
							SendMessage(GetDlgItem(hWndConfig , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&parenttvi);
							BOOL bAppMatch = FALSE;
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
								if(_tcsicmp( cParentText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {

									BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("Actions"))) {
										//each ignored window
										if(_tcsicmp( sName.c_str(), v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1*/) {
											MessageBox(hDlg, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
											return FALSE;
											break;
										}
									}

									BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
										//each ignored window

										if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1*/) {
											v3.second.put("<xmlattr>.description",sName.c_str());
											_tcscpy(cText,sName.c_str());
											tvi.pszText = cText;
											TreeView_SetItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),&tvi);
											bAppMatch = TRUE;
											sLastChild = sName;
										}
									}
								}
							}
							if(!bAppMatch) {
								BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Actions")) {
									//each ignored window
									if(_tcsicmp( sName.c_str(), v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
										MessageBox(hDlg, ptLang.get("language.Name.Messages.ActionExists",_T("An action with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.ActionExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
										return FALSE;
										break;
									}
								}

								BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Actions")) {

									if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
										v2.second.put("<xmlattr>.description",sName.c_str());
										_tcscpy(cText,sName.c_str());
										tvi.pszText = cText;
										TreeView_SetItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),&tvi);
										sLastChild = sName;
									}
								}
							}
						} else if (bConfigRenameGesture){

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Gestures")) {
								//each ignored window
								if(_tcscmp( sName.c_str(), v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 ) {
									MessageBox(hDlg, ptLang.get("language.Name.Messages.GestureExists",_T("A gesture with the same name already exists. \nPlease enter a different name.")).c_str(), ptLang.get("language.Name.Messages.GestureExistsTitle",_T("Name already exists")).c_str(), MB_ICONEXCLAMATION + MB_APPLMODAL);
									return FALSE;
									break;
								}
							}

							TCHAR szGestureName[255];
							memset(szGestureName,0,sizeof(szGestureName));
							int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
							SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);

							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Gestures")) {

								if(_tcscmp( szGestureName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0) {
									v2.second.put("<xmlattr>.name",sName.c_str());
								}
								for(int i = 0; i < GestureNames.size(); i++) { 
									if(_tcscmp( GestureNames[i].c_str(), szGestureName ) == 0 ){
										GestureNames[i] = sName;
										break;
									}
								}
							}
						}

						if(bIgnoreName)
							TreeView_SelectItem(GetDlgItem(hWndIgnore , IDC_TREEIGNOREDAPPS),hNewItem);

						if(bConfigNameApp)
							TreeView_SelectItem(GetDlgItem(hWndConfig , IDC_TREEAPPS),hNewItem);

						if(bConfigNameHotkey)
							TreeView_SelectItem(GetDlgItem(hWndHotkey , IDC_TREEHOTKEYS),hNewItem);

						if(bConfigRenameGesture){
							BuildGestureDropDown();
							int totalitems = SendMessage(hWndCB,CB_GETCOUNT, 0, 0);
							int loopct = 0;
							int startmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)szString);
							int lastmatchindex = startmatchindex;
							TCHAR szGestureName[255];
							SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
							if(_tcscmp( szString,szGestureName ) != 0)
							{
								if(startmatchindex != CB_ERR)
								{
									lastmatchindex++;
									while(lastmatchindex != startmatchindex && loopct < totalitems)
									{
										loopct++;
										lastmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,lastmatchindex-1,(LPARAM)szString);
										SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
										if(_tcscmp( szString,szGestureName ) == 0)
											break;
									}
								}
							}
							SendMessage(hWndCB, CB_SETCURSEL, lastmatchindex, 0);
							ShowWindow(hWndCB, SW_HIDE);
							ShowWindow(hWndCB, SW_SHOW);
						}

					}
					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}

				if(LOWORD(wParam) == IDCANCEL)
				{
					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}
			}
			break;
		}

		case WM_PAINT:
		{
			SetFocus(GetDlgItem(hDlg, IDC_NAMEEDIT));
			break;
		}

		case WM_CLOSE:
		{
			bIgnoreName = FALSE;
			bIgnoreRename = FALSE;
			bConfigNameApp = FALSE;
			bConfigRenameApp = FALSE;
			bConfigNameAction = FALSE;
			bConfigRenameAction = FALSE;
			bConfigRenameGesture = FALSE;
			bConfigNameHotkey = FALSE;
			bConfigRenameHotkey = FALSE;
			EndDialog(hDlg, LOWORD(wParam));
			//DestroyWindow(hDlg);
			SetForegroundWindow(hWndSettings);
			bNameOpen = FALSE;
		}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion NameProc


#pragma region GestureNameProc

INT_PTR CALLBACK GestureNameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		{
		SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		SetWindowText(hDlg, ptLang.get("language.SaveGesture.Title",_T("Save Gesture")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_GESTURENAME), ptLang.get("language.SaveGesture.NameGesture",_T("What would you like to name this gesture?")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_GESTURETEXT), ptLang.get("language.SaveGesture.Info",_T("Entering the same name as an existing gesture will add this pattern to the existing gesture.  \nHaving multiple samples will improve recognition.")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.SaveGesture.Save",_T("Save")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDCANCEL), ptLang.get("language.SaveGesture.Cancel",_T("Cancel")).c_str());
		setCancel(TRUE);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_GESTURENAME), LastDrawnGestureName.c_str());
		SendDlgItemMessage(hDlg, IDC_EDIT_GESTURENAME, EM_SETSEL, 0, -1);

		SetForegroundWindow(hDlg);

		if(!bGesturesDisabled){
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
		} else {
			clearMyHook(hWndServer);
		}

		bGestureOpen = TRUE;
		return (INT_PTR)TRUE;
		}

	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE){
			wParam = NULL;
			break;
		}
	}

	case WM_WINDOWPOSCHANGED:
		{
			if ((((WINDOWPOS*)lParam)->flags & SWP_SHOWWINDOW) && !g_GestureNameShown) {
				g_GestureNameShown = TRUE;
				SetFocus(GetDlgItem(hDlg, IDC_EDIT_GESTURENAME));
				PostMessage(hDlg, WM_EXITSIZEMOVE, 0, 0);
			}
			break;
		}

	case WM_EXITSIZEMOVE:
		{
			if(IsWindowVisible(hDlg)){
				RECT lpRect;
				lpRect.left = 4*fDPIModifier;
				lpRect.top = 4*fDPIModifier;
				lpRect.right = (lpRect.left + 156)*fDPIModifier;
				lpRect.bottom = (lpRect.top + 156)*fDPIModifier;
				InvalidateRect(hDlg, &lpRect, TRUE);
				UpdateWindow(hDlg);
				drawLastGestureToHandle(hDlg, 4*fDPIModifier, 4*fDPIModifier, -1, -1, false);
			}
			break;
		}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if(LOWORD(wParam) == IDOK)
			{
				static TCHAR szString[100];
				GetDlgItemText(hDlg, IDC_EDIT_GESTURENAME, szString, 90);
				size_t len;
				size_t maxsize = 100;
				len = _tcsnlen(szString, maxsize);
				if(len > 0) {	
					g_Points.clear();
					saveGesture(szString);
				} else {
					clearCaptureVars();
				}
				if(bSettingsOpen == FALSE && bGestureOpen == FALSE && bNameOpen == FALSE && bNewGestureMode == FALSE)
				{
					//setLastActive();
				} else {
					setLearningMode(bPreviousTrainingModeState);

					if(bSettingsOpen){
						int iSize = 28;
						SolidBrush whiteBrush(Color(255, 255, 255, 255));
						BOOST_FOREACH(boost::property_tree::ptree::value_type &vGest, ptConfig.get_child("config.Gestures")) {
							if(_tcscmp( szString, vGest.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 ){
								TempDrawPoints.clear();
								BOOST_FOREACH(boost::property_tree::ptree::value_type &v8, vGest.second) {
									if(v8.first == _T("PointPatterns")) {
										BOOST_FOREACH(boost::property_tree::ptree::value_type &v9, v8.second) {
											//each point pattern
											if(v9.first == _T("PointPattern")) {
												BOOST_FOREACH(boost::property_tree::ptree::value_type &v10, v9.second) {
													//each point 
													if(v10.first == _T("Point")) {
														POINTF Point;
														int x = (int)v10.second.get("<xmlattr>.x",0);
														int y = (int)v10.second.get("<xmlattr>.y",0);
														Point.x = (float)x;
														Point.y = (float)y;
														TempDrawPoints.push_back(Point);						
													}
												}
												break;
											}
										}
										Bitmap* result;
										result = new Bitmap(iSize, iSize,PixelFormat32bppRGB);
										Graphics gr(result);
										gr.FillRectangle(&whiteBrush, 0, 0, iSize, iSize);
										Pen bluePen(Color(255, 0,0,255), iSize/16);
										bluePen.SetLineCap(LineCapRoundAnchor, LineCapArrowAnchor, DashCapRound); 
										gr.SetSmoothingMode(SmoothingModeAntiAlias);
										gr.SetCompositingQuality ( CompositingQualityHighQuality ) ;
										gr.SetInterpolationMode( InterpolationModeHighQuality ) ; 
										ScaleGesture(iSize*.78, iSize*.78);
										int iLeftOffset = (int)floor((double)(((iSize*.78)/2)-(iScaledWidth / 2)));
										int iTopOffset = (int)floor((double)(((iSize*.78)/2)-(iScaledHeight / 2)));
										PointF *arrPoints = new PointF[ScaledDrawPoints.size()];
										for(int i = 0; i < ScaledDrawPoints.size(); i++) { 
											arrPoints[i].X = ScaledDrawPoints[i].x+(iSize*.079)+iLeftOffset;
											arrPoints[i].Y = ScaledDrawPoints[i].y+(iSize*.079)+iTopOffset;
										}
										gr.DrawLines(&bluePen, arrPoints, TempDrawPoints.size());

										HBITMAP handle = NULL;
										result->GetHBITMAP(Color::White, &handle);
										ImageList_Add(hListTree, handle, NULL);

										GestureNames.push_back(vGest.second.get("<xmlattr>.name",_T("")));

										delete[] arrPoints;
										delete result;
										DeleteObject(handle);
										break;
									}
								}
							}
						}
						//GestureNames.push_back(szString);

						static TCHAR sztString[100]; 
						if(!bNewGestureMode){
							SendMessage(hWndCB, CB_GETLBTEXT, SendMessage(hWndCB, CB_GETCURSEL, 0, 0),(LPARAM)sztString);
						}

						BuildGestureDropDown();
						if(bNewGestureMode){
							bNewGestureMode = FALSE;
							//SendMessage(hWndCB, CB_SETCURSEL, SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)szString), 0);
							int totalitems = SendMessage(hWndCB,CB_GETCOUNT, 0, 0);
							int loopct = 0;
							int startmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)szString);
							int lastmatchindex = startmatchindex;
							TCHAR szGestureName[255];
							SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
							if(_tcscmp( szString,szGestureName ) != 0)
							{
								if(startmatchindex != CB_ERR)
								{
									lastmatchindex++;
									while(lastmatchindex != startmatchindex && loopct < totalitems)
									{
										loopct++;
										lastmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,lastmatchindex-1,(LPARAM)szString);
										SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
										if(_tcscmp( szString,szGestureName ) == 0)
											break;
									}
								}
							}
							SendMessage(hWndCB, CB_SETCURSEL, lastmatchindex, 0);
						} else {
							//SendMessage(hWndCB, CB_SETCURSEL, SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)sztString), 0);
							int totalitems = SendMessage(hWndCB,CB_GETCOUNT, 0, 0);
							int loopct = 0;
							int startmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,0,(LPARAM)sztString);
							int lastmatchindex = startmatchindex;
							TCHAR szGestureName[255];
							SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
							if(_tcscmp( sztString,szGestureName ) != 0)
							{
								if(startmatchindex != CB_ERR)
								{
									lastmatchindex++;
									while(lastmatchindex != startmatchindex && loopct < totalitems)
									{
										loopct++;
										lastmatchindex = SendMessage(hWndCB,CB_FINDSTRINGEXACT,lastmatchindex-1,(LPARAM)sztString);
										SendMessage(hWndCB, CB_GETLBTEXT, lastmatchindex, (LPARAM)&szGestureName);
										if(_tcscmp( sztString,szGestureName ) == 0)
											break;
									}
								}
							}
							SendMessage(hWndCB, CB_SETCURSEL, lastmatchindex, 0);
						}

						ShowWindow(hWndSettings, SW_SHOW);
						g_ConfigShown = FALSE;
						bSaveActionSize = FALSE;
						SendMessage(hWndConfig, WM_COMMAND, MAKEWPARAM(0,CBN_SELCHANGE), (long)hWndCB);
					}
					PostMessage(hDlg, WM_CLOSE, 0, 0);
				}
			}

			if(LOWORD(wParam) == IDCANCEL)
			{
				PostMessage(hDlg, WM_CLOSE, 0, 0);
			}
			
		}

		break;
	}
	case WM_CLOSE:
		if(!bGesturesDisabled){
			PostMessage(hWndServer, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
		}
		bGestureOpen = FALSE;
		setCancel(FALSE);
		clearCaptureVars();
		EndDialog(hDlg, LOWORD(wParam));
		if(bSettingsOpen == FALSE && bGestureOpen == FALSE && bNameOpen == FALSE && bNewGestureMode == FALSE)
		{
			setLastActive();
		} else {
			bNewGestureMode = FALSE;
			setLearningMode(bPreviousTrainingModeState);
			if(bSettingsOpen){
				ShowWindow(hWndSettings, SW_SHOW);
				g_ConfigShown = FALSE;
				bSaveActionSize = FALSE;
				PostMessage(hWndSettings, WM_EXITSIZEMOVE, 0, 0);
			}
			return (INT_PTR)FALSE;
		}
		
	}
	return (INT_PTR)FALSE;
}

#pragma endregion GestureNameProc


#pragma region PreferencesTab

INT_PTR CALLBACK PreferencesTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		{
		SetWindowText(GetDlgItem(hDlg, IDC_STROKEBUTTONFRAME), ptLang.get("language.Preferences.StrokeButtonFrame.Title",_T("Stroke Button")).c_str());
		if(GetSystemMetrics(SM_SWAPBUTTON) == 0){
			SetWindowText(GetDlgItem(hDlg, IDC_USELEFT), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Left",_T("Left")).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_USERIGHT), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Right",_T("Right")).c_str());
		} else {
			SetWindowText(GetDlgItem(hDlg, IDC_USELEFT), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Right",_T("Right")).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_USERIGHT), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Left",_T("Left")).c_str());
		}
		SendDlgItemMessage(hDlg, IDC_USERIGHT, BM_SETCHECK, (getHookBtn() == 0) ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_USEMIDDLE, BM_SETCHECK, (getHookBtn() == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_USEMIDDLE), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Middle",_T("Middle")).c_str());
		SendDlgItemMessage(hDlg, IDC_USELEFT, BM_SETCHECK, (getHookBtn() == 2) ? BST_CHECKED : BST_UNCHECKED, 0);
		SendDlgItemMessage(hDlg, IDC_USEXBTN1, BM_SETCHECK, (getHookBtn() == 3) ? BST_CHECKED : BST_UNCHECKED, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_USEXBTN1), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X1",_T("X1")).c_str());
		SendDlgItemMessage(hDlg, IDC_USEXBTN2, BM_SETCHECK, (getHookBtn() == 4) ? BST_CHECKED : BST_UNCHECKED, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_USEXBTN2), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.X2",_T("X2")).c_str());
		SendDlgItemMessage(hDlg, IDC_USECTRL, BM_SETCHECK, (getHookBtn() == -1) ? BST_CHECKED : BST_UNCHECKED, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_USECTRL), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Control",_T("Control")).c_str());
		SendDlgItemMessage(hDlg, IDC_USEALT, BM_SETCHECK, (getHookBtn() == -2) ? BST_CHECKED : BST_UNCHECKED, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_USEALT), ptLang.get("language.Preferences.StrokeButtonFrame.Buttons.Alt",_T("Alt")).c_str());
	#ifdef SP_SYNAPTIC	
		ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICFRAME), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICSDISABLE), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSTEXT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_EDIT_SYNAPTICSSENS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSSUFFIX), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPFINGERS), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYTEXT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAY), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYSUFFIX), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTATEXT), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTA), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTASUFFIX), SW_SHOW);
		ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICSUSE), SW_SHOW);
		//ShowWindow(GetDlgItem(hDlg, IDC_SYNAPTICSCURSOR), SW_SHOW);
		SetWindowText(GetDlgItem(hDlg, IDC_SYNAPTICFRAME), ptLang.get("language.Preferences.SynapticsFrame.Title",_T("Synaptics TouchPad")).c_str());
		SendDlgItemMessage(hDlg, IDC_SYNAPTICSDISABLE, BM_SETCHECK, (bDisableSynaptics == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_SYNAPTICSDISABLE), ptLang.get("language.Preferences.SynapticsFrame.Disable",_T("Disable Integration")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_SYNAPTICSSENSTEXT), ptLang.get("language.Preferences.SynapticsFrame.Sensitivity",_T("Cursor Movement Sensitivity:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_SYNAPTICSSENSSUFFIX), ptLang.get("language.Preferences.SynapticsFrame.Percent",_T("%")).c_str());
	#endif
	#ifdef SP_WHEEL_HOOK
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_FIREWHEELTICK), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowMouseWheelTick",_T("Allow Mouse Wheel Tick Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_FIREWHEELTICK), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_FIREWHEELTICK, BM_SETCHECK, (bMouseWheelTick == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_FIREHORZWHEELTICK), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowHorzMouseWheelTick",_T("Allow Horizontal Mouse Wheel Tick Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_FIREHORZWHEELTICK), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_FIREHORZWHEELTICK, BM_SETCHECK, (bHorzMouseWheelTick == true ? BST_CHECKED : BST_UNCHECKED), 0);
	#endif
		SendDlgItemMessage(hDlg, IDC_CHK_NOSOUNDMATCH, BM_SETCHECK, (bNoMatchSound == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_SOUNDLABEL), ptLang.get("language.Preferences.AdvancedOptionsFrame.NoMatchSoundFile.Title",_T("Sound:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_BTN_PLAY), ptLang.get("language.Preferences.AdvancedOptionsFrame.NoMatchSoundFile.Test",_T("Test")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_BTN_NOMATCHBROWSE), ptLang.get("language.Preferences.AdvancedOptionsFrame.NoMatchSoundFile.Browse",_T("Browse...")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_NOSOUNDMATCH), ptLang.get("language.Preferences.AdvancedOptionsFrame.NoMatchSound",_T("Play Sound for No Match")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_MOUSEWHEEL, BM_SETCHECK, (bMouseWheelFocus == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_MOUSEWHEEL), ptLang.get("language.Preferences.AdvancedOptionsFrame.MouseWheelRelay",_T("Enable Mouse Wheel Relay*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_FIREONWHEEL, BM_SETCHECK, (bFireOnMouseWheel == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_FIREONWHEEL), ptLang.get("language.Preferences.AdvancedOptionsFrame.RecognizeOnMouseWheel",_T("Fire Recognition on Mouse Wheel Scroll*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_GETMODSONMOUSEDOWN, BM_SETCHECK, (bCaptureModifiersOnMouseDown == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_GETMODSONMOUSEDOWN), ptLang.get("language.Preferences.AdvancedOptionsFrame.CaptureModifiersOnMouseDown",_T("Capture Modifiers on Stroke Button Down*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWAFTERSCRIPT, BM_SETCHECK, (bAllowAfterScript == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWAFTERSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowAfterActionScript",_T("Allow After Action Script*")).c_str());
	#ifdef SP_AFTERRELEASESCRIPT
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWAFTERRELEASESCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWAFTERRELEASESCRIPT, BM_SETCHECK, (bFireOnRelease == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWAFTERRELEASESCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowAfterReleaseScript",_T("Allow After Release Script*")).c_str());
	#endif
	#ifdef SP_MOUSE_CLICK_ACTION
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWRIGHTCLICKSCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWRIGHTCLICKSCRIPT, BM_SETCHECK, (bFireOnRightClick == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWRIGHTCLICKSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowRightClickScript",_T("Allow Right Click Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWMIDDLECLICKSCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWMIDDLECLICKSCRIPT, BM_SETCHECK, (bFireOnMiddleClick == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWMIDDLECLICKSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowMiddleClickScript",_T("Allow Middle Click Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWLEFTCLICKSCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWLEFTCLICKSCRIPT, BM_SETCHECK, (bFireOnLeftClick == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWLEFTCLICKSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowLeftClickScript",_T("Allow Left Click Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWX1CLICKSCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWX1CLICKSCRIPT, BM_SETCHECK, (bFireOnX1Click == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWX1CLICKSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowX1ClickScript",_T("Allow X1 Click Script*")).c_str());
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_ALLOWX2CLICKSCRIPT), SW_SHOW);
		SendDlgItemMessage(hDlg, IDC_CHK_ALLOWX2CLICKSCRIPT, BM_SETCHECK, (bFireOnX2Click == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_ALLOWX2CLICKSCRIPT), ptLang.get("language.Preferences.AdvancedOptionsFrame.AllowX2ClickScript",_T("Allow X2 Click Script*")).c_str());
	#endif
		SendDlgItemMessage(hDlg, IDC_CHK_CHECKCURSORFLAGS, BM_SETCHECK, (bCheckCursorFlags == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CHECKCURSORFLAGS), ptLang.get("language.Preferences.AdvancedOptionsFrame.CheckCursorFlags",_T("Check Cursor Flags*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_HIDENEWGMSG, BM_SETCHECK, (bHideNewGestureMessage == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
	#ifdef SP_NOGLOBALACTIONS
		ShowWindow(GetDlgItem(hDlg, IDC_CHK_DISABLEGLOBALAPP), SW_SHOW);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_DISABLEGLOBALAPP), ptLang.get("language.Preferences.AdvancedOptionsFrame.OnlyDefinedApps",_T("Only Capture On Defined Applications (no Global Actions)")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_DISABLEGLOBALAPP, BM_SETCHECK, (bOnlyAllowDefinedApps == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
	#endif
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_HIDENEWGMSG), ptLang.get("language.Preferences.AdvancedOptionsFrame.NewGestureMessage",_T("Disable New Gesture Message")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_HIDEINSTANCE, BM_SETCHECK, (bHideAdditionalInstanceMessage == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_HIDEINSTANCE), ptLang.get("language.Preferences.AdvancedOptionsFrame.MultipleInstanceMessage",_T("Disable Multiple Instance Message (multiple instances are currently unstable)")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_INITONRESUME, BM_SETCHECK, (bReInitOnResume == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_INITONRESUME), ptLang.get("language.Preferences.AdvancedOptionsFrame.Suspend",_T("Release/Reinitialize on Suspend/Resume*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_DRAWWINDOWONTOP, BM_SETCHECK, (bDrawWindowAlwaysOnTop == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_DRAWWINDOWONTOP), ptLang.get("language.Preferences.AdvancedOptionsFrame.GestureWindowOnTop",_T("Keep Gesture Draw Window On Top*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_DRAWWINDOWTOBTM, BM_SETCHECK, (bDrawWindowToBottom == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_DRAWWINDOWTOBTM), ptLang.get("language.Preferences.AdvancedOptionsFrame.GestureWindowToBottom",_T("Don't Hide Gesture Draw Window*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_RESETCANCELDELAY, BM_SETCHECK, (bResetCancelDelayOnMovement == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_RESETCANCELDELAY), ptLang.get("language.Preferences.AdvancedOptionsFrame.ResetCancelDelay",_T("Reset Cancel Delay On Movement/Modifier*")).c_str());
		SendDlgItemMessage(hDlg, IDC_CHK_TRIM, BM_SETCHECK, (bAggressiveTrim == TRUE ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_TRIM), ptLang.get("language.Preferences.AdvancedOptionsFrame.ManageMemory",_T("Aggressively Manage Memory")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_ADVANCEDFRAME), ptLang.get("language.Preferences.AdvancedOptionsFrame.Title",_T("Advanced / Experimental* Options")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MATCHPRECLABEL), ptLang.get("language.Preferences.AdvancedOptionsFrame.MatchPrecision.Title",_T("Match Precision:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MATCHPRECSUFFIX), ptLang.get("language.Preferences.AdvancedOptionsFrame.MatchPrecision.Suffix",_T("points")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MATCHPROBLABEL), ptLang.get("language.Preferences.AdvancedOptionsFrame.MatchProbability.Title",_T("Match Probability:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MATCHPROBSUFFIX), ptLang.get("language.Preferences.AdvancedOptionsFrame.MatchProbability.Suffix",_T("%")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STROKESTYLEFRAME), ptLang.get("language.Preferences.StrokeStyleFrame.Title",_T("Stroke Style")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_REDLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Red.Title",_T("Red:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_REDSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Red.Suffix",_T("0-255")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_GREENLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Green.Title",_T("Green:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_GREENSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Green.Suffix",_T("0-255")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_BLUELABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Blue.Title",_T("Blue:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_BLUESUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Blue.Suffix",_T("0-255")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_WIDTHLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Width.Title",_T("Width:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_WIDTHSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Width.Suffix",_T("px")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MINSEGMENTLBL), ptLang.get("language.Preferences.StrokeStyleFrame.SegmentLength.Title",_T("Min. Segment Length:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_SEGMENTSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.SegmentLength.Suffix",_T("px")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_LENGTHLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Length.Title",_T("Length:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_LENGTHSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Length.Suffix",_T("points")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_OPACITYLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Opacity.Title",_T("Opacity:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_OPACITYSUFFIX), ptLang.get("language.Preferences.StrokeStyleFrame.Opacity.Suffix",_T("1-255")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_SHOWLABEL), ptLang.get("language.Preferences.StrokeStyleFrame.Show.Title",_T("Show:")).c_str());
		SetDlgItemInt(hDlg, IDC_PREFSRED, getDrawColorR(),false);
		SetDlgItemInt(hDlg, IDC_PREFSGREEN, getDrawColorG(),false);
		SetDlgItemInt(hDlg, IDC_PREFSBLUE, getDrawColorB(),false);
		SetDlgItemInt(hDlg, IDC_PREFSWIDTH, getDrawWidth(),false);
		SetDlgItemInt(hDlg, IDC_PREFSSEGMENT, iMinSegmentLength,false);
		SetDlgItemInt(hDlg, IDC_PREFSALPHA, opacity,false);
		SetWindowText(GetDlgItem(hDlg, IDC_BTN_COLOR), ptLang.get("language.Preferences.StrokeStyleFrame.PickColor",_T("Pick Color...")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CANCELDELAYLABEL), ptLang.get("language.Preferences.StrokeButtonFrame.CancelDelay.Title",_T("Cancel Delay:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CANCELDELAYSUFFIX), ptLang.get("language.Preferences.StrokeButtonFrame.CancelDelay.Suffix",_T("ms")).c_str());
		SetDlgItemInt(hDlg, IDC_PREFSCANCELDELAY, getCancelDelay(),false);
		SendDlgItemMessage(hDlg,IDC_PREFSDRAWS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeStyleFrame.Show.No",_T("No")).c_str()));
		SendDlgItemMessage(hDlg,IDC_PREFSDRAWS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeStyleFrame.Show.Yes",_T("Yes")).c_str()));
		SetWindowText(GetDlgItem(hDlg, IDC_IGNOREKEYLABEL), ptLang.get("language.Preferences.StrokeButtonFrame.IgnoreKey.Title",_T("Ignore Key:")).c_str());
		SendDlgItemMessage(hDlg,IDC_PREFSDRAWS, CB_SETCURSEL, (getDrawGesture() == TRUE ? 1 : 0), 0);
		SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeButtonFrame.IgnoreKey.DropDown.Control",_T("Control")).c_str()));
		SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeButtonFrame.IgnoreKey.DropDown.Alt",_T("Alt")).c_str()));
		SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeButtonFrame.IgnoreKey.DropDown.Shift",_T("Shift")).c_str()));
		SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.StrokeButtonFrame.IgnoreKey.DropDown.None",_T("None")).c_str()));
		SendDlgItemMessage(hDlg,IDC_PREFSIGNOREKEY, CB_SETCURSEL, getIgnoreKey(), 0);
	#ifdef SP_SYNAPTIC
		SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.SynapticsFrame.Fingers.One",_T("1 Finger (tap & hold)")).c_str()));
		if(bTouchPadMultiFinger || iTouchPadFingers > 1){
			SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.SynapticsFrame.Fingers.Two",_T("2 Fingers")).c_str()));
			SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Preferences.SynapticsFrame.Fingers.Three",_T("3 Fingers")).c_str()));
		}
		SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS, CB_SETCURSEL, iTouchPadFingers-1, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_SYNAPTICSUSE), ptLang.get("language.Preferences.SynapticsFrame.StrokeSequence",_T("Stroke Sequence:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTATEXT), ptLang.get("language.Preferences.SynapticsFrame.HoldDeltaText",_T("1 Finger Movement Threshold:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTASUFFIX), ptLang.get("language.Preferences.SynapticsFrame.HoldDeltaSuffix",_T("units")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYTEXT), ptLang.get("language.Preferences.SynapticsFrame.HoldDelayText",_T("1 Finger Hold Delay:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYSUFFIX), ptLang.get("language.Preferences.SynapticsFrame.HoldDelaySuffix",_T("ms")).c_str());
		SetDlgItemInt(hDlg, IDC_PREFSTPHOLDDELTA, iTouchPadHoldThresholdDelta, false);
		SetDlgItemInt(hDlg, IDC_PREFSTPHOLDDELAY, iTouchPadHoldDelay, false);
		EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICFRAME),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSUSE),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPFINGERS),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTATEXT),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTA),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTASUFFIX),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAY),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYTEXT),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYSUFFIX),(mSynaptic == NULL || bDisableSynaptics || iTouchPadFingers != 1 ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSTEXT),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSSUFFIX),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT_SYNAPTICSSENS),(mSynaptic == NULL || bDisableSynaptics ? false : true));
		SetDlgItemInt(hDlg, IDC_EDIT_SYNAPTICSSENS, iTouchPadSensitivity, false);
		SendDlgItemMessage(hDlg, IDC_CHK_CHECKRELAYGESTURE, BM_SETCHECK, (bRelayGesture == true ? BST_CHECKED : BST_UNCHECKED), 0);
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CHECKRELAYGESTURE), ptLang.get("language.Preferences.AdvancedOptionsFrame.RelayGestureOnNoMatch",_T("Relay Gesture on No Match*")).c_str());

	#endif
		SetDlgItemText(hDlg, IDC_EDIT_NOMATCHWAV, sNoMatchSoundWAV.c_str());
		SetDlgItemInt(hDlg, IDC_EDIT_MAXDRAWPTS, iMaxDrawPoints, false);
		SetDlgItemInt(hDlg, IDC_EDIT_PRECISION, iPrecision, false);
		SetDlgItemInt(hDlg, IDC_EDIT_PROBABILITY, MatchProbabilityThreshold, false);


		return (INT_PTR)TRUE;
		}

	case WM_COMMAND:
	{
	#ifdef SP_SYNAPTIC
		if(LOWORD(wParam) == IDC_SYNAPTICSDISABLE && HIWORD(wParam) == BN_CLICKED){
			bool bState = (bool)SendDlgItemMessage(hDlg, IDC_SYNAPTICSDISABLE, BM_GETCHECK, 0, 0);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICFRAME),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSTEXT),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSSENSSUFFIX),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT_SYNAPTICSSENS),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNAPTICSUSE),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPFINGERS),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTATEXT),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTA),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTASUFFIX),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAY),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYTEXT),!bState);
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYSUFFIX),!bState);
		}
		if(LOWORD(wParam) == IDC_PREFSTPFINGERS && HIWORD(wParam) == CBN_SELCHANGE){
			int iFing = SendDlgItemMessage(hDlg,IDC_PREFSTPFINGERS, CB_GETCURSEL, 0, 0);
			iFing++;
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTATEXT),(iFing==1 ? true : false));
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTA),(iFing==1 ? true : false));
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELTASUFFIX),(iFing==1 ? true : false));
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAY),(iFing==1 ? true : false));
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYTEXT),(iFing==1 ? true : false));
			EnableWindow(GetDlgItem(hDlg, IDC_PREFSTPHOLDDELAYSUFFIX),(iFing==1 ? true : false));
		}
	#endif
		if(LOWORD(wParam) == IDC_BTN_COLOR){
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_COLOR),false);
			static COLORREF  colorrefCustomColors[16] = {0} ;
			CHOOSECOLOR cc;
              
			cc.hwndOwner = hDlg;
			cc.hInstance = NULL;
			cc.rgbResult = RGB(GetDlgItemInt(hDlg, IDC_PREFSRED, NULL, false),GetDlgItemInt(hDlg, IDC_PREFSGREEN, NULL, false),GetDlgItemInt(hDlg, IDC_PREFSBLUE, NULL, false));
			//cc.rgbResult = RGB(getDrawColorR(), getDrawColorG(), getDrawColorB());
			cc.lpfnHook =NULL;
			cc.Flags = CC_SOLIDCOLOR|CC_PREVENTFULLOPEN|CC_RGBINIT|CC_FULLOPEN|CC_ANYCOLOR;
			cc.lpCustColors = colorrefCustomColors;
			cc.lpTemplateName = NULL;
			cc.lCustData = NULL;
			cc.lStructSize = sizeof(cc);

			ChooseColor(&cc);

			//setDrawColor(RGB(GetRValue(cc.rgbResult),GetGValue(cc.rgbResult),GetBValue(cc.rgbResult))); 
			SetDlgItemInt(hDlg, IDC_PREFSRED, GetRValue(cc.rgbResult),false);
			SetDlgItemInt(hDlg, IDC_PREFSGREEN, GetGValue(cc.rgbResult),false);
			SetDlgItemInt(hDlg, IDC_PREFSBLUE, GetBValue(cc.rgbResult),false);

			drawStrokeColor(hDlg);

			EnableWindow(GetDlgItem(hDlg, IDC_BTN_COLOR),true);
		}
		if(LOWORD(wParam) == IDC_BTN_NOMATCHBROWSE) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_NOMATCHBROWSE),false);
			OPENFILENAME ofn;
			CHAR szFile[_MAX_PATH];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = szFile;
 
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "WAV Files: wav\0*.wav;\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_NONETWORKBUTTON | OFN_ENABLESIZING | OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if(GetOpenFileName(&ofn)==TRUE){
				SetDlgItemText(hDlg, IDC_EDIT_NOMATCHWAV, szFile);
			}
			SetCurrentDirectoryA(strExePath);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_NOMATCHBROWSE),true);
		}
		if(LOWORD(wParam) == IDC_BTN_PLAY)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_PLAY),false);
			CHAR szWAVFile[_MAX_PATH];
			GetDlgItemText(hDlg, IDC_EDIT_NOMATCHWAV, szWAVFile, _MAX_PATH);
			PlaySound(szWAVFile, NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_PLAY),true);
		}
		if((LOWORD(wParam) == IDC_PREFSRED || LOWORD(wParam) == IDC_PREFSGREEN || LOWORD(wParam) == IDC_PREFSBLUE) && HIWORD(wParam) == EN_CHANGE)
		{
			drawStrokeColor(hDlg);
		}

		#ifdef SP_WHEEL_HOOK
			if(LOWORD(wParam) == IDC_CHK_MOUSEWHEEL && HIWORD(wParam) == BN_CLICKED){
				if(SendDlgItemMessage(hDlg, IDC_CHK_MOUSEWHEEL, BM_GETCHECK, 0, 0) == 1)
					SendDlgItemMessage(hDlg, IDC_CHK_FIREWHEELTICK, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			if(LOWORD(wParam) == IDC_CHK_FIREWHEELTICK && HIWORD(wParam) == BN_CLICKED){
				if(SendDlgItemMessage(hDlg, IDC_CHK_FIREWHEELTICK, BM_GETCHECK, 0, 0) == 1)
					SendDlgItemMessage(hDlg, IDC_CHK_MOUSEWHEEL, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		#endif

		break;
	}
	case WM_WINDOWPOSCHANGED:
		{
			KillTimer(NULL,strokeswatchtimerid);
			strokeswatchtimerid = SetTimer(NULL, WM_IDT_STROKESWATCH_DRAW, 100, TimerStrokeSwatch);
		}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion PreferencesTab


#pragma region ConfigTab (Actions)

INT_PTR CALLBACK ConfigTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{

		if(GetSystemMetrics(SM_SWAPBUTTON) == 0){
			SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_LEFTBTN), ptLang.get("language.Actions.MouseKeyModifierFrame.Left",_T("Left Button")).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_RIGHTBTN), ptLang.get("language.Actions.MouseKeyModifierFrame.Right",_T("Right Button")).c_str());
		} else {
			SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_LEFTBTN), ptLang.get("language.Actions.MouseKeyModifierFrame.Right",_T("Right Button")).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_RIGHTBTN), ptLang.get("language.Actions.MouseKeyModifierFrame.Left",_T("Left Button")).c_str());
		}
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_GESTURE_GROUP), ptLang.get("language.Actions.GestureFrame.Title",_T("Gesture")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_NEWG), ptLang.get("language.Actions.GestureFrame.New",_T("New")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_RENG), ptLang.get("language.Actions.GestureFrame.Rename",_T("Rename")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_DELG), ptLang.get("language.Actions.GestureFrame.Delete",_T("Delete")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_MOUSEMOD_GROUP), ptLang.get("language.Actions.MouseKeyModifierFrame.Title",_T("Mouse / Key Modifiers")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_MIDDLEBTN), ptLang.get("language.Actions.MouseKeyModifierFrame.Middle",_T("Middle Button")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_X1BTN), ptLang.get("language.Actions.MouseKeyModifierFrame.X1",_T("X1 Button")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_X2BTN), ptLang.get("language.Actions.MouseKeyModifierFrame.X2",_T("X2 Button")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_CTRL), ptLang.get("language.Actions.MouseKeyModifierFrame.Control",_T("Control")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_ALT), ptLang.get("language.Actions.MouseKeyModifierFrame.Alt",_T("Alt")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_SHIFT), ptLang.get("language.Actions.MouseKeyModifierFrame.Shift",_T("Shift")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_SCROLLUP), ptLang.get("language.Actions.MouseKeyModifierFrame.ScrollUp",_T("Scroll Up")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_CFG_SCROLLDOWN), ptLang.get("language.Actions.MouseKeyModifierFrame.ScrollDown",_T("Scroll Down")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_MODTEXT), ptLang.get("language.Actions.FormText.NoGestureRequired",_T("Check Capture Modifiers on Stroke Button Down in Preferences to enable the Modifier Check options.\n\nYou don't have to select a gesture, modifier(s) alone can define an action.")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_GRP_WHENFIRE), ptLang.get("language.Actions.ModifierPress.Title",_T("Modifier Check")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_R_BEFORE), ptLang.get("language.Actions.ModifierPress.Before",_T("On Stroke Button Down")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_R_AFTER), ptLang.get("language.Actions.ModifierPress.After",_T("After Stroke Button Down")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_R_EITHER), ptLang.get("language.Actions.ModifierPress.Either",_T("Either")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_GRP_ACTIONHELP), ptLang.get("language.Actions.AvailableActionsFrame.Title",_T("Available Actions")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_INFOACTION), ptLang.get("language.Actions.AvailableActionsFrame.Info",_T("Info")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_INSACTION), ptLang.get("language.Actions.AvailableActionsFrame.Insert",_T("Insert")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CLUA), ptLang.get("language.Actions.FormText.LuaScriptTitle",_T("Lua Script:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFGBTNADDAPP), ptLang.get("language.Actions.FormButtons.AddApp",_T("Add App")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFGBTNADDACTION), ptLang.get("language.Actions.FormButtons.AddAction",_T("Add Action")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFGBTNRENAME), ptLang.get("language.Actions.FormButtons.Rename",_T("Rename")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFGBTNDELETE), ptLang.get("language.Actions.FormButtons.Delete",_T("Delete")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDCLOSE), ptLang.get("language.Actions.FormButtons.Close",_T("Close")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_COT), ptLang.get("language.Actions.Application.OwnerTitle",_T("Owner Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_COTR), ptLang.get("language.Actions.Application.OwnerTitlePattern",_T("Owner Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCN), ptLang.get("language.Actions.Application.OwnerClassName",_T("Owner Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCNR), ptLang.get("language.Actions.Application.OwnerClassNamePattern",_T("Owner Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CPT), ptLang.get("language.Actions.Application.ParentTitle",_T("Parent Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CPTR), ptLang.get("language.Actions.Application.ParentTitlePattern",_T("Parent Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CPC), ptLang.get("language.Actions.Application.ParentClassName",_T("Parent Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR), ptLang.get("language.Actions.Application.ParentClassNamePattern",_T("Parent Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCT), ptLang.get("language.Actions.Application.ControlTitle",_T("Control Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCTR), ptLang.get("language.Actions.Application.ControlTitlePattern",_T("Control Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCC), ptLang.get("language.Actions.Application.ControlClassName",_T("Control Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CCCR), ptLang.get("language.Actions.Application.ControlClassNamePattern",_T("Control Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CID), ptLang.get("language.Actions.Application.ControlID",_T("Control ID:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CFN), ptLang.get("language.Actions.Application.FileName",_T("File Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CFNR), ptLang.get("language.Actions.Application.FileNamePattern",_T("File Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_CPR), ptLang.get("language.Actions.Application.FilePathPattern",_T("File Path Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_NOGLOBAL), ptLang.get("language.Actions.Application.DoNotProcess",_T("Do not process Global Actions for this app, only app actions")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CFG_FW), ptLang.get("language.Actions.Application.FindWindowFrame",_T("Find Window")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CFG_APP_TEXT), ptLang.get("language.Actions.Application.Qualifiers",_T("All qualifiers in this list will determine if the app is a match when the stroke button is pressed. The pattern fields are for regular expression matches. \n\nOnly one field is required; often times, clearing all but the File Name is sufficient.")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHKACTACTIVE), ptLang.get("language.Actions.Active",_T("Action Active ")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHKAPPACTIVE), ptLang.get("language.Actions.Application.Active",_T("Application Active ")).c_str());

		//move match criteria fields to the left, in view
		//so yea, this was the first window I made and before I knew how to do things better..thus actions and app definition fields are on the same dialog and actually repositioned, lol

		RECT lpControl;
		POINT ptControl;
		GetWindowRect(GetDlgItem(hDlg, IDC_CHK_CFG_CTRL),&lpControl);
		ptControl.x = lpControl.left;
		ptControl.y = lpControl.top;
		ScreenToClient(hDlg, &ptControl);

		RECT lpDelete;
		POINT ptDelete;
		GetWindowRect(GetDlgItem(hDlg, IDC_CFG_DELG),&lpDelete);
		ptDelete.x = lpDelete.left;
		ptDelete.y = lpDelete.top;
		ScreenToClient(hDlg, &ptDelete);

		RECT lpRect;
		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_COT),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_COT),GetDlgItem(hDlg, IDC_CHKACTACTIVE), ptDelete.x, 13*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE),GetDlgItem(hDlg, IDC_STATIC_CFG_COT), ptDelete.x, 29*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_COTR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_COTR),GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE), ptControl.x, 13*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_COTR), ptControl.x, 29*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCN),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCN),GetDlgItem(hDlg, IDC_EDITCFG_OWNERTITLE_REGEX), ptDelete.x, 60*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS),GetDlgItem(hDlg, IDC_STATIC_CFG_CCN), ptDelete.x, 76*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCNR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCNR),GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS), ptControl.x, 60*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CCNR), ptControl.x, 76*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CPT),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CPT),GetDlgItem(hDlg, IDC_EDITCFG_OWNERCLASS_REGEX), ptDelete.x, 107*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE),GetDlgItem(hDlg, IDC_STATIC_CFG_CPT), ptDelete.x, 123*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CPTR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CPTR),GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE), ptControl.x, 107*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CPTR), ptControl.x, 123*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CPC),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CPC),GetDlgItem(hDlg, IDC_EDITCFG_PARENTTITLE_REGEX), ptDelete.x, 154*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS),GetDlgItem(hDlg, IDC_STATIC_CFG_CPC), ptDelete.x, 170*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR),GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS), ptControl.x, 154*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CPCR), ptControl.x, 170*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCT),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCT),GetDlgItem(hDlg, IDC_EDITCFG_PARENTCLASS_REGEX), ptDelete.x, 201*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE),GetDlgItem(hDlg, IDC_STATIC_CFG_CCT), ptDelete.x, 217*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCTR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCTR),GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE), ptControl.x, 201*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CCTR), ptControl.x, 217*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCC),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCC),GetDlgItem(hDlg, IDC_EDITCFG_CONTROLTITLE_REGEX), ptDelete.x, 248*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS),GetDlgItem(hDlg, IDC_STATIC_CFG_CCC), ptDelete.x, 264*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CCCR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CCCR),GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS), ptControl.x, 248*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CCCR), ptControl.x, 264*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CID),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CID),GetDlgItem(hDlg, IDC_EDITCFG_CONTROLCLASS_REGEX), ptDelete.x, 295*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLID),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_CONTROLID),GetDlgItem(hDlg, IDC_STATIC_CFG_CID), ptDelete.x, 311*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CPR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CPR),GetDlgItem(hDlg, IDC_EDITCFG_CONTROLID), ptControl.x, 295*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_PATH_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_PATH_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CPR), ptControl.x, 311*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CFN),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CFN),GetDlgItem(hDlg, IDC_EDITCFG_PATH_REGEX), ptDelete.x, 342*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_FILENAME),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_FILENAME),GetDlgItem(hDlg, IDC_STATIC_CFG_CFN), ptDelete.x, 358*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_CFNR),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_CFNR),GetDlgItem(hDlg, IDC_EDITCFG_FILENAME), ptControl.x, 342*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_EDITCFG_FILENAME_REGEX),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_EDITCFG_FILENAME_REGEX),GetDlgItem(hDlg, IDC_STATIC_CFG_CFNR), ptControl.x, 358*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_CFG_FW),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_CFG_FW),GetDlgItem(hDlg, IDC_EDITCFG_FILENAME_REGEX), ptDelete.x, 427*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_ICON_FINDER_TOOL),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_ICON_FINDER_TOOL),GetDlgItem(hDlg, IDC_STATIC_CFG_FW), ptDelete.x+(35*fDPIModifier), 460*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_CFG_APP_TEXT),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_CFG_APP_TEXT),GetDlgItem(hDlg, IDC_STATIC_ICON_FINDER_TOOL), ptDelete.x+(150*fDPIModifier), 434*fDPIModifier, lpRect.right-lpRect.left, lpRect.bottom-lpRect.top, SWP_NOACTIVATE);

		GetWindowRect(GetDlgItem(hDlg, IDC_CHK_NOGLOBAL),&lpRect);
		SetWindowPos(GetDlgItem(hDlg, IDC_CHK_NOGLOBAL),GetDlgItem(hDlg, IDC_CFG_APP_TEXT), ptDelete.x, 395*fDPIModifier, lpRect.right-lpRect.left-1, lpRect.bottom-lpRect.top-1, SWP_NOACTIVATE);

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
			if(v1.second.get("<xmlattr>.name",_T("")) != _T("")){
				SendDlgItemMessage(hDlg,IDC_CBO_ACTIONS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)v1.second.get("<xmlattr>.name",_T("")).c_str()));
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					SendDlgItemMessage(hDlg,IDC_CBO_ACTIONS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)v2.second.get("<xmlattr>.name",_T("")).c_str()));
				}
			}
		}

		//This creates the enhanced gesture dropdown (to support gesture thumbnails in the dropdown)
		hWndCB = CreateWindowEx(0, WC_COMBOBOXEX, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_TABSTOP,
						// No size yet--resize after setting image list.
						ptDelete.x+1,      // Horizontal position of Combobox
						32*fDPIModifier,      // Vertical position of Combobox
						lpDelete.right-lpDelete.left-2,      // Sets the width of Combobox
						600,    // Sets the height of Combobox
						hDlg, NULL, hInst, NULL);
		ShowWindow(hDlg, SW_SHOW);

		ShowConfigFields(hDlg, true, true);

		EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNDELETE),false);
		EnableWindow(GetDlgItem(hDlg , IDC_CFGBTNRENAME),false);
		
		BuildGestureImageList(TRUE,28);
		BuildGestureDropDown();
		BuildConfigTree(hDlg);

		// Assign the existing image list to the ComboBoxEx control 
		SendMessage(hWndCB,CBEM_SETIMAGELIST,0,(LPARAM)hListTree);

		// Set size of control to make sure it's displayed correctly now
		// that the image list is set.
		SetWindowPos(hWndCB,GetDlgItem(hDlg, IDC_CFGBTNDELETE),ptDelete.x+1,32*fDPIModifier,lpDelete.right-lpDelete.left-2,120*fDPIModifier,SWP_NOACTIVATE);
		SendMessage(hWndCB, CB_SETDROPPEDWIDTH, 300, 0);

		hWndConfig = hDlg;

		return (INT_PTR)FALSE;
	}

	case WM_NOTIFY:
	{
	    case IDC_TREEAPPS:
		{
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGING) 
			{
				if(bAutosave && !bIsSaving) {
					BOOL bContinue = FALSE;
					EnterCriticalSection(&CriticalSection); 
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
					if(!bIsSaving){
						bIsSaving = TRUE;
						if(hDlg,pnmtv->itemOld.hItem != NULL){
							bContinue = SaveConfigChange(hDlg,pnmtv->itemOld.hItem);
						} else {
							bContinue = true;
						}
					}
					bIsSaving = FALSE;
					LeaveCriticalSection(&CriticalSection);
					if(!bContinue){
						SetWindowLongPtr(hDlg, DWLP_MSGRESULT, 1);
						return TRUE;
					}
				}
			}
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGED) 
			{
				LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
				LoadConfigNode(hDlg,pnmtv->itemNew.hItem);
			}
			if(((LPNMHDR)lParam)->code == NM_RCLICK) 
			{
				LPNMHDR lpnmh = (LPNMHDR) lParam;
				TVHITTESTINFO lpht;
				ZeroMemory(&lpht, sizeof(TVHITTESTINFO));
				GetCursorPos(&lpht.pt);
				ScreenToClient(lpnmh->hwndFrom, &lpht.pt);
				itemUnderMouse = NULL;
				itemUnderMouse = TreeView_HitTest(lpnmh->hwndFrom, &lpht);
				if(itemUnderMouse != NULL)
				{
					ClientToScreen(lpnmh->hwndFrom, &lpht.pt);
					ShowActionTreePopupMenu(lpht.pt);
					return (INT_PTR)TRUE;
				}
			}
		}
		break;
	}

	case WM_MOUSEMOVE :
	{
	  if (g_bStartSearchWindow)
	  {
	    // Only when we have started the Window Searching operation will we 
		// track mouse movement.
	    DoMouseMove(hDlg, message, wParam, lParam);
	  }

	  break;
	}

	case WM_LBUTTONUP :
	{
	  if (g_bStartSearchWindow)
	  {
	    // Only when we have started the window searching operation will we
		// be interested when the user lifts up the left mouse button.
	    DoMouseUp(hDlg, message, wParam, lParam);
	  }

	  break;
	}

	case WM_COMMAND:

		if((HWND)lParam== hWndCB && HIWORD(wParam) == CBN_SELCHANGE)
		{
			g_Points.clear();
			BOOL bAppMatchFound = FALSE;

			CHAR szGestureName[255];
			memset(szGestureName,0,sizeof(szGestureName));
			int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
			SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);

			//BOOL bFoundMatch = FALSE;
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v7, ptConfig.get_child("config.Gestures")) {
				//each ignored window
				if(v7.second.get("<xmlattr>.name",_T("")) != _T("")){
					if(_tcscmp( szGestureName, v7.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
					{
						//bFoundMatch = TRUE;
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v8, v7.second) {
							//get point Patterns
							if(v8.first == "PointPatterns") {
								BOOST_FOREACH(boost::property_tree::ptree::value_type &v9, v8.second) {
									//each point pattern
									if(v9.first == _T("PointPattern")) {
										BOOST_FOREACH(boost::property_tree::ptree::value_type &v10, v9.second) {
											//each point 
											if(v10.first == "Point") {
												POINT Point;
												int x = (int)v10.second.get("<xmlattr>.x",0);
												int y = (int)v10.second.get("<xmlattr>.y",0);
												Point.x = (float)x;
												Point.y = (float)y;
												g_Points.push_back(Point);						
											}
										}
										break;
									}
									break;
								}
								break;
							}
						}
					}
				}
			}
			EnableWindow(GetDlgItem(hDlg , IDC_CFG_DELG),(g_Points.size() > 0));	
			EnableWindow(GetDlgItem(hDlg , IDC_CFG_RENG),(g_Points.size() > 0));
			drawConfigGesture(hDlg);
			break;
		}

		if (LOWORD(wParam) == IDC_STATIC_ICON_FINDER_TOOL)
		{
			// Because the IDC_STATIC_ICON_FINDER_TOOL static control is set with the SS_NOTIFY
			// flag, the Search Window's dialog box will be sent a WM_COMMAND message when this 
			// static control is clicked.
			// We start the window search operation by calling the DoSearchWindow() function.
			if(bCriteraFieldsActive) {
				SearchWindow(hDlg);
			}
			break;
		}

		if (LOWORD(wParam) == IDC_CFGBTNDELETE) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNDELETE),false);
			HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEAPPS));
			TVITEM tvi;
			ZeroMemory(&tvi,sizeof(tvi));
			tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
			tvi.hItem=hSelected;
			char cText[255] = {0};
			tvi.pszText = cText;
			tvi.cchTextMax=256;
			SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
			if(tvi.cChildren == 1){
				//Application
				HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEAPPS));
				TVITEM tvi;
				ZeroMemory(&tvi,sizeof(tvi));
				tvi.mask=TVIF_TEXT;
				tvi.hItem=hSelected;
				char cText[255] = {0};
				tvi.pszText = cText;
				tvi.cchTextMax=256;
				SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

				if(MessageBox(hWndSettings,ptLang.get("language.Actions.Messages.DeleteTreeItem",_T("Are you sure?")).c_str(),ptLang.get("language.Actions.Messages.DeleteTreeItemTitle",_T("Delete")).c_str(),MB_ICONWARNING + MB_YESNO + MB_DEFBUTTON2 + MB_APPLMODAL) == IDYES)
				{
					HTREEITEM hNext = TreeView_GetNextSibling(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					HTREEITEM hPrev = TreeView_GetPrevSibling(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					HTREEITEM hPar = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);

					TreeView_DeleteItem(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
						//each app
						if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
							v2.second.clear();
						}
					}

					TreeView_SelectItem(GetDlgItem(hDlg , IDC_TREEAPPS),(hNext != NULL ? hNext : (hPrev != NULL ? hPrev : hPar)));
				}
			} else {
				//Action

				if(MessageBox(hWndSettings,ptLang.get("language.Actions.Messages.DeleteTreeItem",_T("Are you sure?")).c_str(),ptLang.get("language.Actions.Messages.DeleteTreeItemTitle",_T("Delete")).c_str(),MB_ICONWARNING + MB_YESNO + MB_DEFBUTTON2 + MB_APPLMODAL) == IDYES)
				{

					HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEAPPS));
					TVITEM tvi;
					ZeroMemory(&tvi,sizeof(tvi));
					tvi.mask=TVIF_TEXT;
					tvi.hItem=hSelected;
					char cText[255] = {0};
					tvi.pszText = cText;
					tvi.cchTextMax=256;
					SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

					HTREEITEM hNext = TreeView_GetNextSibling(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					HTREEITEM hPrev = TreeView_GetPrevSibling(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					HTREEITEM hPar = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);

					BOOL bAppMatch = FALSE;

					HTREEITEM hSelectedParent = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
					TVITEM parenttvi;
					ZeroMemory(&parenttvi,sizeof(parenttvi));
					parenttvi.mask=TVIF_TEXT | TVIF_CHILDREN;
					parenttvi.hItem=hSelectedParent;
					char cParentText[255] = {0};
					parenttvi.pszText = cParentText;
					parenttvi.cchTextMax=256;
					SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&parenttvi);

					BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Applications")) {
						//each ignored window
						if(_tcsicmp( cParentText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child("Actions")) {
								//each ignored window
								if(_tcsicmp( cText, v3.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v3.second.get("<xmlattr>.enabled",0) == 1*/) {
									v3.second.clear();
									TreeView_DeleteItem(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
									bAppMatch = TRUE;
								}
							}
						}
					}

					if(!bAppMatch) {
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Actions")) {
							//each app
							if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
								v2.second.clear();
								TreeView_DeleteItem(GetDlgItem(hDlg , IDC_TREEAPPS),hSelected);
							}
						}						
					}
					TreeView_SelectItem(GetDlgItem(hDlg , IDC_TREEAPPS),(hNext != NULL ? hNext : (hPrev != NULL ? hPrev : hPar)));
				}
			}
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNDELETE),true);
		}

		if (LOWORD(wParam) == IDC_CFGBTNRENAME) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNRENAME),false);
			if(!bNameOpen){
				HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEAPPS));
				TVITEM tvi;
				ZeroMemory(&tvi,sizeof(tvi));
				tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
				tvi.hItem=hSelected;
				char cText[255] = {0};
				tvi.pszText = cText;
				tvi.cchTextMax=256;
				SendMessage(GetDlgItem(hDlg , IDC_TREEAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
				if(tvi.cChildren == 1){
					bConfigRenameApp = TRUE;
				} else {
					bConfigRenameAction = TRUE;
				}
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNRENAME),true);
			break;	
		}

		if (LOWORD(wParam) == IDC_CFGBTNADDAPP)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNADDAPP),false);
			if(!bNameOpen) {
				bConfigNameApp = TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNADDAPP),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_CFG_INSACTION)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_INSACTION),false);
			CHAR szActionName[255];
			memset(szActionName,0,sizeof(szActionName));
			GetDlgItemText(hDlg, IDC_CBO_ACTIONS, szActionName, 255);
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					if(_tcsicmp( szActionName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
					{
						std::basic_string<TCHAR> sHelpBuffer = _T("");
						sHelpBuffer.append(v2.second.get("snippet",_T("")));
						SetFocus(GetDlgItem(hDlg, IDC_EDITLUA));
						SendEditor(SCI_REPLACESEL, 0, LPARAM(sHelpBuffer.c_str()));
						SetFocus(m_hwndEditor);
					}
				}
			}
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_INSACTION),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_CFG_INFOACTION)
		{

			EnableWindow(GetDlgItem(hDlg, IDC_CFG_INFOACTION),false);
			CHAR szActionName[255];
			memset(szActionName,0,sizeof(szActionName));
			GetDlgItemText(hDlg, IDC_CBO_ACTIONS, szActionName, 255);
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					if(_tcsicmp( szActionName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
					{
						std::basic_string<TCHAR> sHelpBuffer = ptLang.get("language.Actions.AvailableActionsFrame.Messages.Info.Description",_T("Description:")).append("\r\n  ");
						sHelpBuffer.append(v2.second.get("description",_T("")));
						sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Actions.AvailableActionsFrame.Messages.Info.Usage",_T("Usage:")).append("\r\n  "));
						sHelpBuffer.append(v2.second.get("signature",_T("")));
						sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Actions.AvailableActionsFrame.Messages.Info.Returns",_T("Returns:")).append("\r\n  "));
						sHelpBuffer.append(v2.second.get("return.<xmlattr>.type",_T("")));
						sHelpBuffer.append(_T(" - "));
						sHelpBuffer.append(v2.second.get("return",_T("")));
						BOOL bParamWritten = FALSE;
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("parameters"))) {
							if(!bParamWritten){
								sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Actions.AvailableActionsFrame.Messages.Info.Parameters",_T("Parameters:")));
								bParamWritten = TRUE;
							}
							sHelpBuffer.append(_T("\r\n  "));
							sHelpBuffer.append(v3.second.get("<xmlattr>.name",_T("")));
							sHelpBuffer.append(_T(": "));
							sHelpBuffer.append(v3.second.get_value(_T("")));
						}

						MessageBox(hDlg, sHelpBuffer.c_str(), szActionName, NULL);
					}
				}
			}
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_INFOACTION),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_CFG_NEWG)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_NEWG),false);
			if(!bHideNewGestureMessage){
				MessageBox(hWndSettings, ptLang.get("language.Actions.Messages.NewGesture",_T("Click OK, then draw the new gesture. Nothing will be lost in this window, it's just hidden.")).c_str(), ptLang.get("language.Actions.Messages.NewGestureTitle",_T("New Gesture")).c_str(), MB_ICONINFORMATION + MB_APPLMODAL);
			}
			ShowWindow(hWndSettings, SW_HIDE);
			bNewGestureMode = TRUE;
			bPreviousTrainingModeState = getLearningMode();
			if(bGesturesDisabled){
				setMyHook(hWndServer);
			}
			setLearningMode(TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_NEWG),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_CFG_RENG)
		{
			bConfigRenameGesture = TRUE;
			bNameOpen = TRUE;
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_RENG),false);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_RENG),true);
			break;
		}

		if (LOWORD(wParam) == IDC_CFG_DELG)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CFG_DELG),false);
			if(MessageBox(hWndSettings,ptLang.get("language.Actions.Messages.DeleteGesture",_T("Are you sure?")).c_str(),ptLang.get("language.Actions.Messages.DeleteGestureTitle",_T("Delete")).c_str(),MB_ICONWARNING + MB_YESNO + MB_DEFBUTTON2 + MB_APPLMODAL) == IDYES)
			{
				CHAR szGestureName[255];
				memset(szGestureName,0,sizeof(szGestureName));
				int nIndex = SendMessage(hWndCB, CB_GETCURSEL, 0, 0);
				SendMessage(hWndCB, CB_GETLBTEXT, nIndex, (LPARAM)&szGestureName);
					
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Gestures")) {
					//each gesture
					if(v2.second.get("<xmlattr>.name",_T("")) != _T("")){
						if(_tcscmp( szGestureName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0) {
							v2.second.clear();
							SendMessage(hWndCB,CB_DELETESTRING,nIndex,0);
							SendMessage(hWndCB,CB_SETCURSEL,0,0);
							SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(0,CBN_SELCHANGE), (long)hWndCB);
							for(int i = 0; i < GestureNames.size(); i++) { 
								if(_tcscmp( GestureNames[i].c_str(), szGestureName ) == 0 ){
									int iSize = 28;
									Bitmap* blank;
									blank = new Bitmap(iSize, iSize,PixelFormat32bppRGB);
									Graphics bgr(blank);
									SolidBrush whiteBrush(Color(255, 255, 255, 255));
									bgr.FillRectangle(&whiteBrush, 0, 0, iSize, iSize);
									Pen grayPen(Color(255, 200,200,200), iSize/16);
									grayPen.SetLineCap(LineCapNoAnchor, LineCapNoAnchor, DashCapRound); 
									bgr.SetSmoothingMode(SmoothingModeAntiAlias);
									bgr.SetCompositingQuality ( CompositingQualityHighQuality ) ;
									bgr.SetInterpolationMode( InterpolationModeHighQuality ) ; 
									Point startPoint;
									Point endPoint;

									startPoint.X = iSize/8;
									startPoint.Y = iSize/8;
									endPoint.X = iSize - iSize/8;
									endPoint.Y = iSize - iSize/8;

									bgr.DrawLine(&grayPen, startPoint, endPoint);

									startPoint.Y = iSize - iSize/8;
									endPoint.Y = iSize/8;
									bgr.DrawLine(&grayPen, startPoint, endPoint);

									HBITMAP bhandle = NULL;
									blank->GetHBITMAP(Color::White, &bhandle);
									ImageList_Replace(hListTree, i, bhandle, NULL);
									delete blank;
									DeleteObject(bhandle);

									GestureNames[i] = "%%STROKESPLUS_DELETED_GESTURE%%";

									ShowWindow(GetDlgItem(hDlg, IDC_TREEAPPS), SW_HIDE);
									ShowWindow(GetDlgItem(hDlg, IDC_TREEAPPS), SW_SHOW);
									break;
								}
							}
							break;
						}
					}
				}
			}

			EnableWindow(GetDlgItem(hDlg, IDC_CFG_DELG),true);

			break;
		}	

		if (LOWORD(wParam) == IDC_CFGBTNADDACTION)
		{

			if(!bNameOpen) {
				bConfigNameAction = TRUE;
				EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNADDACTION),false);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
				EnableWindow(GetDlgItem(hDlg, IDC_CFGBTNADDACTION),true);
			}

		}		

		if (LOWORD(wParam) == ID_TVPOPUP_COPY) {
			rightClickCopyNode = itemUnderMouse;
			rightClickCutMode = FALSE;
		}

		if (LOWORD(wParam) == ID_TVPOPUP_PASTE) {
			PasteAction();
			if(rightClickCutMode) {
				RemoveAction(rightClickCopyNode);
				rightClickCopyNode = NULL;
				rightClickCutMode = FALSE;
			}
		}

		if (LOWORD(wParam) == ID_TVPOPUP_CUT) {
			rightClickCopyNode = itemUnderMouse;
			rightClickCutMode = TRUE;
		}

		if (LOWORD(wParam) == ID_TVPOPUP_ADDAPP) {
			PostMessage(hDlg,WM_COMMAND, MAKEWPARAM(IDC_CFGBTNADDAPP,0), MAKELPARAM(0,0));
		}

		if (LOWORD(wParam) == ID_TVPOPUP_ADDACTION) {
			TreeView_SelectItem(GetDlgItem(hDlg, IDC_TREEAPPS), itemUnderMouse);
			PostMessage(hDlg,WM_COMMAND, MAKEWPARAM(IDC_CFGBTNADDACTION,0), MAKELPARAM(0,0));
		}

		if (LOWORD(wParam) == ID_TVPOPUP_RENAME) {
			TreeView_SelectItem(GetDlgItem(hDlg, IDC_TREEAPPS), itemUnderMouse);
			PostMessage(hDlg,WM_COMMAND, MAKEWPARAM(IDC_CFGBTNRENAME,0), MAKELPARAM(0,0));
		}

		if (LOWORD(wParam) == ID_TVPOPUP_DELETE) {
			TreeView_SelectItem(GetDlgItem(hDlg, IDC_TREEAPPS), itemUnderMouse);
			PostMessage(hDlg,WM_COMMAND, MAKEWPARAM(IDC_CFGBTNDELETE,0), MAKELPARAM(0,0));
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#pragma endregion ConfigTab (Actions)


#pragma region HotkeyTab

INT_PTR CALLBACK HotkeyTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		LoadHotkeyVectors();
		SetWindowText(GetDlgItem(hDlg, IDC_GRP_HKACTIONHELP), ptLang.get("language.Hotkeys.AvailableActionsFrame.Title",_T("Available Actions")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HK_INFOACTION), ptLang.get("language.Hotkeys.AvailableActionsFrame.Info",_T("Info")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HK_INSACTION), ptLang.get("language.Hotkeys.AvailableActionsFrame.Insert",_T("Insert")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HKTEXT), ptLang.get("language.Hotkeys.FormText.InfoText",_T("Hotkeys are global actions that will fire whenever you press the corresponding key combination. \n\nNote that mouse-related variables are not passed into hotkey Lua scripts; such as gsx, gwd, etc.")).c_str());
		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
			if(v1.second.get("<xmlattr>.name",_T("")) != _T("")){
				SendDlgItemMessage(hDlg,IDC_CBO_HKACTIONS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)v1.second.get("<xmlattr>.name",_T("")).c_str()));
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					SendDlgItemMessage(hDlg,IDC_CBO_HKACTIONS,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)v2.second.get("<xmlattr>.name",_T("")).c_str()));
				}
			}
		}		
		
		SetWindowText(GetDlgItem(hDlg, IDC_CHKHKACTIVE), ptLang.get("language.Hotkeys.Active",_T("Hotkey Active ")).c_str());

		SetWindowText(GetDlgItem(hDlg, IDC_HK_HOTKEY_GROUP), ptLang.get("language.Hotkeys.HotkeysFrame.Title",_T("Hotkey")).c_str());
		for(int i = 1; i < 5; i++) { 
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))),CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Hotkeys.HotkeysFrame.Modifiers.None",_T("None")).c_str()));
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))),CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Hotkeys.HotkeysFrame.Modifiers.Control",_T("Control")).c_str()));
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))),CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Hotkeys.HotkeysFrame.Modifiers.Alt",_T("Alt")).c_str()));
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))),CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Hotkeys.HotkeysFrame.Modifiers.Shift",_T("Shift")).c_str()));
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))),CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)ptLang.get("language.Hotkeys.HotkeysFrame.Modifiers.Win",_T("Windows")).c_str()));
			SendDlgItemMessage(hDlg,(i==1 ? IDC_HOTKEY_MOD1 : (i==2 ? IDC_HOTKEY_MOD2 : (i==3 ? IDC_HOTKEY_MOD3 : IDC_HOTKEY_MOD4))), CB_SETCURSEL, 0, 0);
		}
		SetWindowText(GetDlgItem(hDlg, IDC_HOTKEY_LABEL_MOD1), ptLang.get("language.Hotkeys.HotkeysFrame.Modifier1",_T("Modifier 1:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HOTKEY_LABEL_MOD2), ptLang.get("language.Hotkeys.HotkeysFrame.Modifier2",_T("Modifier 2:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HOTKEY_LABEL_MOD3), ptLang.get("language.Hotkeys.HotkeysFrame.Modifier3",_T("Modifier 3:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HOTKEY_LABEL_MOD4), ptLang.get("language.Hotkeys.HotkeysFrame.Modifier4",_T("Modifier 4:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HOTKEY_LABEL_KEY), ptLang.get("language.Hotkeys.HotkeysFrame.Key",_T("Key:")).c_str());
		for(int i=0; i < HotkeyNames.size(); i++){
			SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY,CB_ADDSTRING,0, reinterpret_cast<LPARAM>((LPCTSTR)HotkeyNames[i].c_str()));
		}
		SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_SETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_SETDROPPEDWIDTH, 200, 0);
		
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_HK_CLUA), ptLang.get("language.Hotkeys.FormText.LuaScriptTitle",_T("Lua Script:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HKBTNADDHOTKEY), ptLang.get("language.Hotkeys.FormButtons.AddHotkey",_T("Add Hotkey")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HKBTNRENAME), ptLang.get("language.Hotkeys.FormButtons.Rename",_T("Rename")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_HKBTNDELETE), ptLang.get("language.Hotkeys.FormButtons.Delete",_T("Delete")).c_str());

		hWndHotkey = hDlg;

		ShowWindow (GetDlgItem(hDlg, IDC_EDITHKLUA), SW_HIDE);

		EnableHotkeyFields(hDlg, false);

		BuildHotkeyTree(hDlg);
				
		return (INT_PTR)FALSE;
	}

	case WM_NOTIFY:
	{
	    case IDC_TREEHOTKEYS:
		{
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGING) 
			{
				if(bAutosave && !bIsSaving) {
					BOOL bContinue = FALSE;
					EnterCriticalSection(&CriticalSection); 
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
					if(!bIsSaving){
						bIsSaving = TRUE;
						if(hDlg,pnmtv->itemOld.hItem != NULL){
							bContinue = SaveHotkeyChange(hDlg,pnmtv->itemOld.hItem);
						} else {
							bContinue = true;
						}
					}
					bIsSaving = FALSE;
					LeaveCriticalSection(&CriticalSection);
					if(!bContinue){
						SetWindowLongPtr(hDlg, DWLP_MSGRESULT, 1);
						return TRUE;
					}
				}
			}
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGED) 
			{
				LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
				LoadHotkeyNode(hDlg,pnmtv->itemNew.hItem);
			}

		}
		break;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_HKBTNDELETE) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_HKBTNDELETE),false);
			HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEHOTKEYS));
			TVITEM tvi;
			ZeroMemory(&tvi,sizeof(tvi));
			tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
			tvi.hItem=hSelected;
			char cText[255] = {0};
			tvi.pszText = cText;
			tvi.cchTextMax=256;
			SendMessage(GetDlgItem(hDlg , IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

				if(MessageBox(hWndSettings,ptLang.get("language.Hotkeys.Messages.DeleteTreeItem",_T("Are you sure?")).c_str(),ptLang.get("language.Hotkeys.Messages.DeleteTreeItemTitle",_T("Delete")).c_str(),MB_ICONWARNING + MB_YESNO + MB_DEFBUTTON2 + MB_APPLMODAL) == IDYES)
				{
					EnableHotkeyFields(hDlg, false);
					HTREEITEM hNext = TreeView_GetNextSibling(GetDlgItem(hDlg , IDC_TREEHOTKEYS),hSelected);
					HTREEITEM hPrev = TreeView_GetPrevSibling(GetDlgItem(hDlg , IDC_TREEHOTKEYS),hSelected);
					//HTREEITEM hPar = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEHOTKEYS),hSelected);

					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD1, CB_SETCURSEL, -1, 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD2, CB_SETCURSEL, -1, 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD3, CB_SETCURSEL, -1, 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_MOD4, CB_SETCURSEL, -1, 0);
					SendDlgItemMessage(hDlg,IDC_HOTKEY_KEY, CB_SETCURSEL, -1, 0);
					SendDlgItemMessage(hDlg, IDC_CHKHKACTIVE, BM_SETCHECK, BST_CHECKED, 0);

					std::basic_string<TCHAR> sLua = _T("");
					SendHotkeyEditor(SCI_SETTEXT, 0, (LPARAM)(LPCTSTR)sLua.c_str());

					TreeView_DeleteItem(GetDlgItem(hDlg , IDC_TREEHOTKEYS),hSelected);
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Global.Hotkeys")) {
						//each app
						if(_tcsicmp( cText, v2.second.get("<xmlattr>.description",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
							v2.second.clear();
						}
					}

					TreeView_SelectItem(GetDlgItem(hDlg , IDC_TREEHOTKEYS),(hNext != NULL ? hNext : (hPrev != NULL ? hPrev : NULL)));
				} else {
					EnableWindow(GetDlgItem(hDlg, IDC_HKBTNDELETE),true);
				}
		
		}

		if (LOWORD(wParam) == IDC_HKBTNRENAME) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_HKBTNRENAME),false);
			if(!bNameOpen){
				HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEHOTKEYS));
				TVITEM tvi;
				ZeroMemory(&tvi,sizeof(tvi));
				tvi.mask=TVIF_TEXT | TVIF_CHILDREN;
				tvi.hItem=hSelected;
				char cText[255] = {0};
				tvi.pszText = cText;
				tvi.cchTextMax=256;
				SendMessage(GetDlgItem(hDlg , IDC_TREEHOTKEYS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
				bConfigRenameHotkey = TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_HKBTNRENAME),true);
			break;	
		}


		if (LOWORD(wParam) == IDC_HKBTNADDHOTKEY)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_HKBTNADDHOTKEY),false);
			if(!bNameOpen) {
				bConfigNameHotkey = TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_HKBTNADDHOTKEY),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_HK_INSACTION)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_HK_INSACTION),false);
			CHAR szActionName[255];
			memset(szActionName,0,sizeof(szActionName));
			GetDlgItemText(hDlg, IDC_CBO_HKACTIONS, szActionName, 255);
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					if(_tcsicmp( szActionName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
					{
						std::basic_string<TCHAR> sHelpBuffer = _T("");
						sHelpBuffer.append(v2.second.get("snippet",_T("")));
						SetFocus(GetDlgItem(hDlg, IDC_EDITHKLUA));
						SendHotkeyEditor(SCI_REPLACESEL, 0, LPARAM(sHelpBuffer.c_str()));
						SetFocus(m_hwndHotkeyEditor);
					}
				}
			}
			EnableWindow(GetDlgItem(hDlg, IDC_HK_INSACTION),true);
			break;
		}	

		if (LOWORD(wParam) == IDC_HK_INFOACTION)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_HK_INFOACTION),false);
			CHAR szActionName[255];
			memset(szActionName,0,sizeof(szActionName));
			GetDlgItemText(hDlg, IDC_CBO_HKACTIONS, szActionName, 255);
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, ptActions.get_child("actionshelp.groups")) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v1.second.get_child("actions")) {
					if(_tcsicmp( szActionName, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0 )
					{
						std::basic_string<TCHAR> sHelpBuffer = ptLang.get("language.Hotkeys.AvailableActionsFrame.Messages.InfoPopup.Description",_T("Description:")).append("\r\n  ");
						sHelpBuffer.append(v2.second.get("description",_T("")));
						sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Hotkeys.AvailableActionsFrame.Messages.InfoPopup.Usage",_T("Usage:")).append("\r\n  "));
						sHelpBuffer.append(v2.second.get("signature",_T("")));
						sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Hotkeys.AvailableActionsFrame.Messages.InfoPopup.Returns",_T("Returns:")).append("\r\n  "));
						sHelpBuffer.append(v2.second.get("return.<xmlattr>.type",_T("")));
						sHelpBuffer.append(_T(" - "));
						sHelpBuffer.append(v2.second.get("return",_T("")));
						BOOL bParamWritten = FALSE;
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second.get_child(_T("parameters"))) {
							if(!bParamWritten){
								sHelpBuffer.append(_T("\r\n\r\n")).append(ptLang.get("language.Hotkeys.AvailableActionsFrame.Messages.InfoPopup.Parameters",_T("Parameters:")));
								bParamWritten = TRUE;
							}
							sHelpBuffer.append(_T("\r\n  "));
							sHelpBuffer.append(v3.second.get("<xmlattr>.name",_T("")));
							sHelpBuffer.append(_T(": "));
							sHelpBuffer.append(v3.second.get_value(_T("")));
						}

						MessageBox(hDlg, sHelpBuffer.c_str(), szActionName, NULL);
					}
				}
			}
			EnableWindow(GetDlgItem(hDlg, IDC_HK_INFOACTION),true);
			break;
		}	
	}	
	}
	
	return (INT_PTR)FALSE;
}

#pragma endregion HotkeyTab


#pragma region IgnoredTab

INT_PTR CALLBACK IgnoredTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetWindowText(GetDlgItem(hDlg, IDC_OWNT), ptLang.get("language.Ignored.OwnerTitle",_T("Owner Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_OWNTR), ptLang.get("language.Ignored.OwnerTitlePattern",_T("Owner Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_OWNC), ptLang.get("language.Ignored.OwnerClassName",_T("Owner Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_OWNCR), ptLang.get("language.Ignored.OwnerClassNamePattern",_T("Owner Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PART), ptLang.get("language.Ignored.ParentTitle",_T("Parent Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PARTR), ptLang.get("language.Ignored.ParentTitlePattern",_T("Parent Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PARC), ptLang.get("language.Ignored.ParentClassName",_T("Parent Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_PARCR), ptLang.get("language.Ignored.ParentClassNamePattern",_T("Parent Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CTT), ptLang.get("language.Ignored.ControlTitle",_T("Control Title:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CID), ptLang.get("language.Ignored.ControlID",_T("Control ID:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CTTR), ptLang.get("language.Ignored.ControlTitlePattern",_T("Control Title Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CTC), ptLang.get("language.Ignored.ControlClassName",_T("Control Class Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CTCR), ptLang.get("language.Ignored.ControlClassNamePattern",_T("Control Class Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_FN), ptLang.get("language.Ignored.FileName",_T("File Name:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_FNR), ptLang.get("language.Ignored.FileNamePattern",_T("File Name Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_FPR), ptLang.get("language.Ignored.FilePathPattern",_T("File Path Pattern:")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_FINDWINDOW), ptLang.get("language.Ignored.FindWindowFrame",_T("Find Window")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_DISABLEACTIVE), ptLang.get("language.Ignored.DisableOnActivate",_T("Disable S+ if this application is activated")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_QUALIFIER), ptLang.get("language.Ignored.Qualifiers",_T("Anything which matches all of the qualifiers in this list will be ignored when the stroke button is pressed. The pattern fields are for regular expression matches. Only one field is required.")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_IGNBTNADD), ptLang.get("language.Ignored.Add",_T("Add")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_IGNBTNRENAME), ptLang.get("language.Ignored.Rename",_T("Rename")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_IGNBTNDELETE), ptLang.get("language.Ignored.Delete",_T("Delete")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_CHK_IGNAPPACTIVE), ptLang.get("language.Ignored.Active",_T("Rule Active ")).c_str());

		hWndIgnore = hDlg;

		ShowIgnoreFields(hDlg, false);
		EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNDELETE),false);
		EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNRENAME),false);
		BuildIgnoreTree(hDlg);
		
		SetFocus(GetDlgItem(hDlg, IDC_TREEIGNOREDAPPS));

		return (INT_PTR)TRUE;
	}

	case WM_NOTIFY:
	{

	    case IDC_TREEIGNOREDAPPS:
		{
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGING) 
			{
				if(bAutosave && !bIgnoredIsSaving) {
					BOOL bContinue = FALSE;
					EnterCriticalSection(&CriticalSection); 
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
					if(!bIgnoredIsSaving){
						bIgnoredIsSaving = TRUE;
						if(hDlg,pnmtv->itemOld.hItem != NULL){
							bContinue = SaveIgnoredChange(hDlg,pnmtv->itemOld.hItem);
						} else {
							bContinue = true;
						}
					}
					bIgnoredIsSaving = FALSE;
					LeaveCriticalSection(&CriticalSection);
					if(!bContinue){
						SetWindowLongPtr(hDlg, DWLP_MSGRESULT, 1);
						return TRUE;
					}
				}
			}

			if(((LPNMHDR)lParam)->code == TVN_SELCHANGED) // if code == NM_CLICK - Single click on an item
			{
				sLastIgnore = _T("");
				EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNDELETE),false);
				EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNRENAME),false);
				HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS));
				TVITEM tvi;
				ZeroMemory(&tvi,sizeof(tvi));
				tvi.mask=TVIF_TEXT;
				tvi.hItem=hSelected;
				char cText[255] = {0};
                tvi.pszText = cText;
				tvi.cchTextMax=256;
				SendMessage(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);
				sLastIgnore = cText;
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Ignored")) {
					//each ignored window
					if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/)
					{
						BOOST_FOREACH(boost::property_tree::ptree::value_type &v3, v2.second) {
							//each match (technically, everything at the MatchCriteria level, like Actions..but leaving for now, TODO: optimize, no reason to loop
							if(v3.first == "MatchCriteria"){
								SetDlgItemText (hDlg, IDC_EDIT_OWNERTITLE, v3.second.get("ownertitle",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_OWNERTITLE_REGEX, v3.second.get("ownertitleregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_OWNERCLASS, v3.second.get("ownerclass",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_OWNERCLASS_REGEX, v3.second.get("ownerclassregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_PARENTTITLE, v3.second.get("parenttitle",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_PARENTTITLE_REGEX, v3.second.get("parenttitleregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_PARENTCLASS, v3.second.get("parentclass",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_PARENTCLASS_REGEX, v3.second.get("parentclassregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_CONTROLTITLE, v3.second.get("controltitle",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_CONTROLTITLE_REGEX, v3.second.get("controltitleregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_CONTROLCLASS, v3.second.get("controlclass",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_CONTROLCLASS_REGEX, v3.second.get("controlclassregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_CONTROLID, v3.second.get("controlid",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_FILENAME, v3.second.get("file",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_FILENAME_REGEX, v3.second.get("fileregex",_T("")).c_str());
								SetDlgItemText (hDlg, IDC_EDIT_PATH_REGEX, v3.second.get("pathregex",_T("")).c_str());
								SendDlgItemMessage(hDlg, IDC_CHK_DISABLEACTIVE, BM_SETCHECK, (v3.second.get("disableonactivate",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								SendDlgItemMessage(hDlg, IDC_CHK_IGNAPPACTIVE, BM_SETCHECK, (v2.second.get("<xmlattr>.enabled",0) == 1) ? BST_CHECKED : BST_UNCHECKED, 0);
								EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNDELETE),true);
								EnableWindow(GetDlgItem(hDlg , IDC_IGNBTNRENAME),true);
								ShowIgnoreFields(hDlg, true);
							}
						}
					}

				}
			}
		}
	}

	case WM_MOUSEMOVE :
	{
	  if (g_bStartSearchWindow)
	  {
	    // Only when we have started the Window Searching operation will we 
		// track mouse movement.
	    DoMouseMove(hDlg, message, wParam, lParam);
	  }

	  break;
	}

	case WM_LBUTTONUP :
	{
	  if (g_bStartSearchWindow)
	  {
	    // Only when we have started the window searching operation will we
		// be interested when the user lifts up the left mouse button.
	    DoMouseUp(hDlg, message, wParam, lParam);
	  }

	  break;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_IGNBTNDELETE) 
		{
			//load config
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNDELETE),false);
			HTREEITEM hSelected = TreeView_GetSelection(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS));
			TVITEM tvi;
			ZeroMemory(&tvi,sizeof(tvi));
			tvi.mask=TVIF_TEXT;
			tvi.hItem=hSelected;
			char cText[255] = {0};
            tvi.pszText = cText;
			tvi.cchTextMax=256;
			SendMessage(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),TVM_GETITEM,TVGN_CARET,(LPARAM)&tvi);

			if(MessageBox(hWndSettings,ptLang.get("language.Ignored.Messages.DeleteItem",_T("Are you sure?")).c_str(),ptLang.get("language.Ignored.Messages.DeleteItemTitle",_T("Delete")).c_str(),MB_ICONWARNING + MB_YESNO + MB_DEFBUTTON2 + MB_APPLMODAL) == IDYES)
			{

				HTREEITEM hNext = TreeView_GetNextSibling(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),hSelected);
				HTREEITEM hPrev = TreeView_GetPrevSibling(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),hSelected);
				HTREEITEM hPar = TreeView_GetParent(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),hSelected);

				TreeView_DeleteItem(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),hSelected);

				BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, ptConfig.get_child("config.Ignored")) {
					//each ignored window
					if(_tcsicmp( cText, v2.second.get("<xmlattr>.name",_T("")).c_str() ) == 0/* && v2.second.get("<xmlattr>.enabled",0) == 1*/) {
						v2.second.clear();
					}
				}

				TreeView_SelectItem(GetDlgItem(hDlg , IDC_TREEIGNOREDAPPS),(hNext != NULL ? hNext : (hPrev != NULL ? hPrev : hPar)));
			}
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNDELETE),true);

		}

		if (LOWORD(wParam) == IDC_IGNBTNRENAME) 
		{
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNRENAME),false);
			if(!bNameOpen){
				bIgnoreRename = TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNRENAME),true);
			return (INT_PTR)TRUE;		
		}

		if (LOWORD(wParam) == IDC_IGNBTNADD)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNADD),false);
			if(!bNameOpen) {
				bIgnoreName = TRUE;
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGNAME), hWndSettings, NameProc);	
			}
			EnableWindow(GetDlgItem(hDlg, IDC_IGNBTNADD),true);
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDC_STATIC_ICON_FINDER_TOOL)
		{
			// Because the IDC_STATIC_ICON_FINDER_TOOL static control is set with the SS_NOTIFY
			// flag, the Search Window's dialog box will be sent a WM_COMMAND message when this 
			// static control is clicked.
			// We start the window search operation by calling the DoSearchWindow() function.
			SearchWindow(hDlg);
			break;
		}

		break;
	}
	}
	return (INT_PTR)FALSE;
}

#pragma endregion IgnoredTab


#pragma region ReportTab

INT_PTR CALLBACK ReportTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		{

		SetWindowText(GetDlgItem(hDlg, IDC_TOPTEXT), ptLang.get("language.Report.TopText",_T("This is a list of all active actions.  Click the column headings to sort the report as desired.")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_BTN_REFRESH), ptLang.get("language.Report.Refresh",_T("Refresh")).c_str());
		SetWindowText(GetDlgItem(hDlg, IDOK), ptLang.get("language.Report.Close",_T("Close")).c_str());

		//COLUMN SETUP
		LVCOLUMN lvcol;
		TCHAR scolDesc[255];
		ZeroMemory (&lvcol, sizeof(LVCOLUMN));
		lvcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvcol.cx = 66;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.Gesture",_T("Gesture")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.cchTextMax = 255;
		lvcol.iSubItem = 0;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 0, &lvcol);
		lvcol.cx = 130;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.GestureName",_T("Gesture Name")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.iSubItem = 1;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 1, &lvcol);
		lvcol.cx = 147;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.Modifiers",_T("Modifiers")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.iSubItem = 2;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 2, &lvcol);
		lvcol.cx = 170;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.AppName",_T("App Name")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.iSubItem = 3;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 3, &lvcol);
		lvcol.cx = 214;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.ActionName",_T("Action Name")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.iSubItem = 4;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 4, &lvcol);
		lvcol.cx = 205;
		_tcscpy(scolDesc, ptLang.get("language.Report.Columns.LuaScript",_T("Lua Script")).c_str());
		lvcol.pszText = scolDesc;
		lvcol.iSubItem = 5;
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_LISTAPPS), 5, &lvcol);

		hWndReport = hDlg;

		LPARAM lParamSort = -5;
		ListView_SortItemsEx(GetDlgItem(hDlg,IDC_LISTAPPS), ListViewCompareProc, lParamSort);

		lParamSort = -2;
		ListView_SortItemsEx(GetDlgItem(hDlg,IDC_LISTAPPS), ListViewCompareProc, lParamSort);

		return (INT_PTR)TRUE;
		}

	case WM_NOTIFY:
	{
	    case IDC_LISTAPPS:
		{
			if((((LPNMHDR)lParam)->idFrom == IDC_LISTAPPS) && ((LPNMHDR)lParam)->code == LVN_COLUMNCLICK) 
			{

				// combine sort info into a single value we can send to our sort function
				static int nSortColumn = 0;
				static BOOL bSortAscending = TRUE;
				LPARAM lParamSort;

				// get new sort parameters
				if (((LPNMLISTVIEW)lParam)->iSubItem == nSortColumn)
					bSortAscending = !bSortAscending;
				else
				{
					nSortColumn = ((LPNMLISTVIEW)lParam)->iSubItem;
					bSortAscending = TRUE;
				}

				lParamSort = 1 + nSortColumn;
				if (!bSortAscending)
					lParamSort = -lParamSort;

				// sort list
				ListView_SortItemsEx(((LPNMLISTVIEW)lParam)->hdr.hwndFrom, ListViewCompareProc, lParamSort);
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BTN_REFRESH)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_REFRESH),false);
			ListView_DeleteAllItems(GetDlgItem(hDlg,IDC_LISTAPPS));

			BuildConfigList(hDlg);

			LPARAM lParamSort = -5;
			ListView_SortItemsEx(GetDlgItem(hDlg,IDC_LISTAPPS), ListViewCompareProc, lParamSort);

			lParamSort = -2;
			ListView_SortItemsEx(GetDlgItem(hDlg,IDC_LISTAPPS), ListViewCompareProc, lParamSort);

			EnableWindow(GetDlgItem(hDlg, IDC_BTN_REFRESH),true);
		}
		break;
	}

	}
	return (INT_PTR)FALSE;
}

#pragma endregion ReportTab


#pragma region LuaTab

INT_PTR CALLBACK LuaTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
			SetWindowText(GetDlgItem(hDlg, IDC_LUATEXT), ptLang.get("language.LuaTab.Text",_T("The Lua code below is your StrokesPlus.lua file. You can utilize this file to store your own functions or variables which are accessible by all action scripts.\nAny changes here are saved and loaded into the Lua engine immediately when you click Apply or OK.")).c_str());

			InitialiseLuaEditor();

		return (INT_PTR)TRUE;
	}

	}
	return (INT_PTR)FALSE;
}

#pragma endregion LuaTab


#pragma region AboutTab

INT_PTR CALLBACK AboutTab(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//Version from file	
			WIN32_FILE_ATTRIBUTE_DATA fad;
			SYSTEMTIME st;
			TCHAR pFmt[255];
			TCHAR szFileName[MAX_PATH];
			DWORD               dwSize              = 0;
			BYTE                *pbVersionInfo      = NULL;
			VS_FIXEDFILEINFO    *pFileInfo          = NULL;
			UINT                puLenFileInfo       = 0;
			TCHAR buffer[256];
			TCHAR szLocalDate[255];

			GetModuleFileName( NULL, szFileName, MAX_PATH );
			GetFileAttributesEx(szFileName, GetFileExInfoStandard, &fad);
		
			FileTimeToSystemTime( &fad.ftLastWriteTime, &st );
			wsprintf(szLocalDate, _T("%04d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);

			_tcscpy(buffer,_T("(error)"));
			dwSize = GetFileVersionInfoSize( szFileName, NULL );
			if ( dwSize != 0 )
			{
				pbVersionInfo = new BYTE[ dwSize ];
				if(GetFileVersionInfo( szFileName, 0, dwSize, pbVersionInfo ) ){
					if(VerQueryValue( pbVersionInfo, TEXT("\\"), (LPVOID*) &pFileInfo, &puLenFileInfo ) ){
						wsprintf( buffer,"%d.%d.%d.%d",
							HIWORD(pFileInfo->dwFileVersionMS),
							LOWORD(pFileInfo->dwFileVersionMS),
							HIWORD(pFileInfo->dwFileVersionLS),
							LOWORD(pFileInfo->dwFileVersionLS)
							);
					}
				}
				delete[] pbVersionInfo;		
			}

			SetWindowText(GetDlgItem(hDlg, IDC_HLPVERSION), ptLang.get("language.About.Version",_T("Version: ")).append(buffer).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_HLPRELEASEDATE), ptLang.get("language.About.ReleaseDate",_T("Release Date: ")).append(szLocalDate).c_str());
			SetWindowText(GetDlgItem(hDlg, IDC_HLPSPLUS), ptLang.get("language.About.StrokesPlus",_T("StrokesPlus")).c_str());

			
		return (INT_PTR)TRUE;
	}

	}
	return (INT_PTR)FALSE;
}

#pragma endregion AboutTab
