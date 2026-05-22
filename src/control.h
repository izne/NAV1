#ifndef NAV1_CONTROL_H
#define NAV1_CONTROL_H

#include "dll.h"

typedef struct {
    double y;
} NAV_Sup_LowPassFilter;

NAV_EXPORT void NAV_SUP_lpfInit(NAV_Sup_LowPassFilter *f, double initial);
NAV_EXPORT double NAV_SUP_lpfUpdate(NAV_Sup_LowPassFilter *f, double x, double alpha);

typedef struct {
    double last;
} NAV_Sup_SlewLimiter;

NAV_EXPORT void NAV_SUP_slewInit(NAV_Sup_SlewLimiter *s, double initial);
NAV_EXPORT double NAV_SUP_slewUpdate(NAV_Sup_SlewLimiter *s, double x, double maxRate, double dt);

typedef struct {
    double *buffer;
    int size;
    int index;
    int count;
    double sum;
} NAV_Sup_MovingAverage;

NAV_EXPORT void NAV_SUP_maInit(NAV_Sup_MovingAverage *ma, double *buffer, int size);
NAV_EXPORT double NAV_SUP_maUpdate(NAV_Sup_MovingAverage *ma, double x);

typedef struct {
    double Kp, Ki, Kd;
    double integral;
    double lastError;
    double outMin, outMax;
} NAV_Sup_PIDController;

NAV_EXPORT void NAV_SUP_pidInit(NAV_Sup_PIDController *pid, double Kp, double Ki, double Kd, double outMin, double outMax);
NAV_EXPORT double NAV_SUP_pidUpdate(NAV_Sup_PIDController *pid, double setpoint, double input, double dt);

typedef struct {
    double angle;
} NAV_Sup_ComplementaryFilter;

NAV_EXPORT void NAV_SUP_cfInit(NAV_Sup_ComplementaryFilter *f, double initial);
NAV_EXPORT double NAV_SUP_cfUpdate(NAV_Sup_ComplementaryFilter *f, double accelAngle, double gyroRate, double dt, double alpha);

#endif
