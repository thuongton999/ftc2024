#include <PID.h>

PID_Controller::PID_Controller() {}

void PID_Controller::init(double kp, double ki, double kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PID_Controller::set_kp(double kp)
{
    this->kp = kp;
}

void PID_Controller::set_ki(double ki)
{
    this->ki = ki;
}

void PID_Controller::set_kd(double kd)
{
    this->kd = kd;
}

void PID_Controller::set_target(double target)
{
    this->target = target;
}

double PID_Controller::update(double input)
{
    this->error = this->target - input;
    this->integral += this->error;
    this->derivative = this->error - this->last_error;
    this->last_error = this->error;
    return (this->kp * this->error) + (this->ki * this->integral) + (this->kd * this->derivative);
}