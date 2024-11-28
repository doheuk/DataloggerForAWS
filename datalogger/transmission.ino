// Wi-Fi 정보
String ssid = "D308", password = "kookmin3088";

// 서버 정보
String serverIP = "106.249.180.230", serverPort = "41002";

bool connectWiFi() {
  Serial.println("Connecting to WiFi...");

  String command = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";  
  int i = 0;
  while(!Serial1.find("WIFI CONNECTED")){ // 연결 성공 확인
    if(i % 5 == 0) {
      digitalWrite(ESPCTR, LOW);
      delay(50);
      digitalWrite(ESPCTR, HIGH);
      Serial1.println(command); // WiFi 네트워크 연결
    }
    if(i == 16 ){
      Serial.println("fail to connect wifi");
      return false;
    }
    i ++;
    delay(1000);
  }
  Serial.println("WiFi connected.");
  return true;
}


bool sendDataToServer() {
  int i = 0;
  Serial.println("Connecting to server...");
  String connectCommand = "AT+CIPSTART=\"TCP\",\""+serverIP+"\","+serverPort;
  Serial1.println(connectCommand);        // 서버 접속
  while(!Serial1.find("CONNECT")){ // 연결 성공 확인
    if(i > 20){
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
