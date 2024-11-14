#include <Arduino.h>

// CRC16-CCITT 계산을 위한 함수
uint16_t crc16_ccitt(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF; // 초기 CRC 값

    for (size_t i = 0; i < length; i++) {
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
    return crc; // 계산된 CRC 값 반환
}

void setup() {
    Serial.begin(9600);

    // 테스트 데이터
    const char *data = "Hello, World!";
    size_t length = strlen(data);
    uint16_t crc = crc16_ccitt((const uint8_t *)data, length);

    // CRC 결과 출력
    Serial.print("CRC16-CCITT: 0x");
    Serial.println(crc, HEX);
}

void loop() {
    // 메인 루프는 비워두어도 됩니다.
}
