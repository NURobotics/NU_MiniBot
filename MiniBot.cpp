#include <MiniBot.h>

using namespace nurc;

void MiniBot::initialize()
{
  // PUT ALL OF YOUR INITIALIZATION CODE HERE
  // This would include setting up any objects you inserted into the MiniBot header file
  // You could add more Motors, Servos, or your own functionality for sensing, etc.
  
}


void MiniBot::release()
{
  // PUT ANY EXIT CODE HERE
  
}

/********************************************************************************/

/**
 *  Initializes the Servo using an emum specified by timer_id
 */
Servo::Servo(Timer timer_id)
{
  // Configuring a duty cycle to angle range conversion
  // Using a 50Hz servo operating between appoximately 0.75ms and 2.4ms
  bottom_count_ = 6;
  top_count_ = 19;
  configureTimer(timer_id);
}

void Servo::configureTimer(Timer timer_id)
{
  if(timer_id == TIMER_0) {
    DDRB |= (1 << 2);             // port 7 is the OC0 output
    DDRA |= (1 << 7);
    TCCR0A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    OCR0A = 110;
    OCR0B = bottom_count_;
    timer_ = timer_id;
  }
  else if(timer_id == TIMER_1) {
    DDRA = (1 << 6);              // port 8 is the OC1 output
    DDRA = (1 << 5);
    TCCR1A = (1 << COM0A0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR1B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
    OCR1A = 110;
    OCR1B = bottom_count_;
    timer_ = timer_id;
  }
  else
    timer_ = INVALID_TIMER;
}

void Servo::reset(Timer timer_id)
{
  // Turn off the previous timer and configure the desired timer
  if(timer_ == TIMER_0)
    TCCR0A &= ~(1 << 31);
  else if(timer_ == TIMER_1)
    TCCR1A &= ~(1 << 31);
  configureTimer(timer_id);
}

/**
 *  Set the servo angle by mapping the angle between the min and max timer counts
 */
void Servo::setAngle(int angle)
{
  // Map the servo angle to the timer count using the servo delay conversion
  if(timer_ == TIMER_0)
    OCR0B = map(angle, 0, 180, bottom_count_, top_count_);
  else
    OCR1B = map(angle, 0, 180, bottom_count_, top_count_);
}

/**
 *  Turn the output compare modules off when deleted
 */
Servo::~Servo()
{
  // Turn off the pwm output
  if(timer_ == TIMER_0)
    TCCR0A &= ~(1 << 31);
  else if(timer_ == TIMER_1)
    TCCR1A &= ~(1 << 31);
}

Motor::Motor(int drive_pin, int direction_pin, bool inverted)
{
  reset(drive_pin, direction_pin, inverted);
}

void Motor::reset(int drive_pin, int direction_pin, bool inverted)
{
  // Configure the new desired pins and inversion
  drive_pin_ = drive_pin;
  direction_pin_ = direction_pin;
  drive_state_ = false;
  inverted_ = inverted;
  
  pinMode(drive_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);
  setDirection(FORWARD);
  driving(false);
}

void Motor::driving(bool state)
{
  // Enable both motors
  drive_state_ = state;
  if(drive_state_ == true)
    digitalWrite(drive_pin_, HIGH);
  else
    digitalWrite(drive_pin_, LOW);
}

void Motor::setDirection(Direction d)
{
  // Change the phase for both motors for directional control
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

Motor::~Motor()
{
  digitalWrite(drive_pin_, LOW);
}

MiniBot::MiniBot()
{
  // Making sure that the clock frequency is maxed
  if (F_CPU == 8000000) clock_prescale_set(clock_div_1);
  reset();
}

void MiniBot::reset()
{
  left_motor_ = Motor(0, 1);
  right_motor_ = Motor(2, 3);
  initialize();
}

void MiniBot::drive()
{
  left_motor_.driving(true);
  right_motor_.driving(true);
}

void MiniBot::stop()
{
  left_motor_.driving(false);
  right_motor_.driving(false);
}

void MiniBot::moveForward()
{
  left_motor_.setDirection(FORWARD);
  right_motor_.setDirection(FORWARD);
  drive();
}

void MiniBot::moveBackward()
{
  left_motor_.setDirection(BACKWARD);
  right_motor_.setDirection(BACKWARD);
  drive();
}

void MiniBot::turnLeft()
{
  left_motor_.setDirection(BACKWARD);
  right_motor_.setDirection(FORWARD);
  drive();
}

void MiniBot::turnRight()
{
  left_motor_.setDirection(FORWARD);
  right_motor_.setDirection(BACKWARD);
  drive();
}

MiniBot::~MiniBot()
{
  release();
}
