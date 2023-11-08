#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <FS.h>
#include <SD.h>
#include "soundfile.h"

#define ROOTFOLDER "/frame/"
#define ROOTSOUNDFOLDER "/frame/sounds"
#define EXAMPLEFILE "example.wav"


bool filesystemSetup();
fs::SDFS* getFileSystem();
const char* getExampleFilePath();
soundfile* getSoundFileForImage(int imageNumber);
const bool loadImageList();

/**
 Expected Structure
 /frame/
    example.wav
    README.wav
    folder1  - contains a set of soundfiles for each image
    folder2
    folder3
    ....
        01SoundForFirstImage.wav
        02SOundForScndImage.wav
        03SoundForThirdImage.wav
        01SoundForFirstImage2.wav
        ....
        


*/

#endif