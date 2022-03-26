/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
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
#include "FAT32Command.h"
#include "ids.h"
 
class EditorFrame : public wxFrame
{
public:
    EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
 
private:
    FSCommand* fsCommand = nullptr;
    wxTextCtrl* editorTextBox;

    void InitializeComponents();

    void OnExit(wxCommandEvent& event);
 
    wxDECLARE_EVENT_TABLE();
}; 

#endif  // SAMPLE_EDITORFRAME_H_
