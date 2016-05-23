package com.faoxis.calculation;

/**
 * Created by Sergei on 12.05.2016.
 */
public class Coordinate {
    private double distance = 0;
    private long angle = 0;
    private int[] setValues = new int[27];
    private int countValues = 0;

    public void pushValue(int value) {
        countValues++;
        setValues[(int) (countValues % 27)] = value;

        if (countValues % 27 == 26) {
            int sum = 0;
            angle++;
            for (int i = 0; i < setValues.length; i++) {
                sum += setValues[i];
                distance = (sum / 27.0) * 0.2;
                countValues = 0;
            }
        }
    }

    public double getDistance() {
        return distance;
    }

    public long getAngle() {
        return angle;
    }

}
