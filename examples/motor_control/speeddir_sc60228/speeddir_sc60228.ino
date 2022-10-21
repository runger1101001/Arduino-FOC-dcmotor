
#include "SimpleFOC.h"
#include "DCMotor.h"
#include "drivers/DCDriverSpeedDir.h"

#define PIN_PWM 1
#define PIN_DIR 2
#define PIN_EN 5

DCDriver driver = DCDriverSpeedDir(PIN_PWM, PIN_DIR, PIN_EN);
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