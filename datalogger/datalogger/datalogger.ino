#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <SD.h>
#include <SPI.h>
#include <TimerOne.h>
#include <RTClib.h>

#define DHTPIN A0 // 온습도센서
#define WINDPIN A1 // 풍속센서
#define CSPIN 8 //sd카드 

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
void sample();
float computAverage(float buffer[], int size);

void setup() {
  Serial.begin(9600);
  // 샘플링 타임 인터럽트
  Timer1.initialize(250000); //0.25초
  Timer1.attachInterrupt(sample);
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
}
void loop() {
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
    String output = String(" Temp: ") + avrTemp + String(" Wind: ") + avrWind + String(" Humi: ") + avrHumi;
    Serial.println(output);
    tempDT = nowDT;
  }
}

void sample(){
  windSpeedBuffer[windIndex] = analogRead(WINDPIN) * 30 / 1023.0;
  if(windIndex == 239){ 
    windIndex = 0;
  } else {
    windIndex++;
  }
}

float computAverage(float buffer[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += buffer[i];
  }
  return (sum / size);
}













