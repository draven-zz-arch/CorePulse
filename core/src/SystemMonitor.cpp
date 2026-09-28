#include "SystemMonitor.hpp"
#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

SystemMonitor::SystemMonitor() {}

SystemMonitor::~SystemMonitor() {
    stop();
}

void SystemMonitor::start() {
    if (m_running) return;
    m_running = true;
    m_worker_thread = std::thread(&SystemMonitor::worker_loop, this);
}

void SystemMonitor::stop() {
    if (!m_running) return;
    m_running = false;
    if (m_worker_thread.joinable()) {
        m_worker_thread.join(); // Îți amintești? Așteptăm thread-ul secundar
    }
}

SystemStats SystemMonitor::get_latest_stats() {
    std::lock_guard<std::mutex> lock(m_data_mutex); // RAII Lock standard
    return m_latest_stats;
}

#ifdef _WIN32
static ULONGLONG FileTimeToULL(const FILETIME& ft) {
    return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}
#endif

void SystemMonitor::worker_loop() {
#ifdef _WIN32
    FILETIME idleTime, kernelTime, userTime;
    FILETIME prevIdleTime, prevKernelTime, prevUserTime;
    GetSystemTimes(&prevIdleTime, &prevKernelTime, &prevUserTime);
#endif

    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        double cpu_load = 0.0;

#ifdef _WIN32
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
            ULONGLONG idle = FileTimeToULL(idleTime) - FileTimeToULL(prevIdleTime);
            ULONGLONG kernel = FileTimeToULL(kernelTime) - FileTimeToULL(prevKernelTime);
            ULONGLONG user = FileTimeToULL(userTime) - FileTimeToULL(prevUserTime);
            ULONGLONG system = kernel + user;

            if (system > 0) {
                cpu_load = static_cast<double>(system - idle) * 100.0 / system;
            }
            prevIdleTime = idleTime; prevKernelTime = kernelTime; prevUserTime = userTime;
        }
#else
        cpu_load = 15.0; // Placeholder pentru când vom rula pe Linux/WSL
#endif

        {
            std::lock_guard<std::mutex> lock(m_data_mutex);
            m_latest_stats.cpu_usage = cpu_load;
            m_latest_stats.ram_usage = 45.2;
        }
    }
}
