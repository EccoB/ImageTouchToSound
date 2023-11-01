#ifndef TOUCHE_H
#define TOUCHE_H


#define AUTOCAL_MILLIS 500
#define TOUCH_SAMPLES  30
#define TOUCH_MAX_NB_OF_BUTTONS 3
#define TOUCH_NB_OF_BUTTONS 2

void touchloop();
void touchEsetup();
bool touchCheckIsPressed(int buttonNb);

// 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
// 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

#endif