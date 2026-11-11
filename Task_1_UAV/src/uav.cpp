#include "uav.h"
#include <QDebug>
#include <stdexcept>

void UAV::changeFlightState(bool flying) {
    if (isFlying() == flying)
        throw std::logic_error(flying ? "UAV is already airborne" : "UAV is already on ground");
    flying_ = flying;
}

void Quadcopter::takeoff() { changeFlightState(true); qDebug().noquote() << type() << takeoff_type(); }
void Quadcopter::land() { changeFlightState(false); qDebug().noquote() << type() << landing_type(); }
QString Quadcopter::type() const { return QStringLiteral("Квадрокоптер"); }
int Quadcopter::motor_count() const { return 4; }
QString Quadcopter::takeoff_type() const { return QStringLiteral("Вертикальный взлёт · 4 ротора"); }
QString Quadcopter::landing_type() const { return QStringLiteral("Вертикальная посадка · 4 ротора"); }

void Hexacopter::takeoff() { changeFlightState(true); qDebug().noquote() << type() << takeoff_type(); }
void Hexacopter::land() { changeFlightState(false); qDebug().noquote() << type() << landing_type(); }
QString Hexacopter::type() const { return QStringLiteral("Гексакоптер"); }
int Hexacopter::motor_count() const { return 6; }
QString Hexacopter::takeoff_type() const { return QStringLiteral("Вертикальный взлёт · 6 роторов"); }
QString Hexacopter::landing_type() const { return QStringLiteral("Вертикальная посадка · 6 роторов"); }

void Octocopter::takeoff() { changeFlightState(true); qDebug().noquote() << type() << takeoff_type(); }
void Octocopter::land() { changeFlightState(false); qDebug().noquote() << type() << landing_type(); }
QString Octocopter::type() const { return QStringLiteral("Октокоптер"); }
int Octocopter::motor_count() const { return 8; }
QString Octocopter::takeoff_type() const { return QStringLiteral("Вертикальный взлёт · 8 роторов"); }
QString Octocopter::landing_type() const { return QStringLiteral("Вертикальная посадка · 8 роторов"); }

void print_vehicle_info(const UAV &vehicle) {
    qDebug().noquote() << "Тип:" << vehicle.type()
                       << "| Двигатели:" << vehicle.motor_count()
                       << "| Взлёт:" << vehicle.takeoff_type()
                       << "| Посадка:" << vehicle.landing_type();
}
