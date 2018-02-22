/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions
 * related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs pre-initialization code. This function will be started in kernel mode
 * one time while the VEX Cortex is starting up. As the scheduler is still
 * paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes
 * (pinMode()) and port states (digitalWrite()) of limit switches, push buttons,
 * and solenoids. It can also safely configure a UART port (usartOpen()) but
 * cannot set up an LCD (lcdInit()).
 */
void initializeIO() {

  watchdogInit(); // According to PROS API, this may help with the cortex
  // partially resetting when the I2C line (for IMEs) gets a
  // static shock.
}

/*
 * Runs user initialization code. This function will be started in its own task
 * with the default priority and stack size once when the robot is starting up.
 * It is possible that the VEXnet communication link may not be fully
 * established at this time, so reading from the VEX Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics),
 * LCDs, global variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and
 * autonomous() tasks will not start. An autonomous mode selection menu like the
 * pre_auton() in other environments can be implemented in this task if desired.
 */

void initialize() {

  setTeamName("CART9498");

  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  lcdSetText(uart1, 1, "Init Started");
  print("Init Done");

  // Inititialize IMEs
  int IMECount = imeInitializeAll();
  // Ensure all IMEs are plugged in
  if (IMECount != NUMBER_OF_IME) {
    printf("ALL IMEs ARE NOT CONNECTED. There are only %d of %d connected.\n",
           IMECount, NUMBER_OF_IME);
  }
  // Initialize Motor Manger Controller
  // There's actually examples for this, and it took 2 minutes to implement!

  // this library allows us to have a slew rate controller (slows the
  // acceleration and decelaration) for the drive mootors to decrease current
  // spikes that can cause the PTC's to trip.

  // blrsMotorInit(LEFT_MOTOR_PORT, false, 2.0, NULL);
  // blrsMotorInit(RIGHT_MOTOR_PORT, false, 2.0, NULL);

  // Initialize PID controllers

  /* They (PROS developers) really need better documentation of how to properly
   * initialize and run the PID controller.  This took several hours to figure
   * out based on the source and header files.  There are no examples of
   * how any of these are supposed to be laid out. (Also I'm dumb) -ERH
   */

  // Mobile Goal PID Controller
  imeReset(IME_MOBILE_GOAL_MOTOR); // Reset the IME on startup
  fbcPIDInitializeData(/*PID controller*/ &mobileGoalPIDfbc,
                       /*P*/ 0.1,
                       /*I*/ 0.01,
                       /*D*/ 0,
                       /*minI*/ 0,
                       /*minI*/ 0);
  fbcInit(/*FBC controller*/ &mobileGoalFBC,
          /*move*/ setMobileGoalToPower,
          /*sensor*/ getMobileGoalPosition,
          /*reset function pointer (not used, so null)*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -15,
          /*pos_deadband*/ 15,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 5);
  fbcPIDInit(/*FBC controller*/ &mobileGoalFBC,
             /*PID controller*/ &mobileGoalPIDfbc);

  // Left Wheels PID Controller
  // imeReset(IME_LEFT_FRONT_MOTOR); // Reset the IMEs on startup
  imeReset(IME_LEFT_BACK_MOTOR);
  fbcPIDInitializeData(/*PID controller*/ &leftPIDfbc,
                       /*P*/ 0.1,
                       /*I*/ 0.01,
                       /*D*/ 0,
                       /*minI*/ 0,
                       /*minI*/ 0);
  fbcInit(/*FBC controller*/ &leftFBC,
          /*move*/ setLeftWheelsToPower,
          /*sensor*/ getLeftWheelEncoderAverage,
          /*reset function pointer (not used, so null*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -15,
          /*pos_deadband*/ 15,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 5);
  fbcPIDInit(/*FBC controller*/ &leftFBC,
             /*PID controller*/ &leftPIDfbc);

  // Right Wheels PID Controller
  // imeReset(IME_RIGHT_FRONT_MOTOR); // Reset the IMEs on startup
  imeReset(IME_RIGHT_BACK_MOTOR);
  fbcPIDInitializeData(/*PID controller*/ &rightPIDfbc,
                       /*P*/ 0.1,
                       /*I*/ 0.01,
                       /*D*/ 0,
                       /*minI*/ 0,
                       /*minI*/ 0);
  fbcInit(/*FBC controller*/ &rightFBC,
          /*move*/ setRightWheelsToPower,
          /*sensor*/ getRightWheelEncoderAverage,
          /*reset function pointer (not used, so null*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -15,
          /*pos_deadband*/ 15,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 5);
  fbcPIDInit(/*FBC controller*/ &rightFBC,
             /*PID controller*/ &rightPIDfbc);

  // Two Bar Lift PID Controller
  imeReset(IME_TWO_BAR_LIFT_MOTOR); // Reset the IME on startup
  fbcPIDInitializeData(/*PID controller*/ &twoBarLiftPIDfbc,
                       /*P*/ 0.1,
                       /*I*/ 0.01,
                       /*D*/ 0,
                       /*minI*/ 0,
                       /*minI*/ 0);
  fbcInit(/*FBC controller*/ &twoBarLiftFBC,
          /*move*/ setTwoBarLiftToPower,
          /*sensor*/ getTwoBarLiftIMEposition,
          /*reset function pointer (not used, so null*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -15,
          /*pos_deadband*/ 15,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 5);
  fbcPIDInit(/*FBC controller*/ &twoBarLiftFBC,
             /*PID controller*/ &twoBarLiftPIDfbc);

  // Chain Lift PID Controller
  imeReset(IME_CHAIN_LIFT_MOTOR); // Reset the IME on startup
  fbcPIDInitializeData(/*PID controller*/ &chainLiftPIDfbc,
                       /*P*/ 0.5,
                       /*I*/ 0.0,
                       /*D*/ 0.0,
                       /*minI*/ 0.1,
                       /*minI*/ -0.1);
  fbcInit(/*FBC controller*/ &chainLiftFBC,
          /*move*/ setChainLiftToPower,
          /*sensor*/ getChainLiftIMEposition,
          /*reset function pointer (not used, so null)*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -30,
          /*pos_deadband*/ 30,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 3);
  fbcPIDInit(/*FBC controller*/ &chainLiftFBC,
             /*PID controller*/ &chainLiftPIDfbc);

  // Cone Intake PID Controller
  imeReset(IME_CONE_INTAKE_MOTOR); // Reset the IME on startup
  fbcPIDInitializeData(/*PID controller*/ &coneIntakePIDfbc,
                       /*P*/ 0.1,
                       /*I*/ 0.01,
                       /*D*/ 0,
                       /*minI*/ 0,
                       /*minI*/ 0);
  fbcInit(/*FBC controller*/ &coneIntakeFBC,
          /*move*/ setConeIntakeToPower,
          /*sensor*/ getConeIntakePosition,
          /*reset function pointer (not used, so null*/ NULL,
          /*stall detect (using default)*/ fbcStallDetect,
          /*neg_deadband*/ -15,
          /*pos_deadband*/ 15,
          /*acceptableTolerance*/ 10,
          /*acceptableConfidence*/ 5);
  fbcPIDInit(/*FBC controller*/ &coneIntakeFBC,
             /*PID controller*/ &coneIntakePIDfbc);

  // lcdScriptInit(uart1);
  // lcdScriptSelect();
}
