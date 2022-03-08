# OpenFSL README

![openfsl-logo](https://raw.githubusercontent.com/kms1212/OpenFSL/main/doc/logo/openfsl-dark-320x135.png)

![Platforms](https://img.shields.io/badge/platforms-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20FreeBSD-blue) [![Build](https://github.com/kms1212/OpenFSL/actions/workflows/Build.yml/badge.svg)](https://github.com/kms1212/OpenFSL/actions/workflows/Build.yml) [![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL?ref=badge_shield) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/f1e571ec17324d228a85e182f7f649f1)](https://www.codacy.com/gh/kms1212/OpenFSL/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=kms1212/OpenFSL&amp;utm_campaign=Badge_Grade)

## Progress

File systems:

| #   | File System | Status               |
| --- | ----------- | -------------------- |
| 1   | FAT32       | Finished             |
| 2   | NTFS        | In progress          |
| 3   | ISO9660     | Preparing to develop |

## Downloads

### Shared Library

| OS            | x86-64                                                                                                                                                                                                        |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Windows-MSVC  | [![Win_MSVCdl64SharedZip](https://img.shields.io/badge/Download-ZIP-blue)][Win_MSVCdl64SharedZip] [![Win_MSVCdl64SharedExe](https://img.shields.io/badge/Download-Installer-green)][Win_MSVCdl64SharedExe]    |
| Windows-MinGW | [![Win_MGWdl64SharedZip](https://img.shields.io/badge/Download-ZIP-blue)][Win_MGWdl64SharedZip] [![Win_MGWdl64SharedExe](https://img.shields.io/badge/Download-Installer-green)][Win_MGWdl64SharedExe]        |
| Linux         | [![Lin_GCCdl64SharedTgz](https://img.shields.io/badge/Download-TGZ-blue)][Lin_GCCdl64SharedTgz] [![Lin_GCCdl64SharedDeb](https://img.shields.io/badge/Download-Debian%20Package-green)][Lin_GCCdl64SharedDeb] |
| macOS         | [![Dwn_ACLdl64SharedTgz](https://img.shields.io/badge/Download-TGZ-blue)][Dwn_ACLdl64SharedTgz] [![Dwn_ACLdl64SharedZip](https://img.shields.io/badge/Download-ZIP-blue)][Dwn_ACLdl64SharedZip]               |

### Static Library

| OS            | x86                                                                                                                                                                                                           | x86-64                                                                                                                                                                                                        |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Windows-MSVC  | [![Win_MSVCdl86StaticZip](https://img.shields.io/badge/Download-ZIP-blue)][Win_MSVCdl86StaticZip] [![Win_MSVCdl86StaticExe](https://img.shields.io/badge/Download-Installer-green)][Win_MSVCdl86StaticExe]    | [![Win_MSVCdl64StaticZip](https://img.shields.io/badge/Download-ZIP-blue)][Win_MSVCdl64StaticZip] [![Win_MSVCdl64StaticExe](https://img.shields.io/badge/Download-Installer-green)][Win_MSVCdl64StaticExe]    |
| Windows-MinGW |                                                                                                                                                                                                               | [![Win_MGWdl64StaticZip](https://img.shields.io/badge/Download-ZIP-blue)][Win_MGWdl64StaticZip] [![Win_MGWdl64StaticExe](https://img.shields.io/badge/Download-Installer-green)][Win_MGWdl64StaticExe]        |
| Linux         | [![Lin_GCCdl86StaticTgz](https://img.shields.io/badge/Download-TGZ-blue)][Lin_GCCdl86StaticTgz] [![Lin_GCCdl86StaticDeb](https://img.shields.io/badge/Download-Debian%20Package-green)][Lin_GCCdl86StaticDeb] | [![Lin_GCCdl64StaticTgz](https://img.shields.io/badge/Download-TGZ-blue)][Lin_GCCdl64StaticTgz] [![Lin_GCCdl64StaticDeb](https://img.shields.io/badge/Download-Debian%20Package-green)][Lin_GCCdl64StaticDeb] |
| macOS         |                                                                                                                                                                                                               | [![Dwn_ACLdl64StaticTgz](https://img.shields.io/badge/Download-TGZ-blue)][Dwn_ACLdl64StaticTgz] [![Dwn_ACLdl64StaticZip](https://img.shields.io/badge/Download-ZIP-blue)][Dwn_ACLdl64StaticZip]               |

## Description

Sometimes you may want to modify the file system image. And sometimes you may want to mount and read or write an unsupported filesystem. I created this project to do these things without installing and mounting with [libfuse](https://github.com/libfuse/libfuse).  <br/>

Cross-compiling is not possible at this time because there are so many OS-specific build tasks. This library provides only file system control, so you have to implement the abstraction layer for the library. However, in this regard, it can have the advantage of being able to integrate into the operating system. 

<img src="https://raw.githubusercontent.com/kms1212/OpenFSL/main/doc/images/OpenFSLtoApplicationCodeStructure.png" alt="image1" width="50%" height="50%"/>

In this image, the disk device abstraction layer is in the user application block. But if I transform this image a little bit...

<img src="https://raw.githubusercontent.com/kms1212/OpenFSL/main/doc/images/OpenFSLtoOSCodeStructure.png" alt="image1" width="35%" height="35%"/>

You can integrate the library into your operating system!

## Arch/platforms[^note]

| Platform                         | Windows | Linux(Ubuntu) | FreeBSD | macOS |
| -------------------------------- | ------- | ------------- | ------- | ----- |
| x86(i686)                        | O       | O             | O       |       |
| AMD64(x86-64)                    | O       | O             | O       | O     |
| aarch64                          | X       | O             | O       | O     |
| armel (ARM 32-bit LE[^le])       | X       | O             | O       |       |
| ppc64el (PowerPC 64-bit LE[^le]) |         | X             | X       |       |
| ppc64 (PowerPC 64-bit BE[^be])   |         |               | X       |       |
| s390x                            |         | X             |         |       |

[^note]: 'O' means supporting, 'X' means not tested and not supported, blank means unable to build.
[^le]: Little-Endian system.
[^be]: Big-Endian system

## Other

### Documents

Go to [OpenFSL Mainpage](https://kms1212.github.io/OpenFSL) to see Documents.

### Discord

[![Discord](https://img.shields.io/badge/Discord-go-blue)](https://discord.gg/BvwV4U3Skr)

### License

OpenFSL and its documents are licensed under the BSD-3-Clause.

See the LICENSE file for more details.

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fkms1212%2FOpenFSL?ref=badge_large)

[Win_MSVCdl86SharedZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.Win32-MSVC-SHARED.zip
[Win_MSVCdl86SharedExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.Win32-MSVC-SHARED.exe
[Win_MSVCdl64SharedZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x64-MSVC-SHARED.zip
[Win_MSVCdl64SharedExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x64-MSVC-SHARED.exe
[Win_MGWdl64SharedZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x86-64-GNU-SHARED.zip
[Win_MGWdl64SharedExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x86-64-GNU-SHARED.exe
[Lin_GCCdl86SharedTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.i686-GNU-SHARED.tar.gz
[Lin_GCCdl86SharedDeb]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.i686-GNU-SHARED.deb
[Lin_GCCdl64SharedTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.x86-64-GNU-SHARED.tar.gz
[Lin_GCCdl64SharedDeb]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.x86-64-GNU-SHARED.deb
[Dwn_ACLdl64SharedTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Darwin.x86-64-AppleClang-SHARED.tar.gz
[Dwn_ACLdl64SharedZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Darwin.x86-64-AppleClang-SHARED.zip
[Win_MSVCdl86StaticZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.Win32-MSVC-STATIC.zip
[Win_MSVCdl86StaticExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.Win32-MSVC-STATIC.exe
[Win_MSVCdl64StaticZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x64-MSVC-STATIC.zip
[Win_MSVCdl64StaticExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x64-MSVC-STATIC.exe
[Win_MGWdl64StaticZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x86-64-GNU-STATIC.zip
[Win_MGWdl64StaticExe]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Windows.x86-64-GNU-STATIC.exe
[Lin_GCCdl86StaticTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.i686-GNU-STATIC.tar.gz
[Lin_GCCdl86StaticDeb]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.i686-GNU-STATIC.deb
[Lin_GCCdl64StaticTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.x86-64-GNU-STATIC.tar.gz
[Lin_GCCdl64StaticDeb]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Linux.x86-64-GNU-STATIC.deb
[Dwn_ACLdl64StaticTgz]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Darwin.x86-64-AppleClang-STATIC.tar.gz
[Dwn_ACLdl64StaticZip]: https://github.com/kms1212/OpenFSL/releases/download/nightly/openfsl-Darwin.x86-64-AppleClang-STATIC.zip
