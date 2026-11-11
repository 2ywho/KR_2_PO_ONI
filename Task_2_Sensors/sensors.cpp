#include "sensors.h"
#include <QDebug>
#include <cmath>
#include <limits>
NavigationSensor::NavigationSensor(int count, double accuracy):count_(count),accuracy_(accuracy){}
bool NavigationSensor::is_ready() const{return ready_;}
double NavigationSensor::accuracy() const{return accuracy_;}
void NavigationSensor::setReady(bool value){ready_=value;}
void NavigationSensor::setAccuracy(double value){accuracy_=value;}
void NavigationSensor::setCount(int value){count_=value;}
int NavigationSensor::count() const{return count_;}
GPS::GPS():NavigationSensor(10,1.5){}
GLONASS::GLONASS():NavigationSensor(8,2.0){}
VisualOdometry::VisualOdometry():NavigationSensor(124,3.2){}
QString GPS::name() const{return "GPS";}
QString GLONASS::name() const{return "GLONASS";}
QString VisualOdometry::name() const{return "Visual Odometry";}
double GPS::read(){qDebug()<<name()<<"satellites:"<<count();return count();}
double GLONASS::read(){qDebug()<<name()<<"satellites:"<<count();return count();}
double VisualOdometry::read(){qDebug()<<name()<<"tracked visual features:"<<count();return count();}
Sensor* choose_best_sensor(const std::vector<Sensor*>& sensors){
    Sensor* best=nullptr;
    double smallest=std::numeric_limits<double>::infinity();
    for(Sensor* sensor:sensors){
        if(!sensor || !sensor->is_ready()) continue;
        const double error=sensor->accuracy();
        if(std::isfinite(error) && error>=0.0 && error<smallest){best=sensor;smallest=error;}
    }
    if(best) qDebug()<<"Selected:"<<best->name()<<"accuracy (m):"<<best->accuracy();
    else qDebug()<<"ERROR: no available sensor with valid accuracy";
    return best;
}
