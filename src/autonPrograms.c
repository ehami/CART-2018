#include "main.h"

// All of the different autotnomous routines for the normal game and for robot
// skills

const char *titles[] = {"Simple Stat", "MG_LEFT", "MG_RIGHT", "WreckAll",
                        "Skills"};
void (*scripts[])() = {&simpleStationaryGoal, &mobileGoalMoveToTenLEFT,
                       &mobileGoalMoveToTenRIGHT, &wreckEverything,
                       &chaseSkillsAuton};

// Puts one cone onto the stationary goal without moving. (very few teams go for
// stationary goal so high stack likely)  SPEED: <5 sec, ACCURACY: 80-90%,
// VULNERABILITY: none, POSITION: anywhere that reaches stationary goal
void simpleStationaryGoal() {
  setChainLiftToAngleTarget(200);
  fbcRunCompletion(&chainLiftFBC, 3000);

  delay(200);
  setChainLiftToAngleTarget(0);
  fbcRunCompletion(&chainLiftFBC, 3000);
}

// drives forward from the left side of the zone, picks up mobile goal, and
// then turns around and drops it into the 10 point zone (only for left side)
// SPEED: 13 seconds
// ACCURACY: 50%,
// VULNERABILTIY: can be overtaken
// by fast charging robots
// POSITION: facing mobile goal ON LEFT SIDE OF CORNER
void mobileGoalMoveToTenLEFT() {
  mobileGoalToTenLeftOrRight(true);
  // Kinda accurate
  /*lcdSetText(uart1, 1, "Starting Auton");
  setMobileGoalToPower(127);
  delay(900);
  setMobileGoalToPower(0);
  driveForDistanceSetGoal(4.7 * 12);
  waitUntilDriveGoalReached();
  setMobileGoalToPower(-127);
  delay(1000);
  setMobileGoalToPower(0);
  setDriveWheelsToPower(-127, -127);
  delay(2000);
  while (analogRead(LEFT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("LEFT FRONT LINE SENSOR:%d\n", analogRead(LEFT_FRONT_LINE_SENSOR));
  }
  setDriveWheelsToPower(-85, 85);
  delay(1700);
  setDriveWheelsToPower(127, 127);
  delay(1500);
  setDriveWheelsToPower(0, 0);
  setMobileGoalToPower(127);
  delay(1000);
  setMobileGoalToPower(0);
  setDriveWheelsToPower(-127, -127);
  delay(700);
  setDriveWheelsToPower(0, 0);*/
}

// drives forward from the right side of the zone, picks up mobile goal, and
// then turns around and drops it into the 10 point zone (only for right side)
// SPEED: 13 seconds,
// ACCURACY: 50%,
// VULNERABILTIY: can be overtaken
// by fast charging robots,
// POSITION: facing mobile goal ON RIGHT SIDE OF CORNER
void mobileGoalMoveToTenRIGHT() { mobileGoalToTenLeftOrRight(false); }

// drives forward from right or left (based on parameters) and picks up the
// mobile goal and drops it into the 10 or 20 point zone
// SPEED: 13 seconds
// ACCURACY: 50% for 20 pt, 70% for 10 pt.
// VULNERABILTIY: can be overtaken by fast robot
// SCORE: 10/20 pts.
// POSITION: facing mobile goal ON RIGHT SIDE OF CORNER
void mobileGoalToTenLeftOrRight(bool isLeft) {
  bool isTwenty = false; // not implimented yet
  lcdSetText(uart1, 1, "Starting Auton");
  /*setMobileGoalToPower(127);
  delay(900);
  setMobileGoalToPower(0);*/
  setDriveWheelsToPower(127, 127);
  setMobileGoalToPower(127);
  delay(1000);
  setMobileGoalToPower(0);
  delay(1950);
  setDriveWheelsToPower(0, 0);
  delay(200);
  setMobileGoalToPower(-127);
  delay(1200);
  setMobileGoalToPower(0);
  setDriveWheelsToPower(-127, -127);
  delay(2000);
  if (isLeft) {
    while (analogRead(LEFT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
      printf("LEFT FRONT LINE SENSOR:%d\n", analogRead(LEFT_FRONT_LINE_SENSOR));
      lcdPrint(uart1, 2, "LF:%d", analogRead(LEFT_FRONT_LINE_SENSOR));
    }
  } else {
    while (analogRead(RIGHT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
      printf("RIGHT FRONT LINE SENSOR:%d\n",
             analogRead(RIGHT_FRONT_LINE_SENSOR));
      lcdPrint(uart1, 2, "RF:%d", analogRead(RIGHT_FRONT_LINE_SENSOR));
    }
  }

  if (!isTwenty) {
    if (isLeft) {
      setDriveWheelsToPower(-127, 20);
    } else {
      setDriveWheelsToPower(25, -127);
    }
    delay(2450);
    setDriveWheelsToPower(80, 80);
    delay(700);
    setMobileGoalToPower(127);
    delay(1000);
    setMobileGoalToPower(-127);
    delay(100);
    setDriveWheelsToPower(0, 0);
    delay(100);
    setMobileGoalToPower(0);
    setDriveWheelsToPower(-127, -127);
    delay(850);
    setDriveWheelsToPower(0, 0);
  } else {
    // not implimented yet
  }
}

// wreck the other team's cones and mobile goal (to prevent auton
// scoring).
// SPEED: <5 seconds
// ACCURACY: 90% (depends on position)
// VULNERABILTIY: none :)
// SCORE: 0, but blocks scoring
// POSITION: anywhere
void wreckEverything() {
  setDriveWheelsToPower(127, 127);
  delay(3700);
  setDriveWheelsToPower(0, 0);
}

void chaseSkillsAuton() {

  setMobileGoalToPower(127);
  delay(100);
  setDriveWheelsToPower(80, 80);
  delay(800);
  setMobileGoalToPower(0);
  // This sets the motors and mobile goal intake and drive wheels to power
  // and waits to pass the first white line and put the mobile goal intake
  // down then stops mobile goal intake.
  while (analogRead(RIGHT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("Front Right Line Sensor: %d\n",
           analogRead(RIGHT_FRONT_LINE_SENSOR));
    delay(20);
  }
  setMobileGoalToPower(-127);
  setDriveWheelsToPower(0, 0);
  delay(300);
  setDriveWheelsToPower(-80, -80);
  delay(600);
  while (analogRead(RIGHT_BACK_LINE_SENSOR) > LINE_SENSOR_THRESHOLD &&
         analogRead(LEFT_BACK_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("Back Right Line Sensor: %d\n", analogRead(RIGHT_BACK_LINE_SENSOR));
    printf("Back Left Line Sensor: %d\n", analogRead(LEFT_BACK_LINE_SENSOR));
    delay(20);
  }
  setMobileGoalToPower(0);
  delay(300);
  setDriveWheelsToPower(-60, 60);
  while (analogRead(RIGHT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("Front Right Line Sensor: %d\n",
           analogRead(RIGHT_FRONT_LINE_SENSOR));
    delay(20);
  }
  delay(200);
  setDriveWheelsToPower(50, 50);
  delay(5000);
  setDriveWheelsToPower(-80, -80);
  delay(1000);
  setDriveWheelsToPower(127, 127);
  delay(2500);
  setMobileGoalToPower(127);
  delay(1500);
  setDriveWheelsToPower(-127, -127);
  delay(700);
  setDriveWheelsToPower(0, 0);
  setMobileGoalToPower(-127);
  delay(800);
  setMobileGoalToPower(0);
  setDriveWheelsToPower(-127, -127);
  delay(1300);
  setDriveWheelsToPower(-80, 80);
  delay(1300);
  setDriveWheelsToPower(127, 127);
  delay(550);
  setDriveWheelsToPower(80, -80);
  delay(1000);
  setDriveWheelsToPower(-127, -127);
  delay(10000);
}
