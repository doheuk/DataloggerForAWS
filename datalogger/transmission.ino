// Wi-Fi 정보
String ssid = "D308", password = "kookmin3088";

// 서버 정보
String serverIP = "106.249.180.230", serverPort = "41002";

void connectWiFi() {
  Serial.println("Connecting to WiFi...");

  String command = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";  
  int i = 0;
  while(!Serial1.find("WIFI CONNECTED")){ // 연결 성공 확인
    if(i % 5 == 0) {
      Serial1.println(command); // WiFi 네트워크 연결
    }
    if(i == 16 ){
      Serial.println("restarting datalogger");
      Watchdog.enable(1000);
    }
    i ++;
    delay(1000);
  }
  Serial.println("WiFi connected.");
}

void sendDataToServer() {
  Serial.println("Connecting to server...");
  String connectCommand = "AT+CIPSTART=\"TCP\",\""+serverIP+"\","+serverPort;
  Serial1.println(connectCommand);        // 서버 접속
  delay(1000);
  
  String sendCommand = "AT+CIPSEND=153";
  Serial1.println(sendCommand);         // 데이터 크기 명시
  delay(1000);
  Serial1.write(packet,153);
  Serial1.println("AT+CIPCLOSE");       // 연결 닫기
  delay(1000);
  Serial.println("Connection closed.");
}
