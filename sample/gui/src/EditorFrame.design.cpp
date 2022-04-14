/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "EditorFrame.h"

void EditorFrame::InitializeComponents() {
    menuFile = new wxMenu;
    menuFile->Append(ID_EDITOR_SAVE, wxT("&Save"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar(menuBar);
    
    // Create a wxGrid object
    editorTextBox = new wxTextCtrl(this, ID_EDITOR_TEXTBOX, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

void EditorFrame::DeinitializeComponents() {
    delete editorTextBox;
    delete menuBar;
    delete menuFile;
    delete menuHelp;
}
