#ifndef OHJOY_EVENT_H
#define OHJOY_EVENT_H

namespace ohjoy
{
  struct Event
  {
    unsigned int timestamp;
    short value;
    unsigned char type;
    unsigned char id;

    bool isInit() const;
    bool isAxis() const;
    bool isButton() const;
    bool isPressed() const;
    double getValue() const;
  };
}

#endif // OHJOY_EVENT_H
