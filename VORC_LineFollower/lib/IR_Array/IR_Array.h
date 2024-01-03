#ifndef __IR_ARRAY_H
#define __IR_ARRAY_H

#include <stdint-gcc.h>

#define NUM_IR_SENSORS 4
#define ADC_RESOLUTION 12

class IR_Sensor_Array
{
public:
    IR_Sensor_Array();
    void init(uint8_t pins[NUM_IR_SENSORS]);
    void calibrate();
    double get_line_position(int16_t x[NUM_IR_SENSORS]);
    double get_line_position(int16_t x[NUM_IR_SENSORS], double multiplier);
    uint16_t get_ir(uint8_t index);
    uint16_t get_white(uint8_t index);
    uint16_t get_black(uint8_t index);
    uint16_t get_avg(uint8_t index);
    uint16_t get_calibrated_min();
    uint16_t get_calibrated_max();
    uint8_t get_bitmask();
private:
    uint16_t y_min;
    uint16_t y_max;
    uint16_t y_offset;
    uint16_t x_max[NUM_IR_SENSORS];
    uint16_t x_min[NUM_IR_SENSORS];
    uint16_t x_avg[NUM_IR_SENSORS];
    uint16_t x_offset[NUM_IR_SENSORS];
    uint16_t __get_ir(uint8_t index);
    uint8_t SENSOR_PINS[NUM_IR_SENSORS];
    uint8_t BITMASK;
};

#endif