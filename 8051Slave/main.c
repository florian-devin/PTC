

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "config_globale.h"
#include "PTC_timer.h"



void main(void) {
setup();
while(1)
    loop();
}

void setup(void) {
    Init_Device();
}

void loop(void) {

}