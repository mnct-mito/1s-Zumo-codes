#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;

#define FORWARD_SPEED   200  // 前進速度
#define BACKWARD_SPEED  -200 // 後退速度
#define TURN_SPEED      300  // 旋回速度
#define BACKWARD_TIME   200  // 後退時間
#define TURN_TIME       300  // 旋回時間
#define LINE_THRESHOLD  500  // 白線検出のしきい値

unsigned int lineSensorValues[5];

void setup() 
{
    lineSensors.initFiveSensors();
    lcd.print("Press A");
    motors.setSpeeds(0, 0);
    buttonA.waitForButton();
    lcd.clear();
    delay(200);
}

void loop() 
{



} 