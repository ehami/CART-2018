#include "main.h"

// All of the different autotnomous routines for the normal game and for robot
// skills

const char *titles[] = {"Simple Stat", "ConeMG_LEFT", "ConeMG_RIGHT",
                        "WreckEverything"};
void (*scripts[])() = {&simpleStationaryGoal, &coneOnMobileGoalAndMoveToTenLEFT,
                       &coneOnMobileGoalAndMoveToTenRIGHT, &wreckEverything};

// Puts one cone onto the stationary goal without moving. (very few teams go for
// stationary goal, so high stack likely)  SPEED: <5 sec, ACCURACY: 80-90%,
// VULNERABILITY: none, POSITION: anywhere that reaches stationary goal
void simpleStationaryGoal() {
  setChainLiftToAngleTarget(200);
  fbcRunCompletion(&chainLiftFBC, 3000);

  delay(200);
  setChainLiftToAngleTarget(0);
  fbcRunCompletion(&chainLiftFBC, 3000);
}

// drives forward from the left side of the field, picks up mobile goal, drops
// cone on, and then turns around and drops it into the 10/20 point zone (only
// for left side)  SPEED: long?, ACCURACY: 50%, VULNERABILTIY: can be overtaken
// by fast charging robots, POSITION: facing mobile goals ON LEFT SIDE OF CORNER
void coneOnMobileGoalAndMoveToTenLEFT() {
  setMobileGoalToPower(400);
  setDriveWheelsToPower(80, 80);
  delay(800);
  setMobileGoalToPower(0);
  while (analogRead(RIGHT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("%d\n", analogRead(RIGHT_FRONT_LINE_SENSOR));
    delay(20);
  }
  setMobileGoalToPower(-400);
  setDriveWheelsToPower(-80, -80);
  delay(800);
  while (analogRead(RIGHT_FRONT_LINE_SENSOR) > LINE_SENSOR_THRESHOLD) {
    printf("%d\n", analogRead(RIGHT_FRONT_LINE_SENSOR));
    delay(20);
  }
  setDriveWheelsToPower(0, 0);
}

// drives forward, picks up mobile goal, drops cone on, and then turns around
// and drops it into the 10/20 point zone (only for right side)  SPEED: long?,
// ACCURACY: 50%, VULNERABILTIY: can be overtaken by fast charging robots,
// POSITION: facing mobile goals ON RIGHT SIDE OF CORNER
void coneOnMobileGoalAndMoveToTenRIGHT() {
  // Drop a single cone on the stationary goal and then return to starting
  // position "A1"
  // drive forward 28.5"
  // driveForDistance(28.5);

  // raise lifts to measured angles
  // setChainLiftToAngleTarget(45, 127);
  // setTwoBarLiftToAngleTarget(20, 127);
  /*while (true) {
    fbcRunContinuous(&chainLiftFBC);
    fbcRunContinuous(&twoBarLiftFBC);
    lcdPrint(uart1, 2, "eCL: %d, e2B: %d", getChainLiftIMEposition(),
             getTwoBarLiftIMEposition());

    lcdPrint(uart1, 1, "eMG:%d,eLB:%d", getMobileGoalPosition(),
             getLeftWheelEncoderAverage());
    delay(20);
  }

  lcdPrint(uart1, 1, "Auton Done");
  print("Auton Done\n");*/
}

// wreck the other team's cones and mobile goal (to prevent auton
// scoring).
void wreckEverything() {
  setDriveWheelsToPower(127, 127);
  delay(3700);
  setDriveWheelsToPower(0, 0);
}
