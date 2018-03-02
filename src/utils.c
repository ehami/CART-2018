// Various converters and calculators that dont really fit anywhere else

#include "main.h"
#include <stdlib.h>
#include <string.h>

// Converts radians to degrees
double radiansToDegrees(double radians) {
  return (radians / M_PI * 180); // 1 pi radian = 180 degrees
}

// Converts degrees to radians
double degreesToRadians(double degrees) {
  return (degrees / 180 * M_PI); // 1 pi radian = 180 degrees
}

// Converts degrees to IME ticks
double degreesToIMEticks(double degrees, double ticksPerRevolution) {
  return (degrees / 360 * ticksPerRevolution); // Degrees -> Rotations -> Ticks
}

// Calculates gear ratio multiple (returns the inverse of the gear reduction)
// (how much to multiply the motor ticks by in order to account for gearing)).
double calculateGearRatioMultiple(int inputGearTeeth, int outputGearTeeth) {
  double invReduction =
      ((double)outputGearTeeth) /
      ((double)inputGearTeeth); // casted to double in order to get
  // an actual decimal multiple (intead of 0 because of integer math)
  return invReduction;
}
