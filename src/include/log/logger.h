#ifndef ICEE_INCLUDE_LOG_LOGGER_H
#define ICEE_INCLUDE_LOG_LOGGER_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "core/utils.h"

namespace Icee {

using std::vector;

enum class LogLevel { INFO, WARNING, ERROR, FATAL, DEBUG };

const std::string FILE_PATH{"IceeLog"};

// 阈值：分别是 count threshold & flush duration threshold
constexpr int COUNT_THRESHOLD = 1000;
constexpr std::chrono::duration TIME_THRESHOLD = std::chrono::microseconds(3000);  // 三秒钟强制进行一次刷盘

class Logger {
 public:
  NON_COPYABLE_AND_MOVEABLE(Logger);

  static void DoLog(LogLevel log_level, const std::string &) noexcept;

  static auto GetInstance() noexcept -> Logger &;

  struct Log {
    // each log entry initialize to a message with the timestamp
    std::string stamped_message_;

    explicit Log(LogLevel log_level, const std::string &message) noexcept;

    friend auto operator<<(std::ostream &os, const Log &log) -> std::ostream & {
      os << log.stamped_message_;
      return os;
    }
  };

 private:
  // 私有的构造 Logger, 只能是通过 Singleton mode function 来进行创造, 写到输入缓冲区或者是写到file
  explicit Logger(const std::function<void(const vector<Log> &logs)> &log_strategy);

  void PushNewLog(Log &&log);

  void LogWrite() noexcept;

  ~Logger();

  std::atomic<bool> done_{false};
  std::mutex latch_;
  std::condition_variable cond_;
  vector<Log> front_log_entries_;  // 前端的 log entries, 达到一定的阈值之后会唤醒后台的刷盘线程去执行 swap
  std::chrono::microseconds last_flush_;                   // the timestamp of the last flush
  std::function<void(const vector<Log> &)> log_strategy_;  // 写日志的策略是什么(输出到输出缓冲区Or文件)
  std::thread background_flush_thread_;
};

#ifdef NOLOG  // if set the nolog macro define, just empty implement
#define LOG_INFO(x) {};
#define LOG_DEBUG(x) {};
#define LOG_WARNING(x) {};
#define LOG_FATAL(x) {};
#define LOG_ERROR(x) {};
#else
#define LOG_INFO(x) Icee::Logger::DoLog(Icee::LogLevel::INFO, (x));
#define LOG_ERROR(x) Icee::Logger::DoLog(Icee::LogLevel::ERROR, (x));
#define LOG_WARNING(x) Icee::Logger::DoLog(Icee::LogLevel::WARNING, (x));
#define LOG_FATAL(x) Icee::Logger::DoLog(Icee::LogLevel::FATAL, (x));
#define LOG_DEBUG(x) Icee::Logger::DoLog(Icee::LogLevel::DEBUG, (x));
#endif

}  // namespace Icee
#endif
