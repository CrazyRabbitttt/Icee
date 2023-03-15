#ifndef SRC_INCLUDE_TIMER_TIMER_H_
#define SRC_INCLUDE_TIMER_TIMER_H_

#include "core/utils.h"

#include <chrono>
#include <functional>

namespace Icee::Timer {

using TimePonit = std::chrono::time_point<std::chrono::steady_clock>;
using Duration = std::chrono::milliseconds;

class Timer {
 public:

  explicit Timer(std::function<void()> &&callback, TimePonit expire_time, Duration interval);

  NON_COPYABLE(Timer);

  ~Timer() = default;

  void Run();

  auto expireTime() const noexcept -> TimePonit;

  void Restart(TimePonit now);

 private:
  TimePonit expire_time_;
  Duration interval_;                // 周期性的重复时间
  std::function<void()> call_back_;  // time out call_back function
  bool repeat_{false};
};

}  // namespace Icee::Timer

#endif