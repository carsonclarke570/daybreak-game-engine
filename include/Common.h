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

// Debug config
#define API_DEBUG
#define API_LOG

// Window init config
#define WIDTH 800
#define HEIGHT 600
#define TITLE "Daybreak - Vulkan Version"

// Enums


// Macros
#define COUNT_OF(a) (sizeof(a)/sizeof(*a))

#define ASSERT(exp, msg) if(!exp) Log::fatal(msg);

#define NO_COPY(class)                          \
    class(const class&) = delete;               \
    class& operator=(const class&) = delete;    \

#ifdef API_LOG
#define LOG(msg) Log::log(msg);
#define WARN(msg) Log::warn(msg);
#define FATAL(msg) Log::fatal(msg);
#endif

#endif
