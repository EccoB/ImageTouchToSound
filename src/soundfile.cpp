#include "soundfile.h"
#include <Arduino.h>
// --- Constructors ---
soundfile::soundfile(){
    this->path="UNDEF";
    this->filename="UNDEF";
}
soundfile::soundfile(String folderpath, String filename){
    this->path=folderpath;
    this->filename=filename;
    isValidB=true;
    Serial.printf("Soundfile created - Folder: %s |filename: %s \n",folderpath.c_str(),filename.c_str());
}
soundfile::soundfile(const soundfile& other){
    this->path=other.getPath();
    this->filename=other.getFileName();
    this->isValidB=other.isValid();
}

String soundfile::getPath() const{
    return path;
}
String soundfile::getFileName() const{
    return filename;
}
String soundfile::getFullPath() const{
    return path+String("/")+filename;
}
bool soundfile::isValid() const{
    return isValidB;
}