#include <Adafruit_Fingerprint.h>

#define TOUCH_PIN 2
#define FINGERPRINT_RX 1
#define FINGERPRINT_TX 2
#define FINGERPRINT_VCC 8
#define STANDBY_DELAY 5000 // 지연시간
int touchValue = 0;
unsigned long lastActiveTime = 0;

bool fingerprintActive = false;
bool touchDetected = false;


void setup(){
  Serial.begin(9600);
  pinMode(TOUCH_PIN,INPUT);
  pinMode(FINGERPRINT_VCC,OUTPUT);
  while(!Serial){
    delay(1);
  }
  Serial.println("도어락을 시작합니다.");
}

void loop(){
  unsigned long currentTime = millis();
  delay(500); // 나중에 지우기
  int touchValue = digitalRead(TOUCH_PIN);

  if(touchValue == HIGH){
    if(!touchDetected){
      touchDetected = true;
      lastActiveTime = millis();
      if(!fingerprintActive){
        activateFingerpirntSensor();
      }
    }
  }
  else{
    touchDetected = false;
    unsigned long currentTime = millis();
    if((currentTime-lastActiveTime)>=(STANDBY_DELAY)){
      deactivateFingerprintSensor();
    }
  }
}
// 지문인식 센서를 켜는 함수
void activateFingerpirntSensor(){
  fingerprintActive  = true;
  digitalWrite(FINGERPRINT_VCC,HIGH);
}

void deactivateFingerprintSensor(){
  fingerprintActive = false;
  digitalWrite(FINGERPRINT_VCC,LOW);
  
}


