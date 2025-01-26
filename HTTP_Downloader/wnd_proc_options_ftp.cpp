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

#include "options.h"
#include "lite_gdi32.h"
#include "utilities.h"

#define BTN_PASSIVE_MODE				1000
#define BTN_ACTIVE_MODE					1001
#define BTN_FALLBACK_MODE				1002

#define BTN_TYPE_FTP_HOST				1003
#define BTN_TYPE_FTP_IP_ADDRESS			1004
#define EDIT_FTP_HOST					1005
#define EDIT_FTP_IP_ADDRESS				1006
#define EDIT_FTP_PORT_START				1007
#define EDIT_FTP_PORT_END				1008

#define BTN_SEND_KEEP_ALIVE				1009

HWND g_hWnd_static_transfer_mode = NULL;

HWND g_hWnd_chk_passive_mode = NULL;
HWND g_hWnd_chk_active_mode = NULL;
HWND g_hWnd_chk_fallback_mode = NULL;

HWND g_hWnd_static_active_listen_info = NULL;
HWND g_hWnd_chk_type_ftp_hostname = NULL;
HWND g_hWnd_chk_type_ftp_ip_address = NULL;
HWND g_hWnd_ftp_hostname = NULL;
HWND g_hWnd_ftp_ip_address = NULL;
HWND g_hWnd_ftp_static_colon = NULL;
HWND g_hWnd_static_ftp_port_start = NULL;
HWND g_hWnd_static_ftp_port_end = NULL;
HWND g_hWnd_ftp_static_dash = NULL;
HWND g_hWnd_ftp_port_start = NULL;
HWND g_hWnd_ftp_port_end = NULL;

HWND g_hWnd_chk_send_keep_alive = NULL;

HFONT hFont_copy_ftp = NULL;

LRESULT CALLBACK FTPTabWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			g_hWnd_static_transfer_mode = _CreateWindowW( WC_BUTTON, ST_V_Data_Transfer_Mode, BS_GROUPBOX | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );

			g_hWnd_chk_passive_mode = _CreateWindowW( WC_BUTTON, ST_V_Passive, BS_AUTORADIOBUTTON | WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_PASSIVE_MODE, NULL, NULL );
			g_hWnd_chk_active_mode = _CreateWindowW( WC_BUTTON, ST_V_Active, BS_AUTORADIOBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_ACTIVE_MODE, NULL, NULL );

			g_hWnd_chk_fallback_mode = _CreateWindowW( WC_BUTTON, ST_V_Use_other_mode_on_failure, BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_FALLBACK_MODE, NULL, NULL );

			g_hWnd_static_active_listen_info = _CreateWindowW( WC_BUTTON, ST_V_Active_Listen_Information, BS_GROUPBOX | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );

			g_hWnd_chk_type_ftp_hostname = _CreateWindowW( WC_BUTTON, ST_V_Hostname___IPv6_address_, BS_AUTORADIOBUTTON | WS_CHILD | WS_GROUP | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_TYPE_FTP_HOST, NULL, NULL );
			g_hWnd_chk_type_ftp_ip_address = _CreateWindowW( WC_BUTTON, ST_V_IPv4_address_, BS_AUTORADIOBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_TYPE_FTP_IP_ADDRESS, NULL, NULL );

			g_hWnd_ftp_hostname = _CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | ES_CENTER | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )EDIT_FTP_HOST, NULL, NULL );
			// Needs a width and height when it's created because it's a stupid control.
			g_hWnd_ftp_ip_address = _CreateWindowExW( WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_CHILD | WS_TABSTOP, 0, 0, 310, 23, hWnd, ( HMENU )EDIT_FTP_IP_ADDRESS, NULL, NULL );

			g_hWnd_ftp_static_colon = _CreateWindowW( WC_STATIC, ST_V_COLON, SS_CENTER | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );

			g_hWnd_static_ftp_port_start = _CreateWindowW( WC_STATIC, ST_V_Port_start_, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );
			g_hWnd_ftp_port_start = _CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | ES_CENTER | ES_NUMBER | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )EDIT_FTP_PORT_START, NULL, NULL );

			g_hWnd_ftp_static_dash = _CreateWindowW( WC_STATIC, ST_V_DASH, SS_CENTER | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );

			g_hWnd_static_ftp_port_end = _CreateWindowW( WC_STATIC, ST_V_Port_end_, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, NULL, NULL );
			g_hWnd_ftp_port_end = _CreateWindowExW( WS_EX_CLIENTEDGE, WC_EDIT, NULL, ES_AUTOHSCROLL | ES_CENTER | ES_NUMBER | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )EDIT_FTP_PORT_END, NULL, NULL );

			g_hWnd_chk_send_keep_alive = _CreateWindowW( WC_BUTTON, ST_V_Send_keep_alive_requests, BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, ( HMENU )BTN_SEND_KEEP_ALIVE, NULL, NULL );


			_SendMessageW( g_hWnd_static_transfer_mode, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_chk_passive_mode, WM_SETFONT, ( WPARAM )hFont_options, 0 );
			_SendMessageW( g_hWnd_chk_active_mode, WM_SETFONT, ( WPARAM )hFont_options, 0 );
			_SendMessageW( g_hWnd_chk_fallback_mode, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_static_active_listen_info, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_chk_type_ftp_hostname, WM_SETFONT, ( WPARAM )hFont_options, 0 );
			_SendMessageW( g_hWnd_chk_type_ftp_ip_address, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_ftp_hostname, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_ftp_static_colon, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_static_ftp_port_start, WM_SETFONT, ( WPARAM )hFont_options, 0 );
			_SendMessageW( g_hWnd_ftp_port_start, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_ftp_static_dash, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_static_ftp_port_end, WM_SETFONT, ( WPARAM )hFont_options, 0 );
			_SendMessageW( g_hWnd_ftp_port_end, WM_SETFONT, ( WPARAM )hFont_options, 0 );

			_SendMessageW( g_hWnd_chk_send_keep_alive, WM_SETFONT, ( WPARAM )hFont_options, 0 );



			_SendMessageW( g_hWnd_ftp_hostname, EM_LIMITTEXT, MAX_DOMAIN_LENGTH, 0 );
			_SendMessageW( g_hWnd_ftp_port_start, EM_LIMITTEXT, 5, 0 );
			_SendMessageW( g_hWnd_ftp_port_end, EM_LIMITTEXT, 5, 0 );

			//hFont_copy_ftp = _CreateFontIndirectW( &g_default_log_font );
			hFont_copy_ftp = UpdateFont( current_dpi_options );
			_SendMessageW( g_hWnd_ftp_ip_address, WM_SETFONT, ( WPARAM )hFont_copy_ftp, 0 );

			//

			BOOL enable;

			if ( cfg_ftp_mode_type == 1 )
			{
				enable = TRUE;

				_SendMessageW( g_hWnd_chk_active_mode, BM_SETCHECK, BST_CHECKED, 0 );
				_SendMessageW( g_hWnd_chk_passive_mode, BM_SETCHECK, BST_UNCHECKED, 0 );
			}
			else
			{
				enable = FALSE;

				_SendMessageW( g_hWnd_chk_passive_mode, BM_SETCHECK, BST_CHECKED, 0 );
				_SendMessageW( g_hWnd_chk_active_mode, BM_SETCHECK, BST_UNCHECKED, 0 );
			}

			if ( cfg_ftp_enable_fallback_mode )
			{
				enable = TRUE;

				_SendMessageW( g_hWnd_chk_fallback_mode, BM_SETCHECK, BST_CHECKED, 0 );
			}
			else
			{
				_SendMessageW( g_hWnd_chk_fallback_mode, BM_SETCHECK, BST_UNCHECKED, 0 );
			}

			_EnableWindow( g_hWnd_static_active_listen_info, enable );
			_EnableWindow( g_hWnd_chk_type_ftp_hostname, enable );
			_EnableWindow( g_hWnd_chk_type_ftp_ip_address, enable );
			_EnableWindow( g_hWnd_ftp_hostname, enable );
			_EnableWindow( g_hWnd_ftp_ip_address, enable );
			_EnableWindow( g_hWnd_ftp_static_colon, enable );
			_EnableWindow( g_hWnd_static_ftp_port_start, enable );
			_EnableWindow( g_hWnd_ftp_port_start, enable );

			if ( cfg_ftp_address_type == 1 )
			{
				_SendMessageW( g_hWnd_chk_type_ftp_ip_address, BM_SETCHECK, BST_CHECKED, 0 );
				_SendMessageW( g_hWnd_chk_type_ftp_hostname, BM_SETCHECK, BST_UNCHECKED, 0 );

				_ShowWindow( g_hWnd_ftp_hostname, SW_HIDE );
				_ShowWindow( g_hWnd_ftp_ip_address, SW_SHOW );
			}
			else
			{
				_SendMessageW( g_hWnd_chk_type_ftp_hostname, BM_SETCHECK, BST_CHECKED, 0 );
				_SendMessageW( g_hWnd_chk_type_ftp_ip_address, BM_SETCHECK, BST_UNCHECKED, 0 );

				_ShowWindow( g_hWnd_ftp_ip_address, SW_HIDE );
				_ShowWindow( g_hWnd_ftp_hostname, SW_SHOW );
			}

			if ( cfg_ftp_hostname == NULL )
			{
				_SendMessageW( g_hWnd_ftp_hostname, WM_SETTEXT, 0, ( LPARAM )L"localhost" );
			}
			else
			{
				_SendMessageW( g_hWnd_ftp_hostname, WM_SETTEXT, 0, ( LPARAM )cfg_ftp_hostname );
			}

			_SendMessageW( g_hWnd_ftp_ip_address, IPM_SETADDRESS, 0, cfg_ftp_ip_address );

			char value[ 6 ];
			_memzero( value, sizeof( char ) * 6 );
			__snprintf( value, 6, "%hu", cfg_ftp_port_start );
			_SendMessageA( g_hWnd_ftp_port_start, WM_SETTEXT, 0, ( LPARAM )value );
			__snprintf( value, 6, "%hu", cfg_ftp_port_end );
			_SendMessageA( g_hWnd_ftp_port_end, WM_SETTEXT, 0, ( LPARAM )value );

			if ( cfg_ftp_port_start == 0 )
			{
				enable = FALSE;
			}

			_EnableWindow( g_hWnd_ftp_port_end, enable );
			_EnableWindow( g_hWnd_ftp_static_dash, enable );
			_EnableWindow( g_hWnd_static_ftp_port_end, enable );

			_SendMessageW( g_hWnd_chk_send_keep_alive, BM_SETCHECK, ( cfg_ftp_send_keep_alive ? BST_CHECKED : BST_UNCHECKED ), 0 );

			return 0;
		}
		break;

		case WM_SIZE:
		{
			RECT rc;
			_GetClientRect( hWnd, &rc );

			HDWP hdwp = _BeginDeferWindowPos( 16 );
			_DeferWindowPos( hdwp, g_hWnd_static_transfer_mode, HWND_TOP, 0, 0, _SCALE_O_( 402 ), _SCALE_O_( 63 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_chk_passive_mode, HWND_TOP, _SCALE_O_( 11 ), _SCALE_O_( 17 ), _SCALE_O_( 80 ), _SCALE_O_( 20 ), SWP_NOZORDER );
			_DeferWindowPos( hdwp, g_hWnd_chk_active_mode, HWND_TOP, _SCALE_O_( 11 ), _SCALE_O_( 37 ), _SCALE_O_( 80 ), _SCALE_O_( 20 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_chk_fallback_mode, HWND_TOP, _SCALE_O_( 101 ), _SCALE_O_( 17 ), _SCALE_O_( 290 ), _SCALE_O_( 20 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_static_active_listen_info, HWND_TOP, 0, _SCALE_O_( 73 ), _SCALE_O_( 522 ), _SCALE_O_( 72 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_chk_type_ftp_hostname, HWND_TOP, _SCALE_O_( 11 ), _SCALE_O_( 90 ), _SCALE_O_( 200 ), _SCALE_O_( 20 ), SWP_NOZORDER );
			_DeferWindowPos( hdwp, g_hWnd_chk_type_ftp_ip_address, HWND_TOP, _SCALE_O_( 216 ), _SCALE_O_( 90 ), _SCALE_O_( 110 ), _SCALE_O_( 20 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_ftp_hostname, HWND_TOP, _SCALE_O_( 11 ), _SCALE_O_( 110 ), _SCALE_O_( 310 ), _SCALE_O_( 23 ), SWP_NOZORDER );
			_DeferWindowPos( hdwp, g_hWnd_ftp_ip_address, HWND_TOP, _SCALE_O_( 11 ), _SCALE_O_( 110 ), _SCALE_O_( 310 ), _SCALE_O_( 23 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_ftp_static_colon, HWND_TOP, _SCALE_O_( 322 ), _SCALE_O_( 113 ), _SCALE_O_( 8 ), _SCALE_O_( 17 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_static_ftp_port_start, HWND_TOP, _SCALE_O_( 331 ), _SCALE_O_( 92 ), _SCALE_O_( 85 ), _SCALE_O_( 17 ), SWP_NOZORDER );
			_DeferWindowPos( hdwp, g_hWnd_ftp_port_start, HWND_TOP, _SCALE_O_( 331 ), _SCALE_O_( 110 ), _SCALE_O_( 85 ), _SCALE_O_( 23 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_ftp_static_dash, HWND_TOP, _SCALE_O_( 417 ), _SCALE_O_( 113 ), _SCALE_O_( 8 ), _SCALE_O_( 17 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_static_ftp_port_end, HWND_TOP, _SCALE_O_( 426 ), _SCALE_O_( 92 ), _SCALE_O_( 85 ), _SCALE_O_( 17 ), SWP_NOZORDER );
			_DeferWindowPos( hdwp, g_hWnd_ftp_port_end, HWND_TOP, _SCALE_O_( 426 ), _SCALE_O_( 110 ), _SCALE_O_( 85 ), _SCALE_O_( 23 ), SWP_NOZORDER );

			_DeferWindowPos( hdwp, g_hWnd_chk_send_keep_alive, HWND_TOP, 0, _SCALE_O_( 154 ), _SCALE_O_( 300 ), _SCALE_O_( 20 ), SWP_NOZORDER );

			_EndDeferWindowPos( hdwp );

			return 0;
		}
		break;

		case WM_GET_DPI:
		{
			return current_dpi_options;
		}
		break;

		case WM_DPICHANGED_AFTERPARENT:
		{
			// This stupid control doesn't adapt to the change in font size. It needs to be resized first.
			_SetWindowPos( g_hWnd_ftp_ip_address, HWND_TOP, 0, 0, _SCALE_O_( 310 ), _SCALE_O_( 23 ), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
			_DeleteObject( hFont_copy_ftp );
			hFont_copy_ftp = UpdateFont( current_dpi_options );
			_SendMessageW( g_hWnd_ftp_ip_address, WM_SETFONT, ( WPARAM )hFont_copy_ftp, 0 );

			// Return value is ignored.
			return TRUE;
		}
		break;

		case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
				case BTN_PASSIVE_MODE:
				case BTN_ACTIVE_MODE:
				case BTN_FALLBACK_MODE:
				{
					BOOL enable;

					if ( _SendMessageW( g_hWnd_chk_fallback_mode, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
					{
						enable = TRUE;
					}
					else
					{
						enable = ( _SendMessageW( g_hWnd_chk_active_mode, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? TRUE : FALSE );
					}

					_EnableWindow( g_hWnd_static_active_listen_info, enable );
					_EnableWindow( g_hWnd_chk_type_ftp_hostname, enable );
					_EnableWindow( g_hWnd_chk_type_ftp_ip_address, enable );
					_EnableWindow( g_hWnd_ftp_hostname, enable );
					_EnableWindow( g_hWnd_ftp_ip_address, enable );
					_EnableWindow( g_hWnd_ftp_static_colon, enable );
					_EnableWindow( g_hWnd_static_ftp_port_start, enable );
					_EnableWindow( g_hWnd_ftp_port_start, enable );

					char value[ 11 ];
					_SendMessageA( g_hWnd_ftp_port_end, WM_GETTEXT, 6, ( LPARAM )value );
					if ( _strtoul( value, NULL, 10 ) == 0 )
					{
						enable = FALSE;
					}

					_EnableWindow( g_hWnd_ftp_port_end, enable );
					_EnableWindow( g_hWnd_ftp_static_dash, enable );
					_EnableWindow( g_hWnd_static_ftp_port_end, enable );

					_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
				}
				break;

				case EDIT_FTP_PORT_START:
				case EDIT_FTP_PORT_END:
				{
					if ( HIWORD( wParam ) == EN_UPDATE )
					{
						DWORD sel_start;

						char value[ 11 ];
						_SendMessageA( g_hWnd_ftp_port_end, WM_GETTEXT, 6, ( LPARAM )value );
						unsigned long num_end = _strtoul( value, NULL, 10 );
						unsigned int start_length = ( unsigned int )_SendMessageA( g_hWnd_ftp_port_start, WM_GETTEXT, 6, ( LPARAM )value );
						unsigned long num_start = _strtoul( value, NULL, 10 );

						if ( num_start > 65535 )	// If the start port is beyond the port range, set it and the end port to the highest port number.
						{
							_SendMessageA( g_hWnd_ftp_port_start, EM_GETSEL, ( WPARAM )&sel_start, NULL );

							_SendMessageA( g_hWnd_ftp_port_start, WM_SETTEXT, 0, ( LPARAM )"65535" );

							_SendMessageA( g_hWnd_ftp_port_start, EM_SETSEL, sel_start, sel_start );

							num_end = num_start;
						}
						else if ( num_start == 0 )	// Make sure that if the start port is 0, that the end port is as well.
						{
							if ( start_length == 0 )
							{
								_SendMessageA( g_hWnd_ftp_port_start, WM_SETTEXT, 0, ( LPARAM )"0" );
							}

							if ( num_end > 0 )
							{
								_SendMessageA( g_hWnd_ftp_port_end, WM_SETTEXT, 0, ( LPARAM )"0" );
							}

							num_end = num_start;
						}

						if ( num_end > 65535 )
						{
							_SendMessageA( g_hWnd_ftp_port_end, EM_GETSEL, ( WPARAM )&sel_start, NULL );

							_SendMessageA( g_hWnd_ftp_port_end, WM_SETTEXT, 0, ( LPARAM )"65535" );

							_SendMessageA( g_hWnd_ftp_port_end, EM_SETSEL, sel_start, sel_start );
						}
						else if ( num_start > num_end )	// Make sure the end port is always greater than or equal to the start port.
						{
							_SendMessageA( g_hWnd_ftp_port_end, WM_SETTEXT, 0, ( LPARAM )value );

							_SendMessageA( g_hWnd_ftp_port_end, EM_SETSEL, start_length, -1 );

							num_end = num_start;
						}

						BOOL enable = ( num_start > 0 ? TRUE : FALSE );
						_EnableWindow( g_hWnd_ftp_port_end, enable );
						_EnableWindow( g_hWnd_ftp_static_dash, enable );
						_EnableWindow( g_hWnd_static_ftp_port_end, enable );

						if ( ( LOWORD( wParam ) == EDIT_FTP_PORT_START && num_start != cfg_ftp_port_start ) ||
							 ( LOWORD( wParam ) == EDIT_FTP_PORT_END && num_end != cfg_ftp_port_end ) )
						{
							_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
						}
					}
					/*else if ( HIWORD( wParam ) == EN_KILLFOCUS )
					{
						char value[ 11 ];
						_SendMessageA( g_hWnd_ftp_port_end, WM_GETTEXT, 6, ( LPARAM )value );
						unsigned long num_end = _strtoul( value, NULL, 10 );
						unsigned int start_length = ( unsigned int )_SendMessageA( g_hWnd_ftp_port_start, WM_GETTEXT, 6, ( LPARAM )value );
						unsigned long num_start = _strtoul( value, NULL, 10 );

						if ( num_start > num_end )	// Make sure the end port is always greater than the start port.
						{
							_SendMessageA( g_hWnd_ftp_port_end, WM_SETTEXT, 0, ( LPARAM )value );

							_SendMessageA( g_hWnd_ftp_port_end, EM_SETSEL, start_length, -1 );
						}

						if ( ( LOWORD( wParam ) == EDIT_FTP_PORT_START && num_start != cfg_ftp_port_start ) ||
							 ( LOWORD( wParam ) == EDIT_FTP_PORT_END && num_end != cfg_ftp_port_end ) )
						{
							_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
						}
					}*/
				}
				break;

				case EDIT_FTP_HOST:
				{
					if ( HIWORD( wParam ) == EN_UPDATE )
					{
						_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
					}
				}
				break;

				case EDIT_FTP_IP_ADDRESS:
				{
					if ( HIWORD( wParam ) == EN_CHANGE )
					{
						_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
					}
				}
				break;

				case BTN_TYPE_FTP_HOST:
				{
					if ( _SendMessageW( g_hWnd_chk_type_ftp_hostname, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
					{
						_ShowWindow( g_hWnd_ftp_ip_address, SW_HIDE );
						_ShowWindow( g_hWnd_ftp_hostname, SW_SHOW );
					}

					_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
				}
				break;

				case BTN_TYPE_FTP_IP_ADDRESS:
				{
					if ( _SendMessageW( g_hWnd_chk_type_ftp_ip_address, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
					{
						_ShowWindow( g_hWnd_ftp_hostname, SW_HIDE );
						_ShowWindow( g_hWnd_ftp_ip_address, SW_SHOW );
					}

					_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
				}
				break;

				case BTN_SEND_KEEP_ALIVE:
				{
					_SendMessageW( g_hWnd_options, WM_OPTIONS_CHANGED, TRUE, 0 );
				}
				break;
			}

			return 0;
		}
		break;

		case WM_SAVE_OPTIONS:
		{
			cfg_ftp_mode_type = ( _SendMessageW( g_hWnd_chk_active_mode, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? 1 : 0 );

			cfg_ftp_enable_fallback_mode = ( _SendMessageW( g_hWnd_chk_fallback_mode, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );

			cfg_ftp_address_type = ( _SendMessageW( g_hWnd_chk_type_ftp_ip_address, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? 1 : 0 );

			unsigned int hostname_length = ( unsigned int )_SendMessageW( g_hWnd_ftp_hostname, WM_GETTEXTLENGTH, 0, 0 ) + 1;	// Include the NULL terminator.
			if ( cfg_ftp_hostname != NULL )
			{
				GlobalFree( cfg_ftp_hostname );
			}
			cfg_ftp_hostname = ( wchar_t * )GlobalAlloc( GMEM_FIXED, sizeof( wchar_t ) * hostname_length );
			_SendMessageW( g_hWnd_ftp_hostname, WM_GETTEXT, hostname_length, ( LPARAM )cfg_ftp_hostname );

			_SendMessageW( g_hWnd_ftp_ip_address, IPM_GETADDRESS, 0, ( LPARAM )&cfg_ftp_ip_address );

			char value[ 6 ];

			_SendMessageA( g_hWnd_ftp_port_start, WM_GETTEXT, 6, ( LPARAM )value );
			cfg_ftp_port_start = ( unsigned short )_strtoul( value, NULL, 10 );

			_SendMessageA( g_hWnd_ftp_port_end, WM_GETTEXT, 6, ( LPARAM )value );
			cfg_ftp_port_end = ( unsigned short )_strtoul( value, NULL, 10 );

			cfg_ftp_send_keep_alive = ( _SendMessageW( g_hWnd_chk_send_keep_alive, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? true : false );

			return 0;
		}
		break;

		case WM_DESTROY:
		{
			_DeleteObject( hFont_copy_ftp );
			hFont_copy_ftp = NULL;

#ifdef ENABLE_DARK_MODE
			if ( g_use_dark_mode )
			{
				CleanupButtonGlyphs( hWnd );
			}
#endif

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
