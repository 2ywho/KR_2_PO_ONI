#pragma once
#include <QString>

class UAV {
public:
    virtual ~UAV() = default;
    virtual void takeoff() = 0;
    virtual void land() = 0;
    virtual QString type() const = 0;
    virtual int motor_count() const = 0;
    virtual QString takeoff_type() const = 0;
    virtual QString landing_type() const = 0;
    bool isFlying() const { return flying_; }
protected:
    void changeFlightState(bool flying);
private:
    bool flying_ = false;
};

class Quadcopter final : public UAV {
public:
    void takeoff() override;
    void land() override;
    QString type() const override;
    int motor_count() const override;
    QString takeoff_type() const override;
    QString landing_type() const override;
};
class Hexacopter final : public UAV {
public:
    void takeoff() override;
    void land() override;
    QString type() const override;
    int motor_count() const override;
    QString takeoff_type() const override;
    QString landing_type() const override;
};
class Octocopter final : public UAV {
public:
    void takeoff() override;
    void land() override;
    QString type() const override;
    int motor_count() const override;
    QString takeoff_type() const override;
    QString landing_type() const override;
};

void print_vehicle_info(const UAV &vehicle);
