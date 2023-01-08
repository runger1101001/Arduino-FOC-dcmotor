
#pragma once


#include "./DCDriver.h"


/**
 * Intended to drive DC motor drivers with one PWM speed control signal, and two direction control signals.
 * Speed is determined by PWM duty cycle, and direction is determined by the state of the direction control 
 * signals. Typically used with H-bridge motor drivers where the direction pins control the H-bridge and the
 * PWM pin controls the enable/speed pin.
 */
class DCDriver1PWM2Dir : public DCDriver {
    public:
        DCDriver1PWM2Dir(int pinPWM, int pinDIR1, int pinDIR2, int pinEN = NOT_SET);

        virtual int init() override;

        /** 
         * Set phase voltages to the harware 
         * Positive voltages are associated with the "forward" direction, negative voltages with the "reverse" direction
         * @param U voltage
        */
        virtual void setPwm(float U) override;

        int pinPWM;
        int pinDIR1;
        int pinDIR2;
};