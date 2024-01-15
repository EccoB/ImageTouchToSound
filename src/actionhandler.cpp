#include <actionhandler.h>
#define PIN_IO0 0
#include <Arduino.h>
#include "filesystem.h"


actionHandler::actionHandler(){
    pinMode(PIN_IO0, INPUT_PULLUP);
}
void actionHandler::setTouchClass(touchSensors *touchClass){
    this->touchClass = touchClass;
}

unsigned long ioPressLength[1]={0};
bool actionHandler::loop(){
    bool aButtonIsPressed = false;
    if ((millis()-lastCheck)>checkPeriod){
        lastCheck=millis();
        if (digitalRead(PIN_IO0)==LOW){
            // is pressed
            ioPressLength[0] += checkPeriod;
            
            aButtonIsPressed=true;
        }
        else{
            if(ioPressLength[0]>150){
                ioPressed(PIN_IO0,ioPressLength[0]); //at least 100 ms press
            }
            ioPressLength[0]=0;
        }
    }
    return aButtonIsPressed;

}

void actionHandler::ioPressed(unsigned int pin, unsigned long durationMS){
    if(pin == PIN_IO0){
        if (durationMS<1000){
            // switch between folders
            Serial.print("Short press Button in ms:");
            Serial.println(durationMS);
            int nextFolder = getSountFolderNb()+1;
            Serial.print("Folder Nb:");
            Serial.println(nextFolder);
            setSoundFolderNb(nextFolder);
        }
        else{
            Serial.print("Long press Button with ");
            Serial.println(durationMS);
            // turn on the DEBUG
            if (touchClass != nullptr){
                touchClass->enableMonitor(touchClass->getMonitorStatus()+1);    //we cycle through
            }
        }


    }
}