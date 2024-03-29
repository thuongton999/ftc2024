#include <Arduino.h>
#include <EEPROM.h>
#include <IR_Array.h>
#include <math.h>

IR_Sensor_Array::IR_Sensor_Array() {}

void IR_Sensor_Array::init(uint8_t pins[NUM_IR_SENSORS])
{
    uint16_t max_x = std::pow(2, ADC_RESOLUTION) - 1;
    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        this->x_max[i] = 0;
        this->x_min[i] = max_x;
    }
    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        this->SENSOR_PINS[i] = pins[i];
    }
}

uint16_t IR_Sensor_Array::__get_ir(uint8_t index)
{
    return analogRead(SENSOR_PINS[index]);
}
void IR_Sensor_Array::__cache_x()
{
    for (int i = 0; i < NUM_IR_SENSORS; i++)
    {
        this->x_offset[i] = this->x_max[i] - this->x_min[i];
        this->x_avg[i] = (this->x_max[i] + this->x_min[i]) / 2;
    }
}
void IR_Sensor_Array::__cache_y()
{
    this->y_offset = this->y_max - this->y_min;
}

void IR_Sensor_Array::calibrate()
{
    uint16_t x_max_total = 0;
    uint16_t x_min_total = 0;
    uint16_t ir_val;

    for (int i = 0; i < NUM_IR_SENSORS; i++)
    {
        ir_val = __get_ir(i);
        this->x_min[i] = fmin(this->x_min[i], ir_val);
        this->x_max[i] = fmax(this->x_max[i], ir_val);
        this->y_min = (this->y_min + this->x_min[i]) / 2;
        this->y_max = (this->y_max + this->x_max[i]) / 2;
    }
    __cache_x();
    __cache_y();
}

uint16_t IR_Sensor_Array::get_ir(uint8_t index)
{
    int16_t x = __get_ir(index) - this->x_min[index];
    return (this->y_min + ((double)this->y_offset / this->x_offset[index] * x));
}

double IR_Sensor_Array::get_line_position(int16_t x[NUM_IR_SENSORS])
{
    BITMASK = 0;
    double mass = 0;
    double total_weight = 0;
    static int16_t y;

    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        y = get_ir(i);
        mass += x[i] * y;
        total_weight += y;
        BITMASK |= (y >= this->x_avg[i]) << i;
    }
    return (mass / total_weight);
}
double IR_Sensor_Array::get_line_position(int16_t x[NUM_IR_SENSORS], double multiplier)
{
    return (get_line_position(x) * multiplier);
}
double IR_Sensor_Array::get_avg_line_position(int16_t x[NUM_IR_SENSORS])
{
    BITMASK = 0;
    double line_pos = 0;
    static int16_t y;

    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        y = get_ir(i);
        line_pos += x[i] * y; 
        BITMASK |= (y >= this->x_avg[i]) << i;
    }
    return line_pos / NUM_IR_SENSORS;
}

uint16_t IR_Sensor_Array::get_white(uint8_t index)
{
    return this->x_max[index];
}
uint16_t IR_Sensor_Array::get_black(uint8_t index)
{
    return this->x_min[index];
}
uint16_t IR_Sensor_Array::get_avg(uint8_t index)
{
    return this->x_avg[index];
}
uint16_t IR_Sensor_Array::get_calibrated_min()
{
    return this->y_min;
}
uint16_t IR_Sensor_Array::get_calibrated_max()
{
    return this->y_max;
}
uint8_t IR_Sensor_Array::get_bitmask()
{
    return this->BITMASK;
}

void IR_Sensor_Array::read_from_eeprom(uint16_t addr)
{
    EEPROM.get(addr, this->y_min);
    addr += sizeof(this->y_min);
    EEPROM.get(addr, this->y_max);
    addr += sizeof(this->y_max);
    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        EEPROM.get(addr, this->x_min[i]);
        addr += sizeof(this->x_min[i]);
        EEPROM.get(addr, this->x_max[i]);
        addr += sizeof(this->x_max[i]);
    }
    __cache_x();
    __cache_y();
}

void IR_Sensor_Array::save_to_eeprom(uint16_t addr)
{
    EEPROM.begin(sizeof(this->y_min) + sizeof(this->y_max) + sizeof(this->x_min) + sizeof(this->x_max));
    EEPROM.put(addr, this->y_min);
    addr += sizeof(this->y_min);
    EEPROM.put(addr, this->y_max);
    addr += sizeof(this->y_max);
    for (uint8_t i = 0; i < NUM_IR_SENSORS; i++)
    {
        EEPROM.put(addr, this->x_min[i]);
        addr += sizeof(this->x_min[i]);
        EEPROM.put(addr, this->x_max[i]);
        addr += sizeof(this->x_max[i]);
    }
    EEPROM.commit();
}