#include "navigation.h"
#include <cmath>
#include <stdexcept>
#include <QDebug>
bool position_is_valid(const Position&p){return std::isfinite(p.latitude)&&std::isfinite(p.longitude)&&std::isfinite(p.altitude)&&std::abs(p.latitude)<=90&&std::abs(p.longitude)<=180&&p.altitude>=0;}
void GPSNavigation::set_satellites(int v){if(v<0)throw std::invalid_argument("negative satellites");satellites=v;}
void NavigationManager::refresh()const{active_system=nullptr;for(auto*s:priority)if(s&&s->is_valid()&&position_is_valid(s->get_position())){active_system=s;break;}}
void NavigationManager::set_system(NavigationSystem*s){priority={s};refresh();}
void NavigationManager::set_sources(const std::vector<NavigationSystem*>&s){priority=s;refresh();}
bool NavigationManager::is_available()const{refresh();return active_system;}
Position NavigationManager::get_position()const{refresh();if(!active_system)throw std::runtime_error("Navigation unavailable");return active_system->get_position();}
NavigationSystem* NavigationManager::active()const{refresh();return active_system;}
void NavigationManager::print_status()const{for(auto*s:priority)if(s)qDebug().noquote()<<s->system_name()<<":"<<(s->is_valid()?"valid":"invalid");refresh();if(active_system)qDebug().noquote()<<"Active navigation system:"<<active_system->system_name();else qDebug()<<"Critical error: no navigation system available";}
