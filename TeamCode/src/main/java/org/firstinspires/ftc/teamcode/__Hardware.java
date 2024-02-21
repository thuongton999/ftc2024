package org.firstinspires.ftc.teamcode;

import static org.firstinspires.ftc.robotcore.external.BlocksOpModeCompanion.hardwareMap;

import com.qualcomm.robotcore.hardware.DcMotor;
import com.qualcomm.robotcore.hardware.Servo;

public class __Hardware {
    private Servo[] servos;
    private DcMotor[] motors;

    public enum MotorName {
        FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT
    }

    public enum ServoName {
        PIXEL_HOLDER_L, PIXEL_HOLDER_R
    }

    private void initNames() {
        for (MotorName name : MotorName.values()) {
            motors[name.ordinal()] = hardwareMap.dcMotor.get(name.name());
        }

        for (ServoName name : ServoName.values()) {
            servos[name.ordinal()] = hardwareMap.servo.get(name.name());
        }
    }

    public __Hardware() {
        this.servos = new Servo[ServoName.values().length];
        this.motors = new DcMotor[MotorName.values().length];

        initNames();
    }

    public DcMotor getMotor(MotorName name) {
        return motors[name.ordinal()];
    }

    public Servo getServo(ServoName name) {
        return servos[name.ordinal()];
    }
}
