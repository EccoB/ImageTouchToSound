#ifndef SOUND_H
#define SOUND_H
#include <FS.h>
//DAC1 (GPIO25) und DAC2 (GPIO26)
#define SOUTPUTPIN 25

void soundLoop();
bool setupSound();

bool playWAV(fs::File *file);
bool playWAV(const char *filepath);

#endif