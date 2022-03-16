/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifdef ISO9660_BUILD

#include <openfsl/iso9660/fs_iso9660.h>

#include "header.h"

int cdfsshell(openfsl::BlockDevice* bd) {
    openfsl::ISO9660 cdfs(bd, "", "\\/", "::");

    error_t result = cdfs.initialize();
    if (result) {
        std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        return result;
    }

    std::cin.ignore();

    std::string command;
    std::chrono::time_point<std::chrono::steady_clock> operationStart =
        std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> operationEnd;

    std::cout << "Type \"help\" to list commands\n";
    while (true) {
        std::vector<std::string> cmd;

        std::cout << "::> ";
        getline(std::cin, command);
        split(command, &cmd, ' ');

        if (settings["MeasureElapsedTime"] == true) {
            operationStart = std::chrono::steady_clock::now();
        }

        if (cmd[0] == "") {
            std::cout << "\n";
            continue;
        } else if ((cmd[0] == "exit") || (cmd[0] == "quit")) {
            break;
        } else if (cmd[0] == "help") {
            if (cmd.size() == 1) {
                std::cout << " Help\n"
                          << "---------------------------------------------\n"
                          << "exit, quit:            "
                          << "Exit program\n"
                          << "set:                   "
                          << "Set properties\n"
                          << "help:                  "
                          << "Show this message.\n";
            } else if (cmd.size() == 2) {
                if (cmd[1] == "set") {
                    std::cout << " Help: set\n"
                              << "-----------------------------------------\n"
                              << "Usage: "
                              << "set <Property Name> <true,t,1/false,f,0>\n"
                              << "MeasureElapsedTime: "
                              << "Measure filesystem operation time\n";
                }
            }
        } else if (cmd[0] == "set") {
            if (cmd.size() == 3) {
                if (settings.find(cmd[1]) != settings.end()) {
                    if (cmd[2] == "true" || cmd[2] == "t" || cmd[2] == "1")
                        settings[cmd[1]] = true;
                    else if (cmd[2] == "false" ||
                        cmd[2] == "f" || cmd[2] == "0")
                        settings[cmd[1]] = false;
                    else
                        std::cout << "Failed to parse input.\n";
                } else {
                    std::cout << "Property is not found.\n";
                }
            }
        } else {
            std::cerr << command << ": No such command\n";
        }

        if (settings["MeasureElapsedTime"] == true) {
            operationEnd = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsedTime =
                operationEnd - operationStart;

            std::cout << "Elapsed time: " << elapsedTime.count() << "ms\n";
        }
    }

    return 0;
}

#endif  // ISO9660_BUILD
