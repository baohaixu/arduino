#include "Threshold.h"

Threshold::Threshold(int l, long h, const char *msg, int led, bool bz)
    : low(l), high(h), message(msg), ledPin(led), buzzer(bz) {}
