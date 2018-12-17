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
