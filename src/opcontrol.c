/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any
 * functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own
 * task with the default priority and stack size whenever the robot is enabled
 * via the Field Management System or the VEX Competition Switch in the operator
 * control mode. If the robot is disabled or communications is lost, the
 * operator control task will be stopped by the kernel. Re-enabling the robot
 * will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the
 * VEX Cortex will run the operator control task. Be warned that this will also
 * occur if the VEX Cortex is tethered directly to a computer via the USB A to A
 * cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is
 * available and the scheduler is operational. However, proper use of delay() or
 * taskDelayUntil() is highly recommended to give other tasks (including system
 * tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop,
 * even if empty.
 */
void operatorControl() {
  int power;
  int turn;

  // Used to keep track of button state to prevent multiple triggers.
  bool button5uPressed = false;
  bool button5dPressed = false;

  bool button6uPressed = false;
  bool button6dPressed = false;

  bool button7uPressed = false;
  bool button7dPressed = false;
  bool button7lPressed = false;
  bool button7rPressed = false;

  bool button8uPressed = false;
  bool button8dPressed = false;
  bool button8lPressed = false;
  bool button8rPressed = false;

  while (true) {

    // Wheels
    power = joystickGetAnalog(1, 3); // vertical axis on right joystick
    turn = joystickGetAnalog(1, 1);  // horizontal axis on right joystick
    // printf("Power: %d, Turn: %d\n", power, turn);

    if (abs(power) > 5 || abs(turn) > 5) {
      setDriveWheelsToPower(power + turn, power - turn);
    } else {
      setDriveWheelsToPower(0, 0);
    }

    // Raise chain lift on press of 5U (lower on 5D)
    if (joystickGetDigital(1, 5, JOY_UP)) {
      setChainLiftToPower(127);
    } else if (joystickGetDigital(1, 5, JOY_DOWN)) {
      setChainLiftToPower(-127);
    } else {
      setChainLiftToPower(0);
    }

    // Raise 2 bar lift on press of 6U (lower on 6D)
    if (joystickGetDigital(1, 6, JOY_UP)) {
      setTwoBarLiftToPower(127);
    } else if (joystickGetDigital(1, 6, JOY_DOWN)) {
      setTwoBarLiftToPower(-127);
    } else {
      setTwoBarLiftToPower(0);
    }

    // Pickup mobile goal on press of 7U (and reverse on 7D)
    if (joystickGetDigital(1, 7, JOY_UP)) {
      setMobileGoalToPower(127);
    } else if (joystickGetDigital(1, 7, JOY_DOWN)) {
      setMobileGoalToPower(-127);
    } else {
      setMobileGoalToPower(0);
    }

    /*
                    // Raise chain lift on press of 7U (lower on 7D)
        if (joystickGetDigital(1, 7, JOY_UP) && !button7uPressed) {
          // if this is a new button press
          setLiftToDepthAndHeight(0, 10);
                            printf("7U Pressed\n");
        } else if (joystickGetDigital(1, 7, JOY_DOWN) && !button7dPressed) {
          //if this is a new button press
          setLiftToDepthAndHeight(0, 40);
                            printf("7D Pressed\n");
        } else {
                            setChainLiftToPower(0);
                    }
                    button7uPressed = joystickGetDigital(1, 7, JOY_UP);
                    button7dPressed = joystickGetDigital(1, 7, JOY_DOWN);
    */

    // Raise chain lift on press of 7U (lower on 7D)
    if (joystickGetDigital(1, 8, JOY_UP) && !button8uPressed) {
      // if this is a new button press
      printf("8U Pressed\n");
      setMobileGoalToDistance(5);
    } else if (joystickGetDigital(1, 8, JOY_DOWN) && !button8dPressed) {
      // if this is a new button press
      printf("8D Pressed\n");
      setMobileGoalToDistance(-1);
    }
    if (joystickGetDigital(1, 8, JOY_RIGHT) && !button8rPressed) {
      // if this is a new button press
      printf("8R Pressed\n");
      setMobileGoalToPower(127);
      delay(100);
      setMobileGoalToPower(0);
      printf("%d\n", getMobileGoalPosition());
    } else {
    }
    button8uPressed = joystickGetDigital(1, 8, JOY_UP);
    button8dPressed = joystickGetDigital(1, 8, JOY_DOWN);
    button8lPressed = joystickGetDigital(1, 8, JOY_LEFT);
    button8rPressed = joystickGetDigital(1, 8, JOY_RIGHT);

    delay(20); // Motors and Joystics are only updated every 20 ms.
  }
}
