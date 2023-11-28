# SimpleDCMotor


![Library Compile](https://github.com/simplefoc/Arduino-FOC-dcmotor/workflows/Library%20Compile/badge.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Release](https://www.ardu-badge.com/badge/SimpleDCMotor.svg?)


Release 1.0.2 for SimpleFOC 2.3.1 or later

:warning: code in development! Please help us test it!


**Control DC motors using SimpleFOC's infrastructure.**

SimpleFOC is really intended for field oriented control of PMSM motors, it's in the name ;-). But for different reasons, sometimes DC motors are preferred, and while we're not focused on this use-case, we do have a fair amount of code that can be leveraged to help with DC-Motor control applications.

So this represents a less-supported effort to provide some useful building blocks for DC-Motors.

## DCMotor

This library contains an initial implementation of a DCMotor class for closed loop control of DC motors using SimpleFOC's infrastructure.

It supports closed loop motor control based on a position sensor on the motor shaft, with the following modes:

- Torque-voltage control
- Velocity control
- Position control
- Torque-current control (:warning: not yet implemented)

It is intended to be used with one of the [DCDriver classes](src/drivers/). Different driver classes are provided for commonly used driver control schemes.

You can use any of the SimpleFOC sensor classes from [the drivers repository](https://github.com/simplefoc/Arduino-FOC-drivers/tree/master/src/encoders) or the [main repository](https://github.com/simplefoc/Arduino-FOC/tree/master/src/sensors).

### Closed loop control

The closed loop control modes supported are:

- Velocity control, to set velocities to the motor and have the motor track them
- Position control, to set angles to the motor and have the motor move to the defined position (like a servo)
- Torque-current control (:warning: not yet implemented)


### Open loop control

Open loop control for DC motors is equivalent to torque-voltage mode. You can use torque-voltage mode (and no sensor is required). Or you can open loop control a DC motor just by directly using the DCDriver class and its `driver.setPwm(volts)` method.

### Usage

Use velocity mode to set specific speeds to the motor:

```c++
#include <Arduino.h>
#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
#include "SimpleDCMotor.h"
#include "encoders/sc60228/MagneticSensorSC60228.h"

DCMotor motor = DCMotor();
DCDriver2PWM driver = DCDriver2PWM(5, 6);

SPISettings mySC60228SPISettings(1000000, SC60228_BITORDER, SPI_MODE1);
MagneticSensorSC60228 sensor = MagneticSensorSC60228(10, mySC60228SPISettings);

Commander commander = Commander(Serial);
void onMotor(char* cmd){ commander.motor(&motor, cmd); }

void setup() {
  Serial.begin(115200);
  while (!Serial) {};
  SimpleFOCDebug::enable();
  
  driver.voltage_power_supply = 6.0f;
  driver.voltage_limit = 6.0f;
  driver.pwm_frequency = 5000;

  driver.init();
  sensor.init();
  motor.linkDriver(&driver);
  motor.linkSensor(&sensor);

  motor.voltage_limit = 6.0f;
  motor.velocity_limit = 500.0f;
  motor.controller = MotionControlType::velocity;
  motor.torque_controller = TorqueControlType::voltage;
  motor.init();
  
  motor.PID_velocity.P = 0.05f;
  motor.PID_velocity.I = 0.02f;
  motor.PID_velocity.D = 0.0f;
  motor.PID_velocity.output_ramp = 200.0f;
  motor.LPF_velocity.Tf = 0.01f;
  
  motor.target = 0.0f;
  motor.enable();
  
  commander.add('M', onMotor, "dc motor");
  motor.useMonitoring(Serial);
  Serial.println("Initialization complete.");
}

void loop() {
  motor.move();
  commander.run();
  motor.monitor();
}
```

In above example, the motor starts stopped, and you have to connect the serial console to the MCU to set a speed to the motor using our commander class. The motor will print the telemetry information to the Serial console at the same time, but don't worry, you can still type into it. Use M100 to set a speed of 100rad/s. Use M-25.5 to set a speed of -25.5rad/s (e.g. in the other direction).



Use torque-voltage mode to open-loop control a DC motor:

```c++
#include <Arduino.h>

#include "SimpleFOC.h"
#include "DCMotor.h"
#include "drivers/DCDriver2PWM.h"


DCMotor motor = DCMotor();
DCDriver2PWM driver = DCDriver2PWM(2, 3);

void setup() {

  driver.init();
  motor.linkDriver(&driver);

  driver.voltage_power_supply = 10.0f;
  driver.voltage_limit = 10.0f;
  driver.pwm_frequency = 5000;
  motor.torque_controller = TorqueControlType::voltage;
  motor.controller = MotionControlType::torque;

  motor.init();

}

void loop() {
  motor.move(3.0f); // 3 Volts
}
```

More detailed examples can be found [here](examples/).
