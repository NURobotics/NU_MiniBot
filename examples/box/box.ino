#include <MiniBot.h>

using namespace nurc;

MiniBot my_robot;

void setup()
{
  // Any configuration settings separate from the robot can be done here once
  // You should also be able to insert any long-term code for your MiniBot in the library
}

void loop()
{
  my_robot.moveForward();
  delay_ms(5000);
  my_robot.turnLeft();
  delay_ms(5000);
}
