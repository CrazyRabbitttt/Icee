#ifndef SRC_INCLUDE_TIMER_TIMER_QUEUE_H
#define SRC_INCLUDE_TIMER_TIMER_QUEUE_H

#include <set>
#include "Timer/timer.h"

namespace Icee::Timer {

class EventLooper;

class TimerQueue {
 public:
  using TimerEntry = std::pair<TimePonit, Timer *>;  // sort by the time point(expire time)
  using TimerSet = std::set<TimerEntry>;             // sort by the expire time

  TimerQueue(EventLooper *eventlooper);

 private:
  EventLooper *owner_looper_;
  int timerfd_;
};

}  // namespace Icee::Timer

#endif