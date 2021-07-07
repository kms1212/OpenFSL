# OpenFSL README

![openfsl-logo](https://raw.githubusercontent.com/kms1212/OpenFSL/main/doc/logo/openfsl-dark-320x135.png)

An open source filesystem library

[![Build](https://github.com/kms1212/OpenFSL/actions/workflows/Build.yml/badge.svg)](https://github.com/kms1212/OpenFSL/actions/workflows/Build.yml) [![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL?ref=badge_shield) [![CodeFactor](https://www.codefactor.io/repository/github/kms1212/openfsl/badge)](https://www.codefactor.io/repository/github/kms1212/openfsl) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/f1e571ec17324d228a85e182f7f649f1)](https://www.codacy.com/gh/kms1212/OpenFSL/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=kms1212/OpenFSL&amp;utm_campaign=Badge_Grade)

## Progress

File systems:

| #    | File System Name     | Status          |
| ---- | -------------------- | --------------- |
| 1    | FAT(12/16/32)        | In progress     |

## Pseudo code for main

```c++
extern Data disk_in(uint64_t offset, size_t sector);
extern int disk_out(uint64_t offset, Data data);

extern int mem_copy(char* dest, char* src, size_t size);

void fsctrl_main() {
    DiskDriver dd = DiskDriver(disk_in, disk_out); // Initialize disk I/O driver
    FS_FAT32 fat32 = FS_FAT32(dd, FAT32_LFN, "/"); // Initialize file system
    
    fat32.start(); // Start file system
    fat32.chdir("::/test1"); // Set directory cursor
    
    FAT32_file file = fat32.openFile("./test2/test3.txt"); // Read file from disk
    
    uint8_t buf[64];
    
    file.read(buf, 64); // Get data from file
    
    file.close(); // Close file
}
```

## Other

### Documents

Go to [OpenFSL Mainpage](https://kms1212.github.io/OpenFSL) to see Documents.

### Discord

[![Discord](https://img.shields.io/badge/Discord-go-blue)](https://discord.gg/BvwV4U3Skr)

### License

OpenFSL is licensed under the BSD-3-Clause.

See the LICENSE file for more details.

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL?ref=badge_large)
