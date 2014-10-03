

#pragma once

#include "Resource.h"
#include "..\StrokesPlusHook\StrokesPlusHook.h"

const int WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
BOOL RunningAsAdministrator ( VOID);
void createTrayIcon();


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
