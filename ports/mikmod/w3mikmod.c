/* W3MikMod
     (c) 2021 Toyoyo

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
     02111-1307, USA.
*/

#define MAINWINDOW_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <commctrl.h>
#include <winuser.h>
#include <mikmod.h>

#define W3MIDMOKVERSION "0.1.5"
#define MOD_EXT "*.669;*.amf;*.dsm;*.far;*.gdm;*.it;*.imf;*.mod;*.med;*.mtm;*.okt;*.s3m;*.stm;*.stx;*.ult;*.umx;*.xm"
#define NOMOD "-- No module loaded --"
#define MAXVOICES 256
#define MikMod_Sleep(ns) Sleep(ns / 1000)

/* function definitions */
LRESULT CALLBACK fnWndProcMain(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK fnWndProcSamples(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK fnWndProcInstr(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK fnWndProcVol(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
void CALLBACK Timer0Proc(HWND hWnd, unsigned int msg, unsigned int idTimer, DWORD dwTime);
void UpdateSamples(void);
void UpdateInstr(void);
void UpdateVol(BOOL UpdateChn);
void UpdateStatus(void);
void SwitchSamplesBTN();
void SwitchInstrBTN();
void SwitchVolBTN();
void PlayListNext(int PlayListCount, int Start);
void PlayListPrev(int PlayListCount, int Start);
void ModuleLoad(char *ModFN, int Play);
void ClientResize(HWND hWnd, int nWidth, int nHeight);
char *ReplStr(const char *str, const char *from, const char *to);

/* Global Variables */
MODULE *module;
BOOL isPlaying = 0;
BOOL isStarted = 0;
BOOL isLoaded = 0;
BOOL CfgEmpty = 0;
BOOL CfgAveWidth = 0;
BOOL CfgMixFreq = 44100;
HWND hStatus;
HWND hSongName;
HWND hTrackerType;
HWND hFileName;
HWND hSamplesStatic;
HWND hSamplesWin;
HWND hInstrStatic;
HWND hInstrWin;
HWND hVolStatic;
HWND hVolWin;
HWND hPList;
HWND hToolbar;
HBRUSH hBrushMain;
HBRUSH hBrushSamples;
HBRUSH hBrushInstr;
HBRUSH hBrushVol;
HBRUSH hBrushNfo;
HBRUSH hBrushGreen;
HBRUSH hBrushYellow;
HBRUSH hBrushRed;
HFONT s_hFont;
int TimeCount = 0;
int FontX, FontY;
int PlayListPos = -1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASS wcMain;
  WNDCLASS wcSamples;
  WNDCLASS wcInstr;
  WNDCLASS wcVol;
  MSG messages;
  HWND hwnd;
  HACCEL KbdAccelTable;

  char CfgFont[1024];
  int CfgFontSize = 10;
  int i;

  hBrushMain = CreateSolidBrush(RGB(0,0,0));
  hBrushSamples = CreateSolidBrush(RGB(0,0,0));
  hBrushInstr = CreateSolidBrush(RGB(0,0,0));
  hBrushVol = CreateSolidBrush(RGB(0,0,0));
  hBrushNfo = CreateSolidBrush(RGB(0,0,205));

  hBrushGreen = CreateSolidBrush(RGB(0,205,0));
  hBrushYellow = CreateSolidBrush(RGB(205,205,0));
  hBrushRed = CreateSolidBrush(RGB(205,0,0));

  CoInitialize(NULL);
  InitCommonControls();

  /* main window */
  memset(&wcMain,0,sizeof(WNDCLASS));
  wcMain.lpszClassName = "w3mikmod_main";
  wcMain.lpfnWndProc   = fnWndProcMain;
  wcMain.hbrBackground = hBrushMain;
  wcMain.hInstance     = hInstance;
  wcMain.cbWndExtra    = sizeof(void*);
  wcMain.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(0), IMAGE_ICON, 32, 32, 0);
  RegisterClass(&wcMain);

  /* samples window */
  memset(&wcSamples,0,sizeof(WNDCLASS));
  wcSamples.lpszClassName = "w3mikmod_samples";
  wcSamples.lpfnWndProc   = fnWndProcSamples;
  wcSamples.hbrBackground = hBrushSamples;
  wcSamples.hInstance     = hInstance;
  wcSamples.cbWndExtra    = sizeof(void*);
  wcSamples.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(3), IMAGE_ICON, 32, 32, 0);
  RegisterClass(&wcSamples);

  /* instruments window */
  memset(&wcInstr,0,sizeof(WNDCLASS));
  wcInstr.lpszClassName = "w3mikmod_instr";
  wcInstr.lpfnWndProc   = fnWndProcInstr;
  wcInstr.hbrBackground = hBrushInstr;
  wcInstr.hInstance     = hInstance;
  wcInstr.cbWndExtra    = sizeof(void*);
  wcInstr.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(4), IMAGE_ICON, 32, 32, 0);
  RegisterClass(&wcInstr);

  /* volume window */
  memset(&wcVol,0,sizeof(WNDCLASS));
  wcVol.lpszClassName = "w3mikmod_vol";
  wcVol.lpfnWndProc   = fnWndProcVol;
  wcVol.hbrBackground = hBrushVol;
  wcVol.hInstance     = hInstance;
  wcVol.cbWndExtra    = sizeof(void*);
  wcVol.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(5), IMAGE_ICON, 32, 32, 0);
  RegisterClass(&wcVol);

  /* w3mikmod.ini parsing */
  GetPrivateProfileString("W3MikMod", "font", "Courier", CfgFont, 1023, ".\\w3mikmod.ini");
  CfgFontSize = GetPrivateProfileInt("W3MikMod", "size", 10, ".\\w3mikmod.ini");
  CfgEmpty = GetPrivateProfileInt("W3MikMod", "empty", 1, ".\\w3mikmod.ini");
  CfgAveWidth = GetPrivateProfileInt("W3MikMod", "avewidth", 0, ".\\w3mikmod.ini");
  CfgMixFreq = GetPrivateProfileInt("W3MikMod", "mixfreq", 44100, ".\\w3mikmod.ini");
  s_hFont = CreateFont(CfgFontSize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CfgFont);

  md_mixfreq = CfgMixFreq;
  md_mode |= DMODE_INTERP | DMODE_SURROUND | DMODE_HQMIXER | DMODE_16BITS;

  MikMod_RegisterAllDrivers();
  MikMod_RegisterAllLoaders();

  /* New default value (2) just... doesn't really work on my potato*/
  md_device=1;
  if(MikMod_Init("count=6") != 0) {
    MessageBox(NULL, "Could not initialize MikMod\r\nExiting", "W3MikMod - Error", MB_OK|MB_ICONERROR);
    goto cleanup;
  }

  hwnd = CreateWindowEx(0, "w3mikmod_main", "W3MikMod - "W3MIDMOKVERSION, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
           CW_USEDEFAULT, CW_USEDEFAULT, 320, 100,
           NULL, NULL, hInstance, NULL);

  /* Modules on command line */
  if(__argc > 1) {
    for(i = 1; i < __argc ; i++) {
      SendMessage(hPList, LB_ADDSTRING, 0, (LPARAM)__argv[i]);
    }
    isPlaying = 1; /* next timer tick will handle everything */
  }

  SetTimer(hwnd, 1001, 100, (TIMERPROC) Timer0Proc);

  SetWindowLong(hwnd,0*sizeof(void*),(long)hBrushMain);
  ShowWindow(hwnd, 5);

  KbdAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(6));

  while(GetMessage(&messages, NULL, 0, 0)) {
    /* For keyboard control */
    TranslateAcceleratorA(hwnd, KbdAccelTable, &messages);
    /* Required for WS_TABSTOP to work*/
    if(!IsDialogMessage(hwnd, &messages)) {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
    }
  }

  cleanup:
  DeleteObject(hBrushMain);
  DeleteObject(hBrushSamples);
  DeleteObject(hBrushInstr);
  DeleteObject(hBrushVol);
  DeleteObject(hBrushNfo);
  DeleteObject(hBrushGreen);
  DeleteObject(hBrushYellow);
  DeleteObject(hBrushRed);

  DeleteObject(s_hFont);
  UnregisterClass("w3mikmod_main", hInstance);
  UnregisterClass("w3mikmod_samples", hInstance);
  UnregisterClass("w3mikmod_instr", hInstance);
  UnregisterClass("w3mikmod_vol", hInstance);
  Player_Stop();
  Sleep(1000);
  MikMod_free(module);
  MikMod_Exit();

  ExitProcess(0);

  /* Useless, avoids a warning, ExitProcess() is mandatory for win32s */
  return 0;
}

LRESULT CALLBACK fnWndProcMain(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
  HDC hdcMain;
  TEXTMETRIC tm;
  TBADDBITMAP tbab;
  TBBUTTON tbb[11];
  HDROP hDropInfo = NULL;

  switch(msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;
      HWND hPListAdd;
      HWND hPListDel;
      HWND hPListLoad;
      int SamplesInitSize;

      /* Toolbar creation */
      hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)4000, GetModuleHandle(NULL), NULL);
      SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

      tbab.hInst = (HINSTANCE)GetModuleHandle(NULL);
      tbab.nID = 2;
      SendMessage(hToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM( 26, 26 ));
      SendMessage(hToolbar, TB_ADDBITMAP, 1, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 2, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 3, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 4, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 5, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 6, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 7, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 8, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 9, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 10, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 11, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 12, (LPARAM)&tbab);
      SendMessage(hToolbar, TB_ADDBITMAP, 13, (LPARAM)&tbab);

      /* Toolbar definition */
      ZeroMemory(tbb, sizeof(tbb));
      tbb[0].iBitmap = 0;
      tbb[0].fsState = TBSTATE_ENABLED;
      tbb[0].fsStyle = TBSTYLE_BUTTON;
      tbb[0].idCommand = 2000;

      tbb[1].iBitmap = 1;
      tbb[1].fsState = TBSTATE_ENABLED;
      tbb[1].fsStyle = TBSTYLE_BUTTON;
      tbb[1].idCommand = 2001;

      tbb[2].iBitmap = 2;
      tbb[2].fsState = TBSTATE_ENABLED;
      tbb[2].fsStyle = TBSTYLE_BUTTON;
      tbb[2].idCommand = 2002;

      tbb[3].iBitmap = 3;
      tbb[3].fsState = TBSTATE_ENABLED;
      tbb[3].fsStyle = TBSTYLE_BUTTON;
      tbb[3].idCommand = 2003;

      tbb[4].iBitmap = 4;
      tbb[4].fsState = TBSTATE_ENABLED;
      tbb[4].fsStyle = TBSTYLE_BUTTON;
      tbb[4].idCommand = 2004;

      tbb[5].iBitmap = 5;
      tbb[5].fsState = TBSTATE_ENABLED;
      tbb[5].fsStyle = TBSTYLE_BUTTON;
      tbb[5].idCommand = 2005;

      tbb[6].iBitmap = 6;
      tbb[6].fsState = TBSTATE_ENABLED;
      tbb[6].fsStyle = TBSTYLE_BUTTON;
      tbb[6].idCommand = 2009;

      tbb[7].iBitmap = 7;
      tbb[7].fsState = TBSTATE_ENABLED;
      tbb[7].fsStyle = TBSTYLE_BUTTON;
      tbb[7].idCommand = 2006;

      tbb[8].iBitmap = 8;
      tbb[8].fsState = TBSTATE_ENABLED;
      tbb[8].fsStyle = TBSTYLE_BUTTON;
      tbb[8].idCommand = 2007;

      tbb[9].iBitmap = 9;
      tbb[9].fsState = TBSTATE_ENABLED;
      tbb[9].fsStyle = TBSTYLE_BUTTON;
      tbb[9].idCommand = 2008;

      tbb[10].fsStyle = TBSTYLE_SEP;
      tbb[10].iBitmap = 6;

      ClientResize(hWnd, 331, 161);
      SendMessage(hToolbar, TB_ADDBUTTONS, sizeof(tbb)/sizeof(TBBUTTON), (LPARAM)&tbb);
      SendMessage(hToolbar, TB_AUTOSIZE, 0, 0);

      /* Our controls */
      hFileName = CreateWindowEx(0,"Static","-- FileName --",WS_CHILD|WS_VISIBLE,0,38,331,14,hWnd,(HMENU)3002,hIns,0);
      hSongName = CreateWindowEx(0,"Static","-- SongName --",WS_CHILD|WS_VISIBLE,0,52,331,14,hWnd,(HMENU)3001,hIns,0);
      hTrackerType = CreateWindowEx(0,"Static","-- Tracker Name --",WS_CHILD|WS_VISIBLE,0,66,331,14,hWnd,(HMENU)3002,hIns,0);
      CreateWindowEx(0,"Static","",WS_CHILD|WS_VISIBLE|SS_WHITERECT,309,97,22,100,hWnd,(HMENU)3004,hIns,0);
      hStatus = CreateWindowEx(0,"Static","-- Status --", WS_CHILD|WS_VISIBLE,0,80,331,16,hWnd,(HMENU)3000,hIns,0);
      hPList = CreateWindowEx(0,"Listbox","",WS_TABSTOP|WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|LBS_EXTENDEDSEL|LBS_NOINTEGRALHEIGHT|WS_VSCROLL,0,98,307,62,hWnd,(HMENU)3100,hIns,0);
      hPListAdd = CreateWindowEx(0,"Button","+",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,310,98,20,20,hWnd,(HMENU)3101,hIns,0);
      hPListDel = CreateWindowEx(0,"Button","-",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,310,119,20,20,hWnd,(HMENU)3102,hIns,0);
      hPListLoad = CreateWindowEx(0,"Button",">",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,310,140,20,20,hWnd,(HMENU )3103,hIns,0);
      DragAcceptFiles(hWnd, TRUE);

      SendMessage(hStatus, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      SendMessage(hSongName, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      SendMessage(hTrackerType, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      SendMessage(hFileName, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      SendMessage(hPList, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));

      /* Create the Samples window */
      hdcMain=GetDC(NULL);
      SelectObject(hdcMain, s_hFont);
      GetTextMetrics(hdcMain, &tm);
      ReleaseDC(NULL, hdcMain);

      if(CfgAveWidth) {
        FontX=tm.tmAveCharWidth;
      } else {
        FontX=tm.tmMaxCharWidth;
      }
      FontY=tm.tmHeight;
      SamplesInitSize=GetSystemMetrics(SM_CXSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER)+FontY;

      hSamplesWin=CreateWindowEx(0, "w3mikmod_samples", "W3MikMod - Samples", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, SamplesInitSize,
        NULL, NULL, hIns, NULL);
      SetWindowLong(hSamplesWin,0*sizeof(void*),(long)hBrushSamples);

      hInstrWin=CreateWindowEx(0, "w3mikmod_instr", "W3MikMod - Instruments", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, SamplesInitSize,
        NULL, NULL, hIns, NULL);
      SetWindowLong(hInstrWin,0*sizeof(void*),(long)hBrushInstr);

      hVolWin=CreateWindowEx(0, "w3mikmod_vol", "W3MikMod - Volume", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, SamplesInitSize,
        NULL, NULL, hIns, NULL);
      SetWindowLong(hVolWin,0*sizeof(void*),(long)hBrushVol);

      return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
      HDC hdcSTC = (HDC)wParam;

      if((LPARAM)lParam == (LPARAM)hFileName) {
        SetTextColor(hdcSTC, RGB(255,255,255));
        SetBkColor(hdcSTC, RGB(0,0,205));
        return (long)hBrushNfo;
      }
      if((LPARAM)lParam == (LPARAM)hSongName) {
        SetTextColor(hdcSTC, RGB(255,255,255));
        SetBkColor(hdcSTC, RGB(0,0,205));
        return (long)hBrushNfo;
      }
      if((LPARAM)lParam == (LPARAM)hTrackerType) {
        SetTextColor(hdcSTC, RGB(255,255,255));
        SetBkColor(hdcSTC, RGB(0,0,205));
        return (long)hBrushNfo;
      }
      if((LPARAM)lParam == (LPARAM)hStatus) {
        SetTextColor(hdcSTC, RGB(255,255,255));
        SetBkColor(hdcSTC, RGB(0,0,205));
        return (long)hBrushNfo;
      }

      SetBkMode((HDC)wParam,TRANSPARENT);
      return GetWindowLong(hWnd,0*sizeof(void*));
    }
    case WM_CTLCOLORLISTBOX:
    {
      HDC hdcPL;

      if((LPARAM)lParam == (LPARAM)hPList) {
        hdcPL = (HDC)wParam;
        SetTextColor(hdcPL, RGB(0,205,205));
        SetBkColor(hdcPL, RGB(0,0,0));
        return (long)hBrushSamples;
      }
    }
    case WM_DESTROY:
    {
      MikMod_DisableOutput();
      KillTimer(hWnd, 1001);
      DestroyWindow(hSamplesWin);
      DestroyWindow(hInstrWin);
      DestroyWindow(hVolWin);
      PostQuitMessage(0);
      return 0;
    }
    case WM_DROPFILES:
    {
      char FileName[PATH_MAX] = {0};
      UINT NumFiles = 0;
      int i;
      hDropInfo = (HDROP)wParam;
      NumFiles = DragQueryFile (hDropInfo, 0xFFFFFFFF, NULL, PATH_MAX);

      if(NumFiles > 0) {
        for (i=0; i<NumFiles; i++) {
          (LPARAM)DragQueryFile (hDropInfo, i, FileName, PATH_MAX);
          SendMessage(hPList, LB_ADDSTRING, 0, (LPARAM)FileName);
        }
      }
      return 0;
    }
    case WM_COMMAND:
    {
      /* Load a module */
      if(LOWORD(wParam) == 2000)
      {
        OPENFILENAMEA ofn_mod = {0};
        char FileName[PATH_MAX] = {0};
        int err = 0;

        ofn_mod.lStructSize = sizeof(OPENFILENAME);
        ofn_mod.hwndOwner = NULL;
        ofn_mod.lpstrFile = FileName;
        ofn_mod.lpstrFile[0] = '\0';
        ofn_mod.nMaxFile = sizeof(FileName);
        ofn_mod.lpstrFilter = "Modules\0"MOD_EXT"\0All Files\0*.*\0";
        ofn_mod.nFilterIndex = 1;
        ofn_mod.lpstrFileTitle = NULL;
        ofn_mod.nMaxFileTitle = 0 ;
        ofn_mod.lpstrInitialDir = NULL;
        ofn_mod.lpstrTitle="w3mikmod - Open Module";
        ofn_mod.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
        err = GetOpenFileNameA(&ofn_mod);
        if(err != 0) {
          ModuleLoad(FileName, isPlaying);
        }
      }

      /* Rewind */
      if(LOWORD(wParam) == 2001)
      {
        if(module) {
          Player_SetPosition(0);
          TimeCount=0;
        }
      }

      /* Play */
      if(LOWORD(wParam) == 2002)
      {
        SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2003, (LPARAM)0);
        if(!isPlaying) {
          if(Player_Paused() && isStarted) {
            Player_TogglePause();
            isPlaying = 1;
          } else {
            MikMod_EnableOutput();
            Player_SetPosition(0);
            Player_Start(module);
            TimeCount=0;
            MikMod_Update();
            isPlaying = 1;
            isStarted = 1;
          }
        }
      }

      /* Pause */
      if(LOWORD(wParam) == 2003)
      {
        /* We don't have anything to play */
        if(!module || (isStarted == 0 && isLoaded == 0)) break;

        /* Just loaded, pause enabled */
        if(isLoaded == 1 && isStarted == 0 && isPlaying == 0) {
          MikMod_EnableOutput();
          isPlaying = 1;
          isStarted = 1;
          MikMod_Update();
          Player_SetPosition(0);
          Player_Start(module);
          MikMod_Update();
          SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2003, (LPARAM)0);
          break;
        }

        /* Standard pause toggle */
        if(isPlaying) {
          if(!Player_Paused()) Player_TogglePause();
          SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2003, (LPARAM)1);
          isPlaying = 1 - isPlaying;
        } else {
          if(Player_Paused()) Player_TogglePause();
          SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2003, (LPARAM)0);
          MikMod_Update();
          isPlaying = 1 - isPlaying;
        }
      }
      /* Samples */
      if(LOWORD(wParam) == 2004)
      {
        SwitchSamplesBTN();
      }

      /* instruments */
      if(LOWORD(wParam) == 2005)
      {
        SwitchInstrBTN();
      }

      /* volume bars */
      if(LOWORD(wParam) == 2009)
      {
        SwitchVolBTN();
      }

      /* Playlist prev */
      if(LOWORD(wParam) == 2006)
      {
        int PlayListCount=SendMessage(hPList, LB_GETCOUNT, 0, 0);

        if(PlayListCount > 0) {
          PlayListPrev(PlayListCount, isPlaying);
        }
      }

      /* Playlist next */
      if(LOWORD(wParam) == 2007)
      {
        int PlayListCount=SendMessage(hPList, LB_GETCOUNT, 0, 0);
        if(PlayListCount > 0) {
          PlayListNext(PlayListCount, isPlaying);
        }
      }

      /* About */
      if(LOWORD(wParam) == 2008)
      {
        char AboutMsg[1024];
        char *InfoDriver;
        InfoDriver = MikMod_InfoDriver();

        snprintf(&AboutMsg, 1024,
          "W3Mikmod %s\r\n(c) 2021 Toyoyo\r\n\r\nLibMikmod %d.%d.%d\r\n(c) 2004 Raphael Assenat and others\r\n\r\n"
          "Driver info:\r\n%s\r\n"
          "Mix Frequency: %d\r\n\r\n"
          "Key usage:\r\n"
          "H: This help\r\n"
          "O: Open module\r\n"
          "G: Play\r\n"
          "S: Toggle samples window\r\n"
          "I: Toggle Intruments window\r\n"
          "V: Toggle volume bars window\r\n"
          "+: Add module to playlist\r\n"
          "-: Remove selected module(s) from playlist\r\n"
          "L: Load selected module\r\n"
          "N: Next\r\n"
          "P: Previous\r\n"
          "R: Rewind\r\n"
          "Space: Pause\r\n\r\n"
          "This program is free software,\r\ncovered by the GNU GPL.\r\n"
          , W3MIDMOKVERSION, (MikMod_GetVersion() >> 16) & 255, (MikMod_GetVersion() >>  8) & 255, (MikMod_GetVersion()) & 255,
          InfoDriver, md_mixfreq);

          MikMod_free(InfoDriver);

          MessageBox(NULL, AboutMsg, "W3MikMod - About", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
          SetForegroundWindow(hWnd);
      }

      /* Playlist Load */
      if(LOWORD(wParam) == 3103)
      {
        int SelCount=0;
        int SelInBuffer;
        int i;
        int *SelList;
        char PLSelect[PATH_MAX];

        SelCount = SendMessage(hPList, LB_GETSELCOUNT, 0, 0);
        SelList = malloc(sizeof(int)*SelCount);

        if(SelCount != LB_ERR && SelCount == 1) {
          SelInBuffer = SendMessage(hPList,LB_GETSELITEMS, SelCount, (LPARAM) SelList);
          for (i = SelInBuffer - 1; i >= 0; i--) {
            SendMessage(hPList, LB_GETTEXT, SelList[i], (LPARAM) &PLSelect);
          }
          PlayListPos=SendMessage(hPList, LB_GETCURSEL, 0, 0);
          ModuleLoad(PLSelect, isPlaying);
        }
        free(SelList);
      }

      /* Playlist remove */
      if(LOWORD(wParam) == 3102)
      {
        int SelCount=0;
        int SelInBuffer;
        int i;
        int *SelList;

        SelCount = SendMessage(hPList, LB_GETSELCOUNT, 0, 0);
        SelList = malloc(sizeof(int)*SelCount);

        if(SelCount != LB_ERR) {
            SelInBuffer = SendMessage(hPList,LB_GETSELITEMS, SelCount, (LPARAM) SelList);
          for (i = SelInBuffer - 1; i >= 0; i--) {
            SendMessage(hPList, LB_DELETESTRING, SelList[i], 0);
            if(SelList[i] <= PlayListPos) PlayListPos--;
          }
        }
        free(SelList);
      }

      /* Playlist add */
      if(LOWORD(wParam) == 3101)
      {
        OPENFILENAMEA ofn_mod = {0};
        char FileSelection[4096];
        char SelFileName[PATH_MAX];
        char SelDirName[PATH_MAX];
        char *token;
        int Pass = 0;
        int err = 0;

        ofn_mod.lStructSize = sizeof(OPENFILENAME);
        ofn_mod.hwndOwner = NULL;
        ofn_mod.lpstrFile = FileSelection;
        ofn_mod.lpstrFile[0] = '\0';
        ofn_mod.nMaxFile = sizeof(FileSelection);
        ofn_mod.lpstrFilter = "Modules\0"MOD_EXT"\0All Files\0*.*\0";
        ofn_mod.nFilterIndex = 1;
        ofn_mod.lpstrFileTitle = NULL;
        ofn_mod.nMaxFileTitle = 0 ;
        ofn_mod.lpstrInitialDir = NULL;
        ofn_mod.lpstrTitle="w3mikmod - Add Module";
        ofn_mod.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;
        err = GetOpenFileNameA(&ofn_mod);

        if(err != 0) {
          if(strchr(FileSelection, ' ')) {
            token = strtok(FileSelection, " ");
            while(token != NULL) {
              if (Pass == 0) {
                snprintf(SelDirName, PATH_MAX, "%s", token);
              }
              else {
                snprintf(SelFileName, PATH_MAX, "%s\\%s", SelDirName, token);
                SendMessage(hPList, LB_ADDSTRING, 0, (LPARAM)SelFileName);
              }
              Pass++;
              token = strtok(NULL, " ");
            }
          } else {
            SendMessage(hPList, LB_ADDSTRING, 0, (LPARAM)FileSelection);
          }
        }
      }
    }
  }

  return (DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK fnWndProcSamples(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;
      hSamplesStatic = CreateWindowEx(0,"Static",NOMOD, WS_CHILD|WS_VISIBLE, 0, 0, 1280, 1024, hWnd,(HMENU)3000,hIns,0);
      SendMessage(hSamplesStatic, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      return 0;
    }
    case WM_CLOSE:
    {
      SwitchSamplesBTN();
      return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
      SetBkMode((HDC)wParam,TRANSPARENT);
      SetTextColor((HDC)wParam, RGB(255,255,255));
      return GetWindowLong(hWnd,0*sizeof(void*));
    }
    case WM_SIZE:
    {
      if(LOWORD(wParam) == 0) {
        UpdateSamples();
      }
    }
    case WM_DESTROY:
    {
      return 0;
    }
  }
  return (DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK fnWndProcVol(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;
      hVolStatic = CreateWindowEx(0,"Static",NOMOD, WS_CHILD|WS_VISIBLE, 0, 0, 1280, 1024, hWnd,(HMENU)3000,hIns,0);
      SendMessage(hVolStatic, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      return 0;
    }
    case WM_CLOSE:
    {
      SwitchVolBTN();
      return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
      SetBkMode((HDC)wParam,TRANSPARENT);
      SetTextColor((HDC)wParam, RGB(255,255,255));
      return GetWindowLong(hWnd,0*sizeof(void*));
    }
    case WM_SIZE:
    {
      if(LOWORD(wParam) == 0) {
        UpdateVol(TRUE);
      }
    }
    case WM_DESTROY:
    {
      return 0;
    }
  }
  return (DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK fnWndProcInstr(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hIns = ((LPCREATESTRUCT)lParam)->hInstance;
      hInstrStatic = CreateWindowEx(0,"Static",NOMOD, WS_CHILD|WS_VISIBLE, 0, 0, 1280, 1024, hWnd,(HMENU)5000,hIns,0);
      SendMessage(hInstrStatic, WM_SETFONT, (WPARAM)s_hFont, (LPARAM)MAKELONG(TRUE, 0));
      return 0;
    }
    case WM_CLOSE:
    {
      SwitchInstrBTN();
      return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
      SetBkMode((HDC)wParam,TRANSPARENT);
      SetTextColor((HDC)wParam, RGB(255,255,255));
      return GetWindowLong(hWnd,0*sizeof(void*));
    }
    case WM_SIZE:
    {
      if(LOWORD(wParam) == 0) {
        UpdateInstr();
      }
    }
    case WM_DESTROY:
    {
      return 0;
    }
  }
  return (DefWindowProc(hWnd, msg, wParam, lParam));
}

void CALLBACK Timer0Proc(HWND hWnd, unsigned int msg, unsigned int idTimer, DWORD dwTime)
{
  int PlayListCount=SendMessage(hPList, LB_GETCOUNT, 0, 0);

  if(Player_Paused() && isStarted) UpdateVol(FALSE);

  if(isPlaying) {
    PlayListCount=SendMessage(hPList, LB_GETCOUNT, 0, 0);
    if(Player_Active()) {
      MikMod_Sleep(100);
      MikMod_EnableOutput();
      MikMod_Update();
      UpdateStatus();
      UpdateVol(FALSE);
      TimeCount++;
    } else {
      if(PlayListCount == 0 || PlayListCount == LB_ERR) {
        /* No Playlist, just looping */
        Player_SetPosition(0);
      } else {
        /* We have a Playlist */
        PlayListNext(PlayListCount, 1);
      }
    }
  }
}

void PlayListNext(int PlayListCount, int Start) {
  char PlayListItem[MAX_PATH];

  PlayListPos++;
  if (PlayListPos >= PlayListCount) PlayListPos = 0;

  SendMessage(hPList, LB_GETTEXT, PlayListPos, (long)&PlayListItem);
  ModuleLoad(PlayListItem, isPlaying);
}

void PlayListPrev(int PlayListCount, int isPlaying) {
  char PlayListItem[MAX_PATH];

  PlayListPos--;
  if (PlayListPos < 0) PlayListPos = 0;

  SendMessage(hPList, LB_GETTEXT, PlayListPos, (long)&PlayListItem);
  ModuleLoad(PlayListItem, isPlaying);
}

void UpdateSamples(void) {
  if(module && !IsIconic(hSamplesWin)) {
    char SamplesMsg[1024] = {0};
    char* SamplesMsgDisp;
    int Lines = 0;
    int MaxWidth = 0;
    int CurWidth = 0;
    int TitleBarSize = 0;
    int t = 0;
    int c = 0;

    for(t = 0; t < module->numsmp; t++) {
      if (strlen(strchr(&SamplesMsg, '\0')) < 768)
        if((!CfgEmpty && strncmp(module->samples[t].samplename, "", 255)) || CfgEmpty)
          snprintf(strchr(&SamplesMsg, '\0'), 255, "%02d: %s\n", t, module->samples[t].samplename);
    }
    if(!strlen(&SamplesMsg)) sprintf(&SamplesMsg, "No non-empty samples\r\n");

    /* '&' have a special effect on winapi controls */
    SamplesMsgDisp=ReplStr(SamplesMsg, "&", "&&");
    SetWindowText(hSamplesStatic, SamplesMsgDisp);

    for(c=0; SamplesMsg[c]!='\0'; c++) {
      CurWidth++;
      if(SamplesMsg[c] == '\n') {
        if(CurWidth > MaxWidth) MaxWidth = CurWidth;
        Lines++;
        CurWidth=0;
      }
    }

    TitleBarSize=GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER);
    SetWindowPos(hSamplesWin, HWND_BOTTOM, 0, 0 , MaxWidth*FontX, TitleBarSize+(Lines*FontY),
      SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    SetWindowPos(hSamplesStatic, HWND_BOTTOM, 0, 0, MaxWidth*FontX, Lines*FontY, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    RedrawWindow(hSamplesWin, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  }
}

void UpdateInstr(void) {
  if(module && !IsIconic(hInstrWin)) {
    char InstrMsg[1024] = {0};
    char* InstrMsgDisp;
    int Lines = 0;
    int MaxWidth = 0;
    int CurWidth = 0;
    int TitleBarSize = 0;
    int t = 0;
    int c = 0;

    if(module->flags & UF_INST) {
      for(t = 0; t < module->numins ; t++) {
        if (strlen(strchr(&InstrMsg, '\0')) < 768) {
          if((!CfgEmpty && strncmp(module->instruments[t].insname, "", 255)) || CfgEmpty)
            snprintf(strchr(&InstrMsg, '\0'), 255, "%02d: %s\n", t, module->instruments[t].insname);
        }
      }
    } else {
      snprintf(&InstrMsg, 255, "Module only have samples\n");
    }

    if(!strlen(&InstrMsg)) sprintf(&InstrMsg, "No non-empty instruments\r\n");

    /* '&' have a special effect on winapi controls */
    InstrMsgDisp=ReplStr(InstrMsg, "&", "&&");
    SetWindowText(hInstrStatic, InstrMsgDisp);

    for(c=0; InstrMsg[c]!='\0'; c++) {
      CurWidth++;
      if(InstrMsg[c] == '\n') {
        if(CurWidth > MaxWidth) MaxWidth = CurWidth;
        Lines++;
        CurWidth=0;
      }
    }

    TitleBarSize=GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER);
    SetWindowPos(hInstrWin, HWND_BOTTOM, 0, 0 , MaxWidth*FontX, TitleBarSize+(Lines*FontY),
      SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    SetWindowPos(hInstrStatic, HWND_BOTTOM, 0, 0, MaxWidth*FontX, Lines*FontY, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
    RedrawWindow(hInstrWin, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
  }
}

void UpdateVol(BOOL UpdateChn) {
  if(module && !IsIconic(hVolWin)) {
    char VolMsg[1024] = {0};
    int Lines = 0;
    int MaxWidth = 0;
    int CurWidth = 0;
    int TitleBarSize = 0;
    int t = 0;
    int c = 0;
    int ChanVol = 0;
    HDC hdc;
    static VOICEINFO vi[MAXVOICES];

    for (t = 0; t < module->numchn; t++) {
      if (strlen(strchr(&VolMsg, '\0')) < 768)
        snprintf(strchr(&VolMsg, '\0'), 255, "%02d: \n", t);
    }

    for(c=0; VolMsg[c]!='\0'; c++) {
      CurWidth++;
      if(VolMsg[c] == '\n') {
        if(CurWidth > MaxWidth) MaxWidth = CurWidth;
        Lines++;
        CurWidth=0;
      }
    }

    if(UpdateChn) {
      SetWindowText(hVolStatic, &VolMsg);
      TitleBarSize=GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYBORDER);
      SetWindowPos(hVolWin, HWND_BOTTOM, 0, 0 , FontX*4+101+GetSystemMetrics(SM_CXSIZEFRAME)+GetSystemMetrics(SM_CXBORDER), TitleBarSize+(Lines*FontY),
        SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
      SetWindowPos(hVolStatic, HWND_BOTTOM, 0, 0, FontX*4, 1024, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
      RedrawWindow(hVolWin, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
    }

    hdc = GetDC(hVolWin);
    Player_QueryVoices(module->numchn, vi);
    for (t = 0; t < module->numchn; t++) {
      if (vi[t].volume > 0) {
        ChanVol = Voice_RealVolume(Player_GetChannelVoice(t))*100/65535;
      } else {
        ChanVol = 0;
      }
      SelectObject(hdc, hBrushVol);
      Rectangle(hdc, FontX*4, FontY*t, FontX*4+100, FontY+FontY*t);
      SelectObject(hdc, hBrushGreen);
      if(ChanVol > 50) SelectObject(hdc, hBrushYellow);
      if(ChanVol > 80) SelectObject(hdc, hBrushRed);
      Rectangle(hdc, FontX*4, FontY*t, FontX*4+ChanVol, FontY+FontY*t);
    }
    ReleaseDC(hVolWin, hdc);
  }
}

void ModuleLoad(char *ModFN, int Play) {
  if(module) {
    Player_Stop();
    Player_Free(module);
  }

  module = Player_Load(ModFN, 64, 0);
  if(module) {
    UpdateSamples();
    UpdateInstr();
    UpdateVol(TRUE);
    module->loop = 0;

    isLoaded=1;
    if(Play == 0) {
      MikMod_DisableOutput();
      isPlaying = 0;
      isStarted = 0;
      MikMod_Update();
      Player_SetPosition(0);
    } else {
      MikMod_EnableOutput();
      isPlaying = 1;
      isStarted = 1;
      MikMod_Update();
      Player_SetPosition(0);
      Player_Start(module);
      MikMod_Update();
    }
    TimeCount=0;
    SetWindowText(hSongName, module->songname);
    SetWindowText(hTrackerType, module->modtype);
    SetWindowText(hFileName, ModFN);
    UpdateStatus();
  }
}

void UpdateStatus(void) {
  int PlayListCount=SendMessage(hPList, LB_GETCOUNT, 0, 0);
  char Status[255] = {0};
  char OldStatus[255];

  GetWindowText(hStatus, OldStatus, 255);

  snprintf(&Status, 255, "pat:%03d/%03d pos:%2.2X P:%d/%d time:%d",
  module->sngpos, module->numpos-1, module->patpos, PlayListPos+1, PlayListCount, TimeCount/10);

  /* Only updating if changed, to reduce potential flickering */
  if(strncmp(&Status, &OldStatus, 255)) SetWindowText(hStatus, &Status);
}

void SwitchSamplesBTN() {
  int WinVisible=IsWindowVisible(hSamplesWin);
  SendMessage(hToolbar, TB_CHANGEBITMAP, (WPARAM)2004, (LPARAM)4+6*(1-WinVisible));
  SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2004, (LPARAM)1-WinVisible);
  ShowWindow(hSamplesWin, 5*(1-WinVisible));
}

void SwitchInstrBTN() {
  int WinVisible=IsWindowVisible(hInstrWin);
  SendMessage(hToolbar, TB_CHANGEBITMAP, (WPARAM)2005, (LPARAM)5+6*(1-WinVisible));
  SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2005, (LPARAM)1-WinVisible);
  ShowWindow(hInstrWin, 5*(1-WinVisible));
}

void SwitchVolBTN() {
  int WinVisible=IsWindowVisible(hVolWin);
  SendMessage(hToolbar, TB_CHANGEBITMAP, (WPARAM)2009, (LPARAM)6+6*(1-WinVisible));
  SendMessage(hToolbar, TB_CHECKBUTTON, (WPARAM)2009, (LPARAM)1-WinVisible);
  ShowWindow(hVolWin, 5*(1-WinVisible));
}

char *ReplStr(const char *str, const char *from, const char *to) {
        /* https://creativeandcritical.net/str-replace-c */
	/* Adjust each of the below values to suit your needs. */

	/* Increment positions cache size initially by this number. */
	size_t cache_sz_inc = 16;
	/* Thereafter, each time capacity needs to be increased,
	 * multiply the increment by this factor. */
	const size_t cache_sz_inc_factor = 3;
	/* But never increment capacity by more than this number. */
	const size_t cache_sz_inc_max = 1048576;

	char *pret, *ret = NULL;
	const char *pstr2, *pstr = str;
	size_t i, count = 0;
	uintptr_t *pos_cache_tmp, *pos_cache = NULL;
	size_t cache_sz = 0;
	size_t cpylen, orglen, retlen, tolen, fromlen = strlen(from);

	/* Find all matches and cache their positions. */
	while ((pstr2 = strstr(pstr, from)) != NULL) {
		count++;

		/* Increase the cache size when necessary. */
		if (cache_sz < count) {
			cache_sz += cache_sz_inc;
			pos_cache_tmp = realloc(pos_cache, sizeof(*pos_cache) * cache_sz);
			if (pos_cache_tmp == NULL) {
				goto end_repl_str;
			} else pos_cache = pos_cache_tmp;
			cache_sz_inc *= cache_sz_inc_factor;
			if (cache_sz_inc > cache_sz_inc_max) {
				cache_sz_inc = cache_sz_inc_max;
			}
		}

		pos_cache[count-1] = pstr2 - str;
		pstr = pstr2 + fromlen;
	}

	orglen = pstr - str + strlen(pstr);

	/* Allocate memory for the post-replacement string. */
	if (count > 0) {
		tolen = strlen(to);
		retlen = orglen + (tolen - fromlen) * count;
	} else	retlen = orglen;
	ret = malloc(retlen + 1);
	if (ret == NULL) {
		goto end_repl_str;
	}

	if (count == 0) {
		/* If no matches, then just duplicate the string. */
		strcpy(ret, str);
	} else {
		/* Otherwise, duplicate the string whilst performing
		 * the replacements using the position cache. */
		pret = ret;
		memcpy(pret, str, pos_cache[0]);
		pret += pos_cache[0];
		for (i = 0; i < count; i++) {
			memcpy(pret, to, tolen);
			pret += tolen;
			pstr = str + pos_cache[i] + fromlen;
			cpylen = (i == count-1 ? orglen : pos_cache[i+1]) - pos_cache[i] - fromlen;
			memcpy(pret, pstr, cpylen);
			pret += cpylen;
		}
		ret[retlen] = '\0';
	}

end_repl_str:
	/* Free the cache and return the post-replacement string,
	 * which will be NULL in the event of an error. */
	free(pos_cache);
	return ret;
}

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
  /* Thanks, random reddit post */
  RECT rcClient, rcWind;
  POINT ptDiff;
  GetClientRect(hWnd, &rcClient);
  GetWindowRect(hWnd, &rcWind);
  ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
  ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
  MoveWindow(hWnd,rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}
