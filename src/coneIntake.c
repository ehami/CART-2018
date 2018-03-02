// Functions for controlling the cone intake mechanism

#include "main.h"

// sets the intake mechanism to a specific power level. (positive goes out)
void setConeIntakeToPower(int power) {
  motorSet(CONE_INTAKE_MOTOR_PORT, power);
}

// sets the cone intake to rotate to specific angle
void setConeIntakeToAngle(double angle) {}

// sets the cone intake to rotate a specific number of rotations (positive
// numbers pick up, negative release)
void rotateConeIntake(double rotations) {
  int ticksToAdd = rotations * IME_TICKS_PER_REV_LOW_SPEED * 5 / 3;
  int current = getChainLiftIMEposition();
  fbcSetGoal(&coneIntakeFBC, current + ticksToAdd);
  fbcRunContinuous(&coneIntakeFBC);
}

// sets the cone intake to pick up a cone
void pickUpCone() {}

// sets the cone intake to release a cone
void releaseCone() {}

// gets the position (in IME ticks)
int getConeIntakePosition() {
  int ticks;
  imeGet(IME_CONE_INTAKE_MOTOR, &ticks);
  return ticks;
}
