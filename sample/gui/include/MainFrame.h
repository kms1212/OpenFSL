/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_MAINFRAME_H_
#define SAMPLE_MAINFRAME_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>

#include <openfsl/openfsl.h>

#include "utils.h"
#include "FAT32Command.h"
#include "ids.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
 
private:
    wxDataViewCtrl* dataViewCtrl;
    wxDataViewTextRenderer* renderer0;
    wxDataViewColumn* column0;
    wxListCtrl* listCtrl;
    wxFileDialog* openFileDialog;
    wxMenuBar *menuBar;
    wxSplitterWindow* mainSplitter;

    openfsl::FileBlockDevice fbd;
    FSCommand* fsCommand = nullptr;

    void InitializeComponents();
    void DeinitializeComponents();

    void OnFileListClick(wxListEvent& event);

    void OnOpenImgFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void InitializeFilesystem(wxString imagePath);

    void RefreshDirectoryList();
 
    wxDECLARE_EVENT_TABLE();
}; 

#endif  // SAMPLE_MAINFRAME_H_
