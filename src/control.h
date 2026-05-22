#ifndef NAV1_CONTROL_H
#define NAV1_CONTROL_H

#include "nav1.h"

typedef struct {
    double y;
} NAV1_LowPassFilter;

NAV_EXPORT void NAV1_CTL_lpfInit(NAV1_LowPassFilter *f, double initial);
NAV_EXPORT double NAV1_CTL_lpfUpdate(NAV1_LowPassFilter *f, double x, double alpha);

typedef struct {
    double last;
} NAV1_SlewLimiter;

NAV_EXPORT void NAV1_CTL_slewInit(NAV1_SlewLimiter *s, double initial);
NAV_EXPORT double NAV1_CTL_slewUpdate(NAV1_SlewLimiter *s, double x, double maxRate, double dt);

typedef struct {
    double *buffer;
    int size;
    int index;
    int count;
    double sum;
} NAV1_MovingAverage;

NAV_EXPORT void NAV1_CTL_maInit(NAV1_MovingAverage *ma, double *buffer, int size);
NAV_EXPORT double NAV1_CTL_maUpdate(NAV1_MovingAverage *ma, double x);

typedef struct {
    double Kp, Ki, Kd;
    double integral;
    double lastError;
    double outMin, outMax;
} NAV1_PIDController;

NAV_EXPORT void NAV1_CTL_pidInit(NAV1_PIDController *pid, double Kp, double Ki, double Kd, double outMin, double outMax);
NAV_EXPORT double NAV1_CTL_pidUpdate(NAV1_PIDController *pid, double setpoint, double input, double dt);

typedef struct {
    double angle;
} NAV1_ComplementaryFilter;

NAV_EXPORT void NAV1_CTL_cfInit(NAV1_ComplementaryFilter *f, double initial);
NAV_EXPORT double NAV1_CTL_cfUpdate(NAV1_ComplementaryFilter *f, double accelAngle, double gyroRate, double dt, double alpha);

#endif
