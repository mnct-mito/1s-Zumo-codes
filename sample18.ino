#include <Zumo32U4.h>

Zumo32U4LCD               lcd;
Zumo32U4ButtonA           buttonA;
Zumo32U4ButtonB           buttonB;
Zumo32U4Motors            motors;
Zumo32U4LineSensors       lineSensors;
Zumo32U4ProximitySensors  proxSensors;

#define FORWARD_SPEED   150
#define BACKWARD_SPEED  300
#define ATACK_SPEED     400
#define TURN_SPEED      200
#define BACKWARD_TIME   200
#define TURN_TIME       200
#define LINE_THRESHOLD  500

unsigned int  lineSensorValues[5];
const uint8_t sensorThreshold = 6;
int           buttonB_state   = 0;

void setup() 
{
  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();
  lcd.print("Press A");
  motors.setSpeeds(0, 0);
  buttonA.waitForButton();
  lcd.clear();
  delay(200);
}

void loop() 
{
  buttonB_state = buttonB.isPressed();
  while (buttonB_state == 1){
    lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
    if(lineSensorValues[0] < LINE_THRESHOLD){



    }
    else if(lineSensorValues[4] < LINE_THRESHOLD){



    }
    else{



    }
  }
}