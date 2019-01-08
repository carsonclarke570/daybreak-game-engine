#include "include/Game.h"
#include "include/Daybreak.h"
#include "include/Scene.h"
#include "include/AssetManager.h"
#include "include/Timer.h"
#include "include/UniformBufferObject.h"

using namespace daybreak;

class TestApplication : public Game {
public:
    AssetManager assets;
    Scene* m_test_scene;
    GameObject* wolf;

    TestApplication() : wolf(nullptr), m_test_scene(nullptr) {
    }

    void init() override {

        // Load Assets
        assets.load_fragment_shader("frag", "../resources/shaders/frag.spv");
        assets.load_vertex_shader("vert", "../resources/shaders/vert.spv");
        assets.load_mesh("wolf", "../resources/models/cube.obj");

        std::vector<Shader*> shaders = {
                assets.get_shader("frag"),
                assets.get_shader("vert"),
        };

        std::vector<Binding> bindings = {
                {"m_mat",  sizeof(MMat),  0, VK_SHADER_STAGE_VERTEX_BIT},
                {"vp_mat", sizeof(VPMat), 1, VK_SHADER_STAGE_VERTEX_BIT}
        };

        auto pipeline = new Pipeline(shaders, bindings);
        auto set = new DescriptorSet(pipeline);
        auto renderer = new Renderer(pipeline, set);

        m_test_scene = new Scene(renderer);

        wolf = new GameObject(assets.get_mesh("wolf"));
        auto test = new GameObject();

        m_test_scene->add_object(wolf);
        m_test_scene->add_object(test);
    }

    void update(double_t delta) override {
        glm::vec3 up = {0.0f, 1.0f, 0.0f};
        wolf->set_rotation((float_t) glm::radians(delta / 10.0f), up);
        m_test_scene->update(delta);
    }

    void render() override {
        m_test_scene->render();
    }

    void end() override {
        assets.release_all_assets();
    }

    bool quit() override {
        return false;
    }
};

#include <Windows.h>

int main() {
    //Sleep(30000);

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