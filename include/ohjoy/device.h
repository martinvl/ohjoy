#ifndef OHJOY_DEVICE_H
#define OHJOY_DEVICE_H

#include <fstream>
#include <functional>
#include <future>
#include <map>
#include <mutex>
#include <ostream>
#include <string>

namespace ohjoy
{
  struct Event;

  struct AxisState
  {
    double value;
  };

  struct ButtonState
  {
    bool is_pressed;
  };

  using ButtonCallback = std::function<void(const bool is_pressed)>;
  using AxisCallback = std::function<void(const double value)>;

  class Device
  {
    public:
      struct State
      {
        std::map<int, AxisState> axes;
        std::map<int, ButtonState> buttons;
      };

      Device();
      Device(const int dev_idx);
      Device(const std::string& dev_path);

      Device(Device&&) = default;
      Device(Device const&) = delete;

      virtual ~Device();

      void start();
      void stop();

      State getState();
      void setButtonCallback(const int button_id, const ButtonCallback& cb);
      void setAxisCallback(const int axis_id, const AxisCallback& cb);

    private:
      std::ifstream in_;
      std::future<void> worker_;
      bool is_running_ = false;
      std::map<int, ButtonCallback> button_callbacks_;
      std::map<int, AxisCallback> axis_callbacks_;

      std::mutex state_mutex_;
      State state_;

      Device(std::ifstream&& in);

      void init();
      void workerLoop();
      bool attemptRead(Event& event);
      Event read();
      void handleEvent(const Event& event);
      void handleCallback(const Event& event);

      static std::string getDevicePath(const int dev_num);
      static std::ifstream getDeviceStream(const std::string& dev_path);
  };
}

#endif // OHJOY_DEVICE_H
