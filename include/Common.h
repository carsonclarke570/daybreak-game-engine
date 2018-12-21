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

#ifndef _COMMON_H_
#define _COMMON_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <shaderc/shaderc.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>

#include "Log.h"
#include "Util.h"

// Game loop config
#define FPS_CAP 200
#define TIME_STEP 1000.0f / FPS_CAP

// Debug config
#define API_DEBUG
#define API_LOG
#define FPS_DEBUG

// Window init config
#define WIDTH 800
#define HEIGHT 600
#define TITLE "Daybreak - Vulkan Version"

// Allocation config
#define ALLOCATION_SIZE 10

// Enums


// Macros
#define COUNT_OF(a) (sizeof(a)/sizeof(*(a)))

#define ASSERT(exp, msg) if(!(exp)) Log::fatal(msg);

#define NO_COPY(class)                          \
    class(const class&) = delete;               \
    class& operator=(const class&) = delete;    \

#ifdef API_LOG
#define LOG(msg) Log::log(msg);
#define WARN(msg) Log::warn(msg);
#define FATAL(msg) Log::fatal(msg);
#else
#define LOG(msg) 0;
#define WARN(msg) 0;
#define FATAL(msg) 0;
#endif

#endif
