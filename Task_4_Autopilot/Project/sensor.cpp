#include "sensor.h"
#include <stdexcept>
double GPS::read(){if(!ready)throw std::runtime_error("sensor not ready");return 10;}
