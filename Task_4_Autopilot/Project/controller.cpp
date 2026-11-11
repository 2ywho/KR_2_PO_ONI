#include "controller.h"
#include <cmath>
#include <stdexcept>
double SimpleController::calculate_control(double error,double dt){if(!std::isfinite(error)||!std::isfinite(dt)||dt<=0)throw std::invalid_argument("invalid controller input");return coefficient*error;}
