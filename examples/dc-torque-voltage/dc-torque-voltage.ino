/**
 * DCMotor torque-voltage example.
 * For DC motors, torque-voltage mode is quite similar to open-loop control,
 * in the sense that unlike for BLDCs, the rotor position is not needed in
 * order to set a voltage to the motor.
 * 
 * So why include it? There are some advantages which may be of interest
 * to some users. For example, by using the sensor we can compute the velocity
 * and thereby estimate the back-electromotive force (BEMF) of the motor, and 
 * take this into account when setting voltage.
 * 
 * Nonetheless, we expect most users will be more intested in the velocity mode,
 * position mode or other torque control modes when working with SimpleFOC and 
 * DC motors.
 */
#include <Arduino.h>

#include "SimpleFOC.h"
#include "SimpleDCMotor.h"


// DCMotor object
DCMotor motor = DCMotor();
// DCDriver object
// there are different types to choose from, please select the correct one
// that matches your motor driver hardware.
DCDriver2PWM driver = DCDriver2PWM(2, 3);
// Sensor object
// different sensor and encoder types are supported. Please choose the
// right type for your sensor. All the sensors are supported for use
// with DC motors, including the hardware-specific sensor drivers from
// our "SimpleFOC Drivers" library.
// This example uses a SPI absolute magnetic angle sensor of type AMS AS5048A
MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, 10);



/**
 * Setup function, in which you should intialize sensor, driver and motor.
 * Before calling the init() methods of these objects you can set relevant
 * parameters on them. 
 */
void setup() {
  // basic driver setup - set power supply voltage
  driver.voltage_power_supply = 10.0f;
  // if you want, you can limit the voltage used by the driver.
  // This value has to be same as or lower than the power supply voltage.
  driver.voltage_limit = 10.0f;
  // often, you will want to set a lower PWM frequency than the default
  driver.pwm_frequency = 5000;
  // init driver
  driver.init();
  // init sensor
  sensor.init();
  // link driver to motor
  motor.linkDriver(&driver);
  // link sensor to motor
  motor.linkSensor(&sensor);

  // set a voltage limit on the motor, optional. The value set here
  // has to be lower than the power supply voltage.
  motor.voltage_limit = 5.0f;
  // choose control type - in this case we want to control the torque,
  // by setting a voltage. Really its like open loop.
  motor.controller = MotionControlType::torque;
  // set the torque control type to voltage for the simple torque mode.
  motor.torque_controller = TorqueControlType::voltage;
  // init motor
  motor.init();
}

/**
 * Loop function. Don't add delays or time-intensive code here, the motor
 * control loop should run at several kHz if possible.
 * 
 * Call the motor.move() function once per loop iteration, which will read the
 * sensor and update the voltage set to the motor to maintain the target speed.
 */
void loop() {
  // call motor.move() once per iteration, ideally at a rate of 1kHz or more.
  // rates of more than 10kHz might need a delay, as the sensor may not be able to
  // update quickly enough (depends on sensor)
  motor.move(3.0f); // 3 Volts
}
