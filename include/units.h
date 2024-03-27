#ifndef UNITS_H
#define UNITS_H
#include <math.h>
#define ONE_TENTH_MILLIMETER 1
#define FROM_MM(v) (v*10)
#define TO_MM(v) (v/10)
#ifndef M_PI
#define M_PI 3.14159265359
#endif // M_PI
#define DEGREES_TO_RADIANS(v) (v*M_PI/180.0)
#define RADIANS_TO_DEGREES(v) (v*180.0/M_PI)
#endif // UNITS_H
