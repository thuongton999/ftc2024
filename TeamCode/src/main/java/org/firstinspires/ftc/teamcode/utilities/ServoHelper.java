package org.firstinspires.ftc.teamcode.utilities;

public class ServoHelper {
//    REV Robotics Smart Robot Servo
    static final double MAX_DEGREES = 280;
    static final double MIN_DEGREES = 0;

    public static double degreesToPosition(double degrees) {
        if (degrees < MIN_DEGREES) {
            return 0;
        } else if (degrees > MAX_DEGREES) {
            return 1;
        }
        return degrees / MAX_DEGREES;
    }

    public static double positionToDegrees(double position) {
        if (position < 0) {
            return 0;
        } else if (position > 1) {
            return MAX_DEGREES;
        }
        return position * MAX_DEGREES;
    }
}
