package org.firstinspires.ftc.teamcode.mechanism.drivetrains;

import static org.firstinspires.ftc.teamcode.utilities.MathHelper.map;

import com.qualcomm.robotcore.hardware.DcMotor;
import com.qualcomm.robotcore.hardware.Gamepad;

public class DifferentialDrive implements DriveTrain {
    Gamepad gamepad;
    DcMotor leftMotor;
    DcMotor rightMotor;

    protected final double minJoystick = -1.0;
    protected final double maxJoystick = 1.0;
    protected final double minSpeed = -1.0;
    protected final double maxSpeed = 1.0;

    private double getLeftPower() {
        return gamepad.left_stick_y;
    }
    private double getRightPower() {
        return gamepad.right_stick_y;
    }

    @Override
    public void init(Gamepad gp, DcMotor... motors) {
        this.gamepad = gp;
        this.leftMotor = motors[0];
        this.rightMotor = motors[1];

        this.leftMotor.setDirection(DcMotor.Direction.REVERSE);
        this.leftMotor.setMode(DcMotor.RunMode.RUN_USING_ENCODER);
        this.rightMotor.setMode(DcMotor.RunMode.RUN_USING_ENCODER);
    }

    @Override
    public void update() {
        double x = -gamepad.right_stick_x;
        double y = -gamepad.left_stick_y;

        double hypotenuse = Math.sqrt(x * x + y * y);
        double angleRadians = Math.acos(Math.abs(x) / hypotenuse);
        double angleDegrees = angleRadians * 180 / Math.PI;

        double turnCoefficient = -1 + (angleDegrees / 90) * 2;
        double turnValue = turnCoefficient * Math.abs(Math.abs(y) - Math.abs(x));
        turnValue = Math.round(turnValue * 100.0) / 100.0;  // Round to two decimal places

        double movementValue = Math.max(Math.abs(y), Math.abs(x));

        double rawLeft, rawRight;
        if ((x >= 0 && y >= 0) || (x < 0 && y < 0)) {
            rawLeft = movementValue;
            rawRight = turnValue;
        } else {
            rawRight = movementValue;
            rawLeft = turnValue;
        }

        if (y < 0) {
            rawLeft = -rawLeft;
            rawRight = -rawRight;
        }

        double rightOut = map(rawRight, minJoystick, maxJoystick, minSpeed, maxSpeed);
        double leftOut = map(rawLeft, minJoystick, maxJoystick, minSpeed, maxSpeed);

        leftMotor.setPower(leftOut);
        rightMotor.setPower(rightOut);
    }
}
