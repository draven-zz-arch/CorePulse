#include <iostream>
#include <chrono>
#include <thread>
#include "SystemMonitor.hpp" // Vine direct din librăria noastră 'core'

int main() {
    std::cout << "[MAIN] Pornim aplicatia CorePulse cu structura modulara...\n";
    
    SystemMonitor monitor;
    monitor.start(); // Pornim thread-ul din Core-ul C++

    // Citim datele din thread-ul principal de 5 ori, o dată pe secundă
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        SystemStats stats = monitor.get_latest_stats();
        std::cout << "[UI THREAD] Date reale Windows -> CPU: " << stats.cpu_usage << "% | RAM: " << stats.ram_usage << "%\n";
    }

    std::cout << "[MAIN] Oprirea aplicatiei...\n";
    monitor.stop(); 
    
    return 0;
}
