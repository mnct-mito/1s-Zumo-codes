#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;

const uint8_t sensorThreshold = 6;

void setup() {
    proxSensors.initFrontSensor();
    lcd.clear();
    lcd.print(F("Press A"));
    buttonA.waitForButton();
    lcd.clear();
}
void loop() {





}