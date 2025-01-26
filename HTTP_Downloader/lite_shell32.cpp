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
#include "lite_shell32.h"

#ifndef SHELL32_USE_STATIC_LIB

	pShell_NotifyIconW		_Shell_NotifyIconW;
	pShellExecuteW			_ShellExecuteW;

	pStrChrA				_StrChrA;
	pStrChrW				_StrChrW;
	pStrStrA				_StrStrA;
	pStrStrW				_StrStrW;
	pStrStrIA				_StrStrIA;
	pStrStrIW				_StrStrIW;

	pStrCmpNA				_StrCmpNA;
	pStrCmpNW				_StrCmpNW;
	pStrCmpNIA				_StrCmpNIA;

	pStrCmpNIW				_StrCmpNIW;

	pSHBrowseForFolderW		_SHBrowseForFolderW;
	pSHGetPathFromIDListW	_SHGetPathFromIDListW;

	pSHGetFileInfoW			_SHGetFileInfoW;
	pSHGetFolderPathW		_SHGetFolderPathW;
	pSHGetKnownFolderPath	_SHGetKnownFolderPath;

	pSHFileOperationW		_SHFileOperationW;

	pSHOpenFolderAndSelectItems	_SHOpenFolderAndSelectItems;
	pILCreateFromPathW	_ILCreateFromPathW;
	//pILFree				_ILFree;

	pCommandLineToArgvW		_CommandLineToArgvW;

	//pDragAcceptFiles		_DragAcceptFiles;
	pDragQueryFileW			_DragQueryFileW;
	//pDragFinish				_DragFinish;

	pSHCreateStdEnumFmtEtc	_SHCreateStdEnumFmtEtc;

	pExtractIconExW			_ExtractIconExW;

	HMODULE hModule_shell32 = NULL;

	unsigned char shell32_state = SHELL32_STATE_SHUTDOWN;

	bool InitializeShell32()
	{
		if ( shell32_state != SHELL32_STATE_SHUTDOWN )
		{
			return true;
		}

		hModule_shell32 = LoadLibraryDEMW( L"shell32.dll" );

		if ( hModule_shell32 == NULL )
		{
			return false;
		}

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_Shell_NotifyIconW, "Shell_NotifyIconW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_ShellExecuteW, "ShellExecuteW" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrChrA, "StrChrA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrChrW, "StrChrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrStrA, "StrStrA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrStrW, "StrStrW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrStrIA, "StrStrIA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrStrIW, "StrStrIW" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrCmpNA, "StrCmpNA" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrCmpNW, "StrCmpNW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrCmpNIA, "StrCmpNIA" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_StrCmpNIW, "StrCmpNIW" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHBrowseForFolderW, "SHBrowseForFolderW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHGetPathFromIDListW, "SHGetPathFromIDListW" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHGetFileInfoW, "SHGetFileInfoW" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHGetFolderPathW, "SHGetFolderPathW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHGetKnownFolderPath, "SHGetKnownFolderPath" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHFileOperationW, "SHFileOperationW" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHOpenFolderAndSelectItems, "SHOpenFolderAndSelectItems" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_ILCreateFromPathW, "ILCreateFromPathW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_ILFree, "ILFree" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_CommandLineToArgvW, "CommandLineToArgvW" ) )

		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_DragAcceptFiles, "DragAcceptFiles" ) )
		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_DragQueryFileW, "DragQueryFileW" ) )
		//VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_DragFinish, "DragFinish" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_SHCreateStdEnumFmtEtc, "SHCreateStdEnumFmtEtc" ) )

		VALIDATE_FUNCTION_POINTER( SetFunctionPointer( hModule_shell32, ( void ** )&_ExtractIconExW, "ExtractIconExW" ) )

		shell32_state = SHELL32_STATE_RUNNING;

		return true;
	}

	bool UnInitializeShell32()
	{
		if ( shell32_state != SHELL32_STATE_SHUTDOWN )
		{
			shell32_state = SHELL32_STATE_SHUTDOWN;

			return ( FreeLibrary( hModule_shell32 ) == FALSE ? false : true );
		}

		return true;
	}

#endif

int _StrCmpA( PCSTR psz1, PCSTR psz2 )
{
	if ( psz1 == NULL && psz2 == NULL )
	{
		return 0;
	}
	else if ( psz1 == NULL )
	{
		return 1;
	}
	else if ( psz2 == NULL )
	{
		return -1;
	}

	while ( *psz1 && *psz1 == *psz2 )
	{
		++psz1;
		++psz2;
	}

	return ( *psz1 - *psz2 );
}

int _StrCmpW( PCWSTR psz1, PCWSTR psz2 )
{
	if ( psz1 == NULL && psz2 == NULL )
	{
		return 0;
	}
	else if ( psz1 == NULL )
	{
		return 1;
	}
	else if ( psz2 == NULL )
	{
		return -1;
	}

	while ( *psz1 && *psz1 == *psz2 )
	{
		++psz1;
		++psz2;
	}

	return ( *psz1 - *psz2 );
}