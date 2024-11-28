#include <DHT11.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include "Utility.h"
#include <Adafruit_SleepyDog.h>
#include <SoftwareSerial.h>

#define CSPIN 4 // Micro SD CS 
#define DHTPIN A0 // DHT 11 temp and humi
#define WINDPIN A1 // Wind Speed
#define WINDCTR A7 // Wind Control
#define DHTCTR A6 // DHT Control
#define ESPCTR A5// ESP8266 Control

// Wi-Fi 정보
String ssid = "D308", password = "kookmin3088";

RTC_DS3231 rtc;
DHT11 dht(A0); // dht11 설정

DateTime tempDT;
float tempBuffer[6] = {0},  humiBuffer[6] = {0}, windSpeedBuffer[240]={0};
float avrWind, avrTemp, avrHumi;
int tempIndex = 0, windIndex = 0;
unsigned long lastWindSample;
const unsigned long windInterval = 250;
uint8_t packet[153];
// 센서 설정 저장용 변수
SensorConfig tempConfig, humiConfig, windConfig;
bool connectWiFi();
bool sendDataToServer(String ssid, String password);

void setup() {
  // Serial.begin(9600);
  Serial1.begin(9600);        // ESP-01과 통신
  pinMode(DHTCTR, OUTPUT);
  pinMode(WINDCTR, OUTPUT);
  pinMode(ESPCTR, OUTPUT);
  digitalWrite(DHTCTR, HIGH);
  digitalWrite(WINDCTR, HIGH);
  digitalWrite(ESPCTR, HIGH);
  // rtc 시작
  if(!rtc.begin()){
    // Serial.println("Couldn't find RTC");
    while(1); //수정 필요
  }
  if(rtc.lostPower()){
    // Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }
  tempDT = rtc.now();

  // SD 카드 초기화
  while(!SD.begin(CSPIN)){
    // Serial.println("SD카드 초기화 실패");
    delay(1000);
  }
  // Serial.println("SD카드 초기화 성공");
  readConfig();
  connectWiFi(ssid, password);
  Watchdog.enable(4000); // 와치도그 리셋 실행 4초
  lastWindSample = millis(); 
}

int errorTempCount = 0, errorHumiCount = 0, errorWindCount = 0, errorTotal = 0;
int resetFlag = 0;

void loop() {
  unsigned long currentTime = millis();
  DateTime nowDT = rtc.now();
  if(nowDT.second()%10 ==0){
    tempBuffer[tempIndex] = dht.readTemperature();
    humiBuffer[tempIndex] = dht.readHumidity();
    if (tempBuffer[tempIndex] < tempConfig.min || tempBuffer[tempIndex] > tempConfig.max) {
      errorTempCount++;
    }
    if (humiBuffer[tempIndex] < humiConfig.min || humiBuffer[tempIndex] > humiConfig.max) {
      errorHumiCount++;
    }
    if(tempIndex == 5){
      tempIndex = 0;
    } else {
      tempIndex++;
    }
  }
  if(currentTime - lastWindSample >= windInterval) {
    windSpeedBuffer[windIndex] = analogRead(WINDPIN) * 30 / 1023.0;
    if (windSpeedBuffer[windIndex] < windConfig.min || windSpeedBuffer[windIndex] > windConfig.max) {
      errorWindCount++;
    }
    if(windIndex == 239){ 
      windIndex = 0;
    } else {
      windIndex++;
    }
    lastWindSample = currentTime;
  }
  //장애 판단
  if(errorHumiCount >= humiConfig.count){
    errorHumiCount = 0;
    digitalWrite(DHTCTR, LOW);
    delay(50);
    digitalWrite(DHTCTR, HIGH);
    logError("humi sensor error");
    errorTotal++;
  }
  if(errorTempCount >= tempConfig.count){
    errorTempCount = 0;
    //Serial.print("온도 센서 복구");
    digitalWrite(DHTCTR, LOW);
    delay(50);
    digitalWrite(DHTCTR, HIGH);
    logError("temp sensor error");
    errorTotal++;
  }
  if(errorWindCount >= windConfig.count){
    errorWindCount = 0;
    digitalWrite(WINDCTR, LOW);
    delay(50);
    digitalWrite(WINDCTR, HIGH);
    logError("wind sensor error");
    errorTotal++;
  }
  while(errorTotal > 10){
  }
  //시간단위 명령
  if(tempDT.hour() != nowDT.hour()){
    errorTotal = 0;
    errorHumiCount = errorTempCount = errorWindCount = 0;
  }
  
  //1분단위 명령
  if(tempDT.minute() != nowDT.minute()){
    avrTemp = calAvr(tempBuffer,6);
    avrHumi = calAvr(humiBuffer,6);
    avrWind = calAvr(windSpeedBuffer,240);
    // if(tempDT.hour() < 10) //HH:MM 형식을 위함
      // Serial.print("0");
    // Serial.print(tempDT.hour(),DEC);
    // Serial.print(":");
    // if(tempDT.minute() <10) //HH:MM 형식을 위함
      // Serial.print("0");
    // Serial.print(tempDT.minute(),DEC);
    String avrString = String(" Temp: ") + avrTemp + String(" Wind: ") + avrWind + String(" Humi: ") + avrHumi;
    // Serial.println(avrString);
    logAverageData();
    logSamplingData(tempBuffer,6,"t");
    logSamplingData(humiBuffer,6,"h");
    logSamplingData(windSpeedBuffer,240,"w");
    Watchdog.disable();
    createPacket();
    bool tx = sendDataToServer();
    for(int i = 0; i++; i<3){
      if(tx){
        break;
      }else{
        // Serial.println("sendfail");
        connectWiFi(ssid, password);
      }
      if(i == 2) {
        logError("Transmission error");
        errorTotal++;
      }
      tx = sendDataToServer();
    }
    Watchdog.enable(4000);
    tempDT = nowDT;
    // Serial.print(errorHumiCount);
    // Serial.print(" ");
    // Serial.print(errorTempCount);
    // Serial.print(" ");
    // Serial.println(errorWindCount);
  }
  Watchdog.reset();//와치도그 타이머 리셋
}




