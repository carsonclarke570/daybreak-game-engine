#ifndef _LOG_H_
#define _LOG_H_


#include <string>
#include <ctime>
#include <chrono>

namespace daybreak {

    using namespace std::chrono;

    class Log {
    private:
        inline static char* get_time();
    public:
        static void log(const std::string& message);
        static void warn(const std::string& message);
        static void fatal(const std::string& message);
    };
}

#endif
