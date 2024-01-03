#include <Arduino.h>
#include <IR_Array.h>
#include <MotorDriver.h>
#include <PID.h>
#include "__define.h"

#define KP 80
#define KI 0
#define KD 320

int16_t weighted_x[NUM_IR_SENSORS] = {-30, -15, 15, 30};
uint8_t SENSOR_PINS[NUM_IR_SENSORS] = {
    SENSOR_1_PIN,
    SENSOR_2_PIN,
    SENSOR_3_PIN,
    SENSOR_4_PIN
};

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

// print calibrated values
void print_calibrated_values()
{
	Serial.println("Sensors\tS1\tS2\tS3\tS4");
	Serial.print("White\t");
	for (int i = 0; i < NUM_IR_SENSORS; i++)
	{
		Serial.print(ir_sensor_array.get_white(i));
		Serial.print("\t");
	}
	Serial.println();
	Serial.print("Black\t");
	for (int i = 0; i < NUM_IR_SENSORS; i++)
	{
		Serial.print(ir_sensor_array.get_black(i));
		Serial.print("\t");
	}
	Serial.println();
	Serial.print("Min\t");
	Serial.print(ir_sensor_array.get_calibrated_min());
	Serial.print("\tMax\t");
	Serial.print(ir_sensor_array.get_calibrated_max());
}

void print_line_position()
{
	Serial.println("\nSensors\tS1\tS2\tS3\tS4");
	Serial.print("Raw\t");
	for (int i = 0; i < NUM_IR_SENSORS; i++)
	{
		Serial.print(ir_sensor_array.get_ir(i));
		Serial.print("\t");
	}
	Serial.println();
	Serial.print("Avg\t");
	for (int i = 0; i < NUM_IR_SENSORS; i++)
	{
		Serial.print(ir_sensor_array.get_avg(i));
		Serial.print("\t");
	}
	Serial.print("Line Position: ");
	Serial.println(ir_sensor_array.get_line_position(weighted_x));
}

bool calibrating = true;
bool calibrated = false;
void TriggerCalibrateMode()
{
	calibrating = !calibrating;
	if (calibrated) {
		print_line_position();
		Serial.println("Bitmask: " + String(ir_sensor_array.get_bitmask(), BIN));
	}
}

void setup_calibrate_mode()
{
	pinMode(BUTTON, INPUT_PULLUP);
	attachInterrupt(BUTTON, TriggerCalibrateMode, RISING);
}

void calibrate_ir_array()
{
	motor_driver.turn_around_right();
	Serial.println("Calibrating...");
	for (;calibrating;)
	{
		ir_sensor_array.calibrate();
	}
	motor_driver.stop();
	Serial.println("Calibrated!");
	print_calibrated_values();
	calibrated = true;
}

void setup()
{
	Serial.begin(115200);
	setup_motor();
	setup_ir_array();
	setup_pid_controller();

	setup_calibrate_mode();
	calibrate_ir_array();

	delay(5000);
}

#define BASE_SPEED 3072
int32_t left_motor = BASE_SPEED;
int32_t right_motor = BASE_SPEED;
void loop()
{
	if (!calibrated) return;
	double line_pos = ir_sensor_array.get_line_position(weighted_x);
	// if (ir_sensor_array.get_bitmask() == 0b0000) {
	// 	// turn left 90 degrees
	// 	motor_driver.set_speed_percentage(-75, 100);
	// 	delay(200);
	// 	return;
	// }
	int32_t output = std::round(pid_controller.update(line_pos));
	left_motor = BASE_SPEED + output;
	right_motor = BASE_SPEED - output;
	motor_driver.set_speed(left_motor, right_motor);
}