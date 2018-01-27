#include "main.h" // includes API.h and other headers

// Sets the left and right motors to the specifified values
void setDriveWheelsToPower(int left, int right) {
  setLeftWheelsToPower(left);
  setRightWheelsToPower(right);
}

// Sets the left motors to a specific power level. (positive values go forward
// since power level is reversed in this function)
void setLeftWheelsToPower(int power) { motorSet(LEFT_MOTOR_PORT, power); }

// Sets the right motors to a specific power level. (positive values go forward)
void setRightWheelsToPower(int power) { motorSet(RIGHT_MOTOR_PORT, power); }

// Gets the average encoder value for the two left wheels
int getLeftWheelEncoderAverage() {

  int front, back;
  // imeGet(IME_LEFT_FRONT_MOTOR, &front);
  imeGet(IME_LEFT_BACK_MOTOR, &back);
  // int average = (front + back) / 2;
  // return average;
  return back; // front encoder currently disconected
}

// Gets the average encoder value for the two right wheels
int getRightWheelEncoderAverage() {
  int front, back;
  // imeGet(IME_RIGHT_FRONT_MOTOR, &front);
  imeGet(IME_RIGHT_BACK_MOTOR, &back);
  // int average = (front + back) / 2;
  // return average;
  return back; // front encoder currently disconected
}
