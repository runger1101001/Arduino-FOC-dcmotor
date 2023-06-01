

#include "./DCDriver1PWM.h"

DCDriver1PWM::DCDriver1PWM(int pinPWM, float threshold, int pinEN) {
    this->pinPWM = pinPWM;
    this->threshold = threshold;
    this->pinEN = pinEN;
    this->pwm_frequency = NOT_SET;
};


void DCDriver1PWM::configureMicroseconds(int hz, int min_us, int zero_us, int max_us, bool active_high){
    pwm_frequency = hz;
    this->active_high = active_high;
    pwm_period_us = 1000000.0f / hz;
    threshold = (float)zero_us / pwm_period_us;
    pwm_max = (float)max_us / pwm_period_us;
    pwm_min = (float)min_us / pwm_period_us;
    if (!active_high) {
        float temp = 1.0f - pwm_min;
        pwm_min = 1.0f - pwm_max;
        pwm_max = temp;
        threshold = 1.0f - threshold;
    }
    pwm_max = _constrain(pwm_max, 0.0f, 1.0f);
    pwm_min = _constrain(pwm_min, 0.0f, 1.0f);
    threshold = _constrain(threshold, 0.0f, 1.0f);
};



int DCDriver1PWM::init() {
    if (pinEN!=NOT_SET) {
        pinMode(pinEN, OUTPUT);
        digitalWrite(pinEN, enable_active_high ? LOW : HIGH);
    }
    pinMode(pinPWM, OUTPUT);
    params = _configure1PWM(pwm_frequency, pinPWM);
    initialized = (params!=SIMPLEFOC_DRIVER_INIT_FAILED);
    return params!=SIMPLEFOC_DRIVER_INIT_FAILED;
};


void DCDriver1PWM::setPwm(float U){
    U = _constrain(U, -voltage_limit, voltage_limit);
    if (U>dead_zone) {
        U = threshold + ( U/voltage_power_supply ) * (pwm_max - threshold);
        _writeDutyCycle1PWM(U, params);
        U = _constrain(U, pwm_min, pwm_max);
    } else if (U<-dead_zone) {
        if (!scale_reverse)
            U = threshold + ( U/voltage_power_supply ) * (pwm_max - threshold); // same scale as forward
        else
            U = threshold + ( U/voltage_power_supply ) * (threshold - pwm_min);      // full reverse scale
        U = _constrain(U, pwm_min, pwm_max);
        _writeDutyCycle1PWM(U, params);
    } else {
        _writeDutyCycle1PWM(threshold, params);
    }
};



void DCDriver1PWM::setPwmMicroseconds(int us){
    float U = (float)us / pwm_period_us;
    if (!active_high) 
        U = 1.0f - U;
    U = _constrain(U, pwm_min, pwm_max);
    _writeDutyCycle1PWM(U, params);
};