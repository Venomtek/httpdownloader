/*
	HTTP Downloader can download files through HTTP(S), FTP(S), and SFTP connections.
	Copyright (C) 2015-2025 Eric Kutcher

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lite_dlls.h"
#include "lite_user32.h"

#ifndef USER32_USE_STATIC_LIB

	//pAdjustWindowRectEx		_AdjustWindowRectEx;
	pBeginDeferWindowPos	_BeginDeferWindowPos;
	pBeginPaint				_BeginPaint;
	pCallWindowProcW		_CallWindowProcW;
	//pCharLowerA				_CharLowerA;
	//pCharLowerBuffA			_CharLowerBuffA;
	//pCharUpperA			_CharUpperA;
	pCheckMenuItem			_CheckMenuItem;
	pClientToScreen			_ClientToScreen;
	pCloseClipboard			_CloseClipboard;
	pCreateMenu				_CreateMenu;
	pCreatePopupMenu		_CreatePopupMenu;
	pCreateWindowExW		_CreateWindowExW;
	pDefWindowProcW			_DefWindowProcW;
	pDeferWindowPos			_DeferWindowPos;
	//pDeleteMenu				_DeleteMenu;
	pDestroyIcon			_DestroyIcon;
	pDestroyMenu			_DestroyMenu;
	pDestroyWindow			_DestroyWindow;
	pDispatchMessageW		_DispatchMessageW;
	//pDrawEdge				_DrawEdge;
	pDrawFocusRect			_DrawFocusRect;
	pDrawIconEx				_DrawIconEx;
	//pDrawMenuBar				_DrawMenuBar;
	pDrawTextW				_DrawTextW;
	pEmptyClipboard			_EmptyClipboard;
	pEnableMenuItem			_EnableMenuItem;
	pEnableWindow			_EnableWindow;
	pEndDeferWindowPos		_EndDeferWindowPos;
	//pEndMenu				_EndMenu;
	pEndPaint				_EndPaint;
	//pEnumChildWindows		_EnumChildWindows;
	pExitWindowsEx			_ExitWindowsEx;
	pFillRect				_FillRect;
	pFindWindowW			_FindWindowW;
	pFlashWindow			_FlashWindow;
	pFrameRect				_FrameRect;
	//pGetClassLongPtrW		_GetClassLongPtrW;
	pGetClientRect			_GetClientRect;
	pGetClipboardData		_GetClipboardData;
	pGetCursorPos			_GetCursorPos;
	pGetDC					_GetDC;
	pGetDlgCtrlID			_GetDlgCtrlID;
	//pGetDlgItem				_GetDlgItem;
	pGetFocus				_GetFocus;
	pGetIconInfo			_GetIconInfo;
	pGetKeyState			_GetKeyState;
	//pGetMenuItemInfoW		_GetMenuItemInfoW;
	//pGetMessagePos			_GetMessagePos;
	pGetMessageW			_GetMessageW;
	pGetMonitorInfoW		_GetMonitorInfoW;
	pGetParent				_GetParent;
	pGetScrollInfo			_GetScrollInfo;
	//pGetSubMenu				_GetSubMenu;
	pGetSysColor			_GetSysColor;
	pGetSysColorBrush		_GetSysColorBrush;
	pGetSystemMetrics		_GetSystemMetrics;
	pGetWindow				_GetWindow;
	pGetWindowDC			_GetWindowDC;
	//pGetWindowLongW			_GetWindowLongW;
	pGetWindowLongPtrW		_GetWindowLongPtrW;
	pGetWindowRect			_GetWindowRect;
	pInsertMenuItemW		_InsertMenuItemW;
	pInvalidateRect			_InvalidateRect;
	//pIsCharAlphaNumericA		_IsCharAlphaNumericA;
	pIsClipboardFormatAvailable _IsClipboardFormatAvailable;
	pIsDialogMessageW		_IsDialogMessageW;
	pIsIconic				_IsIconic;
	pIsWindowEnabled		_IsWindowEnabled;
	pIsWindowVisible		_IsWindowVisible;
	pIsZoomed				_IsZoomed;
	pKillTimer				_KillTimer;
	pLoadCursorW				_LoadCursorW;
	pLoadIconW				_LoadIconW;
	pLoadImageW				_LoadImageW;
	pLockWorkStation		_LockWorkStation;
	pMapWindowPoints		_MapWindowPoints;
	//pMessageBoxA			_MessageBoxA;
	pMessageBoxW			_MessageBoxW;
	pMonitorFromRect		_MonitorFromRect;
	pMonitorFromWindow		_MonitorFromWindow;
	pOffsetRect				_OffsetRect;
	pOpenClipboard			_OpenClipboard;
	pPostMessageW			_PostMessageW;
	pPostQuitMessage		_PostQuitMessage;
	pRegisterClassExW		_RegisterClassExW;
	pRegisterClipboardFormatW	_RegisterClipboardFormatW;
	pRegisterWindowMessageW	_RegisterWindowMessageW;
	pReleaseCapture			_ReleaseCapture;
	pReleaseDC				_ReleaseDC;
	//pRemoveMenu				_RemoveMenu;
	pScreenToClient			_ScreenToClient;
	pScrollWindow			_ScrollWindow;
	pSendMessageA			_SendMessageA;
	pSendMessageW			_SendMessageW;
	pSendNotifyMessageW		_SendNotifyMessageW;
	pSetCapture				_SetCapture;
	//pSetClassLongPtrW		_SetClassLongPtrW;
	pSetClipboardData		_SetClipboardData;
	pSetCursor				_SetCursor;
	pSetFocus				_SetFocus;
	pSetForegroundWindow	_SetForegroundWindow;
	pSetLayeredWindowAttributes		_SetLayeredWindowAttributes;
	pSetMenu					_SetMenu;
	pSetMenuItemInfoW		_SetMenuItemInfoW;
	//pSetRect					_SetRect;
	pSetScrollInfo			_SetScrollInfo;
	pSetScrollPos			_SetScrollPos;
	pSetTimer				_SetTimer;
	//pSetWindowLongW			_SetWindowLongW;
	pSetWindowLongPtrW		_SetWindowLongPtrW;
	pSetWindowPos			_SetWindowPos;
	//pSetWindowTextW			_SetWindowTextW;
	pShowWindow				_ShowWindow;
	pSystemParametersInfoW	_SystemParametersInfoW;
	pTrackMouseEvent		_TrackMouseEvent;
	pTrackPopupMenu			_TrackPopupMenu;
	pTranslateMessage		_TranslateMessage;

	HMODULE hModule_user32 = NULL;

	unsigned char user32_state = 0;	// 0 = Not running, 1 = running.

	bool InitializeUser32()
	{
		if ( user32_state != USER32_STATE_SHUTDOWN )
		{
			return true;
		}

		hModule_user32 = LoadLibraryDEMW( L"user32.dll" );

		if ( hModule_user32 == NULL )
		{
			return false;
		}

		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_AdjustWindowRectEx, "AdjustWindowRectEx" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_BeginDeferWindowPos, "BeginDeferWindowPos" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_BeginPaint, "BeginPaint" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CallWindowProcW, "CallWindowProcW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CharLowerA, "CharLowerA" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CharLowerBuffA, "CharLowerBuffA" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CharUpperA, "CharUpperA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CheckMenuItem, "CheckMenuItem" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ClientToScreen, "ClientToScreen" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CloseClipboard, "CloseClipboard" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CreateMenu, "CreateMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CreatePopupMenu, "CreatePopupMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_CreateWindowExW, "CreateWindowExW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DefWindowProcW, "DefWindowProcW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DeferWindowPos, "DeferWindowPos" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DeleteMenu, "DeleteMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DestroyIcon, "DestroyIcon" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DestroyMenu, "DestroyMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DestroyWindow, "DestroyWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DispatchMessageW, "DispatchMessageW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DrawEdge, "DrawEdge" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DrawFocusRect, "DrawFocusRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DrawIconEx, "DrawIconEx" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DrawMenuBar, "DrawMenuBar" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_DrawTextW, "DrawTextW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EmptyClipboard, "EmptyClipboard" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EnableMenuItem, "EnableMenuItem" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EnableWindow, "EnableWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EndDeferWindowPos, "EndDeferWindowPos" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EndMenu, "EndMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EndPaint, "EndPaint" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_EnumChildWindows, "EnumChildWindows" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ExitWindowsEx, "ExitWindowsEx" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_FillRect, "FillRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_FindWindowW, "FindWindowW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_FlashWindow, "FlashWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_FrameRect, "FrameRect" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetClassLongPtrW, "GetClassLongPtrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetClientRect, "GetClientRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetClipboardData, "GetClipboardData" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetCursorPos, "GetCursorPos" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetDC, "GetDC" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetDlgCtrlID, "GetDlgCtrlID" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetDlgItem, "GetDlgItem" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetFocus, "GetFocus" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetIconInfo, "GetIconInfo" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetKeyState, "GetKeyState" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetMenuItemInfoW, "GetMenuItemInfoW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetMessagePos, "GetMessagePos" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetMessageW, "GetMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetMonitorInfoW, "GetMonitorInfoW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetParent, "GetParent" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetScrollInfo, "GetScrollInfo" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetSubMenu, "GetSubMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetSysColor, "GetSysColor" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetSysColorBrush, "GetSysColorBrush" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetSystemMetrics, "GetSystemMetrics" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetWindow, "GetWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetWindowDC, "GetWindowDC" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetWindowLongW, "GetWindowLongW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetWindowLongPtrW, "GetWindowLongPtrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_GetWindowRect, "GetWindowRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_InsertMenuItemW, "InsertMenuItemW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_InvalidateRect, "InvalidateRect" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsCharAlphaNumericA, "IsCharAlphaNumericA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsClipboardFormatAvailable, "IsClipboardFormatAvailable" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsDialogMessageW, "IsDialogMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsIconic, "IsIconic" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsWindowEnabled, "IsWindowEnabled" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsWindowVisible, "IsWindowVisible" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_IsZoomed, "IsZoomed" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_KillTimer, "KillTimer" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_LoadCursorW, "LoadCursorW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_LoadIconW, "LoadIconW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_LoadImageW, "LoadImageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_LockWorkStation, "LockWorkStation" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_MapWindowPoints, "MapWindowPoints" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_MessageBoxA, "MessageBoxA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_MessageBoxW, "MessageBoxW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_MonitorFromRect, "MonitorFromRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_MonitorFromWindow, "MonitorFromWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_OffsetRect, "OffsetRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_OpenClipboard, "OpenClipboard" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_PostMessageW, "PostMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_PostQuitMessage, "PostQuitMessage" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RegisterClassExW, "RegisterClassExW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RegisterClipboardFormatW, "RegisterClipboardFormatW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RegisterWindowMessageW, "RegisterWindowMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ReleaseCapture, "ReleaseCapture" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ReleaseDC, "ReleaseDC" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_RemoveMenu, "RemoveMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ScreenToClient, "ScreenToClient" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ScrollWindow, "ScrollWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SendMessageA, "SendMessageA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SendMessageW, "SendMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SendNotifyMessageW, "SendNotifyMessageW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetCapture, "SetCapture" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetClassLongPtrW, "SetClassLongPtrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetClipboardData, "SetClipboardData" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetCursor, "SetCursor" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetFocus, "SetFocus" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetForegroundWindow, "SetForegroundWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetLayeredWindowAttributes, "SetLayeredWindowAttributes" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetMenu, "SetMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetMenuItemInfoW, "SetMenuItemInfoW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetRect, "SetRect" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetScrollInfo, "SetScrollInfo" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetScrollPos, "SetScrollPos" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetTimer, "SetTimer" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetWindowLongW, "SetWindowLongW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetWindowLongPtrW, "SetWindowLongPtrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetWindowPos, "SetWindowPos" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SetWindowTextW, "SetWindowTextW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_ShowWindow, "ShowWindow" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_SystemParametersInfoW, "SystemParametersInfoW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_TrackMouseEvent, "TrackMouseEvent" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_TrackPopupMenu, "TrackPopupMenu" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_user32, ( void ** )&_TranslateMessage, "TranslateMessage" ) )

		user32_state = USER32_STATE_RUNNING;

		return true;
	}

	bool UnInitializeUser32()
	{
		if ( user32_state != USER32_STATE_SHUTDOWN )
		{
			user32_state = USER32_STATE_SHUTDOWN;

			return ( FreeLibrary( hModule_user32 ) == FALSE ? false : true );
		}

		return true;
	}

#endif

#include "lite_gdi32.h"

pGetDpiForWindow			_GetDpiForWindow;
pSystemParametersInfoForDpi	_SystemParametersInfoForDpi;
pGetSystemMetricsForDpi		_GetSystemMetricsForDpi;
//pAdjustWindowRectExForDpi	_AdjustWindowRectExForDpi;

pGetDpiForMonitor			_GetDpiForMonitor;

HMODULE hModule_dpi = NULL;

bool InitializeDPIFunctions()
{
	hModule_dpi = LoadLibraryDEMW( L"user32.dll" );
	if ( SetFunctionPointer( hModule_dpi, ( void ** )&_GetDpiForWindow, "GetDpiForWindow" ) != NULL &&
		 SetFunctionPointer( hModule_dpi, ( void ** )&_SystemParametersInfoForDpi, "SystemParametersInfoForDpi" ) != NULL &&
		 SetFunctionPointer( hModule_dpi, ( void ** )&_GetSystemMetricsForDpi, "GetSystemMetricsForDpi" ) != NULL /*&&
		 SetFunctionPointer( hModule_dpi, ( void ** )&_AdjustWindowRectExForDpi, "AdjustWindowRectExForDpi" ) != NULL*/ )
	{
		return true;
	}
	else
	{
		hModule_dpi = LoadLibraryDEMW( L"Shcore.dll" );
		if ( hModule_dpi != NULL && SetFunctionPointer( hModule_dpi, ( void ** )&_GetDpiForMonitor, "GetDpiForMonitor" ) != NULL )
		{
			return true;
		}
	}

	return false;
}

bool UnInitializeDPIFunctions()
{
	return ( hModule_dpi != NULL && FreeLibrary( hModule_dpi ) == FALSE ? false : true );
}

UINT gdfw_dpi = 0;

UINT __GetDpiForWindow( HWND hWnd )
{
	if ( _GetDpiForWindow )	// Per-monitor DPI support.
	{
		UINT dpi = _GetDpiForWindow( hWnd );
		if ( dpi != 0 )
		{
			return dpi;
		}
	}
	else if ( _GetDpiForMonitor )	// If we're not on Windows 10+, then this is available on Windows 8.1.
	{
		UINT x = 0;
		UINT y;

		HMONITOR hMon = _MonitorFromWindow( hWnd, MONITOR_DEFAULTTONEAREST );
		if ( _GetDpiForMonitor( hMon, MDT_EFFECTIVE_DPI, &x, &y ) == S_OK && x != 0 )
		{
			return x;
		}
	}

	// No per-monitor support. Program won't scale when moving across monitors that have differnt DPI values.
	if ( gdfw_dpi == 0 )
	{
		HDC hDC = _GetDC( NULL );
		gdfw_dpi = _GetDeviceCaps( hDC, LOGPIXELSX );
		_ReleaseDC( NULL, hDC );

		if ( gdfw_dpi == 0 )
		{
			gdfw_dpi = USER_DEFAULT_SCREEN_DPI;
		}
	}

	return gdfw_dpi;
}

BOOL __SystemParametersInfoForDpi( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi )
{
	if ( _SystemParametersInfoForDpi )
	{
		return _SystemParametersInfoForDpi( uiAction, uiParam, pvParam, fWinIni, dpi );
	}
	else
	{
		return _SystemParametersInfoW( uiAction, uiParam, pvParam, fWinIni );
	}
}

int __GetSystemMetricsForDpi( int nIndex, UINT dpi )
{
	if ( _GetSystemMetricsForDpi )
	{
		return _GetSystemMetricsForDpi( nIndex, dpi );
	}
	else
	{
		return _GetSystemMetrics( nIndex );
	}
}

/*BOOL __AdjustWindowRectExForDpi( LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi )
{
	if ( _AdjustWindowRectExForDpi )
	{
		return _AdjustWindowRectExForDpi( lpRect, dwStyle, bMenu, dwExStyle, dpi );
	}
	else
	{
		return _AdjustWindowRectEx( lpRect, dwStyle, bMenu, dwExStyle );
	}
}*/
