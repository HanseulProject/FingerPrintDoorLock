#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include<Wire.h> // I2C 통신 라이브러리

#define TOUCH_PIN 2
#define FINGERPRINT_RX 3
#define FINGERPRINT_TX 4
#define FINGERPRINT_VCC 8
#define STANDBY_DELAY 5000 // 지연시간
#define SERVO_PWM 9
int touchValue = 0;
unsigned long lastActiveTime = 0;
unsigned long currentTime;

int fingerCount =0;
bool checkFinger = false;
bool fingerprintActive = false;
bool touchDetected = false;

SoftwareSerial mySerial(FINGERPRINT_RX,FINGERPRINT_TX);
Adafruit_Fingerprint fingerSensor = Adafruit_Fingerprint(&mySerial);
Servo myServo;

void setup(){
  Serial.begin(9600);
  pinMode(TOUCH_PIN,INPUT);
  pinMode(FINGERPRINT_VCC,OUTPUT);
  myServo.attach(SERVO_PWM);

  while(!Serial){
    delay(1);
  }
  Serial.println("도어락을 시작합니다.");
  fingerSensor.begin(57600);
  if(fingerSensor.verifyPassword()){ // 전원이 켜져 있으면 센서 확인이 가능함.
    Serial.println("Found fingerprint Sensor");
  }
  else{
    Serial.println("Didn't Found fingerprint Sensor");
  }
  setMotorReset();
}

void loop(){
  
  int touchValue = digitalRead(TOUCH_PIN);

  if(touchValue == HIGH){ // 터치가 활성화 되었을때
    lastActiveTime = millis();
    activateFingerpirntSensor();
  }
  else{
    currentTime = millis();
    if((currentTime-lastActiveTime)>=STANDBY_DELAY){
      deactivateFingerprintSensor();
      setMotorReset();
    }
    else if((currentTime-lastActiveTime)<STANDBY_DELAY){
      inputFingerPrint();
    }
  }
  
}
// 지문인식 센서를 켜는 함수
void activateFingerpirntSensor(){
  fingerprintActive  = true;
  digitalWrite(FINGERPRINT_VCC,HIGH);
  fingerCount = fingerSensor.getTemplateCount();
  checkFinger = false;
  Serial.println("FingerPrintOn");
}

void deactivateFingerprintSensor(){
  fingerprintActive = false;
  digitalWrite(FINGERPRINT_VCC,LOW);
  Serial.println("FinerPrintOff");
}

void inputFingerPrint(){
  int data = fingerSensor.getImage();
  Serial.println("Scanning");
  Serial.println(fingerCount);
  Serial.println(data);
  for(int i=1;i<=fingerCount;i++){
    if(data==i){
      checkFinger = true;
      Serial.println("동일한 지문이 있습니다.");
    }
    else{
      Serial.println("동일한 지문이 없습니다.");
    }
  }
  if(checkFinger == true){
    setMotorOpen();
  }
}

void setMotorReset(){
  myServo.write(55); // 서보 모터 off 값
  checkFinger = false;
}
void setMotorOpen(){
  myServo.write(160); // 서보 모터 on 값
}