#include <sound.h>
#include <FS.h>

#include <Arduino.h>
#include <AudioFileSourceFunction.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

// To run, set your ESP8266 build to 160MHz, and include a SPIFFS of 512KB or greater.
// Use the "Tools->ESP8266/ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
// Then upload the sketch normally.  

// pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html

AudioGeneratorWAV* wav;
AudioFileSourceFunction* file;
AudioFileSourceSD* sourceFile;
AudioGeneratorWAV* decoderWAV;
AudioOutputI2S* out;


bool setupSound(){

  decoderWAV = new AudioGeneratorWAV();

  // ===== create instance with length of song in [sec] =====
  file = new AudioFileSourceFunction(15.,1,44100);
  //
  // you can set (sec, channels, hz, bit/sample) but you should care about
  // the trade-off between performance and the audio quality
  //
  // file = new AudioFileSourceFunction(sec, channels, hz, bit/sample);
  // channels   : default = 1
  // hz         : default = 8000 (8000, 11025, 22050, 44100, 48000, etc.)
  // bit/sample : default = 16 (8, 16, 32)

  
    // ===== set your sound function =====
  file->addAudioGenerators([&](const float time) {
    float v = sin(TWO_PI * 500. * time);  // generate sine wave

    //v *= fmod(time, 1.f);               // change linear
    v *= 0.5;                           // scale
    return v;
  });

  //
  // sound function should have one argument(float) and one return(float)
  // param  : float (current time [sec] of the song)
  // return : float (the amplitude of sound which varies from -1.f to +1.f)
  //
  // sound function can be registered only one or the same number with channels
  // if the channels > 1 && the number of function == 1,
  // same function are used to generate the sound in every channel
  //
  // file = new AudioFileSourceFunction(8., 2);
  // file->addAudioGenerators(
  //   // L (channel 0)
  //   [](const float time) {
  //     return 0.25 * sin(TWO_PI * 440.f * time) * fmod(time, 1.f); // C
  //   },
  //   // R (channel 1)
  //   [](const float time) {
  //     return 0.25 * sin(TWO_PI * 550.f * time) * fmod(time, 1.f); // E
  //   }
  // );
  //
  // you can also use the pre-defined function
  // file->addAudioGenerators(sine_wave);
  //out = new AudioOutputI2S();
  out = new AudioOutputI2S(0,1); //Uncomment this line, comment the next one to use the internal DAC channel 1 (pin25) on ESP32
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
  sourceFile = new AudioFileSourceSD();
  return true;
}

void soundLoop(){
    Serial.println("SLoop");
    if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    Serial.println("function done!");
    delay(1000);
  }
}


bool setFileSystem(fs::SDFS *fs){
  Serial.print("FileSystemaddress in setFileSystem:");
  Serial.println((unsigned int) (fs));

  //fileSystem=fs;
  //sourceFile = new AudioFileSourceFS(*fileSystem);
  Serial.println("File System was set in Soundfile");
}
bool playWAV(fs::File *file){
  Serial.println("Not supported");
}
bool playWAV(const char *filepath){
  // setFileSystem must be called beforehand
  Serial.print("Playing ");
  Serial.println(filepath);

  if(sourceFile==nullptr){
    Serial.println("Sorry, Filesystem was not set, please set it first!");
    return false;
  }

  sourceFile->open(filepath);
  decoderWAV->begin(sourceFile,out);

  // Immediately start playing:
  while ((decoderWAV) && (decoderWAV->isRunning())) {
    if (!decoderWAV->loop()) decoderWAV->stop();
  } 

}

