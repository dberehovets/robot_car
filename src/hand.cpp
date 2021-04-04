#include "hand.h"
#include "Arduino.h"

void Hand::attach(int horiz_servo_pin, int forward_back_servo_pin, int vertical_servo_pin, int claws_servo_pin){
  if (_attached)
    return;

  horiz_servo.attach(horiz_servo_pin);
  forward_back_servo.attach(forward_back_servo_pin);
  vertical_servo.attach(vertical_servo_pin);
  claws_servo.attach(claws_servo_pin);

  delay(1000);
  horiz_servo.write(_horiz_angle);
  forward_back_servo.write(_middle_angle);
  vertical_servo.write(_vertical_angle);
  claws_servo.write(_claws_angle);

  _attached = true;
}

void Hand::update_angles(){

  // make sure the angles don't exceed the given limits.
  if (_horiz_angle > _max_horiz_angle || _horiz_angle < _min_horiz_angle) {
    _horiz_angle = (_horiz_angle > _max_horiz_angle) ? _max_horiz_angle : _min_horiz_angle;
  }
  if (_middle_angle > _max_middle_angle || _middle_angle < _min_middle_angle) {
    _middle_angle = (_middle_angle > _max_middle_angle) ? _max_middle_angle : _min_middle_angle;
  }
  if (_vertical_angle > _max_vertical_angle || _vertical_angle < _min_vertical_angle) {
    _vertical_angle = (_vertical_angle > _max_vertical_angle) ? _max_vertical_angle : _min_vertical_angle;
  }
  if (_claws_angle > _max_claws_angle || _claws_angle < _min_claws_angle) {
    _claws_angle = (_claws_angle > _max_claws_angle) ? _max_claws_angle : _min_claws_angle;
  }
  horiz_servo.write(_horiz_angle);
  forward_back_servo.write(_middle_angle);
  vertical_servo.write(_vertical_angle);
  claws_servo.write(_claws_angle);
}

void Hand::raise_down(int speed){
  if (speed == -1 || (millis() - _last_moved_vertical < 1000 / abs(speed)))
    return;
  _vertical_angle = (speed < 0) ? _vertical_angle + 1 : _vertical_angle - 1;
  _last_moved_vertical = millis();
}

void Hand::right_left(int speed){
  if (millis() - _last_moved_horiz < 1000 / abs(speed)) return;
  _horiz_angle = (speed < 0) ? _horiz_angle + 1 : _horiz_angle - 1;
  _last_moved_horiz = millis();
}

void Hand::forward_back(int speed){
  if (millis() - _last_moved_middle < 1000 / abs(speed)) return;
  _middle_angle = (speed > 0) ? _middle_angle + 1 : _middle_angle - 1;
  _last_moved_middle = millis();
}

void Hand::activate_claws(int speed){
  _claws_angle = (speed > 0) ? _claws_angle + 2 : _claws_angle - 2;
}

void Hand::set_should_go_sleep(bool value = false){
  _should_go_sleep = value;
}

void Hand::_go_sleep(){
  if (_horiz_angle < sleep_angles[0]){
    _horiz_angle++;
  } else if (_horiz_angle > sleep_angles[0]){
    _horiz_angle--;
  }

  if (_middle_angle < sleep_angles[1]){
    _middle_angle++;
  } else if (_middle_angle > sleep_angles[1]){
    _middle_angle--;
  }

  if (_vertical_angle < sleep_angles[2]){
    _vertical_angle++;
  } else if (_vertical_angle > sleep_angles[2]){
    _vertical_angle--;
  }

  if (_claws_angle < sleep_angles[3]){
    _claws_angle++;
  } else if (_claws_angle > sleep_angles[3]){
    _claws_angle--;
  }

}

bool Hand::_is_sleep_position(){
  return (_horiz_angle == sleep_angles[0] && _middle_angle == sleep_angles[1]
    && _vertical_angle == sleep_angles[2] && _claws_angle == sleep_angles[3]);
}

void Hand::move_hand(PS2X ps2x){
  int left_stick_y = -(ps2x.Analog(PSS_LY) - 127);
  int left_stick_x = ps2x.Analog(PSS_LX) - 128;

  right_left(left_stick_x);

  (ps2x.Button(PSB_R2)) ? forward_back(left_stick_y) : raise_down(left_stick_y);

  if (ps2x.Button(PSB_RED)) activate_claws(1);
  if (ps2x.Button(PSB_PINK)) activate_claws(-1);

  if (ps2x.Button(PSB_GREEN)){
    _should_go_sleep = true;
  }

  if (_should_go_sleep && !_is_sleep_position()){
    _go_sleep();
  } else {
    _should_go_sleep = false;
  }
  Serial.println(_horiz_angle);
  update_angles();
}
