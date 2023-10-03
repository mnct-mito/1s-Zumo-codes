#include <Zumo32U4.h>

#define MaxSpeed 200

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;

void setup()
{
    lcd.clear();
    lcd.print("Press A");
}

void loop()
{
    // 以下のコードを入力しなさい
    bool buttonPress = buttonA.getSingleDebouncedPress();
    if(buttonPress == true)
    {
        for(int speed = 0; speed < MaxSpeed; speed++)
        {
            motors.setSpeeds(speed, speed);
            delay(5);
        }
        for(int speed = MaxSpeed; speed >= 0; speed--)
        {
            motors.setSpeeds(speed, speed);
            delay(5);
        }
    }
}