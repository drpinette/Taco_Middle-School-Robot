#define DEBUG
#include <Condition.h>
#include <Taco.h>

bool Condition::Compare(float value1, Comparison comparison, float value2)
{
  switch (comparison) {
  case Same: return value1 == value2;
  case Different: return value1 != value2;
  case MoreThan: return value1 > value2;
  case AtLeast: return value1 >= value2;
  case LessThan: return value1 < value2;
  case AtMost: return value1 <= value2;
  default: return true;  // Undefined operator.  Should never reach this point.
  }
}

DistanceFor::DistanceFor(int sonarId, Comparison comparison, float thresholdDistance, RobotController* robotController)
{
  this->sonarId = sonarId;
  this->comparison = comparison;
  this->thresholdDistance = thresholdDistance;
  this->robotController = robotController;
}

bool DistanceFor::test() 
{
  float distance = robotController->readDistanceSonar(sonarId);
  bool result = Compare(distance, comparison, thresholdDistance);
  _D(distance); _D(sonarId); _NL;
  delay(10);
  return result;
}

All::All(Condition* conditions[])
{
  this->conditions = conditions;
}

All::~All()
{
  if (conditions != NULL) {
    for (int i = 0; i < (sizeof(conditions)/sizeof(*conditions)); i++) delete conditions[i];
    delete[] conditions;
  }
}

bool All::test()
{
  for (int i = 0; i < (sizeof(conditions)/sizeof(*conditions)); i++) if (!conditions[i]->test()) return false;
  return true;
}

Any::Any(Condition* conditions[])
{
  this->conditions = conditions;
}

Any::~Any()
{
  if (conditions != NULL) {
    for (int i = 0; i < (sizeof(conditions)/sizeof(*conditions)); i++) delete conditions[i];
    delete[] conditions;
  }
}

bool Any::test()
{
  for (int i = 0; i < (sizeof(conditions)/sizeof(*conditions)); i++) if (conditions[i]->test()) return true;
  return false;
}

bool True::test()
{
  return true;
}

bool False::test()
{
  return false;
}

MovedBy::MovedBy(float distanceToMove, Heading heading, RobotController* robotController)
{
  this->distanceToMove = distanceToMove;
  this->heading = heading;
  initialDistance = 0;
}

bool MovedBy:: test()
{
  if (initialDistance = 0)
  {
    initialDistance = robotController->readDistanceSonar(robotController->sonarIdAt(heading, NoSide, CW));
    return false;
  }
  else return robotController->readDistanceSonar(robotController->sonarIdAt(heading, NoSide, CW)) > (initialDistance + distanceToMove);
}
