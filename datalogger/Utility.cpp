#include "Utility.h"

float calAvr(float buffer[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += buffer[i];
  }
  return (sum / size);
}

void clearSerialBuffer() {
  // while (Serial.available() > 0) {
  //   Serial.read();  // 버퍼에서 데이터를 읽어서 무시
  // }
  while (Serial1.available() > 0) {
    Serial1.read();  // 버퍼에서 데이터를 읽어서 무시
  }
}