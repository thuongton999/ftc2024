#ifndef __PID_H
#define __PID_H

class PID_Controller {
    public:
        PID_Controller();
        void init(double kp, double ki, double kd);
        void set_kp(double kp);
        void set_ki(double ki);
        void set_kd(double kd);
        void set_target(double target);
        double update(double input);
    private:
        double kp;
        double ki;
        double kd;
        double target;
        double error;
        double integral;
        double derivative;
        double last_error;
};

#endif