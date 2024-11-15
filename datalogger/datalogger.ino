#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <SD.h>
#include <SPI.h>
#include <RTClib.h>

#define CSPIN 4 //sd카드 
#define DHTPIN A0 // 온습도센서
#define WINDPIN A1 // 풍속센서

RTC_DS3231 rtc; // dht 설정
DHT11 dht(A0); // dht11 설정
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 설정

float tempBuffer[6] = {0};
float humiBuffer[6] = {0};
float windSpeedBuffer[240] = {0};
float avrWind, avrTemp, avrHumi;
int tempIndex = 0;
int windIndex = 0;
DateTime tempDT;
unsigned long lastWindSample;
void setup() {
  Serial.begin(9600);
  // rtc 시작
  if(!rtc.begin()){
    Serial.println("Couldn't find RTC");
    while(1);
  }
  if(rtc.lostPower()){
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }
  tempDT = DateTime(__DATE__,__TIME__);

  // SD 카드 초기화
  while(!SD.begin(CSPIN)){
    Serial.println("SD카드 초기화 실패");
    delay(1000);
  }
  Serial.println("SD카드 초기화 성공");
  lastWindSample = millis(); 
}

const unsigned long windInterval = 250;

void loop() {
  unsigned long currentTime = millis();
  DateTime nowDT = rtc.now();
  if(nowDT.second()%10 ==0){
    tempBuffer[tempIndex] = dht.readTemperature();
    humiBuffer[tempIndex] = dht.readHumidity();
    if(tempIndex == 5){
      tempIndex = 0;
    } else {
      tempIndex++;
    }
  }
  if(currentTime - lastWindSample >= windInterval) {
    windSpeedBuffer[windIndex] = analogRead(WINDPIN) * 30 / 1023.0;
    if(windIndex == 239){ 
      windIndex = 0;
    } else {
      windIndex++;
    }
    lastWindSample = currentTime;
  }
  if(tempDT.minute() != nowDT.minute()){
    avrTemp = computAverage(tempBuffer,6);
    avrHumi = computAverage(humiBuffer,6);
    avrWind = computAverage(windSpeedBuffer,240);
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
    tempDT = nowDT;
  }
}




