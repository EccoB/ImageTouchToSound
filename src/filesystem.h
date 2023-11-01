#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <FS.h>
#include <SD.h>
void filesystemSetup();
fs::SDFS* getFileSystem();
const char* getExampleFilePath();



#endif