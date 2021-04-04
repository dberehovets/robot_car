#ifndef Hand_h
#define Hand_h

#include "Arduino.h"
#include <PS2X_lib.h>
#include <Servo.h>


class Hand {
  int _horiz_angle = 100;
  int _middle_angle = 20;
  int _vertical_angle = 130;
  int _claws_angle = 10;

  const int sleep_angles[4] = {100, 20, 135, 180};

  const short _max_horiz_angle = 180;
  const short _min_horiz_angle = 0;
  const short _max_middle_angle = 180;
  const short _min_middle_angle = 15;
  const short _max_vertical_angle = 172;
  const short _min_vertical_angle = 0;
  const short _max_claws_angle = 180;
  const short _min_claws_angle = 0;

  bool _attached = false;
  bool _should_go_sleep = false;

  long _last_moved_horiz = millis();
  long _last_moved_vertical = millis();
  long _last_moved_middle = millis();

  Servo horiz_servo, forward_back_servo, vertical_servo, claws_servo;

  bool _is_sleep_position();

  void _go_sleep();

  public:
    void attach(int horiz_servo_pin, int forward_back_servo_pin, int vertical_servo_pin, int claws_servo_pin);

    void update_angles();

    void raise_down(int speed);
    void right_left(int speed);
    void forward_back(int speed);
    void activate_claws(int speed);

    void set_should_go_sleep(bool value);

    void move_hand(PS2X ps2x);
};

#endif
