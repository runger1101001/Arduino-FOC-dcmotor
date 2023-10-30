/**
 * DCMotor open loop control example.
 * We feel that SimpleFOC adds little value for controlling DC motors
 * in open-loop mode. Our library is really mainly for BLDC motor control
 * and open-loop control of DC motors does not really use much of our
 * code-base. It can usually be accomplished just by calling arduino's
 * analogWrite() function to set the PWM duty cycle.
 * 
 * Nonetheless, for people who wish to make use of our infrastructure for
 * this use case, here is a simple example.
 */

#include <Arduino.h>

#include "SimpleFOC.h"
#include "SimpleDCMotor.h"

// DCDriver object - this is the only thing needed for open-loop control.
// There are different types to choose from, please select the correct one
// that matches your motor driver hardware.
DCDriver1PWM1Dir driver = DCDriver1PWM1Dir(2, 3);

/**
 * Setup function, in which you should intialize the driver.
 * Before calling the init() methods of the driver, you can set
 * relevant parameters on it. 
 */
void setup() {
  // basic driver setup - set power supply voltage
  driver.voltage_power_supply = 10.0f;
  // if you want, you can limit the voltage used by the driver.
  // This value has to be same as or lower than the power supply voltage.
  driver.voltage_limit = 10.0f;
  // init driver
  driver.init();
}


/**
 * Loop function.
 * Call driver.setPwm() to set a voltage to the motor.
 * This voltage will be maintained until you set a different one.
 * There is no need to call driver.setPwm() regularly, and loop iteration 
 * speed does not matter for DCMotor open-loop control.
 */
void loop() {
  driver.setPwm(3.0f); // 3 Volts
}
