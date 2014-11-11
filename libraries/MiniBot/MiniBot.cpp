#include <MiniBot.h>

using namespace nurc;

nurc::Servo::Servo(int timer_id) :
  bottom_count_(6),
  top_count_(19)
{
  // Configure the timers
  if(timer_id == TIMER_0) {
    // Set A7 as an output
    DDRA = (1 << 7);
    
    // Set non-inverting mode
    // Set fast PWM Mode
    TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    
    // Set the frequency to 50Hz and initial duty cycle of 0
    OCR0A = 110;
    OCR0B = angle;
  }
  else if(timer_id == TIMER_1) {
    // Set A7 as an output
    DDRA = (1 << 5);
    
    // Set non-inverting mode
    // Set fast PWM Mode
    TCCR1A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR1B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    
    // Set the frequency to 50Hz and initial duty cycle of 0
    OCR1A = 110;
    OCR1B = angle;
  }
}

nurc::Servo::setAngle(int angle)
{
  OCR0B = map(angle, 0, 180, bottom_count_, top_count_);
}

nurc::Servo::~Servo()
{
  if(timer_ == TIMER_0) {
    TCCR0A &= ~(1 << 31);
  }
  else if(timer_ == TIMER_1) {
    TCCR1A &= ~(1 << 31);
  }
}

nurc::Motor::Motor(int drive_pin, int direction_pin) :
  drive_pin_(drive_pin),
  direction_pin_(direction_pin),
  drive_state_(false)
{
  pinMode(pin, OUTPUT);
  changeDirection(FORWARD);
  driving(false);
}

nurc::Motor::driving(bool state)
{
  digitalWrite(drive_pin_, LOW);
  drive_state_ = state;
}

nurc::Motor::changeDirection(Direction d)
{
  direction_ = d;
  digitalWrite(direction_pin_, d);
}

nurc::Motor::~Motor()
{
  digitalWrite(drive_pin_, LOW);
}

nurc::MiniBot() :
  left_motor_(),
  right_motor_(),
  gripper_(),
  lifter_()
{
  // Making sure that the clock frequency is maxed
  if (F_CPU == 8000000) clock_prescale_set(clock_div_1);
}

void nurc::MiniBot::drive()
{
  left_motor_.driving(true);
  right_motor_.driving(true);
}

void nurc::MiniBot::stop()
{
  left_motor_.driving(false);
  right_motor_.driving(false);
}

nurc::MiniBot::moveForward()
{
  left_motor_.setDirection(FORWARD);
  right_motor_.setDirection(FORWARD);
  drive();
}

nurc::MiniBot::moveBackward()
{
  left_motor_.setDirection(BACKWARD);
  right_motor_.setDirection(BACKWARD);
  drive();
}

nurc::MiniBot::turnLeft()
{
  left_motor_.setDirection(BACKWARD);
  right_motor_.setDirection(FORWARD);
  drive();
}

nurc::MiniBot::turnRight()
{
  left_motor_.setDirection(FORWARD);
  right_motor_.setDirection(BACKWARD);
  drive();
}


