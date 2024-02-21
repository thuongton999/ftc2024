package org.firstinspires.ftc.teamcode.mechanism.drivetrains;

import com.qualcomm.robotcore.hardware.DcMotor;
import com.qualcomm.robotcore.hardware.Gamepad;

public interface DriveTrain {
    void init(Gamepad gp, DcMotor... motors);

    void update();
}
