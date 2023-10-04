#include <Zumo32U4.h>

Zumo32U4Encoders  encoders;
Zumo32U4Motors    motors;
Zumo32U4ButtonA   buttonA;
Zumo32U4LCD       lcd;

void setup()
{
    lcd.clear();
}

void move(int SL, int SR, int EC)
{
    int cLeft, cRight;
    delay(500);
    // 左右のエンコーダを初期化し値を取得する
    cLeft  = encoders.getCountsAndResetLeft();
    cRight = encoders.getCountsAndResetRight();
    motors.setSpeeds(SL, SR);
    do{
        // 左右のエンコーダの値を取得する
        cLeft  = encoders.getCountsLeft();
        cRight = encoders.getCountsRight();
        lcd.clear();
        lcd.print(cLeft/10);
        lcd.gotoXY(4, 0);
        lcd.print(cRight/10);
    } while(abs(cLeft) < EC && abs(cRight) < EC);
    motors.setSpeeds(0, 0);
}

void loop()
{
    lcd.gotoXY(0, 1);
    lcd.print("A Start");
    buttonA.waitForButton();
    // 左右のモータの速度を200，エンコーダの値を9000まで動作させる
    move(200, 200, 9000);
}