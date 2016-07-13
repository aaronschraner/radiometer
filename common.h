#ifndef COMMON_H
#define COMMON_H

#include <wiringPi.h>
#include <stdint.h>

typedef uint8_t byte;
typedef int Pin;
void setPin(Pin p, bool value);


#endif

