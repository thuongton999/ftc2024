package org.firstinspires.ftc.teamcode.mechanism.pixelholder;

import com.qualcomm.robotcore.hardware.Servo;

import org.firstinspires.ftc.teamcode.utilities.ServoHelper;

public class PixelHolder {
    Servo pixelHolderL, pixelHolderR;
    boolean isTriggeredL = false;
    boolean isTriggeredR = false;
    static final double PIXEL_HOLDER_CLOSE = 5;
    static final double PIXEL_HOLDER_OPEN = 90;
    public enum UseServo {
        LEFT, RIGHT
    }

    public PixelHolder(Servo left, Servo right) {
        this.pixelHolderL = left;
        this.pixelHolderR = right;
    }

    public void open(UseServo side) {
        double position = ServoHelper.degreesToPosition(PIXEL_HOLDER_OPEN);
        if (side == UseServo.LEFT) {
            pixelHolderL.setPosition(position);
        } else {
            pixelHolderR.setPosition(position);
        }
    }

    public void close(UseServo side) {
        double position = ServoHelper.degreesToPosition(PIXEL_HOLDER_CLOSE);
        if (side == UseServo.LEFT) {
            pixelHolderL.setPosition(position);
        } else {
            pixelHolderR.setPosition(position);
        }
    }

    public void trigger(UseServo side) {
        if (side == UseServo.LEFT) {
            if (isTriggeredL) {
                close(UseServo.LEFT);
            } else {
                open(UseServo.LEFT);
            }
            isTriggeredL = !isTriggeredL;
            return;
        }
        if (isTriggeredR) {
            close(UseServo.RIGHT);
        } else {
            open(UseServo.RIGHT);
        }
        isTriggeredR = !isTriggeredR;
    }
}
