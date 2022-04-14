/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_APPLICATION_H_
#define SAMPLE_APPLICATION_H_

#include <wx/wx.h>

#include "MainFrame.h"
#include "EditorFrame.h"
 
class Application : public wxApp
{
public:
    virtual bool OnInit();
};

#endif  // SAMPLE_APPLICATION_H_
