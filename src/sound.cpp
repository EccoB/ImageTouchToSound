#include <sound.h>
#include <FS.h>

#include <Arduino.h>
#include <AudioFileSourceFunction.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>


// To run, set your ESP8266 build to 160MHz, and include a SPIFFS of 512KB or greater.
// Use the "Tools->ESP8266/ESP32 Sketch Data Upload" menu to write the MP3 to SPIFFS
// Then upload the sketch normally.  

// pno_cs from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html

AudioGeneratorWAV* wav;
AudioFileSourceFunction* file;
AudioFileSourceSD* sourceFile;
AudioGeneratorWAV* decoderWAV;

#include <AudioOutputI2S.h>
AudioOutputI2S* out;

#define PIN_BLKClK  16
#define PIN_WCLK    17
#define PIN_DOUT    4
    //BLKClk 1
    //wclkPin 22
    //doutPin 3


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
  #ifdef AUDIO_DIRECT
    out = new AudioOutputI2S(0,1); //Uncomment this line, comment the next one to use the internal DAC channel 1 (pin25) on ESP32
  #else
    out = new AudioOutputI2S();//0, 0, 8, 1); //int port=0, int output_mode=EXTERNAL_I2S, int dma_buf_count = 8, int use_apll=APLL_DISABLE)
    //out->SetPinout(16, 17, 4);
    out->SetPinout(PIN_BLKClK, PIN_WCLK,PIN_DOUT);
    //BLKClk 1
    //wclkPin 22
    //doutPin 3
    //out = new AudioOutputI2S(); 
    //I2S pin 	Common label* 	ESP8266 pin
    //LRC 	D4 	GPIO2
    //BCLK 	D8 	GPIO15
    //DIN 	RX 	GPIO3
  #endif

  //wav = new AudioGeneratorWAV();
  //wav->begin(file, out);
  sourceFile = new AudioFileSourceSD();
  return true;
}

void soundDeepSleep(){
  delete out;
  /*
  pinMode(PIN_BLKClK,OUTPUT);
  pinMode(PIN_WCLK,OUTPUT);
  pinMode(PIN_DOUT,OUTPUT);
  digitalWrite(PIN_BLKClK,LOW);
  digitalWrite(PIN_WCLK,LOW);
  digitalWrite(PIN_DOUT,LOW);
  */
  //out->SetPinout(PIN_BLKClK, PIN_WCLK,PIN_DOUT)
}

/**
 * Returns true while sound is playing
*/
bool soundLoop(){
  bool soundPlaying = false;

  if (decoderWAV && (decoderWAV->isRunning())) {
      soundPlaying = true;
      if (!decoderWAV->loop()) {
        decoderWAV->stop();
        if (sourceFile!=nullptr)
          sourceFile->close();
      }
    }
  return soundPlaying;

/*
    Serial.println("SLoop");
    if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    Serial.println("function done!");
    delay(1000);
  }
  */
}


bool setFileSystem(fs::SDFS *fs){
  Serial.print("FileSystemaddress in setFileSystem:");
  Serial.println((unsigned int) (fs));

  //fileSystem=fs;
  //sourceFile = new AudioFileSourceFS(*fileSystem);
  Serial.println("File System was set in Soundfile");
  return true;
}

bool playInfoSound(const infosound sound){
  auto soundFile = AudioFileSourceFunction(1.,1,8000,16);
  //
  // you can set (sec, channels, hz, bit/sample) but you should care about
  // the trade-off between performance and the audio quality
  //
  // file = new AudioFileSourceFunction(sec, channels, hz, bit/sample);
  // channels   : default = 1
  // hz         : default = 8000 (8000, 11025, 22050, 44100, 48000, etc.)
  // bit/sample : default = 16 (8, 16, 32)


  switch(sound){
    case infosound::noSDCard:
      soundFile.addAudioGenerators([&](const float time) {
        if (time<0.5)
          return 0.1 * sin(TWO_PI * 440.f * time); // C
        else
          return 0.1 * sin(TWO_PI * 540.f * time); // C
      });
      break;
    default:
      soundFile.addAudioGenerators([&](const float time) {
          if (time<0.3)
            return 0.1 * sin(TWO_PI * 220.f * time); // C
          else if(time<0.6)
            return 0.;
          else
            return 0.1 * sin(TWO_PI * 220.f * time); // C
      });
      break;
  }
  decoderWAV->begin(&soundFile,out);

  // Immediately start playing:
  while ((decoderWAV) && (decoderWAV->isRunning())) {
    if (!decoderWAV->loop()){
        decoderWAV->stop();
    } 
  } 
  Serial.println("Finished InfoSOund");
  delay(100);
  Serial.println("Finish");
  return true;
}


bool playWAV(fs::File *file){
  Serial.println("Not supported");
  return false;
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
  return true;

  // Immediately start playing:
  #ifdef SOUND_NO_LOOP
    while ((decoderWAV) && (decoderWAV->isRunning())) {
      if (!decoderWAV->loop()) {
        decoderWAV->stop();
        return true;
      }
    }
  #endif
  return false; 

}

bool playSoundFile(soundfile *sndFile){
  if(sndFile==nullptr || !sndFile->isValid()){
    playInfoSound(infosound::noSoundFile);
    return false;
  }
  char stringBuffer[40];
  sndFile->getFullPath().toCharArray(stringBuffer,40);
  return playWAV(stringBuffer);
}