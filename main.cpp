#include "include/Game.h"
#include "include/Daybreak.h"
#include "include/Pipeline.h"
#include "include/AssetManager.h"

using namespace daybreak;

class TestApplication : public Game {

    AssetManager assets;
    Pipeline* pipeline;
    Mesh* mesh;

    TestApplication() : mesh(nullptr), pipeline(nullptr) {

    }

    void init() override {
        assets.load_fragment_shader("frag", "../resources/shaders/frag.spv");
        assets.load_vertex_shader("vert", "../resources/shaders/vert.spv");
        assets.load_mesh("wolf", "../resources/models/Wolf.obj");

        std::vector<Shader*> shaders = {
                assets.get_shader("frag"),
                assets.get_shader("vert"),
        };
        pipeline = new Pipeline(shaders);

//        std::vector<Vertex> vertices = {
//                {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//                {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//                {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
//                {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
//        };
//
//        std::vector<uint32_t> indices = {
//                0, 1, 2, 2, 3, 0
//        };
//
//        mesh = new Mesh(vertices, indices);
        mesh = assets.get_mesh("wolf");
    }

    void update() override {

    }

    void render() override {
        VkCommandBuffer cmd = API::begin_render_command_buffer();
        API::begin_present_pass();

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());

        { // Resize viewport and scissor
            VkViewport viewport = API::viewport();
            VkRect2D scissor = API::scissor();
            vkCmdSetScissor(cmd, 0, 1, &scissor);
            vkCmdSetViewport(cmd, 0, 1, &viewport);
        }

        mesh->render(cmd);

        API::end_present_pass();
        API::end_render_command_buffer();
    }

    void end() override {
        assets.release_all_assets();
    }

    bool quit() override {
        return false;
    }
};

int main() {
    TestApplication app;
    Daybreak::load(&app);

    try {
        Daybreak::start();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}