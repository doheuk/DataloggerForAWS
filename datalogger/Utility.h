#ifndef UTILITY_H
#define UTILITY_H

#include "Arduino.h"

//평균 함
float calAvr(float buffer[], int size);

// 센서 범위와 초기화 횟수 구조체 정의
struct SensorConfig {
    String name;
    int min;
    int max;
    int count;
};

// 설정 정보를 출력하는 함수
void readConfig();
void printConfig(SensorConfig config); 

#endif
