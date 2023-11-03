#ifndef SOUNDFILE_H
#define SOUNDFILE_H
#include <string>
#include <WString.h>
class soundfile{
    public:
        soundfile(String);
        soundfile();
        soundfile(const soundfile&);
        String getPath() const;
    protected:
        String path;
        int nbPlayed=0;

};

#endif