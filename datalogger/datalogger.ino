#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>
#include "Utility.h"
#include <Adafruit_SleepyDog.h>
#include <SoftwareSerial.h>

#define CSPIN 4 //sd카드 
#define DHTPIN A0 // 온습도센서
#define WINDPIN A1 // 풍속센서

RTC_DS3231 rtc; // dht 설정
DHT11 dht(A0); // dht11 설정
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 설정

DateTime tempDT;
float tempBuffer[6] = {0},  humiBuffer[6] = {0}, windSpeedBuffer[240]={0};
float avrWind, avrTemp, avrHumi;
int tempIndex = 0, windIndex = 0;
unsigned long lastWindSample;
const unsigned long windInterval = 250;
// 센서 설정 저장용 변수
SensorConfig tempConfig, humiConfig, windConfig;
void connectWiFi();
void sendDataToServer(String data);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);        // ESP-01과 통신
  // rtc 시작
  if(!rtc.begin()){
    Serial.println("Couldn't find RTC");
    while(1); //수정 필요
  }
  if(rtc.lostPower()){
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }
  tempDT = rtc.now();

  // SD 카드 초기화
  while(!SD.begin(CSPIN)){
    Serial.println("SD카드 초기화 실패");
    delay(1000);
  }
  Serial.println("SD카드 초기화 성공");
  readConfig();
  connectWiFi();
  //Watchdog.enable(4000); // 와치도그 리셋 실행 4초
  lastWindSample = millis(); 
  
  // 디버깅
  Serial.print(tempConfig.name);
  Serial.print(" ");
  Serial.print(tempConfig.min);
  Serial.print(" ");
  Serial.print(tempConfig.max);
  Serial.print(" ");
  Serial.println(tempConfig.count);
  Serial.print(humiConfig.name);
  Serial.print(" ");
  Serial.print(humiConfig.min);
  Serial.print(" ");
  Serial.print(humiConfig.max);
  Serial.print(" ");
  Serial.println(humiConfig.count);
  Serial.print(windConfig.name);
  Serial.print(" ");
  Serial.print(windConfig.min);
  Serial.print(" ");
  Serial.print(windConfig.max);
  Serial.print(" ");
  Serial.println(windConfig.count);
}

int errorTempCount = 0, errorHumiCount = 0, errorWindCount = 0;
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
  //시간단위 명령
  if(tempDT.hour() != nowDT.hour()){
    errorHumiCount = errorTempCount = errorWindCount = 0;
  }
  if(errorHumiCount >= humiConfig.count){
    resetFlag = 1;
    logError("datalogger restarting because of temp sensor");
  }
  if(errorTempCount >= tempConfig.count){
    logError("datalogger restarting because of humi sensor");
    resetFlag = 1;
  }
  if(errorWindCount >= windConfig.count){
    logError("datalogger restarting because of wind sensor");
    resetFlag = 1;
  }
  /*
  if(resetFlag == 1) {
    while(1){
      delay(1000); // 무한루프 --> 와치도그 리셋
    }
  }
  */
  //1분단위 명령
  if(tempDT.minute() != nowDT.minute()){
    avrTemp = calAvr(tempBuffer,6);
    avrHumi = calAvr(humiBuffer,6);
    avrWind = calAvr(windSpeedBuffer,240);
    if(tempDT.hour() < 10) //HH:MM 형식을 위함
      Serial.print("0");
    Serial.print(tempDT.hour(),DEC);
    Serial.print(":");
    if(tempDT.minute() <10) //HH:MM 형식을 위함
      Serial.print("0");
    Serial.print(tempDT.minute(),DEC);
    String avrString = String(" Temp: ") + avrTemp + String(" Wind: ") + avrWind + String(" Humi: ") + avrHumi;
    Serial.println(avrString);
    logAverageData();
    logSamplingData(tempBuffer,6,"t");
    logSamplingData(humiBuffer,6,"h");
    logSamplingData(windSpeedBuffer,240,"w");
    String dataP = createPacket();
    sendDataToServer(dataP);// 데이터 전송
    tempDT = nowDT;
    Serial.print(errorHumiCount);
    Serial.print(" ");
    Serial.print(errorTempCount);
    Serial.print(" ");
    Serial.println(errorWindCount);
  }
  //Watchdog.reset();//와치도그 타이머 리셋
}




