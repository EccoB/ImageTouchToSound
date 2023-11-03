#include "soundfile.h"

// --- Constructors ---
soundfile::soundfile(){
    this->path="UNDEF";
}
soundfile::soundfile(String path){
    this->path=path;
}
soundfile::soundfile(const soundfile& other){
    this->path=other.getPath();
}

String soundfile::getPath() const{
    return path;
}