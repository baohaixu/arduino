#ifndef THRESHOLD_H
#define THRESHOLD_H

class Threshold
{
public:
  int low;
  long high;
  const char *message;
  int ledPin;
  bool buzzer;

  Threshold(int l, long h, const char *msg, int led, bool bz);
};

#endif // THRESHOLD_H
