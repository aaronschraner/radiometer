#include "common.h"
void setPin(Pin p, bool value)
{
	digitalWrite(p, value);
	delayMicroseconds(2);
}
