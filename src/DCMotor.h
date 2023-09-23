
#pragma once

#include "common/base_classes/FOCMotor.h"
#include "drivers/DCDriver.h"

/**
 * Class implementing control of DC motors.
 * 
 * SimpleFOC is really for FOC, but it can also be interesting to use its other functionality to control DC motors.
 * This class implements closed-loop control of DC motors based on a position sensor on the motor shaft.
 * 
 * Open-loop control is not implemented. If you want to do open-loop voltage control of DC motors, you can directly
 * use the DCDriver classes. Open-loop velocity or position modes are not supported.
 * 
 */
class DCMotor : public FOCMotor {
public:
    DCMotor();

    /**
     * Function linking a motor and a pwm driver 
     * 
     * @param driver DCDriver class implementing all the hardware specific functions necessary PWM setting
     */
    void linkDriver(DCDriver* driver);

    /** 
      * DCDriver link
      */
    DCDriver* driver;
    
    /**  Motor hardware init function */
  	void init() override;
    /** Motor disable function */
  	void disable() override;
    /** Motor enable function */
    void enable() override;


    /**
     * Function executing the control loops set by the controller parameter of the DCMotor.
     * 
     * @param new_target  Either voltage, angle or velocity based on the motor.controller
     *                    If it is not set the motor will use the target set in its variable motor.target
     * 
     * This function doesn't need to be run upon each loop execution - depends of the use case
     */
    void move(float new_target = NOT_SET) override;

   /**
    * @param Uq Voltage to set to the motor
    * @param Ud ignored for DC motors
    * @param angle_el ignored for DC motors
    */
    virtual void setPhaseVoltage(float Uq, float Ud, float angle_el) override;

    // not implemented for DC motors
    virtual int initFOC() override; 
    // not implemented for DC motors
    virtual void loopFOC() override;

};