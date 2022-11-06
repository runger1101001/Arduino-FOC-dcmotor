# SimpleFOC DCMotor


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

It is intended to be used with one of the [DCDriver classes](../../drivers/dc/).

You can use any of the SimpleFOC sensor classes from [this repository](../../encoders/) or the [main repository](https://github.com/simplefoc/Arduino-FOC/tree/master/src/sensors).

### Open loop control

Open loop control is *not supported* by this class. But you can open loop control a DC motor just by directly using the DCDriver class and its `driver.setPwm(volts)` method.

### Usage

```c++
#include <Arduino.h>

#include "SimpleFOC.h"
#include "DCMotor.h"
#include "drivers/DCDriver2PWM.h"


DCMotor motor = DCMotor();
DCDriver2PWM driver = DCDriver2PWM(2, 3);
MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, 10);

void setup() {

  driver.init();
  sensor.init();
  motor.linkDriver(&driver);
  motor.linkSensor(&sensor);

  driver.voltage_power_supply = 10.0f;
  driver.voltage_limit = 10.0f;
  motor.voltage_limit = 5.0f;
  motor.torque_controller = TorqueControlType::voltage;
  motor.controller = MotionControlType::torque;

  motor.init();

}

void loop() {
  motor.move(3.0f); // 3 Volts
}
```

More detailed examples can be found [here](examples/).
