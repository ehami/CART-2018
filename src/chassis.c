#include "main.h" // includes API.h and other headers

// Sets the left and right motors to the specifified values
void setDriveWheelsToPower(int left, int right) {
  setLeftWheelsToPower(left);
  setRightWheelsToPower(-right);
}

// Sets the left motors to a specific power level. (positive values go forward)
void setLeftWheelsToPower(int power) {
  // blrsMotorSet(LEFT_MOTOR_PORT, power, true);
  motorSet(LEFT_MOTOR_PORT, power);
}

// Sets the right motors to a specific power level. (positive values go
// backward)
void setRightWheelsToPower(int power) {
  // blrsMotorSet(RIGHT_MOTOR_PORT, power, true);
  motorSet(RIGHT_MOTOR_PORT, power);
}

// Sets the left and right motors to the specifified values with ramping speed.
// (positive values go forward)
void setDriveWheelsToPowerWithRamping(int left, int right) {
  setLeftWheelsToPowerWithRamping(left);
  setRightWheelsToPowerWithRamping(right);
}

// Sets the left motors to a specific power level with ramping speed.(positive
// values go forward)
void setLeftWheelsToPowerWithRamping(int power) {
  // blrsMotorSet(LEFT_MOTOR_PORT, power, false);
  motorSet(RIGHT_MOTOR_PORT, power);
}

// Sets the right motors to a specific power level with ramping speed (positive
// values go forward)
void setRightWheelsToPowerWithRamping(int power) {
  // blrsMotorSet(RIGHT_MOTOR_PORT, power, false);
  motorSet(RIGHT_MOTOR_PORT, power);
}

// Gets the average encoder value for the two left wheels
int getLeftWheelEncoderAverage() {
  int front, back;
  imeGet(IME_LEFT_FRONT_MOTOR, &front);
  imeGet(IME_LEFT_BACK_MOTOR, &back);
  int average = (front + back) / 2;
  return average;
}

// Gets the average encoder value for the two right wheels
int getRightWheelEncoderAverage() {
  int front, back;
  imeGet(IME_RIGHT_FRONT_MOTOR, &front);
  imeGet(IME_RIGHT_BACK_MOTOR, &back);
  int average = (front + back) / 2;
  return average;
}

// Sets the robot to drive a specific distance (in inches).
void driveForDistanceSetGoal(double inches) {
  // take inches, divide by circumference, mutiply by 360 to get degrees.
  int degrees = inches / WHEEL_FOUR_INCH_EFFECTIVE_CIRCUMFERENCE * 360;
  // use my handy util to calculate ticks from degrees.
  int ticksToAdd = degreesToIMEticks(degrees, IME_TICKS_PER_REV_LOW_SPEED);

  // sets left wheels to current location plus the ticks prev. calculated
  fbcSetGoal(&leftFBC, getLeftWheelEncoderAverage() + ticksToAdd);

  // sets right wheels to current location plus the ticks prev. calculated
  fbcSetGoal(&rightFBC, getRightWheelEncoderAverage() - ticksToAdd);
  /*
   while (!fbcIsConfident(&leftFBC) && !fbcIsConfident(&rightFBC)) {
     fbcRunContinuous(&leftFBC);
     fbcRunContinuous(&rightFBC);
     delay(25);
   }
   */
}

// Waits until the drive wheels have reached their goals and then stops
void waitUntilDriveGoalReached() {
  unsigned long startTime = millis();
  while ((fbcIsConfident(&leftFBC) != 1 && fbcIsConfident(&rightFBC) != 1) &&
         (startTime + 10000) > millis()) {
    fbcRunContinuous(&leftFBC);
    fbcRunContinuous(&rightFBC);
    // printf("LeftENC: %d. RightENC: %d\n",
    // getLeftWheelEncoderAverage(),getRightWheelEncoderAverage());
    delay(40);
  }
  setDriveWheelsToPower(0, 0);
}
