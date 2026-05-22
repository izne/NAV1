#include "control.h"

NAV_EXPORT void NAV_SUP_lpfInit(NAV_Sup_LowPassFilter *f, double initial)
{
    f->y = initial;
}

NAV_EXPORT double NAV_SUP_lpfUpdate(NAV_Sup_LowPassFilter *f, double x, double alpha)
{
    f->y += alpha * (x - f->y);
    return f->y;
}

NAV_EXPORT void NAV_SUP_slewInit(NAV_Sup_SlewLimiter *s, double initial)
{
    s->last = initial;
}

NAV_EXPORT double NAV_SUP_slewUpdate(NAV_Sup_SlewLimiter *s, double x, double maxRate, double dt)
{
    double maxStep = maxRate * dt;
    double diff = x - s->last;
    if (diff > maxStep) diff = maxStep;
    if (diff < -maxStep) diff = -maxStep;
    s->last += diff;
    return s->last;
}

NAV_EXPORT void NAV_SUP_maInit(NAV_Sup_MovingAverage *ma, double *buffer, int size)
{
    ma->buffer = buffer;
    ma->size = size;
    ma->index = 0;
    ma->count = 0;
    ma->sum = 0.0;
}

NAV_EXPORT double NAV_SUP_maUpdate(NAV_Sup_MovingAverage *ma, double x)
{
    if (ma->count < ma->size) {
        ma->sum += x;
        ma->buffer[ma->index] = x;
        ma->index++;
        ma->count++;
    } else {
        if (ma->index >= ma->size) ma->index = 0;
        ma->sum -= ma->buffer[ma->index];
        ma->buffer[ma->index] = x;
        ma->sum += x;
        ma->index++;
    }
    return ma->sum / ma->count;
}

NAV_EXPORT void NAV_SUP_pidInit(NAV_Sup_PIDController *pid, double Kp, double Ki, double Kd, double outMin, double outMax)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0.0;
    pid->lastError = 0.0;
    pid->outMin = outMin;
    pid->outMax = outMax;
}

NAV_EXPORT double NAV_SUP_pidUpdate(NAV_Sup_PIDController *pid, double setpoint, double input, double dt)
{
    double error = setpoint - input;
    double pTerm = pid->Kp * error;
    pid->integral += error * dt;
    double iTerm = pid->Ki * pid->integral;
    if (iTerm > pid->outMax) iTerm = pid->outMax;
    if (iTerm < pid->outMin) iTerm = pid->outMin;
    double dTerm = pid->Kd * (error - pid->lastError) / dt;
    pid->lastError = error;
    double output = pTerm + iTerm + dTerm;
    if (output > pid->outMax) output = pid->outMax;
    if (output < pid->outMin) output = pid->outMin;
    return output;
}

NAV_EXPORT void NAV_SUP_cfInit(NAV_Sup_ComplementaryFilter *f, double initial)
{
    f->angle = initial;
}

NAV_EXPORT double NAV_SUP_cfUpdate(NAV_Sup_ComplementaryFilter *f, double accelAngle, double gyroRate, double dt, double alpha)
{
    f->angle = alpha * accelAngle + (1.0 - alpha) * (f->angle + gyroRate * dt);
    return f->angle;
}
