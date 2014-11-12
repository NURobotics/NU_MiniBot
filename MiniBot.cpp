#include <MiniBot.h>

using namespace nurc;

nurc::Servo::Servo(Timer timer_id)
{
  // Configuring a duty cycle to angle range conversion
  // Using a 50Hz servo operating between 0.75ms and 2.4ms
  bottom_count_ = 6;
  top_count_ = 19;
  
  if(timer_id == TIMER_0) {
    DDRB |= (1 << 2);  //port A7 is an output
    DDRA |= (1 << 7);
    TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    OCR0A = 110;
    OCR0B = bottom_count_; //bottom_count_;
  }
  else if(timer_id == TIMER_1) {
    DDRA = (1 << 6);
    DDRA = (1 << 5);
    TCCR1A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR1B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    OCR1A = 110;
    OCR1B = bottom_count_;
  }
}

void nurc::Servo::setAngle(int angle)
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

nurc::Motor::Motor(int drive_pin, int direction_pin, bool inverted) :
  drive_pin_(drive_pin),
  direction_pin_(direction_pin),
  drive_state_(false),
  inverted_(inverted)
{
  pinMode(drive_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);
  setDirection(FORWARD);
  driving(false);
}

void nurc::Motor::driving(bool state)
{
  drive_state_ = state;
  if(drive_state_ == true)
    digitalWrite(drive_pin_, HIGH);
  else
    digitalWrite(drive_pin_, LOW);
}

void nurc::Motor::setDirection(Direction d)
{
  direction_ = d;
  if(d == FORWARD) {
    if(!inverted_)
      digitalWrite(direction_pin_, HIGH);
    else
      digitalWrite(direction_pin_, LOW);
  }
  else if(d == BACKWARD) {
    if(!inverted_)
      digitalWrite(direction_pin_, LOW);
    else
      digitalWrite(direction_pin_, HIGH);
  }
}

nurc::Motor::~Motor()
{
  digitalWrite(drive_pin_, LOW);
}

nurc::MiniBot::MiniBot()
{
  // Making sure that the clock frequency is maxed
  if (F_CPU == 8000000) clock_prescale_set(clock_div_1);
  left_motor_ = new Motor(0, 1);
  right_motor_ = new Motor(2, 3);
  lifter_ = new Servo(TIMER_1);
  gripper_ = new Servo(TIMER_0);
}

void nurc::MiniBot::drive()
{
  left_motor_->driving(true);
  right_motor_->driving(true);
}

void nurc::MiniBot::stop()
{
  left_motor_->driving(false);
  right_motor_->driving(false);
}

void nurc::MiniBot::moveForward()
{
  left_motor_->setDirection(FORWARD);
  right_motor_->setDirection(FORWARD);
  drive();
}

void nurc::MiniBot::moveBackward()
{
  left_motor_->setDirection(BACKWARD);
  right_motor_->setDirection(BACKWARD);
  drive();
}

void nurc::MiniBot::turnLeft()
{
  left_motor_->setDirection(BACKWARD);
  right_motor_->setDirection(FORWARD);
  drive();
}

void nurc::MiniBot::turnRight()
{
  left_motor_->setDirection(FORWARD);
  right_motor_->setDirection(BACKWARD);
  drive();
}

nurc::MiniBot::~MiniBot()
{
  delete left_motor_;
  delete right_motor_;
  delete lifter_;
  delete gripper_;
}
