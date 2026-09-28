#pragma once
#include <thread>
#include <mutex>
#include <atomic>

struct SystemStats {
    double cpu_usage;
    double ram_usage;
};

class SystemMonitor {
public:
    SystemMonitor();
    ~SystemMonitor();

    void start();
    void stop();
    SystemStats get_latest_stats();

private:
    void worker_loop();
    std::thread m_worker_thread;
    std::atomic<bool> m_running{false};
    std::mutex m_data_mutex;
    SystemStats m_latest_stats{0.0, 0.0};
};
