#define SD_CARD_PIN 26
#include <SD.h>
#include "SPI.h"
#include "filesystem.h"
#include <list>
#include <array>
#define PIN_C_SCK  18
#define PIN_C_MISO 19
#define PIN_C_MOSI 23
#define PIN_C_CS   5
#define MAXIMAGES 5
#include "soundfile.h"
char currentFilePath[50];
std::list<soundfile*> soundfilelist[MAXIMAGES]; //Array of lists, each one has a list of soundfiles

/*
SPI	  MOSI	    MISO	    CLK	      CS
VSPI	GPIO 23	  GPIO 19	  GPIO 18	  GPIO 5
HSPI	GPIO 13	  GPIO 12	  GPIO 14	  GPIO 15
*/

/*
3V3	    3.3V
CS	    GPIO 5
MOSI    GPIO 23
CLK	    GPIO 18
MISO	GPIO 19
GND	    GND


*/

void beginFile(char * targetPath)
{
  if (!SD.exists(targetPath))
  {
    Serial.printf("File does not exist: %s\n", targetPath);
    return;
  }
  //file->open(targetPath);
  //wav->begin(fileBuffer, out);
}




void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }

}


void testMp3(fs::FS &fs, const char * dirname, uint8_t levels){

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(!file.isDirectory() && file.name()=="test.mp3"){


    }
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }

}

SPIClass* spiOwn;
void filesystemSetup(){
    
    /*
    SPIClass spi = SPIClass(VSPI);
    spi.begin(SCK, MISO, MOSI, CS);
    if (!SD.begin(CS,spi,80000000)) {
  Serial.println("Card Mount Failed");
    return;
    }
*/
    spiOwn = new SPIClass(VSPI);

    spiOwn->begin(PIN_C_SCK, PIN_C_MISO, PIN_C_MOSI, PIN_C_CS);
    pinMode(PIN_C_CS, OUTPUT);
    //begin(uint8_t ssPin=SS, SPIClass &spi=SPI, uint32_t frequency=4000000, const char * mountpoint="/sd", uint8_t max_files=5, bool format_if_empty=false);
    if (!SD.begin(PIN_C_CS,*spiOwn,40000000))
        Serial.println("Card Mount Failed");

  //SD.open("/");

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  Serial.println("--------- Frame Folder -----------");
  listDir(SD, "/frame", 0);

}

void filesystemLoop(){
  //setFileSystem(*spiOwn);

}

fs::SDFS* getFileSystem(){
  Serial.print("FileSystemaddress:");
  Serial.println((unsigned int) &SD);
  return &SD;
}




/*
Returns the full path to the example file
*/
const char* getExampleFilePath(){
  fs::File test =  SD.open(String(ROOTFOLDER)+EXAMPLEFILE,FILE_READ);
  Serial.println(test.name());
  Serial.print(test.getLastWrite());
  (String(ROOTFOLDER)+EXAMPLEFILE).toCharArray(currentFilePath,100);
  return currentFilePath;
}

// We 

soundfile* getSoundFileForImage(int imageNumber){
  // Needed information
  //  current imageSeries (=folder)
  //  imageNb to image mapping
  //  random number as there may be more than one file per image -> play random

  // image needs to start with the image number

  // Stucture
  /*
  Array with pointers to lists of filenames

  */
  std::array<char,30> exampleArray;

  
  //std::list<std::array<char,30>> exampleLIst[5]; //maximum 5 images per folder
  //In each entry a list of a char array with max 30 characters of filename length
  soundfile *soundFileToPlay=nullptr;

  std::list<soundfile*>* soundListForImage=&soundfilelist[imageNumber];
  unsigned int listSIze = soundListForImage->size();
  if (listSIze>0){
    soundFileToPlay=soundListForImage->front();
    soundListForImage->pop_front();
    soundListForImage->push_back(soundFileToPlay);
  }
  return soundFileToPlay;

}

const bool loadImageList(){

}


bool addSoundFile(const char *folderpath, const char* cstr){
  //Adds a 
  // We first need to check for the name:
  String fileName = String(cstr);

  //------------- Checks ------------------
  if(fileName.length()<6){
    return false; //We need at least 6 characters: ex: 00.wav
  }
  String MAXIMAGESimageNb = fileName.substring(0,1);
  if (!isDigit(fileName.charAt(0)) || !isDigit(fileName.charAt(1))){
    return false;  //the first two characters need to be a number, containing the ImageNumber
  }
  if(!fileName.endsWith(".wav")){
    return false; //Ending should be .wav
  }

  // -------- Seems like a valid file -----------
  unsigned int imageNb = fileName.substring(0,2).toInt();
  if(imageNb>=MAXIMAGES){
    return false; //we do not have enough space for the image
  }
  soundfile *snd= new soundfile(String(folderpath)+fileName);
  soundfilelist[imageNb].push_back(snd);


}

bool loadFolderWithSoundFiles(int maxNbImages, const char *folderpath){
  Serial.printf("Loading images from directory: %s\n", folderpath);
  
  fs::SDFS* fs = &SD;
  File root = fs->open(folderpath);
  if(!root){
    Serial.println("Failed to open directory");
    return false;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return false;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());

      // ----- Append to own list ---
      addSoundFile(folderpath, file.name());
    }
    file = root.openNextFile();
  }


}