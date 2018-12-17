#include "../../include/Log.h"

namespace daybreak {

    inline char* Log::get_time() {
        static char buffer[26];
        time_t now = system_clock::to_time_t(system_clock::now());
        ctime_s(buffer, 26, &now);
        buffer[24] = 0;
        return buffer;
    }

    void Log::log(const std::string& message) {
        printf("[%s] %s\n", get_time(), message.c_str());
    }

    void Log::warn(const std::string& message) {
        printf("[%s] WARN: %s\n", get_time(), message.c_str());
    }

    void Log::fatal(const std::string& message) {
        printf("[%s] ERROR: %s\n", get_time(), message.c_str());
        exit(EXIT_FAILURE);
    }
}