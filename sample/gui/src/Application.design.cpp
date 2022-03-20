/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "Application.h"

void MainFrame::InitializeComponents() {
    wxMenu *menuFile = new wxMenu;        
    menuFile->Append(ID_OPENIMGFILE, "&Open image file\tCtrl-O",
            "Select image file to open.");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar(menuBar);

    wxSplitterWindow* mainSplitter = new wxSplitterWindow(this, -1, wxPoint(-1,-1), wxSize(-1,-1), wxSP_LIVE_UPDATE);
    dataViewCtrl = new wxDataViewCtrl(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER);
    listCtrl = new wxListCtrl(mainSplitter, ID_FILELIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    
    mainSplitter->SplitVertically(dataViewCtrl, listCtrl);
    mainSplitter->SetMinimumPaneSize(230);

    wxDataViewTextRenderer* renderer0 = new wxDataViewTextRenderer(wxT("Name"));
    wxDataViewColumn* column0 = new wxDataViewColumn(wxT("Name"), renderer0, 0, 100, wxALIGN_CENTER, wxDATAVIEW_COL_RESIZABLE);
    dataViewCtrl->AppendColumn(column0);
    dataViewCtrl->SetExpanderColumn(column0);

    listCtrl->InsertColumn(0, wxT("Filename"));
    listCtrl->SetColumnWidth(0, 200);
    listCtrl->InsertColumn(1, wxT("Time modified"));
    listCtrl->SetColumnWidth(1, 150);
    listCtrl->InsertColumn(2, wxT("Size"));
    listCtrl->SetColumnWidth(2, 50);
    listCtrl->InsertColumn(3, wxT("Type"));
    listCtrl->SetColumnWidth(3, 80);
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}