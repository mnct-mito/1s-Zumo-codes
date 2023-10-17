// 本サンプルプログラムは以下のコードを授業用に改編したものである
// https://github.com/pvcraven/zumo_32u4_examples/tree/master/MoveAndAvoid
// Copyright (c) 2016 Paul V Craven
// Released under the MIT license.
// see https://opensource.org/licenses/MIT

#include <Zumo32U4.h>
#include <Wire.h>

uint32_t turnAngle      = 0;
int16_t  turnRate;
int16_t  gyroOffset;
uint16_t gyroLastUpdate = 0;

void turnSensorSetup();
void turnSensorReset();
void turnSensorUpdate();

Zumo32U4LCD               lcd;
Zumo32U4ProximitySensors  proxSensors;
Zumo32U4ButtonA           buttonA;
Zumo32U4Motors            motors;
LSM303                    lsm303;
L3G                       gyro;

void turnSensorSetup(){
  Wire.begin();
  gyro.init();
  gyro.writeReg(L3G::CTRL1, 0b11111111);
  gyro.writeReg(L3G::CTRL4, 0b00100000);
  gyro.writeReg(L3G::CTRL5, 0b00000000);
  lcd.clear();
  lcd.print(F("Gyro cal"));
  ledYellow(1);
  delay(500);
  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++){
    while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
    gyro.read();
    total += gyro.g.z;
  }
  ledYellow(0);
  gyroOffset = total / 1024;
}

void turnSensorReset(){
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void turnSensorUpdate(){
  gyro.read();
  turnRate = gyro.g.z - gyroOffset;
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;
  int32_t d = (int32_t)turnRate * dt;
  turnAngle += (int64_t)d * 14680064 / 17578125;
}

enum State {
  pause_state,
  forward_left_state,
  forward_right_state,
  scan_left_state,
  scan_right_state,
  reverse_state,
  forward_state
};

const uint16_t motorSpeed = 350;
const uint16_t turnSpeed = 200;
const int acceleration = 2;
State state = pause_state;
int curSpeed = 0;

void turnLeft(int degrees) {
  turnSensorReset();
  motors.setSpeeds(-turnSpeed, turnSpeed);
  int angle = 0;
  do {
    delay(1);
    turnSensorUpdate();
    angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
    lcd.gotoXY(0, 0);
    lcd.print(angle);
    lcd.print(" ");
  } while (angle < degrees);
  motors.setSpeeds(0, 0);
}

void turnRight(int degrees) {
  turnSensorReset();
  motors.setSpeeds(turnSpeed, -turnSpeed);
  int angle = 0;
  do {
    delay(1);
    turnSensorUpdate();
    angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
    lcd.gotoXY(0, 0);
    lcd.print(angle);
    lcd.print(" ");
  } while (angle > -degrees);
  motors.setSpeeds(0, 0);
}

void stop() {
  motors.setSpeeds(0, 0);
}

void forward() {
  turnSensorUpdate();
  int angle = (((int32_t)turnAngle >> 16) * 360) >> 16;
  motors.setSpeeds(curSpeed + (angle * 5), curSpeed - (angle * 5));
}

void reverse() {
  motors.setSpeeds(-motorSpeed, -motorSpeed);
}

void scanLeft() {
  motors.setSpeeds(-curSpeed, curSpeed);
}

void scanRight() {
  motors.setSpeeds(curSpeed, -curSpeed);
}

void forwardLeft() {
  motors.setSpeeds(curSpeed / 2, curSpeed);
}

void forwardRight() {
  motors.setSpeeds(curSpeed, curSpeed / 2);
}

void setup() {
  proxSensors.initThreeSensors();
  Wire.begin();
  lsm303.init();
  lsm303.enableDefault();
  turnSensorSetup();
  delay(500);
  turnSensorReset();
}

void loop() {
  bool buttonPress = buttonA.getSingleDebouncedPress();
  proxSensors.read();
  int left_sensor = proxSensors.countsLeftWithLeftLeds();
  int centerLeftSensor = proxSensors.countsFrontWithLeftLeds();
  int centerRightSensor = proxSensors.countsFrontWithRightLeds();
  int right_sensor = proxSensors.countsRightWithRightLeds();
  lsm303.read();
  int16_t x = lsm303.a.x;
  int16_t y = lsm303.a.y;
  int32_t magnitudeSquared = (int32_t)x * x + (int32_t)y * y;
  if (state == pause_state) {
    if (buttonPress) {
      state = forward_state;
      turnSensorReset();
    }
  }
  else if (buttonPress) {
    state = pause_state;
  }
  else if (magnitudeSquared > 250000000) {
    state = reverse_state;
  }
  else if (state == scan_left_state) {
    if (centerLeftSensor < 2 && centerRightSensor < 2) {
      state = forward_state;
      turnSensorReset();
    }
  }
  else if (state == scan_right_state) {
    if (centerLeftSensor < 2 && centerRightSensor < 2) {
      state = forward_state;
      turnSensorReset();
    }
  }
  else if (state == forward_state) {
    if (centerLeftSensor >= 4 && centerRightSensor >= 4) {
      if (centerLeftSensor < centerRightSensor) {
        state = scan_left_state;
      } else {
        state = scan_right_state;
      }
    }
    else if (centerLeftSensor >= 2 && centerRightSensor >= 2) {
      if (centerLeftSensor < centerRightSensor) {
        state = forward_left_state;
      } else {
        state = forward_right_state;
      }
    }
  }
  else if (state == forward_left_state || state == forward_right_state) {
    if (centerLeftSensor < 2 && centerRightSensor < 2) {
      state = forward_state;
      turnSensorReset();
    }
    if (centerLeftSensor >= 4 && centerRightSensor >= 4) {
      if (centerLeftSensor < centerRightSensor) {
        state = scan_left_state;
      } else {
        state = scan_right_state;
      }
    }
  }
  if (state != pause_state && curSpeed < motorSpeed) {
    curSpeed += acceleration;
  }
  if (state == pause_state) {
    stop();
    curSpeed = 0;
  }
  else if (state == forward_state)
    forward();
  else if (state == forward_left_state)
    forwardLeft();
  else if (state == forward_right_state)
    forwardRight();
  else if (state == scan_left_state)
    scanLeft();
  else if (state == scan_right_state)
    scanRight();
  else if (state == reverse_state) {
    lcd.gotoXY(0, 0);
    lcd.print("Reverse   ");
    reverse();
    delay(250);
    turnLeft(135);
    curSpeed = 0;
    delay(250);
    state = forward_state;
    lcd.clear();
    turnSensorReset();
  }
  lcd.gotoXY(0, 0);
  if (state == pause_state)
    lcd.print("Stop     ");
  else if (state == forward_state)
    lcd.print("Forward  ");
  else if (state == forward_left_state)
    lcd.print("Left     ");
  else if (state == forward_right_state)
    lcd.print("Right     ");
  else if (state == scan_left_state)
    lcd.print("Scan Left ");
  else if (state == scan_right_state)
    lcd.print("Scan Right");
  lcd.gotoXY(0, 1);
  lcd.print(left_sensor);
  lcd.print(" ");
  lcd.print(centerLeftSensor);
  lcd.print(" ");
  lcd.print(centerRightSensor);
  lcd.print(" ");
  lcd.print(right_sensor);
  delay(2);
}
