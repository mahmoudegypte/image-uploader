/*

    Image Uploader -  free application for uploading images/files to the Internet

    Copyright 2007-2018 Sergey Svistunov (zenden2k@yandex.ru)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

 */

#include "LangSelect.h"

#include "Gui/GuiTools.h"
#include "Func/WinUtils.h"
#include "Core/Settings.h"

// CLangSelect
CLangSelect::CLangSelect()
{
    findfile = NULL;
    memset(&wfd, 0, sizeof(wfd));
}

CLangSelect::~CLangSelect()
{
}

int CLangSelect::GetNextLngFile(LPTSTR szBuffer, int nLength)
{
    *wfd.cFileName = 0;

    if (!findfile)
    {
        findfile = FindFirstFile(WinUtils::GetAppFolder() + "Lang\\*.lng", &wfd);
        if (!findfile)
            goto error;
    }
    else if (!FindNextFile(findfile, &wfd))
        goto error;

    int nLen = lstrlen(wfd.cFileName);

    if (!nLen)
        goto error;
    lstrcpyn(szBuffer, wfd.cFileName, min(nLength, nLen + 1));

    return TRUE;

error:     // File not found
    if (findfile)
        FindClose(findfile);
    return FALSE;
}

LRESULT CLangSelect::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    langListCombo_ = GetDlgItem(IDC_LANGLIST);
    TCHAR buf[256];
    CString buf2;
    LogoImage.SubclassWindow(GetDlgItem(IDC_STATICLOGO));
    LogoImage.SetWindowPos(0, 0,0, 48, 48, SWP_NOMOVE|SWP_NOZORDER );
    LogoImage.LoadImage(0, 0, IDR_ICONMAINNEW, false, GetSysColor(COLOR_BTNFACE));
    
    GuiTools::MakeLabelBold(GetDlgItem(IDC_PLEASECHOOSE));

    // English language is always in language list
    langListCombo_.AddString(_T("English"));

    int n = 0;
    while (GetNextLngFile(buf, sizeof(buf) / sizeof(TCHAR)) )
    {
        if (buf[0] == '\0' || lstrcmpi(WinUtils::GetFileExt(buf), _T("lng")))
            continue;
        buf2 = WinUtils::GetOnlyFileName(buf);
        if (buf2 == _T("English")) {
            continue;
        }
        langListCombo_.AddString(buf2);
        n++;
    }

    if (!n)
    {
        EndDialog(IDOK);
        Language = _T("English");
        return 0;
    }

    // FIXME: detect system language and select corresponding language file
    if ( !Settings.Language.IsEmpty() ) {
            SelectLang(Settings.Language);
    } else  if (GetUserDefaultLangID() == 0x419) {
        SelectLang(_T("Russian"));
    } else if (GetUserDefaultLangID() == 0x0418) {
        SelectLang(_T("Romanian"));
    } else {
        SelectLang(_T("English"));
    }

    return 1;  // ��������� ������� �������������� ���������� ����� �����
}

LRESULT CLangSelect::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    int Index = langListCombo_.GetCurSel();
    if (Index < 0) {
        return 0;
    }
    langListCombo_.GetLBText(Index, Language);
    return EndDialog(wID);
}

LRESULT CLangSelect::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

void CLangSelect::SelectLang(LPCTSTR Lang)
{
    int Index = langListCombo_.FindStringExact(0, Lang);
    if (Index < 0) {
        return;
    }
    langListCombo_.SetCurSel(Index);
}
