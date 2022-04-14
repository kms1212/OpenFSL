/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_EDITORFRAME_H_
#define SAMPLE_EDITORFRAME_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>

#include <openfsl/openfsl.h>

#include "utils.h"
#include "FileCommand.h"
#include "ids.h"
 
class EditorFrame : public wxFrame
{
public:
    EditorFrame(const wxPoint& pos, const wxSize& size, FSCommand* fsCommand, const std::string path);
 
private:
    FSCommand* fsCommand = nullptr;
    FileCommand* fileCommand = nullptr;
    wxTextCtrl* editorTextBox;
    wxMenuBar* menuBar;
    wxMenu* menuFile;
    wxMenu* menuHelp;
    

    void InitializeComponents();
    void DeinitializeComponents();

    void OnSave(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
 
    wxDECLARE_EVENT_TABLE();
}; 

#endif  // SAMPLE_EDITORFRAME_H_
