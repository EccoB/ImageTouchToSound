#include <Arduino.h>
#include "touch.h"
#include "sound.h"
#include "filesystem.h"
#include <FS.h>

#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
/*
PinLayout:
Audio:
out->SetPinout(16, 17, 4);
SD-Card:
#define PIN_C_SCK  18
#define PIN_C_MISO 19
#define PIN_C_MOSI 23
#define PIN_C_CS   5
Touch:
{15,4};
*/
#define PIN_SD_POWER  25

// put function declarations here:
#define TPIN 2

#define SD_CARD_PIN 26
touchSensors touch;
void print_wakeup_reason();

void playSound(int imageNb){
  soundfile *snd = getSoundFileForImage(imageNb);
  if(snd==nullptr){
    Serial.printf("Soundfile for image %d not found",imageNb);
    playInfoSound(infosound::noSoundFile);
    return;
  }
  Serial.printf("Play soundfile: %s \n",snd->getFullPath().c_str());
  playSoundFile(snd);
}

void setup() {
  //esp_sleep_pd_config(ESP_PD_DOMAIN_VDDSDIO, ESP_PD_OPTION_OFF);
  //esp_deep_sleep_start();
  // put your setup code here, to run once:
  pinMode(PIN_SD_POWER,OUTPUT);
  digitalWrite(PIN_SD_POWER,HIGH);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  Serial.begin(115200);
  Serial.println("Start");
  print_wakeup_reason();
  touch.touchEsetup(&playSound);
  pinMode(TPIN,OUTPUT);
  setupSound();
  
  bool fileSystemOK=filesystemSetup(); //inits the SD card
  if(!fileSystemOK){
    playInfoSound(infosound::noSDCard);
    //ToDo: stop here and go into deep sleep 
    Serial.println("Finished");
  }
  else{
  fs::SDFS* fsystem =getFileSystem();
  Serial.print("FileSystemaddress:");
  Serial.println((unsigned int) &(*fsystem));

  loadImageList();

  //Example play
  
 

  //playWAV(getExampleFilePath());

  //---- Testing soundFile play
  //Serial.println("Start to play sound");
  //delay(10);
  //playSound(0);
  /*
  playSound(0);
  Serial.println("Play agian");
  playSound(0);
  playSound(1);
  playSound(2);
  playSound(1);
  */
  }
}

void loop() {
  bool active = false;
  // put your main code here, to run repeatedly:
  active |= touch.touchloop();
  digitalWrite(TPIN,!digitalRead(TPIN));
  //delay(500);
  active |= soundLoop();

  if(!active){
    //We may go to sleep:
    //FSdeepSleep();
    digitalWrite(PIN_SD_POWER,LOW);
    //soundDeepSleep();
   
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO);
    
    //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html
    //gpio_hold_en()
    //gpio_hold_dis(GPIO_NUM_21);
    //gpio_deep_sleep_hold_dis();

    //gpio_deep_sleep_hold_en();
    //esp_sleep_enable_touchpad_wakeup();
    
    esp_deep_sleep_start();
  }

}



void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
