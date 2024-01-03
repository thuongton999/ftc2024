#include <Arduino.h>
#include <MotorDriver.h>
#include <esp32-hal-ledc.h>

MotorDriver::MotorDriver() {}

void MotorDriver::init_left_motor(uint8_t PWM_PIN_L_A, uint8_t PWM_PIN_L_B, uint8_t left_motor_channel_a, uint8_t left_motor_channel_b) {
    this->PWM_PIN_L_A = PWM_PIN_L_A;
    this->PWM_PIN_L_B = PWM_PIN_L_B;
    this->left_motor_channel_a = left_motor_channel_a;
    this->left_motor_channel_b = left_motor_channel_b;
    ledcSetup(left_motor_channel_a, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(left_motor_channel_b, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN_L_A, left_motor_channel_a);
    ledcAttachPin(PWM_PIN_L_B, left_motor_channel_b);
}

void MotorDriver::init_right_motor(uint8_t PWM_PIN_R_A, uint8_t PWM_PIN_R_B, uint8_t right_motor_channel_a, uint8_t right_motor_channel_b) {
    this->PWM_PIN_R_A = PWM_PIN_R_A;
    this->PWM_PIN_R_B = PWM_PIN_R_B;
    this->right_motor_channel_a = right_motor_channel_a;
    this->right_motor_channel_b = right_motor_channel_b;
    ledcSetup(right_motor_channel_a, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(right_motor_channel_b, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN_R_A, right_motor_channel_a);
    ledcAttachPin(PWM_PIN_R_B, right_motor_channel_b);
}

void MotorDriver::set_speed(int32_t left, int32_t right) {
    left = constrain(left, -PWM_MAX, PWM_MAX);
    right = constrain(right, -PWM_MAX, PWM_MAX);
    if (left >= 0) {
        ledcWrite(left_motor_channel_a, left);
        ledcWrite(left_motor_channel_b, 0);
    } else {
        ledcWrite(left_motor_channel_a, 0);
        ledcWrite(left_motor_channel_b, -left);
    }
    if (right >= 0) {
        ledcWrite(right_motor_channel_a, right);
        ledcWrite(right_motor_channel_b, 0);
    } else {
        ledcWrite(right_motor_channel_a, 0);
        ledcWrite(right_motor_channel_b, -right);
    }
}

void MotorDriver::set_speed_percentage(double left, double right) {
    double left_speed = (left * PWM_MAX) / 100;
    double right_speed = (right * PWM_MAX) / 100;
    set_speed((int32_t)left_speed, (int32_t)right_speed);
}

void MotorDriver::stop() {
    ledcWrite(left_motor_channel_a, 0);
    ledcWrite(left_motor_channel_b, 0);
    ledcWrite(right_motor_channel_a, 0);
    ledcWrite(right_motor_channel_b, 0);
}

void MotorDriver::differential_drive(uint16_t velocity, int16_t omega) {
    int16_t left = velocity - omega;
    int16_t right = velocity + omega;
    set_speed(left, right);
}

void MotorDriver::turn_around_right() {
    ledcWrite(left_motor_channel_a, PWM_MAX);
    ledcWrite(left_motor_channel_b, 0);
    ledcWrite(right_motor_channel_a, 0);
    ledcWrite(right_motor_channel_b, PWM_MAX);
}

void MotorDriver::turn_around_left() {
    ledcWrite(left_motor_channel_a, 0);
    ledcWrite(left_motor_channel_b, PWM_MAX);
    ledcWrite(right_motor_channel_a, PWM_MAX);
    ledcWrite(right_motor_channel_b, 0);
}