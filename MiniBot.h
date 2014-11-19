#ifndef MINI_BOT_H
#define MINI_BOT_H

#include <Arduino.h>
#include <avr/power.h>

namespace nurc {

enum Timer {
  TIMER_0,
  TIMER_1,
  NUM_TIMERS,
  INVALID_TIMER
};

enum Direction {
  BACKWARD,
  FORWARD
};

class Servo {
public:
  Servo(Timer timer_id = TIMER_0);
  ~Servo();
  void reset(Timer timer_id);
  
  void setAngle(int angle);
  void configureTimer(Timer timer_id);
  int duty_cycle_;
  Timer timer_;
  int bottom_count_;
  int top_count_;
  int angle_;
};

class Motor {
public:
  Motor(int drive_pin = -1, int direction_pin = -1, bool inverted = false);
  ~Motor();
  void reset(int drive_pin, int direction_pin, bool inverted = false);

  void driving(bool state);
  void setDirection(Direction d);
  int drive_pin_;
  int direction_pin_;
  bool inverted_;
  int direction_;
  bool drive_state_;
};

class MiniBot {
public:
  MiniBot();
  ~MiniBot();
  void reset();
  void initialize();
  void release();

  void moveForward();
  void moveBackward();
  void turnLeft();
  void turnRight();
  void drive();
  void stop();
  Motor left_motor_;
  Motor right_motor_;
};
  
inline void delay_ms(volatile unsigned long ms) {
  ms *= 240;
  for(; ms != 0; ms--); 
}

} // namespace nurc

#endif // MINI_BOT_H
