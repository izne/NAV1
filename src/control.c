#include "control.h"
#include <math.h>

NAV1_EXPORT void NAV1_CTL_lpfInit(NAV1_LowPassFilter *f, double initial)
{
    f->y = initial;
}

NAV1_EXPORT double NAV1_CTL_lpfUpdate(NAV1_LowPassFilter *f, double x, double alpha)
{
    f->y += alpha * (x - f->y);
    return f->y;
}

NAV1_EXPORT void NAV1_CTL_slewInit(NAV1_SlewLimiter *s, double initial)
{
    s->last = initial;
}

NAV1_EXPORT double NAV1_CTL_slewUpdate(NAV1_SlewLimiter *s, double x, double maxRate, double dt)
{
    double maxStep = maxRate * dt;
    double diff = x - s->last;
    if (diff > maxStep) diff = maxStep;
    if (diff < -maxStep) diff = -maxStep;
    s->last += diff;
    return s->last;
}

NAV1_EXPORT void NAV1_CTL_maInit(NAV1_MovingAverage *ma, double *buffer, int size)
{
    ma->buffer = buffer;
    ma->size = size;
    ma->index = 0;
    ma->count = 0;
    ma->sum = 0.0;
}

NAV1_EXPORT double NAV1_CTL_maUpdate(NAV1_MovingAverage *ma, double x)
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

NAV1_EXPORT void NAV1_CTL_pidInit(NAV1_PIDController *pid, double Kp, double Ki, double Kd, double outMin, double outMax)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0.0;
    pid->lastError = 0.0;
    pid->outMin = outMin;
    pid->outMax = outMax;
}

NAV1_EXPORT double NAV1_CTL_pidUpdate(NAV1_PIDController *pid, double setpoint, double input, double dt)
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

NAV1_EXPORT void NAV1_CTL_cfInit(NAV1_ComplementaryFilter *f, double initial)
{
    f->angle = initial;
}

NAV1_EXPORT double NAV1_CTL_cfUpdate(NAV1_ComplementaryFilter *f, double accelAngle, double gyroRate, double dt, double alpha)
{
    f->angle = alpha * accelAngle + (1.0 - alpha) * (f->angle + gyroRate * dt);
    return f->angle;
}

NAV1_EXPORT void NAV1_CTL_angularSlewInit(NAV1_AngularSlew *s, double initial)
{
    s->output = initial;
}

NAV1_EXPORT double NAV1_CTL_angularSlewUpdate(NAV1_AngularSlew *s, double target, double maxRate, double dt)
{
    double err = NAV1_GEO_shortestAngularDistance(s->output, target);
    double step = maxRate * dt;
    if (err > step) err = step;
    if (err < -step) err = -step;
    s->output = NAV1_GEO_normalizeAngle(s->output + err);
    return s->output;
}

NAV1_EXPORT double NAV1_CTL_deadband(double value, double width)
{
    if (fabs(value) < width) return 0.0;
    return value;
}
