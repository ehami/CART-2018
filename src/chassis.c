#include "main.h"    // includes API.h and other headers

void setDriveWheels(int left, int right) {
  motorSet(2, left);
  motorSet(3, -right);
}
