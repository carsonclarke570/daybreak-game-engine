#include "include/Game.h"
#include "include/Daybreak.h"
#include "include/Pipeline.h"
#include "include/AssetManager.h"
#include "include/DescriptorSet.h"
#include "include/Timer.h"

using namespace daybreak;

class TestApplication : public Game {
public:
    AssetManager assets;
    Timer timer;

    Pipeline* pipeline;
    DescriptorSet* descriptor_set;

    Mesh* mesh;

    double_t elapsed;

    struct UBO {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    TestApplication() : mesh(nullptr), pipeline(nullptr), descriptor_set(nullptr) {
        elapsed = 0;
    }

    void init() override {
        assets.load_fragment_shader("frag", "../resources/shaders/frag.spv");
        assets.load_vertex_shader("vert", "../resources/shaders/vert.spv");
        assets.load_mesh("wolf", "../resources/models/Wolf.obj");

        std::vector<Shader*> shaders = {
                assets.get_shader("frag"),
                assets.get_shader("vert"),
        };

        std::vector<Binding> bindings = {
                {"transform", sizeof(UBO), 0, VK_SHADER_STAGE_VERTEX_BIT}
        };

        pipeline = new Pipeline(shaders, bindings);
        descriptor_set = new DescriptorSet(*pipeline);

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
        timer.reset();
    }

    void update(double_t delta) override {
        elapsed += delta;

        UBO ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), (float) (elapsed / 1000.0f) * glm::radians(90.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), API::viewport().width / API::viewport().height, 0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;

        descriptor_set->set_value("transform", &ubo, sizeof(ubo));
    }

    void render() override {
        VkCommandBuffer cmd = API::begin_render_command_buffer();
        API::begin_present_pass();

        // Bind current pipeline && description set
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline());
        VkDescriptorSet set = descriptor_set->set();
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout(), 0, 1, &set, 0, nullptr);

        { // Resize viewport and scissor
            VkViewport viewport = API::viewport();
            VkRect2D scissor = API::scissor();
            vkCmdSetScissor(cmd, 0, 1, &scissor);
            vkCmdSetViewport(cmd, 0, 1, &viewport);
        }

        // Render mesh
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
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}