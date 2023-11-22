#include <Arduino.h>
#include <touch.h>
#define TOUCH_USE_INTERRUPT
#define TOUCH_DEADTIME_MS 500

uint8_t touchpins[TOUCH_NB_OF_BUTTONS]={13,12,14};
touch_value_t touchThreshold[TOUCH_NB_OF_BUTTONS]={29,29,29};
bool touchDetected[TOUCH_NB_OF_BUTTONS];

// An Pin 2 hängt die blaue LED!



//---------------- The touch Functions ---------------
void gotTouch0(){
 touchDetected[0] = true;
}

void gotTouch1(){
 touchDetected[1] = true;
}

void gotTouch2(){
 touchDetected[2] = true;
}

void gotTouch3(){
 touchDetected[3] = true;
}
void (*touchButtonISRs[TOUCH_MAX_NB_OF_BUTTONS])() = {gotTouch0, gotTouch1, gotTouch2};
//-------------------------------------

void touchSensors::touchEsetup(void (*callback)(int)){
    Serial.println("Init touch - ESP");
    this->touchEvent=callback;
    
    for(int i=0; i<TOUCH_NB_OF_BUTTONS;i++){
        touchDetected[i]=false;
        touchAttachInterrupt(touchpins[i], touchButtonISRs[i], touchThreshold[i]);
        touchSleepWakeUpEnable(touchpins[i], touchThreshold[i]);
    }

    #ifdef TOUCH_USE_INTERRUPT
        

    #endif

}

bool touchSensors::touchloop(){
    bool buttonPressed = false;
    for (int btn=0; btn<TOUCH_NB_OF_BUTTONS;btn++){
        

    
        #ifndef TOUCH_USE_INTERRUPT
            uint8_t button=touchpins[btn];
            int touchValue= touchRead(button);
            detectTouch(btn,touchValue);
            #ifdef DEBUG
                Serial.print(touchValue) ;
                Serial.print("\t");
                    
            #endif
        #else
            // Interrupt driven
            if(touchDetected[btn]==true){
                touchDetected[btn]=false;
                callBackTouchEvent(btn);
                buttonPressed=true;
            }


        #endif

        
        
        
        
        //Serial.print(ispressed);
        //Serial.print("\t");   
    }
    #ifdef DEBUG
    Serial.println();
    #endif
    return buttonPressed;



}

void  touchSensors::callBackTouchEvent(int buttonNb){
    if(lastTouch+TOUCH_DEADTIME_MS > millis()){
        #ifdef DEBUG
            Serial.println("Skip Touch");
        #endif
        lastTouch = millis();
        return;
    }
    lastTouch = millis();
    Serial.print("Exec. TouchCallback for button");
    Serial.println(buttonNb);
    if (touchEvent != nullptr){
        (*touchEvent)(buttonNb);
    }
    else{
        Serial.println("No Callback was set!");
    }
}

bool touchSensors::detectTouch(int button, int value){
    if(value <30){
       callBackTouchEvent(button);
       return true;
    }
    return false;

}