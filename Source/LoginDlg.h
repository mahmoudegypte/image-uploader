/*
    Image Uploader - program for uploading images/files to Internet
    Copyright (C) 2007-2010 ZendeN <zenden2k@gmail.com>
	 
    HomePage:    http://zenden.ws/imageuploader

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

#pragma once
#include "maindlg.h"
#include "resource.h"       // main symbols
// CLoginDlg


LoginInfo LoadLogin(int ServerId);
class CLoginDlg : 
	public CDialogImpl<CLoginDlg>	
{
	public:
		int ServerId;
		CLoginDlg(int ServerId);
		~CLoginDlg();
		enum { IDD = IDD_LOGINDLG };

			BEGIN_MSG_MAP(CLoginDlg)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			//MESSAGE_HANDLER(WM_TIMER, OnTimer)
					COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
					COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
			 COMMAND_HANDLER(IDC_USEIECOOKIES, BN_CLICKED, OnClickedUseIeCookies)
		END_MSG_MAP()
			// Handler prototypes:
			//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
			//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnClickedUseIeCookies(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};


