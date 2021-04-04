#include "car.h"
#include "hand.h"
#include "Arduino.h"
#include <PS2X_lib.h>
#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures true
#define rumble true

/****** Car pins ******/
byte LEFT_DIR_PIN = 2; //define direction control pin of motor A as D2
byte LEFT_SPEED_PIN = 3; //define speed control pin of motor A as D3
byte RIGHT_DIR_PIN = 4; //define direction control pin of motor B as D4
byte RIGHT_SPEED_PIN = 5; //define speed control pin of motor B as D5

int ECHO_PIN = A4; // ultrasonic module ECHO to A3
int TRIG_PIN = A3;
/********************************/

/****** Hand pins ******/
byte HORIZ_SERVO_PIN = A0;
byte FORWARD_BACK_SERVO_PIN = A1;
byte VERTICAL_SERVO_PIN = A2;
byte CLAWS_SERVO_PIN = 8;
/********************************/

bool follow_mode = false;
Hand hand;
Car car(LEFT_DIR_PIN, LEFT_SPEED_PIN, RIGHT_DIR_PIN, RIGHT_SPEED_PIN, ECHO_PIN, TRIG_PIN);

PS2X ps2x;

int error = 0;

void setup() {
  Serial.begin(57600);
  delay(300);
  error=ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  pinMode(HORIZ_SERVO_PIN, OUTPUT);
  pinMode(FORWARD_BACK_SERVO_PIN, OUTPUT);
  pinMode(VERTICAL_SERVO_PIN, OUTPUT);
  pinMode(CLAWS_SERVO_PIN, OUTPUT);

  hand.attach(HORIZ_SERVO_PIN, FORWARD_BACK_SERVO_PIN, VERTICAL_SERVO_PIN, CLAWS_SERVO_PIN);

  if(error != 0){
    Serial.print("Controller error: "); Serial.println(error, DEC);
  }
}

void loop() {
  if(error==1) {
    error=ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print("Controller error: "); Serial.println(error, DEC);
    return;
  }
  if(error==2){
    ps2x.read_gamepad();
  } else {
    ps2x.read_gamepad();
    hand.move_hand(ps2x);

    if (follow_mode){
      car.follow();
    } else {
      car.move_car(ps2x);
    }
  }

  delay(15);
}
