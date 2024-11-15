#include <SD.h>
#include <SPI.h>

#define CSPIN 4 //sd카드 

File policeFile;
// 센서 범위와 초기화 횟수 구조체 정의
struct SensorConfig {
    String name;
    int minVal;
    int maxVal;
    int initCount;
};
// 센서 설정 저장용 변수
SensorConfig tempConfig, humiConfig, windConfig;

// 설정 정보를 출력하는 함수
void printConfig(SensorConfig config) {
    Serial.print("Sensor: ");
    Serial.println(config.name);
    Serial.print("  Min Value: ");
    Serial.println(config.minVal);
    Serial.print("  Max Value: ");
    Serial.println(config.maxVal);
    Serial.print("  Init Count: ");
    Serial.println(config.initCount);
    Serial.println();
}

void setup() {
  Serial.begin(9600);
  
  // SD 카드 초기화
  while(!SD.begin(CSPIN)){
    Serial.println("SD카드 초기화 실패");
    delay(1000);
  }
  Serial.println("SD카드 초기화 성공");
  
  policeFile = SD.open("police.txt");
  File configFile = SD.open("config.txt");
    if (configFile) {
        while (configFile.available()) {
            // 센서 이름, 최소값, 최대값, 초기화 기준 횟수를 읽음
            String sensorName = configFile.readStringUntil(' ');
            int minValue = configFile.parseInt();
            int maxValue = configFile.parseInt();
            int initCount = configFile.parseInt();
            
            // 센서 이름에 따라 해당 구조체에 값을 저장
            if (sensorName == "temp") {
                tempConfig = {sensorName, minValue, maxValue, initCount};
            } else if (sensorName == "humi") {
                humiConfig = {sensorName, minValue, maxValue, initCount};
            } else if (sensorName == "wind") {
                windConfig = {sensorName, minValue, maxValue, initCount};
            }
        }
        configFile.close();  // 파일 닫기
    } else {
        Serial.println("Failed to open config file.");
    }

    /*// 설정 출력
    printConfig(tempConfig);
    printConfig(humiConfig);
    printConfig(windConfig);
    */
}

void loop() {
  
}
