#include <Arduino.h>
#include <IR_Array.h>
#include <MotorDriver.h>
#include <PID.h>
#include "__define.h"

// #define KP 100
// #define KI 0
// #define KD 460
#define KP 0.135
#define KI 0
#define KD 20

int16_t weighted_x[NUM_IR_SENSORS] = {-30, -15, 15, 30};
uint8_t SENSOR_PINS[NUM_IR_SENSORS] = {
	SENSOR_1_PIN,
	SENSOR_2_PIN,
	SENSOR_3_PIN,
	SENSOR_4_PIN};

MotorDriver motor_driver;
IR_Sensor_Array ir_sensor_array;
PID_Controller pid_controller;

void setup_motor()
{
	motor_driver.init_left_motor(
		PWM_PIN_L_A,
		PWM_PIN_L_B,
		LEFT_MOTOR_CHANNEL_A,
		LEFT_MOTOR_CHANNEL_B);
	motor_driver.init_right_motor(
		PWM_PIN_R_A,
		PWM_PIN_R_B,
		RIGHT_MOTOR_CHANNEL_A,
		RIGHT_MOTOR_CHANNEL_B);
	motor_driver.stop();
}

void setup_ir_array()
{
	ir_sensor_array.init(SENSOR_PINS);
	pinMode(WHITE_LED, OUTPUT);
	pinMode(IR_ARRAY_LED, OUTPUT);

	digitalWrite(WHITE_LED, HIGH);
	digitalWrite(IR_ARRAY_LED, HIGH);
}

void setup_pid_controller()
{
	pid_controller.init(KP, KI, KD);
	pid_controller.set_target(0);
}

bool calibrating = true;
void calibrate_ir_array()
{
	motor_driver.turn_around_right();
	Serial.println("Calibrating...");
	for (; calibrating;)
	{
		ir_sensor_array.calibrate();
	}
	motor_driver.stop();
	digitalWrite(WHITE_LED, LOW);
	Serial.println("Calibrated!");
}
void TriggerCalibrateMode()
{
	calibrating = !calibrating;
}
void setup_interrupt()
{
	pinMode(BUTTON, INPUT_PULLUP);
	attachInterrupt(BUTTON, TriggerCalibrateMode, RISING);
}

void drive_motor(double line_pos)
{
	int32_t output = std::round(pid_controller.update(line_pos));
	int32_t left_motor = PWM_MAX + output;
	int32_t right_motor = PWM_MAX - output;

	motor_driver.set_speed(left_motor, right_motor);
}

void setup()
{
	Serial.begin(115200);
	setup_motor();
	setup_ir_array();
	setup_pid_controller();
	setup_interrupt();

	calibrate_ir_array();
}

void loop()
{
	double line_pos = ir_sensor_array.get_avg_line_position(weighted_x);
	if (ir_sensor_array.get_bitmask() == 0b0000)
	{
		// turn left 90 degrees
		motor_driver.set_speed_percentage(-75, 100);
		delay(200);
		return;
	}
	drive_motor(line_pos);
}