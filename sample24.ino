#include <Zumo32U4.h>

Zumo32U4Encoders  encoders;
Zumo32U4Motors    motors;
Zumo32U4ButtonA   buttonA;
Zumo32U4LCD       lcd;

void setup()
{
    lcd.clear();
}

void move_forward(int Max_Speed, int E_Counter)
{
    int countsLeft, countsRight;
    delay(500);
    countsLeft  = encoders.getCountsAndResetLeft();
    countsRight = encoders.getCountsAndResetRight();
    for(int speed = 0; speed <= Max_Speed; speed++)
    {
        motors.setSpeeds(speed, speed);
        delay(10);
    }
    countsLeft  = encoders.getCountsAndResetLeft();
    countsRight = encoders.getCountsAndResetRight();
    do{
        countsLeft  = encoders.getCountsLeft();
        countsRight = encoders.getCountsRight();
    }while(countsLeft < E_Counter);
    ledRed(1);
    for(int speed = Max_Speed; speed >= 0; speed--)
    {
        motors.setSpeeds(speed, speed);
        delay(10);
    }
    ledRed(0);
}

void move_turn(int Speed_L, int Speed_R, int E_Counter)
{
    // 以下のコードを入力しなさい 
    // ↓ここから





    // ↑ここまで
}

void loop()
{
    lcd.gotoXY(0, 1);
    lcd.print("A Start");
    // ボタンAが押されたら動作を開始する
    buttonA.waitForButton();
    // 以下でmove_forward()やmove_turn()を組合せロボットを動かす
    move_forward(200, 1820);
    move_turn(200, 0, 2600);
    move_forward(200, 1820);
    move_turn(200, 0, 2600);
}