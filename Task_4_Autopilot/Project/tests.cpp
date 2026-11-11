#include "autopilot.h"
#include <QCoreApplication>
#include <iostream>
int main(int argc,char**argv){QCoreApplication app(argc,argv);int n=0,f=0;auto check=[&](bool b){++n;if(!b)++f;};Quadcopter q;GPS s;GPSNavigation nav;SimpleController c;
Autopilot a(&q,{&s},&nav,&c);a.run_cycle();check(a.completed()&&a.control_output()==25);
s.set_ready(false);a.run_cycle();check(!a.completed()&&a.result().contains("sensor failure"));s.set_ready(true);
nav.set_valid(false);a.run_cycle();check(!a.completed()&&a.result().contains("Navigation data invalid"));nav.set_valid(true);
Autopilot b(&q,{&s},&nav,nullptr);b.run_cycle();check(b.result().contains("flight controller is not set"));
nav.set_position({55,37,-1});a.run_cycle();check(a.result().contains("invalid navigation position"));nav.set_position({55,37,150});
Autopilot d(nullptr,{&s},&nav,&c);d.run_cycle();check(d.result().contains("vehicle is not set"));Autopilot e(&q,{nullptr},&nav,&c);e.run_cycle();check(e.result().contains("sensor failure"));
Autopilot z(&q,{&s},nullptr,&c);z.run_cycle();check(z.result().contains("Navigation data invalid"));std::cout<<"Checks "<<n<<" failed "<<f<<'\n';return f?1:0;}
