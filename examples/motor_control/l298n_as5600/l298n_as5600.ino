
#include "SimpleFOC.h"
#include "DCMotor.h"
#include "drivers/DCDriver2PWM.h"

#define PIN_A 1
#define PIN_B 2
#define PIN_EN 5

DCDriver driver = DCDriver2PWM(PIN_A, PIN_B, PIN_EN);
DCMotor motor = DCMotor();

MagneticSensorI2C sensor = MagneticSensorI2C();


void setup() {
    Serial.begin(115200);
    SimpleFOCDebug::enable(); // use Serial for debug
    driver.init();
    sensor.init();
    motor.linkDriver(&driver);
    motor.linkSensor(&sensor);
    motor.init();
}



void loop() {
    motor.move();
}