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

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Common.h"

namespace daybreak {

    class Transform {
    private:
        glm::vec3 m_position;
        glm::vec3 m_scale;
        glm::quat m_rotation;
        glm::mat4 m_transform;

        inline void update();
    public:
        Transform();

        void translate(glm::vec3& translate);
        void scale(glm::vec3& scale);
        void rotate(float_t angle, glm::vec3& axis);

        void set_position(glm::vec3& position);
        void set_scale(glm::vec3& scale);
        void set_rotation(glm::vec3& axis, float_t angle);

        inline void set_transform(const glm::mat4& transform) { m_transform = transform; }

        inline const glm::vec3& get_position() const { return m_position; }

        inline const glm::vec3& get_scale() const { return m_scale; }

        inline const glm::mat4& get_transform() const { return m_transform; }
    };
}

#endif
