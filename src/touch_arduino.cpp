#include "touch.h"
#include <CapacitiveSensor.h>
CapacitiveSensor* tbuttons[3] = {nullptr};
//CapacitiveSensor* tbuttons[3] ={CapacitiveSensor(4,2),CapacitiveSensor(4,6),CapacitiveSensor(4,8)};
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

void touchEsetup(){
    Serial.println("Init touch");
   tbuttons[0] = new CapacitiveSensor(6,2); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
   tbuttons[1] = new CapacitiveSensor(6,3); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
   tbuttons[2] = new CapacitiveSensor(6,4); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
   //tbuttons[3] = new CapacitiveSensor(6,5); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
   tbuttons[0]->set_CS_AutocaL_Millis(AUTOCAL_MILLIS);     // turn off autocalibrate on channel 1 - just as an example
   tbuttons[1]->set_CS_AutocaL_Millis(AUTOCAL_MILLIS);     // turn off autocalibrate on channel 1 - just as an example
   tbuttons[2]->set_CS_AutocaL_Millis(AUTOCAL_MILLIS);     // turn off autocalibrate on channel 1 - just as an example
   tbuttons[0]->set_CS_Timeout_Millis(500);
   tbuttons[1]->set_CS_Timeout_Millis(500);
   tbuttons[2]->set_CS_Timeout_Millis(500);
}

void touchloop(){


    for (int btn=0; btn<TOUCH_NB_OF_BUTTONS;btn++){
        bool ispressed =touchCheckIsPressed(btn);
        //Serial.print(ispressed);
        //Serial.print("\t");   
    }
    Serial.println();


}

bool touchCheckIsPressed(int buttonNb)
{
    if(buttonNb>=TOUCH_NB_OF_BUTTONS)
        return false;
    long total =  tbuttons[buttonNb]->capacitiveSensor(TOUCH_SAMPLES);
    Serial.print(total);
    Serial.print("\t");  
    if(total>30)
        return true;
    return false;
}