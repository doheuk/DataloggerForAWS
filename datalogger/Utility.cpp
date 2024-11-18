#include "Utility.h"

float calAvr(float buffer[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += buffer[i];
  }
  return (sum / size);
}
