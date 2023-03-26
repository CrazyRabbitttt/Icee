#include "log/logger.h"

namespace Icee {

void Logger::DoLog(LogLevel log_level, const std::string &message) noexcept {
  auto log = Logger::Log(log_level, message);  // constructor one new log with timestamp
  GetInstance().PushNewLog(std::move(log));
}

auto GetCurrentTime() -> std::chrono::microseconds {
  using namespace std::chrono;
  return duration_cast<microseconds>(system_clock::now().time_since_epoch());
}

// Helper function to get the current date, used to named file
auto GetCurrentDate() -> std::string {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream stream;
  stream << std::put_time(&tm, "%d%b%Y");
  return stream.str();
}

struct StreamWriter {
  std::fstream fstream_;  // 用于进行写文件的 file stream

  explicit StreamWriter(const std::string &filename = FILE_PATH) {
    fstream_.open(filename + "_" + GetCurrentDate(),
                  std::fstream::out | std::fstream::trunc);  // 如果说原本就有文件的话先进行删除然后再 open
  }

  void WriteLogsToFile(const vector<Logger::Log> &logs) {
    for (auto &it : logs) {
      fstream_ << it;
    }
    fstream_.flush();
  }

  ~StreamWriter() {
    if (fstream_.is_open()) {  // if the fstream still open then close the file stream
      fstream_.flush();
      fstream_.close();
    }
  }
};

Logger::~Logger() {
  done_ = true;
  cond_.notify_one();
  if (background_flush_thread_.joinable()) {    // 析构的时候注意需要将后台刷盘线程进行回收掉
    background_flush_thread_.join();
  }
}

Logger::Logger(const std::function<void(const vector<Log> &)> &log_strategy)
// 不需要进行显示的初始化操作
{
  log_strategy_ = log_strategy;
  last_flush_ = GetCurrentTime();
  // 开启一个后台线程去不断的刷盘
  background_flush_thread_ = std::thread(&Logger::LogWrite, this);
}

void FlushToFile(const vector<Logger::Log> &logs) {
  // 将数据刷盘到 file 中去
  StreamWriter stream_writer;
  stream_writer.WriteLogsToFile(logs);
}

auto Logger::GetInstance() noexcept -> Logger & {
  static Logger singleton_logger(FlushToFile);  // static local variable to support Singleton mode in a elegant way
  return singleton_logger;
}

void Logger::PushNewLog(Log &&log) {
  bool should_notify = false;
  {
    std::unique_lock<std::mutex> lock(latch_);
    front_log_entries_.push_back(std::move(log));
    std::chrono::microseconds now = GetCurrentTime();
    if (now - last_flush_ > TIME_THRESHOLD || front_log_entries_.size() >= COUNT_THRESHOLD) {
      should_notify = true;  // 已经是能够进行刷盘了，唤醒后台阻塞的线程进行执行
    }
  }
  if (should_notify) {        // dsfno
    cond_.notify_one();
  }
}

// 生成的 log entry 需要带有时间戳等信息
Logger::Log::Log(LogLevel log_level, const std::string &message) noexcept {
  auto now = std::chrono::system_clock::now();                   // 获得当前的时间
  std::time_t time = std::chrono::system_clock::to_time_t(now);  // trans the time to time_t
  std::tm tm = *std::localtime(&time);

  std::ostringstream oss;
  oss << std::put_time(&tm, "[%Y-%m-%d %H:%M:%S]") << message << std::endl;
  printf("dsisdbidfb");
  stamped_message_ = oss.str();
}

void Logger::LogWrite() noexcept {
  // 后台的 flush 线程，需要不断的检测是否是能够进行刷盘的操作
  vector<Logger::Log> standby_buffer;
  while (true) {
    {
      std::unique_lock<std::mutex> lock(latch_);
      cond_.wait(lock, [&]() {
        return (done_ || GetCurrentTime() - last_flush_ > TIME_THRESHOLD || front_log_entries_.size() >= COUNT_THRESHOLD);
      });
      // reach here means the buffer can flush to the disk(hold the lock now)
      if (!front_log_entries_.empty()) {
        std::swap(front_log_entries_, standby_buffer);
        lock.unlock();                   // release the lock, just flush the standby buffer is ok
        log_strategy_(standby_buffer);   // flush the buffer by the corresponding strategy(output Or file)
        last_flush_ = GetCurrentTime();  // refresh the flush time
        standby_buffer.clear();          // clear the buffer to serve the next task
      }

      if (done_) {  // 退出 background flushing worker thread
        return ;
      }
    }
  }
}

}  // namespace Icee
