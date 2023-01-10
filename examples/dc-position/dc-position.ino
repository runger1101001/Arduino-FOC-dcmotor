/**
 * Position control example.
 * This example controls a DC motor via a 2PWM-compatible driver hardware,
 * such as a L298N based driver.
 * The example is using a SC60228 magnetic position sensor to monitor the rotor position.
 * 
 * SimpleFOC's position mode is used to move the motor to specific positions (angles)
 * chosen by the user. Positive or negative values determine the direction of
 * rotation. The angle is expressed in radians.
 * 
 * SimpleFOC's commander object is used to enable serial control of the
 * desired angle. After connecting the serial console, type "M100" to
 * set the postion to 100 rad, or "M-2.5" to set it to negative 2.5
 * rad. 
 * Many other motor parameters can be set via the commander, please see 
 * our documentation for details on how to use it.
 */

#include <Arduino.h>

#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
#include "SimpleDCMotor.h"

#include "encoders/sc60228/MagneticSensorSC60228.h"

// DCMotor object
DCMotor motor = DCMotor();
// DCDriver object
// there are different types to choose from, please select the correct one
// that matches your motor driver hardware.
DCDriver2PWM driver = DCDriver2PWM(5, 6);
// Sensor object
// different sensor and encoder types are supported. Please choose the
// right type for your sensor. All the sensors are supported for use
// with DC motors, including the hardware-specific sensor drivers from
// our "SimpleFOC Drivers" library.
// This example uses a SPI absolute magnetic angle sensor of type Semiment SC60228
SPISettings mySC60228SPISettings(1000000, SC60228_BITORDER, SPI_MODE1);
MagneticSensorSC60228 sensor = MagneticSensorSC60228(10, mySC60228SPISettings);

// Commander object, used for serial control
Commander commander = Commander(Serial);
// motor control function - this is needed to link the incoming commands 
// to the motor object
void onMotor(char* cmd){ commander.motor(&motor, cmd); }



/**
 * Setup function, in which you should intialize sensor, driver and motor,
 * and the serial communications and commander object.
 * Before calling the init() methods of these objects you can set relevant
 * parameters on them. 
 */
void setup() {
  // to use serial control we have to initialize the serial port
  Serial.begin(115200); // init serial communication
  // wait for serial connection - doesn't work with all hardware setups
  // depending on your application, you may not want to wait
  while (!Serial) {};   // wait for serial connection
  // enable debug output to the serial port
  SimpleFOCDebug::enable();
  
  // basic driver setup - set power supply voltage
  driver.voltage_power_supply = 6.0f;
  // if you want, you can limit the voltage used by the driver.
  // This value has to be same as or lower than the power supply voltage.
  driver.voltage_limit = 6.0f;
  // Optionally set the PWM frequency.
  driver.pwm_frequency = 5000;
  // init driver
  driver.init();
  // init sensor
  sensor.init();
  pinMode(10, OUTPUT);
  // link driver to motor
  motor.linkDriver(&driver);
  // link sensor to motor
  motor.linkSensor(&sensor);

  // set a voltage limit on the motor, optional. The value set here
  // has to be lower than the power supply voltage.
  motor.voltage_limit = 6.0f;
  motor.velocity_limit = 500.0f;
  // control type - for this example we use position mode.
  motor.controller = MotionControlType::angle;
  motor.torque_controller = TorqueControlType::voltage;
  // init motor
  motor.init();
  // set the PID parameters for velocity control. Velocity PID is the basis also
  // for position mode. If you have not yet tested the velocity mode we strongly
  // suggest you do this first, and find the PID parameters for that mode as
  // the initial values to use here. 
  // Please consult our documentation and forums for tips on PID tuning. The values
  // can be different depending on your PSU voltage, the driver, the sensor
  // and the motor used.
  motor.PID_velocity.P = 0.05f;
  motor.PID_velocity.I = 0.02f;
  motor.PID_velocity.D = 0.0f;
  // output ramp limits the rate of change of the velocity, e.g. limits the
  // accelleration.
  motor.PID_velocity.output_ramp = 200.0f;
  // low pass filter time constant. higher values smooth the velocity measured
  // by the sensor, at the cost of latency and control responsiveness.
  // Generally speaking, the lower this value can be while still producing good
  // response, the better.
  motor.LPF_velocity.Tf = 0.01f;
  // angle P-controller P parameter setting. Normally this can
  // be set to a fairly high value.
  motor.P_angle.P = 15.0f;  
  // set the target velocity to 0, we use the commander to set it later
  motor.target = 0.0f;
  // enable motor
  motor.enable();
  // add the motor and its control function to the commander
  commander.add('M', onMotor, "dc motor");
  // enable monitoring on Serial port
  motor.useMonitoring(Serial);
  Serial.println("Initialization complete.");
}




void loop() {
  // call motor.move() once per iteration, ideally at a rate of 1kHz or more.
  // rates of more than 10kHz might need a delay, as the sensor may not be able to
  // update quickly enough (depends on sensor)
  motor.move(); // target position can be set via commander input

  // call commander.run() once per loop iteration, it will process incoming commands
  commander.run();

  // call motor.monitor() once per loop iteration, it will print the motor state
  motor.monitor();
}