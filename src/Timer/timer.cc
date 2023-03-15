#include "Timer/timer.h"

namespace Icee::Timer {

Timer::Timer(std::function<void()> &&callback, TimePonit expire_time, Duration interval)
    : call_back_(std::move(callback)), expire_time_(expire_time), interval_(interval) {}

auto Timer::expireTime() const noexcept -> TimePonit { return expire_time_; }

void Timer::Run() { call_back_(); }

void Timer::Restart(TimePonit now) { expire_time_ = std::chrono::steady_clock::now() + interval_; }




}  // namespace Icee::Timer
