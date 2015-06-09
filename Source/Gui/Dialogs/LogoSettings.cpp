/*

    Image Uploader -  free application for uploading images/files to the Internet

    Copyright 2007-2015 Sergey Svistunov (zenden2k@gmail.com)

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

#include "LogoSettings.h"
#include "atlheaders.h"
#include <uxtheme.h>
#include "LogWindow.h"
#include "Core/Settings.h"
#include "Gui/GuiTools.h"
#include "InputDialog.h"
#include "Gui/GuiTools.h"
#include "Func/WinUtils.h"

// CLogoSettings
CLogoSettings::CLogoSettings()
{
    ZeroMemory(&lf, sizeof(lf));
   m_CatchChanges = false;
   m_ProfileChanged = false;
}

void CLogoSettings::TranslateUI()
{
    TRC(IDC_CURRENTPROFILELABEL, "Profile:");
    TRC(IDC_FORMATLABEL, "Format:");
    TRC(IDC_QUALITYLABEL, "Quality:");
    TRC(IDC_RESIZEBYWIDTH, "Change width:");
    TRC(IDC_YOURLOGO, "Add watermark");
    TRC(IDC_SMARTCONVERTING, "Skip image processing step if the image has right parameters");
    //TRC(IDC_SAVEPROPORTIONS, "Constrain proportions");
    //TRC(IDC_YOURLOGO, "Add watermark");
    TRC(IDC_YOURTEXT, "Add text to image");
    TRC(IDC_XLABEL, "and/or height:");
    TRC(IDC_RESIZEMODELABEL, "Resize mode:");
    TRC(IDC_FILENAMELABEL, "Filename:");
    TRC(IDC_LOGOPOSITIONLABEL, "Watermark position:");
    TRC(IDC_LOGOGROUP, "Watermark");
    TRC(IDC_TEXTONIMAGEGROUP, "Text on image");
    TRC(IDC_ENTERYOURTEXTLABEL, "Enter your text:");
    TRC(IDC_TEXTCOLORLABEL, "Text color:");
    TRC(IDC_TEXTSTROKECOLOR, "Stroke color:");
    TRC(IDC_SELECTFONT, "Font...");
    TRC(IDC_TEXTPOSITIONLABEL, "Text position:");
    TRC(IDC_PRESERVE_EXIF, "Preserve EXIF data");
    SetWindowText(TR("Additional params"));    
}

CLogoSettings::~CLogoSettings()
{
        
}

LRESULT CLogoSettings::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   �onvert_profiles_ = Settings.ConvertProfiles;
    TabBackgroundFix(m_hWnd);
    // Translating controls
    TranslateUI();
    RECT rc = {13, 20, 290, 95};
    img.Create(GetDlgItem(IDC_LOGOGROUP), rc);
   img.ShowWindow(SW_HIDE);
    img.LoadImage(0);

   GuiTools::AddComboBoxItems(m_hWnd, IDC_RESIZEMODECOMBO, 3, TR("Fit"), TR("Center"), TR("Stretch"));
   GuiTools::AddComboBoxItems(m_hWnd, IDC_FORMATLIST, 4, TR("Auto"), _T("JPEG"), _T("PNG"),_T("GIF"));
  
    SendDlgItemMessage(IDC_TRANSPIN, UDM_SETRANGE, 0, (LPARAM) MAKELONG((short)100, (short)0) );
    SendDlgItemMessage(IDC_QUALITYSPIN,UDM_SETRANGE,0,(LPARAM) MAKELONG((short)100, (short)1));
    
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top left corner"));
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top center"));
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top right corner"));
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom left corner"));
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom center"));
    SendDlgItemMessage(IDC_LOGOPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom right corner"));

    SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top left corner"));
    SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top center"));
    SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Top right corner"));
   SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom left corner"));
   SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom center"));
   SendDlgItemMessage(IDC_TEXTPOSITION, CB_ADDSTRING, 0, (LPARAM)TR("Bottom right corner"));

   TextColor.SubclassWindow(GetDlgItem(IDC_SELECTCOLOR));


   StrokeColor.SubclassWindow(GetDlgItem(IDC_STROKECOLOR));

   CIcon ico = (HICON)LoadIcon(GetModuleHandle(0),MAKEINTRESOURCE(IDI_ICONWHITEPAGE));
   //LoadImage(GetModuleHandle(0),  MAKEINTRESOURCE(IDI_ICONWHITEPAGE), IMAGE_ICON    , 16,16,0);
   RECT profileRect;
   ::GetWindowRect(GetDlgItem(IDC_PROFILETOOBLARPLACEBUTTON), &profileRect);
   ::MapWindowPoints(0, m_hWnd, (LPPOINT)&profileRect, 2);

   m_ProfileEditToolbar.Create(m_hWnd,profileRect,_T(""), WS_CHILD|WS_VISIBLE|WS_CHILD | TBSTYLE_LIST |TBSTYLE_FLAT| TBSTYLE_TOOLTIPS|CCS_NORESIZE|/*CCS_BOTTOM |CCS_ADJUSTABLE|*/CCS_NODIVIDER|TBSTYLE_AUTOSIZE  );
    m_ProfileEditToolbar.SetExtendedStyle(TBSTYLE_EX_MIXEDBUTTONS);
   m_ProfileEditToolbar.SetButtonStructSize();
   m_ProfileEditToolbar.SetButtonSize(17,17);

   CIcon saveIcon = LoadIcon(GetModuleHandle(0),MAKEINTRESOURCE(IDI_ICONSAVE));
    CIcon deleteIcon = LoadIcon(GetModuleHandle(0),MAKEINTRESOURCE(IDI_ICONDELETE));
   CImageList list;
   list.Create(16,16,ILC_COLOR32 | ILC_MASK,0,6);
   list.AddIcon(ico);
   list.AddIcon(saveIcon);
    list.AddIcon(deleteIcon);
   m_ProfileEditToolbar.SetImageList(list);
   m_ProfileEditToolbar.AddButton(IDC_NEWPROFILE, TBSTYLE_BUTTON |BTNS_AUTOSIZE, TBSTATE_ENABLED, 0, TR("New Profile"), 0);
   m_ProfileEditToolbar.AddButton(IDC_SAVEPROFILE, TBSTYLE_BUTTON |BTNS_AUTOSIZE, TBSTATE_ENABLED, 1, TR("Save Profile"), 0);
    m_ProfileEditToolbar.AddButton(IDC_DELETEPROFILE, TBSTYLE_BUTTON |BTNS_AUTOSIZE,  TBSTATE_ENABLED, 2, TR("Delete Profile"), 0);

   ShowParams(Settings.imageServer.getImageUploadParams().ImageProfileName);
   UpdateProfileList();
   return 1; 
}

LRESULT CLogoSettings::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT CLogoSettings::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT CLogoSettings::OnBnClickedLogobrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    TCHAR Buf[MAX_PATH*4];
    GuiTools::SelectDialogFilter(Buf, sizeof(Buf)/sizeof(TCHAR),2, 
        CString(TR("Images"))+ _T(" (jpeg, bmp, png, gif ...)"),
        _T("*.jpg;*.gif;*.png;*.bmp;*.tiff"),
        TR("All files"),
        _T("*.*"));

    CFileDialog fd(true, 0, 0, 4|2, Buf, m_hWnd);
    
    CString s;
    s = WinUtils::GetAppFolder();
    fd.m_ofn.lpstrInitialDir = s;
    if ( fd.DoModal() != IDOK || !fd.m_szFileName ) return 0;

    LPTSTR FileName = fd.m_szFileName;
    SetDlgItemText(IDC_LOGOEDIT, FileName);
    img.LoadImage(FileName);
    img.Invalidate();
   ProfileChanged();
    return 0;
}

LRESULT CLogoSettings::OnBnClickedSelectfont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // Font selection dialog
    CFontDialog dlg(&lf);
    if(dlg.DoModal(m_hWnd) == IDOK)
       ProfileChanged();
    return 0;
}

LRESULT CLogoSettings::OnBnClickedThumbfont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // Font selection dialog
    CFontDialog dlg(&ThumbFont);
    dlg.DoModal(m_hWnd);

    return 0;
}

bool CLogoSettings::Apply()
{
   CString saveToProfile = CurrentProfileName;
   if(CurrentProfileOriginalName == _T("Default"))
    saveToProfile = CurrentProfileOriginalName;

   if(!SaveParams(�onvert_profiles_[saveToProfile]))
      return false;
   Settings.ConvertProfiles = �onvert_profiles_;
    //Settings.CurrentConvertProfileName  = saveToProfile;
    return TRUE;
}

LRESULT CLogoSettings::OnYourLogoCheckboxClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
   bool bChecked = SendDlgItemMessage(IDC_YOURLOGO, BM_GETCHECK) == BST_CHECKED;
    GuiTools::EnableNextN(hWndCtl, 5, bChecked);
    ProfileChanged();
   return 0;
}

LRESULT CLogoSettings::OnAddTextChecboxClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    bool bChecked = SendDlgItemMessage(IDC_YOURTEXT, BM_GETCHECK) == BST_CHECKED;
    GuiTools::EnableNextN(hWndCtl, 9, bChecked);
    ProfileChanged();
   return 0;
}

void CLogoSettings::ShowParams(const ImageConvertingParams& params)
{
   m_ProfileChanged = false;
   m_CatchChanges = false;
   SetDlgItemText(IDC_LOGOEDIT, params.LogoFileName);
    
    if(*params.LogoFileName) 
        img.LoadImage(params.LogoFileName);

    SetDlgItemText(IDC_EDITYOURTEXT,params.Text);
   SendDlgItemMessage(IDC_LOGOPOSITION, CB_SETCURSEL, params.LogoPosition);
    SendDlgItemMessage(IDC_TEXTPOSITION, CB_SETCURSEL, params.TextPosition);
    TextColor.SetColor(params.TextColor);
   StrokeColor.SetColor(params.StrokeColor);
   lf = params.Font;
   if(params.Quality)
        SetDlgItemInt(IDC_QUALITYEDIT,params.Quality);
    else
        SetDlgItemText(IDC_QUALITYEDIT,_T(""));
   SendDlgItemMessage(IDC_FORMATLIST,CB_SETCURSEL, params.Format);
   
    SendDlgItemMessage(IDC_RESIZEMODECOMBO,CB_SETCURSEL, (WPARAM)(int)(ImageConvertingParams::ImageResizeMode&)params.ResizeMode);
   
   SendDlgItemMessage(IDC_YOURLOGO,BM_SETCHECK,  params.AddLogo);
   SendDlgItemMessage(IDC_YOURTEXT,BM_SETCHECK,  params.AddText);

   GuiTools::SetCheck(m_hWnd, IDC_SMARTCONVERTING, params.SmartConverting);
   SetDlgItemText(IDC_IMAGEWIDTH,params.strNewWidth);

    SetDlgItemText(IDC_IMAGEHEIGHT,params.strNewHeight);

   OnYourLogoCheckboxClicked(0,0,GetDlgItem(IDC_YOURLOGO));
    OnAddTextChecboxClicked(0,0,GetDlgItem(IDC_YOURTEXT));
    GuiTools::SetCheck(m_hWnd, IDC_PRESERVE_EXIF, params.PreserveExifInformation);
   m_CatchChanges = true;
}

bool CLogoSettings::SaveParams(ImageConvertingParams& params)
{
    bool addLogo = SendDlgItemMessage(IDC_YOURLOGO,BM_GETCHECK) == BST_CHECKED;
    bool addText = SendDlgItemMessage(IDC_YOURTEXT,BM_GETCHECK) == BST_CHECKED;
    int LogoPos = SendDlgItemMessage(IDC_LOGOPOSITION, CB_GETCURSEL);
    int TextPos = SendDlgItemMessage(IDC_TEXTPOSITION, CB_GETCURSEL);
    
    if(LogoPos == TextPos && addLogo && addText) // ���� "������� ����" � ������� ���������� � ���� � �� �� ����� �� ��������
    {
        if(MessageBox(TR("Are you sure to place text and logo in the same position on image?"),TR("Image settings"),MB_ICONQUESTION|MB_YESNO)!=IDYES)
            return false;
    } 

    params.LogoPosition = LogoPos;
    params.TextPosition = TextPos;
   params.LogoFileName = GuiTools::GetWindowText(GetDlgItem(IDC_LOGOEDIT));
    params.Text = GuiTools::GetWindowText(GetDlgItem(IDC_EDITYOURTEXT));;
    params.Font = lf;
    params.AddLogo = addLogo;
   params.AddText = addText;

    GuiTools::GetCheck(m_hWnd, IDC_SMARTCONVERTING, params.SmartConverting);
    params.TextColor=TextColor.GetColor();
    params.StrokeColor=StrokeColor.GetColor();
   params.Quality = GetDlgItemInt(IDC_QUALITYEDIT);
    params.Format = SendDlgItemMessage(IDC_FORMATLIST, CB_GETCURSEL);
    params.ResizeMode = static_cast<ImageConvertingParams::ImageResizeMode>(SendDlgItemMessage(IDC_RESIZEMODECOMBO,CB_GETCURSEL));
   params.strNewWidth = GuiTools::GetWindowText( GetDlgItem(IDC_IMAGEWIDTH));
   params.strNewHeight =  GuiTools::GetWindowText( GetDlgItem(IDC_IMAGEHEIGHT));
   GuiTools::GetCheck(m_hWnd, IDC_PRESERVE_EXIF, params.PreserveExifInformation );
   return true;
}

 void CLogoSettings::UpdateProfileList()
 {
    SendDlgItemMessage(IDC_PROFILECOMBO, CB_RESETCONTENT);
    std::map<CString, ImageConvertingParams> ::const_iterator it;\
    bool found = false;
    for(it = �onvert_profiles_.begin(); it != �onvert_profiles_.end(); ++it)
    {
      GuiTools::AddComboBoxItem(m_hWnd, IDC_PROFILECOMBO, it->first);
      if(it->first == CurrentProfileName) found = true;
    }
    if(!found) GuiTools::AddComboBoxItem(m_hWnd, IDC_PROFILECOMBO, CurrentProfileName);
    SendDlgItemMessage(IDC_PROFILECOMBO, CB_SELECTSTRING, static_cast<WPARAM>(-1),(LPARAM)(LPCTSTR) CurrentProfileName); 
 }
 LRESULT CLogoSettings::OnSaveProfile(WORD wNotifyCode, WORD wID, HWND hWndCtl)
 {
    CInputDialog dlg(TR("New Profile Name"), TR("Enter new profile name:"), CurrentProfileOriginalName);
    if(dlg.DoModal()==IDOK)
    {
         ImageConvertingParams params;
         SaveParams(params);
         �onvert_profiles_[dlg.getValue()] = params;
         CurrentProfileName = dlg.getValue();
         CurrentProfileOriginalName = dlg.getValue();
         m_ProfileChanged = false;
         UpdateProfileList();
    }
    return 0;
 }

LRESULT CLogoSettings::OnProfileComboSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    if(m_ProfileChanged)
    {
        if(MessageBox(TR("Current profile's changes weren't saved. Do you want to continue?"), APPNAME, MB_YESNO|MB_ICONWARNING)!= IDYES) 
        {
            SendDlgItemMessage(IDC_PROFILECOMBO, CB_SELECTSTRING, static_cast<WPARAM>(-1),(LPARAM)(LPCTSTR) CurrentProfileName); 
            return 0;
        }
    }
    CString profile = GuiTools::GetWindowText(GetDlgItem(IDC_PROFILECOMBO));
    ShowParams(profile);
    UpdateProfileList();
    return 0;
}

void CLogoSettings::ShowParams(const CString profileName)
{
   if(CurrentProfileName == profileName) return;
    CurrentProfileName = profileName;
    CurrentProfileOriginalName = profileName; 
    ShowParams(�onvert_profiles_[profileName]);
    SendDlgItemMessage(IDC_PROFILECOMBO, CB_SELECTSTRING, -1,(LPARAM)(LPCTSTR) profileName); 
}

LRESULT CLogoSettings::OnProfileEditedCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    ProfileChanged();
    return 0;
}
    
void CLogoSettings::ProfileChanged()
{
    if(!m_CatchChanges) return;
    if(!m_ProfileChanged)
    {
        CurrentProfileOriginalName = CurrentProfileName;
        CurrentProfileName.Replace(CString(_T(" "))+TR("(edited)"), _T(""));
        CurrentProfileName = CurrentProfileName + _T(" ")+ TR("(edited)");
        m_ProfileChanged = true;
        UpdateProfileList();
    }
}

LRESULT CLogoSettings::OnProfileEditedNotification(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
   ProfileChanged();
   return 0;
}

LRESULT CLogoSettings::OnNewProfile(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    CString name = TR("New Profile");
    CString generatedName = name;
    int i = 1;
    while(�onvert_profiles_.count(generatedName) > 0)
    {
        generatedName = name  + _T(" ") + WinUtils::IntToStr(i++);
    }

    //CurrentProfileName = CurrentProfileOriginalName = generatedName;
    m_ProfileChanged = true;
    ShowParams(generatedName);
    UpdateProfileList();
    return 0;
}

LRESULT CLogoSettings::OnDeleteProfile(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    CString question;
    question.Format(TR("Are you sure you want to delete profile '%s'?"), CurrentProfileName);
    if(MessageBox(question,TR("Image settings"),MB_ICONQUESTION|MB_YESNO) != IDYES)
        return 0;
    if(CurrentProfileName=="Default") return 0;
    if(�onvert_profiles_.count(CurrentProfileName)>0)
        �onvert_profiles_.erase(CurrentProfileName);
    
    ShowParams("Default");
    UpdateProfileList();
    return 0;
}