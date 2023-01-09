


#include "./DCDriver1PWM2Dir.h"


DCDriver1PWM2Dir::DCDriver1PWM2Dir(int pinPWM, int pinDIR1, int pinDIR2, int pinEN) {
    this->pinPWM = pinPWM;
    this->pinDIR1 = pinDIR1;
    this->pinDIR2 = pinDIR2;
    this->pinEN = pinEN;
    this->pwm_frequency = NOT_SET;
};


int DCDriver1PWM2Dir::init() {
    if (pinEN!=NOT_SET) {
        pinMode(pinEN, OUTPUT);
        digitalWrite(pinEN, enable_active_high ? LOW : HIGH);
    }
    pinMode(pinDIR1, OUTPUT);
    digitalWrite(pinDIR1, HIGH);
    pinMode(pinDIR2, OUTPUT);
    digitalWrite(pinDIR2, LOW);
    pinMode(pinPWM, OUTPUT);
    params = _configure1PWM(pwm_frequency, pinPWM);
    initialized = (params!=SIMPLEFOC_DRIVER_INIT_FAILED);
    return params!=SIMPLEFOC_DRIVER_INIT_FAILED;
};



void DCDriver1PWM2Dir::setPwm(float U){
    if (U>0.0f) {
        U = _constrain(U, 0.0f, voltage_limit);
        U = _constrain(U/voltage_power_supply,0.0f,1.0f);
        _writeDutyCycle1PWM(U, params);
        digitalWrite(pinDIR2, LOW);
        digitalWrite(pinDIR1, HIGH);
    } else if (U<0.0f) {
        U = _constrain(-U, 0.0f, voltage_limit);
        U = _constrain(U/voltage_power_supply,0.0f,1.0f);
        _writeDutyCycle1PWM(U, params);
        digitalWrite(pinDIR1, LOW);
        digitalWrite(pinDIR2, HIGH);
    } else {
        _writeDutyCycle1PWM(0, params);
    }
};