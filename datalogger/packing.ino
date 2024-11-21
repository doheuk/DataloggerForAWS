#define EMPTY 0xFC19

uint8_t start[2] = {0xFA, 0xFB}; //시작
uint8_t protocolVer[3] = {0x14, 0x01, 0x01}; //프로토콜 버전
uint8_t dateTime[5]; // 날짜 시간
uint8_t dataClass = 'I'; //자료구분 I, B
uint8_t dataFormatNum = 0x00; // 자료형식번호
uint8_t id[2] = {0x03, 0x2C};
uint8_t data[135] = {0x00};
uint8_t check[2] = {0x00};
uint8_t end[2] = {0xFF, 0xFF}; // 끝
void hexSplit(uint8_t array[], int n, int i);

void packData() {
  int i;
  for(i = 0; i<135; i+=2){
    hexSplit(data,EMPTY, i);
  }
  hexSplit(data,(avrTemp+100)*10, 0);
  hexSplit(data,avrWind*10, 4);
  hexSplit(data,avrHumi*10, 18);
  for(i = 135 - 11; i <135;i++){
    data[i] = 0x00;
  }
}

void createPacket() {
  int i,k = 0;
  dateTime[0] = (tempDT.year() - 2000);
  dateTime[1] = tempDT.month();
  dateTime[2] = tempDT.day();
  dateTime[3] = tempDT.hour();
  dateTime[4] = tempDT.minute();
  packData();
  for (i = 0; i < 2; i++) {
    packet[k++] = start[i];
  }
  for (i = 0; i < 3; i++) {
    packet[k++] = protocolVer[i];
  }
  for (int i = 0; i < 5; i++) {
    packet[k++] = dateTime[i];
  }
  packet[k++] = dataClass;
  packet[k++] = dataFormatNum;
  for (i = 0; i < 2; i++) {
    packet[k++] = id[i];
  }
  for (i = 0; i < 135; i++) {
    packet[k++] = data[i];
  }
  crc16(packet, 149);
  for (i = 0; i < 2; i++) {
    packet[k++] = check[i];
  }
  for (i = 0; i < 2; i++) {
    packet[k++] = end[i];
  }
}


void hexSplit(uint8_t array[], int n, int i) {
  array[i++] = (n >> 8) & 0xFF;
  array[i] = n & 0xFF;
}

// CRC16-CCITT 계산을 위한 함수
void crc16(String data, size_t length){
    uint16_t crc = 0xFFFF; // 초기 CRC 값

    for (size_t i = 2; i < length; i++) {
        crc ^= (data[i] << 8); // 현재 바이트를 CRC와 XOR 연산

        // 8비트씩 처리
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) { // 가장 높은 비트가 1이면
                crc = (crc << 1) ^ 0x1021; // 왼쪽 시프트 후 다항식과 XOR
            } else {
                crc <<= 1; // 왼쪽으로 시프트
            }
        }
    }
    hexSplit(check, crc, 0); // 계산된 CRC 값 입력
}
