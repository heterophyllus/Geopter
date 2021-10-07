#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


class Environment
{
public:
    Environment();

    /**
     * @brief Set room temperature
     * @param t temperature in Celcius
     */
    static void set_temperature(double t);

    /**
     * @brief Set air pressure
     * @param p air pressure in Pascal
     */
    static void set_air_pressure(double p);

    static double temperature();

    static double air_pressure();

private:
    static double temperature_;
    static double pressure_;
};

#endif // ENVIRONMENT_H
