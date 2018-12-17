#ifndef _MESH_H_
#define _MESH_H_

#include "Common.h"
#include "Buffer.h"

namespace daybreak {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 texcoord;

        Vertex();

        static VkVertexInputBindingDescription get_binding();
        static std::array<VkVertexInputAttributeDescription, 4> get_attributes();

        bool operator==(const Vertex& other) const;
    };

    class Mesh {
    private:
        Buffer m_vertices, m_indices;
        uint32_t m_count;
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

        void render(VkCommandBuffer cmd);
    };
}

// FIND A PLACE TO MOVE THIS

template <class T> inline void hash_combine(std::size_t& s, const T& v) {
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

namespace std {
    template<> struct hash<daybreak::Vertex> {
        size_t operator()(daybreak::Vertex const& vertex) const {
            size_t hsh = 0;
            hash_combine(hsh, vertex.normal);
            hash_combine(hsh, vertex.color);
            hash_combine(hsh, vertex.texcoord);
            hash_combine(hsh, vertex.position);
            return hsh;
        }
    };
}

#endif
