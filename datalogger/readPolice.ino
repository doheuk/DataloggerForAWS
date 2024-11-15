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


void readConfig() {
  File policeFile = SD.open("police.txt");  
    if (policeFile) {
        while (policeFile.available()) {
            // 센서 이름, 최소값, 최대값, 초기화 기준 횟수를 읽음
            String sensorName = policeFile.readStringUntil(' ');
            int minValue = policeFile.parseInt();
            int maxValue = policeFile.parseInt();
            int initCount = policeFile.parseInt();
            
            // 센서 이름에 따라 해당 구조체에 값을 저장
            if (sensorName == "temp") {
                tempConfig = {sensorName, minValue, maxValue, initCount};
            } else if (sensorName == "humi") {
                humiConfig = {sensorName, minValue, maxValue, initCount};
            } else if (sensorName == "wind") {
                windConfig = {sensorName, minValue, maxValue, initCount};
            }
        }
        policeFile.close();  // 파일 닫기
    } else {
        Serial.println("Failed to open config file.");
    }

    /*
    // 설정 출력
    printConfig(tempConfig);
    printConfig(humiConfig);
    printConfig(windConfig);
    */
}
/*
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
*/
