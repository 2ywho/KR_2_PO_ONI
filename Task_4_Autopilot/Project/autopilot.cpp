#include "autopilot.h"
#include <QDebug>
#include <cmath>
void Autopilot::run_cycle(){success=false;output=0;last="Autopilot cycle started\n";auto fail=[&](QString s){last+=s+"\nAutopilot cycle aborted";qDebug().noquote()<<last;};
if(!vehicle){fail("Error: vehicle is not set");return;}
for(auto*s:sensors){if(!s||!s->is_ready()){fail("Reason: sensor failure");return;}try{if(!std::isfinite(s->read())){fail("Reason: sensor failure");return;}}catch(...){fail("Reason: sensor failure");return;}}
last+="All sensors are ready\n";if(!navigation||!navigation->is_valid()){fail("Navigation data invalid");return;}
Position p;try{p=navigation->get_position();}catch(...){fail("Navigation data invalid");return;}if(!position_is_valid(p)){fail("Error: invalid navigation position");return;}
if(!controller){fail("Error: flight controller is not set");return;}double error=200-p.altitude;
try{output=controller->calculate_control(error,.1);}catch(...){fail("Error: invalid controller input");return;}if(!std::isfinite(output)){fail("Error: invalid controller output");return;}
last+=QString("Navigation: %1\nCurrent altitude: %2 m\nTarget altitude: 200 m\nAltitude error: %3 m\nController output: %4\nAutopilot cycle completed").arg(navigation->system_name()).arg(p.altitude).arg(error).arg(output);success=true;qDebug().noquote()<<last;}
