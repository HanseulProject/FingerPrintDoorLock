#include <Adafruit_Fingerprint.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#included <Wire.h>


#define FINGER_RX 4
#define FINGER_TX 5
#define TOUCH_PIN 2
#define FINGER_POWER 8
#define BT_RX 11
#define BT_TX 12


#define DELAYTIME 5

time_t startTime = 10;
time_t endTime = 0;
bool touchInput = false;
uint8_t id;
uint8_t count = 0;


SoftwareSerial mySerial(FINGER_RX,FINGER_TX);
SoftwareSerial bluetooth(BT_RX,BT_TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(57600); // 통신속도 조절
  bluetooth.begin(57600);

  pinMode(TOUCH_PIN,INPUT);
  pinMode(FINGER_POWER,OUTPUT);

}

void loop() {
  if(bluetooth.available()){
    Serial.write(bluetooth.read());
  }
  if(Serial.available()){
    bluetooth.write(Serial.read());
  }
  // touchModule();
}

void touchModule(){
  int touchValue = digitalRead(TOUCH_PIN);
  if(touchValue == HIGH){
    digitalWrite(FINGER_POWER,HIGH);
    startTime = now();
    touchInput = true;
  }
  else{
    endTime = now();
    if(touchInput == true && DELAYTIME>endTime-startTime){
      detectFinger();
    }
    else{
      touchInput = false;
      digitalWrite(FINGER_POWER,LOW);
    }
  }
}

uint8_t detectFinger() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
