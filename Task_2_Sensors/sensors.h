#pragma once
#include <QString>
#include <vector>

class Sensor {
public:
    virtual double read() = 0;
    virtual bool is_ready() const = 0;
    virtual QString name() const = 0;
    virtual double accuracy() const = 0;
    virtual ~Sensor() = default;
};

class NavigationSensor : public Sensor {
public:
    bool is_ready() const override;
    double accuracy() const override;
    void setReady(bool value);
    void setAccuracy(double value);
    void setCount(int value);
protected:
    NavigationSensor(int count, double accuracy);
    int count() const;
private:
    bool ready_ = true;
    int count_;
    double accuracy_;
};
class GPS final : public NavigationSensor {
public:
    GPS();
    double read() override;
    QString name() const override;
};
class GLONASS final : public NavigationSensor {
public:
    GLONASS();
    double read() override;
    QString name() const override;
};
class VisualOdometry final : public NavigationSensor {
public:
    VisualOdometry();
    double read() override;
    QString name() const override;
};
Sensor* choose_best_sensor(const std::vector<Sensor*>& sensors);
