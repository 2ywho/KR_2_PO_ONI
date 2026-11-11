#pragma once
#include <QString>
class FlightController{public:virtual double calculate_control(double error,double dt)=0;virtual QString controller_name()const=0;virtual ~FlightController()=default;};
class SimpleController final:public FlightController{double coefficient=.5;public:double calculate_control(double error,double dt)override;QString controller_name()const override{return "Simple";}};
