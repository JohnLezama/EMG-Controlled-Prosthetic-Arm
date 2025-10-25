#include <Servo.h>

Servo POINTER_FINGER;
Servo MIDDLE_FINGER;
Servo RING_FINGER;
Servo PINKY_FINGER;
Servo THUMB;
Servo WRIST;
Servo FOREARM;
#define BICEP_FORWARD 10
#define BICEP_BACKWARD 11
#define BICEP_ENABLE 12
#define CYCLOID_DRIVE1_UP 13
#define CYCLOID_DRIVE1_DOWN A1
#define CYCLOID_DRIVE1_ENABLE A2
#define CYCLOID_DRIVE2_UP A3
#define CYCLOID_DRIVE2_DOWN A4
#define CYCLOID_DRIVE2_ENABLE A5
#define EMG_FOREARM A6
#define EMG_BICEP_FORWARD A7
#define EMG_BICEP_BACKWARD A8
#define EMG_SHOULDER_UP A9
#define EMG_SHOULDER_DOWN A10
#define BICEP_ENCODER_A 30
#define BICEP_ENCODER_B 31
#define CYCLOID_DRIVE1_ENCODER_A 32
#define CYCLOID_DRIVE1_ENCODER_B 33
#define CYCLOID_DRIVE2_ENCODER_A 34
#define CYCLOID_DRIVE2_ENCODER_B 35

double bicep_encoder_degree = 0.0;
double cycloid_drive1_encoder_degree = 0.0;
double cycloid_drive2_encoder_degree = 0.0;

int prev_bicep_pulse = 0;
int prev_cycloid_drive1_pulse = 0;
int prev_cycloid_drive2_pulse = 0;


void setup() {
  POINTER_FINGER.attach(3);
  MIDDLE_FINGER.attach(4);
  RING_FINGER.attach(5);
  PINKY_FINGER.attach(6);
  THUMB.attach(7);
  WRIST.attach(8);
  FOREARM.attach(9);
  pinMode(BICEP_FORWARD, OUTPUT);
  pinMode(BICEP_BACKWARD, OUTPUT);
  pinMode(BICEP_ENABLE, OUTPUT);
  pinMode(CYCLOID_DRIVE1_UP, OUTPUT);
  pinMode(CYCLOID_DRIVE1_DOWN, OUTPUT);
  pinMode(CYCLOID_DRIVE1_ENABLE, OUTPUT);
  pinMode(CYCLOID_DRIVE2_UP, OUTPUT);
  pinMode(CYCLOID_DRIVE2_DOWN, OUTPUT);
  pinMode(CYCLOID_DRIVE2_ENABLE, OUTPUT);

  pinMode(BICEP_ENCODER_A, INPUT_PULLUP);
  pinMode(BICEP_ENCODER_B, INPUT_PULLUP);
  pinMode(CYCLOID_DRIVE1_ENCODER_A, INPUT_PULLUP);
  pinMode(CYCLOID_DRIVE1_ENCODER_B, INPUT_PULLUP);
  pinMode(CYCLOID_DRIVE2_ENCODER_A, INPUT_PULLUP);
  pinMode(CYCLOID_DRIVE2_ENCODER_B, INPUT_PULLUP);


  POINTER_FINGER.write(0);
  MIDDLE_FINGER.write(0);
  RING_FINGER.write(0);
  PINKY_FINGER.write(0);
  THUMB.write(0);
  WRIST.write(0);
  FOREARM.write(0);
  digitalWrite(BICEP_ENABLE, LOW);
  digitalWrite(CYCLOID_DRIVE1_ENABLE, LOW);
  digitalWrite(CYCLOID_DRIVE2_ENABLE, LOW);
}

void loop() {
  write_hand(0);
  digitalWrite(BICEP_ENABLE, LOW);
  digitalWrite(CYCLOID_DRIVE1_ENABLE, LOW);
  digitalWrite(CYCLOID_DRIVE2_ENABLE, LOW);

  while(analogRead(EMG_FOREARM)>100){
    write_hand(180);
    delay(10);
  }
  
  while(analogRead(EMG_BICEP_FORWARD)>100 && bicep_encoder_degree<225){
    int* encoderData = getDegreeChange(prev_bicep_pulse, digitalRead(BICEP_ENCODER_A), digitalRead(BICEP_ENCODER_B));
    bicep_encoder_degree += encoderData[0] * 360.0/300;
    prev_bicep_pulse = encoderData[1];
    digitalWrite(BICEP_FORWARD, HIGH);
    digitalWrite(BICEP_BACKWARD, LOW);
    digitalWrite(BICEP_ENABLE, HIGH);
    delay(10);
  }

  while(analogRead(EMG_BICEP_BACKWARD)>100 && bicep_encoder_degree>0){
    int* encoderData = getDegreeChange(prev_bicep_pulse, digitalRead(BICEP_ENCODER_A), digitalRead(BICEP_ENCODER_B));
    bicep_encoder_degree += encoderData[0] * 360.0/300;
    prev_bicep_pulse = encoderData[1];    digitalWrite(BICEP_FORWARD, LOW);
    digitalWrite(BICEP_BACKWARD, HIGH);
    digitalWrite(BICEP_ENABLE, HIGH);
    delay(10);
  }

  while(analogRead(EMG_SHOULDER_UP)>100 && cycloid_drive1_encoder_degree<180){
    int* encoderData = getDegreeChange(prev_cycloid_drive1_pulse, digitalRead(CYCLOID_DRIVE1_ENCODER_A), digitalRead(CYCLOID_DRIVE1_ENCODER_B));
    cycloid_drive1_encoder_degree += encoderData[0] * 360.0/300;
    prev_cycloid_drive1_pulse = encoderData[1];
    digitalWrite(CYCLOID_DRIVE1_UP, HIGH);
    digitalWrite(CYCLOID_DRIVE1_DOWN, LOW);
    digitalWrite(CYCLOID_DRIVE1_ENABLE, HIGH);
    delay(10);
  }

  while(analogRead(EMG_SHOULDER_DOWN)>100 && cycloid_drive1_encoder_degree>0){
    int* encoderData = getDegreeChange(prev_cycloid_drive1_pulse, digitalRead(CYCLOID_DRIVE1_ENCODER_A), digitalRead(CYCLOID_DRIVE1_ENCODER_B));
    cycloid_drive1_encoder_degree += encoderData[0] * 360.0/300;
    prev_cycloid_drive1_pulse = encoderData[1];    digitalWrite(CYCLOID_DRIVE1_UP, LOW);
    digitalWrite(CYCLOID_DRIVE1_UP, LOW);
    digitalWrite(CYCLOID_DRIVE1_DOWN, HIGH);
    digitalWrite(CYCLOID_DRIVE1_ENABLE, HIGH);
    delay(10);
  }

}

int* getDegreeChange(int prev, int a, int b){
  int output[2];
  int curr = 0;
  if(a==1 && b==1) curr = 3;
  if(a==1 && b==0) curr = 2;
  if(a==0 && b==1) curr = 1;
  if(prev == 0 && curr == 3) output[0]= -1;
  if(prev == 3 && curr == 0) output[0]= 1;
  output[0] = curr-prev;
  output[1] = curr;
  return output;
}

void write_hand(int x){
  POINTER_FINGER.write(x);
  MIDDLE_FINGER.write(x);
  RING_FINGER.write(x);
  PINKY_FINGER.write(x);
  THUMB.write(x);
}
