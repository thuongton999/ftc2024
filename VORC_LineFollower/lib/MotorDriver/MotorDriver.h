#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

#include <stdint-gcc.h>

// I'm using 40 MHz as the clock frequency
// so to use 12-bit resolution, the PWM frequency is 40 MHz / 2^12 = 9765.625 Hz
#define PWM_FREQ 9765.625      // 9,765625 kHz
#define PWM_RESOLUTION 12   // 12-bit resolution
#define PWM_MAX 4095        // 2^12 - 1

class MotorDriver {
    public:
        MotorDriver();
        void init_left_motor(uint8_t PWM_PIN_L_A, uint8_t PWM_PIN_L_B, uint8_t left_motor_channel_a, uint8_t left_motor_channel_b);
        void init_right_motor(uint8_t PWM_PIN_R_A, uint8_t PWM_PIN_R_B, uint8_t right_motor_channel_a, uint8_t right_motor_channel_b);
        void set_speed(int32_t left, int32_t right);
        void set_speed_percentage(double left, double right);
        void stop();
        void differential_drive(uint16_t velocity, int16_t omega);
        void turn_around_right();
        void turn_around_left();
    private:
        uint8_t PWM_PIN_L_A;
        uint8_t PWM_PIN_L_B;
        uint8_t PWM_PIN_R_A;
        uint8_t PWM_PIN_R_B;
        uint8_t left_motor_channel_a;
        uint8_t left_motor_channel_b;
        uint8_t right_motor_channel_a;
        uint8_t right_motor_channel_b;
};

#endif