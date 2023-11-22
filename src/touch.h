#ifndef TOUCHE_H
#define TOUCHE_H


#define AUTOCAL_MILLIS 500
#define TOUCH_SAMPLES  30
#define TOUCH_MAX_NB_OF_BUTTONS 3
#define TOUCH_NB_OF_BUTTONS 3

class touchSensors{
    
    public:
        bool touchloop();
        void touchEsetup(void (*callback)(int));
        bool touchCheckIsPressed(int buttonNb);
        void callBackTouchEvent(int buttonNb);
    protected:
        void (*touchEvent)(int) = nullptr;
        bool detectTouch(int button, int value);
        unsigned long lastTouch=0;
  
// 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
// 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
};
#endif