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

#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "Common.h"
#include "Shader.h"
#include "Mesh.h"

namespace daybreak {

    class AssetManager {
        NO_COPY(AssetManager)
    private:
        std::map<std::string, Shader*> m_shaders;
        std::map<std::string, Mesh*> m_meshes;
    public:
        AssetManager();

        Shader* load_fragment_shader(std::string name, std::string file);
        Shader* load_vertex_shader(std::string name, std::string file);
        Shader* get_shader(std::string name);
        void release_shader(std::string name);

        Mesh* load_mesh(std::string name, std::string file);
        Mesh* get_mesh(std::string name);
        void release_mesh(std::string name);

        void release_all_shaders();
        void release_all_meshes();
        void release_all_assets();
    };
}

#endif
