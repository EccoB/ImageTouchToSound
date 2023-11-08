#include <Arduino.h>
#include <touch.h>


uint8_t touchpins[TOUCH_NB_OF_BUTTONS]={15,4};
// An Pin 2 hängt die blaue LED!

void touchSensors::touchEsetup(void (*callback)(int)){
    Serial.println("Init touch - ESP");
    this->touchEvent=callback;

}

void touchSensors::touchloop(){


    for (int btn=0; btn<TOUCH_NB_OF_BUTTONS;btn++){
        uint8_t button=touchpins[btn];
        int touchValue= touchRead(button);
        Serial.print(touchValue) ;
        Serial.print("\t");
        //Serial.print(ispressed);
        //Serial.print("\t");   
    }
    Serial.println();


}