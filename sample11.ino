#include <Zumo32U4.h>

#define MaxSpeed 200

Zumo32U4Motors  motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LCD lcd;

int buttonB_state = 0;
int buttonC_state = 0;

void setup()
{
    lcd.clear();
    lcd.print("Press A");
    buttonA.waitForButton();
    lcd.clear();
}

void loop()
{
    buttonB_state = buttonB.isPressed();
    if(buttonB_state == 1) {
        motors.setSpeeds(MaxSpeed, MaxSpeed);
    }
    buttonC_state = buttonC.isPressed();
    if(buttonC_state == 1) {
        motors.setSpeeds(0, 0);
    }
}