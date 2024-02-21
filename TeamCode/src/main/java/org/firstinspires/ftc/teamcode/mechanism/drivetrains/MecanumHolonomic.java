package org.firstinspires.ftc.teamcode.mechanism.drivetrains;

import static java.lang.Math.abs;
import static java.lang.Math.atan2;
import static java.lang.Math.cos;
import static java.lang.Math.hypot;
import static java.lang.Math.max;
import static java.lang.Math.sin;

import com.qualcomm.robotcore.hardware.DcMotor;
import com.qualcomm.robotcore.hardware.Gamepad;

public class MecanumHolonomic implements DriveTrain {
    static final double ONE_QUARTER_PI = Math.PI / 4;

    DcMotor frontLeft, frontRight, backLeft, backRight;
    Gamepad gamepad;

    @Override
    public void init(Gamepad gp, DcMotor... motors) {
        this.gamepad = gp;

        this.frontLeft = motors[0];
        this.frontRight = motors[1];
        this.backLeft = motors[2];
        this.backRight = motors[3];

        this.frontLeft.setMode(DcMotor.RunMode.RUN_USING_ENCODER);
        this.frontRight.setMode(DcMotor.RunMode.RUN_USING_ENCODER);
        this.backLeft.setMode(DcMotor.RunMode.RUN_USING_ENCODER);
        this.backRight.setMode(DcMotor.RunMode.RUN_USING_ENCODER);

        this.frontLeft.setDirection(DcMotor.Direction.REVERSE);
        this.backLeft.setDirection(DcMotor.Direction.REVERSE);
    }

    @Override
    public void update() {
        double x = -gamepad.left_stick_x;
        double y = gamepad.left_stick_y;
        double turn = -gamepad.right_stick_x;

        double theta = atan2(y, x);
        double power = hypot(x, y);

        double sine = sin(theta - ONE_QUARTER_PI);
        double cosine = cos(theta - ONE_QUARTER_PI);
        double max_rotation = max(abs(sine), abs(cosine));

        double frontLeftMotorSpeed = (power * cosine / max_rotation) + turn;
        double frontRightMotorSpeed = (power * sine / max_rotation) - turn;
        double backLeftMotorSpeed = (power * sine / max_rotation) + turn;
        double backRightMotorSpeed = (power * cosine / max_rotation) - turn;

        if ((power + abs(turn)) > 1)
        {
            turn = abs(turn);
            frontLeftMotorSpeed /= (power + turn);
            frontRightMotorSpeed /= (power + turn);
            backLeftMotorSpeed /= (power + turn);
            backRightMotorSpeed /= (power + turn);
        }

        frontLeft.setPower(frontLeftMotorSpeed);
        frontRight.setPower(frontRightMotorSpeed);
        backLeft.setPower(backLeftMotorSpeed);
        backRight.setPower(backRightMotorSpeed);
    }
}
