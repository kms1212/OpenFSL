/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "Application.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_LIST_ITEM_SELECTED(ID_FILELIST, MainFrame::OnFileListClick)

    EVT_MENU(ID_OPENIMGFILE, MainFrame::OnOpenImgFile)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
 
wxIMPLEMENT_APP(Application);
 
bool Application::OnInit()
{
    MainFrame *frame = new MainFrame("OpenFSL Sample Application", wxPoint(50, 50), wxSize(720, 480));
    frame->Show(true);
    return true;
}
 
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    InitializeComponents();

    openFileDialog = new wxFileDialog(this, wxT("Select disk image"), wxEmptyString, wxEmptyString, "Raw disk image (*.img)|*.img|ISO9660 optical image (*.iso)|*.iso|All file (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog->ShowModal() != wxID_CANCEL) {
        InitializeFilesystem(openFileDialog->GetPath());
    }
}
 
void MainFrame::OnExit(wxCommandEvent& event)
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
 
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void MainFrame::OnOpenImgFile(wxCommandEvent& event)
{
    if (openFileDialog->ShowModal() != wxID_CANCEL) {
        InitializeFilesystem(openFileDialog->GetPath());
    }
}
 
void MainFrame::OnFileListClick(wxListEvent& event)
{
    wxListItem litem;
    litem.m_itemId = event.m_itemIndex;
    litem.m_col = 3;
    litem.m_mask = wxLIST_MASK_TEXT;
    listCtrl->GetItem(litem);

    if (litem.m_text == wxT("File")) {
        return;
    } else {
        litem.m_itemId = event.m_itemIndex;
        litem.m_col = 0;
        litem.m_mask = wxLIST_MASK_TEXT;
        listCtrl->GetItem(litem);

        std::string navigatePath;
        fsCommand->GetCurrentDirectory(&navigatePath);

        std::string pathSeparator;
        fsCommand->GetPathSeparator(&pathSeparator);

        navigatePath += pathSeparator.at(0) + litem.m_text;
        fsCommand->NavigateDirectory(navigatePath);

        std::string currentPath;
        fsCommand->GetCurrentDirectory(&currentPath);
        SetStatusText(currentPath);

        std::vector<FileInfo> childList;
        fsCommand->ListDirectoryChild(&childList);

        listCtrl->DeleteAllItems();
        int i = 0;
        for (FileInfo file : childList) {
            listCtrl->InsertItem(i, wxString(file.fileName));
            listCtrl->SetItem(i, 1, wxString(
                std::to_string(static_cast<int>(file.fileCreateTime.time_month)) + "-" + 
                std::to_string(static_cast<int>(file.fileCreateTime.time_day)) + "-" + 
                std::to_string(static_cast<int>(file.fileCreateTime.time_year)) + " " + 
                std::to_string(static_cast<int>(file.fileCreateTime.time_hour)) + ":" + 
                std::to_string(static_cast<int>(file.fileCreateTime.time_min)) + ":" + 
                std::to_string(static_cast<int>(file.fileCreateTime.time_sec))));
            listCtrl->SetItem(i, 2, wxString(std::to_string(file.fileSize)));
            if (file.fileType == FileType::File) {
                listCtrl->SetItem(i, 3, wxT("File"));
            } else {
                listCtrl->SetItem(i, 3, wxT("Directory"));
            }
            i++;
        }
    }
}

void MainFrame::InitializeFilesystem(wxString path) {
    std::string imagePath = path.ToStdString();

    bool isCdrom = false;
    size_t imagedotpos = imagePath.find_last_of('.');
    if (imagedotpos != std::string::npos) {
        std::string imageFileExt = imagePath.substr(imagedotpos + 1);

        for (auto & c : imageFileExt) c = static_cast<char>(toupper(c));

        isCdrom = imageFileExt == "ISO";
    }

    if (isCdrom) {
        openfsl::BlockDevice::DiskParameter parameter;
        parameter.bytesPerSector = 2048;
        fbd.setDiskParameter(parameter);
    } else {
        std::cout << "Reading image file parameter...\n";
        std::fstream diskInfo;
        diskInfo.open(std::string(imagePath) + std::string(".info"),
            std::ios::in);
        if (!diskInfo.fail()) {
            std::string line;
            openfsl::BlockDevice::DiskParameter parameter;

            while (getline(diskInfo, line)) {
                std::cout << line << "\n";

                std::vector<std::string> value;

                split(line, &value, ' ');
                if (value[0] == "SectorPerTrack") {
                    parameter.sectorPerTrack = stoi(value[1]);
                } else if (value[0] == "HeadPerCylinder") {
                    parameter.headPerCylinder = stoi(value[1]);
                } else if (value[0] == "BytesPerSector") {
                    parameter.bytesPerSector = stoi(value[1]);
                }
            }

            fbd.setDiskParameter(parameter);

            diskInfo.close();
        } else {
            std::cout << "Fail to read disk parameter file. ";
            std::cout << "Default values are will be used.\n";
        }
    }

    error_t result = fbd.initialize(std::string(imagePath),
        openfsl::FileBlockDevice::OpenMode::RW);
    if (result) {
        std::cout << openfsl::geterrorstr(result) << "\n";
        exit(result);
    }

    if (isCdrom) {

    } else {
        openfsl::DiskStructure diskStructure;

        result = openfsl::detectDiskStructure(&diskStructure, &fbd);
        if (result) {
            std::cout << openfsl::geterrorstr(result) << "\n";
            exit(result);
        }

        wxArrayString partitionList;
        for (size_t i = 0; i < diskStructure.partList.size(); i++) {
            partitionList.Add(fileSystemTypeToString(diskStructure.partList[i]));
        }

        wxSingleChoiceDialog dialog(this, wxT("Select partition."), wxT(""), partitionList);
        dialog.SetSelection(0);
        if (dialog.ShowModal() != wxID_OK) {
            return;
        }
        
        if (diskStructure.partList[dialog.GetSelection()] ==
            openfsl::FileSystemType::FAT32) {
            if (fsCommand != nullptr) {
                if (dynamic_cast<FAT32Command*>(fsCommand) != nullptr) {
                    delete (FAT32Command*)fsCommand;
                } else {
                    std::cout << "Error\n";
                    exit(result);
                }
            }
#ifdef FAT32_BUILD
            fsCommand = new FAT32Command(&fbd);

            fsCommand->Initialize(diskStructure, dialog.GetSelection());

            std::string currentPath;
            fsCommand->GetCurrentDirectory(&currentPath);
            SetStatusText(currentPath);

            std::vector<FileInfo> childList;
            fsCommand->ListDirectoryChild(&childList);

            listCtrl->DeleteAllItems();
            int i = 0;
            for (FileInfo file : childList) {
                listCtrl->InsertItem(i, wxString(file.fileName));
                listCtrl->SetItem(i, 1, wxString(
                    std::to_string(static_cast<int>(file.fileCreateTime.time_month)) + "-" + 
                    std::to_string(static_cast<int>(file.fileCreateTime.time_day)) + "-" + 
                    std::to_string(static_cast<int>(file.fileCreateTime.time_year)) + " " + 
                    std::to_string(static_cast<int>(file.fileCreateTime.time_hour)) + ":" + 
                    std::to_string(static_cast<int>(file.fileCreateTime.time_min)) + ":" + 
                    std::to_string(static_cast<int>(file.fileCreateTime.time_sec))));
                listCtrl->SetItem(i, 2, wxString(std::to_string(file.fileSize)));
                if (file.fileType == FileType::File) {
                    listCtrl->SetItem(i, 3, wxT("File"));
                } else {
                    listCtrl->SetItem(i, 3, wxT("Directory"));
                }
                i++;
            }

#else
            wxMessageBox("FAT32 option is not enabled.", "Error");
#endif
        }
    }
}