// void printConfig(SensorConfig config) {
    // Serial.print("Sensor: ");
    // Serial.println(config.name);
    // Serial.print("  Min Value: ");
    // Serial.println(config.min);
    // Serial.print("  Max Value: ");
    // Serial.println(config.max);
    // Serial.print("  Init Count: ");
    // Serial.println(config.count);
    // Serial.println();
// }

void readConfig() {
  File policeFile = SD.open("POLICE.TXT");  
    if (policeFile) {
        while (policeFile.available()) {
            String line = policeFile.readStringUntil('\n');
            line.trim(); // 줄 끝의 공백 및 줄바꿈 제거

    // 센서 이름, 최소값, 최대값, 초기화 기준 횟수를 읽음
            int firstSpace = line.indexOf(' ');
            int secondSpace = line.indexOf(' ', firstSpace + 1);
            int thirdSpace = line.indexOf(' ', secondSpace + 1);

            String name = line.substring(0, firstSpace);
            int min = line.substring(firstSpace + 1, secondSpace).toInt();
            int max = line.substring(secondSpace + 1, thirdSpace).toInt();
            int count = line.substring(thirdSpace + 1).toInt();
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
        // Serial.println("정책 읽기 완료");
    } else {
        // Serial.println("Failed to open config file.");
    }

    /*
    // 설정 출력
    printConfig(tempConfig);
    printConfig(humiConfig);
    printConfig(windConfig);
    */
}