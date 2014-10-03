

#pragma region "Definitions, Templates, Includes"

#include "stdafx.h"
#include "StrokesPlus.h"
#include "shellapi.h"
#include "psapi.h"
#include <vector>
using namespace std;

#include "OCIdl.h"
#include "commctrl.h"
#include "Commdlg.h"

#pragma comment( lib, "psapi.lib" )

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define _WIN32_IE 0x500

#define WM_TRAYMESSAGE (WM_USER + 1)
#define WM_LOADHOTKEYS (WM_USER + 22)
#define WM_SHOWGESTURENAMEDLG (WM_USER + 3)
#define WM_SHOWDISABLEICON (WM_USER + 5)
#define WM_RESETDISABLEICON (WM_USER + 6)
#define WM_TOGGLETRAYICON (WM_USER + 7)
#define WM_DISABLEHOTKEY (WM_USER + 8)
#define WM_ENABLEHOTKEY (WM_USER + 9)
#define ID_ICONPOPUP_EXIT 120
#define ID_ICONPOPUP_HOTKEYS 161
#define ID_ICONPOPUP_EXIT_NOSAVE 160
#define ID_ICONPOPUP_DISABLED 122
#define ID_ICONPOPUP_PREF 123
#define ID_ICONPOPUP_MIDDLEBUTTON 124
#define ID_ICONPOPUP_RIGHTBUTTON 125
#define ID_ICONPOPUP_LEFTBUTTON 128
#define ID_ICONPOPUP_X1BUTTON 137
#define ID_ICONPOPUP_X2BUTTON 136
#define ID_ICONPOPUP_CTRLBUTTON 140
#define ID_ICONPOPUP_ALTBUTTON 141
#define ID_ICONPOPUP_SHIFTBUTTON 142
#define ID_ICONPOPUP_DRAW 126
#define ID_ICONPOPUP_LEARN 133
#define ID_ICONPOPUP_CANCEL 131
#define ID_ICONPOPUP_ABOUT 143
#define ID_USERIGHT 127
#define ID_USEMIDDLE 129
#define ID_USELEFT 130
#define ID_ICONPOPUP_CONFIG 132
#define ID_ICONPOPUP_IGNORED 134
#define ID_ICONPOPUP_RELOAD 135
#define ID_ICONPOPUP_HELP 138
#define MAX_LOADSTRING 100
#define ID_ICONPOPUP_CUSTOM 139
#define ID_ICONPOPUP_SETTINGS 144
#define ID_ICONPOPUP_PASSWORD 145
#define ID_ICONPOPUP_LOADCONFIG 162
#define ID_ICONPOPUP_RELOAD_DELAY 163
#define WM_MENUPOPUP (WM_USER + 1024)


#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

#define IDT_DCMSG 1
#define IDT_CLICKMSG 2

#pragma endregion "Definitions, Templates, Includes"


#pragma region "Global Variables"

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
BOOL bDCDisableState = TRUE;
UINT_PTR dctimerid;
int trayID = 100;	//Holds the instance id of the tray icon, incremented if explorer crashes..
HMENU menu;  //For tray icon
HMENU hpopup; // ^
NOTIFYICONDATA nid; // ^
TCHAR strExePath [_MAX_PATH];
TCHAR strStrokesConfigFile [_MAX_PATH];
HWND hWnd;
ULONG_PTR m_gdiplusToken;
BOOL bInToggleTrayIcon = false;
CRITICAL_SECTION CriticalSection; 

#pragma endregion "Global Variables"


#pragma region "Main Entry and Init Functions"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	BOOL bAlreadyRunning = FALSE;

	LOGMESSAGE("About to get mutex");

	HANDLE mut = CreateMutex( NULL, TRUE, _T("Global\\{db87963c-c6fa-46de-866d-4f067aed62a1}") );
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
		LOGMESSAGE("Already running");
		bAlreadyRunning = TRUE;
	}

	//This section is just for handling high DPI settings..it's not perfect, but it makes S+ mostly usable for those configs
	LOGMESSAGE("About to set DPI");
	try{
		int xDPI = 96;
		HDC hDCDesktop = GetDC(GetDesktopWindow());
		xDPI = GetDeviceCaps(hDCDesktop, LOGPIXELSX);
		ReleaseDC(NULL, hDCDesktop);
		if(xDPI != 96){
			setDPIModifier((((float)xDPI)/((float)96))*1.1);
		}
	} catch (std::exception& e) {
		LOGMESSAGE("Failed getting DPI (catch), setting to 1");
		setDPIModifier(1);
	}

 	// TODO: Place code here.   <- Heh, this was from the original start project template..leaving here for historical purposes =)
	MSG msg;

	LOGMESSAGE("About to init global strings");
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_STROKESPLUS, szWindowClass, MAX_LOADSTRING);
	LOGMESSAGE("About to register class");
	MyRegisterClass(hInstance);

	LOGMESSAGE("About to set current path");
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
		LOGMESSAGE("Command line parameters, set config file path to args");
		_tcscpy(strStrokesConfigFile, sCmdLine.c_str());
	} else {
		LOGMESSAGE("About to check for config file along side EXE");
		_tcscpy(strStrokesConfigFile, _T("StrokesPlus.xml"));
		fstream tmpConfig(strStrokesConfigFile, ifstream::in | ifstream::out);
		if(!tmpConfig.good()){
			LOGMESSAGE("Nope, get from %appdata%");
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

	// Perform application initialization:
	LOGMESSAGE("About to call InitInstance");
	if (!InitInstance (hInstance, nCmdShow))
	{
		LOGMESSAGE("InitInstance Failed, exiting");
		return FALSE;
	}

	LOGMESSAGE("About to check setting for multiple instance");
	if(bAlreadyRunning && !getHideAdditionalInstanceMessage()){
		if(MessageBox( NULL, ptLang.get("language.Base.Messages.AlreadyRunning",_T("StrokesPlus is already running.\n\nClick Yes if you really intend to run another instance of StrokesPlus.")).c_str(), ptLang.get("language.Base.Messages.AlreadyRunningTitle",_T("Crafty or just senile?")).c_str(), MB_ICONEXCLAMATION | MB_YESNO | MB_SETFOREGROUND ) == IDNO){
			return 1;
		}
	}

	LOGMESSAGE("About to check tray icon visible status");
	if(getTrayIconVisible()){
		LOGMESSAGE("About to create tray icon");
		createTrayIcon();
	}

	LOGMESSAGE("About to call AllowSetForegroundWindow");
	AllowSetForegroundWindow(ASFW_ANY);

	LOGMESSAGE("About to set priority class");
	SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);

	if(!getGesturesDisabled()) {
		LOGMESSAGE("About to set hook");
		setMyHook(hWnd);
	} else {
		setWindowState();
	}

	LOGMESSAGE("About to init critical section");
	InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);
	
	LOGMESSAGE("About to initLua instances");
	initLua();

	LOGMESSAGE("About to enter main message loop");
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
		
	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STROKESPLUS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= CreateSolidBrush(RGB(255,255,255));
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_STROKESPLUS));
	wcex.lpszMenuName	= NULL;

	return RegisterClassEx(&wcex);
}

void CheckForOtherGesturePrograms()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

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
					GetModuleBaseName( hProcess, hMod, (LPTSTR)szProcessName,
									   sizeof(szProcessName) );
					if(_tcsicmp(szProcessName, _T("strokeit.exe")) == 0
						|| _tcsicmp(szProcessName, _T("highsign.exe")) == 0
						|| _tcsicmp(szProcessName, _T("mousewrangler.exe")) == 0
						|| _tcsicmp(szProcessName, _T("mazzick.exe")) == 0
						|| _tcsicmp(szProcessName, _T("gmote.exe")) == 0
						|| _tcsicmp(szProcessName, _T("sensiva.exe")) == 0
						|| _tcsicmp(szProcessName, _T("JustGestures.exe")) == 0)
					{
						MessageBox(NULL, ptLang.get("language.Base.Messages.OtherGestureAppRunning",_T("At least one of the following programs are running:\n\n\tStrokeIt\n\tHigh Sign\n\tSensiva\n\tMouse Wrangler\n\tgMote\n\tMaZZicK\n\tJust Gestures\n\nBe sure only one of us is active or you will not have a good time.\n\nTo disable this message, set CheckForOtherGesturePrograms to 0 in StrokesPlus.xml.")).c_str(), ptLang.get("language.Name.Messages.OtherGestureAppRunningTitle",_T("Captain Hook")).c_str(), MB_ICONEXCLAMATION | MB_OK | MB_SETFOREGROUND );
						break;
					}
				}
			}

			CloseHandle(hProcess);
		}
	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	LOGMESSAGE("About to set OS version");
	if(!setOSVersion()) return false;

	LOGMESSAGE("About to loadHookConfig");
	if(!loadHookConfig()) return false;

	LOGMESSAGE("About to set surrent directory");
	SetCurrentDirectory(strExePath);
	fstream filestr;
	std::basic_string<TCHAR> sLangPath = strStrokesConfigFile;
	boost::replace_all(sLangPath, _T("StrokesPlus.xml"), _T("Language.xml"));
	
	LOGMESSAGE("About to open lang file");
	filestr.open(sLangPath, fstream::in | fstream::out);
	if(filestr){
		try{
			read_xml(filestr, ptLang);
		}
		catch (std::exception& e) {
			MessageBox(NULL, _T("Failed to load/parse Language.xml. \n\nThis isn't a fatal error, but the program text may be missing or incorrect."), _T("¿Que?"), MB_ICONERROR);
		}
	}
	filestr.close();

	LOGMESSAGE("About to check for other gesture programs");
	if(getCheckForOtherGesturePrograms())
		CheckForOtherGesturePrograms();

	hInst = hInstance;

	LOGMESSAGE("About to create main wndow");
	// Create the gesture draw window
	hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP,  
				GetSystemMetrics(SM_XVIRTUALSCREEN),
				GetSystemMetrics(SM_YVIRTUALSCREEN),
				GetSystemMetrics(SM_CXVIRTUALSCREEN),
				GetSystemMetrics(SM_CYVIRTUALSCREEN),
			NULL, NULL, hInstance, NULL);

	LOGMESSAGE("About to show main window, set it as foreground, set additional properties");
    // Show the window to init drawing (prevents flicker on first gesture)
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	AllowSetForegroundWindow(ASFW_ANY);
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT);

	LOGMESSAGE("About to tell the DLL what the hWnd of main window is");
	sethWndServer(hWnd);

	LOGMESSAGE("About to set main window transparency");
	setWindowTransparency();

	LOGMESSAGE("About to init GDI");
	GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

#ifdef SP_TOUCH
	if((isTouchEnabled() || isPenEnabled()) && isTouchAvailable()){
		InitTouchWindow(hWnd);
	}
#endif

	//Hide gesture draw window
	ShowWindow(hWnd, SW_HIDE);

	LOGMESSAGE("About to check setting for foreground timeout");
	if(getCheckForegroundTimeout())
	{
		DWORD timeout;
		LOGMESSAGE("About to check actual foreground timeout");
		try
		{
			SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,0,&timeout,0);
		}
		catch(...)
		{
			LOGMESSAGE("Get foreground timeout failed");
		}
		if(timeout > 0)
			LOGMESSAGE("timeout > 0");
			if(MessageBox(NULL, ptLang.get("language.Base.Messages.ForegroundLockTimeout",_T("Foreground Lock Timeout > 0\n\nTranslation: I may not successfully bring windows to the foreground.\n\nWould you like me to update this setting?")).c_str(),ptLang.get("language.Base.Messages.ForegroundLockTimeoutTitle",_T("Front and Center!")).c_str(),MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND ) == IDYES)
				LOGMESSAGE("About to set foreground timeout");
			bool bRes = false;
			try
			{
				bRes = SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
			}
			catch(...)
			{
				LOGMESSAGE("Set foreground timeout failed");
			}
			if(bRes == 0)
				MessageBox(NULL,ptLang.get("language.Base.Messages.ForegroundLockTimeoutFail",_T("I failed to update the setting.\n\nPerhaps try reinstalling if you experience issues.")).c_str(), ptLang.get("language.Base.Messages.ForegroundLockTimeoutFailTitle",_T("/wrist")).c_str(),MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
	} else {
		LOGMESSAGE("About to set foreground timeout");
		try
		{
			SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
		}
		catch(...)
		{
			LOGMESSAGE("Set foreground timeout failed");
		}
	}

	return true;
}

#pragma endregion "Main Entry and Init Functions"


#pragma region "Tray and Main Window Functions"

void createTrayIcon()
{
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = trayID;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_TRAYMESSAGE;
	if(!getGesturesDisabled())
	{
		if(!getLearningMode()) {
			#ifdef SP_NOGLOBALACTIONS
				if(getOnlyDefinedApps()==true){
					nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEFINEDAPPS));
				} else {
					nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
				}
			#else
				nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
			#endif
			_tcscpy(nid.szTip, ptLang.get("language.Icon.NormalCaption",_T("StrokesPlus")).c_str());
		} else {
			nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAIN));
			_tcscpy(nid.szTip, ptLang.get("language.Icon.TrainingCaption",_T("StrokesPlus - Training Mode")).c_str());
		}		
	}
	else
	{
		nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DISABLED));
		_tcscpy(nid.szTip, ptLang.get("language.Icon.DisabledCaption",_T("StrokesPlus - Disabled")).c_str());
	}
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	Shell_NotifyIcon(NIM_ADD, &nid);
	setTrayIconVisible(TRUE);

}

static void CALLBACK DisplayChangeTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	KillTimer(NULL, dctimerid);
	SetWindowPos(hWnd,NULL,	GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN),
		SWP_NOACTIVATE);

	if(!bDCDisableState)
	{
		setMyHook(hWnd);
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(WM_RESETDISABLEICON, 1), 0);
	}
}

#pragma endregion "Tray and Main Window Functions"


#pragma region "Windows Message Functions Entry Points"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU hPopupMenu;

	if(message==(UINT)WM_TASKBARCREATED)  //If taskbar is being recreated (like explorer crashed..), recreate tray icon
	{
		if(getTrayIconVisible()){
			//Delete existing tray icon, only for weird scenarios like switching between emergeDesktop and Explorer, each instance would sometimes cause Explorer to remember and display the extra tray icons
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hWnd;
			nid.uID = trayID;
			Shell_NotifyIcon(NIM_DELETE, &nid);

			trayID++;
			setTrayID(trayID);
			createTrayIcon();
			setWindowState();
		}
		return(0);
	}

	switch (message)
	{
#ifdef SP_TOUCH
	case WM_POINTERDEVICECHANGE:
	case WM_POINTERDEVICEINRANGE:
	case WM_POINTERDEVICEOUTOFRANGE:
	case WM_NCPOINTERUPDATE:
	case WM_NCPOINTERDOWN:
	case WM_NCPOINTERUP:
	case WM_POINTERUPDATE:
	case WM_POINTERDOWN:
	case WM_POINTERUP:
	case WM_POINTERENTER:
	case WM_POINTERLEAVE:
	case WM_POINTERACTIVATE:
	case WM_POINTERCAPTURECHANGED:
	case WM_TOUCHHITTESTING:
	case WM_POINTERWHEEL:
	case WM_POINTERHWHEEL:
		{
			processInputMessage(message, wParam, lParam);
			break;
		}
#endif
	case WM_POWERBROADCAST:
		{
			LOGMESSAGE("Begin WM_POWERBROADCAST");
			if(getReInitOnResume() == TRUE){
				LOGMESSAGE("getReInitOnResume() == TRUE");
				if(wParam == PBT_APMSUSPEND)
				{
					LOGMESSAGE("PBT_APMSUSPEND");
					#ifdef SP_SYNAPTIC
						DestroySynaptics();
					#endif
					if(!getGesturesDisabled())
					{
						clearMyHook(hWnd);
					}
					if(getTrayIconVisible()){
						nid.cbSize = sizeof(NOTIFYICONDATA);
						nid.hWnd = hWnd;
						nid.uID = trayID;
						Shell_NotifyIcon(NIM_DELETE, &nid);
					}
					return TRUE;
				} else if (wParam == PBT_APMRESUMEAUTOMATIC)
				{
					LOGMESSAGE("PBT_APMRESUMEAUTOMATIC");
					Sleep(getResumeDelay()); 
					loadHookConfig(); 
					reloadLuaState(); 
					if(getTrayIconVisible()){
						trayID++;
						setTrayID(trayID);
						createTrayIcon();
					}
					if(!getGesturesDisabled())
					{
							setMyHook(hWnd); 
							clearCaptureVars();
							ShowWindow(hWnd,SW_SHOW);
					} else {
						nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DISABLED));
						_tcscpy(nid.szTip, ptLang.get("language.Icon.DisabledCaption",_T("StrokesPlus - Disabled")).c_str());
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
					return TRUE;
				}
			}
			LOGMESSAGE("End WM_POWERBROADCAST");
			break;
		}
	case WM_DISPLAYCHANGE: 
		{
			bDCDisableState = getGesturesDisabled();
			if(!getGesturesDisabled())
			{
				PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(WM_SHOWDISABLEICON, 1), 0);
				clearMyHook(hWnd);
			}
			KillTimer(NULL, dctimerid);
			dctimerid = SetTimer(NULL, IDT_DCMSG, 2000, (TIMERPROC) DisplayChangeTimerProc);
			break;
		}
	case WM_HOTKEY:
		FireHotkey(LOWORD(wParam));
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
			case WM_DISABLEHOTKEY:
				disableHotkey((int)LOWORD(lParam));
				break;
			case WM_ENABLEHOTKEY:
				enableHotkey((int)LOWORD(lParam));
				break;
			case WM_SHOWDISABLEICON:
				if(getTrayIconVisible()){
					nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DISABLED));
					_tcscpy(nid.szTip, ptLang.get("language.Icon.IgnoringGestures",_T("StrokesPlus - Ignoring Gestures")).c_str());
					Shell_NotifyIcon(NIM_MODIFY, &nid);
				}
				break;
			case WM_RESETDISABLEICON:
				if(getTrayIconVisible()){
					if(!getLearningMode()) {
						#ifdef SP_NOGLOBALACTIONS
							if(getOnlyDefinedApps()==true){
								nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEFINEDAPPS));
							} else {
								nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
							}
						#else
							nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
						#endif
						_tcscpy(nid.szTip, ptLang.get("language.Icon.NormalCaption",_T("StrokesPlus")).c_str());
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					} else {
						nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAIN));
						_tcscpy(nid.szTip, ptLang.get("language.Icon.TrainingCaption",_T("StrokesPlus - Training Mode")).c_str());
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
				}
				break;
			case WM_LOADHOTKEYS:
				LoadHotkeys();
				break;
			case WM_TOGGLETRAYICON:
				EnterCriticalSection(&CriticalSection); 
				if(!bInToggleTrayIcon){
					bInToggleTrayIcon = true;
					if(getTrayIconVisible()){
						setTrayIconVisible(FALSE);
						nid.cbSize = sizeof(NOTIFYICONDATA);
						nid.hWnd = hWnd;
						nid.uID = trayID;
						Shell_NotifyIcon(NIM_DELETE, &nid);
					} else {
						trayID++;
						setTrayID(trayID);
						createTrayIcon();
					}
					bInToggleTrayIcon = false;
				}
				LeaveCriticalSection(&CriticalSection);
				break;
			case WM_SHOWGESTURENAMEDLG:
				 openGestureName();
				 break;
			case ID_ICONPOPUP_EXIT_NOSAVE:
				 clearMyHook(hWnd);
				 if(getTrayIconVisible()){
					 nid.cbSize = sizeof(NOTIFYICONDATA);
					 nid.hWnd = hWnd;
					 nid.uID = trayID;
					 Shell_NotifyIcon(NIM_DELETE, &nid);
				 }
				 PostQuitMessage(0);
				 break;
			case ID_ICONPOPUP_EXIT:
				 saveConfig();
				 clearMyHook(hWnd);
				 if(getTrayIconVisible()){
					 nid.cbSize = sizeof(NOTIFYICONDATA);
					 nid.hWnd = hWnd;
					 nid.uID = trayID;
					 Shell_NotifyIcon(NIM_DELETE, &nid);
				 }
				 PostQuitMessage(0);
				 break;
			case ID_ICONPOPUP_DISABLED:
				switch(getGesturesDisabled())
				{
				case true:
					setGesturesDisabled(false);
					setMyHook(hWnd);
					clearCaptureVars();
					if(getTrayIconVisible()){
						if(!getLearningMode()) {
							#ifdef SP_NOGLOBALACTIONS
								if(getOnlyDefinedApps()==true){
									nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEFINEDAPPS));
								} else {
									nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
								}
							#else
								nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
							#endif
							_tcscpy(nid.szTip, ptLang.get("language.Icon.NormalCaption",_T("StrokesPlus")).c_str());
							Shell_NotifyIcon(NIM_MODIFY, &nid);
						} else {
							nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAIN));
							_tcscpy(nid.szTip, ptLang.get("language.Icon.TrainingCaption",_T("StrokesPlus - Training Mode")).c_str());
							Shell_NotifyIcon(NIM_MODIFY, &nid);
						}
					}
					Shell_NotifyIcon(NIM_MODIFY, &nid);
					ShowWindow(hWnd,SW_SHOW);
					break;
				case false:
					setGesturesDisabled(true);
					clearMyHook(hWnd);
					if(getTrayIconVisible()){
						nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DISABLED));
						_tcscpy(nid.szTip, ptLang.get("language.Icon.DisabledCaption",_T("StrokesPlus - Disabled")).c_str());
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
					ShowWindow(hWnd,SW_HIDE);
					break;
				}
				break;
			case ID_ICONPOPUP_MIDDLEBUTTON:
				setHookBtn(1);
				saveConfig();
				break;
			case ID_ICONPOPUP_RIGHTBUTTON:
				setHookBtn(0);
				saveConfig();
				break;
			case ID_ICONPOPUP_LEFTBUTTON:
				setHookBtn(2);
				saveConfig();
				break;
			case ID_ICONPOPUP_X1BUTTON:
				setHookBtn(3);
				saveConfig();
				break;
			case ID_ICONPOPUP_X2BUTTON:
				setHookBtn(4);
				saveConfig();
				break;
			case ID_ICONPOPUP_CTRLBUTTON:
				setHookBtn(-1);
				saveConfig();
				break;
			case ID_ICONPOPUP_ALTBUTTON:
				setHookBtn(-2);
				saveConfig();
				break;
			case ID_ICONPOPUP_SHIFTBUTTON:
				setHookBtn(-3); 
				saveConfig();
				break;
			case ID_ICONPOPUP_HELP:
				openHelp(); 
				break;
			case ID_ICONPOPUP_LOADCONFIG:
				loadHookConfig(); 
				break;
			case ID_ICONPOPUP_RELOAD_DELAY:
				Sleep(500); 
			case ID_ICONPOPUP_RELOAD:
				AllowSetForegroundWindow(ASFW_ANY);
				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = hWnd;
				nid.uID = trayID;
				Shell_NotifyIcon(NIM_DELETE, &nid);
				if(!getGesturesDisabled()) {
					clearMyHook(hWnd);
				}
				loadHookConfig();
				if(!getGesturesDisabled()) {
					setMyHook(hWnd);
				}
				if(getTrayIconVisible()){
					trayID++;
					setTrayID(trayID);
					createTrayIcon();
				}
				reloadLuaState();
				break;
			case ID_ICONPOPUP_SETTINGS:
				openSettings();
				break;
			case ID_ICONPOPUP_PREF:
				openPrefs();
				break;
			case ID_ICONPOPUP_HOTKEYS:
				openHotkeys();
				break;
			case ID_ICONPOPUP_CONFIG:
				openConfig();
				break;
			case ID_ICONPOPUP_PASSWORD:
				openPassword();
				break;
			case ID_ICONPOPUP_ABOUT:
				openAbout();
				break;
			case ID_ICONPOPUP_IGNORED:
				openIgnored();
				break;
			case ID_ICONPOPUP_DRAW:
				setDrawGesture(!getDrawGesture());
				setWindowState();
				saveConfig();
				break;
			case ID_ICONPOPUP_LEARN:
				if(getGesturesDisabled()) {
					break;
				}
				if(!getLearningMode()) {
					setDrawGesture(true);
					nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAIN));
					_tcscpy(nid.szTip, ptLang.get("language.Icon.TrainingCaption",_T("StrokesPlus - Training Mode")).c_str());
					Shell_NotifyIcon(NIM_MODIFY, &nid);
				} else {
					#ifdef SP_NOGLOBALACTIONS
						if(getOnlyDefinedApps()==true){
							nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEFINEDAPPS));
						} else {
							nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
						}
					#else
						nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_STROKESPLUS));
					#endif
					_tcscpy(nid.szTip, ptLang.get("language.Icon.NormalCaption",_T("StrokesPlus")).c_str());
					Shell_NotifyIcon(NIM_MODIFY, &nid);
				}

				setLearningMode(!getLearningMode());
				setPreviousTrainingModeState(getLearningMode());
				setWindowState();
				saveConfig();
				break;
			case ID_ICONPOPUP_CANCEL:
				setLastActive();
				break;
			case WM_MENUPOPUP:
				popupMenu();
				break;
		}
		break;

    case WM_TRAYMESSAGE:
         switch(lParam)
         {
		 case WM_MBUTTONUP:
				PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_LEARN, 1), 0);
				break;
		 case WM_MBUTTONDBLCLK:
				openPrefs();
				break;
		 case WM_LBUTTONUP: 
				PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_ICONPOPUP_DISABLED, 1), 0);
				 break;
		 case WM_LBUTTONDBLCLK:
				openConfig();
				break;
		 case WM_RBUTTONUP:
				POINT p;
				GetCursorPos(&p);
				hPopupMenu = CreatePopupMenu();
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_EXIT, ptLang.get("language.Menu.Exit",_T("Exit")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_HELP, ptLang.get("language.Menu.Help",_T("Help")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_ABOUT, ptLang.get("language.Menu.About",_T("About")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_PASSWORD, ptLang.get("language.Menu.Password",_T("Password...")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_RELOAD, ptLang.get("language.Menu.Reload",_T("Reload Config and Lua Engine")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_PREF, ptLang.get("language.Menu.Preferences",_T("Preferences...")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_IGNORED, ptLang.get("language.Menu.Ignored",_T("Ignore List...")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_HOTKEYS, ptLang.get("language.Menu.Hotkeys",_T("Hotkeys...")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_CONFIG, ptLang.get("language.Menu.Actions",_T("Actions...")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getGesturesDisabled() == TRUE ? MF_CHECKED : MF_UNCHECKED), ID_ICONPOPUP_DISABLED, ptLang.get("language.Menu.Disable",_T("Disable StrokesPlus")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getDrawGesture()==TRUE ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_DRAW, ptLang.get("language.Menu.DrawGestures",_T("Draw Gestures")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				if(getHookBtn() == -4){
					InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | MF_CHECKED | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_SHIFTBUTTON, ptLang.get("language.Menu.Spacebar",_T("Use Spacebar")).c_str());
				}
				if(getHookBtn() == -3){
					InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | MF_CHECKED | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_SHIFTBUTTON, ptLang.get("language.Menu.Shift",_T("Use Shift Button")).c_str());
				}
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == -2 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_ALTBUTTON, ptLang.get("language.Menu.Alt",_T("Use Alt Button")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == -1 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_CTRLBUTTON, ptLang.get("language.Menu.Control",_T("Use Control Button")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == 4 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_X2BUTTON, ptLang.get("language.Menu.X2",_T("Use X2 Button")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == 3 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_X1BUTTON, ptLang.get("language.Menu.X1",_T("Use X1 Button")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == 2 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_LEFTBUTTON, (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? ptLang.get("language.Menu.Left",_T("Use Left Button")).c_str() : ptLang.get("language.Menu.Right",_T("Use Right Button")).c_str()));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == 1 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_MIDDLEBUTTON, ptLang.get("language.Menu.Middle",_T("Use Middle Button")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | (getHookBtn() == 0 ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_RIGHTBUTTON, (GetSystemMetrics(SM_SWAPBUTTON) == 0 ? ptLang.get("language.Menu.Right",_T("Use Right Button")).c_str() : ptLang.get("language.Menu.Left",_T("Use Left Button")).c_str()));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING |(getLearningMode() == TRUE ? MF_CHECKED : MF_UNCHECKED) | (getGesturesDisabled() == TRUE ? MF_DISABLED : 0), ID_ICONPOPUP_LEARN, ptLang.get("language.Menu.TrainingMode",_T("Training Mode")).c_str());
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, _T(""));
				InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_ICONPOPUP_CANCEL, ptLang.get("language.Menu.CloseMenu",_T("Close Menu")).c_str());
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hPopupMenu, TPM_RIGHTBUTTON | TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hWnd, NULL);
				PostMessage(hWnd, WM_NULL, 0, 0);
				break;
         default:
                return DefWindowProc(hWnd, message, wParam, lParam);
         };
         break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		#ifdef SP_TOUCH
			if((isTouchEnabled() || isPenEnabled()) && isTouchAvailable()){
				DestroyTouchWindow(hWnd);
			}
		#endif
		GdiplusShutdown(m_gdiplusToken);
		clearMyHook(hWnd);
		if(getTrayIconVisible()){
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hWnd;
			nid.uID = trayID;
			Shell_NotifyIcon(NIM_DELETE, &nid);
		}
		PostQuitMessage(0);
		break;
	case WM_COPYDATA:
		{
			if(getShowCopyData())
			{
				//Basically added this for testing purposes when using the acSendString action
				COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
				std::string sData;
				sData.assign((char *)pcds->lpData,(int)pcds->cbData);
			
  				TCHAR buffer[256];
				wsprintf(buffer, _T("WM_COPYDATA:\n\n")
				   _T("dwData: %d\n")
				   _T("lpData: %s\n")
				   _T("lpData Size: %d\n"),
				   pcds->dwData,sData.c_str(), sData.size());
				MessageBox(NULL,buffer,_T("WM_COPYDATA - String Only"),MB_OK);
			}
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion "Windows Message Functions Entry Points"