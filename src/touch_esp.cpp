#include <Arduino.h>
#include <touch.h>
#define TOUCH_USE_INTERRUPT
#define TOUCH_DEADTIME_MS 500
#define MAXPHASE 2

uint8_t touchpins[TOUCH_NB_OF_BUTTONS]      =   {13,12,14};
touch_pad_t touchpads[TOUCH_NB_OF_BUTTONS]  =   {TOUCH_PAD_NUM4,TOUCH_PAD_NUM5,TOUCH_PAD_NUM6};
//touch_value_t touchThreshold[TOUCH_NB_OF_BUTTONS]={24,30,25}; //Prototyp
//touch_value_t touchThreshold[TOUCH_NB_OF_BUTTONS]={27,44,25};   //02
touch_value_t touchThreshold[TOUCH_NB_OF_BUTTONS]={26,38,23};   //03
int touchDetected[TOUCH_NB_OF_BUTTONS];

// An Pin 2 hängt die blaue LED!
// /* Mapping between touchPin and Pad
//     TOUCH_PAD_NUM0 = 0, /*!< Touch pad channel 0 is GPIO4(ESP32) */
//     TOUCH_PAD_NUM1,     /*!< Touch pad channel 1 is GPIO0(ESP32) / GPIO1(ESP32-S2) */
//     TOUCH_PAD_NUM2,     /*!< Touch pad channel 2 is GPIO2(ESP32) / GPIO2(ESP32-S2) */
//     TOUCH_PAD_NUM3,     /*!< Touch pad channel 3 is GPIO15(ESP32) / GPIO3(ESP32-S2) */
//     TOUCH_PAD_NUM4,     /*!< Touch pad channel 4 is GPIO13(ESP32) / GPIO4(ESP32-S2) */
//     TOUCH_PAD_NUM5,     /*!< Touch pad channel 5 is GPIO12(ESP32) / GPIO5(ESP32-S2) */
//     TOUCH_PAD_NUM6,     /*!< Touch pad channel 6 is GPIO14(ESP32) / GPIO6(ESP32-S2) */
//     TOUCH_PAD_NUM7,     /*!< Touch pad channel 7 is GPIO27(ESP32) / GPIO7(ESP32-S2) */
//     TOUCH_PAD_NUM8,     /*!< Touch pad channel 8 is GPIO33(ESP32) / GPIO8(ESP32-S2) */
//     TOUCH_PAD_NUM9,     /*!< Touch pad channel 9 is GPIO32(ESP32) / GPIO9(ESP32-S2) */



//---------------- The touch Functions ---------------
// These are the interrupts that get called when a touch is detected
void gotTouch0(){
 touchDetected[0]++;
}

void gotTouch1(){
 touchDetected[1]++;
}

void gotTouch2(){
 touchDetected[2]++;
}

void gotTouch3(){
 touchDetected[3]++;
}
void (*touchButtonISRs[TOUCH_MAX_NB_OF_BUTTONS])() = {gotTouch0, gotTouch1, gotTouch2};
//-------------------------------------

void touchSensors::touchEsetup(void (*callback)(int)){
    Serial.println("Init touch - ESP");
    this->touchEvent=callback;


    for(int i=0; i<TOUCH_NB_OF_BUTTONS;i++){
        touchDetected[i]=false;
    }

    //----Checking for wake-up cause ---
    esp_sleep_wakeup_cause_t wakeup_reason =esp_sleep_get_wakeup_cause();
    if(wakeup_reason == ESP_SLEEP_WAKEUP_TOUCHPAD){
        
        touch_pad_t touchPad = esp_sleep_get_touchpad_wakeup_status();
        Serial.print("TouchPad Pin: ");
        Serial.println(touchPad);
        for (int btn=0; btn<TOUCH_NB_OF_BUTTONS;btn++){
            if (touchPad ==touchpads[btn]){
                touchDetected[btn]=true;
                Serial.print("Touch Button detected:");
                Serial.print(btn);
                int touchValue= touchRead(touchpins[btn]);
                Serial.print(" Value:");
                Serial.println(touchValue);
            }
        }
             
    }

    // We can only assign the touchInterrupts after readout
    for(int i=0; i<TOUCH_NB_OF_BUTTONS;i++){
        touchAttachInterrupt(touchpins[i], touchButtonISRs[i], touchThreshold[i]);
        touchSleepWakeUpEnable(touchpins[i], touchThreshold[i]);
    }

}

void touchSensors::enableMonitor(int phase){

    if(phase>MAXPHASE)
        monitor=0;
    else
        monitor=phase;

    switch(phase){
        case 0: //normal operation
            for(int i=0; i<TOUCH_NB_OF_BUTTONS;i++){
                touchAttachInterrupt(touchpins[i], touchButtonISRs[i], touchThreshold[i]);
            }
        break;
        case 1: //monitor
            for(int i=0; i<TOUCH_NB_OF_BUTTONS;i++){
                touchDetachInterrupt(touchpins[i]);
                //touchAttachInterrupt(touchpins[i], touchButtonISRs[i], touchThreshold[i]);
            }
        break;
    }

}

int touchSensors::getMonitorStatus(){
    return monitor;
}

bool touchSensors::touchloop(){
    bool buttonPressed = false;



    for (int btn=0; btn<TOUCH_NB_OF_BUTTONS;btn++){


        // monitor enabled, overwrites the rest
        if(monitor>0){
            int touchValue= touchRead(touchpins[btn]);

            if(btn>0)
                Serial.print(",");
            
            Serial.print("Btn");
            Serial.print(btn);
            Serial.print("|");
            Serial.print(touchpins[btn]);
            Serial.print(":");

            //according to which phase, we do differnt stuff
            if(monitor==1){
                
                Serial.print(touchValue);
            }
            else{
                bool touchDetected = (touchValue<touchThreshold[btn]);
                Serial.print(touchDetected);

            }

            
            continue;           
        }
        

    
        #ifndef TOUCH_USE_INTERRUPT
            // Polling
            uint8_t button=touchpins[btn];
            int touchValue= touchRead(button);
            detectTouch(btn,touchValue);
            #ifdef DEBUG
                Serial.print(touchValue) ;
                Serial.print("\t");
                    
            #endif
        #else

            // Interrupt driven
            if(touchDetected[btn]){
                touchDetected[btn]=0;
                /*
                int touchValue= touchRead(touchpins[btn]);
                Serial.print("Exec Touch Button:");
                Serial.print(btn);
                Serial.print(" TouchRead:");
                Serial.print(touchValue);
                Serial.print(" TouchValue:");
                Serial.println(touchDetected[btn]);
                
                
                if (touchValue > touchThreshold[btn]){
                    Serial.println("Skip.. too heigh");
                    return true;
                }
                */
                buttonPressed=true;
                callBackTouchEvent(btn);
                
            }


        #endif

        
        

        
        
        //Serial.print(ispressed);
        //Serial.print("\t");   
    }

    if(monitor){
        Serial.println();
        buttonPressed=true;
    }
    #ifdef DEBUG
    Serial.println();
    #endif
    return buttonPressed;



}

void  touchSensors::callBackTouchEvent(int buttonNb){
    if(millis()>500 && (lastTouch+TOUCH_DEADTIME_MS > millis())){
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