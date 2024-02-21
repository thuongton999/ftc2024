package org.firstinspires.ftc.teamcode.utilities;

public class MathHelper {
    public static double map(double value, double fromMin, double fromMax, double toMin, double toMax) {
        value = Math.min(Math.max(value, fromMin), fromMax); // Clamp value to input range
        return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
    }
}
