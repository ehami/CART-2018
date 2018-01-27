//Various converters and calculators that dont really fit anywhere else

#include "main.h"

//Converts radians to degrees
double radiansToDegrees(double radians) {
  return (radians / M_PI * 180);  //1 pi radian = 180 degrees
}

//Converts degrees to radians
double degreesToRadians(double degrees) {
  return (degrees / 180 * M_PI);  // 1 pi radian = 180 degrees
}

double degreesToIMEticks(double degrees, double ticksPerRevolution) {
  return (degrees / 360 * ticksPerRevolution);  //Degrees -> Rotations -> Ticks
}
