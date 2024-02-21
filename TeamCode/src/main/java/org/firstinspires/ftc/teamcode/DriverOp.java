package org.firstinspires.ftc.teamcode;

import com.qualcomm.robotcore.eventloop.opmode.OpMode;
import com.qualcomm.robotcore.eventloop.opmode.TeleOp;
import com.qualcomm.robotcore.hardware.HardwareDevice;

import org.firstinspires.ftc.teamcode.mechanism.drivetrains.DriveTrain;
import org.firstinspires.ftc.teamcode.mechanism.drivetrains.MecanumHolonomic;
import org.firstinspires.ftc.teamcode.mechanism.pixelholder.PixelHolder;
import org.firstinspires.ftc.teamcode.__Hardware.ServoName;

@TeleOp(name="DriverOp", group="DriverOp")
public class DriverOp extends OpMode {
    PixelHolder pixelHolder;
    DriveTrain driveController;

    @Override
    public void init() {
        __Hardware robot = new __Hardware();

        pixelHolder = new PixelHolder(
                robot.getServo(ServoName.PIXEL_HOLDER_L),
                robot.getServo(ServoName.PIXEL_HOLDER_R));
        driveController = new MecanumHolonomic();

        driveController.init(gamepad1,
                robot.getMotor(__Hardware.MotorName.FRONT_LEFT),
                robot.getMotor(__Hardware.MotorName.FRONT_RIGHT),
                robot.getMotor(__Hardware.MotorName.BACK_LEFT),
                robot.getMotor(__Hardware.MotorName.BACK_RIGHT));

        for (HardwareDevice device : hardwareMap) {
            telemetry.addData("Device", device.getDeviceName());
        }
        telemetry.addData("Status", "Initialized");
        telemetry.update();
    }

    @Override
    public void loop() {
        telemetry.addData("Status", "Running");
        telemetry.update();

        if (gamepad1.a) {
            pixelHolder.trigger(PixelHolder.UseServo.LEFT);
        } else if (gamepad1.b) {
            pixelHolder.trigger(PixelHolder.UseServo.RIGHT);
        }
        driveController.update();
    }
}
