#include "ohjoy/ds4.h"

namespace ohjoy
{
  DS4::State DS4::getState()
  {
    State state;
    Device::State raw_state = Device::getState();

    for (const auto& kv : raw_state.axes) {
      state.axes[static_cast<Axis>(kv.first)] = kv.second;
    }

    for (const auto& kv : raw_state.buttons) {
      state.buttons[static_cast<Button>(kv.first)] = kv.second;
    }

    return state;
  }

  void DS4::setButtonCallback(const Button button, const ButtonCallback& cb)
  {
    Device::setButtonCallback(static_cast<int>(button), cb);
  }

  void DS4::setAxisCallback(const Axis axis, const AxisCallback& cb)
  {
    Device::setAxisCallback(static_cast<int>(axis), cb);
  }

  void DS4::setAxisCallback(const Axis2d axis, const Axis2dCallback& cb)
  {
    Axis x_axis;
    Axis y_axis;

    switch (axis) {
      case Axis2d::l3:
        x_axis = Axis::l3_h;
        y_axis = Axis::l3_v;
        break;
      case Axis2d::r3:
        x_axis = Axis::r3_h;
        y_axis = Axis::r3_v;
        break;
      case Axis2d::dpad:
        x_axis = Axis::dpad_h;
        y_axis = Axis::dpad_v;
        break;
      case Axis2d::touchpad:
        x_axis = Axis::touchpad_h;
        y_axis = Axis::touchpad_v;
        break;
    }

    const AxisCallback handler = [this, cb, x_axis, y_axis](const double){
      State state = getState();

      cb(state.axes[x_axis].value, state.axes[y_axis].value);
    };

    setAxisCallback(x_axis, handler);
    setAxisCallback(y_axis, handler);
  }
}
