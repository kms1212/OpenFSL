/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "EditorFrame.h"

void EditorFrame::InitializeComponents() {
    wxMenu *menuFile = new wxMenu;
//    menuFile->Append(ID_OPENIMGFILE, "&Open image file\tCtrl-O",
//            "Select image file to open.");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar(menuBar);
    
    // Create a wxGrid object
    editorTextBox = new wxTextCtrl(this, ID_EDITORTEXTBOX, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}