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

#include "../../include/AssetManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../dependencies/tinyobjloader/tiny_obj_loader.h"

namespace daybreak {

    AssetManager::AssetManager() :
        m_shader_pool(PoolAllocator(ALLOCATION_SIZE * sizeof(Shader), sizeof(Shader))),
        m_mesh_pool(PoolAllocator(ALLOCATION_SIZE * sizeof(Mesh), sizeof(Mesh))),
        m_shaders(std::map<std::string, Shader*>()), m_meshes(std::map<std::string, Mesh*>()) {
    }

    Shader* AssetManager::load_fragment_shader(std::string name, std::string file) {
        ASSERT((m_shaders[name] == nullptr), "Assets - Fragment shader " + name + " already exists");

        auto shader = (Shader*) m_shader_pool.allocate();
        shader = new (shader) Shader(Util::read_file(file), VK_SHADER_STAGE_FRAGMENT_BIT);
        m_shaders[name] = shader;

        LOG("Assets - Created new fragment shader " + name);
        return shader;
    }

    Shader* AssetManager::load_vertex_shader(std::string name, std::string file) {
        ASSERT((m_shaders[name] == nullptr), "Assets - Vertex shader " + name + " already exists");

        auto shader = (Shader*) m_shader_pool.allocate();
        shader = new (shader) Shader(Util::read_file(file), VK_SHADER_STAGE_VERTEX_BIT);
        m_shaders[name] = shader;

        LOG("Assets - Created new vertex shader " + name);
        return shader;
    }

    Shader* AssetManager::get_shader(std::string name) {
        return m_shaders[name];
    }

    void AssetManager::release_shader(std::string name) {
        m_shader_pool.release(m_shaders[name]);
        m_shaders[name] = nullptr;
        LOG("Assets - Released Shader " + name);
    }

    Mesh* AssetManager::load_mesh(std::string name, std::string file) {
        ASSERT((m_meshes[name] == nullptr), "Assets - Mesh " + name + " already exists");

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str())) {
            if (!warn.empty()) {
                WARN(warn);
            } else if (!err.empty()) {
                FATAL(err);
            }
        }

        std::unordered_map<Vertex, uint32_t> unique_vertices = {};
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};

                vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                if (!attrib.texcoords.empty()) {
                    vertex.texcoord = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if (!attrib.colors.empty()) {
                    vertex.color = {
                            attrib.colors[3 * index.vertex_index + 0],
                            attrib.colors[3 * index.vertex_index + 1],
                            attrib.colors[3 * index.vertex_index + 2]
                    };
                }

                if (!attrib.normals.empty()) {
                    vertex.normal = {
                            attrib.normals[3 * index.vertex_index + 0],
                            attrib.normals[3 * index.vertex_index + 1],
                            attrib.normals[3 * index.vertex_index + 2]
                    };
                }

                if (unique_vertices.count(vertex) == 0) {
                    unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(unique_vertices[vertex]);
            }
        }

        auto mesh = (Mesh*) m_mesh_pool.allocate();
        mesh = new (mesh) Mesh(vertices, indices);
        m_meshes[name] = mesh;

        LOG("Assets - Created new Mesh " + name);
        return mesh;
    }

    Mesh* AssetManager::get_mesh(std::string name) {
        return m_meshes[name];
    }

    void AssetManager::release_mesh(std::string name) {
        m_mesh_pool.release(m_meshes[name]);
        m_meshes[name] = nullptr;
        LOG("Assets - Released Mesh " + name);
    }

    void AssetManager::release_all_shaders() {
        LOG("Assets - Released all shaders");
        m_shaders.clear();
        m_shader_pool.reset();
    }

    void AssetManager::release_all_meshes() {
        LOG("Assets - Released all meshes");
        m_meshes.clear();
        m_mesh_pool.reset();
    }

    void AssetManager::release_all_assets() {
        release_all_shaders();
        release_all_meshes();
    }
}