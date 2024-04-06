#include <Adafruit_Fingerprint.h>


#define TOUCH_PIN 2
#define FINGERPRINT_RX 1
#define FINGERPRINT_TX 2
#define FINGERPRINT_VCC 8
#define STANDBY_DELAY 5000 // ????
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
  Serial.println("???? ?????.");
}


void loop(){
  unsigned long currentTime = millis();
  delay(500); // ??? ???
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
// ???? ??? ?? ??
void activateFingerpirntSensor(){
  fingerprintActive  = true;
  digitalWrite(FINGERPRINT_VCC,HIGH);
}


void deactivateFingerprintSensor(){
  fingerprintActive = false;
  digitalWrite(FINGERPRINT_VCC,LOW);
  
}



