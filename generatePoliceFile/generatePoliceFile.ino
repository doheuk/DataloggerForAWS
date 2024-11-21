#include <SD.h>
#include <SPI.h>

#define CSPIN 4 //sd카드 

File policeFile;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!SD.begin(CSPIN)){
    Serial.println("SD카드 초기화 실패");
    delay(1000);
  }
  Serial.println("SD카드 초기화 성공");
  String filename = "police.txt";
  policeFile = SD.open(filename, FILE_WRITE);
  if(policeFile){
    policeFile.println("temp -20 25 3");
    policeFile.println("humi 0 100 3");
    policeFile.println("wind 0 30 120");
    policeFile.close();
    Serial.println("정책 파일 생성");
  }else{
    Serial.println("정책 파일 생성 실패");
  } 
}

void loop() {
  // put your main code here, to run repeatedly:

}
