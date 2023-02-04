#include <arduino.h>
#include "MC33926.h"

MC33926::MC33926(
    int in1Pin,
    int in2Pin)
{
        this->m_in1_pin = in1Pin;
        this->m_in2_pin = in2Pin;
        this->m_current_sense_pin = PIN_NOT_CONNECTED;
        this->m_status_flag_pin = PIN_NOT_CONNECTED;
        this->m_max_pwm_output = DEFAULT_MAX_SPEED_VALUE;
        this->m_min_pwm_output = DEFAULT_MIN_SPEED_VALUE;

        this->init();
}

void MC33926::init() {

    pinMode(this->m_in1_pin, OUTPUT);
    pinMode(this->m_in2_pin, OUTPUT);
}

void MC33926::setStatusFlagPin(int statusFlagPin) {

    this->m_status_flag_pin = statusFlagPin;
    pinMode(this->m_status_flag_pin, INPUT_PULLUP);
}

void MC33926::setCurrentSensePin(int fbCurrentSensePin, float referenceVoltage) {

    this->m_current_sense_pin = fbCurrentSensePin;
    pinMode(this->m_current_sense_pin, INPUT);

    this->m_reference_voltage = referenceVoltage;
}

// speed values should be -255 to 0 to 255
// Convention is the following:
//   positive (or 0) speed value: IN1 = pwm, IN2 = 0
//   negative speed value:        IN1 = 0, IN2 = pwm
void MC33926::setSpeed(int speed) {

    int speedValue = constrain(speed, this->m_min_pwm_output, this->m_max_pwm_output);

    int outputSpeedValue = abs(speedValue);

    if (speedValue >= 0) {
        analogWrite(this->m_in1_pin, outputSpeedValue);
        analogWrite(this->m_in2_pin, 0);
    } else {
        analogWrite(this->m_in1_pin, 0);
        analogWrite(this->m_in2_pin, outputSpeedValue);
    }
}

float MC33926::getCurrent() {

    if (this->m_current_sense_pin == PIN_NOT_CONNECTED) {
        return 0;
    }

    int currentValue = analogRead(this->m_current_sense_pin);

    // TODO: convert to mA (or A?) based on 1023 possible values
    // depends on voltage of controller
    // 1023 = 3.3V for Due
    return (((float)currentValue)/1023.0)*this->m_reference_voltage;
}

boolean MC33926::getStatus() {

    if (this->m_status_flag_pin == PIN_NOT_CONNECTED) {
        // indicate no status on flag output
        return false;
    }

    // SF is active LOW
    return (LOW == digitalRead(this->m_status_flag_pin));
}

void MC33926::setMaxPwmOutput(int maxOutput) {

    if (maxOutput < 0) return;

    this->m_max_pwm_output = maxOutput;
    this->m_min_pwm_output = -maxOutput;
}

void MC33926::registerStatusChangeHandler(void (*statusServiceRoutine)(void), int statusFlagPin) {

    this->setStatusFlagPin(statusFlagPin);

    if (this-> m_status_flag_pin == PIN_NOT_CONNECTED) {
        return;
    }

    attachInterrupt(digitalPinToInterrupt(this->m_status_flag_pin), statusServiceRoutine, FALLING);
}