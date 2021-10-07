#include "Environment/environment.h"

double Environment::temperature_ = 25;
double Environment::pressure_ = 101325.0;

Environment::Environment()
{
    temperature_ = 25;
    pressure_ = 101325.0;
}

void Environment::set_temperature(double t)
{
    temperature_ = t;
}

void Environment::set_air_pressure(double p)
{
    pressure_ = p;
}

double Environment::temperature()
{
    return temperature_;
}

double Environment::air_pressure()
{
    return pressure_;
}
