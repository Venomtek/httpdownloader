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

#ifndef _LITE_KERNEL32_H
#define _LITE_KERNEL32_H

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#define KERNEL32_USE_STATIC_LIB

#ifdef KERNEL32_USE_STATIC_LIB

	//__pragma( comment( lib, "kernel32.lib" ) )

	#define _SetFileInformationByHandle		SetFileInformationByHandle
	#define _GetUserDefaultLocaleName		GetUserDefaultLocaleName
	//#define _GetQueuedCompletionStatusEx	GetQueuedCompletionStatusEx

#else

	#define KERNEL32_STATE_SHUTDOWN	0
	#define KERNEL32_STATE_RUNNING	1

	typedef BOOL ( WINAPI *pSetFileInformationByHandle )( HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize );
	typedef int ( WINAPI *pGetUserDefaultLocaleName )( LPWSTR lpLocaleName, int cchLocaleName );
	//typedef BOOL ( WINAPI *pGetQueuedCompletionStatusEx )( HANDLE CompletionPort, LPOVERLAPPED_ENTRY lpCompletionPortEntries, ULONG ulCount, PULONG ulNumEntriesRemoved, DWORD dwMilliseconds, BOOL fAlertable );

	extern pSetFileInformationByHandle		_SetFileInformationByHandle;
	extern pGetUserDefaultLocaleName		_GetUserDefaultLocaleName;
	//extern pGetQueuedCompletionStatusEx		_GetQueuedCompletionStatusEx;

	extern unsigned char kernel32_state;

	bool InitializeKernel32();
	bool UnInitializeKernel32();

#endif

#endif
