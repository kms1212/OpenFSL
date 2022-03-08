/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_APPLICATION_H_
#define SAMPLE_APPLICATION_H_

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>

#include <openfsl/openfsl.h>

#include "header.h"
#include "FAT32Command.h"

enum {
    ID_OPENIMGFILE = 1
};
 
class Application : public wxApp
{
public:
    virtual bool OnInit();
};
 
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
 
private:
    wxDataViewCtrl* dataViewCtrl;
    wxListCtrl* listCtrl;
    wxFileDialog* openFileDialog;
    openfsl::FileBlockDevice fbd;
    FSCommand* fsCommand = nullptr;

    void InitializeComponents();

    void OnOpenImgFile(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void InitializeFilesystem(wxString imagePath);
 
    wxDECLARE_EVENT_TABLE();
}; 

#endif  // SAMPLE_APPLICATION_H_
