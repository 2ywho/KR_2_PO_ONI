#pragma once
#include <QString>
class UAV{public:virtual void takeoff()=0;virtual void land()=0;virtual QString type()const=0;virtual int motor_count()const=0;virtual ~UAV()=default;};
class Quadcopter final:public UAV{public:void takeoff()override;void land()override;QString type()const override{return "Quadcopter";}int motor_count()const override{return 4;}};
