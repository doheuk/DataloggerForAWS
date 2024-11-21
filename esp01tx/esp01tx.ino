#include <SoftwareSerial.h>
// Wi-Fi 정보
String ssid = "D308", password = "kookmin3088";

// 서버 정보
String serverIP = "106.249.180.230", serverPort = "41002";
// AT 명령어 전송 함수


void setup() {
  Serial.begin(9600);         // PC와 통신 (디버깅용)
  Serial1.begin(9600);        // ESP-01과 통신
  connectWiFi();              // WiFi 연결
}

void loop() {
  String data = "FA FB 14 01 01 18 0B 04 0E 11 49 00 03 2C 04 BA 0D 4A 00 21 0D A2 00 3A 00 05 00 00 00 0A 00 00 02 69 00 00 02 25 1C 8C 04 C6 04 C6 FC 19 04 9F FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 00 00 FC 19 FC 19 00 00 01 4C FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 FC 19 00 00 FC 19 FC 19 00 23 00 00 00 37 00 00 04 9F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 AB 18 FF FE"; // 153바이트의 데이터(예시)
  sendDataToServer(data);                // 서버 접속 및 데이터 전송
  delay(10000);                                              // 10초 대기 후 반복
}

void connectWiFi() {
  Serial.println("Connecting to WiFi...");

  String sendCommand = "AT+CWJAP=\""+ssid+"\",\""+password+"\"";
  Serial1.println(sendCommand); // WiFi 네트워크 연결
  delay(2000);                                      // 연결 대기
  if (Serial1.find("WIFI CONNECTED")) { // 연결 성공 확인
    Serial.println("WiFi connected.");
  } else {
    Serial.println("WiFi connection failed!");
  }
}

void sendDataToServer(String data) {
  Serial.println("Connecting to server...");
  String connectCommand = "AT+CIPSTART=\"TCP\",\""+serverIP+"\","+serverPort;
  Serial1.println(connectCommand);        // 서버 접속
  delay(1000);
  
  String sendCommand = "AT+CIPSEND=" + String(data.length());
  Serial1.println(sendCommand);         // 데이터 크기 명시
  delay(1000);
  Serial1.print(data);                // 데이터 전송
  Serial1.println("AT+CIPCLOSE");       // 연결 닫기
  delay(1000);
  Serial.println("Connection closed.");
 
}