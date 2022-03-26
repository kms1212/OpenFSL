/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "EditorFrame.h"

wxBEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(wxID_EXIT, EditorFrame::OnExit)
wxEND_EVENT_TABLE()
 
EditorFrame::EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    InitializeComponents();
}
 
void EditorFrame::OnExit(wxCommandEvent& event)
{
    if (fsCommand != nullptr) {
        if (dynamic_cast<FAT32Command*>(fsCommand) != nullptr) {
            delete (FAT32Command*)fsCommand;
        } else {
            std::cout << "Error\n";
        }
    }

    Close(true);
}
