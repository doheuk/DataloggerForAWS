File samplingFile;  // 샘플링 파일
File averageFile;       // 평균 파일

// 1분 평균 데이터 로그 함수
void logAverageData() {
    String filename =(tempDT.month() < 10 ? "0" : "") + String(tempDT.month()) + (tempDT.day() < 10 ? "0" : "") + String(tempDT.day())+"m.txt";  // 날짜별 평균 파일명
    averageFile = SD.open(filename, FILE_WRITE);
    // 평균 데이터 기록
    if(averageFile){
    String timeString = (tempDT.hour() < 10 ? "0" : "") + String(tempDT.hour()) + ":" +(tempDT.minute() < 10 ? "0" : "")+ tempDT.minute();
    String avrString = " " + String(avrTemp) + " " + String(avrWind) + " " + String(avrHumi);
    averageFile.print(timeString);
    averageFile.println(avrString);
    averageFile.close();
    } else {
      Serial.println("평균 파일 생성 실패");
    }
}

//평균 함
float computAverage(float buffer[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += buffer[i];
  }
  return (sum / size);
}

//샘플링데이터 로그함수
void logSamplingData(float buffer[], int size, String sensor) {
    String filename = (tempDT.month() < 10 ? "0" : "") + String(tempDT.month()) + (tempDT.day() < 10 ? "0" : "") + String(tempDT.day())+sensor+".txt";  // 날짜별 샘플링 파일명
    samplingFile = SD.open(filename, FILE_WRITE);

    if (samplingFile) {
      String timeString = (tempDT.hour() < 10 ? "0" : "") + String(tempDT.hour()) + ":" +(tempDT.minute() < 10 ? "0" : "")+ tempDT.minute();
      samplingFile.print(timeString);
      for(int i = 0; i < size; i++){
        samplingFile.print(" ");
        samplingFile.print(buffer[i]);
      }
      samplingFile.println();
      samplingFile.close();
    } else {
        Serial.println("샘플링 파일 생성 실패");
    }
}