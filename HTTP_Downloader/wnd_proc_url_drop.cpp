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

#include "globals.h"
#include "lite_gdi32.h"
#include "lite_ole32.h"
#include "drag_and_drop.h"
#include "menus.h"
#include "resource.h"
#include "drop_window.h"

HWND g_hWnd_url_drop_window = NULL;

struct WINDOW_SETTINGS
{
	TRACKMOUSEEVENT tme;
	POINT window_position;
	POINT drag_position;
	bool is_dragging;
	bool is_tracking;
};

HBITMAP hbm_background = NULL;

WINDOW_SETTINGS window_settings;

bool window_on_top = true;

bool use_drag_and_drop_url = true;	// Assumes OLE32_STATE_RUNNING is true.
IDropTarget *URL_DropTarget;

int last_drop_percent = 0;
COLORREF last_drop_border_color = 0;
COLORREF last_drop_progress_color = 0;

bool show_drop_progress = false;

LONG g_dw_width = DW_WIDTH;
LONG g_dw_height = DW_HEIGHT;

UINT current_dpi_url_drop = USER_DEFAULT_SCREEN_DPI;
UINT last_dpi_url_drop = 0;

#define _SCALE_URLD_( x )						_SCALE_( ( x ), dpi_url_drop )

HBITMAP UpdateDropIcon( HWND hWnd )
{
	HBITMAP hBmp;

	_wmemcpy_s( g_program_directory + g_program_directory_length, MAX_PATH - g_program_directory_length, L"\\drop.bmp\0", 10 );
	if ( GetFileAttributesW( g_program_directory ) != INVALID_FILE_ATTRIBUTES )
	{
		hBmp = ( HBITMAP )_LoadImageW( NULL, g_program_directory, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	}
	else
	{
		hBmp = ( HBITMAP )_LoadImageW( GetModuleHandleW( NULL ), MAKEINTRESOURCE( IDB_BITMAP_DROP ), IMAGE_BITMAP, 0, 0, 0 );
	}

	HDC hDC = _GetDC( hWnd );

	HDC hdcMem_bmp = _CreateCompatibleDC( hDC );
	HBITMAP ohbm = ( HBITMAP )_SelectObject( hdcMem_bmp, hBmp );
	_DeleteObject( ohbm );

	BITMAP bmp;
	_memzero( &bmp, sizeof( BITMAP ) );
	_GetObjectW( hBmp, sizeof( BITMAP ), &bmp );

	g_dw_width = bmp.bmWidth;
	g_dw_height = bmp.bmHeight;

	int res_width = _SCALE_URLD_( g_dw_width );
	int res_height = _SCALE_URLD_( g_dw_height );

	HBITMAP hBmp_scaled = _CreateCompatibleBitmap( hDC, res_width, res_height );

	HDC hdcMem_scaled = _CreateCompatibleDC( hDC );
	ohbm = ( HBITMAP )_SelectObject( hdcMem_scaled, hBmp_scaled );
	_DeleteObject( ohbm );

	_SetStretchBltMode( hdcMem_scaled, COLORONCOLOR );
	_StretchBlt( hdcMem_scaled, 0, 0, res_width, res_height, hdcMem_bmp, 0, 0, g_dw_width, g_dw_height, SRCCOPY );

	_DeleteDC( hdcMem_scaled );
	_DeleteDC( hdcMem_bmp );
	_ReleaseDC( hWnd, hDC );

	_DeleteObject( hBmp );

	return hBmp_scaled;
}

void UpdateDropWindow( unsigned long long start, unsigned long long end, COLORREF border_color, COLORREF progress_color, bool show_progress )
{
	int i_percentage;

	show_drop_progress = show_progress;

	if ( end > 0 )
	{
	#ifdef _WIN64
		i_percentage = ( int )( 44.0 * ( ( double )start / ( double )end ) );
	#else
		double f_percentage = 44.0 * ( ( double )start / ( double )end );
		__asm
		{
			fld f_percentage;	//; Load the floating point value onto the FPU stack.
			fistp i_percentage;	//; Convert the floating point value into an integer, store it in an integer, and then pop it off the stack.
		}
	#endif

		if ( i_percentage == last_drop_percent &&
			 border_color == last_drop_border_color &&
			 progress_color == last_drop_progress_color )
		{
			return;
		}

		last_drop_percent = i_percentage;
	}

	last_drop_border_color = border_color;
	last_drop_progress_color = progress_color;

	_InvalidateRect( g_hWnd_url_drop_window, NULL, TRUE );
}

LRESULT CALLBACK URLDropWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			current_dpi_url_drop = __GetDpiForWindow( hWnd );
			last_dpi_url_drop = 0;

			window_settings.window_position.x = 0;
			window_settings.window_position.y = 0;
			window_settings.drag_position.x = 0;
			window_settings.drag_position.y = 0;
			window_settings.is_dragging = false;

			window_settings.tme.cbSize = sizeof( TRACKMOUSEEVENT );
			window_settings.tme.dwFlags = TME_LEAVE;
			window_settings.tme.dwHoverTime = HOVER_DEFAULT;
			window_settings.tme.hwndTrack = hWnd;

			hbm_background = UpdateDropIcon( hWnd );

			#ifndef OLE32_USE_STATIC_LIB
				if ( ole32_state == OLE32_STATE_SHUTDOWN )
				{
					use_drag_and_drop_url = InitializeOle32();
				}
			#endif

			if ( use_drag_and_drop_url )
			{
				_OleInitialize( NULL );

				RegisterDropWindow( hWnd, &URL_DropTarget );
			}

			int width = _SCALE_URLD_( g_dw_width );
			int height = _SCALE_URLD_( g_dw_height );

			RECT rc_mon;
			rc_mon.left = cfg_drop_pos_x;
			rc_mon.top = cfg_drop_pos_y;
			rc_mon.right = rc_mon.left + width;
			rc_mon.bottom = rc_mon.top + height;
			HMONITOR hMon = MonitorFromRect( &rc_mon, MONITOR_DEFAULTTONEAREST );	// This is a popup window and we can't use CW_USEDEFAULT. We'll place this window on the same monitor as the main window.
			MONITORINFO mi;
			mi.cbSize = sizeof( MONITORINFO );
			_GetMonitorInfoW( hMon, &mi );
			int pos_x = cfg_drop_pos_x;
			int pos_y = cfg_drop_pos_y;
			// If the window is offscreen, then move it into the current monitor.
			if ( pos_x + width <= mi.rcMonitor.left ||
				 pos_x >= mi.rcMonitor.right ||
				 pos_y + height <= mi.rcMonitor.top ||
				 pos_y >= mi.rcMonitor.bottom )
			{
				pos_x = mi.rcMonitor.left + ( ( ( mi.rcMonitor.right - mi.rcMonitor.left ) - width ) / 2 );
				pos_y = mi.rcMonitor.top + ( ( ( mi.rcMonitor.bottom - mi.rcMonitor.top ) - height ) / 2 );
			}
			_SetWindowPos( hWnd, NULL, pos_x, pos_y, width, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER );

			return 0;
		}
		break;

		case WM_DPICHANGED:
		{
			UINT last_dpi = current_dpi_url_drop;
			current_dpi_url_drop = HIWORD( wParam );

			if ( hbm_background != NULL )
			{
				_DeleteObject( hbm_background );
			}
			hbm_background = UpdateDropIcon( hWnd );

			RECT *rc = ( RECT * )lParam;

			// Resize the window based on the resource width/height.
			// It's possible that it gets deleted before the update and so we'll want to adjust the width/height for the default resource.
			int width = _SCALE_URLD_( g_dw_width );
			int height = _SCALE_URLD_( g_dw_height );

			if ( last_dpi_url_drop == 0 )
			{
				RECT rc_mon;
				rc_mon.left = cfg_drop_pos_x;
				rc_mon.top = cfg_drop_pos_y;
				rc_mon.right = rc_mon.left + width;
				rc_mon.bottom = rc_mon.top + height;
				HMONITOR hMon = MonitorFromRect( &rc_mon, MONITOR_DEFAULTTONEAREST );	// This is a popup window and we can't use CW_USEDEFAULT. We'll place this window on the same monitor as the main window.
				MONITORINFO mi;
				mi.cbSize = sizeof( MONITORINFO );
				_GetMonitorInfoW( hMon, &mi );
				int pos_x = cfg_drop_pos_x;
				int pos_y = cfg_drop_pos_y;
				// If the window is offscreen, then move it into the current monitor.
				if ( pos_x + width <= mi.rcMonitor.left ||
					 pos_x >= mi.rcMonitor.right ||
					 pos_y + height <= mi.rcMonitor.top ||
					 pos_y >= mi.rcMonitor.bottom )
				{
					pos_x = mi.rcMonitor.left + ( ( ( mi.rcMonitor.right - mi.rcMonitor.left ) - width ) / 2 );
					pos_y = mi.rcMonitor.top + ( ( ( mi.rcMonitor.bottom - mi.rcMonitor.top ) - height ) / 2 );
				}
				_SetWindowPos( hWnd, NULL, pos_x, pos_y, width, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER );
			}
			else
			{
				_SetWindowPos( hWnd, NULL, rc->left, rc->top, width, height, SWP_NOZORDER | SWP_NOACTIVATE );
			}

			last_dpi_url_drop = last_dpi;

			_InvalidateRect( hWnd, NULL, TRUE );

			return 0;
		}
		break;

		case WM_COMMAND:
		{
			if ( LOWORD( wParam ) == MENU_ALWAYS_ON_TOP )
			{
				HWND pos;

				if ( _GetWindowLongPtrW( hWnd, GWL_EXSTYLE ) & WS_EX_TOPMOST )
				{
					_CheckMenuItem( g_hMenuSub_drag_drop, MENU_ALWAYS_ON_TOP, MF_UNCHECKED );

					pos = HWND_NOTOPMOST;
					window_on_top = false;
				}
				else
				{
					_CheckMenuItem( g_hMenuSub_drag_drop, MENU_ALWAYS_ON_TOP, MF_CHECKED );

					pos = HWND_TOPMOST;
					window_on_top = true;
				}

				_SetWindowPos( hWnd, pos, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER );

				_InvalidateRect( hWnd, NULL, TRUE );
			}
			else
			{
				// Pass the other commands to the main window.
				_SendMessageW( g_hWnd_main, msg, wParam, lParam );
			}

			return 0;
		}
		break;

		case WM_ERASEBKGND:
		{
			return TRUE;
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = _BeginPaint( hWnd, &ps );

			RECT rc;
			_GetClientRect( hWnd, &rc );

			// Create a memory buffer to draw to.
			HDC hdcMem = _CreateCompatibleDC( hDC );

			HBITMAP ohbm = ( HBITMAP )_SelectObject( hdcMem, hbm_background );
			_DeleteObject( ohbm );

			// Draw our memory buffer to the main device context.
			_BitBlt( hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY );

			if ( show_drop_progress )
			{
				RECT icon_rc;
				icon_rc.top = rc.bottom - _SCALE_URLD_( 6 );
				icon_rc.left = _SCALE_URLD_( 1 );
				icon_rc.right = rc.right - _SCALE_URLD_( 1 );
				icon_rc.bottom = rc.bottom - _SCALE_URLD_( 1 );

				HBRUSH color = _CreateSolidBrush( last_drop_border_color );
				_FillRect( hDC, &icon_rc, color );
				_DeleteObject( color );

				icon_rc.top = rc.bottom - _SCALE_URLD_( 5 );
				icon_rc.left = _SCALE_URLD_( 2 );
				icon_rc.right = _SCALE_URLD_( 2 + last_drop_percent );
				icon_rc.bottom = rc.bottom - _SCALE_URLD_( 2 );

				color = _CreateSolidBrush( last_drop_progress_color );
				_FillRect( hDC, &icon_rc, color );
				_DeleteObject( color );
			}

			if ( !window_on_top )
			{
				// Create a border.
				HBRUSH color = _CreateSolidBrush( ( COLORREF )RGB( 0x00, 0xFF, 0xFF ) );
				//_FrameRect( hDC, &rc, color );
				HRGN hRgn = _CreateRectRgn( rc.left, rc.top, rc.right, rc.bottom );
				_FrameRgn( hDC, hRgn, color, _SCALE_URLD_( 1 ), _SCALE_URLD_( 1 ) );
				_DeleteObject( hRgn );
				_DeleteObject( color );
			}

			_DeleteDC( hdcMem );
			_EndPaint( hWnd, &ps );

			return 0;
		}
		break;

		case WM_CAPTURECHANGED:
		{
			window_settings.is_dragging = ( ( HWND )lParam == hWnd ) ? true : false;

			return 0;
		}
		break;

		case WM_LBUTTONDOWN:
		{
			_SetCapture( hWnd );

			_GetCursorPos( &window_settings.drag_position );
			RECT rc;
			_GetWindowRect( hWnd, &rc );
			window_settings.window_position.x = rc.left;
			window_settings.window_position.y = rc.top;

			window_settings.is_dragging = true;

			_SetForegroundWindow( hWnd );

			return 0;
		}
		break;

		case WM_LBUTTONUP:
		{
			_ReleaseCapture();

			return 0;
		}
		break;

		case WM_MOUSEMOVE:
		{
			// See if we have the mouse captured.
			if ( window_settings.is_dragging )
			{
				POINT cur_pos;
				//RECT wa;
				RECT rc;
				_GetWindowRect( hWnd, &rc );

				_GetCursorPos( &cur_pos );
				_OffsetRect( &rc, cur_pos.x - ( rc.left + window_settings.drag_position.x - window_settings.window_position.x ), cur_pos.y - ( rc.top + window_settings.drag_position.y - window_settings.window_position.y ) );

				// Allow our main window to attach to the desktop edge.
				//_SystemParametersInfoW( SPI_GETWORKAREA, 0, &wa, 0 );
				HMONITOR hMon = _MonitorFromWindow( hWnd, MONITOR_DEFAULTTONEAREST );
				MONITORINFO mi;
				mi.cbSize = sizeof( MONITORINFO );
				_GetMonitorInfoW( hMon, &mi );
				if ( is_close( rc.left, mi.rcWork.left ) )				// Attach to left side of the desktop.
				{
					_OffsetRect( &rc, mi.rcWork.left - rc.left, 0 );
				}
				else if ( is_close( mi.rcWork.right, rc.right ) )		// Attach to right side of the desktop.
				{
					_OffsetRect( &rc, mi.rcWork.right - rc.right, 0 );
				}

				if ( is_close( rc.top, mi.rcWork.top ) )				// Attach to top of the desktop.
				{
					_OffsetRect( &rc, 0, mi.rcWork.top - rc.top );
				}
				else if ( is_close( mi.rcWork.bottom, rc.bottom ) )		// Attach to bottom of the desktop.
				{
					_OffsetRect( &rc, 0, mi.rcWork.bottom - rc.bottom );
				}

				_SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE );

				// Save our settings for the position/dimensions of the window.
				cfg_drop_pos_x = rc.left;
				cfg_drop_pos_y = rc.top;
			}

			if ( !window_settings.is_tracking )
			{
				window_settings.is_tracking = true;

				_TrackMouseEvent( &window_settings.tme );

				_SetLayeredWindowAttributes( hWnd, 0, 0xFF, LWA_ALPHA );
			}

			return 0;
		}
		break;

		case WM_WINDOWPOSCHANGED:
		{
			// This will capture MoveWindow and SetWindowPos changes.
			WINDOWPOS *wp = ( WINDOWPOS * )lParam;

			if ( !( wp->flags & SWP_NOMOVE ) )
			{
				cfg_drop_pos_x = wp->x;
				cfg_drop_pos_y = wp->y;
			}

			// Let it fall through so we can still get the WM_SIZE message.
			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;

		case WM_MOUSELEAVE:
		{
			if ( window_settings.is_tracking )
			{
				window_settings.is_tracking = false;

				_SetLayeredWindowAttributes( hWnd, 0, cfg_drop_window_transparency, LWA_ALPHA );
			}

			return 0;
		}
		break;

		case WM_NCHITTEST:
		{
			// Allow us to work in the client area, but nothing else (resize, title bar, etc.).
			return ( _DefWindowProcW( hWnd, msg, wParam, lParam ) == HTCLIENT ? HTCLIENT : NULL );
		}
		break;

		case WM_RBUTTONUP:
		{
			// Show our drag and drop context menu as a popup.
			POINT p;
			_GetCursorPos( &p );
			_TrackPopupMenu( g_hMenuSub_drag_drop, 0, p.x, p.y, 0, hWnd, NULL );

			return 0;
		}
		break;

		case WM_LBUTTONDBLCLK:
		{
			_SendMessageW( g_hWnd_main, WM_COMMAND, MENU_ADD_URLS, 0 );

			return 0;
		}
		break;

		case WM_MBUTTONDOWN:
		{
			//_EndMenu();
			_SendMessageW( hWnd, WM_CANCELMODE, 0, 0 );

			_SendMessageW( hWnd, WM_COMMAND, MENU_ALWAYS_ON_TOP, 0 );

			_SetForegroundWindow( hWnd );

			return 0;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			_SetForegroundWindow( hWnd );

			return 0;
		}
		break;

		case WM_DESTROY:
		{
			_DeleteObject( hbm_background );
			hbm_background = NULL;

			g_hWnd_url_drop_window = NULL;

			if ( use_drag_and_drop_url )
			{
				UnregisterDropWindow( hWnd, URL_DropTarget );

				_OleUninitialize();
			}

			return 0;
		}
		break;

		default:
		{
			return _DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		break;
	}
	//return TRUE;
}
