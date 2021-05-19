# Main Plan

It will implement almost all existing file systems for the disk and will unify file systems into one ultimately.



Implemented/planned file systems:

| #    | File System Name     | Status          |
| ---- | -------------------- | --------------- |
| 1    | FAT(12/16/32)        | In progress     |
| 2    | VFAT                 | Not Implemented |
| 3    | exFAT                | Not Implemented |
| 4    | NTFS                 | Not Implemented |
| 5    | ext(2/3/4)           | Not Implemented |
| 6    | BtrFS                | Not Implemented |
| 7    | JFS                  | Not Implemented |
| 8    | XFS                  | Not Implemented |
| 9    | ZFS                  | Not Implemented |
| 10   | HFS+                 | Not Implemented |
| 11   | ISO 9660 + El Torito | Not Implemented |
| 12   | UDF                  | Not Implemented |
| 13   | UFS                  | Not Implemented |



## Library Structure

File System

- Logger driver wrapper (inherited from Driver class)
- Disk driver wrapper (inherited from Driver class)
- File system implementation with abstracted driver

Driver

- User-Initializable I/O methods

## Final expected pseudo code for main

```c++
extern Data disk_in(uint64_t offset, size_t sector);
extern int disk_out(uint64_t offset, Data data);

extern int log_out(char* str);

extern int mem_copy(char* dest, char* src, size_t size);

void fsctrl_main() {
    DiskDriver dd = DiskDriver(disk_in, disk_out); // Disk I/O driver initialization
    LoggerDriver ld = LoggerDriver(null, log_out); // Logger I/O driver initialization
    FS_FAT32 fat32 = FS_FAT32(dd, ld, FAT32_LFN | FAT32_NONSECURE); // File system initialization
    
    fat32.start(); // Starting file system
    
    fat32.setSeparator('/'); // Setting directory separator
    fat32.setDirectory("/test1"); // Setting directory cursor
    
    File file = fat32.openFile("./test2/test3.txt"); // Reading file from disk
    
    char[64] filebuf = { 0, };
    
    mem_copy(filebuf, file.getData(64), 64); // Getting data from file
    
    file.close(); // Closing file
}
```

