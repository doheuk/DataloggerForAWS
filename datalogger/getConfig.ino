void printConfig(SensorConfig config) {
    Serial.print("Sensor: ");
    Serial.println(config.name);
    Serial.print("  Min Value: ");
    Serial.println(config.min);
    Serial.print("  Max Value: ");
    Serial.println(config.max);
    Serial.print("  Init Count: ");
    Serial.println(config.count);
    Serial.println();
}

void readConfig() {
  File policeFile = SD.open("police.txt");  
    if (policeFile) {
        while (policeFile.available()) {
            // 센서 이름, 최소값, 최대값, 초기화 기준 횟수를 읽음
            String name = policeFile.readStringUntil(' ');
            int min = policeFile.parseInt();
            int max = policeFile.parseInt();
            int count = policeFile.parseInt();
            
            // 센서 이름에 따라 해당 구조체에 값을 저장
            if (name == "temp") {
                tempConfig = {name, min, max, count};
            } else if (name == "humi") {
                humiConfig = {name, min, max, count};
            } else if (name == "wind") {
                windConfig = {name, min, max, count};
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