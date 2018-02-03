#include "main.h" // includes API.h and other headers

// Functions for controlling the lift (both parts)

// NOTE: ALL angles are in degrees, and are convered to radians for tric
// calculations, then converted back.

// Sets the Chain Lift to a specific power level.
void setChainLiftToPower(int power) { motorSet(CHAIN_LIFT_PORT, -power); }

// Sets the Two Bar Lift to a specific power level.
void setTwoBarLiftToPower(int power) {
  motorSet(TWO_BAR_LIFT_MOTOR_PORT, -power);
}

// Sets the chain lift to a specific angle (in degrees) using the specific
// motorPower. (Function is blocking (does not exit until complete))
void setChainLiftToAngle(double angle, int motorPower) {
  if (angle > 315) { // lift cannot go past 315 degrees (used to be 270 degrees)
    angle = 315;
  } else if (angle < 0) { // lift cannot go past 0 degrees
    angle = 0;
  }

  // Calculate IME Ticks
  // 12:36 Gear Ratio = 3:1 Gear Reduction, so turn motor 3x to turn output 1x
  int imeTicks = (int)(degreesToIMEticks(angle, IME_TICKS_PER_REV_LOW_SPEED) *
                       calculateGearRatioMultiple(12, 60));

  lcdPrint(uart1, 1, "gCL: %d,cCL: %d", imeTicks, getChainLiftIMEposition());
  fbcSetGoal(&chainLiftFBC, imeTicks);
  fbcRunCompletion(&chainLiftFBC, 1500);
}

// Sets the 2 bar lift to a specific angle (in degrees) using the specific motor
// power.
void setTwoBarLiftToAngle(double angle, int motorPower) {
  if (angle > 60) { // lift cannot go past 60 degrees
    angle = 270;
  } else if (angle < 0) { // lift cannot go past 0 degrees
    angle = 0;
  }

  int imeTicks = (int)(degreesToIMEticks(angle, IME_TICKS_PER_REV_LOW_SPEED) *
                       calculateGearRatioMultiple(12, 84));

  printf("Setting 2 bar lift to %d ticks\n", imeTicks);
  fbcSetGoal(&twoBarLiftFBC, imeTicks);
  fbcRunCompletion(&twoBarLiftFBC, 1500);
}

void setLiftToDepthAndHeight(double depth, double height) {
  // height = 9 + 29 * cos(twoBarAngle) + 13 * sin(chainLiftAngle)
  // depth = 11.25 + 13 * cos(ChainLiftAngle)
  double chainLiftAngle;
  double twoBarAngle;

  if (depth <= 11.25 + 13 * cos(90 / 180 * M_PI)) {
    // the chain lift angle is greater than 90, so it can be in either
    // Quadrant II or Quadrant III, based on the deired height.
    chainLiftAngle = acos((depth + 11.25) / 13) / M_PI * 180;
    twoBarAngle =
        acos((height - 9 - 13 * sin(chainLiftAngle / 180 * M_PI)) / 29) / M_PI *
        180;
  } else {
    // the chain lift is at an angle less than 90, so it has to be in Quadrant I
    chainLiftAngle = acos((depth + 11.25) / 13) / M_PI * 180;
    twoBarAngle =
        acos((height - 9 - 13 * sin(chainLiftAngle / 180 * M_PI)) / 29) / M_PI *
        180;
  }

  if (twoBarAngle > 60 || twoBarAngle < 0) {
    // Two bar lift arm has max rotation of 60 degrees and minimum of 0 degrees
    printf("TWO BAR ANGLE OUT OF BOUNDS: %f, C: %f H: %f, D: %f\n", twoBarAngle,
           chainLiftAngle, height, depth);
    if (twoBarAngle > 60) {
      twoBarAngle = 60;
    } else {
      twoBarAngle = 0;
    }
  }

  if (chainLiftAngle > 270 || chainLiftAngle < 0) {
    // Chain Lift has max rotation of 270 degrees and minimum of 0 degrees
    printf("CHAIN LIFT ANGLE OUT OF BOUNDS: %f, T: %f H: %f, D: %f\n",
           chainLiftAngle, twoBarAngle, height, depth);
    if (chainLiftAngle > 270) {
      chainLiftAngle = 270;
    } else {
      chainLiftAngle = 0;
    }
  }

  setChainLiftToAngle(chainLiftAngle, 127);
  setTwoBarLiftToAngle(chainLiftAngle, 127);
}

// gets the position (in IME ticks) of the two bar lift
int getTwoBarLiftIMEposition() {
  int ticks;
  imeGet(IME_TWO_BAR_LIFT_MOTOR, &ticks);
  return ticks;
}

// gets the position (in IME ticks) of the chain lift
int getChainLiftIMEposition() {
  int ticks;
  imeGet(IME_CHAIN_LIFT_MOTOR, &ticks);
  return ticks;
}
