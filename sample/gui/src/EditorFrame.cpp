/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "EditorFrame.h"

#include "FAT32Command.h"
#include "FAT32FileCommand.h"

wxBEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_MENU(ID_EDITOR_SAVE, EditorFrame::OnSave)
    EVT_MENU(wxID_EXIT, EditorFrame::OnExit)
wxEND_EVENT_TABLE()
 
EditorFrame::EditorFrame(const wxPoint& pos, const wxSize& size, FSCommand* fsCommand, const std::string path)
    : wxFrame(NULL, wxID_ANY, path, pos, size) {
    this->fsCommand = fsCommand;

    if (dynamic_cast<FAT32Command*>(fsCommand) != nullptr) {
        this->fileCommand = new FAT32FileCommand(dynamic_cast<FAT32Command*>(fsCommand), path, openfsl::FSL_OpenMode::Read | openfsl::FSL_OpenMode::Write);
    } else {
        std::cout << "Error\n";
    }

    InitializeComponents();

    error_t result = this->fileCommand->seekg(0, openfsl::FSL_SeekMode::SeekEnd);
    size_t fileSize = this->fileCommand->tellg();
    result = this->fileCommand->seekg(0, openfsl::FSL_SeekMode::SeekSet);

    char* buf = new char[fileSize + 1]();
    this->fileCommand->read(buf, fileSize, 1);
    buf[fileSize] = 0;

    wxString bufstr = buf;
    editorTextBox->ChangeValue(bufstr);
    delete[] buf;
}

void EditorFrame::OnSave(wxCommandEvent& event) {
    std::string str = editorTextBox->GetValue().ToStdString();
    this->fileCommand->write(str.c_str(), str.size(), 1);
}
 
void EditorFrame::OnExit(wxCommandEvent& event)
{
    if (fsCommand != nullptr) {
        if (dynamic_cast<FAT32FileCommand*>(fileCommand) != nullptr) {
            delete (FAT32FileCommand*)fileCommand;
        } else {
            std::cout << "Error\n";
        }
    }

    DeinitializeComponents();
    Close(true);
}
