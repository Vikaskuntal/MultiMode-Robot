#define BLYNK_TEMPLATE_ID "TMPLcyXhGXs_"
#define BLYNK_DEVICE_NAME "LED"
#define BLYNK_AUTH_TOKEN "c3aBle-kHuiv8ay2SJ8xXlyLElzpe4l1"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include<ESP32Servo.h>

Servo motor;
BlynkTimer  timer;

#define ir_Left 21
#define ir_Right 26
#define trig 25
#define echo 19

#define left_front_forward    15
#define left_front_backward   2
#define left_back_backward   4
#define left_back_forward   5

#define right_back_backward  14
#define right_back_forward  27
#define right_front_backward   13
#define right_front_forward   12

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi 9 Power";
char pass[] = "waitbabu";

volatile bool  Mode1 = 0, Mode2 = 0, Mode3 = 0, up = 0, down = 0, Left_turn = 0, Right_turn = 0;
unsigned short  limit = 14;

unsigned int distance = 0;

void search() {
 unsigned int d1, d2;                 //d1=left    d2=right
  
    motor.write(0);
    delay(500);
    ultrasonic();
    d2 = distance;
    
    Serial.print("d1=");
    Serial.println(d1);
    delay(2000);
    motor.write(180);
    delay(500);
    ultrasonic();
    d1 = distance;
    
    Serial.print("d2=");
    Serial.println(d2);
    delay(2000);
    motor.write(90);
    if (d1 >= d2){
      left();
      delay(1000);
      STOP();
    }
    else {
      right();
        delay(1000);
    STOP();
    }
}

void ultrasonic() {

  int duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.println("cm");
}

void front() {
  if (Mode3 == 1 && digitalRead(ir_Left) == 0){
    STOP();
    left();
  }
 else if (Mode3 == 1 && digitalRead(ir_Right) == 0){
    STOP();
    right();
  }
  else{
  Serial.println("forward");
  digitalWrite(left_front_backward, LOW);
  digitalWrite(right_front_backward, LOW);
  digitalWrite(left_back_backward, LOW);
  digitalWrite(right_back_backward, LOW);

  digitalWrite(left_front_forward , HIGH);
  digitalWrite(right_front_forward , HIGH);
  digitalWrite(left_back_forward, HIGH);
  digitalWrite(right_back_forward, HIGH);
  }
}

void left() {
  Serial.println("left");
  digitalWrite(left_front_backward, LOW);
  digitalWrite(left_front_forward, LOW);
  digitalWrite(left_back_backward , LOW);
  digitalWrite(left_back_forward, LOW);

  digitalWrite(right_front_backward , LOW);
  digitalWrite(right_back_backward, LOW);
  digitalWrite(right_front_forward , HIGH);
  digitalWrite(right_back_forward , HIGH);
  
}
void right() {
  Serial.println("right");
  digitalWrite(right_front_backward, LOW);
  digitalWrite(right_back_backward, LOW);
  digitalWrite(right_front_forward, LOW);
  digitalWrite(right_back_forward, LOW);

  digitalWrite(left_front_backward, LOW);
  digitalWrite(left_back_forward, HIGH);
  digitalWrite(left_back_backward, LOW);
  digitalWrite(left_front_forward, HIGH);
  
}
void back() {
  Serial.println("back");
  digitalWrite(left_front_forward , LOW);
  digitalWrite(right_front_forward, LOW);
  digitalWrite(left_back_forward, LOW);
  digitalWrite(right_back_forward, LOW);

  digitalWrite(left_front_backward, HIGH);
  digitalWrite(right_front_backward, HIGH);
  digitalWrite(left_back_backward, HIGH);
  digitalWrite(right_back_backward, HIGH);

}
void STOP() {
  digitalWrite(left_front_forward, LOW);
  digitalWrite(left_front_backward, LOW);
  digitalWrite(right_front_backward, LOW);
  digitalWrite(right_front_forward, LOW);
  digitalWrite(left_back_backward, LOW);
  digitalWrite(left_back_forward, LOW);
  digitalWrite(right_back_backward , LOW);
  digitalWrite(right_back_forward, LOW);
}


void myTimer() {
   if (Mode1 == 1) {
    Blynk.virtualWrite(V5, "Mode1");
    Blynk.virtualWrite(V6, "Auto detect");
  }
  else if (Mode2 == 1) {
    Blynk.virtualWrite(V5, "Mode2");
    Blynk.virtualWrite(V6, "Remote car");
  }
  else if (Mode3 == 1) {
    Blynk.virtualWrite(V5, "Mode3");
    Blynk.virtualWrite(V6, "Follower");
  }
  else {
    Blynk.virtualWrite(V5, "Select Mode");
    Blynk.virtualWrite(V6, "          ");
  }
}

//v2 for mode1, v3 for mode 2, v4 for mode3-------------------------------
BLYNK_WRITE(V2) {
  Mode1 = param.asInt();
}

BLYNK_WRITE(V3) {
  Mode2 = param.asInt();
}
BLYNK_WRITE(V4) {
  Mode3 = param.asInt();
}


//v0 for front, v1 for back, v7 for right and v8 for left

BLYNK_WRITE(V0)
{
  up = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(up);
  //delay(1000);
}
BLYNK_WRITE(V1)
{
  down = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(down);
  //delay(1000);
}
BLYNK_WRITE(V7)
{
  Right_turn = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(Right_turn);
  //delay(1000);
}

BLYNK_WRITE(V8)
{
  Left_turn = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(Left_turn);
  //delay(1000);
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(trig, OUTPUT);
pinMode(ir_Left, INPUT);
pinMode(ir_Right, INPUT);
pinMode(echo, INPUT);

pinMode(34,OUTPUT);//ultrasonic sensor ko supply dene k liye 
  digitalWrite(trig, LOW);

pinMode(left_front_forward ,OUTPUT);pinMode(left_front_backward,OUTPUT);
pinMode(left_back_backward ,OUTPUT);pinMode(left_back_forward,OUTPUT);
pinMode(right_back_backward,OUTPUT);pinMode(right_back_forward ,OUTPUT);
pinMode(right_front_backward,OUTPUT);pinMode(right_front_forward,OUTPUT);

digitalWrite(left_front_forward, LOW);digitalWrite(left_front_backward, LOW);digitalWrite(right_front_backward, LOW);
digitalWrite(right_front_forward, LOW);digitalWrite(left_back_backward, LOW);digitalWrite(left_back_forward, LOW);
digitalWrite(right_back_backward , LOW);digitalWrite(right_back_forward, LOW);

  motor.attach(18);
  motor.write(90);

digitalWrite(34,HIGH);//ultrasonic ko Supply dene k liye 
  
Blynk.begin(auth, ssid, pass);
timer.setInterval(500L, myTimer);

}

void loop() {
  // put your main code here, to run repeatedly:
if (Mode1 == 1) {
    Serial.println("Mode1");
    ultrasonic();
    Serial.print("distance=");
    Serial.println(distance);
    if(distance>limit){
      front();
    }
    
      else{
          STOP();
          search();
        }
    
    }
  
  else if (Mode2 == 1) {
    Serial.println("Mode2");
    if (up == 1)front();
    else if (down == 1)back();
    else if (Left_turn == 1)left();
    else if (Right_turn == 1)right();
    else {
      STOP();
    }
  }
  
  else if (Mode3 == 1) {
    Serial.println("Mode3");
    ultrasonic();
    
    Serial.print("gap =");
    Serial.println(distance);
    
    if (distance > 14) {
      front();

    }
    else if (distance < 6)back();
    else STOP();
  }

  else {
    STOP();
  }

  Blynk.run();
  timer.run();

}
