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

#include "../../include/Transform.h"

namespace daybreak {

    Transform::Transform() :
            m_position({0.0f, 0.0f, 0.0f}),
            m_scale({1.0f, 1.0f, 1.0f}),
            m_rotation(),
            m_transform(1.0f) {
    }

    void Transform::update() {
        glm::mat4 rotation = glm::toMat4(m_rotation);
        m_transform = glm::scale(glm::translate(rotation, m_position), m_scale);
    }

    void Transform::translate(glm::vec3& translate) {
        m_transform = glm::translate(m_transform, translate);
    }

    void Transform::scale(glm::vec3& scale) {
        m_transform = glm::scale(m_transform, scale);
    }

    void Transform::rotate(float_t angle, glm::vec3& axis) {
        m_transform = glm::rotate(m_transform, angle, axis);
    }

    void Transform::set_position(glm::vec3& position) {
        m_position = position;
        update();
    }

    void Transform::set_scale(glm::vec3& scale) {
        m_scale = scale;
        update();
    }

    void Transform::set_rotation(glm::vec3& axis, float_t angle) {
        m_rotation = glm::angleAxis(angle, axis);
        update();
    }
}