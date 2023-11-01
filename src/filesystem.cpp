#define SD_CARD_PIN 26
#include <SD.h>
#include "SPI.h"
#include "filesystem.h"
#define PIN_C_SCK  18
#define PIN_C_MISO 19
#define PIN_C_MOSI 23
#define PIN_C_CS   5

char currentFilePath[50];
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



const char* getExampleFilePath(){
  fs::File test =  SD.open("/frame/example.wav",FILE_READ);
  Serial.println(test.name());
  Serial.print(test.getLastWrite());
  String("/frame/example.wav").toCharArray(currentFilePath,100);
  return currentFilePath;
}