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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Pipeline.h"
#include "DescriptorSet.h"

#include "Mesh.h"

namespace daybreak {

    class Scene {
    private:
        Pipeline m_pipeline;
        DescriptorSet m_set;

        std::vector<Mesh*> m_meshes;
    public:
        Scene(std::vector<Shader*> shaders, std::vector<Binding> bindings);
        ~Scene();

        void update(double_t delta);
        void render();

        inline bool update_uniform(std::string name, const void* data, size_t size) {
            return m_set.set_value(name, data, size);
        }

        void add_mesh(Mesh* mesh);
    };
}

#endif