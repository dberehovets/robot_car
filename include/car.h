#ifndef Car_h
#define Car_h

#include "Arduino.h"
#include <PS2X_lib.h>


class Car {
  byte LEFT_DIR_PIN; //define direction control pin of motor A as D2
  byte LEFT_SPEED_PIN; //define speed control pin of motor A as D3
  byte RIGHT_DIR_PIN; //define direction control pin of motor B as D4
  byte RIGHT_SPEED_PIN; //define speed control pin of motor B as D5

  int ECHO_PIN;
  int TRIG_PIN;

  int right_stick_x = 0;

  float left_wheel_decreaser;
  float right_wheel_decreaser;

  short LEFT_DIR_AHEAD = LOW;
  short LEFT_DIR_BACK = HIGH;
  short RIGHT_DIR_AHEAD = HIGH;
  short RIGHT_DIR_BACK = LOW;

  int get_ultrasonic_distance();

  float map(float x, float in_min, float in_max, float out_min, float out_max);

  public:
    Car(byte left_dir_pin, byte left_speed_pin, byte right_dir_pin, byte right_speed_pin, int echo_pin, int trig_pin);
    void forward_left(int speed);
    void forward_right(int speed);

    void back_left(int speed);
    void back_right(int speed);
    
    void stop();

    void move_car(PS2X ps2x);

    void follow();
};

#endif
