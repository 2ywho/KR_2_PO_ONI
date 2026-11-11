#include "navigation.h"
#include <QCoreApplication>
#include <iostream>
#include <limits>
int main(int argc,char**argv){QCoreApplication app(argc,argv);int n=0,f=0;
auto check=[&](bool ok,const char*msg){++n;if(!ok){++f;std::cerr<<"FAIL "<<msg<<'\n';}};
GPSNavigation gps;InertialNavigation ins;VisualNavigation visual;NavigationManager manager;
check(!manager.is_available(),"empty manager");manager.set_sources({&gps,&ins,&visual});
check(manager.active()==&gps,"GPS first priority");gps.set_valid(false);check(manager.active()==&ins,"automatic fallback INS");
ins.set_valid(false);check(manager.active()==&visual,"automatic fallback Visual");visual.set_valid(false);check(!manager.is_available(),"all invalid");
bool threw=false;try{manager.get_position();}catch(...){threw=true;}check(threw,"no fabricated position");
gps.set_valid(true);check(manager.active()==&gps,"GPS recovery");gps.set_satellites(3);check(!gps.is_valid(),"3 satellites invalid");gps.set_satellites(4);check(gps.is_valid(),"4 satellites valid");
gps.set_position({55,37,-1});check(!manager.is_available(),"negative altitude rejected");gps.set_position({91,37,150});check(!manager.is_available(),"latitude rejected");
gps.set_position({55,181,150});check(!manager.is_available(),"longitude rejected");gps.set_position({55,37,std::numeric_limits<double>::quiet_NaN()});check(!manager.is_available(),"NaN rejected");
gps.set_position({55,37,150});manager.set_sources({nullptr,&gps});check(manager.active()==&gps,"null source skipped");check(manager.get_position().altitude==150,"position forwarded");
manager.set_system(nullptr);check(!manager.is_available(),"set null safe");
std::cout<<"Checks: "<<n<<" failed: "<<f<<'\n';return f?1:0;}
