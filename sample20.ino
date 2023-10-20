#include <Zumo32U4.h>

#define NUM_SENSORS 5

Zumo32U4Buzzer      buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors      motors;
Zumo32U4ButtonA     buttonA;
Zumo32U4LCD         display;

const uint16_t maxSpeed = 400;
int16_t lastError       = 0;
unsigned int lineSensorValues[NUM_SENSORS];

void loadCustomCharacters()
{
    static const char levels[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
    };
    display.loadCustomCharacter(levels + 0, 0);  // 1 bar
    display.loadCustomCharacter(levels + 1, 1);  // 2 bars
    display.loadCustomCharacter(levels + 2, 2);  // 3 bars
    display.loadCustomCharacter(levels + 3, 3);  // 4 bars
    display.loadCustomCharacter(levels + 4, 4);  // 5 bars
    display.loadCustomCharacter(levels + 5, 5);  // 6 bars
    display.loadCustomCharacter(levels + 6, 6);  // 7 bars
}

void printBar(uint8_t height)
{
    if (height > 8) { height = 8; }
    const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, (char)255};
    display.print(barChars[height]);
}

void calibrateSensors()
{
    display.clear();
    delay(1000);
    for(uint16_t i = 0; i < 120; i++)
    {
        if (i > 30 && i <= 90)
        {
            motors.setSpeeds(-200, 200);
        }
        else
        {
            motors.setSpeeds(200, -200);
        }
        lineSensors.calibrate();
    }
    motors.setSpeeds(0, 0);
}

void showReadings()
{
    display.clear();
    while(!buttonA.getSingleDebouncedPress())
    {
        lineSensors.readCalibrated(lineSensorValues);
        display.gotoXY(0, 0);
        for (uint8_t i = 0; i < NUM_SENSORS; i++)
        {
            uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
            printBar(barHeight);
        }
    }
}

void setup()
{



}

void loop()
{



}