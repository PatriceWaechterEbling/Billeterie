// header.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
#define BEFORENAVIGATE2 WM_USER
#define DOWNLOADBEGIN WM_USER+1
#define DOWNLOADCOMPLETE WM_USER+2
#define NAVIGATECOMPLETE2  WM_USER+3
#define DOCUMENTCOMPLETE WM_USER+4
#define COMMANDSTATECHANGE WM_USER+5
#define WC_SHELLEXPLORER "TShellExplorer"

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windowsx.h>
#include <wingdi.h>
#include <commctrl.h>
#include <Winuser.h>
#include <Commdlg.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <stdio.h>
#include <conio.h>
#include <winioctl.h>
#include "resource.h"

#pragma comment (lib,"comctl32.lib") 
#pragma comment (lib,"kernel32.lib") 
#pragma comment (lib,"user32.lib") 
#pragma comment (lib,"gdi32.lib") 
#pragma comment (lib,"winspool.lib") 
#pragma comment (lib,"comdlg32.lib") 
#pragma comment (lib,"advapi32.lib") 
#pragma comment (lib,"shell32.lib") 
#pragma comment (lib,"ole32.lib") 
#pragma comment (lib,"oleaut32.lib") 
#pragma comment (lib,"uuid.lib") 
#pragma comment (lib,"odbc32.lib") 
#pragma comment (lib,"odbccp32.lib") 
#pragma comment (lib,"shlwapi.lib") 
#pragma warning(disable: 4996)