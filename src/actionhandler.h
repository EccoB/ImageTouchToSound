#ifndef H_ACTIONHANDLER
#define H_ACTIONHANDLER

#include <touch.h>

class actionHandler{
public:
    actionHandler();
    bool loop();

    void ioPressed(unsigned int pin, unsigned long duration);
    void setTouchClass(touchSensors *touchClass);
    private:
        touchSensors* touchClass = nullptr;
        unsigned long lastCheck =0;
        unsigned long checkPeriod = 5;

};


#endif