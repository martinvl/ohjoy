#include "ohjoy/device.h"

#include "ohjoy/event.h"

#include <sstream>

namespace ohjoy
{
  Device::Device(std::ifstream&& in)
    : in_{std::move(in)}
  {
    init();
    start();
  }

  Device::Device(const std::string& dev_path)
    : Device{getDeviceStream(dev_path)}
  {}

  Device::Device(const int dev_num)
    : Device{getDevicePath(dev_num)}
  {}

  Device::Device()
    : Device{0}
  {}

  Device::~Device()
  {
    stop();
  }

  void Device::start()
  {
    if (is_running_) {
      return;
    }

    is_running_ = true;

    worker_ = std::async(std::launch::async, [this]()
    {
      workerLoop();
    });
  }

  void Device::stop()
  {
    if (!is_running_) {
      return;
    }

    is_running_ = false;
    worker_.wait();
  }

  Device::State Device::getState()
  {
    std::lock_guard<std::mutex> lock{state_mutex_};
    return state_;
  }

  void Device::setButtonCallback(const int button_id, const ButtonCallback& cb)
  {
    button_callbacks_[button_id] = cb;
  }

  void Device::setAxisCallback(const int axis_id, const AxisCallback& cb)
  {
    axis_callbacks_[axis_id] = cb;
  }

  void Device::init()
  {
    handleEvent(read());

    for (Event event; attemptRead(event);) {
      handleEvent(event);

      if (!event.isInit()) {
        break;
      }
    }
  }

  void Device::workerLoop()
  {
    while (is_running_) {
      const Event event = read();

      handleEvent(event);
      handleCallback(event);
    }
  }

  bool Device::attemptRead(Event& event)
  {
    const size_t bytes_read = in_.readsome(
      reinterpret_cast<char*>(&event),
      sizeof(event)
    );

    return bytes_read == sizeof(event);
  }

  Event Device::read()
  {
    Event event;

    in_.read(
      reinterpret_cast<char*>(&event),
      sizeof(event)
    );

    return event;
  }

  void Device::handleEvent(const Event& event)
  {
    {
      std::lock_guard<std::mutex> lock{state_mutex_};

      if (event.isAxis()) {
        state_.axes[event.id].value = event.getValue();
      } else if (event.isButton()) {
        state_.buttons[event.id].is_pressed = event.isPressed();
      }
    }
  }

  void Device::handleCallback(const Event& event)
  {
    if (event.isAxis()) {
      if (axis_callbacks_[event.id] != nullptr) {
        axis_callbacks_[event.id](event.getValue());
      }
    } else if (event.isButton()) {
      if (button_callbacks_[event.id] != nullptr) {
        button_callbacks_[event.id](event.isPressed());
      }
    }
  }

  std::string Device::getDevicePath(const int dev_num)
  {
    std::ostringstream ss;
    ss << "/dev/input/js" << dev_num;

    return ss.str();
  }

  std::ifstream Device::getDeviceStream(const std::string& dev_path)
  {
    std::ifstream stream{dev_path, std::ifstream::in};

    if (!stream) {
      throw std::runtime_error("Failed finding device " + dev_path);
    }

    return stream;
  }
}
