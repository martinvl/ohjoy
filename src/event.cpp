#include "ohjoy/event.h"

#include <limits>

namespace ohjoy
{
  constexpr unsigned char button_mask = 0x01;
  constexpr unsigned char axis_mask = 0x02;
  constexpr unsigned char init_mask = 0x80;

  bool Event::isInit() const
  {
    return static_cast<bool>(type & init_mask);
  }

  bool Event::isAxis() const
  {
    return static_cast<bool>(type & axis_mask);
  }

  bool Event::isButton() const
  {
    return static_cast<bool>(type & button_mask);
  }

  bool Event::isPressed() const
  {
    return isButton() && value != 0;
  }

  double Event::getValue() const
  {
    if (value < 0) {
      const int lo = std::numeric_limits<short>::min();

      return -static_cast<double>(value)/lo;
    } else {
      const int hi = std::numeric_limits<short>::max();

      return static_cast<double>(value)/hi;
    }
  }
}
