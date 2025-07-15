#include "otpch.h"
#include "error.h"
#include <fstream>
#include <filesystem>
#include <mutex>
#include <memory>

namespace ErrorLog {
namespace {

    std::mutex logMutex;
    const char* logFile = "data/logs/errors.txt";
    const char* consoleFile = "data/logs/server.log";

    std::ofstream consoleOut;
    std::streambuf* oldCout = nullptr;
    std::streambuf* oldCerr = nullptr;
    std::streambuf* oldClog = nullptr;

    class TeeBuf : public std::streambuf {
        std::streambuf* buf1;
        std::streambuf* buf2;

    public:
        TeeBuf(std::streambuf* b1, std::streambuf* b2) : buf1(b1), buf2(b2) {}

    protected:
        int overflow(int c) override {
            if (c == EOF) {
                return !EOF;
            }
            int const r1 = buf1->sputc(c);
            int const r2 = buf2->sputc(c);
            return (r1 == EOF || r2 == EOF) ? EOF : c;
        }

        int sync() override {
            int const r1 = buf1->pubsync();
            int const r2 = buf2->pubsync();
            return (r1 == 0 && r2 == 0) ? 0 : -1;
        }
    };

    std::unique_ptr<TeeBuf> teeBuf;

} // anonymous namespace

void setup() {
    std::lock_guard<std::mutex> lock(logMutex);

    if (consoleOut.is_open()) {
        return;
    }

    std::filesystem::create_directories("data/logs");
    consoleOut.open(consoleFile, std::ios::app);

    if (consoleOut.is_open()) {
        teeBuf = std::make_unique<TeeBuf>(std::cout.rdbuf(), consoleOut.rdbuf());
        oldCout = std::cout.rdbuf(teeBuf.get());
        oldCerr = std::cerr.rdbuf(teeBuf.get());
        oldClog = std::clog.rdbuf(teeBuf.get());
    }
}

void log(std::string_view msg) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream out(logFile, std::ios::app);
    if (out.is_open()) {
        out << msg << '\n';
    }

    if (consoleOut.is_open()) {
        consoleOut << msg << '\n';
    }
}

} // namespace ErrorLog
