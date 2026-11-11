#pragma once
#include "uav.h"
#include "sensor.h"
#include "navigation.h"
#include "controller.h"
class Autopilot{UAV* vehicle;std::vector<Sensor*> sensors;NavigationSystem* navigation;FlightController* controller;QString last;bool success=false;double output=0;
public:Autopilot(UAV*v,const std::vector<Sensor*>&s,NavigationSystem*n,FlightController*c):vehicle(v),sensors(s),navigation(n),controller(c){}void run_cycle();QString result()const{return last;}bool completed()const{return success;}double control_output()const{return output;}};
