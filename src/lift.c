#include "main.h"    // includes API.h and other headers

//Sets the chain lift to a specific angle (in degrees) using the specific motorPower.
void setChainLiftToAngle(double angle, int motorPower) {
    if (angle > 270) {  //lift cannot go past 270 degrees
        angle = 270;
    } else if (angle < 0) { //lift cannot go past 0 degrees
        angle = 0;
    }
    int imeTicks =  angle / 360 * 627.2;    //360 : convert degrees per rotation, 627.2: ticks per rotation
}

//Sets the chain lift to a specific angle (in degrees) using the specific motor power.
void setTwoBarLiftToAngle(double angle, int motorPower) {
    if (angle > 60) {  //lift cannot go past 60 degrees
        angle = 270;
    } else if (angle < 0) { //lift cannot go past 0 degrees
        angle = 0;
    }
    int imeTicks =  angle / 360 * 627.2;
}

void setLiftToDepthAndHeight(double depth, double height) {
    //height = 9 + 29 * cos(twoBarAngle) + 13 * sin(chainLiftAngle)
    //depth = 11.25 + 13 * cos(ChainLiftAngle)
    double chainLiftAngle;
    double twoBarAngle;

    if (depth <= 11.25 + 13 * cos(90)) {
        //the chain lift angle is greater than 90, so it can be in either
        //Quadrant II or Quadrant III, based on the deired height.

    } else {
        //the chain lift is at an angle less than 90, so it has to be in Quadrant I
        chainLiftAngle = acos((depth + 11.25) / 13);
        twoBarAngle = acos((height - 9 - 13 * sin(chainLiftAngle)) / 29);
    }

    if (twoBarAngle > 60 || twoBarAngle < 0) {
        //Two bar lift arm has max rotation of 60 degrees and minimum of 0 degrees
        printf("TWO BAR ANGLE OUT OF BOUNDS: %f, C: %f H: %f, D: %f\n", twoBarAngle, chainLiftAngle, height, depth);
        if (twoBarAngle > 60) {
            twoBarAngle = 60;
        } else {
            twoBarAngle = 0;
        }
    }

    if (chainLiftAngle > 270 || chainLiftAngle < 0) {
        //Two bar lift arm has max rotation of 60 degrees and minimum of 0 degrees
        printf("CHAIN LIFT ANGLE OUT OF BOUNDS: %f, T: %f H: %f, D: %f\n", chainLiftAngle, twoBarAngle, height, depth);
        if (chainLiftAngle > 270) {
            chainLiftAngle = 270;
        } else {
            chainLiftAngle = 0;
        }
    }

    printf("Chain Lift: %f 2 bar Lift: %f\n", chainLiftAngle, twoBarAngle);
}
