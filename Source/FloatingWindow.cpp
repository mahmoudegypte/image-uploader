// This file was generated by WTL subclass control wizard 
// FloatingWindow.cpp : Implementation of FloatingWindow

#include "stdafx.h"
#include "FloatingWindow.h"

// FloatingWindow
CFloatingWindow::CFloatingWindow()
{
	EnableClicks = true;
	hMutex = NULL;
	m_PrevActiveWindow = 0;
	m_bStopCapturingWindows = false;
	WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
}

CFloatingWindow::~CFloatingWindow()
{
	CloseHandle(hMutex);
	DeleteObject(m_hIconSmall);
	m_hWnd = 0;
}

LRESULT CFloatingWindow::OnClose(void)
{
	return 0;
}

bool MyInsertMenu(HMENU hMenu, int pos, UINT id, const LPCTSTR szTitle,  HBITMAP bm=NULL)
{
	MENUITEMINFO MenuItem;
	 
	MenuItem.cbSize = sizeof(MenuItem);
	if(szTitle)
	MenuItem.fType = MFT_STRING;
	else MenuItem.fType = MFT_SEPARATOR;
	MenuItem.fMask = MIIM_TYPE	| MIIM_ID | MIIM_DATA;
	if(bm)
		MenuItem.fMask |= MIIM_CHECKMARKS;
	MenuItem.wID = id;
	MenuItem.hbmpChecked = bm;
	MenuItem.hbmpUnchecked = bm;
	MenuItem.dwTypeData = (LPWSTR)szTitle;
	return InsertMenuItem(hMenu, pos, TRUE, &MenuItem);
}

LRESULT CFloatingWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(m_hIconSmall, FALSE);

	RegisterHotkeys();
	InstallIcon(APPNAME,m_hIconSmall,/*TrayMenu*/0);
	return 0;
}

LRESULT CFloatingWindow::OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pWizardDlg->CloseWizard();
	return 0;
}

LRESULT CFloatingWindow::OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(!EnableClicks ) return 0;
	if (LOWORD(lParam) == WM_LBUTTONDOWN)
	{
		m_bStopCapturingWindows = true;
	}
	if (LOWORD(lParam) == WM_MOUSEMOVE)
	{
		if(!m_bStopCapturingWindows)
		{
			HWND wnd =  GetForegroundWindow();
			if(wnd != m_hWnd)
			m_PrevActiveWindow = GetForegroundWindow();
		}
	}
	if (LOWORD(lParam) == WM_RBUTTONUP)
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(Settings.Hotkeys[Settings.TrayIconSettings.RightClickCommand].commandId,0));
	}
	else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{
		EnableClicks = false;
		KillTimer(1);
		SetTimer(2, GetDoubleClickTime());
		SendMessage(WM_COMMAND, MAKEWPARAM(Settings.Hotkeys[Settings.TrayIconSettings.LeftDoubleClickCommand].commandId,0));
	}
	else if (LOWORD(lParam) == WM_LBUTTONUP)
	{
		m_bStopCapturingWindows = false;
		if(!Settings.Hotkeys[Settings.TrayIconSettings.LeftDoubleClickCommand].commandId)
			SendMessage(WM_COMMAND, MAKEWPARAM(Settings.Hotkeys[Settings.TrayIconSettings.LeftClickCommand].commandId,0));
		else
			SetTimer(1, (UINT) (1.2*GetDoubleClickTime()));
	}
	else if (LOWORD(lParam) == WM_MBUTTONUP)
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(Settings.Hotkeys[Settings.TrayIconSettings.MiddleClickCommand].commandId,0));
	}
	return 0;
}

LRESULT CFloatingWindow::OnMenuSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(!pWizardDlg->IsWindowEnabled()) return 0;
	CSettingsDlg dlg(4);
	dlg.DoModal(pWizardDlg->m_hWnd);
	return 0;
}

LRESULT CFloatingWindow::OnCloseTray(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	pWizardDlg->ShowWindow(SW_SHOW);
	pWizardDlg->SetDlgItemText(IDCANCEL, TR("�����"));
	CloseHandle(hMutex);
	RemoveIcon();
	UnRegisterHotkeys();
	DestroyWindow();
	hMutex = NULL;
	m_hWnd = 0;
	return 0;
}

LRESULT CFloatingWindow::OnReloadSettings(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!lParam)
	UnRegisterHotkeys();

	if(!wParam)
		Settings.LoadSettings();

	if(!lParam)
	RegisterHotkeys();
	return 0;
}

LRESULT CFloatingWindow::OnImportvideo(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("importvideo")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnUploadFiles(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("addfiles")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnUploadImages(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("addimages")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnScreenshotDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("screenshotdlg")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnRegionScreenshot(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pWizardDlg->executeFunc(_T("regionscreenshot_dontshow"));
	return 0;
}

LRESULT CFloatingWindow::OnFullScreenshot(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	pWizardDlg->executeFunc(_T("fullscreenshot"));
	return 0;
}

LRESULT CFloatingWindow::OnWindowScreenshot(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(m_PrevActiveWindow) SetForegroundWindow(m_PrevActiveWindow);
	if(pWizardDlg->executeFunc(_T("windowscreenshot_delayed")));

	return 0;
}

LRESULT CFloatingWindow::OnAddFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("addfolder")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnShowAppWindow(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->IsWindowEnabled())
		pWizardDlg->ShowWindow(SW_SHOWNORMAL);
	else if(pWizardDlg->IsWindowVisible())
		pWizardDlg->SetActiveWindow();
	return 0;
}

LRESULT CFloatingWindow::OnContextMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(!IsWindowEnabled()) return 0;

	HMENU TrayMenu = ::CreatePopupMenu();

	// Inserting menu items
	int i = 0;
	MyInsertMenu(TrayMenu,i++,IDM_UPLOADFILES,TR("��������� �����")+CString(_T("...")));
	MyInsertMenu(TrayMenu,i++,IDM_ADDFOLDER,TR("��������� �����")+CString(_T("...")));
	MyInsertMenu(TrayMenu,i++,0,0);
	bool IsClipboard=false;

	if(OpenClipboard())
	{
		IsClipboard = IsClipboardFormatAvailable(CF_BITMAP);
		CloseClipboard();
	}
	if(IsClipboard)
	{
		MyInsertMenu(TrayMenu,i++,IDM_PASTEFROMCLIPBOARD,TR("�������� �� ������"));
		MyInsertMenu(TrayMenu,i++,0,0);
	}
	MyInsertMenu(TrayMenu,i++,IDM_IMPORTVIDEO ,TR("������ �����"));
	MyInsertMenu(TrayMenu,i++,0,0);
	MyInsertMenu(TrayMenu,i++,IDM_SCREENSHOTDLG,TR("��������")+CString(_T("...")));
	MyInsertMenu(TrayMenu,i++,IDM_REGIONSCREENSHOT,TR("�������� ���������� �������"));
	MyInsertMenu(TrayMenu,i++,IDM_FULLSCREENSHOT,TR("�������� ����� ������"));
	MyInsertMenu(TrayMenu,i++,IDM_WINDOWSCREENSHOT,TR("�������� �������� ����"));

	MyInsertMenu(TrayMenu,i++,0,0);
	MyInsertMenu(TrayMenu,i++,IDM_SHOWAPPWINDOW,TR("�������� ���� ���������"));

	MyInsertMenu(TrayMenu,i++,0,0);
	MyInsertMenu(TrayMenu,i++,IDM_SETTINGS,TR("���������")+CString(_T("...")));
	MyInsertMenu(TrayMenu,i++,0,0);
	MyInsertMenu(TrayMenu,i++,IDM_EXIT,TR("�����"));
	m_hTrayIconMenu = TrayMenu;
	
	if(Settings.Hotkeys[Settings.TrayIconSettings.LeftDoubleClickCommand].commandId)
	{
		SetMenuDefaultItem(TrayMenu, Settings.Hotkeys[Settings.TrayIconSettings.LeftDoubleClickCommand].commandId, false);
	}

	CMenuHandle oPopup(m_hTrayIconMenu);
	PrepareMenu(oPopup);
	CPoint pos;
	GetCursorPos(&pos);
	SetForegroundWindow(m_hWnd);
	oPopup.TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, m_hWnd);
	// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
	PostMessage(WM_NULL);
	return 0;
}

LRESULT CFloatingWindow::OnTimer(UINT id)
{
	if(id == 1)
	{
		KillTimer(1);
		SendMessage(WM_COMMAND, MAKEWPARAM(Settings.Hotkeys[Settings.TrayIconSettings.LeftClickCommand].commandId,0));
	}
	if(id == 2)
		EnableClicks = true;

	KillTimer(id);
	return 0;
}
inline BOOL SetOneInstance(LPCTSTR szName)
{
	HANDLE hMutex=NULL;
	BOOL bFound=FALSE;
	hMutex = ::CreateMutex(NULL, TRUE, szName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		bFound = TRUE;
	if(hMutex)
		::ReleaseMutex(hMutex);
	return bFound;
}

CFloatingWindow floatWnd;

void CFloatingWindow::CreateTrayIcon()
{
    BOOL bFound=FALSE;
    hMutex = ::CreateMutex(NULL, TRUE, _T("ImageUploader_TrayWnd_Mutex"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(hMutex)
        ::ReleaseMutex(hMutex);
   
	 if(!bFound)
	 {
		CRect r(100,100,400,400);
		floatWnd.Create(0,r,_T("ImageUploader_TrayWnd"),WS_OVERLAPPED|WS_POPUP|WS_CAPTION );
		floatWnd.ShowWindow(SW_HIDE);
	 }
}

BOOL IsRunningFloatingWnd()
{
    HANDLE hMutex=NULL;
    BOOL bFound=FALSE;
    hMutex = ::CreateMutex(NULL, TRUE, _T("ImageUploader_TrayWnd_Mutex"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
	 if(hMutex){
        ::ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	 }
    return bFound;
}

void  CFloatingWindow::RegisterHotkeys()
{
	m_hotkeys=Settings.Hotkeys;

	for(int i =0; i< m_hotkeys.GetCount(); i++)
	{
		if(m_hotkeys[i].globalKey.keyCode)
		{
			RegisterHotKey(m_hWnd,i,m_hotkeys[i].globalKey.keyModifier,m_hotkeys[i].globalKey.keyCode);
		}
	}
}
LRESULT CFloatingWindow::OnHotKey(int HotKeyID, UINT flags, UINT vk)
{
	if(HotKeyID <0 || HotKeyID > m_hotkeys.GetCount()-1) return 0;

	if(m_hotkeys[HotKeyID].func == _T("windowscreenshot"))
	{
		pWizardDlg->executeFunc(_T("windowscreenshot"));
	}
	else
	{
		SetActiveWindow();
		SetForegroundWindow(m_hWnd);
		SendMessage(WM_COMMAND, MAKEWPARAM(m_hotkeys[HotKeyID].commandId,0));
	}
	return 0;
}


void  CFloatingWindow::UnRegisterHotkeys()
{
	for(int i =0; i< m_hotkeys.GetCount(); i++)
	{
		if(m_hotkeys[i].globalKey.keyCode)
		UnregisterHotKey(m_hWnd, i);
	}
	m_hotkeys.RemoveAll();
}
LRESULT CFloatingWindow::OnPaste(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("paste")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnMediaInfo(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(pWizardDlg->executeFunc(_T("mediainfo")))
		pWizardDlg->ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CFloatingWindow::OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	InstallIcon(APPNAME, m_hIconSmall, 0);
	return 0;
}
