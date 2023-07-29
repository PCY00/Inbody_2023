#include <SPI.h>
#include <Wire.h>
#include <DFRobot_MAX30102.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "3"; // Wi-Fi SSID
const char* password = "00000011"; // Wi-Fi Password
const char* server = "203.253.128.177"; // 모비우스 서버 IP 주소
const int port = 7579; // 모비우스 서버 포트
const String cnt = "exingrate";
const String ae = "20191546"; // 모비우스에서 사용할 AE 이름

#define SCREEN_WIDTH 128 // OLED 디스플레이 너비, 픽셀 단위
#define SCREEN_HEIGHT 64 // OLED 디스플레이 높이, 픽셀 단위

#define swich 3

// I2C로 연결된 SSD1306 디스플레이에 대한 선언 (SDA, SCL 핀)
#define OLED_RESET     -1 // 리셋 핀 # (또는 Arduino 리셋 핀을 공유하는 경우 -1)
#define SCREEN_ADDRESS 0x3C ///< 데이터 시트 참조; 128x64 경우 0x3D, 128x32 경우 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DFRobot_MAX30102 particleSensor;

void send_data(String payload);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00011110, 0b01111000,
  0b00111111, 0b11111100,
  0b01111111, 0b11111110,
  0b01111111, 0b11111110,
  0b01111111, 0b11111110,
  0b00111111, 0b11111100,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000011, 0b11000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000 };

String number1 = ""; // 하트 옆 숫자
int number2 = 0; // 가운데 숫자

void setup() {
  Serial.begin(9600);

  pinMode(swich, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 할당 실패"));
    for (;;) // 진행하지 않고 영원히 루프 유지
      ;
  }

  WiFi.begin(ssid, password);

  Serial.print("Wi-Fi에 연결 중");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi에 연결되었습니다.");

  display.clearDisplay(); // 디스플레이 지우기
  display.setTextSize(1);      // 보통 크기의 글꼴
  display.setTextColor(SSD1306_WHITE); // 흰색 글자
  display.setCursor(0, 0);     // 좌상단에서 시작
  display.cp437(true);         // 전체 256 문자 'Code Page 437' 폰트 사용

  int startX = display.width() - LOGO_WIDTH - 4; // 하트를 왼쪽으로 이동
  int startY = 0;

  display.drawBitmap(startX, startY, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

  display.display();


  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
  }

  particleSensor.sensorConfiguration(/*ledBrightness=*/0x1F, /*sampleAverage=*/SAMPLEAVG_4, \
                                  /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_400, \
                                  /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_4096);

  
}

int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid 

void loop() {

  if(digitalRead(swich == LOW)){
    particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);

    String payload = "{\"m2m:cin\": {\"con\": \"" + String(heartRate, DEC) + "\"}}";
    send_data(payload);
    
    // 디스플레이 지우기
    display.clearDisplay();
  
    // 하트 그리기
    int startX = display.width() - 32 - LOGO_WIDTH;
    int startY = 0;
    display.drawBitmap(startX, startY, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
  
    // 하트 옆 숫자를 1씩 증가시키고 디스플레이에 표시
    number1 = String(heartRate, DEC);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(display.width() - 24, 4); // 하트 옆에 숫자 표시
    display.print(number1);
  
    // 가운데 숫자를 2씩 증가시키고 디스플레이에 표시
    number2 += 2;
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    int centerX = (display.width() - 8) / 2; // 가운데 위치 계산
    int centerY = (display.height() - 16) / 2;
    centerX -= 8; // 숫자를 왼쪽으로 2칸 이동
    display.setCursor(centerX, centerY); // 가운데에 숫자 표시
    display.print(number2);
  
     // Inbody 문구를 디스플레이에 표시
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, display.height() - 8); // 왼쪽 맨 아래에 표시
    display.print("Inbody");
  
    display.display();
    delay(1000); // 1초마다 숫자 업데이트
  }else {
    Serial.println("rest");
    display.clearDisplay();

    // "BMI Team.10" 문구를 크기 3으로 표시
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    int centerX = (display.width() - 8) / 2; // 가운데 위치 계산
    int centerY = (display.height() - 16) / 2;
    centerX -= 8; // 숫자를 왼쪽으로 2칸 이동
    display.setCursor(centerX, centerY); // 가운데에 숫자 표시
    display.println("BMI\n  Team.10");

    display.display();

  }
}

void send_data(String payload){
  WiFiClient client;
  HTTPClient http;

  // HTTP POST 요청 설정
  http.begin(client, "http://" + String(server) + ":" + String(port) + "/Mobius/" + ae + "/" + cnt);
  http.addHeader("Content-Type", "application/vnd.onem2m-res+json; ty=4");
  http.addHeader("Accept", "application/json");
  http.addHeader("X-M2M-RI", "12345");
  http.addHeader("X-M2M-Origin", ae);
  int httpCode = http.POST(payload); // HTTP POST 요청 전송
  String response = http.getString(); // 서버 응답 수신
  http.end(); // HTTP 연결 종료

  if (httpCode == 201) { // HTTP 응답 코드가 201인 경우
    Serial.println("데이터를 Mobius로 전송하였습니다.");
    Serial.println(response);
  }
  else{
    Serial.print("HTTP POST 실패: ");
    Serial.println(httpCode);
    Serial.println(response);
  }
}
