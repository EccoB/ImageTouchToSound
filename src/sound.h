#ifndef SOUND_H
#define SOUND_H
#include <FS.h>
#include "soundfile.h"
//DAC1 (GPIO25) und DAC2 (GPIO26)
#define SOUTPUTPIN 25
enum class infosound{
    noSDCard,           //no SD card attached or 
    noSoundFile         //no Soundfile for that image available
};


void soundLoop();
bool setupSound();

bool playWAV(fs::File *file);
bool playWAV(const char *filepath);
bool playSoundFile(soundfile *sndFile);
bool playInfoSound(const infosound sound);



#endif