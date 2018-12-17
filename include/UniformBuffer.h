#ifndef _UNIFORM_BUFFER_H_
#define _UNIFORM_BUFFER_H_

#include "Common.h"

namespace daybreak {
    class UniformBuffer {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
}

#endif
