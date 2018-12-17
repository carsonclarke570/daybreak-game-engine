/**
 * Copyright 2018 Carson Clarke-Magrab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
**/

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