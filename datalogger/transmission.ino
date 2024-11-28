// 서버 정보
String serverIP = "106.249.180.230", serverPort = "41002";

bool connectWiFi(String ssid, String password) {
  Serial.println("Connecting to WiFi...");

  String command = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  unsigned long previousMillis = 0;  // 마지막 실행 시간 저장
  const long interval = 1000;  // 1초 간격 (기존의 delay(1000) 대체)

  int i = 0;  // WiFi 연결 시도 횟수
  const int maxAttempts = 16;  // 최대 시도 횟수

  while (!Serial1.find("WIFI CONNECTED")) { // WiFi 연결 성공 확인
    unsigned long currentMillis = millis();  // 현재 시간

    if (currentMillis - previousMillis >= interval) {  // 1초마다 실행
      previousMillis = currentMillis;  // 마지막 실행 시간 업데이트
      
      if (i % 5 == 0) {
        digitalWrite(ESPCTR, LOW);  // ESP-01 모듈 리셋 (Reset)
        delay(500);  // 짧은 딜레이로 리셋 효과 줌
        digitalWrite(ESPCTR, HIGH);  // 리셋 후 다시 활성화
        Serial1.println(command);  // WiFi 연결 명령 재전송
      }
      i++;  // 시도 횟수 증가
    }

    // 최대 시도 횟수를 초과하면 연결 실패
    if (i >= maxAttempts) {
      Serial.println("Fail to connect to WiFi");
      return false;  // 연결 실패 반환
    }
  }

  Serial.println("WiFi Connected!");
  return true;  // 연결 성공
}


bool sendDataToServer(){
  int i = 0;
  Serial.println("Connecting to server...");
  String connectCommand = "AT+CIPSTART=\"TCP\",\""+serverIP+"\","+serverPort;
  while(!Serial1.find("CONNECT")){ // 연결 성공 확인
    if(i==0){
      Serial1.println(connectCommand);        // 서버 접속
    } else if(i > 20) {
      return false;
    }
    delay(100);
    i++;
  }
  
  String sendCommand = "AT+CIPSEND=153";
  Serial1.println(sendCommand);         // 데이터 크기 명시
  delay(1000);
  Serial1.write(packet,153);
  Serial1.println("AT+CIPCLOSE");       // 연결 닫기
  delay(1000);
  Serial.println("Connection closed.");
  return true;
}
