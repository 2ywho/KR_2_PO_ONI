#pragma once
#include <QString>
#include <vector>
struct Position { double latitude; double longitude; double altitude; };
bool position_is_valid(const Position& p);
class NavigationSystem {
public:
 virtual Position get_position() const=0;
 virtual bool is_valid() const=0;
 virtual QString system_name() const=0;
 virtual ~NavigationSystem()=default;
};
class GPSNavigation final:public NavigationSystem {
 Position position{55.75,37.61,150}; int satellites=10; bool valid=true;
public:
 Position get_position()const override{return position;}
 bool is_valid()const override{return satellites>=4&&valid;}
 QString system_name()const override{return "GPS";}
 void set_valid(bool value){valid=value;} void set_satellites(int value);
 void set_position(Position value){position=value;}
 int satellite_count()const{return satellites;}
};
class InertialNavigation final:public NavigationSystem {
 Position position{55.7501,37.6102,149.8}; bool valid=true;
public:
 Position get_position()const override{return position;}
 bool is_valid()const override{return valid;}
 QString system_name()const override{return "INS";}
 void set_valid(bool value){valid=value;} void set_position(Position value){position=value;}
};
class VisualNavigation final:public NavigationSystem {
 Position position{55.7499,37.6098,150.3}; bool target_tracked=true;
public:
 Position get_position()const override{return position;}
 bool is_valid()const override{return target_tracked;}
 QString system_name()const override{return "Visual";}
 void set_valid(bool value){target_tracked=value;} void set_target_tracked(bool value){target_tracked=value;}
 void set_position(Position value){position=value;}
};
class NavigationManager {
 mutable NavigationSystem* active_system=nullptr;
 std::vector<NavigationSystem*> priority;
 void refresh()const;
public:
 void set_system(NavigationSystem* system);
 void set_sources(const std::vector<NavigationSystem*>& sources);
 bool is_available()const;
 Position get_position()const;
 void print_status()const;
 NavigationSystem* active()const;
};
