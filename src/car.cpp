#include "car.h"

Car::Car(byte left_dir_pin, byte left_speed_pin, byte right_dir_pin, byte right_speed_pin, int echo_pin, int trig_pin){
  LEFT_DIR_PIN = left_dir_pin;
  LEFT_SPEED_PIN = left_speed_pin;
  RIGHT_DIR_PIN = right_dir_pin;
  RIGHT_SPEED_PIN = right_speed_pin;

  ECHO_PIN = echo_pin; // ultrasonic module ECHO to A3
  TRIG_PIN = trig_pin;
  
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); 
}

void Car::forward_left(int car_speed){
  digitalWrite(LEFT_DIR_PIN, LEFT_DIR_AHEAD);
  analogWrite(LEFT_SPEED_PIN, car_speed);
//  Serial.print("Left forward speed: "); Serial.print(car_speed, DEC); Serial.print(", ");
}

void Car::forward_right(int car_speed){
  digitalWrite(RIGHT_DIR_PIN, RIGHT_DIR_AHEAD);
  analogWrite(RIGHT_SPEED_PIN, car_speed);
//  Serial.print("Right forward speed: "); Serial.println(car_speed, DEC);
}

void Car::back_left(int car_speed){
  digitalWrite(LEFT_DIR_PIN, LEFT_DIR_BACK);
  analogWrite(LEFT_SPEED_PIN, car_speed);
//  Serial.print("Left back speed: "); Serial.print(car_speed, DEC); Serial.print(", ");
}

void Car::back_right(int car_speed){
  digitalWrite(RIGHT_DIR_PIN, RIGHT_DIR_BACK);
  analogWrite(RIGHT_SPEED_PIN, car_speed);
//  Serial.print("Right back speed: "); Serial.println(car_speed, DEC);
}

void Car::stop(){
  analogWrite(LEFT_SPEED_PIN, 0);
  analogWrite(RIGHT_SPEED_PIN, 0);
}

float Car::map(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Car::move_car(PS2X ps2x){
  right_stick_x = ps2x.Analog(PSS_RX) - 128;

  if (right_stick_x < 0){
    left_wheel_decreaser = map(right_stick_x, -128, 0, 0, 1);
    right_wheel_decreaser = 1;
  } else if (right_stick_x > 0){
    right_wheel_decreaser = map(right_stick_x, 0, 127, 1, 0);
    left_wheel_decreaser = 1;
  } else {
    right_wheel_decreaser = 1;
    left_wheel_decreaser = 1;
  }
  

  if(ps2x.Button(PSB_PAD_UP)){
    this->forward_left(ps2x.Analog(PSAB_PAD_UP) * left_wheel_decreaser);
    this->forward_right(ps2x.Analog(PSAB_PAD_UP) * right_wheel_decreaser);
  } else if (ps2x.Button(PSB_PAD_DOWN)){
    this->back_left(ps2x.Analog(PSAB_PAD_DOWN) * left_wheel_decreaser);
    this->back_right(ps2x.Analog(PSAB_PAD_DOWN) * right_wheel_decreaser);
  } else if (ps2x.Button(PSB_PAD_RIGHT)){
    this->forward_left(ps2x.Analog(PSAB_PAD_RIGHT));
    this->back_right(ps2x.Analog(PSAB_PAD_RIGHT));
  } else if (ps2x.Button(PSB_PAD_LEFT)){
    this->back_left(ps2x.Analog(PSAB_PAD_LEFT));
    this->forward_right(ps2x.Analog(PSAB_PAD_LEFT));
  } else {
    this->stop();
  }
}

int Car::get_ultrasonic_distance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long distance = pulseIn(ECHO_PIN, HIGH); // reading the duration of high level
  
  distance= distance/58; // Transform pulse time to distance
  return distance; //return distance to this function
}

void Car::follow(){
  int distance = get_ultrasonic_distance();
  if (distance < 15){
    this->back_left(100);
    this->back_right(100);
  } else if (distance > 20 && distance < 40) {
    this->forward_left(100);
    this->forward_right(100);
  } else {
    this->stop();
  }
}
