#pragma once
#include <QString>
class Sensor{public:virtual double read()=0;virtual bool is_ready()const=0;virtual QString name()const=0;virtual double accuracy()const=0;virtual ~Sensor()=default;};
class GPS final:public Sensor{bool ready=true;public:double read()override;bool is_ready()const override{return ready;}QString name()const override{return "GPS";}double accuracy()const override{return 1.5;}void set_ready(bool v){ready=v;}};
