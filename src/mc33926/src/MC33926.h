#ifndef MC33926_H
#define MC33926_H

#include <Arduino.h>

/**
 * A wrapping class to operate a MC33926 motor driver
 *
 * Expected logic connections:
 * 
 *  VDD     5v
 *  EN      5v
 *  D1      0v
 *  D2      5v
 *  INV     0v
 *  SLEW    0v for low freq pwm, 5v for high freq pwm
 * 
 *  "fwd":
 *       IN1     PWM
 *       IN2     0v
 *  "reverse":
 *       IN1     0v
 *       IN2     PWM
 * 
 *  FB      0v w/ pull-down resistor (analog)
 *  SF      5v w/ pull-up resistor (digital)
 *
 * Single: https://www.pololu.com/product/1212
 * Dual: https://www.pololu.com/product/1213
 * MC33926 Datasheet: https://pololu.com/file/0J233/MC33926.pdf
 *
 * Uno pwm pins:
 *   3, 5, 6, 9, 10, 11, 490 Hz (pins 5 and 6: 980 Hz)
 *
 * Due pwm pins:
 *   2-13, 1000 Hz
 *   use analogWriteResolution to set higher resolution. i.e. "12" for 12-bit -> 4096 bit resolution 
 *   pins DAC0 and DAC1 are "true" analog. 
*/

#define PIN_NOT_CONNECTED -1
#define DEFAULT_MAX_SPEED_VALUE 255
#define DEFAULT_MIN_SPEED_VALUE -255
#define DEFAULT_MAX_12_BIT_SPEED_VALUE 4095
#define DEFAULT_MIN_12_BIT_SPEED_VALUE -4095

class MC33926 {

    public:
        MC33926(
            int in1Pin,
            int in2Pin
        );

        // See https://www.arduino.cc/reference/en/language/functions/zero-due-mkr-family/analogwriteresolution/
        // for details on the analogWriteResolution() method available in the Due
        // and other boards. The maximum number of bits for the write resolution is
        // 12 for the Due. With a value of 12, possible values for analogWrite are
        // between 0 and 4095. Sketches utilizing this constructor will function as
        // expected when compiling for the Arduino Due target; otherwise, the call
        // to analogWriteResolution() is not executed.

        MC33926(
            int in1Pin,
            int in2Pin,
            int writeResolution // num bits max is 12
        );

        /**
         * Identify the Arduino digital I/O pin on which to check if the status flag is set.
        */
        void setStatusFlagPin(int statusFlagPin);

        /**
         * Identify the Arduino digital I/O pin on which to measure current usage.
         * The referenceVoltage is the base voltage used by the Arduino (eg. 3.3 or 5.0)
        */
        void setCurrentSensePin(int fbCurrentSensePin, float referenceVoltage);

        void setSpeed(int speed);

        /**
         * Obtain the current usage for a motor in milliamps. setCurrentSensePin() must
         * be invoked at least once before this method.
        */
        float getCurrent();

        /**
         * Get the current value of the status flag output by the motor controller.
         * True if the status flag is set; false otherwise.
        */
        boolean getStatus();

        void setMaxPwmOutput(int maxOutput);

        /**
         * Register an interrupt service routine to execute upon reception of a high-to-low
         * event on the status flag input pin.
        */
        void registerStatusChangeHandler(void (*statusServiceRoutine)(void), int statusFlagPin);

    private:

        void init();

        int m_in1_pin;              // One half of the pwm/direction
        int m_in2_pin;              // One half of the pwm/direction

        int m_status_flag_pin;      // Pin for reading status of motor controller
        int m_current_sense_pin;    // Read pin for detecting current consumption

        float m_reference_voltage;  // for current sense. For example, 3.3 for Due, 5.0 for Uno

        int m_max_pwm_output;       // The maximum value to use when setting pwm signals
        int m_min_pwm_output;       // The minimum value to use when setting pwm signals. Typically
                                    // the negative of m_max_pwm_output

        int m_write_resolution;     // The number of bits to use when invoking analogWriteResolution()
};

#endif // MC33926_H