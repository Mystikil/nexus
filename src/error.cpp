#include "otpch.h"
#include "error.h"
#include <fstream>
#include <mutex>

namespace ErrorLog {
namespace {
    std::mutex logMutex;
    const char* logFile = "data/logs/errors.txt";
}

void log(std::string_view msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ofstream out(logFile, std::ios::app);
    if (out.is_open()) {
        out << msg << '\n';
    }
}

} // namespace ErrorLog
