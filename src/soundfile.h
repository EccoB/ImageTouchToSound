#ifndef SOUNDFILE_H
#define SOUNDFILE_H
#include <string>
#include <WString.h>
class soundfile{
    public:
        soundfile(String, String);
        soundfile();
        soundfile(const soundfile&);

        String getPath() const;
        String getFileName() const;
        String getFullPath() const;
        bool isValid() const;
    protected:
        String path;
        String filename;
        int nbPlayed=0;
        bool isValidB=false;

};

#endif