/*
    Image Uploader - program for uploading images/files to Internet
    Copyright (C) 2007-2011 ZendeN <zenden2k@gmail.com>
	 
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

#ifndef DefaultServersSettings_H
#define DefaultServersSettings_H

#pragma once

#include "atlheaders.h"
#include "resource.h"       // main symbols
#include "Gui/Controls/MyImage.h"
#include "Gui/Dialogs/settingspage.h"
#include <Gui/COntrols/ServerSelectorControl.h>
class CServerSelectorControl;
class CDefaultServersSettings : public CDialogImpl<CDefaultServersSettings>, 
	                      public CSettingsPage	
{
	public:
		enum { IDD = IDD_DEFAULTSERVERSSETTINGS };

		CDefaultServersSettings();
		virtual ~CDefaultServersSettings();
		virtual bool Apply();

	protected:
		BEGIN_MSG_MAP(CDefaultServersSettings)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			MESSAGE_HANDLER(WM_SERVERSELECTCONTROL_SERVERLIST_CHANGED, OnServerListChanged)
			
		END_MSG_MAP()
		// Handler prototypes:
		//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		LRESULT OnServerListChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	CServerSelectorControl *imageServerSelector_;
	CServerSelectorControl *fileServerSelector_; 
	CServerSelectorControl *trayServerSelector_;
	CServerSelectorControl *contextMenuServerSelector_;
	CServerSelectorControl *urlShortenerServerSelector_;

};

#endif // DefaultServersSettings_H

