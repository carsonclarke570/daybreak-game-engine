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

#include "../../include/Timer.h"

namespace daybreak {

    Timer::Timer() : m_clock(std::chrono::high_resolution_clock::now()) {}

    void Timer::reset() {
        m_clock = std::chrono::high_resolution_clock::now();
    }

    float_t Timer::elapsed_seconds() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float_t, std::chrono::seconds::period>(current_time - m_clock).count();
    }

    double_t Timer::elapsed_milliseconds() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double_t, std::chrono::milliseconds::period>(current_time - m_clock).count();
    }

    double_t Timer::elapsed_microseconds() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double_t, std::chrono::microseconds::period>(current_time - m_clock).count();
    }

    double_t Timer::elapsed_nanoseconds() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double_t, std::chrono::nanoseconds::period>(current_time - m_clock).count();
    }
}

