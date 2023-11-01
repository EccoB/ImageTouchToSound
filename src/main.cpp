#include <Arduino.h>
#include "touch.h"
#include "sound.h"
#include "filesystem.h"
#include <FS.h>
// put function declarations here:
#define TPIN 2

#define SD_CARD_PIN 26

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");
  touchEsetup();
  pinMode(TPIN,OUTPUT);
  setupSound();
  filesystemSetup();
  fs::SDFS* fsystem =getFileSystem();
  Serial.print("FileSystemaddress:");
  Serial.println((unsigned int) &(*fsystem));

  playWAV(getExampleFilePath());
  Serial.println("_-- Finished init ---");
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  //touchloop();
  digitalWrite(TPIN,!digitalRead(TPIN));
  //delay(500);
  soundLoop();

}
