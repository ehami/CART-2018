// Functions for controlling the mobile goal intake mechanism.

#include "main.h"

// sets the mobile goal motor to a specific power level.
void setMobileGoalToPower(int power) {
  motorSet(MOBILE_GOAL_PICKUP_MOTOR_PORT, power);
}

// sets the mobile goal to a specific ime tick value in relation to the zero
// point
void setMobileGoalToIMEticks(int ticks) {
  if (fbcSetGoal(&mobileGoalFBC, ticks)) {
    printf("Success, setting target to %d \n", ticks);
    printf("The power level is going to be set to %d\n",
           fbcGenerateOutput(&mobileGoalFBC));
    printf("The result of runCompletion was %d\n",
           fbcRunContinuous(&mobileGoalFBC));
    printf("IME value is %d\n", getMobileGoalPosition());

  } else {
    printf("FAILED TO SET TARGET TO %d\n", ticks);
  }
  if (mobileGoalFBC.stallDetect(&mobileGoalFBC)) {
    printf("STALL DETECTED (MOBILE GOAL)\n");
  } else {
    printf("No stall detected (mobile goal)\n");
  }
}

// sets the mobile goal rack to a specific position from the START position
// (positive values lower the intake)
void setMobileGoalToDistance(double inchesFromStart) {
  int ticks = inchesFromStart * 0.5 * M_PI;
  setMobileGoalToIMEticks(ticks);
}

// gets the mobile goal encoder value
int getMobileGoalPosition() {
  int result;
  int *resAddr = &result;
  imeGet(IME_MOBILE_GOAL_MOTOR, resAddr);
  return *resAddr;
}

// resets the mobile goal encoder value
void resetMobileGoalIMEvalue() { imeReset(IME_MOBILE_GOAL_MOTOR); }
