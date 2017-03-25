#ifndef CONDITION_H
#define CONDITION_H
/////////////////////////////////////////////////////////////////////////////

class RobotController;

enum Comparison {Same, Different, MoreThan, AtLeast, LessThan, AtMost};

class Condition
{
public:
    Condition(){}
    virtual ~Condition(){}
    virtual bool test()=0;
    bool Compare(float value1, Comparison comparison, float value2);
};

class DistanceFor : public Condition
{
public:
  DistanceFor(int sonarId, Comparison comparison, float thresholdDistance, RobotController* robotController);
  bool test();
private:
  int sonarId;
  Comparison comparison;
  float thresholdDistance;
  RobotController* robotController;

};

class All : public Condition
{
public:
  All(Condition* conditions[]);
  virtual ~All();
  bool test();
private:
  Condition** conditions;
};

class Any : public Condition
{
public:
  Any(Condition* conditions[]);
  virtual ~Any();
  bool test();
private:
  Condition** conditions;
};

class True : public Condition
{
public:
  bool test();
};

class False : public Condition
{
public:
  bool test();
};
/////////////////////////////////////////////////////////////////////////////
#endif
