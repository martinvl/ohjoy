#ifndef OHJOY_DS4_H
#define OHJOY_DS4_H

#include "ohjoy/device.h"

namespace ohjoy
{
  class DS4 : public Device
  {
    public:
      enum class Button
      {
        square,
        x,
        circle,
        triangle,
        l1,
        r1,
        l2,
        r2,
        share,
        option,
        l3,
        r3,
        ps,
        touchpad
      };

      enum class Axis
      {
        l3_h,
        l3_v,
        r3_h,
        l2,
        r2,
        r3_v,
        dpad_h,
        dpad_v,
        touchpad_h,
        touchpad_v,
        unknown
      };

      enum class Axis2d
      {
        l3,
        r3,
        dpad,
        touchpad
      };

      struct State
      {
        std::map<Axis, AxisState> axes;
        std::map<Button, ButtonState> buttons;
      };

      using Axis2dCallback = std::function<void(const double x, const double y)>;

      State getState();

      void setButtonCallback(const Button button, const ButtonCallback& cb);
      void setAxisCallback(const Axis axis, const AxisCallback& cb);
      void setAxisCallback(const Axis2d axis, const Axis2dCallback& cb);

    private:

  };
}

#endif // OHJOY_DS4_H
