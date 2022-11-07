/**
 * Velocity control example
 * This example controls a DC motor via a 2PWM-compatible driver hardware,
 * using a AS5048A magnetic position sensor to monitor the rotor position.
 * 
 * SimpleFOC's velocity mode is used to drive the motor at a constant set
 * velocity. This velocity will be maintained as the load increases (for 
 * example because the vehicle being powered is going uphill), but is
 * subject to the limits for velocity and voltage set on driver and motor.
 */

#include <Arduino.h>

#include "SimpleFOC.h"
#include "SimpleFOCDCMotor.h"

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
  // choose control type - in this case we want to control the velocity.
  motor.controller = MotionControlType::velocity;
  // set the PID parameters for velocity control. Please consult our
  // documentation and forums for tips on PID tuning. The values
  // can be different depending on your PSU voltage, the driver, the sensor
  // and the motor used.
  motor.PID_velocity.P = 0.4f;
  motor.PID_velocity.I = 1.2f;
  motor.PID_velocity.D = 0.0f;
  // output ramp limits the rate of change of the velocity, e.g. limits the
  // accelleration.
  motor.PID_velocity.output_ramp = 200.0f;
  // low pass filter time constant. higher values smooth the velocity measured
  // by the sensor, at the cost of latency and control responsiveness.
  // Generally speaking, the lower this value can be while still producing good
  // response, the better.
  motor.LPF_velocity.Tf = 0.01f;
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
  motor.move(10.0f); // 10 rad/s == 95.5 RPM
}